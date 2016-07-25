// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <DateUtils.hpp>
#include <set>
#include <Memory>

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "ContactStaff.h"
#include "MMLogging.h"
#include "Login.h"

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// -----------------------------TContactStaff---------------------------------
// ---------------------------------------------------------------------------
TContactStaff::TContactStaff(Database::TDBTransaction &DBTransaction)
{
   ContactType = eStaff;
   ContactTabType = TabStaff;
   DisableManualLogin = false;
   Enabled = true;
   DisableManualLogin = TManagerVariable::Instance().GetBool(DBTransaction, vmDisableManualLogin, false);
}

TDateTime TContactStaff::GetLastUserReportTime(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   TDateTime RetVal = TDateTime(0);
   if (!fEnabled)
	  return RetVal;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "MAX(TIME_STAMP) TIME_STAMP " "FROM " "CONTACTROLLOVER " "WHERE "
		  "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TContactStaff::SetLastUserReportTime(Database::TDBTransaction &DBTransaction, int inContactKey, TDateTime DateTime)
{
   if (!fEnabled)
	  return;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTROLLOVER, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  int KeyVal = IBInternalQuery->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO CONTACTROLLOVER (" "CONTACTROLLOVER_KEY," "CONTACTS_KEY," "TIME_STAMP)" "VALUES (" ":CONTACTROLLOVER_KEY,"
		  ":CONTACTS_KEY," ":TIME_STAMP);";
	  IBInternalQuery->ParamByName("CONTACTROLLOVER_KEY")->AsInteger = KeyVal;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = DateTime;
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

TLoginSuccess TContactStaff::TestIDLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo, SecurityCheck Destination,
   UnicodeString &Description)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess Success = lsPINIncorrect;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "Select CONTACTS_KEY,ACCESS_LEVEL from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and CONTACT_ID = :CONTACT_ID and PIN = :PIN";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("CONTACT_ID")->AsInteger = UserInfo.ContactID;
	  IBInternalQuery->ParamByName("PIN")->AsString = UserInfo.PIN;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 bool AccessSuccess = TestAccessLevel((TAccess)IBInternalQuery->FieldByName("ACCESS_LEVEL")->AsInt64, Destination, Description);
		 if (AccessSuccess)
		 {
			Success = lsAccepted;
			GetContactDetails(DBTransaction, IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger, UserInfo);
		 }
		 else
		 {
			Success = lsDenied;
		 }
	  }
	  else
	  {
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

TLoginSuccess TContactStaff::TestKeyLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo, SecurityCheck Destination,
   UnicodeString &Description)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess Success = lsPINIncorrect;
   try
   {

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "Select CONTACTS_KEY,ACCESS_LEVEL from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and CONTACTS_KEY = :CONTACTS_KEY and PIN = :PIN";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = UserInfo.ContactKey;
	  IBInternalQuery->ParamByName("PIN")->AsString = UserInfo.PIN;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 bool AccessSuccess = TestAccessLevel((TAccess)IBInternalQuery->FieldByName("ACCESS_LEVEL")->AsInt64, Destination, Description);
		 if (AccessSuccess)
		 {
			Success = lsAccepted;
			TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestKeyLogin lsAccepted Found User: " + UserInfo.Name);
		 }
		 else
		 {
			Success = lsDenied;
			TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestKeyLogin lsDenied (Bad Pin) User: " + IntToStr(UserInfo.ContactKey));
		 }
	  }
	  else
	  {
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

TLoginSuccess TContactStaff::TestCardLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo, SecurityCheck Destination,
   UnicodeString &Description)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess Success = lsPINIncorrect;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  UserInfo.ContactKey = GetContactByCard(DBTransaction, UserInfo);
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "Select CONTACTS_KEY,ACCESS_LEVEL from CONTACTS where CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = UserInfo.ContactKey;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 bool AccessSuccess = TestAccessLevel((TAccess)IBInternalQuery->FieldByName("ACCESS_LEVEL")->AsInt64, Destination, Description);
		 if (AccessSuccess)
		 {
			Success = lsAccepted;
			TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestCardLogin lsAccepted Found User: " + UserInfo.Name);
		 }
		 else
		 {
			Success = lsDenied;
			TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestCardLogin lsDenied Card: " + UserInfo.CardStr);
		 }
	  }
	  else
	  {
		 Success = lsUserNotFound;
		 TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestCardLogin lsUserNotFound Card: " + UserInfo.CardStr);
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

TLoginSuccess TContactStaff::TestProxLogin(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo, SecurityCheck Destination,
   UnicodeString &Description)
{
   if (!fEnabled)
	  return lsUserNotFound;
   TLoginSuccess Success = lsPINIncorrect;
   try
   {

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "Select CONTACTS_KEY,ACCESS_LEVEL from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and PROX_CARD = :PROX_CARD";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("PROX_CARD")->AsString = UserInfo.ProxStr;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 bool AccessSuccess = TestAccessLevel((TAccess)IBInternalQuery->FieldByName("ACCESS_LEVEL")->AsInt64, Destination, Description);
		 if (AccessSuccess)
		 {
			Success = lsAccepted;
            UserInfo.ContactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestProxLogin lsAccepted Found User: " + UserInfo.Name);
		 }
		 else
		 {
			Success = lsDenied;
			TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestProxLogin lsDenied Prox: " + UserInfo.ProxStr);
		 }
	  }
	  else
	  {
		 Success = lsUserNotFound;
		 TManagerLogs::Instance().Add(__FUNC__, DEBUGLOG, "TestProxLogin lsUserNotFound Prox: " + UserInfo.ProxStr);
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

bool TContactStaff::TestAccessLevel(TAccess UserAccess, SecurityCheck RequiredAccess, UnicodeString &Description)
{
   if (!fEnabled)
	  return false;
   bool Success = false;
   switch(RequiredAccess)
   {
   case CheckNone:
	  Success = true;
	  Description = "None";
	  break;
   case CheckPOS:
	  Success = UserAccess.POS;
	  Description = "P.O.S System";
	  break;
   case CheckFoodDrinkSales:
	  Success = UserAccess.FoodDrinkSales;
	  Description = "Food and Drink Sales System";
	  break;
   case CheckCredit:
	  Success = UserAccess.Credit;
	  Description = "Credit System";
	  break;
   case CheckPriceAj:
	  Success = UserAccess.PriceAj;
	  Description = "Billing System";
	  break;
   case CheckDiscountBill:
	  Success = UserAccess.DiscountBill;
	  Description = "Discount Bill";
	  break;
   case CheckReservations:
	  Success = UserAccess.Reservations;
	  Description = "Reservations System";
	  break;
   case CheckZeds:
	  Success = UserAccess.PrintZeds;
	  Description = "Analysis System";
	  break;
   case CheckMaintenance:
	  Success = UserAccess.Maintenance;
	  Description = "Maintenance System";
	  break;
   case CheckMenuEditor:
	  Success = UserAccess.MenuEditor;
	  Description = "Menu Editor System";
	  break;
   case CheckViewMembersDetials:
	  Success = UserAccess.ViewMembersDetials;
	  Description = "Drinks Editor System";
	  break;
   case CheckPrinters:
	  Success = UserAccess.Printers;
	  Description = "Printers System";
	  break;
   case CheckUsers:
	  Success = UserAccess.Users;
	  Description = "User Editor System";
	  break;
   case CheckSetup:
	  Success = UserAccess.Setup;
	  Description = "Setup and Configuration System";
	  break;
   case CheckDisable:
	  Success = UserAccess.DisableFoodDrink;
	  Description = "Menu Management System";
	  break;
   case CheckPINChange:
	  Success = UserAccess.PINChange;
	  Description = "PIN Change";
	  break;
   case CheckQuit:
	  Success = UserAccess.Quit;
	  Description = "Quit";
	  break;
   case CheckPaymentTypes:
	  Success = UserAccess.PaymentTypes;
	  Description = "Payment Types";
	  break;
   case CheckSetFloat:
	  Success = UserAccess.SetFloat;
	  Description = "Set Float";
	  break;
   case CheckAdjustFloat:
	  Success = UserAccess.AdjustFloat;
	  Description = "Adjust Float";
	  break;
   case CheckPaymentTypesSec1:
	  Success = UserAccess.PaymentTypesSec1;
	  Description = "Secure Payment 1";
	  break;
   case CheckPaymentTypesSec2:
	  Success = UserAccess.PaymentTypesSec2;
	  Description = "Secure Payment 2";
	  break;
   case CheckPaymentTypesSec3:
	  Success = UserAccess.PaymentTypesSec3;
	  Description = "Secure Payment 3";
	  break;
   case CheckTabPINOverride:
	  Success = UserAccess.TabPINOverride;
	  Description = "Tab Override";
	  break;
   case CheckAccountManager:
	  Success = UserAccess.AccountManager;
	  Description = "Account Manager";
	  break;
   case CheckOpenDrawer:
	  Success = UserAccess.OpenDrawer;
	  Description = "Open Drawer";
	  break;
   case CheckPayInvoice:
	  Success = UserAccess.PayInvoice;
	  Description = "Pay Invoice";
	  break;
   case CheckChargeToInvoice:
	  Success = UserAccess.ChargeToInvoice;
	  Description = "Charge To Invoice";
	  break;
   case CheckAccountCreation:
	  Success = UserAccess.AccountCreation;
	  Description = "Account Creation";
	  break;
   case CheckWriteOff:
	  Success = UserAccess.WriteOff;
	  Description = "Write Off";
	  break;
   case CheckReprintReceipt:
	  Success = UserAccess.ReprintReceipt;
	  Description = "Reprint Receipt";
	  break;
 case CheckPaymentAccess:
	  Success = UserAccess.PaymentAccess;
	  Description = "Payment Access";
	  break;
   case CheckTaxRemoval:
	  Success = UserAccess.TaxRemoval;
	  Description = "Tax Removal";
	  break;
   case CheckComboDiscounts:
	  Success = UserAccess.ComboDiscounts;
	  Description = "Combo Discounts";
	  break;
   case CheckTransactionAuditAccess:
	  Success = UserAccess.TransactionAuditAccess;
	  Description = "View Transaction Audit";
	  break;
   case CheckAllowReprintOrders:
      Success = UserAccess.AllowReprintOrders;
      Description = "Allow Reprint Orders";
      break;
  case CheckAllowForcedHappyHour:
      Success = UserAccess.AllowForcedHappyHour;
      Description = "Allow Forced Happy Hour";
      break;

   }
   return Success;
}

bool TContactStaff::TestAccessLevel(TMMContactInfo &UserInfo, SecurityCheck RequiredAccess)
{
   if (!fEnabled)
	  return false;
   bool Success = false;
   switch(RequiredAccess)
   {
   case CheckNone:
	  Success = true;
	  break;
   case CheckPOS:
	  Success = UserInfo.AccessLevel.POS;
	  break;
   case CheckFoodDrinkSales:
	  Success = UserInfo.AccessLevel.FoodDrinkSales;
	  break;
   case CheckCredit:
	  Success = UserInfo.AccessLevel.Credit;
	  break;
   case CheckPriceAj:
	  Success = UserInfo.AccessLevel.PriceAj;
	  break;
   case CheckDiscountBill:
	  Success = UserInfo.AccessLevel.DiscountBill;
	  break;
   case CheckReservations:
	  Success = UserInfo.AccessLevel.Reservations;
	  break;
   case CheckZeds:
	  Success = UserInfo.AccessLevel.PrintZeds;
	  break;
   case CheckMaintenance:
	  Success = UserInfo.AccessLevel.Maintenance;
	  break;
   case CheckMenuEditor:
	  Success = UserInfo.AccessLevel.MenuEditor;
	  break;
   case CheckViewMembersDetials:
	  Success = UserInfo.AccessLevel.ViewMembersDetials;
	  break;
   case CheckPrinters:
	  Success = UserInfo.AccessLevel.Printers;
	  break;
   case CheckUsers:
	  Success = UserInfo.AccessLevel.Users;
	  break;
   case CheckSetup:
	  Success = UserInfo.AccessLevel.Setup;
	  break;
   case CheckDisable:
	  Success = UserInfo.AccessLevel.DisableFoodDrink;
	  break;
   case CheckPINChange:
	  Success = UserInfo.AccessLevel.PINChange;
	  break;
   case CheckQuit:
	  Success = UserInfo.AccessLevel.Quit;
	  break;
   case CheckPaymentTypes:
	  Success = UserInfo.AccessLevel.PaymentTypes;
	  break;
   case CheckSetFloat:
	  Success = UserInfo.AccessLevel.SetFloat;
	  break;
   case CheckAdjustFloat:
	  Success = UserInfo.AccessLevel.AdjustFloat;
	  break;
   case CheckPaymentTypesSec1:
	  Success = UserInfo.AccessLevel.PaymentTypesSec1;
	  break;
   case CheckPaymentTypesSec2:
	  Success = UserInfo.AccessLevel.PaymentTypesSec2;
	  break;
   case CheckPaymentTypesSec3:
	  Success = UserInfo.AccessLevel.PaymentTypesSec3;
	  break;
   case CheckTabPINOverride:
	  Success = UserInfo.AccessLevel.TabPINOverride;
	  break;
   case CheckAccountManager:
	  Success = UserInfo.AccessLevel.AccountManager;
	  break;
   case CheckOpenDrawer:
	  Success = UserInfo.AccessLevel.OpenDrawer;
	  break;
   case CheckPayInvoice:
	  Success = UserInfo.AccessLevel.PayInvoice;
	  break;
   case CheckChargeToInvoice:
	  Success = UserInfo.AccessLevel.ChargeToInvoice;
	  break;
   case CheckAccountCreation:
	  Success = UserInfo.AccessLevel.AccountCreation;
	  break;
   case CheckWriteOff:
	  Success = UserInfo.AccessLevel.WriteOff;
	  break;
   case CheckReprintReceipt:
	  Success = UserInfo.AccessLevel.ReprintReceipt;
	  break;
   case CheckPaymentAccess:
	  Success = UserInfo.AccessLevel.PaymentAccess;
	  break;
   case CheckTaxRemoval:
	  Success = UserInfo.AccessLevel.TaxRemoval;
	  break;
   case CheckComboDiscounts:
	  Success = UserInfo.AccessLevel.ComboDiscounts;
	  break;
   case CheckAllowPointsRefund:
      Success = UserInfo.AccessLevel.AllowPointsRefund;
      break;
   case CheckAllowReprintOrders:
      Success = UserInfo.AccessLevel.AllowReprintOrders;
      break;
   case CheckAllowForcedHappyHour:
      Success = UserInfo.AccessLevel.AllowForcedHappyHour;
      break;
   }
   return Success;
}

TLoginSuccess TContactStaff::Login(TForm* LoginFormOwner, Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,
   SecurityCheck RequiredAccess)
{
   TLoginSuccess LoginSuccess = lsUserNotFound;
   if (!fEnabled)
	  return LoginSuccess;
   try
   {
	  if (UserInfo.CardStr != "" || UserInfo.ProxStr != "")
	  {
		 // Automatically login this user.
		 UnicodeString Description;
		 if (UserInfo.CardStr != "")
		 {
			LoginSuccess = TestCardLogin(DBTransaction, UserInfo, RequiredAccess, Description);
		 }
		 if (LoginSuccess == lsUserNotFound && UserInfo.ProxStr != "")
		 {
			LoginSuccess = TestProxLogin(DBTransaction, UserInfo, RequiredAccess, Description);
		 }
	  }
	  else if (UserInfo.ContactKey != 0 && UserInfo.PIN != "")
	  {
		 // Try to log this user in.
		 UnicodeString Description;
		 LoginSuccess = TestKeyLogin(DBTransaction, UserInfo, RequiredAccess, Description);
	  }
	  else
	  {
		 std::auto_ptr <TfrmLogin> frmLogin(TfrmLogin::Create <TfrmLogin> (LoginFormOwner, DBTransaction));
		 frmLogin->Contacts = this;
		 frmLogin->DisableManualLogin = DisableManualLogin;
         frmLogin->CheckRequiredAccess = RequiredAccess;

		 // Global Staff Message.
		 UnicodeString StaffMessageOfTheDay = "";
		 int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
		 if (GlobalProfileKey != 0)
		 {
			TManagerVariable::Instance().GetProfileStr(DBTransaction, GlobalProfileKey, vmStaffMessageOfTheDay, StaffMessageOfTheDay);
			if (StaffMessageOfTheDay == "")
			{
			   frmLogin->memStaffMessage->Text = TManagerVariable::Instance().GetStr(DBTransaction, vmStaffMessageOfTheDay, "");
			}
			else
			{
			   frmLogin->memStaffMessage->Text = StaffMessageOfTheDay;
			}
		 }
		 else
		 {
			frmLogin->memStaffMessage->Text = TManagerVariable::Instance().GetStr(DBTransaction, vmStaffMessageOfTheDay, "");
		 }

		 TModalResult Result = frmLogin->ShowModal();
		 if (Result == mrOk)
		 {
			UserInfo.ContactKey = frmLogin->LastUserKey;
			UserInfo.PIN = frmLogin->Password;
			UnicodeString Description;
			LoginSuccess = TestKeyLogin(DBTransaction, UserInfo, RequiredAccess, Description);
		 }
		 else if (Result == mrRetry)
		 {
			UserInfo.ContactKey = frmLogin->LastUserKey;
			UserInfo.PIN = frmLogin->Password;
			LoginSuccess = AdminAccountToGodMode(DBTransaction, frmLogin->Password, frmLogin->LastUserKey);
		 }
		 else if (Result == mrCancel)
		 {
			TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmRootAccount, 0);
			LoginSuccess = lsCancel;
		 }
	  }

	  int RootAccount = TManagerVariable::Instance().GetInt(DBTransaction, vmRootAccount, 0);
	  if (LoginSuccess == lsDenied && UserInfo.ContactKey == RootAccount)
	  {
		 if (TManagerVariable::Instance().GetTime(DBTransaction, vmRootAccountTimer) > Now())
		 {
			LoginSuccess = lsAccepted;
		 }
		 else
		 {
			TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmRootAccount, 0);
		 }
	  }

      if(LoginSuccess == lsAccepted)
      {
   	  	GetContactDetails(DBTransaction, UserInfo.ContactKey, UserInfo);
      }

	  if (RootAccount != 0 && UserInfo.ContactKey != RootAccount)
	  {
		 TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmRootAccount, 0);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return LoginSuccess;
}

TLoginSuccess TContactStaff::AdminAccountToGodMode(Database::TDBTransaction &DBTransaction, UnicodeString PIN, int AccountKey)
{
   TLoginSuccess LoginSuccess = lsPINIncorrect;
   bool CreateAccount = false;
   Word Year, Month, Day, Hour, Miniute, Second, MilliSecond;
   DecodeDateTime(Now(), Year, Month, Day, Hour, Miniute, Second, MilliSecond);
   UnicodeString sMonth = UnicodeString(FormatFloat("00", Month));
   UnicodeString sDay = UnicodeString(FormatFloat("00", Day));
   UnicodeString AdminPin = "";
   int iMonth = StrToInt(sMonth[1]) + StrToInt(sMonth[2]);
   int iDay = StrToInt(sDay[1]) + StrToInt(sDay[2]);
   sMonth = UnicodeString(FormatFloat("00", iMonth));
   sDay = UnicodeString(FormatFloat("00", iDay));

   try
   {
	  if (PIN.Length() == 6)
	  {
		 if (abs(StrToInt(PIN[1]) - StrToInt(PIN[4])) == 3)
		 {
			if (PIN[2] == sMonth[1] && PIN[5] == sMonth[2])
			{
			   if (PIN[3] == sDay[1] && PIN[6] == sDay[2])
			   {
				  CreateAccount = true;
			   }
			}
		 }
	  }

	  if (CreateAccount)
	  {
		 int RootAccountKey = TManagerVariable::Instance().GetInt(DBTransaction, vmRootAccount, 0);
		 TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmRootAccount, AccountKey);
		 TManagerVariable::Instance().SetDeviceTime(DBTransaction, vmRootAccountTimer, IncMinute(Now(), 15));
		 LoginSuccess = lsAccepted;
	  }
   }
   catch(Exception & E)
   {

   }
   return LoginSuccess;
}

void TContactStaff::BuildXMLListStaff(Database::TDBTransaction &DBTransaction, TPOS_XMLBase &Data)
{
   try
   {
	  // Update the IntaMate ID with the Invoice Number.
	  Data.Doc.Clear();

	  TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	  Data.Doc.LinkEndChild(decl);

	  // Insert DOCTYPE definiation here.
	  TiXmlElement * List = new TiXmlElement(xmlEleListStaff);
	  List->SetAttribute(xmlAttrID, AnsiString(Data.IntaMateID).c_str());
	  List->SetAttribute(xmlAttrSiteID, TGlobalSettings::Instance().SiteID);

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT *" " FROM " "  CONTACTS" " WHERE" " CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount && List != NULL)
	  {
		 for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		 {
			TiXmlElement *EleStaff = new TiXmlElement(xmlEleStaff);
			EleStaff->SetAttribute(xmlAttrID, IBInternalQuery->FieldByName("CONTACT_ID")->AsInteger);
			EleStaff->SetAttribute(xmlAttrXmlID, IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger);
			EleStaff->SetAttribute(xmlAttrName, IBInternalQuery->FieldByName("NAME")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrTitle, IBInternalQuery->FieldByName("TITLE")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrSex, IBInternalQuery->FieldByName("SEX")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrFirstName, IBInternalQuery->FieldByName("FIRST_NAME")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrLastName, IBInternalQuery->FieldByName("MIDDLE_NAME")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrMiddleName, IBInternalQuery->FieldByName("LAST_NAME")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrInitials, IBInternalQuery->FieldByName("INITIALS")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrLocation, IBInternalQuery->FieldByName("LOCATION_ADDRESS")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrMailing, IBInternalQuery->FieldByName("MAILING_ADDRESS")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrPhone, IBInternalQuery->FieldByName("PHONE")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrMobile, IBInternalQuery->FieldByName("MOBILE")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrFax, IBInternalQuery->FieldByName("FAX")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrEmail, IBInternalQuery->FieldByName("EMAIL")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrProxCard, IBInternalQuery->FieldByName("PROX_CARD")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrKnownAs, IBInternalQuery->FieldByName("KNOWN_AS")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrDOB, IBInternalQuery->FieldByName("DATEOFBIRTH")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrLastMod, IBInternalQuery->FieldByName("LAST_MODIFIED")->AsString.t_str());
			EleStaff->SetAttribute(xmlAttrPayrollID, IBInternalQuery->FieldByName("PAYROLL_ID")->AsString.t_str());

			Database::TcpIBSQL IBInternalQuery1(new TIBSQL(NULL));
			DBTransaction.RegisterQuery(IBInternalQuery1);
			IBInternalQuery1->Close();
			IBInternalQuery1->SQL->Text = "SELECT * FROM CONTACTCARDS WHERE CONTACTS_KEY = :CONTACTS_KEY";
			IBInternalQuery1->ParamByName("CONTACTS_KEY")->AsInteger = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			IBInternalQuery1->ExecQuery();

			TiXmlElement *MagCards = new TiXmlElement(xmlEleMagCards);
			for (; !IBInternalQuery1->Eof; IBInternalQuery1->Next())
			{
			   TiXmlElement *MagCard = new TiXmlElement(xmlEleMagCard);
			   MagCard->SetAttribute(xmlAttrID, IBInternalQuery1->FieldByName("CONTACTCARDS_KEY")->AsInteger);
			   MagCard->SetAttribute(xmlAttrCard, IBInternalQuery1->FieldByName("SWIPE_CARD")->AsString.t_str());
			   MagCards->LinkEndChild(MagCard);
			}
			EleStaff->LinkEndChild(MagCards);

			List->LinkEndChild(EleStaff);
		 }
	  }
	  Data.Doc.LinkEndChild(List);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

bool TContactStaff::CheckUserAccessLogin(Database::TDBTransaction &DBTransaction, int contact_key, SecurityCheck Destination,
   UnicodeString &Description, TMMContactInfo &UserInfo)
{
   bool Success = false;
   try
   {

	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "Select CONTACTS_KEY,ACCESS_LEVEL from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and CONTACTS_KEY = :CONTACTS_KEY ";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contact_key;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 bool AccessSuccess = TestAccessLevel((TAccess)IBInternalQuery->FieldByName("ACCESS_LEVEL")->AsInt64, Destination, Description);
		 if (AccessSuccess)
		 {
			Success = true;
            GetContactDetails(DBTransaction, contact_key, UserInfo);
		 }
		 else
		 {
			Success = false;
		 }
	  }
	  else
	  {
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
