// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ManagerMembership.h"
#include "SystemConfig.h"
#include "MembershipERS.h"
#include "MembershipExternal.h"
#include "MMLogging.h"
#include "MMMessageBox.h"
#include "DBTab.h"
#include "Processing.h"
#include "ReportDisplayNav.h"
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include <StrUtils.hpp>
#include "Invoice.h"
#include "GlobalSettings.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerMembership::TManagerMembership(Database::TDBControl &inDBControl, TModules &inModules) : DBControl(inDBControl), Modules(inModules), MembershipSystem(NULL), POFMemberSelectionEnabled(false)
{
}

TManagerMembership::~TManagerMembership()
{
}

void TManagerMembership::Initialise(Database::TDBTransaction &DBTransaction)
{
   switch(TGlobalSettings::Instance().MembershipType)
   {
   case MembershipTypeMenuMate:
	  MembershipSystem.reset(new TMembership(Modules));
	  break;
   case MembershipTypeERS:
	  MembershipSystem.reset(new TMembershipERS(Modules, IBMembershipDatabase));
	  break;
   case MembershipTypeExternal:
	  MembershipSystem.reset(new TMembershipExternal(Modules, TGlobalSettings::Instance().MembershipDatabaseIP, TGlobalSettings::Instance().MembershipDatabasePort, TGlobalSettings::Instance().MembershipTillID));
	  break;
   default:
	  MembershipSystem.reset(new TMembership(Modules)); ;
   }
   MembershipSystem->Initialise(DBTransaction);
}

void TManagerMembership::GetValidPointsTransactions(Database::TDBTransaction &DBTransaction, int ContactKey,
   TPointsTransactionContainer &PointsTransactions)
{
   MembershipSystem->GetValidPointsTransactions(DBTransaction, ContactKey, PointsTransactions);
}

void TManagerMembership::UpdatePointsTransactions(Database::TDBTransaction &DBTransaction,
   std::vector <TPointsTransaction> &PointsTransactions)
{
   MembershipSystem->UpdatePointsTransactions(DBTransaction, PointsTransactions);
}

bool TManagerMembership::CheckVersionNumber(Database::TDBTransaction &IBTransaction)
{
   bool RetVal = true;
   if (!MembershipSystem->CheckVersionNumber(IBTransaction))
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Incompatable Membership Database Version");
	  MessageBox("Incompatable Membership Database Version.", "Error", MB_OK + MB_ICONERROR);
	  Modules.Status[eRegMembers]["Enabled"] = false;
	  RetVal = false;
   }
   return RetVal;
}

