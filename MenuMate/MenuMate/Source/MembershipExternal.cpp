#pragma hdrstop

#include "MembershipExternal.h"
#include <set>
#include <Memory>
#include <Sysutils.hpp>
#include "MMLogging.h"
#include <DateUtils.hpp>
#include "Rounding.h"
#include "MMContactInfo.h"
#include "MembershipExternalXML.h"
#include "StringTableRes.h"
#include "DBContacts.h"
#pragma package(smart_init)


TMembershipExternal::TMembershipExternal(TModules &Modules,
	AnsiString inIP, int inPort, int inTillID) : TMembership(Modules),
MembershipGeneralLedgerTCP(new TMembershipGeneralLedgerTCP()) {
	ContactType = eExternalMember;
	ContactTabType = TabMember;
	ReadOnlyInterface = true;
	Modules.Status[eRegMembers]["ReadOnly"] = ReadOnlyInterface;
	Enabled = Modules.Status[eRegMembers]["Enabled"];
	IP = inIP;
	Port = inPort;
	TillID = inTillID;
}

TMembershipExternal::~TMembershipExternal() {
}

void TMembershipExternal::Initialise
(Database::TDBTransaction &DBTransaction)
{
	TMembership::Initialise(DBTransaction);
}

void TMembershipExternal::LoyaltyAddValue
(Database::TDBTransaction &DBTransaction,
	TPointsTransaction &PointsTransaction) {

/* Point transactions are not processed though this interface.
Instead they use the MangerExternalMembership Class that processes the Points
when it processes the entire Payment Transaction. This allows it to fail
on other payment types not just points. without haveing to worry about revsersing
any particular payment type. */
	return;
}

