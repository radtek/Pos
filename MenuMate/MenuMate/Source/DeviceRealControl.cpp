//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealControl.h"
//#include "DeviceRealTerminal.h"
#include "Csv.h"
#include <IdSNTP.hpp>
#include <StrUtils.hpp>
#include "MMLogging.h"
#include "ContactStaff.h"
#include "MMMessageBox.h"
#include "SystemConfig.h"
#include "MMRegistry.h"
#include "Printing.h"
#include "Locations.h"
#include "MMTouchNumpad.h"
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include "DBContacts.h"
#include "split.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TDeviceRealControl *TDeviceRealControl::active_instance = NULL;

void TDeviceRealControl::UpdateInfoSync(TNetMessageInfoSync *Request)
{
	TGlobalSettings::Instance().InfoSync++;
	if (TGlobalSettings::Instance().InfoSync == 0)
	{
		TGlobalSettings::Instance().InfoSync++;
	}
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmInfoSync, TGlobalSettings::Instance().InfoSync);
	DBTransaction.Commit();

	if (Request->Broadcast)
	{
		ManagerNet->SendToAll(Request);
	}
}

void TDeviceRealControl::UpdateClockSyncInfo()
{
	// System Clocks.
	if (TGlobalSettings::Instance().SntpIP != "")
	{
		bool Successful = false;
		bool SNTPFailed = false;
		bool WinTimeFailed = false;
		UnicodeString SNTPExecptionMessage;
		UnicodeString WinTimeExecptionMessage;
		try
		{
			TIdSNTP *Time = new TIdSNTP(NULL);
			try
			{
				Time->Host = TGlobalSettings::Instance().SntpIP;
				Time->ReceiveTimeout = 4000;
				if (Time->SyncTime())
				{
					Successful = true;
				}
				else
				{
					SNTPFailed = true;
				}
			}
			__finally
			{
				delete Time;
			}
		}
		catch(Exception & E)
		{ // Try windows NET command.
			SNTPExecptionMessage = E.Message;
			SNTPFailed = true;
		}

		if (!Successful)
		{
			try
			{
				// If NT do security stuff
				/* SECURITY_DESCRIPTOR sd;
			SECURITY_ATTRIBUTES sa;
			LPSECURITY_ATTRIBUTES lpsa = NULL;
			if(Win32Platform == VER_PLATFORM_WIN32_NT)//IsWindowsNT)
			{
			InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(&sd, true, NULL, false);
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = true;
			sa.lpSecurityDescriptor = &sd;
			lpsa = &sa;
			} */

				// initialize STARTUPINFO struct
				STARTUPINFO si;
				memset(&si, 0, sizeof(STARTUPINFO));
				si.cb = sizeof(STARTUPINFO);
				si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
				si.wShowWindow = SW_HIDE;
				PROCESS_INFORMATION pi;

				UnicodeString Command;
				Command = "NET TIME \\\\" + TGlobalSettings::Instance().SntpIP + " /set /y";
				bool ProcOpen = CreateProcess(NULL, Command.t_str(), NULL, // security
				NULL, // security
				TRUE, // inherits handles
				0, 0, 0, &si, &pi);
				if (ProcOpen)
				{
					DWORD RetVal = WaitForSingleObject(pi.hProcess, 9000);
					if (RetVal == WAIT_TIMEOUT)
					{
						WinTimeExecptionMessage = "Timeed out conencting to SNTP server.";
						WinTimeFailed = true;
					}
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
			}
			catch(Exception & E)
			{
				WinTimeExecptionMessage = E.Message;
				WinTimeFailed = true;
			}
		}

		if (WinTimeFailed && SNTPFailed)
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, SNTPExecptionMessage);
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, WinTimeExecptionMessage);
		}
	}
}

// ---------------------------------------------------------------------------
void TDeviceRealControl::GetHappyHourInfo(TNetMessageHappyHour *Request)
{
	for (int i = 0; i < eEndOfDays; i++)
	{
		Request->HappyDays[i] = TGlobalSettings::Instance().HappyHourDays[i];
	}
	Request->DiscountStart = TGlobalSettings::Instance().HappyHourStart;
	Request->DiscountFinish = TGlobalSettings::Instance().HappyHourFinish;
}

