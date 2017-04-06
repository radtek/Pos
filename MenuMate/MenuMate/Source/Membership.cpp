// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Membership.h"
#include "MMLogging.h"
#include "DBTab.h"
#include "Invoice.h"
#include "MMMessageBox.h"
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include "DateUtils.hpp"
#include "DBContacts.h"
#include "GlobalSettings.h"
#include "DBTierLevel.h"
#include <math.h>
#include <Memory>
#include <set>

#include <System.hpp>

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// -----------------------TContactMember--------------------------------------
// ---------------------------------------------------------------------------

TMembership::TMembership(TModules &Modules)
{
   ContactType = eMember;
   ContactTabType = TabMember;
   ReadOnlyInterface = false;
   Modules.Status[eRegMembers]["ReadOnly"] = ReadOnlyInterface;
   Enabled = Modules.Status[eRegMembers]["Enabled"];
   DemoMode = false;

   EnableIncrementalSearch = false;
   NameOnPoleDisplay = eMNPDNone;
   MinMemberNumber = 1;
   MaxRemembered = 1;
   CurrentYearPoints = 0;
   PreviousYearPoints = 0;
   AvailableBDPoint = 0;
   AvailableFVPoint = 0;
}

TMembership::~TMembership()
{

}

void TMembership::InitTransaction(Database::TDBTransaction &DBTransaction)
{
}

void TMembership::Initialise(Database::TDBTransaction &DBTransaction)
{
   TContact::Initialise(DBTransaction);
   SetEnabled(true);
}

void TMembership::LoyaltyAddValue(Database::TDBTransaction &DBTransaction, TPointsTransaction &PointsTransaction)
{
	if (!fEnabled)
		return;
	if (DemoMode)
		return;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"LOADED_POINTS "
			"FROM "
				"CONTACTS "
			"WHERE "
				"CONTACTS_KEY = :CONTACTS_KEY AND "
				"CONTACT_TYPE = :CONTACT_TYPE";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = PointsTransaction.ContactKey;
		IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		IBInternalQuery->ExecQuery();

		Currency loadedPoints = IBInternalQuery->FieldByName("LOADED_POINTS")->AsCurrency;
		Currency spentValue	= PointsTransaction.Adjustment;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE "
				"CONTACTS "
			"SET "
				"TOTAL_SPENT = TOTAL_SPENT + :SPENTVALUE,"
				"LOADED_POINTS = LOADED_POINTS + :SPENTVALUE "
			"WHERE "
				"CONTACTS_KEY = :CONTACTS_KEY AND "
				"CONTACT_TYPE = :CONTACT_TYPE";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = PointsTransaction.ContactKey;

		if (PointsTransaction.PointsTransactionType == pttRedeemed ||
            PointsTransaction.PointsTransactionType == pttRedeemedBD ||
            PointsTransaction.PointsTransactionType == pttRedeemedFV )
		{
			if (loadedPoints < spentValue)
			{
				IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = -loadedPoints;
				spentValue -= loadedPoints;
			}
			else
			{
				IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = -spentValue;
				spentValue = 0;
			}
		}
		else if(PointsTransaction.PointsTransactionType == pttPurchased)
		{
			IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = PointsTransaction.Adjustment;
		}
        else if(PointsTransaction.PointsTransactionType == pttRefund)
		{
            IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = -spentValue;  ////MM-5999 value changed from +ve to -ve
            spentValue = 0;
		}
		IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		IBInternalQuery->ExecQuery();


		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE "
				"CONTACTS "
			"SET "
				"TOTAL_SPENT = TOTAL_SPENT + :SPENTVALUE,"
				"EARNT_POINTS = EARNT_POINTS + :SPENTVALUE "
			"WHERE "
				"CONTACTS_KEY = :CONTACTS_KEY AND "
				"CONTACT_TYPE = :CONTACT_TYPE";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = PointsTransaction.ContactKey;

		if (PointsTransaction.PointsTransactionType == pttRedeemed ||
            PointsTransaction.PointsTransactionType == pttRedeemedBD ||
            PointsTransaction.PointsTransactionType == pttRedeemedFV)
		{
			IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = -spentValue;
		}
		else if(PointsTransaction.PointsTransactionType == pttEarned ||
                PointsTransaction.PointsTransactionType == pttBirthdayBonus ||
                PointsTransaction.PointsTransactionType == pttFirstVisit)
		{
			IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = PointsTransaction.Adjustment;
		}
        else if(PointsTransaction.PointsTransactionType == pttRefund)
		{
               IBInternalQuery->ParamByName("SPENTVALUE")->AsCurrency = -spentValue;  ///MM-5999 value changed from +ve to -ve


		}
		IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		IBInternalQuery->ExecQuery();


		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		PointsTransaction.PointsTransactionsKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"INSERT INTO POINTSTRANSACTIONS "
			"("
				"POINTSTRANSACTIONS_KEY,"
				"CONTACTS_KEY,"
				"TIME_STAMP,"
				"TIME_STAMP_EXPORTED,"
				"ADJUSTMENT_TYPE,"
				"ADJUSTMENT_SUBTYPE,"
				"ADJUSTMENT,"
				"EXPORTED_STATUS,"
				"MISC,"
				"INVOICE_NUMBER"
			")"
			" VALUES "
			"("
				":POINTSTRANSACTIONS_KEY,"
				":CONTACTS_KEY,"
				":TIME_STAMP,"
				":TIME_STAMP_EXPORTED,"
				":ADJUSTMENT_TYPE,"
				":ADJUSTMENT_SUBTYPE,"
				":ADJUSTMENT,"
				":EXPORTED_STATUS,"
				":MISC,"
				":INVOICE_NUMBER"
			");";
		IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = PointsTransaction.PointsTransactionsKey;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = PointsTransaction.ContactKey;
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = PointsTransaction.TimeStamp;
		IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = PointsTransaction.TimeStampExported;
		IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = PointsTransaction.PointsTransactionType;
		IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = PointsTransaction.PointsTransactionAccountType;
		if (PointsTransaction.PointsTransactionType == pttRedeemed ||
            PointsTransaction.PointsTransactionType == pttRedeemedBD ||
            PointsTransaction.PointsTransactionType == pttRefund||   //this is for refunding points MM-5999
            PointsTransaction.PointsTransactionType == pttRedeemedFV )
		{
            if(PointsTransaction.Adjustment != 0)
            {
				IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = -PointsTransaction.Adjustment;
            }

		}
		else
		{
			if(PointsTransaction.Adjustment != 0)
				IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = PointsTransaction.Adjustment;
		}
		IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = PointsTransaction.ExportStatus;
		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = PointsTransaction.InvoiceNumber;

		TMemoryStream *Streamcheck = PointsTransaction.GetAsStream();
		Streamcheck->Position = 0;
		IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembership::UpdatePointsTransactions(Database::TDBTransaction &DBTransaction, std::vector <TPointsTransaction> &PointsTransactions)
{
   if (!fEnabled)
	  return;
   if (DemoMode)
	  return;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "UPDATE " " POINTSTRANSACTIONS" " SET" " TIME_STAMP_EXPORTED = :TIME_STAMP_EXPORTED,"
		  " EXPORTED_STATUS = :EXPORTED_STATUS," " MISC = :MISC" " WHERE" " POINTSTRANSACTIONS_KEY = :POINTSTRANSACTIONS_KEY";

	  for (std::vector <TPointsTransaction> ::iterator ptrPointsTransaction = PointsTransactions.begin();
		 ptrPointsTransaction != PointsTransactions.end(); ptrPointsTransaction++)
	  {
		 ptrPointsTransaction->TimeStampExported = Now();
		 ptrPointsTransaction->ExportStatus = pesExported;
		 IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = ptrPointsTransaction->PointsTransactionsKey;
		 IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = ptrPointsTransaction->ExportStatus;
		 IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = ptrPointsTransaction->TimeStampExported;

		 TMemoryStream *Streamcheck = ptrPointsTransaction->GetAsStream();
		 Streamcheck->Position = 0;
		 IBInternalQuery->ParamByName("MISC")->LoadFromStream(Streamcheck);
		 IBInternalQuery->ExecQuery();
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TMembership::GetValidPointsTransactions(Database::TDBTransaction &DBTransaction, int ContactKey,
   TPointsTransactionContainer &PointsTransactions)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  // If there is no Restore Point Create one.
	  // Otherwise update the image.
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT " " * " " FROM " "  POINTSTRANSACTIONS " " WHERE " "  CONTACTS_KEY = :CONTACTS_KEY AND "
		  "  EXPORTED_STATUS = :EXPORTED_STATUS";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesNone;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 TPointsTransaction Transaction;
		 Transaction.PointsTransactionsKey = IBInternalQuery->FieldByName("POINTSTRANSACTIONS_KEY")->AsInteger;
		 Transaction.ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		 Transaction.TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
		 Transaction.TimeStampExported = IBInternalQuery->FieldByName("TIME_STAMP_EXPORTED")->AsDateTime;
		 Transaction.PointsTransactionType = IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger;
		 Transaction.PointsTransactionAccountType = IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger;
		 // Adjustment Value is always a Positive number, the Sign in the DB is determined by the
		 // PointsTransactionType when it is written to the db.
		 Transaction.Adjustment = fabs(IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency);
		 Transaction.ExportStatus = IBInternalQuery->FieldByName("EXPORTED_STATUS")->AsInteger;
		 Transaction.InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
		 TMemoryStream *Streamcheck = Transaction.GetAsStream();
		 Streamcheck->Position = 0;
		 IBInternalQuery->FieldByName("MISC")->SaveToStream(Streamcheck);
		 if (Transaction.Validate())
		 {
			PointsTransactions.push_back(Transaction);
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

Currency TMembership::LoyaltyGetSpendValue(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return 0;

   Currency RetVal = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "select SUM((PRICE*QTY) + DISCOUNT)TOTAL_SPENT from ARCHIVE where LOYALTY_KEY = :LOYALTY_KEY " "union "
		  "select SUM((PRICE*QTY) + DISCOUNT)TOTAL_SPENT from DAYARCHIVE where LOYALTY_KEY = :LOYALTY_KEY ";

	  IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();

	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 RetVal += IBInternalQuery->FieldByName("TOTAL_SPENT")->AsCurrency;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

Currency TMembership::LoyaltyGetValue(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return 0;

   Currency RetVal = 0;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT a.EARNT_POINTS, a.LOADED_POINTS FROM CONTACTS a where a.CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();

	  if(!IBInternalQuery->Eof)
	  {
		 RetVal = IBInternalQuery->FieldByName("EARNT_POINTS")->AsCurrency +
                  IBInternalQuery->FieldByName("LOADED_POINTS")->AsCurrency;
	  }


      /*TContactPoints Points;
      TDBContacts::GetPointsBalances(DBTransaction,inContactKey,Points);
      RetVal = Points.getPointsBalance(); */

      /* TODO 1 -o Michael -c Before Release : Testing Required
      This Function used to return the balance from the contacts table.
      This table is now used for readonly reporting.
      To test Generate a member in the old version, uprgade running the parser that
      should put a pttSync record into the POINTSTRANSACTION table to ensure the
      balance returned matches the balance in the TOTAL_SPENT of the CONTACTS table.
      After the parser run check that members points balance.

      Do this for all MenuMate membership only, ERS, SmartCards etc will auto sync
      to the external points store as normal, any bugs will show up in
      your normal testing but points balances issues with external systems
      should auto correct.

      Also note that in many cases with many of the members systems jsut swiping them
      in will generate this sync value for you against the balance of the
      POINTSTRANSACTION table and the external members balance.*/

/*	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "TOTAL_SPENT " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("TOTAL_SPENT")->AsCurrency;
	  }*/
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

TLoginSuccess TMembership::FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
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

	  if (UserInfo.ContactKey == 0 && UserInfo.ProxStr != "")
	  {
		 UserInfo.ContactKey = GetContactByProx(DBTransaction, UserInfo.ProxStr);
	  }

	  if (UserInfo.ContactKey == 0 && UserInfo.MembershipNumber != 0 && UserInfo.SiteID != 0)
	  {
		 UserInfo.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, UserInfo.MembershipNumber,UserInfo.SiteID);
	  }
	}


   if (UserInfo.ContactKey != 0)
   {
	  LoginSuccess = TestKeyLogin(DBTransaction, UserInfo);
   }

   // Even is the Account it Locked you can still return the members infomation.
   GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);

   return LoginSuccess;
}

TLoginSuccess TMembership::TestKeyLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess Success = lsUserNotFound;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "Select CONTACTS_KEY,ACCESS_LEVEL,CONTACTS_3RDPARTY_KEY from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = UserInfo.ContactKey;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 Success = GetSubsPaid(DBTransaction, UserInfo.ContactKey);
	  }
	  IBInternalQuery->Close();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return Success;
}