Currency TMembershipExternal::LoyaltyGetValue
		(Database::TDBTransaction &DBTransaction, int InternalKey)
{
	if (!fEnabled)
		return 0;
	Currency RetVal = 0;
	try {
		int ExternalKey = GetExternalKey(DBTransaction, InternalKey);
		RetVal = LoyaltyGetExternalValue(DBTransaction, ExternalKey);
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

Currency TMembershipExternal::LoyaltyGetExternalValue
		(Database::TDBTransaction &DBTransaction, int in3rdPartyKey) {
	if (!fEnabled)
		return 0;

	Currency RetVal = 0;
	try {
		// TODO: Recover the Member number from the DB.
		// Query the transaction Layer for the Value.

	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

TLoginSuccess TMembershipExternal::TestCardLogin
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	if (!fEnabled)
		return lsUserNotFound;

	TLoginSuccess Success = lsUserNotFound;
	try {
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
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return Success;
}

/* you can only log in via Card this should have already happened beforethis function call.
 */
TLoginSuccess TMembershipExternal::TestKeyLogin
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
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

TContactType TMembershipExternal::GetContactType(int ContactKey) {
	if (!fEnabled)
		return eContactTypeUnknown;
	TContactType RetVal = eExternalMember;
	return RetVal;
}

TLoginSuccess TMembershipExternal::GetSubsPaid
		(Database::TDBTransaction &DBTransaction, int InternalKey) {
	return lsAccepted;
}

bool TMembershipExternal::DeleteContact(int inContactKey) {
	return false;
}

void TMembershipExternal::GetContactDetails(Database::TDBTransaction &DBTransaction,
	int InternalKey,
	TMMContactInfo &Info)
{
	if (!fEnabled)
		return;
	try
    {
		TMembership::GetContactDetails(DBTransaction, Info.ContactKey, Info);
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

/**

 */
void TMembershipExternal::SetContactDetails
		(Database::TDBTransaction &DBTransaction, int InternalKey,
	TMMContactInfo &Info) {
	if (!fEnabled)
		return;
	try {
		TContact::SetContactDetails(DBTransaction, InternalKey, Info);
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipExternal::GetDiscountDetails(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;

	try
    {
        Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
        DBTransaction.RegisterQuery(IBInternalQuery);

        if(Info.DiscountGroup != -1)
        {
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
            " SELECT DISCOUNT_KEY"
            " FROM "
            "  DISCOUNTS"
            " WHERE"
            " DISCOUNT_GROUP = :DISCOUNT_GROUP ORDER BY PRIORITY";
            IBInternalQuery->ParamByName("DISCOUNT_GROUP")->AsInteger = Info.DiscountGroup;
            IBInternalQuery->ExecQuery();
            if(IBInternalQuery->RecordCount)
            {
                for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
                {
                    Info.AutoAppliedDiscounts.insert(IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger);
                }
            }
        }
	}
	catch(Exception & E)
    {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

// Strips the SiteID off the card.
int TMembershipExternal::GetSiteID(AnsiString Card)
{
	AnsiString SiteID = Card.SubString(2,3);
	//Chch Casino Default Site ID.
	return StrToIntDef(SiteID,287);
}

/** *
This fuction returns the internal contacts key.
It is responible for retrieving the Members details.
 */

void TMembershipExternal::GetExternalContactDetails
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info) {
	if (!fEnabled)
		return;
	bool UpdateLocalInfo = false;
	try {
		if (Info.CardStr != "")
		{
			std::auto_ptr<TMSXMLEnquiry> request(new TMSXMLEnquiry());
			request->SetTransNo(1);
			request->SetTerminalID(TillID);
			request->SetTerminalName("MenuMate");
			request->SetCard(Info.CardStr);
/*#ifdef _DEBUG
			request->SetCard("%2872000161906002?");
			request->Build();
			request->SaveToFile();
#endif*/
			MembershipGeneralLedgerTCP->Process(IP,Port,*request.get());
			if (request->Result == eMSAccepted)
			{
					Info.SiteID = GetSiteID(Info.CardStr);
					Info.Name = request->MemberName;
					Info.MembershipNumber = request->AccountNo;
					Info.ExternalKey = StrToIntDef(Info.MembershipNumber, 0);
					// Info.DateOfBirth = MembershipGeneralLedgerTCP->InPacket.MemberDOB();
					Info.CardsToAdd.insert(Info.CardStr);

                    Info.Points.Clear();
					double int64Points = request->PointsBalance;
                    TPointsTypePair typepair(pttEarned,ptstLoyalty);
                    TPointsType type(pasDatabase, typepair,pesExported);
                    Info.Points.Load(type,Currency(int64Points) / Currency(100));

                    Info.AccountInfo = request->Type;
                    Info.Note = request->Type;
                    Info.DiscountGroup = request->MembersGroup;

					/* Comp Balance not shown on display at customer request.
                    	Member Type shown instead.
                    int64Points = request->CompBalance;
					Info.Note = "Comp Balance :" + FormatFloat("$0.00",	Currency(int64Points) / Currency(100));
                    */

                    Info.TabEnabled = true;
					UpdateLocalInfo = true;

					Info.Points.PointsRules << eprNoPointsPurchases;
					Info.Points.PointsRules >> eprAllowedNegitive;
                    GetDiscountDetails(DBTransaction,Info);
			}
			else
			{
				throw Exception("Please contact Players Club\r" + request->ResultText);
			}
		}
		else
		{
			throw Exception("Unable to lookup member, please use there card. ");
		}
		// Update the Member in our Database.
		if (UpdateLocalInfo)
        {
			Info.ContactKey = GetInternalKey(DBTransaction, Info.ExternalKey);
            // Preserve the Contacts PIN for Pre Auth.
			Info.PIN = TDBContacts::GetPIN(DBTransaction, Info.ContactKey);
            TDBContacts::DeleteContactCards(DBTransaction, Info.ContactKey);
            Info.LastVisit = Now();
			SetContactDetails(DBTransaction, Info.ContactKey, Info);
			SyncPointsDetials(DBTransaction, Info);
            /* Points are all synced now discard the Bally ones.
         	they will be reloaded as part of Get contact details*/
         	Info.Points.Clear();
        	Info.Points.PointsRules << eprNoPointsPurchases;
			Info.Points.PointsRules >> eprAllowedNegitive;
		}
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

int TMembershipExternal::GetContactByCard
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
	if (!fEnabled)
		return 0;

	try {
		// Info.CardStr = ";9572000001=?";
		int Track2Start = Info.CardStr.Pos(";") + 1;
		int Track2Delim = Info.CardStr.Pos("=");
		int Track2End = Info.CardStr.Pos("?");
		if (Track2Start != 1 && Track2Delim > 1)
        {
			Info.CardStr = Info.CardStr.SubString(Track2Start,
				Track2Delim - Track2Start);
		}
		else if (Track2Start != 1 && Track2End > 1)
        {
			Info.CardStr = Info.CardStr.SubString(Track2Start,
				Track2End - Track2Start);
		}

		GetExternalContactDetails(DBTransaction, Info);
		Info.ContactKey = GetInternalKey(DBTransaction, Info.ExternalKey);
		// Get or create the member locally / update there info and return the lcoal key.
	}
	catch(Exception &E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return Info.ContactKey;
}

/* Checks to see if the member is within the allowed time frame for lookups with out card
If so will use the card stored in the DB to look up the member.*/
bool TMembershipExternal::GetExternalContactDetialsFromKey(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
    bool RetVal = false;
    const int Hours = 12;
	if(Info.ContactKey != 0)
    {
        TDateTime LastVisit = TMembership::GetLastVisit(DBTransaction, Info.ContactKey);
        if(LastVisit > Now() - (1.0 / 24.0 * double(Hours)) )
        {
            // Will load the Cards details.
            std::set<UnicodeString> Cards;
            TDBContacts::GetContactCards(DBTransaction,Info.ContactKey,Cards);
            if(Cards.size() > 0)
            {
				Info.CardStr = *Cards.begin();
                Info.ContactKey = GetContactByCard(DBTransaction, Info);
                if(Info.ContactKey != 0)
                {
					GetContactDetails(DBTransaction,Info.ContactKey,Info);
					RetVal = true;
                }
            }
        }
    }
    return RetVal;
}

void TMembershipExternal::SyncPointsDetials
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info) {
	try {
		Currency CurrentLocalBalance = TMembership::LoyaltyGetValue(DBTransaction,
			Info.ContactKey);
		Currency SyncAdjustment = Info.Points.getPointsBalance() - CurrentLocalBalance;
		if (SyncAdjustment != 0) {
			TPointsTransaction PointsTransaction;
			PointsTransaction.ContactKey = Info.ContactKey;
			PointsTransaction.TimeStamp = Now();
			PointsTransaction.ExportStatus = pesNone;
			PointsTransaction.PointsTransactionType = pttSync;
			PointsTransaction.PointsTransactionAccountType = ptstLoyalty;
			PointsTransaction.Adjustment = SyncAdjustment;
			PointsTransaction.InvoiceNumber = "NA Bally Sync";
			TMembership::LoyaltyAddValue(DBTransaction, PointsTransaction);
		}
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

int TMembershipExternal::GetContactByCode(int inContactCode) {
	if (!fEnabled)
		return 0;
	int RetVal = 0;
	return RetVal;
}

void TMembershipExternal::SetContactCode(int inContactKey, int ContactID) {
	if (!fEnabled)
		return;
}

void TMembershipExternal::SyncMembers
		(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress) {
	std::auto_ptr<TStringList>List(new TStringList);

	GetContactList(DBTransaction, List.get(), "%Name", "NAME");

	Progress->Show();
	Progress->Max = List->Count;
	int j = 0;
	for (int i = 0; i < List->Count && !Progress->Cancelled; i++) {
		TMMContactInfo Info;
		int ExternalKey = int(List->Objects[i]);
		AnsiString Name = List->Strings[i];
		// Will Cause Sync.
		Progress->Message = "Syncing " + Name;

		int InternalKey = GetInternalKey(DBTransaction, ExternalKey);
		if (InternalKey == 0) {
			// This will cause a creation "sync" if they dont exist in our DB.
			InternalKey = GetExternalKey(DBTransaction, ExternalKey);
		}
		else {
			// However if they do exist we want to sync there points.
			Currency Points = LoyaltyGetExternalValue(DBTransaction, ExternalKey);
			Currency CurrentLocalBalance = TMembership::LoyaltyGetValue
					(DBTransaction, InternalKey);

			Currency SyncAdjustment = Points - CurrentLocalBalance;
			if (SyncAdjustment != 0) {
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
		if (j > 250) {
			j = 0;
			DBTransaction.Commit();
			DBTransaction.StartTransaction();
		}
		j++;
	}
}

void TMembershipExternal::GetKioskDisplayMsg
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info,
		 std::map<UnicodeString, UnicodeString> &Data)
{
	if (!fEnabled)
		return;
	try {
		Data["%TITLE%"] = "Christchurch Casino";
		Data["%MSG%"] = "Welcome";
		Data["%MSG1%"] = Info.Name;
		Data["%VISIT_MSG%"] = "";
		Data["%BIRTHDAY_MSG%"] = "";
		// Data["%POINTS%"] = FormatFloat("0.00",Info.Points.CurrentPoints);
		Data["BIRTHDAY_PRINT"] = "F";
		Data["DOCKET_PRINT"] = "F";
		Data["DOCKET_LUCKYDRAW_PRINT"] = "F";
	}
	catch(Exception & E) {
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

/* This overrides TMembership::FindMember because we do not want to seach by Prox or member number.
Only by Card though the Ebet TCPIP interface. We do not want to bring up the local MenuMate version
of this member */
TLoginSuccess TMembershipExternal::FindMember
		(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo) {
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

	if (UserInfo.ContactKey != 0) {
		LoginSuccess = TestKeyLogin(DBTransaction, UserInfo);
		GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
	}

	return LoginSuccess;
}

TLoginSuccess TMembershipExternal::IsCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo)
{
// %2872000000072002?
// Data on Track 2 it stripped and ignored.
/* First Char must be %
   Last Char must be ?
   First 3 Chars must match Site ID 287 */
	if(UserInfo.CardStr.Pos("%") == 1 &&
       UserInfo.CardStr.Pos("?") == UserInfo.CardStr.Length() &&
       UserInfo.CardStr.SubString(2,3) == "287")
    {
        // Reformat Card.
        UserInfo.CardStr = UserInfo.CardStr.SubString(UserInfo.CardStr.Pos("%"),UserInfo.CardStr.Pos("?"));
        return lsAccepted;
    }
    else
    {
    	return lsDenied;
    }
}

void TMembershipExternal::GetReportMemberInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member, TStringList *Report)
{
   TMembership::GetReportMemberInfo(DBTransaction, Member, Report);
   // Add the notes block to the report.
   UnicodeString TempRow = LoadStr(TABLE_ROW4);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Account Info");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.AccountInfo == "" ? UnicodeString("&nbsp;") : Member.AccountInfo);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", UnicodeString("&nbsp;"));
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", UnicodeString("&nbsp;"));
   Report->Insert(Report->Count-1,TempRow);
}

/* Called Just before a Transaction with Membership applied is closed.
This allows the Membership system to make any final changes as transaction is closed.
*/
void TMembershipExternal::TransactionClosed(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{
	if (Member.Valid())
	{

		if(!MembershipApplied(DBTransaction,Member.ContactKey))
        {
     		TDBContacts::ClearPIN(DBTransaction,Member.ContactKey,ContactType);
        }
    }
}