bool TManagerMembership::OpenMembershipDB(Database::TDBTransaction &IBTransaction, bool Reconfigure)
{
   std::auto_ptr <TfrmSystemConfig> frmSystemConfig(TfrmSystemConfig::Create(Screen->ActiveForm));
   frmSystemConfig->EnableMembership();

   bool RetVal = false;

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
	  if (TGlobalSettings::Instance().MembershipType == MembershipTypeERS)
	  {
		 do
		 {
			try
			{
			   if (TGlobalSettings::Instance().MembershipDatabaseIP != "" && TGlobalSettings::Instance().MembershipDatabasePath != "")
			   {
				  Modules.Status[eRegMembers]["ReadOnly"] = true;
				  IBMembershipDatabase.Init(Database::TDBSettings(TGlobalSettings::Instance().MembershipDatabaseIP, TGlobalSettings::Instance().MembershipDatabasePath, false));
				  IBMembershipDatabase.Connect();
				  Database::TDBUtilities Utils(IBMembershipDatabase);
				  Utils.SetForcedWrites();
				  if (!IBMembershipDatabase.Connected())
				  {
					 IBMembershipDatabase.Connect();
				  }

				  RetVal = true;
				  Abort = false;
				  Modules.Status[eRegMembers]["Enabled"] = true;
				  Initialise(IBTransaction);
			   }
			}
			catch(Exception & Err)
			{
			   TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Unable to Connect to Membership Database. " + Err.Message);
			   MessageBox("Membership is Registered.\rUnable to connect to the Membership DB.\r" + Err.Message, "Error",
				  MB_OK + MB_ICONERROR);
			   Modules.Status[eRegMembers]["Enabled"] = false;
			   IBMembershipDatabase.Disconnect();
			   RetVal = false;
			   Abort = false;
			}

			if (RetVal == false)
			{
			   if (frmSystemConfig->ShowModal() == mrOk)
			   {
				  Abort = false;
			   }
			   else if (frmSystemConfig->ModalResult == mrAbort)
			   {
				  Modules.Status[eRegMembers]["Enabled"] = false;
				  Abort = true;
			   }
			}
		 }
		 while (RetVal == false && Abort == false);
	  }
	  if (TGlobalSettings::Instance().MembershipType == MembershipTypeExternal)
	  {
		 frmSystemConfig->EnableGeneralLedgerMembership();
		 do
		 {

			try
			{
			   if (TGlobalSettings::Instance().MembershipDatabaseIP != "" && TGlobalSettings::Instance().MembershipDatabasePort != 0)
			   {
				  Modules.Status[eRegMembers]["ReadOnly"] = true;
				  // TODO : Blcoking Connect to the EBet TGlobalSettings::Instance().
				  // Create a socket and check it conencts here.
				  RetVal = true;
				  Abort = false;
				  Modules.Status[eRegMembers]["Enabled"] = true;
				  Initialise(IBTransaction);
			   }
			}
			catch(Exception & Err)
			{
			   TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Unable to Connect to General Ledger Membership. " + Err.Message);
			   MessageBox("Membership is Registered.\rUnable to connect to General Ledger Membership.\r" + Err.Message, "Error",
				  MB_OK + MB_ICONERROR);
			   Modules.Status[eRegMembers]["Enabled"] = false;
			   IBMembershipDatabase.Disconnect();
			   RetVal = false;
			   Abort = false;
			}

			if (RetVal == false)
			{
			   // Retrive a new IP and Port here.
			   TGlobalSettings::Instance().MembershipDatabasePort = 10061;
			   if (frmSystemConfig->ShowModal() == mrOk)
			   {
				  Abort = false;
				  if (TGlobalSettings::Instance().MembershipDatabaseIP == "" && TGlobalSettings::Instance().MembershipDatabasePort == 0)
				  {
					 Modules.Status[eRegMembers]["Enabled"] = false;
					 RetVal = true;
				  }
			   }
			   else if (frmSystemConfig->ModalResult == mrAbort)
			   {
				  Modules.Status[eRegMembers]["Enabled"] = false;
				  Abort = true;
			   }
			}
		 }
		 while (RetVal == false && Abort == false);
	  }
	  else
	  {
		 Modules.Status[eRegMembers]["Enabled"] = true;
		 RetVal = true;
	  }
   }

   return RetVal;
}

void TManagerMembership::ArchivePoints(Database::TDBTransaction &DBTransaction,TPointsTransaction &PointsTransaction)
{
   MembershipSystem->LoyaltyAddValue(DBTransaction, PointsTransaction);
}

void TManagerMembership::RegisterTransaction(Database::TDBTransaction &DBTransaction)
{
   if (MembershipSystem.get() != NULL)
   {
	  MembershipSystem->InitTransaction(DBTransaction);
   }
}

TLoginSuccess TManagerMembership::FindMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
   TLoginSuccess LoginSuccess = lsUserNotFound;
   try
   {
	  LoginSuccess = MembershipSystem->FindMember(DBTransaction, UserInfo);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  MessageBox("Unable to load Member information.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
   }
   return LoginSuccess;
}