void TMembership::GetLuckyMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &LuckyMember, int Hours, UnicodeString Location)
{
   std::set <int> DrawPool;

   if (!fEnabled)
	  return;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  UnicodeString OrderLocationFilter = "";
	  UnicodeString DayArchiveLocationFilter = "";
	  UnicodeString ArchiveLocationFilter = "";
	  if (Location != "")
	  {
		 OrderLocationFilter = " AND ORDERS.ORDER_LOCATION = '" + Location + "' ";
		 DayArchiveLocationFilter = " AND DAYARCHIVE.ORDER_LOCATION = '" + Location + "' ";
		 ArchiveLocationFilter = " AND ARCHIVE.ORDER_LOCATION = '" + Location + "' ";
	  }

	  IBInternalQuery->SQL->Text = "SELECT " "DISTINCT(ORDERS.LOYALTY_KEY) " "FROM " "ORDERS " "WHERE "
		  "ORDERS.TIME_STAMP > :TIME_STAMP AND ORDERS.LOYALTY_KEY != 0" + OrderLocationFilter +

		  " Union All "

		  "SELECT " "DISTINCT(DAYARCHIVE.LOYALTY_KEY) " "FROM " "DAYARCHIVE " "WHERE "
		  "DAYARCHIVE.TIME_STAMP > :TIME_STAMP AND DAYARCHIVE.LOYALTY_KEY != 0" + DayArchiveLocationFilter +

		  " Union All "

		  "SELECT " "DISTINCT(ARCHIVE.LOYALTY_KEY) " "FROM " "ARCHIVE " "WHERE "
		  "ARCHIVE.TIME_STAMP > :TIME_STAMP AND ARCHIVE.LOYALTY_KEY != 0" + ArchiveLocationFilter;

	  IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - (1.0 / 24.0 * double(Hours));
	  IBInternalQuery->ExecQuery();
	  while (!IBInternalQuery->Eof)
	  {
		 DrawPool.insert(IBInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger);
		 IBInternalQuery->Next();
	  }

	  if (DrawPool.size() > 0)
	  {
		 int WinningKey = 0;
		 std::set <int> ::iterator ptrDrawPool = DrawPool.begin();
		 int WinningIndex = rand() % DrawPool.size();
		 advance(ptrDrawPool, WinningIndex);
		 if (ptrDrawPool != DrawPool.end())
		 {
			WinningKey = *ptrDrawPool;
			GetContactDetails(DBTransaction, WinningKey, LuckyMember);
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TMembership::GetInvoices(Database::TDBTransaction &DBTransaction, TStringList * TabList, int inContactKey)
{
   if (!fEnabled)
	  return;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->SQL->Text = "SELECT " "INVOICES.INVOICE_KEY, " "INVOICES.INVOICE_NUMBER, " "TAB.TAB_KEY " "FROM " "INVOICES "
		  " LEFT JOIN TAB ON TAB.INVOICE_KEY = INVOICES.INVOICE_KEY " "WHERE " "INVOICES.CONTACTS_KEY = :CONTACTS_KEY AND"
		  " INVOICES.CLOSED != 'T' " " ORDER BY " "INVOICE_NUMBER";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();
	  while (!IBInternalQuery->Eof)
	  {
		 int Index = TabList->Add(IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString);
		 // Little bit of pointer abuse but we just want to store the int somewhere.
		 TabList->Objects[Index] = (TObject*)IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		 IBInternalQuery->Next();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

int TMembership::GetContactByCode(Database::TDBTransaction &DBTransaction, int inContactCode)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "CONTACTS_KEY " "FROM " "CONTACTS " "WHERE " "CONTACT_ID = :CONTACT_ID";
	  IBInternalQuery->ParamByName("CONTACT_ID")->AsInteger = inContactCode;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TMembership::SetContactCode(Database::TDBTransaction &DBTransaction, int inContactKey, int ContactID)
{
   if (!fEnabled)
	  return;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "UPDATE " " CONTACTS" " SET" " CONTACT_ID = :CONTACT_ID" " WHERE" " CONTACTS_KEY = :CONTACTS_KEY AND "
		  " CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("CONTACT_ID")->AsInteger = ContactID;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TMembership::SetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;
   try
   {

      GenerateMembershipNumber(DBTransaction, Info);
	  // Proform Unique checks here so no indexes are Violated.
	  CheckSiteIndex(DBTransaction, inContactKey, Info);
	  OnBeforeSaveMember.Occured(Info);
 	  TContact::SetContactDetails(DBTransaction, inContactKey, Info);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TMembership::GenerateMembershipNumber(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info)
{
      // Update Member Number if Blank.
	  if (Info.MembershipNumber == NULL   || Info.MembershipNumber == "")
	  {
		 if (RecycleMemberNumber)
		 {
			Info.MembershipNumber = GetNextRecycledMemberNumber(DBTransaction);
		 }
		 else
		 {
			Info.MembershipNumber = GetNextMemberNumber(DBTransaction);
		 }
	  }
}

void TMembership::SetContactLoyaltyAttributes(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT ATTRIB_KEY FROM LOYALTYATTRIBUTES WHERE CONTACTS_KEY=:CONTACTS_KEY";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
      IBInternalQuery->ExecQuery();
      bool createRecord = true;
      int attrKey = 0;
      if(!IBInternalQuery->Eof)
      {
         createRecord = false;
         attrKey = IBInternalQuery->FieldByName("ATTRIB_KEY")->AsInteger;
      }

      if(createRecord)
      {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOYALTYATTRIBUTES, 1) FROM RDB$DATABASE";
          IBInternalQuery->ExecQuery();
          int key = IBInternalQuery->Fields[0]->AsInteger;
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text = "INSERT INTO LOYALTYATTRIBUTES "
                                         "(ATTRIB_KEY, UUID, SITE_ID, CONTACTS_KEY, ACTIVATION_TOKEN,CREATION_TIME ) VALUES "
                                         "(:ATTRIB_KEY, :UUID, :SITE_ID, :CONTACTS_KEY, :ACTIVATION_TOKEN,:CREATION_TIME)";
          IBInternalQuery->ParamByName("ATTRIB_KEY")->AsInteger = key;
          IBInternalQuery->ParamByName("UUID")->AsString = Info.CloudUUID;
          IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
          IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
          IBInternalQuery->ParamByName("ACTIVATION_TOKEN")->AsString = Info.ActivationToken;
          IBInternalQuery->ParamByName("CREATION_TIME")->AsDateTime = Now();
          IBInternalQuery->ExecQuery();
          IBInternalQuery->Close();
      }
      else
      {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text = "UPDATE LOYALTYATTRIBUTES SET "
                                       "UUID = :UUID, "
                                       "SITE_ID = :SITE_ID, "
                                       "CONTACTS_KEY = :CONTACTS_KEY, "
                                       "ACTIVATION_TOKEN = :ACTIVATION_TOKEN "
                                       "WHERE  ATTRIB_KEY =:ATTRIB_KEY";
          IBInternalQuery->ParamByName("ATTRIB_KEY")->AsInteger = attrKey;
          IBInternalQuery->ParamByName("UUID")->AsString = Info.CloudUUID;
          IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
          IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
          IBInternalQuery->ParamByName("ACTIVATION_TOKEN")->AsString = Info.ActivationToken;
          IBInternalQuery->ExecQuery();
          IBInternalQuery->Close();
      }
    }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

bool TMembership::CheckSiteIndex(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return 0;

   bool RetVal = false;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = " SELECT CONTACTS_KEY" " FROM " "  CONTACTS" " WHERE" " NAME = :NAME AND "
    " EMAIL = :EMAIL AND "
   " CONTACTS_3RDPARTY_KEY = :CONTACTS_3RDPARTY_KEY AND " " SITE_ID = :SITE_ID AND " " MEMBER_NUMBER = :MEMBER_NUMBER AND "
	   " CONTACTS_KEY != :CONTACTS_KEY";

   IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
   IBInternalQuery->ParamByName("EMAIL")->AsString = Info.EMail;
   IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = Info.ExternalKey;
   IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
   IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Info.MembershipNumber;
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
   {
	  throw Exception("A Member with these details already Exists");
   }
}

void TMembership::GetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;

   try
   {
	  TContact::GetContactDetails(DBTransaction, inContactKey, Info);
	  switch(NameOnPoleDisplay)
	  {
	  case eMNPDNone:
		 Info.PoleDisplayName = "";
		 break;
	  case eMNPDAlias:
		 Info.PoleDisplayName = Info.Alias;
		 break;
	  case eMNPDFullName:
		 Info.PoleDisplayName = Info.Name;
		 break;
	  default:
		 Info.PoleDisplayName = "";
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

bool TMembership::GetContactExists(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return 0;

   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT CONTACTS_KEY" " FROM " "  CONTACTS" " WHERE" " CONTACTS_KEY = :CONTACTS_KEY AND "
		  " CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = true;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

UnicodeString TMembership::GetNextRecycledMemberNumber(Database::TDBTransaction &DBTransaction)
{
   bool NotFound = true;
   int MemberNumber = 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "Select MEMBER_NUMBER from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and MEMBER_NUMBER = :MEMBER_NUMBER";
	  for (int i = MinMemberNumber; NotFound; i++)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		 IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = IntToStr(i);
		 IBInternalQuery->ExecQuery();
		 if (IBInternalQuery->RecordCount == 0)
		 {
			NotFound = false;
			MemberNumber = i;
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return IntToStr(MemberNumber);
}

UnicodeString TMembership::GetNextMemberNumber(Database::TDBTransaction &DBTransaction)
{
   bool NotFound = true;
   int MemberNumber = 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "Select MEMBER_NUMBER from CONTACTS";
	  // You also want to search deleted types as well so removed the type filter.
	  // IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger	= ContactType;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; !IBInternalQuery->Next())
	  {
		 UnicodeString StrMemberNumber = IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString;
		 if (StrToIntDef(StrMemberNumber, 0) > MemberNumber)
		 {
			MemberNumber = StrToIntDef(StrMemberNumber, 0);
		 }
	  }
	  MemberNumber++;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return IntToStr(MemberNumber);
}

TLoginSuccess TMembership::GetSubsPaid(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   return lsAccepted;
}

void TMembership::GetReportMemberStart(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   // Start the Member Table.
   UnicodeString Temp = HTML_START;
   Temp = AnsiReplaceStr(Temp, "%TITLE%", "Member Report");
   Report->Add(Temp);
}

void TMembership::SyncMembers(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress)
{

}

void TMembership::GetReportMemberCardsInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   Database::TcpIBQuery IBInternalQuery(new TIBQuery(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select CARD_STATUS STATUS,CARD_CREATION_DATE \"CREATION DATE\",BACKUP_TIME_STAMP \"BACKUP DATE\",ATR GUID,SMARTCARDS_KEY from SMARTCARDS where CONTACTS_KEY = :LOYALTY_KEY " "order by CARD_CREATION_DATE";
   IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->Open();

   if (IBInternalQuery->RecordCount)
   {
	  std::auto_ptr <TDataSetTableProducer> DataSetTableProducer(new TDataSetTableProducer(NULL));
	  DataSetTableProducer->OnFormatCell = GetReportCardInfoFormatCell;
	  DataSetTableProducer->DataSet = IBInternalQuery.get();
	  DataSetTableProducer->Header->Clear();
	  DataSetTableProducer->MaxRows = 100;
	  DataSetTableProducer->Caption =
		  "<span style=\"font-weight: bold; text-decoration: underline;\"> All Cards Previously Assigned To  " + Member.Name +
		  "<br></span>";
	  DataSetTableProducer->TableAttributes->Border = 1;
	  Report->Add(DataSetTableProducer->Content());
	  Report->Add("<br>");
   }
}

void __fastcall TMembership::GetReportCardInfoFormatCell(TObject *Sender, int CellRow, int CellColumn, THTMLBgColor &BgColor,
   THTMLAlign &Align, THTMLVAlign &VAlign, UnicodeString &CustomAttrs, UnicodeString &CellData)
{
   if (CellRow == 0)
   {
	  Align = haCenter;
   }
   else
   {
	  if (CellColumn == 0)
	  {
		 Align = haCenter;
		 if (CellData == "0")
		 {
			CellData = "Card Blocked";
		 }
		 else if (CellData == "1")
		 {
			CellData = "Card Active";
		 }
	  }
	  else if (CellColumn == 4)
	  {
		 CellData = "<a href=" ":internal:[SmartCardDetails]:{" + CellData + "}" "> Show Card Details... </a>";
	  }
	  else
	  {
		 Align = haRight;
	  }
   }
}
// ---------------------------------------------------------------------------
void TMembership::GetReportThisCardsInfo(Database::TDBTransaction &DBTransaction, int inSmartCardKey, UnicodeString GUID,
   TStringList *Report)
{
   Database::TcpIBQuery IBInternalQuery(new TIBQuery(NULL));
   DBTransaction.RegisterQuery(IBInternalQuery);
   AnsiString ATR = GUID;
   if (inSmartCardKey != 0)
   {
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "select SMARTCARDS.ATR from SMARTCARDS where SMARTCARDS.SMARTCARDS_KEY = :SMARTCARDS_KEY ";
	  IBInternalQuery->ParamByName("SMARTCARDS_KEY")->AsString = inSmartCardKey;
	  IBInternalQuery->Open();
      ATR = IBInternalQuery->FieldByName("ATR")->AsString;
   }

   if (inSmartCardKey != 0)
   {
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "select CARD_STATUS STATUS,BACKUP_TIME_STAMP \"BACKUP DATE\",SMARTCARDS.CARD_CREATION_DATE \"CREATION DATE\",CONTACTS.NAME \"MEMBER NAME\",CONTACTS.MEMBER_NUMBER \"MEMBER NUMBER\",SMARTCARDS_KEY " " from SMARTCARDS " " LEFT JOIN CONTACTS ON SMARTCARDS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY " " where SMARTCARDS.SMARTCARDS_KEY = :SMARTCARDS_KEY " " order by SMARTCARDS.BACKUP_TIME_STAMP desc";
	  IBInternalQuery->ParamByName("SMARTCARDS_KEY")->AsString = inSmartCardKey;
	  IBInternalQuery->Open();
   }
   else
   {
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "select CARD_STATUS STATUS,BACKUP_TIME_STAMP \"BACKUP DATE\",SMARTCARDS.CARD_CREATION_DATE \"CREATION DATE\",CONTACTS.NAME \"MEMBER NAME\",CONTACTS.MEMBER_NUMBER \"MEMBER NUMBER\",SMARTCARDS_KEY " " from SMARTCARDS " " LEFT JOIN CONTACTS ON SMARTCARDS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY " " where SMARTCARDS.ATR = :ATR " " order by SMARTCARDS.BACKUP_TIME_STAMP desc";
	  IBInternalQuery->ParamByName("ATR")->AsString = GUID;
	  IBInternalQuery->Open();
   }

   if (IBInternalQuery->RecordCount)
   {

	  std::auto_ptr <TDataSetTableProducer> DataSetTableProducer(new TDataSetTableProducer(NULL));
	  DataSetTableProducer->OnFormatCell = GetReportThisCardsInfoFormatCell;
	  DataSetTableProducer->DataSet = IBInternalQuery.get();
	  DataSetTableProducer->Header->Clear();
	  DataSetTableProducer->MaxRows = 100;
	  DataSetTableProducer->Caption =
		  "<span style=\"font-weight: bold; text-decoration: underline;\">Database Card History " + ATR + "<br></span>";
	  DataSetTableProducer->TableAttributes->Border = 1;
	  Report->Add(DataSetTableProducer->Content());
	  Report->Add("<br>");
   }
}
// ---------------------------------------------------------------------------
void __fastcall TMembership::GetReportThisCardsInfoFormatCell(TObject *Sender, int CellRow, int CellColumn, THTMLBgColor &BgColor,
   THTMLAlign &Align, THTMLVAlign &VAlign, UnicodeString &CustomAttrs, UnicodeString &CellData)
{
   if (CellRow == 0)
   {
	  Align = haCenter;
   }
   else
   {
	  if (CellColumn == 0)
	  {
		 Align = haCenter;
		 if (CellData == "0")
		 {
			CellData = "Card Blocked";
		 }
		 else if (CellData == "1")
		 {
			CellData = "Card Active";
		 }
		 else if (CellData == "2")
		 {
			CellData = "Card Backup";
		 }

	  }
	  else if (CellColumn == 6)
	  {
		 CellData = "<a href=" ":internal:[SmartCardDetails]:{" + CellData + "}" "> Show Card Details... </a>";
	  }
	  else
	  {
		 Align = haRight;
	  }
   }
}
// ---------------------------------------------------------------------------
void TMembership::GetReportMemberInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   TMMContactInfo ContactInfo;
   GetContactDetails(DBTransaction,Member.ContactKey,ContactInfo);

   // Add the Membership Block.
   UnicodeString Temp = TABLE_START;
   Report->Add(Temp);
   Temp = TABLE_ROW4_HEADER;
   Temp = ReplaceStr(Temp, "%TABLETITLE%", "Member Information");
   //MessageBox(ContactInfo.Name +" "+ContactInfo.Surname + "'s Birthday" + "\n" , "Happy Birthday", MB_OK + MB_ICONINFORMATION);
   Temp = ReplaceStr(Temp, "%TABLEHEADER%", " " + ContactInfo.Title + " " + ContactInfo.Name +" "+ContactInfo.Surname + " (" + ContactInfo.MembershipNumber + ")");
   //MessageBox(Temp + "'s Temp" + "\n" , "Happy Birthday", MB_OK + MB_ICONINFORMATION);
   Report->Add(Temp);

   UnicodeString TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Alias");
   TempRow = ReplaceStr(TempRow, "%ROWCONTENT1%", ContactInfo.Alias == "" ? ContactInfo.Name+" "+ContactInfo.Surname : ContactInfo.Alias);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Initials");
   TempRow = ReplaceStr(TempRow, "%ROWCONTENT2%", ContactInfo.Initials == "" ? UnicodeString("&nbsp;") : ContactInfo.Initials);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;

   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Location Address");
   UnicodeString TempAdd = ContactInfo.LocationAddress;
   if (TempAdd == "")
	  TempAdd = UnicodeString("&nbsp;");
   TempAdd = AnsiReplaceStr(TempAdd, "\r", "<br>");
   TempRow = ReplaceStr(TempRow, "%ROWCONTENT1%", TempAdd);

   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Mailing Address");
   TempAdd = ContactInfo.MailingAddress;
   if (TempAdd == "")
	  TempAdd = UnicodeString("&nbsp;");
   TempAdd = AnsiReplaceStr(TempAdd, "\r", "<br>");
   TempRow = ReplaceStr(TempRow, "%ROWCONTENT2%", TempAdd);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Phone");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", ContactInfo.Phone == "" ? UnicodeString("&nbsp;") : ContactInfo.Phone);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "EMail");
   TempRow = ReplaceStr(TempRow, "%ROWCONTENT2%", ContactInfo.EMail == "" ? UnicodeString("&nbsp;") : ContactInfo.EMail);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "DOB");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", ContactInfo.DateOfBirth.FormatString(" dd/mm/yyyy "));
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Mobile");
   TempRow = ReplaceStr(TempRow, "%ROWCONTENT2%", ContactInfo.Mobile == "" ? UnicodeString("&nbsp;") : ContactInfo.Mobile);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Gender");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", ContactInfo.Sex == "" ? UnicodeString("&nbsp;") : ContactInfo.Sex);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Last Purchase");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", GetLastVisit(DBTransaction, Member.ContactKey).FormatString(" dd-mm-yy "));
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Total Spent");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", FormatFloat("0.00", LoyaltyGetSpendValue(DBTransaction, Member.ContactKey)));
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Current Points");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", FormatFloat("0.00", LoyaltyGetValue(DBTransaction, Member.ContactKey)));
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Tier level");
   TDBTierLevel::GetTierLevelOfMember(DBTransaction,Member);
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", TDBTierLevel::GetTierLevelName(DBTransaction,Member.TierLevel));
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%",  "Current Year Points");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", FormatFloat("0.00",GetEarnedPointsForCurrentYear(DBTransaction, Member)));
   Report->Add(TempRow);
   TempRow = TABLE_STOP;
   Report->Add(TempRow);
}

void TMembership::GetReportMemberInfoRestricted(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   // Add the Membership Block.
   AnsiString Temp = TABLE_START;
   Report->Add(Temp);
   Temp = TABLE_ROW4_HEADER;
   Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Member Information");
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " (" + Member.MembershipNumber + ")");
   Report->Add(Temp);

   AnsiString TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Information");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", "Restricted");
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Access");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", "Denied");
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;

   TempRow = TABLE_STOP;
   Report->Add(TempRow);
}

void TMembership::GetReportMemberFavouritesInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select count(ITEM_NAME) QTY,ITEM_NAME,SIZE_NAME,SUM(PRICE)SPEND from ARCHIVE where LOYALTY_KEY = :LOYALTY_KEY "
	   " and TIME_STAMP > :TIME_STAMP " "group by ITEM_NAME,SIZE_NAME " "union "
	   "select count(ITEM_NAME) QTY,ITEM_NAME,SIZE_NAME,SUM(PRICE)SPEND from DAYARCHIVE where LOYALTY_KEY = :LOYALTY_KEY "
	   " and TIME_STAMP > :TIME_STAMP " "group by ITEM_NAME,SIZE_NAME,PRICE order by 1 desc";

   IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 365;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
   {
	  // Add the Membership Block.
	  UnicodeString Temp = TABLE_START;
	  Report->Add(Temp);
	  Temp = TABLE_ROW6_HEADER;
	  Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Member Information");
	  Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " (" + Member.MembershipNumber + ")");
	  Report->Add(Temp);

	  // Add the Membership Block.
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 UnicodeString TempRow = TABLE_ROW6A;
		 TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Qty :");
		 TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", IBInternalQuery->FieldByName("QTY")->AsString);
		 TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Item :");
		 TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%",
			IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " + IBInternalQuery->FieldByName("SIZE_NAME")->AsString);
		 TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE3%", "Spend :");
		 TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT3%", FormatFloat("0.00", IBInternalQuery->FieldByName("SPEND")->AsFloat));
		 Report->Add(TempRow);
	  }

	  Temp = TABLE_STOP;
	  Report->Add(Temp);
   }
}

Currency TMembership::GetPointsHeldTotal(Database::TDBTransaction &DBTransaction)
{
   Currency RetVal = 0;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "select SUM(ADJUSTMENT)TOTAL from POINTSTRANSACTIONS " " where " " EXPORTED_STATUS = :EXPORTED_STATUS ";
   IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesNone;
   IBInternalQuery->ExecQuery();
   RetVal = IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
   return RetVal;
}

