// ---------------------------------------------------------------------------
#pragma hdrstop

#include "MembershipEBet.h"
#include <set>
#include <Memory>
#include <Sysutils.hpp>
#include "MMLogging.h"
#include <DateUtils.hpp>
#include "Rounding.h"
#include "MMContactInfo.h"
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// -----------------------TContactMemberERS--------------------------------------
// ---------------------------------------------------------------------------

TMembershipEBet::TMembershipEBet(TModules &Modules, AnsiString inIP, int inPort, int inTillID) : TMembership(Modules),
MembershipEBetTCP(new TMembershipEBetTCP())
{
   ContactType = eERSMember;
   ContactTabType = TabMember;
   ReadOnlyInterface = true;
   Modules.Status[eRegMembers]["ReadOnly"] = ReadOnlyInterface;
   Enabled = Modules.Status[eRegMembers]["Enabled"];
   IP = inIP;
   Port = inPort;
   TillID = inTillID;
}

TMembershipEBet::~TMembershipEBet()
{
}

void TMembershipEBet::Initialise(Database::TDBTransaction &DBTransaction)
{
   TMembership::Initialise(DBTransaction);
   MembershipEBetTCP->Init(IP, Port, 20000);
}

UnicodeString TMembershipEBet::ErrorMessage()
{
   UnicodeString RetVal = "";
   switch(MembershipEBetTCP->InPacket.Status())
   {
   case eEbetOk:
	  RetVal = " Status Ok : ";
	  break;
   case eEbetResend:
	  RetVal = " Status Retry : ";
	  break;
   case eEbetAbort:
	  RetVal = " Status Abort : ";
	  break;
   }
   return RetVal + MembershipEBetTCP->InPacket.Msg();
}