void TDeviceRealControl::UpdateHappyHourInfo(TNetMessageHappyHour *Request)
{
	// Happy Hours.
	for (int i = eSunday; i < eEndOfDays; i++)
	{
		TGlobalSettings::Instance().HappyHourDays[i] = Request->HappyDays[i];
	}
	TGlobalSettings::Instance().HappyHourStart = Request->DiscountStart;
	TGlobalSettings::Instance().HappyHourFinish = Request->DiscountFinish;
	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		TManagerVariable::Instance().SetDeviceTime(DBTransaction, vmHappyHourStart, TGlobalSettings::Instance().HappyHourStart);
		TManagerVariable::Instance().SetDeviceTime(DBTransaction, vmHappyHourFinish, TGlobalSettings::Instance().HappyHourFinish);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHSunday, Request->HappyDays[eSunday]);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHMonday, Request->HappyDays[eMonday]);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHTuesday, Request->HappyDays[eTuesday]);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHWednesday, Request->HappyDays[eWednesday]);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHThursday, Request->HappyDays[eThursday]);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHFridays, Request->HappyDays[eFriday]);
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHHSaturday, Request->HappyDays[eSaturday]);

		TGlobalSettings::Instance().HappyHourExempt = Request->TerminalExemptFromHappyHour;
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHappyHourExempt, TGlobalSettings::Instance().HappyHourExempt);

		TGlobalSettings::Instance().HappyHourMembersOnly = Request->HappyHourMembersOnly;
		TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmHappyHourMembersOnly, TGlobalSettings::Instance().HappyHourMembersOnly);

		DBTransaction.Commit();
		TGlobalSettings::Instance().ForceHappyHour = Request->ForceThisTerminalNow;
	}
	catch(Exception & Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}

	if (Request->Broadcast)
	{
		ManagerNet->SendToAll(Request);
	}
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void TDeviceRealControl::UpdatePeople(Database::TDBTransaction &DBTransaction, AnsiString FileName)
{
	try
	{
		TCsv Csv;
        int contactsKey = 0;
		Csv.LoadFromFile(FileName);
		int ErrorsOccured = 0;
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		for (int i = 0; i < Csv.RowCount; i++)
		{
			try
			{
				const int TYPE = 0;
				const int NAME = 1;
				const int PHONE = 2;
				const int CELL_PHONE = 3;
				const int PHONE_EXT = 4;
				const int EMAIL = 5;
				const int MAILADD1 = 6;
				const int INITIALS = 7;
				const int PIN = 8;
				const int CARD = 9;
				const int TOTAL_SPENT_PLUS_MINUS = 10;
				const int TOTAL_SPENT = 11;
				const int MEMBER_NUMBER = 12;
				const int SITE_ID = 13;
				const int FIRST_NAME = 14;
				const int MIDDLE_NAME = 15;
				const int LAST_NAME = 16;
				const int SEX = 17;
				const int TAB_ENALBED = 18;
				const int PAYROLL_ID = 19;
				const int DATEOFBIRTH = 20;
				const int KNOWN_AS = 21;
				const int CARD_CREATION_DATE = 22;
				const int LAST_MODIFIED = 23;
				const int ACCOUNT_NAME = 24;
				const int ACCOUNT_ID = 25;
				const int MAILADD2 = 26;
				const int MAILADD3 = 27;
				const int MAILADD4 = 28;
				const int MAILADD5 = 29;
				const int LOCADD1 = 30;
				const int LOCADD2 = 31;
				const int LOCADD3 = 32;
				const int LOCADD4 = 33;
				const int LOCADD5 = 34;
//-----------------------------------------------------------
                const int FINANCIAL = 35;
                const int ALLOWDISCOUNTS = 36;
                const int NEVEREARNSPOINTS = 37;
                const int NEVERREDEEMSPOINTS = 38;
                const int NEVERPURCHASESPOINTS = 39;
//-----------------------------------------------------------

				AnsiString MemberNumber = Csv.Cells[MEMBER_NUMBER][i];
				int SiteID = StrToInt(Csv.Cells[SITE_ID][i]);

				if (MemberNumber != "")
				{
					// Remove Old Members that might have the same Membernumber  / Site ID.
					TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
					UnicodeString DuplicateSQL =
					"Update Contacts a set a.MEMBER_NUMBER = :MEMBER_NUMBER_1 "
					"where a.MEMBER_NUMBER = :MEMBER_NUMBER AND a.SITE_ID = :SITE_ID AND CONTACT_TYPE = 3";
					IBInternalQuery->SQL->Text = DuplicateSQL;
					IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Csv.Cells[MEMBER_NUMBER][i];
					IBInternalQuery->ParamByName("SITE_ID")->AsInteger = StrToInt(Csv.Cells[SITE_ID][i]);
					IBInternalQuery->ParamByName("MEMBER_NUMBER_1")->AsString = UnicodeString(Csv.Cells[MEMBER_NUMBER][i] + Now().FormatString(":ddmmyy")).SubString(1,10);
					IBInternalQuery->ExecQuery();

					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text =
					"SELECT TOTAL_SPENT FROM CONTACTS WHERE MEMBER_NUMBER = :MEMBER_NUMBER AND SITE_ID = :SITE_ID";
					IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Csv.Cells[MEMBER_NUMBER][i];
					IBInternalQuery->ParamByName("SITE_ID")->AsInteger = StrToInt(Csv.Cells[SITE_ID][i]);
					IBInternalQuery->ExecQuery();

					Currency CurrentPoints = 0;
					Currency AdjustedTotal = 0;

					if(!IBInternalQuery->Eof)
					{
						CurrentPoints = IBInternalQuery->FieldByName("TOTAL_SPENT")->AsCurrency;
					}

					UnicodeString TotalSpentPlusMinus = Csv.Cells[TOTAL_SPENT_PLUS_MINUS][i];
					if (TotalSpentPlusMinus == "+" || TotalSpentPlusMinus.UpperCase() == "PLUS")
					{
						Currency PointsToAdd = 0;
						if (Csv.Cells[TOTAL_SPENT][i] != "")
						{
							PointsToAdd = StrToCurr(Csv.Cells[TOTAL_SPENT][i]);
							AdjustedTotal = CurrentPoints + PointsToAdd;
							Csv.Cells[TOTAL_SPENT][i] = CurrToStr( AdjustedTotal );
						}
					}
					else if (TotalSpentPlusMinus == "-" || TotalSpentPlusMinus.UpperCase() == "MINUS")
					{
						Currency PointsToMinus = 0;
						if (Csv.Cells[TOTAL_SPENT][i] != "")
						{
							PointsToMinus = StrToCurr(Csv.Cells[TOTAL_SPENT][i]);
							AdjustedTotal = CurrentPoints - PointsToMinus;
							Csv.Cells[TOTAL_SPENT][i] = CurrToStr( AdjustedTotal );
						}
					}
					else
					{
						AdjustedTotal = StrToCurr(Csv.Cells[TOTAL_SPENT][i]);
					}

					IBInternalQuery->Close();
					UnicodeString SQL =
					"UPDATE CONTACTS SET "
					"CONTACT_TYPE	   = :CONTACT_TYPE,"
					"NAME			   = :NAME,"
					"PHONE	   		   = :PHONE,"
					"MOBILE			   = :MOBILE,"
					"PHONE_EXTENSION	   = :PHONE_EXTENSION,"
					"EMAIL	   		   = :EMAIL,"
					"MAILING_ADDRESS	   = :MAILING_ADDRESS,"
					"INITIALS		   = :INITIALS,"
					"PIN				   = :PIN,"
					"FIRST_NAME         = :FIRST_NAME,"
					"MIDDLE_NAME        = :MIDDLE_NAME,"
					"LAST_NAME          = :LAST_NAME,"
					"SEX                = :SEX,"
					"TAB_ENALBED        = :TAB_ENALBED,"
					"PAYROLL_ID         = :PAYROLL_ID,"
					"DATEOFBIRTH        = :DATEOFBIRTH,"
					"KNOWN_AS           = :KNOWN_AS,"
					"CARD_CREATION_DATE = :CARD_CREATION_DATE,"
					"LAST_MODIFIED      = :LAST_MODIFIED,"
					"ACCOUNT_NAME       = :ACCOUNT_NAME,"
					"ACCOUNT_ID         = :ACCOUNT_ID,"
					"LOCATION_ADDRESS   = :LOCATION_ADDRESS, "
                    "MEMBER_TYPE        = 1, "
                    "POINTS_RULES       = :POINTS_RULES ";

					if (Csv.Cells[TOTAL_SPENT][i] != "")
					{
						SQL += ",TOTAL_SPENT   	= :TOTAL_SPENT ";
					}

					SQL += " WHERE MEMBER_NUMBER = :MEMBER_NUMBER AND SITE_ID = :SITE_ID";

					IBInternalQuery->SQL->Text = SQL;

					IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Csv.Cells[MEMBER_NUMBER][i];
					IBInternalQuery->ParamByName("SITE_ID")->AsInteger = StrToInt(Csv.Cells[SITE_ID][i]);

					IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = StrToInt(Csv.Cells[TYPE][i]);
					IBInternalQuery->ParamByName("NAME")->AsString = Csv.Cells[NAME][i];
					IBInternalQuery->ParamByName("PHONE")->AsString = Csv.Cells[PHONE][i];
					IBInternalQuery->ParamByName("MOBILE")->AsString = Csv.Cells[CELL_PHONE][i];
					IBInternalQuery->ParamByName("PHONE_EXTENSION")->AsString = Csv.Cells[PHONE_EXT][i];
					IBInternalQuery->ParamByName("EMAIL")->AsString = Csv.Cells[EMAIL][i];
					IBInternalQuery->ParamByName("INITIALS")->AsString = Csv.Cells[INITIALS][i];

					IBInternalQuery->ParamByName("FIRST_NAME")->AsString = Csv.Cells[FIRST_NAME][i];
					IBInternalQuery->ParamByName("MIDDLE_NAME")->AsString = Csv.Cells[MIDDLE_NAME][i];
					IBInternalQuery->ParamByName("LAST_NAME")->AsString = Csv.Cells[LAST_NAME][i];
					IBInternalQuery->ParamByName("SEX")->AsString = Csv.Cells[SEX][i];
					bool TabEnabled = ((UpperCase(Csv.Cells[TAB_ENALBED][i]) == "T") ? true : false) ||
					((UpperCase(Csv.Cells[TAB_ENALBED][i]) == "TRUE") ? true : false);
					IBInternalQuery->ParamByName("TAB_ENALBED")->AsString = (TabEnabled ? "T" : "F");
					IBInternalQuery->ParamByName("PAYROLL_ID")->AsString = Csv.Cells[PAYROLL_ID][i];
					IBInternalQuery->ParamByName("DATEOFBIRTH")->AsDateTime = StrToDateDef(Csv.Cells[DATEOFBIRTH][i], 0);
					IBInternalQuery->ParamByName("KNOWN_AS")->AsString = Csv.Cells[KNOWN_AS][i];
					IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = StrToDateDef(Csv.Cells[CARD_CREATION_DATE][i], 0);
					IBInternalQuery->ParamByName("LAST_MODIFIED")->AsString = StrToDateDef(Csv.Cells[LAST_MODIFIED][i], 0);
					IBInternalQuery->ParamByName("ACCOUNT_NAME")->AsString = Csv.Cells[ACCOUNT_NAME][i];
					IBInternalQuery->ParamByName("ACCOUNT_ID")->AsString = Csv.Cells[ACCOUNT_ID][i];

					IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = AnsiString(Csv.Cells[MAILADD1][i]) + "\r" + AnsiString
					(Csv.Cells[MAILADD2][i]) + "\r" + AnsiString(Csv.Cells[MAILADD3][i]) + "\r" + AnsiString(Csv.Cells[MAILADD4][i])
					+ "\r" + AnsiString(Csv.Cells[MAILADD5][i]);

					IBInternalQuery->ParamByName("LOCATION_ADDRESS")->AsString = AnsiString(Csv.Cells[LOCADD1][i]) + "\r" + AnsiString
					(Csv.Cells[LOCADD2][i]) + "\r" + AnsiString(Csv.Cells[LOCADD3][i]) + "\r" + AnsiString(Csv.Cells[LOCADD4][i])
					+ "\r" + AnsiString(Csv.Cells[LOCADD5][i]);

					if (Csv.Cells[PIN][i] != "")
					{
						IBInternalQuery->ParamByName("PIN")->AsString = Csv.Cells[PIN][i];
					}
					else
					{
						IBInternalQuery->ParamByName("PIN")->AsString = "";
					}

					if (Csv.Cells[TOTAL_SPENT][i] != "")
					{
						IBInternalQuery->ParamByName("TOTAL_SPENT")->AsCurrency = StrToCurr(Csv.Cells[TOTAL_SPENT][i]);
					}
                    int pointsRules = 0;
                    if(Csv.Cells[NEVEREARNSPOINTS][i] == "Y")
                    {
                        pointsRules |= eprNeverEarnsPoints;
                    }
                    if(Csv.Cells[NEVERREDEEMSPOINTS][i] == "Y")
                    {
                        pointsRules |= eprNoPointsRedemption;
                    }
                    if(Csv.Cells[NEVERPURCHASESPOINTS][i] == "Y")
                    {
                        pointsRules |= eprNoPointsPurchases;
                    }
                    TPointsRulesSet PointsRulesSet;
                    TPointsRulesSetUtils().Expand(pointsRules,PointsRulesSet);
                    IBInternalQuery->ParamByName("POINTS_RULES")->AsInteger = TPointsRulesSetUtils().Compress(PointsRulesSet);


					IBInternalQuery->ExecQuery();
                    if(IBInternalQuery->RowsAffected != -1 && IBInternalQuery->RowsAffected != 0)
                    {
                        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
                        SelectQuery->Close();
                        SelectQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE MEMBER_NUMBER = :MEMBER_NUMBER";
                        SelectQuery->ParamByName("MEMBER_NUMBER")->AsString = Csv.Cells[MEMBER_NUMBER][i];
                        SelectQuery->ExecQuery();
                        if(SelectQuery->RecordCount > 0)
                        {
                            int key__ = SelectQuery->FieldByName("CONTACTS_KEY")->AsInteger;

                            TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
                            UpdateQuery->Close();
                            UpdateQuery->SQL->Text = "UPDATE MEMBERSHIP_SUBS_DETAILS SET"
                            " POINTS_RULES_SUBS = :POINTS_RULES_SUBS,"
                            " SUBS_PAID = :SUBS_PAID,"
                            " SUBS_PAID_DATE = :SUBS_PAID_DATE"
                            " WHERE CONTACTS_KEY = :CONTACTS_KEY";
                            UpdateQuery->ParamByName("CONTACTS_KEY")->AsInteger = SelectQuery->FieldByName("CONTACTS_KEY")->AsInteger;
                            int pointsRulesSubs = 0;
                            if(Csv.Cells[FINANCIAL][i] == "Y")
                            {
                                pointsRulesSubs |= eprFinancial;
                            }
                            if(Csv.Cells[ALLOWDISCOUNTS][i] == "Y")
                            {
                                pointsRulesSubs |= eprAllowDiscounts;
                            }
                            if(Csv.Cells[NEVEREARNSPOINTS][i] == "Y")
                            {
                                pointsRulesSubs |= eprNeverEarnsPoints;
                            }
                            if(Csv.Cells[NEVERREDEEMSPOINTS][i] == "Y")
                            {
                                pointsRulesSubs |= eprNoPointsRedemption;
                            }
                            if(Csv.Cells[NEVERPURCHASESPOINTS][i] == "Y")
                            {
                                pointsRulesSubs |= eprNoPointsPurchases;
                            }
                            TPointsRulesSubsSet PointsRulesSubs;
                            TPointsRulesSetUtils().ExpandSubs(pointsRulesSubs,PointsRulesSubs);
                            UpdateQuery->ParamByName("POINTS_RULES_SUBS")->AsInteger = TPointsRulesSetUtils().CompressSubs(PointsRulesSubs);
                            if(PointsRulesSubs.Contains(eprFinancial))
                            {
                               UpdateQuery->ParamByName("SUBS_PAID")->AsString = "T";
                               UpdateQuery->ParamByName("SUBS_PAID_DATE")->AsDateTime = Now();
                            }
                            else
                            {
                               UpdateQuery->ParamByName("SUBS_PAID")->AsString = "F";
                               UpdateQuery->ParamByName("SUBS_PAID_DATE")->AsDateTime = Dateutils::EncodeDateTime(1899,12,30,0,0,00,000);
                            }
                            UpdateQuery->ExecQuery();
                        }
                    }
					if (IBInternalQuery->RowsAffected == -1 || IBInternalQuery->RowsAffected == 0)
					{
						int key;
						IBInternalQuery->Close();
						IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTS, 1) FROM RDB$DATABASE";
						IBInternalQuery->ExecQuery();
						key = IBInternalQuery->Fields[0]->AsInteger;
                        contactsKey = key;

						IBInternalQuery->Close();
						IBInternalQuery->SQL->Clear();
						IBInternalQuery->SQL->Text =
						"INSERT INTO CONTACTS (" "CONTACTS_KEY," "CONTACT_TYPE," "NAME," "PHONE," "MOBILE," "PHONE_EXTENSION,"
						"EMAIL," "MAILING_ADDRESS," "INITIALS," "PIN," "TOTAL_SPENT," "MEMBER_NUMBER," "SITE_ID," "CONTACTS_3RDPARTY_KEY,"
						"FIRST_NAME," "MIDDLE_NAME," "LAST_NAME," "SEX," "TAB_ENALBED," "PAYROLL_ID," "DATEOFBIRTH," "KNOWN_AS,"
						"CARD_CREATION_DATE," "LAST_MODIFIED," "ACCOUNT_NAME," "ACCOUNT_ID," "LOCATION_ADDRESS," "EARNT_POINTS," "LOADED_POINTS," "ACTIVATION_DATE," "MEMBER_TYPE," "POINTS_RULES"") " "VALUES ("
						":CONTACTS_KEY," ":CONTACT_TYPE," ":NAME," ":PHONE," ":MOBILE," ":PHONE_EXTENSION," ":EMAIL," ":MAILING_ADDRESS,"
						":INITIALS," ":PIN," ":TOTAL_SPENT," ":MEMBER_NUMBER," ":SITE_ID," ":CONTACTS_3RDPARTY_KEY," ":FIRST_NAME,"
						":MIDDLE_NAME," ":LAST_NAME," ":SEX," ":TAB_ENALBED," ":PAYROLL_ID," ":DATEOFBIRTH," ":KNOWN_AS,"
						":CARD_CREATION_DATE," ":LAST_MODIFIED," ":ACCOUNT_NAME," ":ACCOUNT_ID," ":LOCATION_ADDRESS," ":EARNT_POINTS," ":LOADED_POINTS," ":ACTIVATION_DATE," ":MEMBER_TYPE," ":POINTS_RULES"");";

						IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Csv.Cells[MEMBER_NUMBER][i];
						IBInternalQuery->ParamByName("SITE_ID")->AsInteger = StrToInt(Csv.Cells[SITE_ID][i]);

						IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = key;
						IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = StrToInt(Csv.Cells[TYPE][i]);
						IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = 0;
						IBInternalQuery->ParamByName("NAME")->AsString = Csv.Cells[NAME][i];
						IBInternalQuery->ParamByName("PHONE")->AsString = Csv.Cells[PHONE][i].Trim();
						IBInternalQuery->ParamByName("MOBILE")->AsString = Csv.Cells[CELL_PHONE][i].Trim();
						IBInternalQuery->ParamByName("PHONE_EXTENSION")->AsString = Csv.Cells[PHONE_EXT][i];
						IBInternalQuery->ParamByName("EMAIL")->AsString = Csv.Cells[EMAIL][i].Trim();
						IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = Csv.Cells[MAILADD1][i].Trim();
						IBInternalQuery->ParamByName("INITIALS")->AsString = Csv.Cells[INITIALS][i];

						IBInternalQuery->ParamByName("FIRST_NAME")->AsString = Csv.Cells[FIRST_NAME][i].Trim();
						IBInternalQuery->ParamByName("MIDDLE_NAME")->AsString = Csv.Cells[MIDDLE_NAME][i].Trim();
						IBInternalQuery->ParamByName("LAST_NAME")->AsString = Csv.Cells[LAST_NAME][i].Trim();
						IBInternalQuery->ParamByName("SEX")->AsString = Csv.Cells[SEX][i];
						bool TabEnabled = ((UpperCase(Csv.Cells[TAB_ENALBED][i]) == "T") ? true : false) ||
						((UpperCase(Csv.Cells[TAB_ENALBED][i]) == "TRUE") ? true : false);
						IBInternalQuery->ParamByName("TAB_ENALBED")->AsString = (TabEnabled ? "T" : "F");
						IBInternalQuery->ParamByName("PAYROLL_ID")->AsString = Csv.Cells[PAYROLL_ID][i];
						IBInternalQuery->ParamByName("DATEOFBIRTH")->AsDateTime = StrToDateDef(Csv.Cells[DATEOFBIRTH][i], 0);
						IBInternalQuery->ParamByName("KNOWN_AS")->AsString = Csv.Cells[KNOWN_AS][i];
						IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = StrToDateDef(Csv.Cells[CARD_CREATION_DATE][i], 0);
						IBInternalQuery->ParamByName("LAST_MODIFIED")->AsString = StrToDateDef(Csv.Cells[LAST_MODIFIED][i], 0);
						IBInternalQuery->ParamByName("ACCOUNT_NAME")->AsString = Csv.Cells[ACCOUNT_NAME][i];
						IBInternalQuery->ParamByName("ACCOUNT_ID")->AsString = Csv.Cells[ACCOUNT_ID][i];

						IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = AnsiString(Csv.Cells[MAILADD1][i]) + "\r" + AnsiString
						(Csv.Cells[MAILADD2][i]) + "\r" + AnsiString(Csv.Cells[MAILADD3][i]) + "\r" + AnsiString(Csv.Cells[MAILADD4][i])
						+ "\r" + AnsiString(Csv.Cells[MAILADD5][i]);

						IBInternalQuery->ParamByName("LOCATION_ADDRESS")->AsString = AnsiString(Csv.Cells[LOCADD1][i]) + "\r" + AnsiString
						(Csv.Cells[LOCADD2][i]) + "\r" + AnsiString(Csv.Cells[LOCADD3][i]) + "\r" + AnsiString(Csv.Cells[LOCADD4][i])
						+ "\r" + AnsiString(Csv.Cells[LOCADD5][i]);

						if (Csv.Cells[PIN][i] != "")
						{
							IBInternalQuery->ParamByName("PIN")->AsString = Csv.Cells[PIN][i];
						}
						else
						{
							IBInternalQuery->ParamByName("PIN")->AsString = "";
						}

						if (Csv.Cells[TOTAL_SPENT][i] != "")
						{
							IBInternalQuery->ParamByName("TOTAL_SPENT")->AsCurrency = StrToCurr(Csv.Cells[TOTAL_SPENT][i]);
						}
						else
						{
							IBInternalQuery->ParamByName("TOTAL_SPENT")->AsCurrency = 0.0;
						}

						IBInternalQuery->ParamByName("EARNT_POINTS")->AsCurrency = 0.0;
						IBInternalQuery->ParamByName("LOADED_POINTS")->AsCurrency = 0.0;
						IBInternalQuery->ParamByName("ACTIVATION_DATE")->AsDateTime = Now();
                        IBInternalQuery->ParamByName("MEMBER_TYPE")->AsInteger = 1;
                        IBInternalQuery->ParamByName("POINTS_RULES")->AsInteger = TPointsRulesSetUtils().Compress(PointsRulesSet);

						IBInternalQuery->ExecQuery();

                        TIBSQL *InsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
                        InsertQuery->Close();
                        InsertQuery->SQL->Text =
                        "INSERT INTO MEMBERSHIP_SUBS_DETAILS "
                        " ( MEMBERSHIP_SUBS_KEY, SUBS_PAID_DATE,  SUBS_PAID_AMOUNT, SUBS_PAID_RECEIPT_NO, "
                        " SUBS_TYPE, SUBS_PAID, POINTS_RULES_SUBS , CONTACTS_KEY, ISLOCAL_MEMBER )"
                        " VALUES ( :MEMBERSHIP_SUBS_KEY, :SUBS_PAID_DATE,  :SUBS_PAID_AMOUNT, :SUBS_PAID_RECEIPT_NO, "
                        " :SUBS_TYPE, :SUBS_PAID, :POINTS_RULES_SUBS , :CONTACTS_KEY, :ISLOCAL_MEMBER ) ";
                        TIBSQL *GenerateQuery = DBTransaction.Query(DBTransaction.AddQuery());
                        GenerateQuery->Close();
                        GenerateQuery->SQL->Text = "SELECT GEN_ID(GEN_MEMBERSHIP_SUBS_KEY, 1) FROM RDB$DATABASE";
                        GenerateQuery->ExecQuery();
                        int subsKeyValue =  GenerateQuery->Fields[0]->AsInteger;

						InsertQuery->ParamByName("MEMBERSHIP_SUBS_KEY")->AsInteger = subsKeyValue;
                        int pointsRulesSubs = 0;
                        TPointsRulesSubsSet PointsRulesSubs;
                        if(Csv.Cells[FINANCIAL][i].Trim().UpperCase() == "Y")
                        {
                            pointsRulesSubs |= eprFinancial;
                        }
                        if(Csv.Cells[ALLOWDISCOUNTS][i].Trim().UpperCase() == "Y")
                        {
                            pointsRulesSubs |= eprAllowDiscounts;
                        }
                        if(Csv.Cells[NEVEREARNSPOINTS][i].Trim().UpperCase() == "Y")
                        {
                            pointsRulesSubs |= eprNeverEarnsPoints;
                        }
                        if(Csv.Cells[NEVERREDEEMSPOINTS][i].Trim().UpperCase() == "Y")
                        {
                            pointsRulesSubs |= eprNoPointsRedemption;
                        }
                        if(Csv.Cells[NEVERPURCHASESPOINTS][i].Trim().UpperCase() == "Y")
                        {
                            pointsRulesSubs |= eprNoPointsPurchases;
                        }
                        TPointsRulesSetUtils().ExpandSubs(pointsRulesSubs,PointsRulesSubs);
                        InsertQuery->ParamByName("POINTS_RULES_SUBS")->AsInteger = TPointsRulesSetUtils().CompressSubs(PointsRulesSubs);
                        InsertQuery->ParamByName("CONTACTS_KEY")->AsInteger = key;
                        InsertQuery->ParamByName("ISLOCAL_MEMBER")->AsString = "T";
                        if(PointsRulesSubs.Contains(eprFinancial))
                        {
                           InsertQuery->ParamByName("SUBS_PAID")->AsString = "T";
                           InsertQuery->ParamByName("SUBS_PAID_DATE")->AsDateTime = Now();
                           InsertQuery->ParamByName("SUBS_PAID_AMOUNT")->AsDouble = 0.0;
                           InsertQuery->ParamByName("SUBS_PAID_RECEIPT_NO")->AsString = "-";
                           InsertQuery->ParamByName("SUBS_TYPE")->AsString = "AUTO";
                        }
                        else
                        {
                           InsertQuery->ParamByName("SUBS_PAID")->AsString = "F";
                           InsertQuery->ParamByName("SUBS_PAID_DATE")->AsDateTime = Dateutils::EncodeDateTime(1899,12,30,0,0,00,000);
                           InsertQuery->ParamByName("SUBS_PAID_AMOUNT")->AsDouble = 0.0;
                           InsertQuery->ParamByName("SUBS_PAID_RECEIPT_NO")->AsString = "-";
                           InsertQuery->ParamByName("SUBS_TYPE")->AsString = "-";
                        }
                        InsertQuery->ExecQuery();
					}

					TMMContactInfo UserInfo;
					UserInfo.SiteID = StrToInt(Csv.Cells[SITE_ID][i]);
					UserInfo.MembershipNumber = MemberNumber;
					UserInfo.ContactKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, UserInfo.MembershipNumber,UserInfo.SiteID);
					Staff->SetContactCard(DBTransaction, UserInfo.ContactKey, Csv.Cells[CARD][i]);

					//once all the imports are done, check to see whether we have adjustment available, if so put that into the pointstransaction table
					Currency Adjustment = AdjustedTotal - CurrentPoints;
					if(  Adjustment != 0 )
					insertMemberPointsAdjustmentRecordToDB( DBTransaction, UserInfo.ContactKey, Adjustment );
				}
			}
			catch(Exception & E)
			{
				ErrorsOccured++;
				AnsiString FailedFile = ExtractFilePath(Application->ExeName) + "\\Import\\Failed\\FailedMembers.csv";
				if (!DirectoryExists(ExtractFilePath(FailedFile)))
				{
					ForceDirectories(ExtractFilePath(FailedFile));
				}

				std::auto_ptr <TStringList> List(new TStringList);

				if (FileExists(FailedFile))
				{
					List->LoadFromFile(FailedFile);
				}
				List->Add("Error on Line Number " + IntToStr(i) + " " + E.Message);
				List->Add(Csv.GetRow(i));
				List->SaveToFile(FailedFile);
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ("User Failed To Import : Line Number " + IntToStr(i) + " Error Msg : " + E.Message));
			}
        //---------------------------------------------------------------------