void TManagerMembership::GetReportMemberInfo(TMMContactInfo &Member, TSyndCode &SyndCode, UnicodeString Title, TStringList *Report)
{
   // Add the Membership Block.
   UnicodeString Temp = TABLE_START;
   Report->Add(Temp);
   Temp = TABLE_ROW4_HEADER;
   Temp = AnsiReplaceStr(Temp, "%TABLETITLE%", Title);
   Temp = AnsiReplaceStr(Temp, "%TABLEHEADER%", " " + Member.Title + " " + Member.Name + " (" + Member.MembershipNumber + ")");
   Report->Add(Temp);

   UnicodeString TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Alias");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.Alias == "" ? UnicodeString("&nbsp;") : Member.Alias);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Initials");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", Member.Initials == "" ? UnicodeString("&nbsp;") : Member.Initials);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;

   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Location Address");
   UnicodeString TempAdd = Member.LocationAddress;
   if (TempAdd == "")
	  TempAdd = UnicodeString("&nbsp;");
   TempAdd = AnsiReplaceStr(TempAdd, "\r", "<br>");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", TempAdd);

   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Mailing Address");
   TempAdd = Member.MailingAddress;
   if (TempAdd == "")
	  TempAdd = UnicodeString("&nbsp;");
   TempAdd = AnsiReplaceStr(TempAdd, "\r", "<br>");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", TempAdd);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Phone");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.Phone == "" ? UnicodeString("&nbsp;") : Member.Phone);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "EMail");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", Member.EMail == "" ? UnicodeString("&nbsp;") : Member.EMail);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "DOB");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.DateOfBirth.FormatString(" dd/mm/yyyy "));
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Mobile");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", Member.Mobile == "" ? UnicodeString("&nbsp;") : Member.Mobile);
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Gender");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", Member.Sex == "" ? UnicodeString("&nbsp;") : Member.Sex);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", "Last Modified");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%", Member.LastModified.FormatString(" dd-mm-yy "));
   Report->Add(TempRow);
   TempRow = TABLE_ROW4;
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE1%", "Syndicate Coding");
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT1%", SyndCode.Name);
   TempRow = AnsiReplaceStr(TempRow, "%ROWTITLE2%", UnicodeString("&nbsp;"));
   TempRow = AnsiReplaceStr(TempRow, "%ROWCONTENT2%",  UnicodeString("&nbsp;"));
   Report->Add(TempRow);
   TempRow = TABLE_STOP;
   Report->Add(TempRow);

   Member.Points.toHTML(Report);

}
void TManagerMembership::SyncMembers(Database::TDBTransaction &DBTransaction, TfrmProcessing *Progress)
{
   try
   {
	  MembershipSystem->SyncMembers(DBTransaction, Progress);
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Err.Message + "Syncing Members.");
   }
}

TModalResult TManagerMembership::DeleteMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
   TModalResult Result = mrCancel;
   if (MembershipSystem->ReadOnlyInterface)
   {
	  MessageBox("You must Add,Edit and Delete Members from your 3rd Party Membership software.", "Warning", MB_ICONWARNING + MB_OK);
   }
   else
   {
	  if (MessageBox("Are you sure you wish to remove this Member?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
	  {
		 int ContactKey = Info.ContactKey;
		 if (Invoice->GetOutStanding(DBTransaction, ContactKey) || TDBTab::GetOutStanding(DBTransaction, ContactKey))
		 {
			MessageBox("You must clear all outstanding Invoices and Tabs before this person can be removed.", "Warning",
			   MB_ICONWARNING + MB_OK);
		 }
		 else
		 {
			MembershipSystem->DeleteContact(DBTransaction, ContactKey);
		 }
	  }
   }
   return Result;
}

TLoginSuccess TManagerMembership::Authorise(TMMContactInfo &UserInfo,Currency wrkPayAmount)
{
   TLoginSuccess LoginSuccess = lsDenied;
   try
   {
	  LoginSuccess = MembershipSystem->Authorise(UserInfo, wrkPayAmount);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return LoginSuccess;
}


TLoginSuccess TManagerMembership::IsCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo)
{
   TLoginSuccess LoginSuccess = lsDenied;
   try
   {
	  LoginSuccess = MembershipSystem->IsCard(DBTransaction, UserInfo);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  MessageBox("Unable to check member card information.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
   }
   return LoginSuccess;
}

bool TManagerMembership::LoyaltyMemberSelected(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode,bool triggeredByCard)
{
   bool memberExist = false;
   try
   {
      memberExist = MembershipSystem->LoyaltyMemberSelected(DBTransaction, UserInfo, memberCardCode,triggeredByCard);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  MessageBox("Unable to load Member information.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
   }
   return false;
}


bool TManagerMembership::UpdateMemberCardCode(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode)
{
   bool memberExist = false;
   try
   {
      memberExist = MembershipSystem->UpdateMemberCardCode(DBTransaction, UserInfo,memberCardCode);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  MessageBox("Unable to update Member information.\r" + E.Message, "Error", MB_OK + MB_ICONERROR);
   }
   return false;
}


