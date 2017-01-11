// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Contact.h"
#include "MMLogging.h"
#include <Memory>
#include <Sysutils.hpp>
#include "Stream.h"

#include "ManagerVariable.h"
#include "SmartCardDefs.h"
#include <iterator>
#include "PointsRulesSetUtils.h"
#include "DBContacts.h"
#pragma package(smart_init)

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// -----------------------------TContact--------------------------------------
// ---------------------------------------------------------------------------

TContact::TContact()
{
	Enabled = false;
   TabsEnabled = false;
}

TContact::~TContact()
{
}

void TContact::Initialise(Database::TDBTransaction &DBTransaction)
{

}

void __fastcall TContact::SetEnabled(bool value)
{
   if (fEnabled != value)
   {
	  fEnabled = value;
   }
}

bool __fastcall TContact::GetEnabled()
{
   return fEnabled;
}

UnicodeString TContact::GetContactNameByCard(Database::TDBTransaction &DBTransaction,TMMContactInfo &Info)
{
   return GetContactName(DBTransaction, GetContactByCard(DBTransaction, Info));
}

UnicodeString TContact::GetContactName(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   if (!fEnabled)
 	  return "";

   UnicodeString RetVal = "";
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT " "NAME, " " LAST_NAME " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("NAME")->AsString + " " + IBInternalQuery->FieldByName("LAST_NAME")->AsString;
	  }
      return RetVal;

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);

   }
   return RetVal;
}

UnicodeString TContact::GetContactNumber(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   if (!fEnabled)
	  return "";

   UnicodeString RetVal = "";
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "MEMBER_NUMBER " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IntToStr(IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsInteger);
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);

   }
   return RetVal;
}