void TMembership::GetReportMemberPointsInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString Temp = TABLE_START;
   Report->Add(Temp);
   Temp = TABLE_ROW2_HEADER;
   Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points Information");
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " (" + Member.MembershipNumber + ")");
   Report->Add(Temp);

   UnicodeString TransactionCount = "0.00";

   // Summary Points.
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select count(CONTACTS_KEY) TRANSCOUNT from POINTSTRANSACTIONS " " where CONTACTS_KEY = :CONTACTS_KEY " " AND EXPORTED_STATUS = :EXPORTED_STATUS ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesExported;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->FieldByName("TRANSCOUNT")->AsString != "")
   {
	  TransactionCount = IBInternalQuery->FieldByName("TRANSCOUNT")->AsString;
   }

   UnicodeString TempRow = TABLE_ROW2;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Transactions :");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", TransactionCount);
   Report->Add(TempRow);
   TDBContacts::GetPointsBalances(DBTransaction,Member.ContactKey, Member.Points);
    TPointsStore Store = Member.Points.getPointsStore();
    TContactPoints::PadOutEmptyStoreTPointsType(Store);
    std::map<TPointsType, Currency>::iterator ptr = Store.begin();
    bool PrintedHeldTitle = false;
    for(; ptr != Store.end(); advance(ptr,1))
    {
       if(ptr->first.ExportStatus == pesNone && PrintedHeldTitle == false)
        {
           TempRow = TABLE_ROW2;
           TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Store Balance :");
           TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.Points.getPointsBalance());
           Report->Add(TempRow);

           TempRow = TABLE_ROW2;
           TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "&nbsp");
           TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", "&nbsp");
           Report->Add(TempRow);

           TempRow = TABLE_ROW2;
           TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Store Points Held :");
           TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", "&nbsp");
           Report->Add(TempRow);
           PrintedHeldTitle = true;
        }
        UnicodeString TempRow = TABLE_ROW2;
        TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", ptr->first.Name());
        TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", FormatFloat("0.00", ptr->second));
        Report->Add(TempRow);
    }


    TempRow = TABLE_ROW2;
    TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Held Total :");
    TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.Points.getPointsHeldTotal());
    Report->Add(TempRow);

   Temp = TABLE_STOP;
   Report->Add(Temp);

   Temp = TABLE_START;
   Report->Add(Temp);
   Temp = TABLE_ROW4_HEADER;
   Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points History");
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " (" + Member.MembershipNumber + ")");
   Report->Add(Temp);

   TempRow = TABLE_ROW4S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "35%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "25%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "25%");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");

   TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Time</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Points</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<b>Invoice Number</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>Adjustment Type</b>");
   Report->Add(TempRow);
   // Summary Points.
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select TIME_STAMP,ADJUSTMENT,INVOICE_NUMBER,ADJUSTMENT_TYPE from POINTSTRANSACTIONS "
            " where CONTACTS_KEY = :CONTACTS_KEY " " AND EXPORTED_STATUS != :EXPORTED_STATUS "
             " AND TIME_STAMP > :TIME_STAMP" " AND ADJUSTMENT <> 0 order by TIME_STAMP desc";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = pesCorrupted;
   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 365;
   IBInternalQuery->ExecQuery();
   for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
   {
	  TempRow = TABLE_ROW4S;
	  TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "35%");
	  TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "15%");
	  TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "25%");
	  TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "25%");
	  TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "left");
	  TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "right");
	  TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
	  TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");

	  TempRow = AnsiReplaceStr(TempRow, "%COL1%", IBInternalQuery->FieldByName("TIME_STAMP")->AsString);
	  TempRow = AnsiReplaceStr(TempRow, "%COL2%", FormatFloat("0.00", IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency));
	  TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<a href=" ":internal:[ShowReceipt]:{" + IBInternalQuery->FieldByName("INVOICE_NUMBER")
		 ->AsString + "}" ">" + IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString + "</a>");
	  switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
	  {
	  case pttInvalid:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Unknown");
		 break;
      case pttRefund:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Refunded");
		 break;
	  case pttPurchased:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Purchased");
		 break;
	  case pttEarned:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Earned");
		 break;
	  case pttRedeemed:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Redeemed");
		 break;
          case pttRedeemedBD:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Redeemed Birthday");
		 break;
          case pttRedeemedFV:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Redeemed First Visit");
		 break;
	  case pttSync:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Sync with Server");
		 break;
          case pttBirthdayBonus:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Birthday Rewards");
		 break;
          case pttFirstVisit:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "First Visit");
		 break;
      case pttReset:
         TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Reset Points");
		 break;
	  default:
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", "Unknown");
		 break;
	  }
	  Report->Add(TempRow);
   }

   Temp = TABLE_STOP;
   Report->Add(Temp);
}

void TMembership::GetReportPointsTransactions(Database::TDBTransaction &DBTransaction, TStringList *Report)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	UnicodeString Temp = HTML_START;
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Points Transaction Report");
	Report->Add(Temp);

	Temp = TABLE_START;
	Report->Add(Temp);
	Temp = TABLE_ROW8_HEADER;
	Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points Transactions");
	Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " Last 30 Days ");
	Report->Add(Temp);



	AnsiString Headers[3][8];
	AnsiString Headers1[3][8] = { {
			"08%", "18%", "22%", "10%", "10%", "10%", "11%", "11%",
		}, {
			"center", "center", "center", "center", "center", "center", "center",
			"center",
		}, {
			"<b>Invoice</b>", "<b>Time</b>", "<b>Member</b>", "<b>Earn</b>",
			"<b>Redeem</b>", "<b>Purchase</b>", "<b>Earn Held</b>",
			"<b>Purch Held</b>",
		}
	};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {
			switch(i) {
			case 0:
				Headers[i][j] = "%SIZEC" + IntToStr(j + 1) + "%";
				break;
			case 1:
				Headers[i][j] = "%ALIGNC" + IntToStr(j + 1) + "%";
				break;
			case 2:
				Headers[i][j] = "%COL" + IntToStr(j + 1) + "%";
				break;
			}
		}
	}

	UnicodeString TempRow = TABLE_ROW8S;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			TempRow = AnsiReplaceStr(TempRow, Headers[i][j], Headers1[i][j]);

	Report->Add(TempRow);

	// Summary Points.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		 "select CONTACTS_KEY,ADJUSTMENT,TIME_STAMP,ADJUSTMENT_TYPE,EXPORTED_STATUS,INVOICE_NUMBER,TIME_STAMP_EXPORTED " " FROM POINTSTRANSACTIONS " " WHERE TIME_STAMP > :TIME_STAMP " " AND EXPORTED_STATUS != :CORRUPTED_EXPORTED_STATUS " " order by TIME_STAMP desc";
	IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 30;
	IBInternalQuery->ParamByName("CORRUPTED_EXPORTED_STATUS")
		 ->AsInteger = pesCorrupted;
	IBInternalQuery->ExecQuery();

	UnicodeString ProcessingInvoiceNumber = IBInternalQuery->FieldByName
		 ("INVOICE_NUMBER")->AsString;
	TDateTime TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
	int ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;

	Currency Points[8] = { 0 };

	for (; !IBInternalQuery->Eof; ) {

		if (IBInternalQuery->FieldByName("EXPORTED_STATUS")
			->AsInteger == pesExported) {
			switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger) {
			case pttPurchased:
				Points[2] += IBInternalQuery->FieldByName("ADJUSTMENT")
					 ->AsCurrency;
				break;
			case pttEarned:
				Points[0] += IBInternalQuery->FieldByName("ADJUSTMENT")
					 ->AsCurrency;
				break;
			case pttRedeemed:
				Points[1] += IBInternalQuery->FieldByName("ADJUSTMENT")
					 ->AsCurrency;
				break;
			}
		}
		else if (IBInternalQuery->FieldByName("EXPORTED_STATUS")
			->AsInteger == pesNone) {
			switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger) {
			case pttPurchased:
				Points[6] += IBInternalQuery->FieldByName("ADJUSTMENT")
					 ->AsCurrency;
				break;
			case pttEarned:
				Points[5] += IBInternalQuery->FieldByName("ADJUSTMENT")
					 ->AsCurrency;
				break;
			case pttRedeemed:
				Points[1] += IBInternalQuery->FieldByName("ADJUSTMENT")
					 ->AsCurrency;
				break;
			}
		}
		else {
			Points[7] += IBInternalQuery->FieldByName("ADJUSTMENT")
				 ->AsCurrency;
		}

		// Move on.
		IBInternalQuery->Next();
		// New Invoice or End of Table Write current Data out.
		if (IBInternalQuery->Eof || (IBInternalQuery->FieldByName
				("INVOICE_NUMBER")->AsString !=
				ProcessingInvoiceNumber)) {
			// Write it out.
			TempRow = TABLE_ROW8S;



		AnsiString Data[8] = {
		"<a href=" ":internal:[ShowReceipt]:{" + ProcessingInvoiceNumber + "}" ">" + ProcessingInvoiceNumber + "</a>",
		TimeStamp.FormatString("dd/mm/yy t"),
		GetContactName(DBTransaction, ContactKey),
		FormatFloat("0.00", Points[0]),
		FormatFloat("0.00", Points[1]),
		FormatFloat("0.00", Points[2]),
		FormatFloat("0.00", Points[5]),
		FormatFloat("0.00", Points[6])
		};


	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 8; j++)
			TempRow = AnsiReplaceStr(TempRow, Headers[i][j], Headers1[i][j]);


	for(int j = 0; j < 8; j++)
	{
		TempRow = AnsiReplaceStr(TempRow, Headers[2][j], Data[j]);
	}
			Report->Add(TempRow);

			// Reset Lines.
			ProcessingInvoiceNumber = IBInternalQuery->FieldByName
				 ("INVOICE_NUMBER")->AsString;
			ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			for(int j = 0; j < 8; j++)
				Points[j] = 0;
		}
	}
	Temp = TABLE_STOP;
	Report->Add(Temp);

	// Start the Member Table.
	Temp = HTML_BODY_STOP;
	Report->Add(Temp);
}

void TMembership::GetReportPointsTransactions(Database::TDBTransaction &DBTransaction, TStringList *Report, bool t)
{
// TODO 1 -o Michael -c Before Release : Testing Required

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	UnicodeString Temp = HTML_START;
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Points Transaction Report");
	Report->Add(Temp);

	Temp = TABLE_START;
	Report->Add(Temp);
	Temp = TABLE_ROW9_HEADER;
	Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points Transactions");
	Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " Last 30 Days ");
	Report->Add(Temp);

	std::map<int, InvoiceData> Key;
	std::map<int, InvoiceData>::iterator itKey;
	AnsiString Headers[3][9];
	AnsiString Headers1[3][9] = { {
			"08%", "20%", "24%", "8%", "8%", "8%", "8%","8%", "8%",
		}, {
			"center", "center", "center", "center", "center", "center", "center","center", "center",
		}, {
			"<b>Invoice</b>", "<b>Time</b>", "<b>Member</b>", "<b>Loyalty Earned</b>",
			"<b>Loyalty Purcahsed</b>","<b>Loyalty Redeemed</b>", "<b>Account Earned</b>","<b>Account Loaded</b>", "<b>Account Redeemed</b>",
		}
	};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			switch(i) {
			case 0:
				Headers[i][j] = "%SIZEC" + IntToStr(j + 1) + "%";
				break;
			case 1:
				Headers[i][j] = "%ALIGNC" + IntToStr(j + 1) + "%";
				break;
			case 2:
				Headers[i][j] = "%COL" + IntToStr(j + 1) + "%";
				break;
			}
		}
	}

	UnicodeString TempRow = TABLE_ROW9S;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 9; j++)
			TempRow = AnsiReplaceStr(TempRow, Headers[i][j], Headers1[i][j]);

	Report->Add(TempRow);

	// Summary Points.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		 "select CONTACTS_KEY,ADJUSTMENT,TIME_STAMP,ADJUSTMENT_TYPE,ADJUSTMENT_SUBTYPE,EXPORTED_STATUS,INVOICE_NUMBER,TIME_STAMP_EXPORTED " " FROM POINTSTRANSACTIONS " " WHERE TIME_STAMP > :TIME_STAMP " " AND EXPORTED_STATUS != :CORRUPTED_EXPORTED_STATUS " " order by TIME_STAMP desc";
	IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 30;
	IBInternalQuery->ParamByName("CORRUPTED_EXPORTED_STATUS")
		 ->AsInteger = pesCorrupted;
	IBInternalQuery->ExecQuery();

	int InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
	int ContactKey = 0;
	std::vector<TransData> Points;
	Points.resize(6);

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		if(IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger != InvoiceNumber)
		{
        	/* If the Invoice # Changes relaod the contact details.
            	The above If ensures This is skipped for the first record. */
			InvoiceData D(InvoiceNumber, GetContactName(DBTransaction, ContactKey), Points);
			Key.insert( std::pair<int, InvoiceData>(InvoiceNumber, D) );
			InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
			Points.clear();
			Points.resize(6);
		}
		ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		switch(IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger)
		{
			case ptstLoyalty:
                         {
                            switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                            {
                                case pttEarned:	  Points[0].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                                          Points[0].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                                    break;
                                case pttPurchased:  Points[1].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                                          Points[1].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                                    break;
                                case pttRedeemed:   Points[2].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                                          Points[2].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                                    break;
                            }
			    break;
                          }
			case ptstAccount:
                          {
                            switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                            {
                                case pttEarned:	  Points[3].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                                          Points[3].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                                    break;
                                case pttPurchased:  Points[4].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                                          Points[4].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                                    break;
                                case pttRedeemed:   Points[5].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                                          Points[5].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                                    break;
                            }
                            break;
                          }
		}
	}
	InvoiceData D(InvoiceNumber, GetContactName(DBTransaction, ContactKey), Points);
	Key.insert( std::pair<int, InvoiceData>(InvoiceNumber, D) );

	for(itKey = Key.begin(); itKey != Key.end(); itKey++)
	{
		AnsiString InvoiceNumber = IntToStr((*itKey).first);
		InvoiceData D = (*itKey).second;
		TempRow = TABLE_ROW9S;
		int i = 0;

		for(i; i < 6; i++)
			if(D.Transaction[i].Points != 0)
				break;

		AnsiString Data[9] =
        {
		"<a href=" ":internal:[ShowReceipt]:{" + InvoiceNumber + "}" ">" + InvoiceNumber + "</a>",
		D.Transaction[i].Time.FormatString("dd/mm/yy t"),
		D.Contact,
		FormatFloat("0.00", D.Transaction[0].Points),
		FormatFloat("0.00", D.Transaction[1].Points),
		FormatFloat("0.00", D.Transaction[2].Points),
		FormatFloat("0.00", D.Transaction[3].Points),
		FormatFloat("0.00", D.Transaction[4].Points),
		FormatFloat("0.00", D.Transaction[5].Points),
		};

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 9; j++)
				TempRow = AnsiReplaceStr(TempRow, Headers[i][j], Headers1[i][j]);

		for(int j = 0; j <9; j++)
			TempRow = AnsiReplaceStr(TempRow, Headers[2][j], Data[j]);

		Report->Add(TempRow);
	}
	Temp = TABLE_STOP;
	Report->Add(Temp);

	// Start the Member Table.
	Temp = HTML_BODY_STOP;
	Report->Add(Temp);
}

