// ---------------------------------------------------------------------------
#pragma hdrstop
#include "MembershipERS.h"
#include <set>
#include <Memory>
#include <Sysutils.hpp>
#include "MMLogging.h"
#include "DBTab.h"
#include "Invoice.h"
#include "MMMessageBox.h"
#include "DB_Utilities.h"
#include <DateUtils.hpp>
#include "MMContactInfo.h"
#include "StringTableRes.h"

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// -----------------------TContactMemberERS--------------------------------------
// ---------------------------------------------------------------------------

TMembershipERS::TMembershipERS(TModules &Modules, Database::TDBControl &inIBDatabase) : TMembership(Modules), IBDatabase(inIBDatabase)
{
	ContactType = eERSMember;
	ContactTabType = TabMember;
	ReadOnlyInterface = true;
	Modules.Status[eRegMembers]["ReadOnly"] = ReadOnlyInterface;
	Enabled = Modules.Status[eRegMembers]["Enabled"];
	DBIndex = 0;
}

TMembershipERS::~TMembershipERS()
{
}

void TMembershipERS::Initialise(Database::TDBTransaction &DBTransaction)
{
	TMembership::Initialise(DBTransaction);

	if (IBDatabase.Connected() && RunOnce == false && DBIndex != 0)
	{
		try
		{
			RunOnce = true;
			SyncAndFix3rdPartyTabs(DBTransaction);
		}
		catch(Exception & Err)
		{
			Enabled = false;
			TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err.Message + "Unable to Open ERS Membership Database.");
		}
	}
}

void TMembershipERS::InitTransaction(Database::TDBTransaction &DBTransaction)
{
	if (IBDatabase.Connected())
	{
		DBIndex = DBTransaction.AddDatabase(IBDatabase);
	}
}

bool TMembershipERS::CheckVersionNumber(Database::TDBTransaction &DBTransaction)
{
	if (!fEnabled)
		return 0;
	bool RetVal = false;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "POS_LICENSE " "FROM " "SITE ";
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->FieldByName("POS_LICENSE")->AsString >= "4.02")
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