void TMembershipEBet::LoyaltyAddValue(Database::TDBTransaction &DBTransaction, TPointsTransaction &PointsTransaction)
{
   if (!fEnabled)
	  return;
   if (DemoMode)
	  return;

   try
   {

	  /* POINTS_TYPE INTEGER,

	  0)	Normal earned points.
	  1)	VIP Purchased Points.
	  2)	Reward Points. */

	  /* ADJUSTMENT_TYPE
	  0)	Manual Adjustment from Membership.
	  1)	Stock Master Adjustment from Membership.
	  2)	Payment Adjustment from Membership.
	  3)	Manual Adjustment from MenuMate.
	  4)	Adjustment from MenuMate.
	  5)	Birthday Adjustment from MenuMate.
	   */
      TMMContactInfo Info;
      Info.MembershipNumber = GetContactNumber(DBTransaction, PointsTransaction.ContactKey);
      GetExternalContactDetails(DBTransaction, Info);
      if (Info.ExternalKey != 0)
      {
         // We round the Adjument to be sent into our DB and to EBet to cents as Ebet can only handle cents.
         // This will stop rounding isses between the two systems.
//      	 PointsTransaction.Adjustment = Currency(CentsToInt(PointsTransaction.Adjustment,false)) / Currency(100);
//      	 PointsTransaction.Adjustment = Currency(CentsToInt(PointsTransaction.Adjustment,false));
         __int64 PointsTransactionAdjustmentCents = RoundToNearest(PointsTransaction.Adjustment, 0.01, false).Val / 100;
      	 PointsTransaction.Adjustment.Val = (PointsTransactionAdjustmentCents * 10000);
         switch(PointsTransaction.PointsTransactionType)
         {
         case pttEarned:
         case pttPurchased:
            if (MembershipEBetTCP->RequestAddPoints(TillID, Info.MembershipNumber, PointsTransactionAdjustmentCents))
            {
               TMembership::LoyaltyAddValue(DBTransaction, PointsTransaction);
               Info.Points.Clear();

               TPointsTypePair typepair(pttEarned,ptstLoyalty);
               TPointsType type(pasDatabase, typepair,pesExported);
               Info.Points.Load(type,MembershipEBetTCP->InPacket.MemberPoints());

               SyncPointsDetials(DBTransaction, Info);
            }
            else
            {
               throw Exception(ErrorMessage());
            }
            break;
         case pttRedeemed:
            if (MembershipEBetTCP->RequestRedeemedPoints(TillID, Info.MembershipNumber, PointsTransactionAdjustmentCents))
            {
               TMembership::LoyaltyAddValue(DBTransaction, PointsTransaction);
               Info.Points.Clear();
               TPointsTypePair typepair(pttEarned,ptstLoyalty);
               TPointsType type(pasDatabase, typepair,pesExported);
               Info.Points.Load(type,MembershipEBetTCP->InPacket.MemberPoints());
               SyncPointsDetials(DBTransaction, Info);
            }
            else
            {
               throw Exception(ErrorMessage());
            }
            break;
         }
      }
      else
      {
         throw Exception("Unable to locate member for transaction \rSwipe card again.\rCheck Ebet Server.\r " + ErrorMessage());
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

Currency TMembershipEBet::LoyaltyGetValue(Database::TDBTransaction &DBTransaction, int InternalKey)
{
   if (!fEnabled)
	  return 0;
   Currency RetVal = 0;
   try
   {
	  int ExternalKey = GetExternalKey(DBTransaction, InternalKey);
	  RetVal = LoyaltyGetExternalValue(DBTransaction, ExternalKey);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

Currency TMembershipEBet::LoyaltyGetExternalValue(Database::TDBTransaction &DBTransaction, int in3rdPartyKey)
{
   if (!fEnabled)
	  return 0;

   Currency RetVal = 0;
   try
   {
	  // TODO: Recover the Member number from the DB.
	  // Query the transaction Layer for the Value.

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

TLoginSuccess TMembershipEBet::TestCardLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess Success = lsUserNotFound;

   try
   {

	  GetExternalContactDetails(DBTransaction, UserInfo);
	  if (UserInfo.ExternalKey != 0)
	  {
		 Success = GetSubsPaid(DBTransaction, UserInfo.ContactKey);
	  }
	  else
	  {
		 Success = lsUserNotFound;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return Success;
}


/* you can only log in via Card this should have already happened beforethis function call.
*/
TLoginSuccess TMembershipEBet::TestKeyLogin(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
   if (!fEnabled)
	  return lsUserNotFound;

   TLoginSuccess Success = lsUserNotFound;
   if (UserInfo.ContactKey != 0)
   {
      Success = lsAccepted;
   }
   else
   {
      Success = lsUserNotFound;
   }
   return Success;
}

TContactType TMembershipEBet::GetContactType(int ContactKey)
{
   if (!fEnabled)
	  return eContactTypeUnknown;
   TContactType RetVal = eEBetMember;
   return RetVal;
}

TLoginSuccess TMembershipEBet::GetSubsPaid(Database::TDBTransaction &DBTransaction, int InternalKey)
{
   return lsAccepted;
}

bool TMembershipEBet::DeleteContact(int inContactKey)
{
   if (!fEnabled)
	  return false;
   bool RetVal = false;
   return RetVal;
}

void TMembershipEBet::GetContactDetails(Database::TDBTransaction &DBTransaction, int InternalKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;
   try
   {
	  Info.ExternalKey = GetExternalKey(DBTransaction, InternalKey);
	  GetExternalContactDetails(DBTransaction, Info);
	  TMembership::GetContactDetails(DBTransaction, Info.ContactKey, Info);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

/**

 */
void TMembershipEBet::SetContactDetails(Database::TDBTransaction &DBTransaction, int InternalKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;
   try
   {
	  TContact::SetContactDetails(DBTransaction, InternalKey, Info);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TMembershipEBet::GetDiscountDetails(Database::TDBTransaction &DBTransaction, int inInternalKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;

   try
   {
	  /* TODO: Inorder to Support discounts the AutoAppliedDiscounts list will need to hold discounts
	  not keys. Then we can create a discount here and pass it back. */

	  /* Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery,DBIndex);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
	  " SELECT DISCOUNT_KEY"
	  " FROM "
	  "  MEMBERS_DISCOUNTS"
	  " WHERE"
	  " MEMBER = :MEMBER ORDER BY MEMBER_DISCOUNTS_KEY";
	  IBInternalQuery->ParamByName("MEMBER")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();
	  if(IBInternalQuery->RecordCount)
	  {
	  for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
	  {
	  Info.AutoAppliedDiscounts.insert(IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger);
	  }
	  } */
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

/** *
This fuction returns the internal contacts key.
It is responible for retrieving the Members details.
 */

void TMembershipEBet::GetExternalContactDetails(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;
   bool UpdateLocalInfo = false;
   try
   {
	  if (Info.CardStr != "")
	  {
		 if (MembershipEBetTCP->RequestMemberInfoByCard(TillID,Info.CardStr))
		 {
			Info.Name = MembershipEBetTCP->InPacket.MemberName();
			Info.MembershipNumber = MembershipEBetTCP->InPacket.MemberNumber();
			Info.ExternalKey = StrToIntDef(MembershipEBetTCP->InPacket.MemberNumber(), 0);
			Info.DateOfBirth = MembershipEBetTCP->InPacket.MemberDOB();
			Info.CardsToAdd.insert(Info.CardStr);
			Info.Points.Clear();
            TPointsTypePair typepair(pttEarned,ptstLoyalty);
            TPointsType type(pasDatabase, typepair,pesExported);
            Info.Points.Load(type,MembershipEBetTCP->InPacket.MemberPoints());
			UpdateLocalInfo = true;
		 }
	  }
	  else if (Info.MembershipNumber != "")
	  {
		 if (MembershipEBetTCP->RequestMemberInfoByMemberNumber(TillID, Info.MembershipNumber))
		 {
			Info.Name = MembershipEBetTCP->InPacket.MemberName();
			Info.MembershipNumber = MembershipEBetTCP->InPacket.MemberNumber();
			Info.ExternalKey = StrToIntDef(MembershipEBetTCP->InPacket.MemberNumber(), 0);
			Info.DateOfBirth = MembershipEBetTCP->InPacket.MemberDOB();

			Info.Points.Clear();
            TPointsTypePair typepair(pttEarned,ptstLoyalty);
            TPointsType type(pasDatabase, typepair,pesExported);
            Info.Points.Load(type,MembershipEBetTCP->InPacket.MemberPoints());

			UpdateLocalInfo = true;
		 }
	  }
	  else if (Info.ExternalKey != 0)
	  {
		 if (MembershipEBetTCP->RequestMemberInfoByMemberNumber(TillID, IntToStr(Info.ExternalKey)))
		 {
			Info.Name = MembershipEBetTCP->InPacket.MemberName();
			Info.MembershipNumber = MembershipEBetTCP->InPacket.MemberNumber();
			Info.ExternalKey = StrToIntDef(MembershipEBetTCP->InPacket.MemberNumber(), 0);
			Info.DateOfBirth = MembershipEBetTCP->InPacket.MemberDOB();

			Info.Points.Clear();
            TPointsTypePair typepair(pttEarned,ptstLoyalty);
            TPointsType type(pasDatabase, typepair,pesExported);
            Info.Points.Load(type,MembershipEBetTCP->InPacket.MemberPoints());

			UpdateLocalInfo = true;
		 }
	  }
	  // Update the Member in our Database.
	  if (UpdateLocalInfo)
	  {
		 Info.ContactKey = GetInternalKey(DBTransaction, Info.ExternalKey);
		 SetContactDetails(DBTransaction, Info.ContactKey, Info);
		 SyncPointsDetials(DBTransaction, Info);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

int TMembershipEBet::GetContactByCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return 0;

   try
   {
   	  //Info.CardStr = ";9572000001=?";
      int Track2Start = Info.CardStr.Pos(";")+1;
      int Track2Delim = Info.CardStr.Pos("=");
      int Track2End = Info.CardStr.Pos("?");
      if(Track2Start != 1 && Track2Delim > 1)
      {
      	Info.CardStr = Info.CardStr.SubString(Track2Start,Track2Delim - Track2Start);
      }
      else if(Track2Start != 1 && Track2End > 1)
      {
   		Info.CardStr = Info.CardStr.SubString(Track2Start,Track2End - Track2Start);
      }

	  GetExternalContactDetails(DBTransaction, Info);
	  Info.ContactKey = GetInternalKey(DBTransaction, Info.ExternalKey);
	  // Get or create the member locally / update there info and return the lcoal key.
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
   return Info.ContactKey;
}

void TMembershipEBet::SyncPointsDetials(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
   try
   {
	  Currency CurrentLocalBalance = TMembership::LoyaltyGetValue(DBTransaction, Info.ContactKey);
	  Currency SyncAdjustment = Info.Points.getPointsBalance() - CurrentLocalBalance;
	  if (SyncAdjustment != 0)
	  {
		 TPointsTransaction PointsTransaction;
		 PointsTransaction.ContactKey = Info.ContactKey;
		 PointsTransaction.TimeStamp = Now();
		 PointsTransaction.ExportStatus = pesNone;
		 PointsTransaction.PointsTransactionType = pttSync;
		 PointsTransaction.PointsTransactionAccountType = ptstLoyalty;
		 PointsTransaction.Adjustment = SyncAdjustment;
		 PointsTransaction.InvoiceNumber = "NA Ebet Sync";
		 TMembership::LoyaltyAddValue(DBTransaction, PointsTransaction);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

int TMembershipEBet::GetContactByCode(int inContactCode)
{
   if (!fEnabled)
	  return 0;
   int RetVal = 0;
   return RetVal;
}

void TMembershipEBet::SetContactCode(int inContactKey, int ContactID)
{
   if (!fEnabled)
	  return;
}

void TMembershipEBet::SyncMembers(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress)
{
   std::auto_ptr <TStringList> List(new TStringList);

   GetContactList(DBTransaction, List.get(), "%Name", "NAME");

   Progress->Show();
   Progress->Max = List->Count;
   int j = 0;
   for (int i = 0; i < List->Count && !Progress->Cancelled; i++)
   {
	  TMMContactInfo Info;
	  int ExternalKey = int(List->Objects[i]);
	  AnsiString Name = List->Strings[i];
	  // Will Cause Sync.
	  Progress->Message = "Syncing " + Name;

	  int InternalKey = GetInternalKey(DBTransaction, ExternalKey);
	  if (InternalKey == 0)
	  {
		 // This will cause a creation "sync" if they dont exist in our DB.
		 InternalKey = GetExternalKey(DBTransaction, ExternalKey);
	  }
	  else
	  {
		 // However if they do exist we want to sync there points.
		 Currency Points = LoyaltyGetExternalValue(DBTransaction, ExternalKey);
		 Currency CurrentLocalBalance = TMembership::LoyaltyGetValue(DBTransaction, InternalKey);

		 Currency SyncAdjustment = Points - CurrentLocalBalance;
		 if (SyncAdjustment != 0)
		 {
			TPointsTransaction PointsTransaction;

			PointsTransaction.ContactKey = InternalKey;
			PointsTransaction.TimeStamp = Now();
			PointsTransaction.ExportStatus = pesNone;
			PointsTransaction.PointsTransactionType = pttSync;
			PointsTransaction.PointsTransactionAccountType = ptstLoyalty;
			PointsTransaction.Adjustment = SyncAdjustment;
			PointsTransaction.InvoiceNumber = "NA Manual Sync";
			TMembership::LoyaltyAddValue(DBTransaction, PointsTransaction);
		 }
	  }

	  Progress->Position = i;
	  Application->ProcessMessages();
	  if (j > 250)
	  {
		 j = 0;
		 DBTransaction.Commit();
		 DBTransaction.StartTransaction();
	  }
	  j++;
   }
}

void TMembershipEBet::GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, std::map <UnicodeString,
   UnicodeString> &Data)
{
   if (!fEnabled)
	  return;
   try
   {
	  Data["%TITLE%"] = "Ebet Koisk not supported";
	  Data["%MSG%"] = "Ebet Koisk not supported"; ;
	  Data["%MSG1%"] = "Ebet Koisk not supported";
	  Data["%VISIT_MSG%"] = "Ebet Koisk not supported";
	  Data["%BIRTHDAY_MSG%"] = "Ebet Koisk not supported";
	  Data["BIRTHDAY_PRINT"] = "F";
	  Data["DOCKET_PRINT"] = "F";
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

/* This overrides TMembership::FindMember because we do not want to seach by Prox or member number.
Only by Card though the Ebet TCPIP interface. We do not want to bring up the local MenuMate version
of this member */
TLoginSuccess TMembershipEBet::FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
   if (!fEnabled)
	  return lsUserNotFound;
	TLoginSuccess LoginSuccess = lsUserNotFound;

   if (UserInfo.ContactKey == 0)
   {
	  if (UserInfo.CardStr != "")
	  {
		 UserInfo.ContactKey = GetContactByCard(DBTransaction, UserInfo);
	  }
	}

   if (UserInfo.ContactKey != 0)
   {
	  LoginSuccess = TestKeyLogin(DBTransaction, UserInfo);
      GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
   }

   return LoginSuccess;
}