void TMembership::GetReportMemberAudit(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report,
   UnicodeString TerminalName)
{
// TODO 1 -o Michael -c Before Release : Testing Required
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   TDateTime PrevZedTime = Now() - 1;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT " "MAX(TIME_STAMP)TIME_STAMP FROM ZEDS " "WHERE " "TERMINAL_NAME = :TERMINAL_NAME";
   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TerminalName;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
	{
	  PrevZedTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
   }

   UnicodeString Temp = TABLE_START;
   Report->Add(Temp);
   Temp = TABLE_ROW7_HEADER;
   Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "<a href=" ":internal:[ShowReportPointsInformation]:{" + IntToStr(Member.ContactKey)
	  + "}" ">" + "Points Activity " + Member.Name + " (" + Member.MembershipNumber + ") " + "</a>");
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " Since Last Zed. ");
	Report->Add(Temp);

    /* Ok I figured out whats going on, if EnableSeperateEarntPts is enabled we have two different headers for the report.*/
	AnsiString Header[2][7] = { {"<b>Invoice</b>", "<b>Time</b>", "<b>&nbsp;</b>", "<b>&nbsp;</b>", "<b>Earned / Purchased</b>",
									"<b>Redeemed</b>", "<b>&nbsp;</b>"},
									 {"<b>Invoice</b>", "<b>Time</b>", "<b>&nbsp;</b>", "<b>Loyalty Earned</b>", "<b>Loyalty Redeemed</b>",
									  "<b>Account Loaded</b>", "<b>Account Redeemed</b>"} };

   bool EnableSeperateEarntPts = TGlobalSettings::Instance().EnableSeperateEarntPts;

   UnicodeString TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "12%");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "center");

	TempRow = AnsiReplaceStr(TempRow, "%COL1%", Header[EnableSeperateEarntPts][0]);
	TempRow = AnsiReplaceStr(TempRow, "%COL2%", Header[EnableSeperateEarntPts][1]);
	TempRow = AnsiReplaceStr(TempRow, "%COL3%", Header[EnableSeperateEarntPts][2]);
	TempRow = AnsiReplaceStr(TempRow, "%COL4%", Header[EnableSeperateEarntPts][3]);
	TempRow = AnsiReplaceStr(TempRow, "%COL5%", Header[EnableSeperateEarntPts][4]);
	TempRow = AnsiReplaceStr(TempRow, "%COL6%", Header[EnableSeperateEarntPts][5]);
	TempRow = AnsiReplaceStr(TempRow, "%COL7%", Header[EnableSeperateEarntPts][6]);

   Report->Add(TempRow);

   // Summary Points.
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select * "
	   " FROM POINTSTRANSACTIONS " " WHERE TIME_STAMP > :TIME_STAMP " " AND EXPORTED_STATUS != :CORRUPTED_EXPORTED_STATUS "
	   " AND CONTACTS_KEY = :CONTACTS_KEY " " order by TIME_STAMP ";
   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime > PrevZedTime;
   IBInternalQuery->ParamByName("CORRUPTED_EXPORTED_STATUS")->AsInteger = pesCorrupted;
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ExecQuery();


	int ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;


/*  These variables were not being used very confusing having them in the code.
	Currency TotalPoints = 0.00;
	Currency PointsEarned = 0.00;
	Currency PointsPurchased = 0.00;
	Currency PointsRedeemed = 0.00;
	Currency TotalPointsTampered = 0.00;*/
	int InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;

	/* Currency GrandTotal = 0, TotalEarned = 0, TotalRedeemed = 0;*/
	std::map<int, InvoiceData> Key;
	std::map<int, InvoiceData>::iterator itKey;
	std::vector<TransData> Points;
	Points.resize(6);

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		if(IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger != InvoiceNumber)
		{
			InvoiceData D(InvoiceNumber, GetContactName(DBTransaction, ContactKey), Points);
			Key.insert( std::pair<int, InvoiceData>(InvoiceNumber, D) );
			InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
			Points.clear();
			Points.resize(6);
		}
		ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		switch(IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger)
		{
			case ptstLoyalty:
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case pttEarned:	  Points[0].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[0].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttPurchased:  Points[1].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[1].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttRedeemed:   Points[2].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[2].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                }
				break;
            }
			case ptstAccount:
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case pttEarned:	  Points[3].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[3].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttPurchased:  Points[4].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[4].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttRedeemed:   Points[5].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[5].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                }
            	break;
            }
		}
	}
	InvoiceData Da(InvoiceNumber, GetContactName(DBTransaction, ContactKey), Points);
	Key.insert( std::pair<int, InvoiceData>(InvoiceNumber, Da) );

	Currency Totals[6] = { 0 };
	for(itKey = Key.begin(); itKey != Key.end(); itKey++)
	{
		InvoiceData D = (*itKey).second;
		int i = 0;
		for(i; i < 6; i++)
			if(D.Transaction[i].Points != 0)
				break;

		AnsiString Data[2][4] = {
			{"&nbsp;",
        	/*Earned */FormatFloat("0.00", D.Transaction[0].Points + D.Transaction[1].Points + D.Transaction[3].Points + D.Transaction[4].Points),
            /*Redeemed*/FormatFloat("0.00", D.Transaction[2].Points + D.Transaction[5].Points), "&nbsp;" },
			/*AU Loyalty Earned */{FormatFloat("0.00", D.Transaction[0].Points + D.Transaction[1].Points), // Loyalty Earned
            /*AU Loyalty Redeemed */FormatFloat("0.00", D.Transaction[2].Points),  // Loyalty Spent
            /*AU Account Earned/Purchased */FormatFloat("0.00",  D.Transaction[3].Points + D.Transaction[4].Points),  // Acc Loaded
            /*AU Loyalty Redeemed */FormatFloat("0.00", D.Transaction[5].Points) } // Acc Spent
		};


		TempRow = TABLE_ROW7S;
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "15%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "15%");
		TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "12%");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "left");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "left");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "right");
		TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "right");

		TempRow = AnsiReplaceStr(TempRow, "%COL1%",
		"<a href=" ":internal:[ShowReceipt]:{" + IntToStr((*itKey).first) + "}" ">" + IntToStr((*itKey).first) + "</a>");
		TempRow = AnsiReplaceStr(TempRow, "%COL2%", D.Transaction[i].Time.FormatString("dd/mm/yy t"));
		TempRow = AnsiReplaceStr(TempRow, "%COL3%", "&nbsp;");
		TempRow = AnsiReplaceStr(TempRow, "%COL4%", Data[EnableSeperateEarntPts][0]);
		TempRow = AnsiReplaceStr(TempRow, "%COL5%", Data[EnableSeperateEarntPts][1]);
		TempRow = AnsiReplaceStr(TempRow, "%COL6%", Data[EnableSeperateEarntPts][2]);
		TempRow = AnsiReplaceStr(TempRow, "%COL7%", Data[EnableSeperateEarntPts][3]);
		Report->Add(TempRow);

        // Loyalty Earned
		Totals[0] += D.Transaction[0].Points;
    	// Loyalty Purchased
		Totals[1] += D.Transaction[1].Points;
		// Loyalty Redeemed
		Totals[2] += D.Transaction[2].Points;
        // Account Earned
		Totals[3] += D.Transaction[3].Points;
        // Account Purchased
		Totals[4] += D.Transaction[4].Points;
        // Account Redeemed
		Totals[5] += D.Transaction[5].Points;
	}

		AnsiString Data[2][4] = {
			{"&nbsp;",
        	/*Earned */FormatFloat("0.00", Totals[0] + Totals[1] + Totals[3] + Totals[4]),
            /*Redeemed*/FormatFloat("0.00", Totals[2] + Totals[5]), "&nbsp;" },
			/*AU Loyalty Earned */{FormatFloat("0.00", Totals[0] + Totals[1]), // Loyalty Earned
            /*AU Loyalty Redeemed */FormatFloat("0.00", Totals[2]),  // Loyalty Spent
            /*AU Account Earned/Purchased */FormatFloat("0.00",  Totals[3] + Totals[4]),  // Acc Loaded
            /*AU Loyalty Redeemed */FormatFloat("0.00", Totals[5]) } // Acc Spent
		};

   TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "12%");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "left");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "left");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "right");

   TempRow = AnsiReplaceStr(TempRow, "%COL1%", "&nbsp;");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Total Balance</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL3%", "&nbsp;");
	TempRow = AnsiReplaceStr(TempRow, "%COL4%", Data[EnableSeperateEarntPts][0]);
	TempRow = AnsiReplaceStr(TempRow, "%COL5%", Data[EnableSeperateEarntPts][1]);
	TempRow = AnsiReplaceStr(TempRow, "%COL6%", Data[EnableSeperateEarntPts][2]);
	TempRow = AnsiReplaceStr(TempRow, "%COL7%", Data[EnableSeperateEarntPts][3]);
   Report->Add(TempRow);

   Temp = TABLE_STOP;
   Report->Add(Temp);

   // Start the Member Table.
   Temp = HTML_BODY_STOP;
   Report->Add(Temp);

	/* struct HtmlLine
   {
   TDateTime TimeStamp
   UnicodeString Invoice;
   UnicodeString Title;
   UnicodeString ValueIn;
   UnicodeString ValueOut;
   };

   bool SortPredicate(const HtmlLine& d1, const HtmlLine& d2)
   {
   return d1.TimeStamp < d2.TimeStamp;
   }

   std::vector<HtmlLine> Data;

   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   TDateTime PrevZedTime = Now() - 1;
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "SELECT "
   "MAX(TIME_STAMP)TIME_STAMP FROM ZEDS "
   "WHERE "
   "TERMINAL_NAME = :TERMINAL_NAME";
   IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
   {
   PrevZedTime = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
   }

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
   "select ArcBill.invoice_number,count(ITEM_NAME) QTY,ITEM_NAME,SIZE_NAME,SUM(PRICE + DISCOUNT)SPEND,"
   " POINTS_EARNED,REDEEMED,POINTS_PERCENT,DISCOUNT,ArcBill.TIME_STAMP "
   " from ARCHIVE "
   " left join ArcBill on ArcBill.Arcbill_key = archive.Arcbill_key "
   " where ARCHIVE.LOYALTY_KEY = :LOYALTY_KEY "
   " and ArcBill.TIME_STAMP > :TIME_STAMP "
   "group by ArcBill.invoice_number,ITEM_NAME,SIZE_NAME,PRICE,POINTS_EARNED,REDEEMED,POINTS_PERCENT,DISCOUNT,ArcBill.TIME_STAMP "
   "union "
   "select DayArcBill.invoice_number,count(ITEM_NAME) QTY,ITEM_NAME,SIZE_NAME,SUM(PRICE + DISCOUNT)SPEND, "
   " POINTS_EARNED,REDEEMED,POINTS_PERCENT,DISCOUNT,DayArcBill.TIME_STAMP "
   " from DAYARCHIVE "
   " left join DayArcBill on DayArcBill.Arcbill_key = DAYARCHIVE.Arcbill_key "
   " where DAYARCHIVE.LOYALTY_KEY = :LOYALTY_KEY "
   " and DayArcBill.TIME_STAMP > :TIME_STAMP "
   "group by DayArcBill.invoice_number,ITEM_NAME,SIZE_NAME,PRICE,POINTS_EARNED,REDEEMED,POINTS_PERCENT,DISCOUNT,DayArcBill.TIME_STAMP order by 1 desc";

   IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = PrevZedTime;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount)
   {
   // Add the Membership Block.
   UnicodeString TempRow = LoadStr(TABLE_START);
   Report->Add(TempRow);
   TempRow = TABLE_ROW7_HEADER;
   TempRow = AnsiReplaceStr(TempRow, "%TABLETITLE%", "Member Information");
   TempRow = AnsiReplaceStr(TempRow, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " ("+ Member.MembershipNumber + ")");
   Report->Add(TempRow);

   TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","30%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%","center");

   TempRow = AnsiReplaceStr(TempRow, "%COL1%","<b>Invoice</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Item</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL3%","<b>Total</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL4%","<b>Earned</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL5%","<b>Redeemed</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL6%","<b>Points %</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL7%","<b>Discount</b>");

   Report->Add(TempRow);

   Currency GrandTotal = 0,TotalEarned = 0 , TotalRedeemed = 0, TotalDiscount = 0;
   // Add the Membership Block.
   for (;!IBInternalQuery->Eof ;IBInternalQuery->Next())
   {
   //Write it out.
   TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","30%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","left");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%","right");

   TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<a href="":internal:[ShowReceipt]:{" + IBInternalQuery->FieldByName("invoice_number")->AsString + "}"">" + IBInternalQuery->FieldByName("invoice_number")->AsString + "</a>");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " +
   IBInternalQuery->FieldByName("SIZE_NAME")->AsString);
   TempRow = AnsiReplaceStr(TempRow, "%COL3%", FormatFloat("$0.00",IBInternalQuery->FieldByName("SPEND")->AsFloat));
   TempRow = AnsiReplaceStr(TempRow, "%COL4%", FormatFloat("$0.00",IBInternalQuery->FieldByName("POINTS_EARNED")->AsFloat));
   TempRow = AnsiReplaceStr(TempRow, "%COL5%", FormatFloat("$0.00",IBInternalQuery->FieldByName("REDEEMED")->AsFloat));
   TempRow = AnsiReplaceStr(TempRow, "%COL6%", FormatFloat("0.00%",IBInternalQuery->FieldByName("POINTS_PERCENT")->AsFloat));
   TempRow = AnsiReplaceStr(TempRow, "%COL7%", FormatFloat("$0.00",IBInternalQuery->FieldByName("DISCOUNT")->AsFloat));
   Report->Add(TempRow);
   GrandTotal += IBInternalQuery->FieldByName("SPEND")->AsFloat;
   TotalEarned += IBInternalQuery->FieldByName("POINTS_EARNED")->AsFloat;
   TotalRedeemed += IBInternalQuery->FieldByName("REDEEMED")->AsFloat;
   TotalDiscount += IBInternalQuery->FieldByName("DISCOUNT")->AsFloat;
   }

   TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%","15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%","30%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%","15%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%","10%");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%","center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%","left");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%","right");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%","right");

   TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Totals</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", "&nbsp;");
   TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<b>" + FormatFloat("$0.00",GrandTotal) + "</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>" + FormatFloat("$0.00",TotalEarned) + "</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL5%", "<b>" + FormatFloat("$0.00",TotalRedeemed) + "</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL6%", "&nbsp;");
	TempRow = AnsiReplaceStr(TempRow, "%COL7%", "<b>" + FormatFloat("$0.00",TotalDiscount) + "</b>");
	Report->Add(TempRow);

	TempRow = TABLE_STOP;
	Report->Add(TempRow);
	} */
}