//                IBInternalQuery->Close();
//                IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MEMBERSHIP_SUBS_KEY, 1) FROM RDB$DATABASE";
//                IBInternalQuery->ExecQuery();
//                int key = IBInternalQuery->Fields[0]->AsInteger;
//
//                IBInternalQuery->Close();
//                IBInternalQuery->SQL->Clear();
//                IBInternalQuery->SQL->Text =
//                "INSERT INTO MEMBERSHIP_SUBS_DETAILS (" "MEMBERSHIP_SUBS_KEY," "CONTACTS_KEY," "SUBS_PAID_DATE," "SUBS_EXPIRY_DATE," "SUBS_PAID_AMOUNT," "SUBS_PAID_RECEIPT_NO" ") " "VALUES ("
//                ":MEMBERSHIP_SUBS_KEY," ":CONTACTS_KEY," ":SUBS_PAID_DATE," ":SUBS_EXPIRY_DATE," ":SUBS_PAID_AMOUNT," ":SUBS_PAID_RECEIPT_NO" ");";
//
//                IBInternalQuery->ParamByName("MEMBERSHIP_SUBS_KEY")->AsString = key;
//                IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactsKey;
//
//                IBInternalQuery->ParamByName("SUBS_PAID_DATE")->AsDateTime = "";
//                IBInternalQuery->ParamByName("SUBS_EXPIRY_DATE")->AsDateTime = "";
//                IBInternalQuery->ParamByName("SUBS_PAID_AMOUNT")->AsDouble = 0.0;
//                IBInternalQuery->ParamByName("SUBS_PAID_RECEIPT_NO")->AsString = Csv.Cells[PHONE][i];
        //---------------------------------------------------------------------

		}
		if (!DirectoryExists(ExtractFilePath(Application->ExeName) + "Import\\Old"))
		{
			ForceDirectories(ExtractFilePath(Application->ExeName) + "Import\\Old");
		}
		AnsiString Dest = ExtractFilePath(Application->ExeName) + "Import\\Old\\" + ChangeFileExt(ExtractFileName(FileName),
		"_" + Now().FormatString("yyyy_mm_dd hh_nn_ss") + ".csv");
		CopyFile(FileName.c_str(), Dest.c_str(), false);
		DeleteFile(FileName);

		if (ErrorsOccured > 0)
		{
			MessageBox(IntToStr(ErrorsOccured) + "/" + IntToStr(Csv.RowCount) +
			" Members Failed to Import. See the FailedMembers.csv in the Failed Folder for Detials.", "Some Imports Failed",
			MB_ICONINFORMATION + MB_OK);
		}
	}
	catch(Exception & Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		if (FileExists(FileName))
		{
			RenameFile(FileName, FileName + ".Failed");
		}
	}
}