void TMembershipERS::LoyaltyAddValue(Database::TDBTransaction &DBTransaction, TPointsTransaction &PointsTransaction)
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

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MEMBERS_POINTS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int KeyVal = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "INSERT INTO MEMBERS_POINTS (" "MEMBERS_POINTS_KEY," "MEMBER," "EARNED," "REDEEMED," "ADJUSTMENT_TYPE," "POINTS_TYPE," "TIME_STAMP," "INVOICE_NUMBER) " "VALUES ("
			":MEMBERS_POINTS_KEY," ":MEMBER," ":EARNED," ":REDEEMED," ":ADJUSTMENT_TYPE," ":POINTS_TYPE," ":TIME_STAMP," ":INVOICE_NUMBER);";
		IBInternalQuery->ParamByName("MEMBERS_POINTS_KEY")->AsInteger = KeyVal;
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = GetExternalKey(DBTransaction, PointsTransaction.ContactKey);
		if (PointsTransaction.PointsTransactionType == pttEarned)
		{
			IBInternalQuery->ParamByName("EARNED")->AsCurrency = PointsTransaction.Adjustment;
			IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = 0;
		}
		else if (PointsTransaction.PointsTransactionType == pttPurchased)
		{
			IBInternalQuery->ParamByName("EARNED")->AsCurrency = 0;
			IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = -PointsTransaction.Adjustment;
		}
		else
		{
			IBInternalQuery->ParamByName("EARNED")->AsCurrency = 0;
			IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = PointsTransaction.Adjustment;
		}

		IBInternalQuery->ParamByName("POINTS_TYPE")->AsInteger = 0;
		IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = 4;
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("INVOICE_NUMBER")->AsString = PointsTransaction.InvoiceNumber;
		IBInternalQuery->ExecQuery();

		try
		{
			TMembership::LoyaltyAddValue(DBTransaction, PointsTransaction);
		}
		catch(Exception & E)
		{
			TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

Currency TMembershipERS::LoyaltyGetValue(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	if (!fEnabled)
		return 0;
	Currency RetVal = 0;
	try
	{
		int inExternalKey = GetExternalKey(DBTransaction, inContactKey);
		RetVal = LoyaltyGetValueFromMemberKey(DBTransaction, inExternalKey);
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

Currency TMembershipERS::LoyaltyGetValueFromMemberKey(Database::TDBTransaction &DBTransaction, int inExternalKey)
{
	if (!fEnabled)
		return 0;

	Currency RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "SUM(EARNED - REDEEMED) POINTS_AVAILABLE " "FROM " "MEMBERS_POINTS " "WHERE " "MEMBER = :MEMBER";
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = inExternalKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("POINTS_AVAILABLE")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "POINTS_AVAILABLE " "FROM " "MEMBERS " "WHERE " "MEMBER = :MEMBER";
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = inExternalKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal += IBInternalQuery->FieldByName("POINTS_AVAILABLE")->AsCurrency;
		}

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

TLoginSuccess TMembershipERS::TestCardLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	if (!fEnabled)
		return lsUserNotFound;
	TLoginSuccess Success = lsUserNotFound;

	try
	{
		int ContactKey = GetContactByCard(DBTransaction, UserInfo);
		if (ContactKey != 0)
		{
			GetContactDetails(DBTransaction, ContactKey, UserInfo);
			Success = GetSubsPaid(DBTransaction, UserInfo.ExternalKey);
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

TLoginSuccess TMembershipERS::TestKeyLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	if (!fEnabled)
		return lsUserNotFound;

	TLoginSuccess Success = lsUserNotFound;

	try
	{
		UserInfo.ExternalKey = GetExternalKey(DBTransaction, UserInfo.ContactKey);
		Success = GetSubsPaid(DBTransaction, UserInfo.ExternalKey);
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return Success;
}

TContactType TMembershipERS::GetContactType(int ContactKey)
{
	if (!fEnabled)
		return eContactTypeUnknown;
	TContactType RetVal = eERSMember;
	return RetVal;
}

TLoginSuccess TMembershipERS::GetSubsPaid(Database::TDBTransaction &DBTransaction, int ExternalKey)
{
	if (!fEnabled)
		return lsAccountBlocked;
	TLoginSuccess Success = lsAccountBlocked;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select SUB_PAID from MEMBERS where MEMBER = :MEMBER";
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = ExternalKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			if (IBInternalQuery->FieldByName("SUB_PAID")->AsInteger != 0)
			{
				Success = lsAccepted;
			}
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

bool TMembershipERS::DeleteContact(int inContactKey)
{
	if (!fEnabled)
		return false;
	bool RetVal = false;
	return RetVal;
}

void TMembershipERS::GetContactDetails(Database::TDBTransaction &DBTransaction, int InternalKey, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;
	try
	{
		int ExternalKey = GetExternalKey(DBTransaction, InternalKey);
		TMMContactInfo TempUserInfo;
		Get3rdPartyContactDetails(DBTransaction, ExternalKey, Info);
		if (Info.ExternalKey != 0) // Contact has not been removed from 3rd Party system.
		{
			Synchronize3rdPartyDetails(DBTransaction, InternalKey, Info);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipERS::SetContactDetails(Database::TDBTransaction &DBTransaction, int inInternalKey, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;
	try
	{
		if (DBIndex != 0)
		{
			TContact::SetContactDetails(DBTransaction, inInternalKey, Info);
			Set3rdPartyContactDetails(DBTransaction, Info.ExternalKey, Info);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipERS::Set3rdPartyContactDetails(Database::TDBTransaction &DBTransaction, int inExternalKey, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery, DBIndex);

		if (Info.CardStr != "")
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "UPDATE MEMBERS SET SWIPE_CARD = '' WHERE SWIPE_CARD	= :SWIPE_CARD";
			IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = Info.CardStr;
			IBInternalQuery->ExecQuery();

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "UPDATE MEMBERS SET SWIPE_CARD = :SWIPE_CARD WHERE MEMBER = :MEMBER";
			IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = Info.CardStr;
			IBInternalQuery->ParamByName("MEMBER")->AsInteger = inExternalKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

bool TMembershipERS::GetContactNameUnique(Database::TDBTransaction &DBTransaction, UnicodeString ContactName)
{
	if (!fEnabled)
		return false;
	if (ContactName == "")
		return false;

	bool RetVal = true;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT COUNT(MEMBER) MEMBERCOUNT FROM MEMBERS " "WHERE " "FULL_NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = ContactName;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->FieldByName("MEMBERCOUNT")->AsInteger > 1)
		{
			RetVal = false;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TMembershipERS::GetDiscountDetails(Database::TDBTransaction &DBTransaction, int inExternalKey, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery, DBIndex);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT DISCOUNT_KEY" " FROM " "  MEMBERS_DISCOUNTS" " WHERE" " MEMBER = :MEMBER ORDER BY MEMBER_DISCOUNTS_KEY";
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = inExternalKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				Info.AutoAppliedDiscounts.insert(IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger);
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipERS::Get3rdPartyContactDetails(Database::TDBTransaction &DBTransaction, int inExternalKey, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery, DBIndex);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" SELECT FULL_NAME,TITLE,MALE_FEMALE,INITIALS,HOME_PHONE,CELL_PHONE,EMAIL,ALLOW_CREDIT,"
			" DATE_OF_BIRTH,SWIPE_CARD,KNOWN_AS,ADDRESS,ADDRESS1,DISTRICT,CITY,COUNTRY,MAILING_CODE,"
            " STREET_ADDRESS,STREET_ADDRESS1,CARD_TYPE,STREET_DISTRICT,STREET_CITY,STREET_COUNTRY,"
            " TOTAL_VISITS,VISITS, LAST_VISIT,LAST_BIRTHDAY_PROCESSED,EARN_POINTS,EXPIRES"
            " FROM" " MEMBERS" " WHERE" " MEMBER = :MEMBER";
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = inExternalKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			Info.Name = EnsureASCII(IBInternalQuery->FieldByName("FULL_NAME")->AsString);
			Info.Title = EnsureASCII(IBInternalQuery->FieldByName("TITLE")->AsString);
			Info.Sex = EnsureASCII(IBInternalQuery->FieldByName("MALE_FEMALE")->AsString);
			Info.Initials = EnsureASCII(IBInternalQuery->FieldByName("INITIALS")->AsString);
			Info.Phone = EnsureASCII(IBInternalQuery->FieldByName("HOME_PHONE")->AsString);
			Info.Mobile = EnsureASCII(IBInternalQuery->FieldByName("CELL_PHONE")->AsString);

			Info.LocationAddress = "";
			Info.LocationAddress += EnsureASCII(IBInternalQuery->FieldByName("STREET_ADDRESS")->AsString) + "\r";
			Info.LocationAddress += EnsureASCII(IBInternalQuery->FieldByName("STREET_ADDRESS1")->AsString) + "\r";
			Info.LocationAddress += EnsureASCII(IBInternalQuery->FieldByName("STREET_DISTRICT")->AsString) + "\r";
			Info.LocationAddress += EnsureASCII(IBInternalQuery->FieldByName("STREET_CITY")->AsString) + "\r";
			Info.LocationAddress += EnsureASCII(IBInternalQuery->FieldByName("STREET_COUNTRY")->AsString);

			Info.MailingAddress = "";
			Info.MailingAddress += EnsureASCII(IBInternalQuery->FieldByName("ADDRESS")->AsString) + "\r";
			Info.MailingAddress += EnsureASCII(IBInternalQuery->FieldByName("ADDRESS1")->AsString) + "\r";
			Info.MailingAddress += EnsureASCII(IBInternalQuery->FieldByName("DISTRICT")->AsString) + "\r";
			Info.MailingAddress += EnsureASCII(IBInternalQuery->FieldByName("CITY")->AsString) + "\r";
			Info.MailingAddress += EnsureASCII(IBInternalQuery->FieldByName("COUNTRY")->AsString);

			Info.EMail = EnsureASCII(IBInternalQuery->FieldByName("EMAIL")->AsString);
			Info.TabEnabled = IBInternalQuery->FieldByName("ALLOW_CREDIT")->AsString.UpperCase() == "N" ? false : true;
			Info.DateOfBirth = IBInternalQuery->FieldByName("DATE_OF_BIRTH")->AsDateTime;
			Info.CardStr = IBInternalQuery->FieldByName("SWIPE_CARD")->AsString;
			Info.Alias = IBInternalQuery->FieldByName("KNOWN_AS")->AsString;
			Info.TotalVisits = IBInternalQuery->FieldByName("TOTAL_VISITS")->AsInteger;
			Info.RewardVisits = IBInternalQuery->FieldByName("VISITS")->AsInteger;
			Info.LastVisit = IBInternalQuery->FieldByName("LAST_VISIT")->AsDateTime;
			Info.LastBirthdayProcessed = IBInternalQuery->FieldByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime;
            Info.Expires = IBInternalQuery->FieldByName("EXPIRES")->AsDateTime;

            TPointsTypePair typepair(pttEarned,ptstLoyalty);
            TPointsType type(pasDatabase, typepair,pesExported);
            Info.Points.Load(type,LoyaltyGetValueFromMemberKey(DBTransaction, inExternalKey));

			if (EnsureASCII(IBInternalQuery->FieldByName("CARD_TYPE")->AsString) == EXPENSECARD)
			{
				Info.Points.PointsRules << eprAllowedNegitive << eprNeverEarnsPoints;
			}
			else if (EnsureASCII(IBInternalQuery->FieldByName("CARD_TYPE")->AsString) == FUNCTIONCARD)
			{
				Info.Points.PointsRules << eprAllowedNegitive;
			}

			if (!(IBInternalQuery->FieldByName("EARN_POINTS")->AsString.UpperCase() == "Y" ? true : false))
			{
				Info.Points.PointsRules << eprNeverEarnsPoints;
			}

			Info.ExternalKey = inExternalKey;
			Info.MembershipNumber = IntToStr(inExternalKey);
			Info.SiteID = -1;

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
			GetDiscountDetails(DBTransaction, inExternalKey, Info);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipERS::GetContactList(Database::TDBTransaction &DBTransaction, TStringList *List, UnicodeString Format, UnicodeString OrderBy)
{
	if (!fEnabled)
		return;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		if (OrderBy == "NAME")
			OrderBy = "FULL_NAME";
		if (OrderBy == "MEMBER_NUMBER")
			OrderBy = "MEMBER";
		if (OrderBy == "PHONE,NAME")
			OrderBy = "HOME_PHONE,FULL_NAME";

		UnicodeString SQLString = Format;
		SQLString = StringReplace(SQLString, "%Name", ",FULL_NAME", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
		SQLString = StringReplace(SQLString, "%Phone", ",HOME_PHONE", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
		SQLString = StringReplace(SQLString, "%Number", ",MEMBER", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT MEMBER MEMBER_KEY" + SQLString + " " " FROM " "  MEMBERS" " WHERE FULL_NAME IS NOT NULL" " ORDER BY " + OrderBy;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount && List != NULL)
		{
			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				UnicodeString Data;
				for (int i = 1; i < IBInternalQuery->FieldCount; i++)
				{
					Data += IBInternalQuery->Fields[i]->AsString.Trim() + " ";
				}

				int Index = List->Add(Data);
				// Little bit of pointer abuse but we just want to store the int somewhere.
				List->Objects[Index] = (TObject*)IBInternalQuery->FieldByName("MEMBER_KEY")->AsInteger;
			}
		}
		List->CaseSensitive = false;
		// List->Sort();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

int TMembershipERS::RecordCount(Database::TDBTransaction &DBTransaction)
{
	if (!fEnabled)
		return 0;

	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = " SELECT COUNT(MEMBER)" " FROM " "  MEMBERS";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->FieldByName("COUNT")->AsInteger;

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

int TMembershipERS::GetContactByCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
	if (!fEnabled)
		return 0;

	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "MEMBER " "FROM " "MEMBERS " "WHERE " "SWIPE_CARD = :SWIPE_CARD";
		IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = UserInfo.CardStr;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = GetContactKey(DBTransaction, IBInternalQuery->FieldByName("MEMBER")->AsInteger);
		}

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
	return RetVal;
}

int TMembershipERS::GetContactKey(Database::TDBTransaction &DBTransaction, int in3rdPartyContactKey)
{
	if (!fEnabled)
		return 0;

	// Kiosk No MenuMate DB
	if (DBIndex == 0)
		return in3rdPartyContactKey;

	int RetVal = 0;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE " "CONTACTS_3RDPARTY_KEY = :CONTACTS_3RDPARTY_KEY AND " "CONTACT_TYPE = :CONTACT_TYPE";
		IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = in3rdPartyContactKey;
		IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		}
		else
		{ // Create a new Member in our system.
			TMMContactInfo Info;
			Get3rdPartyContactDetails(DBTransaction, in3rdPartyContactKey, Info);
			Synchronize3rdPartyDetails(DBTransaction, 0, Info);
			RetVal = Info.ContactKey;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

UnicodeString TMembershipERS::GetContactNumber(Database::TDBTransaction &DBTransaction, int InternalKey)
{
	if (!fEnabled)
		return "";
	UnicodeString RetVal = "";
	try
	{
		RetVal = IntToStr(GetExternalKey(DBTransaction, InternalKey));
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TMembershipERS::Synchronize3rdPartyDetails(Database::TDBTransaction &DBTransaction, int inInternalKey, TMMContactInfo &Info)
{
	try
	{
		SetContactDetails(DBTransaction, inInternalKey, Info);
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TMembershipERS::SyncAndFix3rdPartyTabs(Database::TDBTransaction &DBTransaction)
{
	if (!fEnabled)
		return;

	if (DBIndex == 0)
		return;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuery1 = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT " "TAB.TAB_KEY, " "TAB.CONTACTS_KEY " "FROM " "TAB " "WHERE " "TAB_TYPE = :TAB_TYPE";
		IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = 99;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
			{
				TMMContactInfo Info;
				int ExternalKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
				Get3rdPartyContactDetails(DBTransaction, ExternalKey, Info);
				Synchronize3rdPartyDetails(DBTransaction, 0, Info);
				if (Info.ContactKey != 0)
				{
					IBInternalQuery1->Close();
					IBInternalQuery1->SQL->Text = "UPDATE TAB " "SET " "CONTACTS_KEY = :CONTACTS_KEY, " "TAB_TYPE = :TAB_TYPE " "WHERE " "TAB_KEY	= :TAB_KEY";
					IBInternalQuery1->ParamByName("CONTACTS_KEY")->AsString = Info.ContactKey;
					IBInternalQuery1->ParamByName("TAB_TYPE")->AsString = ContactTabType;
					IBInternalQuery1->ParamByName("TAB_KEY")->AsInteger = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
					IBInternalQuery1->ExecQuery();

					IBInternalQuery1->Close();
					IBInternalQuery1->SQL->Text = "UPDATE ARCHIVE " "SET " "LOYALTY_KEY = :LOYALTY_KEY " "WHERE " "LOYALTY_KEY	= :OLD_3RDPARTY_LOYALTY_KEY";
					IBInternalQuery1->ParamByName("LOYALTY_KEY")->AsString = Info.ContactKey;
					IBInternalQuery1->ParamByName("OLD_3RDPARTY_LOYALTY_KEY")->AsInteger = ExternalKey;
					IBInternalQuery1->ExecQuery();

					IBInternalQuery1->Close();
					IBInternalQuery1->SQL->Text = "UPDATE DAYARCHIVE " "SET " "LOYALTY_KEY = :LOYALTY_KEY " "WHERE " "LOYALTY_KEY	= :OLD_3RDPARTY_LOYALTY_KEY";
					IBInternalQuery1->ParamByName("LOYALTY_KEY")->AsString = Info.ContactKey;
					IBInternalQuery1->ParamByName("OLD_3RDPARTY_LOYALTY_KEY")->AsInteger = ExternalKey;
					IBInternalQuery1->ExecQuery();
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

int TMembershipERS::GetContactByCode(int inContactCode)
{
	if (!fEnabled)
		return 0;
	int RetVal = 0;
	/*
	try
	{
	if (!IBTransaction->InTransaction) {IBTransaction->StartTransaction(); StartTrans = true; }


	IBQuery->Close();
	IBQuery->SQL->Text =
	"SELECT "
	"CONTACTS_KEY "
	"FROM "
	"CONTACTS "
	"WHERE "
	"CONTACT_ID = :CONTACT_ID";
	IBQuery->ParamByName("CONTACT_ID")->AsInteger = inContactCode;
	IBQuery->Open();

	if(!IBQuery->IsEmpty())
	{
	RetVal = IBQuery->FieldByName("CONTACTS_KEY")->AsInteger;
	}
	if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Commit(); }
	}
	catch(Exception &E)
	{
	TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Rollback(); }
	} */
	return RetVal;
}

void TMembershipERS::SetContactCode(int inContactKey, int ContactID)
{
	if (!fEnabled)
		return;
	/*
	try
	{
	if (!IBTransaction->InTransaction) {IBTransaction->StartTransaction(); StartTrans = true; }

	IBQuery->Close();
	IBQuery->SQL->Text =
	"UPDATE "
	" CONTACTS"
	" SET"
	" CONTACT_ID = :CONTACT_ID"
	" WHERE"
	" CONTACTS_KEY = :CONTACTS_KEY AND "
	" CONTACT_TYPE = :CONTACT_TYPE";
	IBQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	IBQuery->ParamByName("CONTACT_ID")->AsInteger = ContactID;
	IBQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	IBQuery->ExecSQL();

	if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Commit(); }
	}
	catch(Exception &E)
	{
	TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
	if (IBTransaction->InTransaction && StartTrans) { IBTransaction->Rollback(); }
	} */
}

UnicodeString TMembershipERS::EnsureASCII(UnicodeString Data)
{
	UnicodeString RetVal = "";
	for (int i = 1; i <= Data.Length(); i++)
	{
		if (isascii(Data[i]) && !iscntrl(Data[i]))
		{
			RetVal += Data[i];
		}
		else
		{
			RetVal += " ";
		}
	}
	return RetVal;
}

void TMembershipERS::SyncMembers(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress)
{
	std::auto_ptr<TStringList>List(new TStringList);

	GetContactList(DBTransaction, List.get(), "%Name", "NAME");

	Progress->Show();
	Progress->Max = List->Count;
	int j = 0;
	for (int i = 0; i < List->Count && !Progress->Cancelled; i++)
	{
		TMMContactInfo Info;
		int inExternalKey = int(List->Objects[i]);
		AnsiString Name = List->Strings[i];
		// Will Cause Sync.
		Progress->Message = "Syncing " + Name;

		int ContactKey = GetInternalKey(DBTransaction, inExternalKey);
		if (ContactKey == 0)
		{
			// This will cause a creation "sync" if they dont exist in our DB.
			ContactKey = GetContactKey(DBTransaction, inExternalKey);
		}
		else
		{
			// However if they do exist we want to sync there points.
			Currency Points = LoyaltyGetValueFromMemberKey(DBTransaction, inExternalKey);
			Currency CurrentLocalBalance = TMembership::LoyaltyGetValue(DBTransaction, ContactKey);

			Currency SyncAdjustment = Points - CurrentLocalBalance;
			if (SyncAdjustment != 0)
			{
				TPointsTransaction PointsTransaction;

				PointsTransaction.ContactKey = ContactKey;
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

void TMembershipERS::GetKioskDisplayMsg(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, std::map<UnicodeString, UnicodeString> &Data)
{
	if (!fEnabled)
		return;
	try
	{

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select COMP_NAME from Site";
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			Data["%TITLE%"] = IBInternalQuery->FieldByName("COMP_NAME")->AsString.Trim();
		}

		// ------------- Visits Docket-------------
		if (Info.Valid())
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "Select * from Kiosk where KIOSK_ID = :KIOSK_ID";
			IBInternalQuery->ParamByName("KIOSK_ID")->AsInteger = Info.RewardVisits;
			IBInternalQuery->ExecQuery();
			if (IBInternalQuery->RecordCount)
			{
				Data["DOCKET_VISIT_PRINT"] = "T";
				Data["%MSG%"] = StringReplace(IBInternalQuery->FieldByName("MESSAGE")->AsString, ",", "<br>", TReplaceFlags() << rfReplaceAll);
				Data["%MSG1%"] = StringReplace(IBInternalQuery->FieldByName("MESSAGE1")->AsString, ",", "<br>", TReplaceFlags() << rfReplaceAll);
			}
         else
         {
				Data["DOCKET_VISIT_PRINT"] = "F";
				Data["%MSG%"] = "";
				Data["%MSG1%"] = "";
         }
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select * from MISC";
		IBInternalQuery->ExecQuery();
		Data["%VISIT_HEADER%"] = IBInternalQuery->FieldByName("VISIT_MSG")->AsString;
		Data["%DRAW_HEADER%"] = IBInternalQuery->FieldByName("LUCKY_DRAW_MSG")->AsString;
		Data["%BIRTHDAY_HEADER%"] = IBInternalQuery->FieldByName("BIRTHDAY_MSG")->AsString;

		Data["%DRAW_TEXT%"] = IBInternalQuery->FieldByName("LUCKY_DRAW_TEXT")->AsString;
		Data["%BIRTHDAY_TEXT%"] = IBInternalQuery->FieldByName("BIRTHDAY_TEXT")->AsString;

		// ------------- Birthdays Docket-------------
		bool PrintBirthdayDocket = false;
		if (IBInternalQuery->FieldByName("BIRTHDAY_PRINT")->AsString == "T")
		{
			if (ProcessBirthday(DBTransaction, Info))
			{
				Data["%BIRTHDAY_MSG%"] = IBInternalQuery->FieldByName("BIRTHDAY_TEXT")->AsString;
				PrintBirthdayDocket = true;
			}
		}
		Data["DOCKET_BIRTHDAY_PRINT"] = PrintBirthdayDocket ? "T" : "F";

		// ------------- Lucky Drawer Docket-------------
		switch(DayOfTheWeek(Now()))
		{
		case 1:
			if (!IBInternalQuery->FieldByName("MONDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("MONDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("MONDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("MONDAY_PRINT")->AsString;
			}
			break;
		case 2:
			if (!IBInternalQuery->FieldByName("TUESDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("TUESDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("TUESDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("TUESDAY_PRINT")->AsString;
			}
			break;
		case 3:
			if (!IBInternalQuery->FieldByName("WEDNESDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("WEDNESDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("WEDNESDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("WEDNESDAY_PRINT")->AsString;
			}
			break;
		case 4:
			if (!IBInternalQuery->FieldByName("THURSDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("THURSDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("THURSDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("THURSDAY_PRINT")->AsString;
			}
			break;
		case 5:
			if (!IBInternalQuery->FieldByName("FRIDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("FRIDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("FRIDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("FRIDAY_PRINT")->AsString;
			}
			break;
		case 6:
			if (!IBInternalQuery->FieldByName("SATURDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("SATURDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("SATURDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("SATURDAY_PRINT")->AsString;
			}
			break;
		case 7:
			if (!IBInternalQuery->FieldByName("SUNDAY_END_TIME")->IsNull)
			{
				TDateTime Time = IBInternalQuery->FieldByName("SUNDAY_END_TIME")->AsDateTime;
				Time = RecodeDate(Time, YearOf(Now()), MonthOf(Now()), DayOf(Now()));
				if (Now() < Time)
				{
					Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("SUNDAY_PRINT")->AsString;
				}
			}
			else
			{
				Data["DOCKET_LUCKYDRAW_PRINT"] = IBInternalQuery->FieldByName("SUNDAY_PRINT")->AsString;
			}
			break;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

bool TMembershipERS::ProcessBirthday(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
	bool RetVal = false;
	if (Info.Valid())
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery(DBIndex));
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select * from MISC";
		IBInternalQuery->ExecQuery();

		int BirthdayPrintSpan = 0;
		if (DB_Utilities::FieldExists(IBInternalQuery, "BIRTHDAY_SPAN"))
		{
			BirthdayPrintSpan = IBInternalQuery->FieldByName("BIRTHDAY_SPAN")->AsInteger;
		}

		if (DateOf(Info.LastBirthdayProcessed) != DateOf(RecodeYear(Info.DateOfBirth, YearOf(Now()))))
		{
			if (BirthdayPrintSpan == 0)
			{
				if (IsSameDay(RecodeYear(Info.DateOfBirth, YearOf(Now())), Today()))
				{
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "UPDATE MEMBERS SET LAST_BIRTHDAY_PROCESSED = :LAST_BIRTHDAY_PROCESSED WHERE MEMBER = :MEMBER";
					IBInternalQuery->ParamByName("MEMBER")->AsInteger = Info.ExternalKey;
					IBInternalQuery->ParamByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime = DateOf(RecodeYear(Info.DateOfBirth, YearOf(Now())));
					IBInternalQuery->ExecQuery();
					RetVal = true;
				}
			}
			else if (abs(DaysBetween(RecodeYear(Info.DateOfBirth, YearOf(Now())), Today())) < BirthdayPrintSpan)
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "UPDATE MEMBERS SET LAST_BIRTHDAY_PROCESSED = :LAST_BIRTHDAY_PROCESSED WHERE MEMBER = :MEMBER";
				IBInternalQuery->ParamByName("MEMBER")->AsInteger = Info.ExternalKey;
				IBInternalQuery->ParamByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime = DateOf(RecodeYear(Info.DateOfBirth, YearOf(Now())));
				IBInternalQuery->ExecQuery();
				RetVal = true;
			}
		}
	}
	return RetVal;
}

void TMembershipERS::SetVisitCount(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
	if (!fEnabled)
		return;

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery, DBIndex);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE MEMBERS " "SET " "TOTAL_VISITS = :TOTAL_VISITS, " "VISITS 	  = :VISITS, " "LAST_VISIT   = :LAST_VISIT " "WHERE " "MEMBER	= :MEMBER";
		IBInternalQuery->ParamByName("TOTAL_VISITS")->AsInteger = Info.TotalVisits;
		IBInternalQuery->ParamByName("VISITS")->AsInteger = Info.RewardVisits;
		IBInternalQuery->ParamByName("LAST_VISIT")->AsDateTime = Now();
		IBInternalQuery->ParamByName("MEMBER")->AsInteger = Info.ExternalKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

int TMembershipERS::GetExternalKey(Database::TDBTransaction &DBTransaction, int InternalKey)
{
	if (!fEnabled)
		return 0;

	int RetVal = 0;
	try
	{
		// Kiosk No MenuMate DB so Internal Key is ERS External Key.
		if (DBIndex == 0)
		{
			RetVal = InternalKey;
		}
		else
		{
			RetVal = TContact::GetExternalKey(DBTransaction, InternalKey);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TMembershipERS::GetReportMemberOnPremisesInfo(Database::TDBTransaction &DBTransaction, TStringList *Report)
{

	Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	DBTransaction.RegisterQuery(IBInternalQuery, DBIndex);

   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	   "select MEMBER MEMBER_NUMBER,FULL_NAME NAME,LAST_VISIT from MEMBERS where LAST_VISIT > :LAST_VISIT ORDER BY LAST_VISIT";
   IBInternalQuery->ParamByName("LAST_VISIT")->AsDateTime = Now() - 1;
   IBInternalQuery->ExecQuery();
   if (IBInternalQuery->RecordCount)
   {
	  // Add the Membership Block.
	  UnicodeString Temp = LoadStr(TABLE_START);
	  Report->Add(Temp);
	  Temp = LoadStr(TABLE_ROW7_HEADER);
	  Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", "Members On Premises (24 hr)");
	  Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", "");
	  Report->Add(Temp);
 		UnicodeString TempRow = LoadStr(TABLE_ROW7S);
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
      	   UnicodeString TempRow = LoadStr(TABLE_ROW7S);
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
	  Temp = LoadStr(TABLE_STOP);
	  Report->Add(Temp);
   }
}

void TMembershipERS::FastContactListing(TIBSQL **query,
                                     Database::TDBTransaction &transaction,
                                     int contact_type,
                                     int search_type,
                                     bool with_account_profile_only)
{
	TIBSQL *populated_query;

	AnsiString order_clause[3] = {
		"order by full_name ",
		"order by member ",
		"order by home_phone, full_name "
	};
    AnsiString where_clause = "WHERE members.full_name IS NOT NULL ";

	AnsiString base_clause[3] = {
		"select full_name n,cast('' as varchar(1)) as l, "
      "       member k "
		"       from members ",
		"select full_name || ' ' || member n, cast('' as varchar(1)) as l,"
      "       member k "
		"       from members ",
		"select case when (home_phone is null) then "
	   "                 full_name "
		"            else full_name || ' ' || home_phone "
		"       end n, "
      "       member k, cast('' as varchar(1)) as l "
		"       from members "
	};

	populated_query =
	  (*query = transaction.Query(transaction.AddQuery(DBIndex)));

	transaction.StartTransaction();

	populated_query->Close();
	populated_query->SQL->Text = base_clause[search_type];
	if (with_account_profile_only)
		populated_query->SQL->Text =
		  populated_query->SQL->Text + where_clause + " and account_profile <> 0 ";
	populated_query->SQL->Text =
	  populated_query->SQL->Text + where_clause + order_clause[search_type];
	populated_query->ExecQuery();
}