void TMembership::GetReportPointsTransferred(Database::TDBTransaction &DBTransaction, TStringList *Report)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString Temp = HTML_START;
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Points Held & Transferred Report");
   Report->Add(Temp);

   Temp = TABLE_START;
	Report->Add(Temp);
   Temp = TABLE_ROW7_HEADER;
	Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points Held & Transferred");
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " Last 30 Days ");
	Report->Add(Temp);

	UnicodeString TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
	TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
	TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "10%");
	TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "22%");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "center");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "center");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "center");

	TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Invoice</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Time</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<b>Member</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>Earned</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL5%", "<b>Purchased</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL6%", "<b>Redeemed</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL7%", "<b>Transferred To Card</b>");

	Report->Add(TempRow);

	// Summary Points.
   IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	   "select CONTACTS_KEY,ADJUSTMENT,TIME_STAMP,ADJUSTMENT_TYPE,EXPORTED_STATUS,INVOICE_NUMBER,TIME_STAMP_EXPORTED "
		" FROM POINTSTRANSACTIONS " " WHERE TIME_STAMP > :TIME_STAMP " " AND EXPORTED_STATUS != :CORRUPTED_EXPORTED_STATUS "
	   " order by TIME_STAMP desc";
	IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 30;
   IBInternalQuery->ParamByName("CORRUPTED_EXPORTED_STATUS")->AsInteger = pesCorrupted;
	IBInternalQuery->ExecQuery();

	UnicodeString ProcessingInvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
   TDateTime TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
	TDateTime TimeStampExported = IBInternalQuery->FieldByName("TIME_STAMP_EXPORTED")->AsDateTime;
   UnicodeString TimeExported = "";
	int ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;

	Currency TotalPoints = 0.00;
   Currency PointsEarned = 0.00;
	Currency PointsPurchased = 0.00;
   Currency PointsRedeemed = 0.00;
	Currency TotalPointsTampered = 0.00;

	for (; !IBInternalQuery->Eof; )
   {

	  switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
	  {
	  case pttPurchased:
		 PointsPurchased += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
		 break;
	  case pttEarned:
		 PointsEarned += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
		 break;
	  case pttRedeemed:
		 PointsRedeemed += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
		 break;
	  }

	  if (IBInternalQuery->FieldByName("EXPORTED_STATUS")->AsInteger == pesExported)
	  {
		 TimeExported = TimeStampExported.FormatString("dd/mm/yy t");
	  }
	  else if (IBInternalQuery->FieldByName("EXPORTED_STATUS")->AsInteger == pesNone)
	  {
		 TimeExported = "Held @ Store";
	  }
	  else
	  {
		 TotalPointsTampered += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
	  }

	  // Move on.
	  IBInternalQuery->Next();
	  // New Invoice or End of Table Write current Data out.
	  if (IBInternalQuery->Eof || (IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString != ProcessingInvoiceNumber))
	  {
		 // Write it out.
		 TempRow = TABLE_ROW7S;
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "10%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "10%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "22%");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "left");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "left");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "right");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "right");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "right");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "left");

		 TempRow = AnsiReplaceStr(TempRow, "%COL1%",
			"<a href=" ":internal:[ShowReceipt]:{" + ProcessingInvoiceNumber + "}" ">" + ProcessingInvoiceNumber + "</a>");
		 TempRow = AnsiReplaceStr(TempRow, "%COL2%", TimeStamp.FormatString("dd/mm/yy t"));
		 TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<a href=" ":internal:[ShowReportPointsInformation]:{" + IntToStr(ContactKey)
			+ "}" ">" + GetContactName(DBTransaction, ContactKey) + "</a>");
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", FormatFloat("0.00", PointsEarned));
		 TempRow = AnsiReplaceStr(TempRow, "%COL5%", FormatFloat("0.00", PointsPurchased));
		 TempRow = AnsiReplaceStr(TempRow, "%COL6%", FormatFloat("0.00", PointsRedeemed));
		 TempRow = AnsiReplaceStr(TempRow, "%COL7%", TimeExported);
		 Report->Add(TempRow);

		 // Reset Lines.
		 ProcessingInvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
		 ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		 TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
		 TimeStampExported = IBInternalQuery->FieldByName("TIME_STAMP_EXPORTED")->AsDateTime;
		 PointsEarned = 0.00;
		 PointsRedeemed = 0.00;
		 PointsPurchased = 0.00;
		 TotalPointsTampered = 0.00;
	  }
	}
	Temp = TABLE_STOP;
	Report->Add(Temp);

	// Start the Member Table.
	Temp = HTML_BODY_STOP;
	Report->Add(Temp);
}

/* TODO 1 -o Michael -c Descrespancy : Why would you discriminate
void TMembership::GetReportPointsTransferred(Database::TDBTransaction &DBTransaction, TStringList *Report)
WITH
void TMembership::GetReportPointsTransferred(Database::TDBTransaction &DBTransaction, TStringList *Report, bool t)
By bassing one a Bool that is never USED????? AFAICT

Change the DAM function name!!!!

ONE formats a report for Points Transferred Report and the other for Points Held & Transferred Report.
These can and be merged back into the one function now.
*/

void TMembership::GetReportPointsTransferred(Database::TDBTransaction &DBTransaction, TStringList *Report, bool t)
{
    // TODO 1 -o Michael -c Before Release : Testing Required
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	UnicodeString Temp = HTML_START;
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Points Transferred Report");
	Report->Add(Temp);

	Temp = TABLE_START;
	Report->Add(Temp);
	Temp = TABLE_ROW7_HEADER;
	Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points Transferred");
	Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " Last 30 Days ");
	Report->Add(Temp);

	std::map<int, InvoiceData> Key;
	std::map<int, InvoiceData>::iterator itKey;
	AnsiString Headers[3][7];
	AnsiString Headers1[3][7] = { {
			"08%", "21%", "25%", "10%", "13%", "10%", "13%",
		}, {
			"center", "center", "center", "center", "center", "center", "center",
		}, {
			"<b>Invoice</b>", "<b>Time</b>", "<b>Member</b>", "<b>Loyalty Earned</b>",
			"<b>Loyalty Redeemed</b>", "<b>Account Loaded</b>", "<b>Account Redeemed</b>",
		}
	};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 7; j++) {
			switch(i) {
			case 0:
				Headers[i][j] = "%SIZEC" + IntToStr(j + 1) + "%";
				break;
			case 1:
				Headers[i][j] = "%ALIGNC" + IntToStr(j + 1) + "%";
				break;
			case 2:
				Headers[i][j] = "%COL" + IntToStr(j + 1) + "%";
				break;
			}
		}
	}

	UnicodeString TempRow = TABLE_ROW7S;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 7; j++)
			TempRow = AnsiReplaceStr(TempRow, Headers[i][j], Headers1[i][j]);

	Report->Add(TempRow);

	// Summary Points.
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
		 "select CONTACTS_KEY,ADJUSTMENT,TIME_STAMP,ADJUSTMENT_TYPE,ADJUSTMENT_SUBTYPE,EXPORTED_STATUS,INVOICE_NUMBER,TIME_STAMP_EXPORTED " " FROM POINTSTRANSACTIONS " " WHERE TIME_STAMP > :TIME_STAMP " " AND EXPORTED_STATUS != :CORRUPTED_EXPORTED_STATUS " " order by TIME_STAMP desc";
	IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 30;
	IBInternalQuery->ParamByName("CORRUPTED_EXPORTED_STATUS")
		 ->AsInteger = pesCorrupted;
	IBInternalQuery->ExecQuery();

	int InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
	int ContactKey = 0;
	std::vector<TransData> Points;
	Points.resize(6);

	for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
		if(IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger != InvoiceNumber)
		{
			InvoiceData D(InvoiceNumber, GetContactName(DBTransaction, ContactKey), Points);
			Key.insert( std::pair<int, InvoiceData>(InvoiceNumber, D) );
			InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsInteger;
			Points.clear();
			Points.resize(6);
		}
		ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		switch(IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger)
		{
			case ptstLoyalty:
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case pttEarned:	  Points[0].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[0].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttPurchased:  Points[1].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[1].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttRedeemed:   Points[2].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[2].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                }
				break;
            }
			case ptstAccount:
            {
                switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
                {
                    case pttEarned:	  Points[3].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[3].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttPurchased:  Points[4].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[4].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                    case pttRedeemed:   Points[5].Points += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
                                              Points[5].Time =  IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
                        break;
                }
            	break;
            }
		}
	}
	InvoiceData D(InvoiceNumber, GetContactName(DBTransaction, ContactKey), Points);
	Key.insert( std::pair<int, InvoiceData>(InvoiceNumber, D) );

	for(itKey = Key.begin(); itKey != Key.end(); itKey++)
	{
		AnsiString InvoiceNumber = IntToStr((*itKey).first);
		InvoiceData D = (*itKey).second;
		TempRow = TABLE_ROW7S;
		int i = 0;

		for(i; i < 6; i++)
			if(D.Transaction[i].Points != 0)
				break;

		AnsiString Data[7] = {
		"<a href=" ":internal:[ShowReceipt]:{" + InvoiceNumber + "}" ">" + InvoiceNumber + "</a>",
		D.Transaction[i].Time.FormatString("dd/mm/yy t"),
		D.Contact,
		FormatFloat("0.00", D.Transaction[0].Points + D.Transaction[1].Points),
		FormatFloat("0.00", D.Transaction[2].Points),
		FormatFloat("0.00", D.Transaction[3].Points + D.Transaction[4].Points),
		FormatFloat("0.00", D.Transaction[5].Points),
		};

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 7; j++)
				TempRow = AnsiReplaceStr(TempRow, Headers[i][j], Headers1[i][j]);

		for(int j = 0; j < 7; j++)
			TempRow = AnsiReplaceStr(TempRow, Headers[2][j], Data[j]);

		Report->Add(TempRow);
	}
	Temp = TABLE_STOP;
	Report->Add(Temp);

	// Start the Member Table.
	Temp = HTML_BODY_STOP;
	Report->Add(Temp);