void TDeviceRealControl::UpdateSeatName(TNetMessageSeatName * Request)
{
#ifdef PalmMate
	// -------------- Send Seat Names to other Palms--------------
	// This Function is only called when another terminal has
	// changed the a seat name. The DB will be updated by that terminal
	// we just have to get the name out to the palms.
#endif
}

void TDeviceRealControl::CloseDB()
{
	DBControl.Disconnect();
}

bool TDeviceRealControl::OpenMenuMateDB()
{
	std::auto_ptr <TfrmSystemConfig> frmSystemConfig(TfrmSystemConfig::Create(Screen->ActiveForm));
	frmSystemConfig->EnableMenuMate();

	bool RetVal = false;

	TGlobalSettings::Instance().DatabasePath = "";
	RegistryRead(MMBaseKey "Database", "DatabasePath", TGlobalSettings::Instance().DatabasePath);

	TGlobalSettings::Instance().InterbaseIP = "";
	RegistryRead(MMBaseKey "Database", "InterbaseIP", TGlobalSettings::Instance().InterbaseIP);

	bool Abort = false;
	do
	{
		try
		{
			if (TGlobalSettings::Instance().InterbaseIP != "" && TGlobalSettings::Instance().DatabasePath != "")
			{
				DBControl.Disconnect();
				DBControl.Init(Database::TDBSettings(TGlobalSettings::Instance().InterbaseIP, TGlobalSettings::Instance().DatabasePath, false));
				DBControl.Connect();
				Database::TDBUtilities Utils(DBControl);
				// Utils.SetForcedWrites();

				RetVal = true;
				OpSys OS;

				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				OS.Initialise(DBTransaction);
				DBTransaction.Commit();

				if (OS.MMSoftwareVersion != OS.MMDBVersion)
				{
					MessageBox("This MenuMate version ( " + OS.MMSoftwareVersion + " )\rdoes not match the Database version ( " +
					OS.MMDBVersion + " ).\rYou may have to run Parser.Exe", "Database Mismatch", MB_ICONINFORMATION + MB_OK);
					RetVal = false;
				}
#ifdef MenuMate
				TManagerLogs::Instance().Add("MenuMate POS", VERSIONLOG, OS.SoftwareVersion);
#endif
#ifdef PalmMate
				TManagerLogs::Instance().Add("PalmMate", VERSIONLOG, OS.SoftwareVersion);
#endif

			}
			Abort = false;
		}
		catch(Exception & Err)
		{
			MessageBox("Unable to Connect to MenuMate Database.\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
			CloseDB();
			Abort = false;
			RetVal = false;
		}

		if (RetVal == false)
		{
			// Grey out all the other settings.
			if (frmSystemConfig->ShowModal() == mrOk)
			{
				Abort = false;
			}
			else if (frmSystemConfig->ModalResult == mrAbort)
			{
				Abort = true;
			}
		}
	}
	while (RetVal == false && Abort == false);

	return RetVal;
}

void TDeviceRealControl::LoadHdrFtr()
{
	if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_HEADER))
	{
		TGlobalSettings::Instance().Header->Clear();
		TGlobalSettings::Instance().Header->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_HEADER);
	}
	if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_PRELIM_HEADER))
	{
		TGlobalSettings::Instance().PHeader->Clear();
		TGlobalSettings::Instance().PHeader->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_PRELIM_HEADER);
	}

	if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_FOOTER))
	{
		TGlobalSettings::Instance().Footer->Clear();
		TGlobalSettings::Instance().Footer->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_FOOTER);
	}

	if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_ZEDHEADER))
	{
		TGlobalSettings::Instance().ZedHeader->Clear();
		TGlobalSettings::Instance().ZedHeader->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_ZEDHEADER);
	}
    if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_VOID_FOOTER))
	{
		TGlobalSettings::Instance().VoidFooter->Clear();
		TGlobalSettings::Instance().VoidFooter->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_VOID_FOOTER);
	}
    if (FileExists(ExtractFilePath(Application->ExeName) + RECEIPT_SUBHEADER))
	{
		TGlobalSettings::Instance().SubHeader->Clear();
		TGlobalSettings::Instance().SubHeader->LoadFromFile(ExtractFilePath(Application->ExeName) + RECEIPT_SUBHEADER);
	}

	bool IsRegistered = false;
	UnicodeString pRegisteredName = "";

    bool Registered = false;
    Registered = TGlobalSettings::Instance().IsRegistrationVerified;
    if (Registered)

	{
		if (Receipt)
		{

            Receipt->SetHeaderFooter(TGlobalSettings::Instance().Header.get(), TGlobalSettings::Instance().PHeader.get(), TGlobalSettings::Instance().Footer.get(), TGlobalSettings::Instance().VoidFooter.get(),
            TGlobalSettings::Instance().SubHeader.get());

		}
	}
	else
	{
		TGlobalSettings::Instance().Header->Clear();
		TGlobalSettings::Instance().Header->Add("MenuMate By IQWorks Ltd");
		TGlobalSettings::Instance().Header->Add("Pre-Install Version");
		TGlobalSettings::Instance().Header->Add("NOT TO BE USED IN A COMMERCIAL CONTEXT");
		TGlobalSettings::Instance().Header->Add("PLEASE CONTACT MENUMATE LTD");
		TGlobalSettings::Instance().Header->Add("Phone : 0800 657 300");
		TGlobalSettings::Instance().Header->Add("EMail : Sales@MenuMate.com");
		if (SumStrings(TGlobalSettings::Instance().Header.get()) != 12005)
		{
			ShowMessage("Exe Checksum Failure");
		}
		if (Receipt)
		{
			Receipt->SetHeaderFooter(TGlobalSettings::Instance().Header.get(), TGlobalSettings::Instance().Header.get(), TGlobalSettings::Instance().Header.get(), TGlobalSettings::Instance().Header.get(),TGlobalSettings::Instance().Header.get());
		}
	}

}