TContactType TContact::GetContactType(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   if (!fEnabled)
	  return eContactTypeUnknown;

   TContactType RetVal = eContactTypeUnknown;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "CONTACT_TYPE " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = (TContactType)IBInternalQuery->FieldByName("CONTACT_TYPE")->AsInteger;
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

UnicodeString TContact::GetContactInitials(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   if (!fEnabled)
	  return "";

   UnicodeString RetVal = "";
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "INITIALS " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("INITIALS")->AsString;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

int TContact::GetContactByCard(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "CONTACTS.CONTACTS_KEY " "FROM "
		  "CONTACTCARDS INNER JOIN CONTACTS ON CONTACTCARDS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY " "WHERE " "SWIPE_CARD = :SWIPE_CARD AND "
		  "CONTACT_TYPE = :CONTACT_TYPE AND CONTACTCARDS.IS_ACTIVE = 'T'";
	  IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = Info.CardStr;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
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

int TContact::GetContactKey(Database::TDBTransaction &DBTransaction, int in3rdPartyContactKey)
{
   if (!fEnabled)
	  return 0;
   return in3rdPartyContactKey;
}

bool TContact::UpdateContactKey(Database::TDBTransaction &DBTransaction, int OldContactKey, int NewContactKey) // OldKey,NewKey
{
   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  if (GetContact(DBTransaction, NewContactKey) == 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "UPDATE CONTACTS " "SET " "CONTACTS_KEY = :NEWCONTACTS_KEY " "WHERE "
			 "CONTACTS_KEY	= :OLDCONTACTS_KEY";
		 IBInternalQuery->ParamByName("NEWCONTACTS_KEY")->AsInteger = NewContactKey;
		 IBInternalQuery->ParamByName("OLDCONTACTS_KEY")->AsInteger = OldContactKey;
		 IBInternalQuery->ExecQuery();
		 RetVal = true;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
   return RetVal;
}

void TContact::SetContactCard(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString Card)
{
   if (!fEnabled)
	  return;

   try
   {
		TDBContacts::SetContactCard(DBTransaction, inContactKey, Card);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

void TContact::DeleteContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return;

   try
   {
		 TDBContacts::DeleteContactDiscounts(DBTransaction,inContactKey);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

void TContact::SetContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey, int DiscountKey)
{
   if (!fEnabled)
	  return;

   try
   {
	  TDBContacts::SetContactDiscounts(DBTransaction,inContactKey,DiscountKey);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

int TContact::GetExternalKey(Database::TDBTransaction &DBTransaction, int InternalKey)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);
	  // TODO: ExternalKey is probably going to be membership number.
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT CONTACTS_3RDPARTY_KEY FROM CONTACTS WHERE CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = InternalKey;
	  IBInternalQuery->ExecQuery();
	  RetVal = IBInternalQuery->FieldByName("CONTACTS_3RDPARTY_KEY")->AsInteger;
	  // If you can find the member there is nothing we can do at this level.
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TContact::DeleteContactCard(Database::TDBTransaction &DBTransaction, UnicodeString Card,int inContactKey)
{
   if (!fEnabled)
	  return;
   try
   {
   		TDBContacts::DeleteContactCard(DBTransaction, Card, inContactKey);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TContact::DeleteContactCards(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "DELETE FROM CONTACTCARDS " "WHERE " "CONTACTS_KEY	= :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

bool TContact::GetContactExists(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return 0;
   bool RetVal = false;
   try
   {
	  RetVal = TDBContacts::GetContactExists(DBTransaction,inContactKey);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}


void TContact::GetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
	try
   {
	  TDBContacts::GetContactDetails(DBTransaction,inContactKey,Info);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

int TContact::GetContactIDByKey(Database::TDBTransaction &DBTransaction, int ContactKey)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "CONTACT_ID " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY AND "
		  "CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("CONTACT_ID")->AsInteger;
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

int TContact::GetContactByProx(Database::TDBTransaction &DBTransaction, UnicodeString Prox)
{
   if (!fEnabled)
	  return 0;

   // Fix for Interbase Bug where are Space is seen as an empty VarChar string.
   if (Prox.Trim() == "")
	  return 0;

   int RetVal = 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "CONTACTS_KEY " "FROM " "CONTACTS " "WHERE " "PROX_CARD = :PROX_CARD AND "
		  "CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("PROX_CARD")->AsString = Prox;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
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

int TContact::GetContactByName(Database::TDBTransaction &DBTransaction, UnicodeString Name)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "CONTACTS_KEY " "FROM " "CONTACTS " "WHERE " "UPPER(NAME) = :NAME AND "
		  "CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("NAME")->AsString = Name.UpperCase();
      IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;


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

int TContact::GetInternalKey(Database::TDBTransaction &DBTransaction, int ExternalKey)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE "
		  "CONTACTS_3RDPARTY_KEY = :CONTACTS_3RDPARTY_KEY AND CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = ExternalKey;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
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

int TContact::GetOrCreateContact(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   int RetVal = 0;
   if (!fEnabled)
	  return 0;
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  bool CreateContact = true;
	  if (inContactKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " SELECT " " CONTACTS_KEY " " FROM " "  CONTACTS " " WHERE " "  CONTACTS_KEY = :CONTACTS_KEY AND "
			 "  CONTACT_TYPE = :CONTACT_TYPE";
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		 IBInternalQuery->ExecQuery();
		 if (inContactKey == IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger)
		 {
			RetVal = inContactKey;
			CreateContact = false;
		 }
	  }

	  if (CreateContact)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTS, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 RetVal = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO CONTACTS (" "CONTACTS_KEY," "CONTACT_TYPE," "NAME," "PIN," "SITE_ID," "CARD_CREATION_DATE,"
			 "CONTACTS_3RDPARTY_KEY," "MEMBER_NUMBER, ACTIVATION_DATE,MEMBER_TYPE,MAILING_ADDRESS,PHONE,MOBILE,TITLE,DATEOFBIRTH,POINTS_RULES) " "VALUES (" ":CONTACTS_KEY," ":CONTACT_TYPE," ":NAME," ":PIN," ":SITE_ID,"
			 ":CARD_CREATION_DATE," ":CONTACTS_3RDPARTY_KEY," ":MEMBER_NUMBER, :ACTIVATION_DATE,:MEMBER_TYPE,:MAILING_ADDRESS,"
                         ":PHONE, :MOBILE, :TITLE, :DATEOFBIRTH, :POINTS_RULES);";
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = RetVal;
		 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		 IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
		 IBInternalQuery->ParamByName("PIN")->AsString = "";
		 IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
		 IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = Info.CardCreationDate;
		 IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = 0;
		 IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsInteger = 0;
         IBInternalQuery->ParamByName("ACTIVATION_DATE")->AsDateTime = Now();
         IBInternalQuery->ParamByName("MEMBER_TYPE")->AsInteger = Info.MemberType;
         IBInternalQuery->ParamByName("DATEOFBIRTH")->AsDateTime = Info.DateOfBirth;
         IBInternalQuery->ParamByName("TITLE")->AsString = Info.Title;
         IBInternalQuery->ParamByName("PHONE")->AsString = Info.Phone  ;
		 IBInternalQuery->ParamByName("MOBILE")->AsString = Info.Mobile ;
         IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = Info.MailingAddress;
         IBInternalQuery->ParamByName("POINTS_RULES")->AsInteger = Info.PointRule;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

int TContact::GetContact(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   int RetVal = 0;
   if (!fEnabled)
	  return 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  if (inContactKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " SELECT " " CONTACTS_KEY " " FROM " "  CONTACTS " " WHERE " "  CONTACTS_KEY = :CONTACTS_KEY";
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		 IBInternalQuery->ExecQuery();
		 if (inContactKey == IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger)
		 {
			RetVal = inContactKey;
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

bool TContact::DeleteContact(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return false;

   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  TMMContactInfo Info;
	  GetContactDetails(DBTransaction, inContactKey, Info);
	  Info.Name = Now().FormatString("dd-mm-yyyy hh-mm-ss") + " " + Info.Name;
	  Info.Name = Info.Name.SubString(1, 50);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "UPDATE " " CONTACTS" " SET" " NAME = :NAME," " LAST_MODIFIED = :LAST_MODIFIED,"
		  " CONTACT_TYPE = :TOCONTACT_TYPE" " WHERE" " CONTACTS_KEY = :CONTACTS_KEY AND " " CONTACT_TYPE = :FROMCONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
	  IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Now();
	  IBInternalQuery->ParamByName("FROMCONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("TOCONTACT_TYPE")->AsInteger = ContactType + 1;

	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RowsAffected <= 0)
	  {
		 RetVal = false;
	  }
	  else
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

// ---------------------------------------------------------------------------
bool TContact::UndeleteContact(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   if (!fEnabled)
	  return false;

   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  TMMContactInfo Info;
	  GetContactDetails(DBTransaction, inContactKey, Info);
	  UnicodeString NewName = Info.Name;
	  if (NewName.Pos(" ") != 0);
	  NewName = NewName.SubString(NewName.Pos(" ") + 1, NewName.Length());
	  if (NewName.Pos(" ") != 0);
	  NewName = NewName.SubString(NewName.Pos(" ") + 1, NewName.Length());

	  Info.Name = NewName.SubString(1, 50);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "UPDATE " " CONTACTS" " SET" " NAME = :NAME," " LAST_MODIFIED = :LAST_MODIFIED,"
		  " CONTACT_TYPE = :TOCONTACT_TYPE" " WHERE" " CONTACTS_KEY = :CONTACTS_KEY AND " " CONTACT_TYPE = :FROMCONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
	  IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Now();
	  IBInternalQuery->ParamByName("FROMCONTACT_TYPE")->AsInteger = ContactType + 1;
	  IBInternalQuery->ParamByName("TOCONTACT_TYPE")->AsInteger = ContactType;

	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RowsAffected <= 0)
	  {
		 RetVal = false;
	  }
	  else
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

// ---------------------------------------------------------------------------
bool TContact::SetContactType(Database::TDBTransaction &DBTransaction, int inContactKey, TContactType Type)
{
   if (!fEnabled)
	  return false;

   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "UPDATE " " CONTACTS" " SET" " LAST_MODIFIED = :LAST_MODIFIED," " CONTACT_TYPE = :CONTACT_TYPE" " WHERE"
		  " CONTACTS_KEY = :CONTACTS_KEY;";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Now();
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = Type;

	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RowsAffected <= 0)
	  {
		 RetVal = false;
	  }
	  else
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

// ---------------------------------------------------------------------------

void TContact::SetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   if (!fEnabled)
	  return;

   try
   {
	  TDBContacts::SetContactDetails(DBTransaction, inContactKey,ContactType, Info);
	  OnSaveContact.Occured(Info);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message + " Contact Name : " + Info.Name + " Contact Type : " + IntToStr(ContactType)
		 + " Contact 3rdPartyKey : " + IntToStr(Info.ExternalKey));
	  throw;
   }
}

// ---------------------------------------------------------------------------

bool TContact::GetContactBlocked(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{

   bool RetVal = false;
   try
   {
	  if (Info.ATRStr != "" && Info.ATRStr != NULL)
	  {
		 if (CardBanned(DBTransaction, Info.ContactKey, Info.ATRStr))
		 {
			RetVal = true;
			Info.AccountInfo = "Your Smart Card has been blocked";
		 }
		 else if (CardReplaced(DBTransaction, Info.ContactKey, Info.ATRStr))
		 {
			RetVal = true;
			Info.AccountInfo = "Your Smart Card has been replaced";
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

// ---------------------------------------------------------------------------
bool TContact::CardBanned(Database::TDBTransaction &DBTransaction, int ContactKey, UnicodeString Atr)
{
   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  // If there is no Restore Point Create one.
	  // Otherwise update the image.
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT " " * " " FROM " "  SMARTCARDS " " WHERE " "  ATR = :ATR AND "
		  "  CONTACTS_KEY = :CONTACTS_KEY AND " "  CARD_STATUS = :CARD_STATUS";
	  IBInternalQuery->ParamByName("ATR")->AsString = Atr;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ParamByName("CARD_STATUS")->AsInteger = SMARTCARD_CARD_STATUS_BANNED;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount != 0)
	  { // Card Banned.
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

bool TContact::CardReplaced(
  Database::TDBTransaction &tr,
  int ContactKey,
  UnicodeString Atr)
{
	try {
		TIBSQL  *qr = tr.Query(tr.AddQuery());
		UnicodeString current_atr;

		qr->SQL->Text = "select first 2 "
		                "       atr "
		                "       from smartcards "
		                "       where contacts_key = :ck "
		                "             and card_status = 1 "
		                "       order by card_creation_date desc;";
		qr->ParamByName("ck")->AsInteger = ContactKey;
		if (qr->ExecQuery(), !qr->RecordCount)
			return false;

		current_atr = qr->FieldByName("atr")->AsString;
		qr->Close();
		return Atr.Length() != current_atr.Length() || Atr != current_atr;
	} catch (Exception &e) {
   	TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, e.Message);
		throw;
	}
}

void TContact::GetContactList(Database::TDBTransaction &DBTransaction, TStringList *List, UnicodeString Format, UnicodeString OrderBy)
{
   if (!fEnabled)
	  return;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  " SELECT NAME,MEMBER_NUMBER,CONTACTS_KEY,PHONE" " FROM " "  CONTACTS" " WHERE" " CONTACT_TYPE = :CONTACT_TYPE"
		  " ORDER BY " + OrderBy;
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;

	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount && List != NULL)
	  {
		 for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		 {
			UnicodeString FormatedString = Format;
			FormatedString = StringReplace(FormatedString, "%Name", IBInternalQuery->FieldByName("NAME")->AsString,
			   TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			// Pad the member number out to 4 digits.
			if (StrToIntDef(IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString, -1) != -1)
			{
			   FormatedString = StringReplace(FormatedString, "%Number",
				  FormatFloat("0000", StrToIntDef(IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString, -1)),
				  TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			}
			else
			{
			   FormatedString = StringReplace(FormatedString, "%Number", IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString,
				  TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			}
			FormatedString = StringReplace(FormatedString, "%Phone", IBInternalQuery->FieldByName("PHONE")->AsString,
			   TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			int Index = List->Add(FormatedString);
			// Little bit of pointer abuse but we just want to store the int somewhere.
			List->Objects[Index] = (TObject*)IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		 }
	  }
	  List->CaseSensitive = false;
	  List->Sort();

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

int TContact::RecordCount(Database::TDBTransaction &DBTransaction)
{
   if (!fEnabled)
	  return 0;

   int RetVal = 0;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT COUNT(CONTACTS_KEY)" " FROM " "  CONTACTS" " WHERE" " CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ExecQuery();
	  RetVal = IBInternalQuery->FieldByName("COUNT")->AsInteger;

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);

   }
   return RetVal;
}

int TContact::GetNextContactID(Database::TDBTransaction &DBTransaction)
{
   bool NotFound = true;
   int UserID = 0;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "Select CONTACT_ID from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE and CONTACT_ID = :CONTACT_ID";
	  for (int i = 1; NotFound; i++)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
		 IBInternalQuery->ParamByName("CONTACT_ID")->AsInteger = i;
		 IBInternalQuery->ExecQuery();
		 if (IBInternalQuery->RecordCount == 0)
		 {
			NotFound = false;
			UserID = i;
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return UserID;

}

void TContact::GetTabs(Database::TDBTransaction &DBTransaction, TStringList * List)
{
   if (!fEnabled)
	  return;

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "TAB.TAB_KEY, " "CONTACTS.NAME NAME " "FROM "
		  "TAB INNER JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = TAB.CONTACTS_KEY " "WHERE " "CONTACTS.CONTACT_TYPE = :CONTACT_TYPE AND "
		  "TAB.TAB_TYPE = :TAB_TYPE " "ORDER BY " "CONTACTS.NAME";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = ContactType;
	  IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = ContactTabType;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount && List != NULL)
	  {
		 for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		 {
			int Index = List->Add(IBInternalQuery->FieldByName("NAME")->AsString);
			// Little bit of pointer abuse but we just want to store the int somewhere.
			List->Objects[Index] = (TObject*)IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		 }
	  }
	  List->CaseSensitive = false;
	  List->Sort();

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}
//------------------------------------------------------------------------

void TContact::GetContactNameList(Database::TDBTransaction &DBTransaction, std::vector<UnicodeString> &vectorMembers, int inContactType)
{



   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  " SELECT NAME" " FROM " "  CONTACTS "
		  " WHERE CONTACT_TYPE = :CONTACT_TYPE "
		  " ORDER BY " " NAME ";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = inContactType;
	  IBInternalQuery->ExecQuery();
		 for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		 {

                	vectorMembers.push_back(IBInternalQuery->FieldByName("NAME")->AsString);


                 /*
			UnicodeString FormatedString = Format;
			FormatedString = StringReplace(FormatedString, "%Name", IBInternalQuery->FieldByName("NAME")->AsString,
			   TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			// Pad the member number out to 4 digits.
			if (StrToIntDef(IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString, -1) != -1)
			{
			   FormatedString = StringReplace(FormatedString, "%Number",
				  FormatFloat("0000", StrToIntDef(IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString, -1)),
				  TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			}
			else
			{
			   FormatedString = StringReplace(FormatedString, "%Number", IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString,
				  TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			}
			FormatedString = StringReplace(FormatedString, "%Phone", IBInternalQuery->FieldByName("PHONE")->AsString,
			   TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
			int Index = List->Add(FormatedString);
			// Little bit of pointer abuse but we just want to store the int somewhere.
			List->Objects[Index] = (TObject*)IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
                        */

		 }


   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }



}

//-----------------------------------------------------------------------

int TContact::GetContactKeyByName(Database::TDBTransaction &DBTransaction, UnicodeString Name)
{
   int RetVal;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " SELECT " " CONTACTS_KEY " " FROM " "  CONTACTS " " WHERE " "  NAME = :NAME";
		 IBInternalQuery->ParamByName("NAME")->AsString = Name;
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