/*
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   UnicodeString Temp = LoadStr(HTML_START);
	Temp = AnsiReplaceStr(Temp, "%TITLE%", "Points Held & Transferred Report");
   Report->Add(Temp);

   Temp = LoadStr(TABLE_START);
	Report->Add(Temp);
   Temp = TABLE_ROW7_HEADER;
	Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Points Held & Transferred");
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " Last 30 Days ");
	Report->Add(Temp);

	UnicodeString TempRow = TABLE_ROW7S;
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
	TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
	TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "10%");
	TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "10%");
   TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "22%");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "center");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "center");
   TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "center");
	TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "center");

	TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Invoice</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Time</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<b>Member</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>Earned</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL5%", "<b>Purchased</b>");
   TempRow = AnsiReplaceStr(TempRow, "%COL6%", "<b>Redeemed</b>");
	TempRow = AnsiReplaceStr(TempRow, "%COL7%", "<b>Transferred To Card</b>");

	Report->Add(TempRow);

	// Summary Points.
   IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	   "select CONTACTS_KEY,ADJUSTMENT,TIME_STAMP,ADJUSTMENT_TYPE,EXPORTED_STATUS,INVOICE_NUMBER,TIME_STAMP_EXPORTED "
		" FROM POINTSTRANSACTIONS " " WHERE TIME_STAMP > :TIME_STAMP " " AND EXPORTED_STATUS != :CORRUPTED_EXPORTED_STATUS "
	   " order by TIME_STAMP desc";
	IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 30;
   IBInternalQuery->ParamByName("CORRUPTED_EXPORTED_STATUS")->AsInteger = pesCorrupted;
	IBInternalQuery->ExecQuery();

	UnicodeString ProcessingInvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
   TDateTime TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
	TDateTime TimeStampExported = IBInternalQuery->FieldByName("TIME_STAMP_EXPORTED")->AsDateTime;
   UnicodeString TimeExported = "";
	int ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;

	Currency TotalPoints = 0.00;
   Currency PointsEarned = 0.00;
	Currency PointsPurchased = 0.00;
   Currency PointsRedeemed = 0.00;
	Currency TotalPointsTampered = 0.00;

	for (; !IBInternalQuery->Eof; )
   {

	  switch(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger)
	  {
	  case pttSystem:
		 break;
	  case pttReward:
		 break;
	  case pttPurchased:
		 PointsPurchased += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
		 break;
	  case pttEarned:
		 PointsEarned += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
		 break;
	  case pttRedeemed:
		 PointsRedeemed += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
		 break;
	  }

	  if (IBInternalQuery->FieldByName("EXPORTED_STATUS")->AsInteger == pesExported)
	  {
		 TimeExported = TimeStampExported.FormatString("dd/mm/yy t");
	  }
	  else if (IBInternalQuery->FieldByName("EXPORTED_STATUS")->AsInteger == pesNone)
	  {
		 TimeExported = "Held @ Store";
	  }
	  else
	  {
		 TotalPointsTampered += IBInternalQuery->FieldByName("ADJUSTMENT")->AsCurrency;
	  }

	  // Move on.
	  IBInternalQuery->Next();
	  // New Invoice or End of Table Write current Data out.
	  if (IBInternalQuery->Eof || (IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString != ProcessingInvoiceNumber))
	  {
		 // Write it out.
		 TempRow = TABLE_ROW7S;
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "08%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "18%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "22%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "10%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "10%");
		 TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "22%");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "left");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "left");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "right");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "right");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "right");
		 TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "left");

		 TempRow = AnsiReplaceStr(TempRow, "%COL1%",
			"<a href=" ":internal:[ShowReceipt]:{" + ProcessingInvoiceNumber + "}" ">" + ProcessingInvoiceNumber + "</a>");
		 TempRow = AnsiReplaceStr(TempRow, "%COL2%", TimeStamp.FormatString("dd/mm/yy t"));
		 TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<a href=" ":internal:[ShowReportPointsInformation]:{" + IntToStr(ContactKey)
			+ "}" ">" + GetContactName(DBTransaction, ContactKey) + "</a>");
		 TempRow = AnsiReplaceStr(TempRow, "%COL4%", FormatFloat("0.00", PointsEarned));
		 TempRow = AnsiReplaceStr(TempRow, "%COL5%", FormatFloat("0.00", PointsPurchased));
		 TempRow = AnsiReplaceStr(TempRow, "%COL6%", FormatFloat("0.00", PointsRedeemed));
		 TempRow = AnsiReplaceStr(TempRow, "%COL7%", TimeExported);
		 Report->Add(TempRow);

		 // Reset Lines.
		 ProcessingInvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
		 ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		 TimeStamp = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
		 TimeStampExported = IBInternalQuery->FieldByName("TIME_STAMP_EXPORTED")->AsDateTime;
		 PointsEarned = 0.00;
		 PointsRedeemed = 0.00;
		 PointsPurchased = 0.00;
		 TotalPointsTampered = 0.00;
	  }
	}
	Temp = TABLE_STOP;
	Report->Add(Temp);

	// Start the Member Table.
	Temp = LoadStr(HTML_BODY_STOP);
	Report->Add(Temp);   */
}

void TMembership::GetReportMemberPurchaseInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select TIME_STAMP,count(ITEM_NAME) QTY,ITEM_NAME,SIZE_NAME,SUM((PRICE*QTY) + DISCOUNT)SPEND from ARCHIVE where LOYALTY_KEY = :LOYALTY_KEY "
	   " and TIME_STAMP > :TIME_STAMP " "group by TIME_STAMP,ITEM_NAME,SIZE_NAME " "union "
	   "select TIME_STAMP,count(ITEM_NAME) QTY,ITEM_NAME,SIZE_NAME,SUM((PRICE*QTY) + DISCOUNT)SPEND from DAYARCHIVE where LOYALTY_KEY = :LOYALTY_KEY "
	   " and TIME_STAMP > :TIME_STAMP " "group by TIME_STAMP,ITEM_NAME,SIZE_NAME,PRICE order by 1 desc";

   IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now() - 365;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
   {
	  // Add the Membership Block.
	  UnicodeString Temp = TABLE_START;
	  Report->Add(Temp);
	  Temp = TABLE_ROW6_HEADER;
	  Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Member Information");
	  Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " (" + Member.MembershipNumber + ")");
	  Report->Add(Temp);

	  // Add the Membership Block.
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 UnicodeString TempRow = TABLE_ROW6;
		 TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Date :");
		 TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime.FormatString
			("dd-mm-yy hh-nn am/pm"));
		 TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Item :");
		 TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%",
			IBInternalQuery->FieldByName("ITEM_NAME")->AsString + " " + IBInternalQuery->FieldByName("SIZE_NAME")->AsString);
		 TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE3%", "Spend :");
		 TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT3%", FormatFloat("0.00", IBInternalQuery->FieldByName("SPEND")->AsFloat));
		 Report->Add(TempRow);
	  }

	  Temp = TABLE_STOP;
	  Report->Add(Temp);
   }
}

void TMembership::GetReportMemberStop(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   // Start the Member Table.
   UnicodeString Temp = HTML_BODY_STOP;
   Report->Add(Temp);
}

void TMembership::GetReportStop(Database::TDBTransaction &DBTransaction, TStringList *Report)
{
   // Start the Member Table.
   UnicodeString Temp = HTML_BODY_STOP;
   Report->Add(Temp);
}

TDateTime TMembership::GetLastVisit(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   TDateTime RetVal = Now();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "select LAST_VISIT from CONTACTS where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
   IBInternalQuery->ExecQuery();
   RetVal = IBInternalQuery->FieldByName("LAST_VISIT")->AsDateTime;
   return RetVal;
}

TDateTime TMembership::GetDateOfBirth(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   TDateTime RetVal = double(0);
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "select DATEOFBIRTH from CONTACTS where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
   IBInternalQuery->ExecQuery();
   RetVal = IBInternalQuery->FieldByName("DATEOFBIRTH")->AsDateTime;
   return RetVal;
}

TDateTime TMembership::GetLastBirthdayProcessed(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   TDateTime RetVal = Now();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "select LAST_BIRTHDAY_PROCESSED from CONTACTS where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
   IBInternalQuery->ExecQuery();
   RetVal = IBInternalQuery->FieldByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime;
   return RetVal;
}

void TMembership::SetLastVisit(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member)
{
   Member.LastVisit = Now();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	  "UPDATE CONTACTS SET LAST_VISIT = :LAST_VISIT, LAST_MODIFIED = :LAST_MODIFIED where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
	IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Member.LastVisit;
   IBInternalQuery->ParamByName("LAST_VISIT")->AsDateTime = Member.LastVisit;
   IBInternalQuery->ExecQuery();
}

void TMembership::SetLastBirthdayProcessed(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member)
{
   Member.LastBirthdayProcessed = Now();
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	  "UPDATE CONTACTS SET LAST_BIRTHDAY_PROCESSED = :LAST_BIRTHDAY_PROCESSED, LAST_MODIFIED = :LAST_MODIFIED where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Member.LastBirthdayProcessed;
   IBInternalQuery->ParamByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime = Member.LastBirthdayProcessed;
   IBInternalQuery->ExecQuery();
}

void TMembership::ClearLastVisitTimes(Database::TDBTransaction &DBTransaction)
{
   TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBQuery->Close();
   IBQuery->SQL->Text = "UPDATE CONTACTS SET LAST_VISIT = :LAST_VISIT, LAST_MODIFIED = :LAST_MODIFIED;";
   IBQuery->ParamByName("LAST_VISIT")->AsDateTime = 0;
	IBQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Now();
   IBQuery->ExecQuery();
}

void TMembership::ResetLastVisitTimes(Database::TDBTransaction &DBTransaction)
{
   TIBSQL *IBQuery = DBTransaction.Query(DBTransaction.AddQuery());
   TIBSQL *IBQuery1 = DBTransaction.Query(DBTransaction.AddQuery());

   IBQuery->Close();
   IBQuery->SQL->Text =
	   "select loyalty_key from ARCHIVE where LOYALTY_KEY != 0 and LOYALTY_KEY is not null group by LOYALTY_KEY";
   IBQuery->ExecQuery();
   if (IBQuery->RecordCount != 0)
   {
	  for (; !IBQuery->Eof; IBQuery->Next())
	  {
		 IBQuery1->Close();
		 IBQuery1->SQL->Text = "UPDATE CONTACTS SET LAST_VISIT = '30.12.1899, 00:00:00.000', LAST_MODIFIED = :LAST_MODIFIED WHERE CONTACTS_KEY = :CONTACTS_KEY";
		 IBQuery1->ParamByName("CONTACTS_KEY")->AsString = IBQuery->FieldByName("LOYALTY_KEY")->AsString;
       IBQuery1->ParamByName("LAST_MODIFIED")->AsDateTime = Now();
		 IBQuery1->ExecQuery();
	  }
   }
}

void TMembership::GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, std::map <UnicodeString,
   UnicodeString> &Data)
{

}

bool TMembership::ValidateVisit(TMMContactInfo &UserInfo)
{
   bool NewVisistOnNewDay = false;
   TDate Date5AM = Now().CurrentTime();
   if (Time() < StrToTime("05:00:00"))
   {
	  Date5AM = Date() - 1;
   }
   else
   {
	  Date5AM = Date();
   }

   if (DateOf(UserInfo.LastVisit) != Date())
   {
	  NewVisistOnNewDay = true;
   }
   return NewVisistOnNewDay;
}

void TMembership::SetVisitCount(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
}