bool TDeviceRealControl::ContainsCompanyName(TStrings *inHeader, UnicodeString CompanyName)
{
	UnicodeString WorkingStr = CompanyName.Trim(); // Strip leading and trailing spaces.

	UnicodeString NextStr = "";
	int CurrentIndex = 1;
	do
	{
		NextStr = "";
		// Recive in the next word.
		for (; CurrentIndex <= WorkingStr.Length(); CurrentIndex++)
		{
			if (WorkingStr[CurrentIndex] != ' ')
			{
				NextStr += WorkingStr[CurrentIndex];
			}
			else
			{ // Step over spaces untill the next word is reached.
				while ((CurrentIndex < WorkingStr.Length()) && (WorkingStr[CurrentIndex] == ' '))
				{
					CurrentIndex++;
				}
				// All set up of the next word check the current one.
				break;
			}
		}

		if (NextStr != "")
		{
			bool Wordnotfound = true;
			for (int i = 0; i < inHeader->Count; i++)
			{
				UnicodeString CurrentString = (inHeader->Strings[i]).UpperCase();
				if (CurrentString.Pos(NextStr.UpperCase()) != 0)
				{
					Wordnotfound = false;
				}
			}

			if (Wordnotfound)
			{
				return false;
			}
		}
	}
	while (NextStr != "");

	return true;
}

int TDeviceRealControl::RegisteredPalmLicences()
{
	int RetVal = 1;
#ifdef PalmMate
	const UnicodeString PRODUCT_NAME = "PalmMate";
#endif
#ifdef MenuMate
	const UnicodeString PRODUCT_NAME = "MenuMate";
#endif
	try
	{
		UnicodeString TerminalName = ID.Name;
		if (TerminalName != "")
		{
			TMachineInfo Info;
			Info.ProductName = PRODUCT_NAME;
			Info.TerminalName = TerminalName;

			TRegInfo RegInfo(DBControl);
			if (RegInfo.Registered(&Info))
			{
				RetVal = Info.PalmLicences;
			}
		}
	}
	catch(Exception & E)
	{
		Application->ShowException(&E);
	}

	return RetVal;
}

void TDeviceRealControl::RegisteredModule(bool *pModuleRegistered, eRegisteredModules Module)
{

#ifdef PalmMate
	const UnicodeString PRODUCT_NAME = "PalmMate";
#endif
#ifdef MenuMate
	const UnicodeString PRODUCT_NAME = "MenuMate";
#endif

	try
	{
		*pModuleRegistered = false;

		UnicodeString TerminalName = ID.Name;
		if (TerminalName != "")
		{
			TMachineInfo Info;
			Info.ProductName = PRODUCT_NAME;
			Info.TerminalName = TerminalName;

			TRegInfo RegInfo(DBControl);
			if (RegInfo.Registered(&Info))
			{
				if (Info.MenuMateModules & Module)
				{
					*pModuleRegistered = true;
				}
			}
		}
	}
	catch(Exception & E)
	{
		Application->ShowException(&E);
	}
}

void TDeviceRealControl::Registered(bool *pRegistered, UnicodeString *pRegisteredName)
{

#ifdef PalmMate
	const UnicodeString PRODUCT_NAME = "PalmMate";
#endif
#ifdef MenuMate
	const UnicodeString PRODUCT_NAME = "MenuMate";
#endif

	*pRegistered = false;
	if (pRegisteredName)
	* pRegisteredName = "";
	try
	{
		UnicodeString TerminalName = ID.Name;
		if (TerminalName != "")
		{
			TMachineInfo Info;
			Info.ProductName = PRODUCT_NAME;
			Info.TerminalName = TerminalName;

			TRegInfo RegInfo(DBControl);
			if (!RegInfo.Registered(&Info))
			{
				Info.ProductName = PRODUCT_NAME;
				Info.TerminalName = TerminalName;
				RegInfo.AddRegistrationInformation(&Info);
				if (pRegisteredName)
				* pRegisteredName = "";
				*pRegistered = false;
			}
			else
			{
				if (pRegisteredName)
				* pRegisteredName = Info.CompanyName;
				*pRegistered = true;
			}
		}
	}
	catch(Exception & E)
	{
		*pRegistered = false;
		if (pRegisteredName)
		* pRegisteredName = "";
		Application->ShowException(&E);
	}
}

void TDeviceRealControl::Initialise(Database::TDBTransaction &DBTransaction)
{
	TDevicePC::Initialise(DBTransaction);
	System.Initialise(DBTransaction);
	SecurityPort->Initialise(DBTransaction);
	SecurityPort->Open(SecurityPort->PortNumber);
	ManagerNet->Initialise(DBTransaction);
	TGlobalSettings::Instance().CurrentIPAddress = ManagerNet->GetIPAddress();
}

void TDeviceRealControl::RegisterTransaction(Database::TDBTransaction &DBTransaction)
{
}

TDeviceRealControl::TDeviceRealControl(TManagerMenus *menu_manager)
: ManagerStock(new TManagerStock),
ManagerNet(new TManagerNet()),
raw_menu_manager(menu_manager)
{
	KitchenMod = new TKitchenMod(DBControl);
	KitchenMod->Initialise();
	SecurityPort = new TSecurityPort();

	User.ContactID = 0;
	User.ContactKey = 0;
	User.Name = "";
	User.Initials = "";

	active_instance = this;

	// Menus = NULL;
}

__fastcall TDeviceRealControl::~TDeviceRealControl()
{
	delete KitchenMod;
	delete SecurityPort;
}

// ---------------------------------------------------------------------------


bool TDeviceRealControl::OpenDatabases()
{
	bool Proceed;
	Proceed = OpenMenuMateDB();

	if (!Proceed)
	return Proceed;

	Database::TDBTransaction DBBootTransaction(DBControl);
	DBBootTransaction.StartTransaction();
	TManagerVariable::Instance().InitialisePOSVars(DBBootTransaction);
	DBBootTransaction.Commit();

	TManagerLogs::Instance().Initialise(ID.Name);

	DBBootTransaction.StartTransaction();
	CreateDevice(DBBootTransaction);
	CreateProfile(DBBootTransaction);
	LoadRegistrationConfig();
	Initialise(DBBootTransaction);
	DBBootTransaction.Commit();

	if (!Proceed)
	return Proceed;
	Proceed = OpenStockDB();
	if (!Proceed)
	return Proceed;
	return Proceed;
}

void TDeviceRealControl::CheckLocation(Database::TDBTransaction & DBTransaction)
{
	if (ID.Location == "")
	{
		MessageBox("Please select a location for this terminal.", "Location Request", MB_ICONINFORMATION + MB_OK);
		std::auto_ptr <TfrmLocations> frmLocations(TfrmLocations::Create <TfrmLocations> (Screen->ActiveForm, DBControl));
		do
		{
			frmLocations->ShowModal();
			if (frmLocations->SelectedLocation == "")
			{
				MessageBox("All termianls must have a location.", "Error", MB_OK + MB_ICONWARNING);
			}
		}
		while (frmLocations->SelectedLocation == "");
		ID.Location = frmLocations->SelectedLocation;
		ID.LocationKey = frmLocations->SelectedLocationKey;
		Create(DBTransaction);
	}
}

// ---------------------------------------------------------------------------

bool TDeviceRealControl::SetSiteID(Database::TDBTransaction &DBTransaction, bool Reset)
{
	bool proceed = false;
	if (TGlobalSettings::Instance().SiteID <= 0 || Reset == true)
	{
		std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (Screen->ActiveForm));
		do
		{
			frmTouchNumpad->Caption = "Enter the Site's Site ID";
			frmTouchNumpad->btnSurcharge->Caption = "Ok";
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->Mode = pmNumber;
			frmTouchNumpad->INTInitial = 0;
			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				TGlobalSettings::Instance().SiteID = frmTouchNumpad->INTResult;
				if (TGlobalSettings::Instance().SiteID == 0)
				{
					MessageBox("The Site ID cannot be 0", "Error", MB_OK + MB_ICONERROR);
				}
				else
				{
					int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
					if (GlobalProfileKey == 0)
					{
						GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eSystemProfiles, "Globals");
					}
					TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmSiteID, TGlobalSettings::Instance().SiteID);
					proceed = true;

					TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "UPDATE CONTACTS SET SITE_ID = :SITE_ID WHERE SITE_ID IS NULL OR SITE_ID = -1";
					IBInternalQuery->ParamByName("SITE_ID")->AsInteger = TGlobalSettings::Instance().SiteID;
					IBInternalQuery->ExecQuery();
				}
			}
		}
		while (frmTouchNumpad->ModalResult == mrOk && (TGlobalSettings::Instance().SiteID == 0 || TGlobalSettings::Instance().SiteID == -1));
	}
	else
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE CONTACTS SET SITE_ID = :SITE_ID WHERE SITE_ID IS NULL OR SITE_ID = -1";
		IBInternalQuery->ParamByName("SITE_ID")->AsInteger = TGlobalSettings::Instance().SiteID;
		IBInternalQuery->ExecQuery();

		proceed = true;
	}
	return proceed;
}