TLoginSuccess TMembership::Login(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{

}

void TMembership::GetReportMemberOnPremisesStart(Database::TDBTransaction &DBTransaction, TStringList *Report)
{
   // Start the Member Table.
   UnicodeString Temp = HTML_START;
   Temp = AnsiReplaceStr(Temp, "%TITLE%", "Members On Premises Report");
   Report->Add(Temp);
}

void TMembership::GetReportMemberOnPremisesInfo(Database::TDBTransaction &DBTransaction, TStringList *Report)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select MEMBER_NUMBER,NAME,LAST_VISIT from CONTACTS where LAST_VISIT > :LAST_VISIT ORDER BY LAST_VISIT";
   IBInternalQuery->ParamByName("LAST_VISIT")->AsDateTime = Now() - 1;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
   {
	  // Add the Membership Block.
	  UnicodeString Temp = TABLE_START;
	  Report->Add(Temp);
	  Temp = TABLE_ROW7_HEADER;
	  Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Members On Premises (24 hr)");
	  Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", "");
	  Report->Add(Temp);
 		UnicodeString TempRow = TABLE_ROW7S;
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "15%");
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "15%");
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "15%");
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "15%");
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "15%");
        TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "15%");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "center");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "center");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "center");
        TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "center");

        TempRow = AnsiReplaceStr(TempRow, "%COL1%", "<b>Name</b>");
        TempRow = AnsiReplaceStr(TempRow, "%COL2%", "<b>Number</b>");
        TempRow = AnsiReplaceStr(TempRow, "%COL3%", "<b>Last Visit (24hr)</b>");
        TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>&nbsp;</b>");
        TempRow = AnsiReplaceStr(TempRow, "%COL5%", "<b>Name</b>");
        TempRow = AnsiReplaceStr(TempRow, "%COL6%", "<b>Number</b>");
        TempRow = AnsiReplaceStr(TempRow, "%COL7%", "<b>Last Visit (24hr)</b>");
        Report->Add(TempRow);
	  // Add the Membership Block.
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
      	   UnicodeString TempRow = TABLE_ROW7S;
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC1%", "15%");
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC2%", "15%");
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC3%", "15%");
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC4%", "10%");
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC5%", "15%");
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC6%", "15%");
           TempRow = AnsiReplaceStr(TempRow, "%SIZEC7%", "15%");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC1%", "center");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC2%", "center");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC3%", "center");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC4%", "center");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC5%", "center");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC6%", "center");
           TempRow = AnsiReplaceStr(TempRow, "%ALIGNC7%", "center");

            TempRow = AnsiReplaceStr(TempRow, "%COL1%", IBInternalQuery->FieldByName("NAME")->AsString);
            TempRow = AnsiReplaceStr(TempRow, "%COL2%", IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString);
            TempRow = AnsiReplaceStr(TempRow, "%COL3%", IBInternalQuery->FieldByName("LAST_VISIT")->AsDateTime.FormatString("dd/mm HH:nn"));
            TempRow = AnsiReplaceStr(TempRow, "%COL4%", "<b>&nbsp;</b>");
			IBInternalQuery->Next();
            if(!IBInternalQuery->Eof)
            {

                TempRow = AnsiReplaceStr(TempRow, "%COL5%", IBInternalQuery->FieldByName("NAME")->AsString);
                TempRow = AnsiReplaceStr(TempRow, "%COL6%", IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString);
                TempRow = AnsiReplaceStr(TempRow, "%COL7%", IBInternalQuery->FieldByName("LAST_VISIT")->AsDateTime.FormatString("dd/mm HH:nn"));
            }
            else
            {
            	TempRow = AnsiReplaceStr(TempRow, "%COL5%", "<b>&nbsp;</b>");
            	TempRow = AnsiReplaceStr(TempRow, "%COL6%", "<b>&nbsp;</b>");
            	TempRow = AnsiReplaceStr(TempRow, "%COL7%", "<b>&nbsp;</b>");
            }
           Report->Add(TempRow);
  	  }

	  Temp = TABLE_STOP;
	  Report->Add(Temp);
   }
}

void TMembership::FastContactListing(TIBSQL **query,
                                     Database::TDBTransaction &transaction,
                                     int contact_type,
                                     int search_type,
                                     bool with_account_profile_only)
{
	TIBSQL *populated_query;

	AnsiString order_clause[3] = {
		"order by name ",
		"order by member_number ",
		"order by phone, name "
	};

	AnsiString base_clause[3] = {
		"select name n, last_name l, "
      "       contacts_key k "
		"       from contacts "
	   "       where contact_type = :contact_type and member_type <> 2 ",
		"select name || ' ' ||  last_name n,member_number l, "
      "       contacts_key k "
		"       from contacts "
	   "       where contact_type = :contact_type and member_type <> 2 ",
		"select case when (phone is not null) then "
		"                 name || ' ' || last_name "
		"            else name "
		"       end n, "
        "       case when (phone = '') then MOBILE  else phone end l , "
      "       contacts_key k "
		"       from contacts "
	   "       where contact_type = :contact_type and member_type <> 2 ",
	};

	populated_query =
	  (*query = transaction.Query(transaction.AddQuery()));

	transaction.StartTransaction();

	populated_query->Close();
	populated_query->SQL->Text = base_clause[search_type];
	if (with_account_profile_only)
		populated_query->SQL->Text =
		  populated_query->SQL->Text + " and account_profile <> 0 ";
	populated_query->SQL->Text =
	  populated_query->SQL->Text + order_clause[search_type];
   populated_query->ParamByName("contact_type")->AsInteger = contact_type;
	populated_query->ExecQuery();
}

const char *TMembership::FastFormatMemberName(AnsiString mbr_name)
{
	char *strbeg = mbr_name.c_str();
	char *strend = strbeg + mbr_name.Length();
	char *strpos = strbeg;

	*strpos = toupper(*strpos);
   for (strpos++ ; strpos < strend; strpos++) {
		for ( ; *strpos - 0x21 > 0; strpos++)
			*strpos = tolower(*strpos);
		*(strpos += *strpos == 0x20) = toupper(*strpos);
	}

	return strbeg;
}

InvoiceData::InvoiceData(int inInvoiceNumber, AnsiString inContact, std::vector<TransData> inTransaction):
InvoiceNumber(inInvoiceNumber), Contact(inContact), Transaction(inTransaction)
{
}

/* Membership systems can overide this to implement some kind of Auth on Points transactions
If this is not accepted a dilog should be shown to say why.*/
TLoginSuccess TMembership::Authorise(TMMContactInfo &UserInfo,Currency wrkPayAmount)
{
	return lsAccepted;
}

/* This function only checks if the input is a card and does't do any member checking.
This may want to be changed in future.
This is a hack to get members with keyboard and barcode scanners working again.
This should be cleaned up as part of tech debt. */
TLoginSuccess TMembership::IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
	TLoginSuccess LoginResult = FindMember(DBTransaction, UserInfo);
	if (LoginResult == lsAccountBlocked)
	{
		LoginResult = lsAccepted;
    }
	return LoginResult;
}

/* Called Just before a Transaction with Membership applied is closed.
This allows the Membership system to make any final changes as transaction is closed.
*/
void TMembership::TransactionClosed(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{

}

bool TMembership::MembershipApplied(Database::TDBTransaction &DBTransaction,__int64 MemberKey)
{
    bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->SQL->Text =
			"SELECT "
			"ORDER_KEY "
			"FROM "
			"ORDERS "
			"WHERE "
			"LOYALTY_KEY = :LOYALTY_KEY";
		IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = MemberKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return RetVal;
}

bool TMembership::CheckForBirthdayBenifit(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{
   TDateTime DateOfBirth = GetDateOfBirth(DBTransaction,Member.ContactKey);
   if((double)DateOfBirth <= 0)
     return false;

   TDateTime LastBirthDayProcessed  = GetLastBirthdayProcessed(DBTransaction,Member.ContactKey);
   TDateTime currentDate = Today();
   TDateTime bithDate =  RecodeYear(DateOfBirth, YearOf(Now()));
   if((double)LastBirthDayProcessed <= 0 && IsSameDay(bithDate, Today()))
      return true;
   //Check that birthday benefit for current year has been redeemed or not
   if(YearOf(LastBirthDayProcessed) ==  YearOf(currentDate))
     {
       return false;
     }
   //Compare birth date with current date
   int result =  Dateutils::CompareDate(bithDate,currentDate);
   if(result == EqualsValue)
     {
        return true;
     }
   else //if(result == GreaterThanValue)
    {
       return DaySpan(bithDate,currentDate) <= TGlobalSettings::Instance().BirthdayBuffer;
    }
   return false;
}

Currency TMembership::GetEarnedPointsInDateRange(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member,TDateTime startDate,TDateTime endDate)
{
   Currency earnedPoints = 0;
   try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT SUM(ADJUSTMENT) EARNED_POINTS FROM POINTSTRANSACTIONS "
                                     "WHERE TIME_STAMP >= :START_TIME AND "
                                     "TIME_STAMP <= :END_TIME AND "
                                     "ADJUSTMENT_TYPE = 2 AND CONTACTS_KEY = :CONTACTS_KEY";
        IBInternalQuery->ParamByName("START_TIME")->AsDateTime = StartOfTheDay(startDate);
        IBInternalQuery->ParamByName("END_TIME")->AsDateTime = EndOfTheDay(endDate);
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
        IBInternalQuery->ExecQuery();
        if (!IBInternalQuery->Eof)
        {
            earnedPoints = IBInternalQuery->FieldByName("EARNED_POINTS")->AsCurrency;
        }
    }
   catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
    return earnedPoints;
}

bool TMembership::DisplayBirthDayNotification(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{
   if(TGlobalSettings::Instance().EnableBirthdayNotifications && Member.MemberType == 1 &&
        CheckForBirthdayBenifit(DBTransaction,Member))
        {
            AnsiString birthDate = FormatDateTime("dd mmmm",Member.DateOfBirth) ;
            MessageBox(Member.Name + "'s Birthday" + "\n" + birthDate, "Happy Birthday", MB_OK + MB_ICONINFORMATION);
            return true;
         }
   return false;
}

bool TMembership::DisplayFirstVisitNotification(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{
  bool canEarnPoints = (TGlobalSettings::Instance().LoyaltyMateEnabled && !TGlobalSettings::Instance().IsPOSOffline)
                        || !TGlobalSettings::Instance().LoyaltyMateEnabled;
  if (canEarnPoints && TGlobalSettings::Instance().AcknowledgeFirstVisit && Member.MemberType == 1 &&
     !TDBContacts::IsFirstVisitRewarded(DBTransaction, Member.ContactKey) )
    {
            MessageBox(Member.Name + "'s First Visit", "First Visit", MB_OK + MB_ICONINFORMATION);
            return true;
    }
  return false;
 }

double TMembership::GetEarnedPointsForCurrentYear(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
 {
   double earnedPoints  = 0;
   TDateTime activationDate = Member.ActivationDate;
   TDateTime startDate = RecodeYear(activationDate, YearOf(Now()));
   int result =  Dateutils::CompareDate(startDate,Now());
    if(result == 1)
    {
            startDate = RecodeYear(activationDate, YearOf(Now()) - 1) ;
    }
   TDateTime endDate = Now();
   if(TGlobalSettings::Instance().LoyaltyMateEnabled)
     {
        earnedPoints = CurrentYearPoints;
     }
     else
     {
        earnedPoints =  GetEarnedPointsInDateRange(DBTransaction,Member,startDate,endDate);
        CurrentYearPoints = earnedPoints;
     }
    return earnedPoints;
 }

 AnsiString TMembership::SendRequestForPay(std::vector<TTenderDetails> tenderDetailsList,std::vector<TItemDetailsThor> itemsList)
{}
AnsiString TMembership::SendRequestForRefund(TRefundTransaction refundTransaction)
{}
bool TMembership::CallInitMethod()
{}

bool TMembership::LoyaltyMemberSelected(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode,bool triggeredByCard)
{
  return LoyaltyMemberSelected(DBTransaction, UserInfo, memberCardCode,triggeredByCard);
}

bool TMembership::UpdateMemberCardCode(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo,AnsiString memberCardCode)
{
  return UpdateMemberCardCode(DBTransaction, UserInfo,memberCardCode);
}


void TMembership::ResetPoints()
{
   CurrentYearPoints = 0;
   PreviousYearPoints = 0;
   AvailableBDPoint = 0;
   AvailableFVPoint = 0;
   RedeemedVoucherName = "";
   RedeemedVoucherDiscount = "";
   MemberVouchers.clear();
}