bool TDeviceRealControl::OpenStockDB(bool Reconfigure)
{
	std::auto_ptr <TfrmSystemConfig> frmSystemConfig(TfrmSystemConfig::Create(Screen->ActiveForm));
	frmSystemConfig->EnableStock();

	bool RetVal;
	bool Abort = false;

	if (Reconfigure)
	{
		if (frmSystemConfig->ShowModal() == mrOk)
		{
			Abort = false;
		}
		else if (frmSystemConfig->ModalResult == mrAbort)
		{
			Abort = true;
		}
	}

	if (!Abort)
	{
		do
		{
			try
			{
				ManagerStock->OpenDB();
				RetVal = true;
				Abort = false;
			}
			catch(Exception & Err)
			{
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Unable to Connect to Stock Database. " + Err.Message);
				MessageBox("Unable to Connect to Stock Database.\r" + Err.Message, "Error", MB_OK + MB_ICONERROR);
				Abort = false;
				RetVal = false;
			}

			if (RetVal == false)
			{
				if (frmSystemConfig->ShowModal() == mrOk)
				{
					Abort = false;
				}
				else if (frmSystemConfig->ModalResult == mrAbort)
				{
					RetVal = false;
					Abort = true;
				}
			}
		}
		while (RetVal == false && Abort == false);

		if (Abort == false)
		{
			Database::TDBTransaction DBBootTransaction(DBControl);
			DBBootTransaction.StartTransaction();
			TManagerVariable::Instance().SetDeviceStr(DBBootTransaction, vmStockInterbaseIP, TGlobalSettings::Instance().StockInterbaseIP);
			TManagerVariable::Instance().SetDeviceStr(DBBootTransaction, vmStockDatabasePath, TGlobalSettings::Instance().StockDatabasePath);
			DBBootTransaction.Commit();
		}
	}
	return RetVal;
}

void TDeviceRealControl::StatusReport(TStringList *html)
{
	try
	{
		UnicodeString Temp = STATUSREPORT_TOP;
		html->Add(Temp);
		// Load and repalce Course Section.
		TModType::iterator ptrMods = Modules.Status.begin();
		for (; ptrMods != Modules.Status.end(); ptrMods++)
		{
			Temp = STATUSREPORT_MOD_ROW;
			UnicodeString ModName = "<Unknown>";
			switch(ptrMods->first)
			{
			case eRegKitchenScreen:
				ModName = "ChefMate";
				break;
			case eRegRooms:
				ModName = "Rooms";
				break;
			case eRegSaleTurnAround:
				ModName = "Time Tracking";
				break;
			case eRegMembers:
				ModName = "Membership";
				break;
			case eEFTPOS:
				ModName = "Intergrated Eftpos";
				break;
			case eExpires:
				ModName = "Temperory Licence";
				break;
			case ePhoenixHotelSystem:
				ModName = "PMS Hotel System";
				break;
			case eIntaMate:
				ModName = "IntaMate System";
				break;
			case eAccounting:
				ModName = "MYOB Interface";
				break;
			case eSmartCardSystem:
				ModName = "Smart Cards";
				break;
			}
			Temp = AnsiReplaceStr(Temp, "%MOD_NAME%", ModName);
			html->Add(Temp);

			std::map <UnicodeString, Variant> ::iterator ptrSettings = ptrMods->second.begin();
			for (; ptrSettings != ptrMods->second.end(); ptrSettings++)
			{
				Temp = STATUSREPORT_VAL_ROW;
				Temp = AnsiReplaceStr(Temp, "%VARNAME%", ptrSettings->first);
				Temp = AnsiReplaceStr(Temp, "%VAR_VALUE%", ptrSettings->second);
				html->Add(Temp);
			}
		}

		Temp = CSREPORT_BOTTOM;
		html->Add(Temp);
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

TDeviceRealControl::LoadRegistrationConfig()
{
	bool MembersRegistered = false;
	RegisteredModule(&MembersRegistered, eRegMembers);
	Modules.Status[eRegMembers]["Registered"] = MembersRegistered;
	Modules.Status[eRegMembers]["Enabled"] = MembersRegistered;

	bool RoomsRegistered = false;
	RegisteredModule(&RoomsRegistered, eRegRooms);
	Modules.Status[eRegRooms]["Registered"] = RoomsRegistered;

	bool PHSRegistered = false;
	RegisteredModule(&PHSRegistered, ePhoenixHotelSystem);
	Modules.Status[ePhoenixHotelSystem]["Registered"] = PHSRegistered;

	bool IntaMateRegistered = false;
	RegisteredModule(&IntaMateRegistered, eIntaMate);
	Modules.Status[eIntaMate]["Registered"] = IntaMateRegistered;

	bool KitchenRegistered = false;
	RegisteredModule(&KitchenRegistered, eRegKitchenScreen);
	Modules.Status[eRegKitchenScreen]["Registered"] = KitchenRegistered;

	bool SalesRegistered = false;
	RegisteredModule(&SalesRegistered, eRegSaleTurnAround);
	Modules.Status[eRegSaleTurnAround]["Registered"] = SalesRegistered;

	bool EFTPOSRegistered = false;
	RegisteredModule(&EFTPOSRegistered, eEFTPOS);
	Modules.Status[eEFTPOS]["Registered"] = EFTPOSRegistered;

	bool SmartCardsRegistered = false;
	RegisteredModule(&SmartCardsRegistered, eSmartCardSystem);
	SmartCardsRegistered = MembersRegistered;
	Modules.Status[eSmartCardSystem]["Registered"] = SmartCardsRegistered;

    bool AccountinterfaceRegistered = false;
	RegisteredModule(&AccountinterfaceRegistered, eAccounting);
	Modules.Status[eAccounting]["Registered"] = AccountinterfaceRegistered ;
}

int TDeviceRealControl::SumStrings(TStrings *Strings)
{
	int retval = 0;
	for (int i = 1; i <= Strings->Text.Length(); i++)
	{
		retval += (int)(Strings->Text[i]);
	}
	return retval;
}

bool TDeviceRealControl::insertMemberPointsAdjustmentRecordToDB( Database::TDBTransaction &DBTransaction, int contactKey, Currency adjustment )
{
	bool result = false;

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "Select GEN_ID(GEN_POINTSTRANSACTIONS, 1) from rdb$database";
	IBInternalQuery->ExecQuery();

	if(!IBInternalQuery->Eof)
	{
		int pointsTransactionKey = IBInternalQuery->Fields[0]->AsInteger;

		UnicodeString adjustmentSQL =
		"insert into POINTSTRANSACTIONS ( POINTSTRANSACTIONS_KEY, CONTACTS_KEY, TIME_STAMP, ADJUSTMENT_TYPE, ADJUSTMENT_SUBTYPE, ADJUSTMENT, EXPORTED_STATUS, TIME_STAMP_EXPORTED )"
		"values( :POINTSTRANSACTIONS_KEY, :CONTACTS_KEY, :TIME_STAMP, :ADJUSTMENT_TYPE, :ADJUSTMENT_SUBTYPE, :ADJUSTMENT, :EXPORTED_STATUS, :TIME_STAMP_EXPORTED)";

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = adjustmentSQL;
		IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = pointsTransactionKey;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
		IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Now();
		IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = 1; // at the time of implementation, this was the pttsync
		IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = 1; // at the time of implementation, this was ptstAccount
		IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = adjustment;
		IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = 1; //1 being the exported
		IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = Now();
		IBInternalQuery->ExecQuery();

		result = IBInternalQuery->RowsAffected > 0;
	}

	return result;
}

void TDeviceRealControl::UpdateBarStockPrice(Database::TDBTransaction &DBTransaction, AnsiString FileName)
{
	try
	{
		BarExachange barexchang;
		barexchang.IsRequiredBarStockPriceUpdate=false;
		AnsiString ProcFile = FileName;
		std::ifstream FNFile; // stream set up
		FNFile.open(ProcFile.c_str());
		if (!FNFile)                    // Cannot open selected file
		{
			// Error opening file
			AnsiString MsgStr = "I cannot open " + ProcFile + "!";
			MessageDlg(MsgStr, mtError, TMsgDlgButtons() << mbOK, 0);
			return;
		}                               // Cannot open selected file
		else                            // Processing
		{
			std::string ArcLine;
			AnsiString  BreakValue;
			int   ErrorsOccured=0;
			linenumber=0;
			while (getline(FNFile, ArcLine, '\n'))
			{          // Do the processing here
				try
				{
					linenumber=linenumber+1;
					Counter=0;
					vector<string> s = split(ArcLine, ";");
					for (vector<string>::iterator c = s.begin(); c != s.end(); ++c)
					{
						string val= *c;
						BreakValue =  AnsiString(val.c_str());
						if(Counter==0)
						ThirdPartyCode =  BreakValue;
						if(Counter==1)
						price =  BreakValue;
						Counter=Counter+1;
					}

					TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text ="SELECT a.THIRDPARTYCODES_KEY FROM THIRDPARTYCODES a where a.CODE=:CODE " ;
					IBInternalQuery->ParamByName("CODE")->AsString =Trim(ThirdPartyCode)  ;
					IBInternalQuery->ExecQuery();
					AnsiString  TPKey = IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsString;


					IBInternalQuery->Close();
					UnicodeString ThirdPartyCodeUpdatePriceSQL =
					"update ITEMSIZE set ITEMSIZE.price=:price "
					"where ITEMSIZE.THIRDPARTYCODES_KEY=:THIRDPARTYCODES_KEY ";
					IBInternalQuery->SQL->Text = ThirdPartyCodeUpdatePriceSQL;
					IBInternalQuery->ParamByName("price")->AsCurrency =StrToCurr(price) ;
					IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsString = TPKey;
					IBInternalQuery->ExecQuery();
					IBInternalQuery->Close();
					barexchang.IsRequiredBarStockPriceUpdate = true;
				}
				catch(Exception & E)
				{
					ErrorsOccured++;
					AnsiString FailedFile =  TGlobalSettings::Instance().BarExchangePath + "\\Failed\\FailedBs.plu";

					if (!DirectoryExists(ExtractFilePath(FailedFile)))
					{
						ForceDirectories(ExtractFilePath(FailedFile));
					}
					std::auto_ptr <TStringList> List(new TStringList);
					if (FileExists(FailedFile))
					{
						List->LoadFromFile(FailedFile);
					}
					List->Add("Error on Line Number " + IntToStr(linenumber) + " " + E.Message);
					List->Add(ArcLine.c_str());
					List->SaveToFile(FailedFile);
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ("User Failed To Import : Line Number " + IntToStr(linenumber) + " Error Msg : " + E.Message));
				}

			}                           // As long as there is data

			/***********************************/
			if (!DirectoryExists(TGlobalSettings::Instance().BarExchangePath + "\\old Stock Exchange ") )
			{
				ForceDirectories(TGlobalSettings::Instance().BarExchangePath + "\\old Stock Exchange");
			}
			FNFile.close();
            	AnsiString Dest="";
               if( TGlobalSettings::Instance().BarExchangeSelect=="BarStock" )
            {
                Dest =   TGlobalSettings::Instance().BarExchangePath + "\\" + ChangeFileExt(ExtractFileName(FileName),
                ".plu.old");

            }
            else if( TGlobalSettings::Instance().BarExchangeSelect=="The Exchange")
             {
                Dest =   TGlobalSettings::Instance().BarExchangePath + "\\" + ChangeFileExt(ExtractFileName(FileName),
              ".plu.old");
             }
                //DeleteFile(Dest);
			CopyFile(FileName.c_str(), Dest.c_str(), false);
			DeleteFile(FileName);

			if (ErrorsOccured > 0)
			{
				MessageBox(IntToStr(ErrorsOccured) + "/" + IntToStr(linenumber) +
				"  Failed to Import. See the Faileddata.plu in the Failed Folder for Detials.", "Some Imports Failed",
				MB_ICONINFORMATION + MB_OK);
			}
			/************************************/


		}


	}
	catch(Exception & E)
	{


	}

}

bool TDeviceRealControl::SelectBarStockTurnOver(Database::TDBTransaction &DBTransaction, AnsiString FileName)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =

"SELECT THIRDPARTYCODES.CODE THIRDPARTYCODES_KEY, sum(a.QTY) qty FROM DAYARCHIVE a "
"left join THIRDPARTYCODES on THIRDPARTYCODES.THIRDPARTYCODES_KEY=a.THIRDPARTYCODES_KEY "
"where a.THIRDPARTYCODES_KEY<>0 and (a.TIME_STAMP<=cast('NOW' as timestamp) and  a.TIME_STAMP>=dateadd (-30 second to cast('NOW' as timestamp)) )group by 1 "
"union all "
"SELECT THIRDPARTYCODES.CODE THIRDPARTYCODES_KEY, sum(a.QTY) qty FROM ARCHIVE a "
"left join THIRDPARTYCODES on THIRDPARTYCODES.THIRDPARTYCODES_KEY=a.THIRDPARTYCODES_KEY "
"where a.THIRDPARTYCODES_KEY<>0 and (a.TIME_STAMP<=cast('NOW' as timestamp) and  a.TIME_STAMP>=dateadd (-30 second to cast('NOW' as timestamp)) )group by 1 "  ;
	IBInternalQuery->ExecQuery();

        if(!IBInternalQuery->Eof)
        {
            if(!FileExists( FileName ) )
            {
                ForceDirectories(ExtractFilePath(FileName));
                std::auto_ptr<TFileStream> NewFile(new TFileStream(FileName, fmCreate | fmShareDenyRead));
            }
        }

	std::auto_ptr <TStringList> List(new TStringList);

	if (FileExists(FileName))
	{
        std::map<AnsiString, int> finalValues;
        std::map<AnsiString, int>::iterator finalValuesIterator;

        std::map<AnsiString, int> dbValues;
        std::map<AnsiString, int>::iterator dbValuesIterator;

		std::map<AnsiString, int> fileValues;
        std::map<AnsiString, int>::iterator fileValuesIterator;

        std::ifstream FNFile;
        FNFile.open(FileName.c_str());

        if (!FNFile)
		{
			AnsiString MsgStr = "I cannot open " + FileName + "!";
			MessageDlg(MsgStr, mtError, TMsgDlgButtons() << mbOK, 0);
		}

        std::string line;

        while (getline(FNFile, line, '\n'))
        {
            vector<string> lineSplitValues = split(line, ";");

            if(lineSplitValues.size() > 1)
            {
                fileValues.insert(std::pair<AnsiString, int>(AnsiString(Trim((lineSplitValues[0]).c_str())), StrToInt(Trim((lineSplitValues[1]).c_str()))));
            }
        }

        FNFile.close();
        DeleteFile(FileName);

		while (!IBInternalQuery->Eof)
		{
			AnsiString  dbThirdPartyCode = IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsString;
			int  dbQuantity = IBInternalQuery->FieldByName("QTY")->AsInteger;

            int quantity;

            fileValuesIterator = fileValues.find(dbThirdPartyCode);

            if(fileValuesIterator != fileValues.end() && finalValues.find(dbThirdPartyCode) == finalValues.end())
            {
                quantity = dbQuantity + fileValuesIterator->second;
                fileValues.erase(fileValuesIterator);

                finalValues.insert(std::pair<AnsiString, int>(dbThirdPartyCode, quantity));
            }
            else if(dbValues.find(dbThirdPartyCode) == dbValues.end())
            {
                dbValues.insert(std::pair<AnsiString, int>(dbThirdPartyCode, dbQuantity));
            }
			IBInternalQuery->Next();
		}

        List->Clear();

        for (finalValuesIterator = finalValues.begin(); finalValuesIterator != finalValues.end(); ++finalValuesIterator)
        {
            if(finalValuesIterator->first != "0")
            {
                List->Add(finalValuesIterator->first + "; " + AnsiString(finalValuesIterator->second));
            }
        }

        for (dbValuesIterator = dbValues.begin(); dbValuesIterator != dbValues.end(); ++dbValuesIterator)
        {
            if(dbValuesIterator->first != "0")
            {
                List->Add(dbValuesIterator->first + "; " + AnsiString(dbValuesIterator->second));
            }
        }

        for (fileValuesIterator = fileValues.begin(); fileValuesIterator != fileValues.end(); ++fileValuesIterator)
        {
            if(fileValuesIterator->first != "0")
            {
                List->Add(fileValuesIterator->first + "; " + AnsiString(fileValuesIterator->second));
            }
        }

        std::ofstream ofs;
		ofs.open(FileName.c_str(), std::ofstream::out | std::ofstream::trunc);
		ofs.close();
		List->SaveToFile(FileName);
	}
	return true;
}
//--------------------------------------------------------------------------------------




