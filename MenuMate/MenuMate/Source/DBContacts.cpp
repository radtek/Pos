//---------------------------------------------------------------------------


#pragma hdrstop
#include "DBContacts.h"
#include "MMLogging.h"
#include "PointsRulesSetUtils.h"
#include "enumPoints.h"
#include "DBGroups.h"
#include "GlobalSettings.h"
#include "PaySubsUtility.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


int TDBContacts::GetContactByMemberNumberSiteID(Database::TDBTransaction &DBTransaction,UnicodeString MemberNumber, int SiteID)
{
   int RetVal = 0;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE MEMBER_NUMBER = :MEMBER_NUMBER AND SITE_ID = :SITE_ID";
      IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = MemberNumber;
      IBInternalQuery->ParamByName("SITE_ID")->AsInteger = SiteID;
      IBInternalQuery->ExecQuery();

      if(IBInternalQuery->RecordCount)
      {
         RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      }
   }
	catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      throw;
   }

	return RetVal;
}

bool TDBContacts::GetContactDetailsByCode(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info, AnsiString cardCode, MemberMode &inMemberMode)
{
    bool RetVal = false;
    inMemberMode = eInvalidMode;
    if(GetContactDetailsByMemberCode(DBTransaction,cardCode,Info))
    {
       RetVal = true;
       inMemberMode = eMemberCodeMode;
    }

    if(!RetVal && GetContactDetailsBySwipeCard(DBTransaction,cardCode,Info))
    {
       RetVal = true;
       inMemberMode = eSwipeCardMode;
    }

    if(!RetVal && GetContactDetailsByProxCard(DBTransaction,cardCode,Info))
    {
       RetVal = true;
       inMemberMode = eProxCardMode;
    }
    return RetVal;
}

bool TDBContacts::GetContactDetailsByMemberCode(Database::TDBTransaction &DBTransaction,AnsiString cardCode,TMMContactInfo &Info)
{
    bool RetVal = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE MEMBER_CARD_CODE = :MEMBER_CARD_CODE";
    IBInternalQuery->ParamByName("MEMBER_CARD_CODE")->AsString = cardCode;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
      int contactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      GetContactDetails(DBTransaction,contactKey,Info);
      RetVal = true;
    }
    return RetVal;
}

bool TDBContacts::GetContactDetailsBySwipeCard(Database::TDBTransaction &DBTransaction,AnsiString cardCode,TMMContactInfo &Info)
{
    bool RetVal = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTCARDS WHERE SWIPE_CARD = :SWIPE_CARD AND IS_ACTIVE = 'T'";
    IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = cardCode;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
      int contactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      GetContactDetails(DBTransaction,contactKey,Info);
      RetVal = true;
    }
    return RetVal;
}

bool TDBContacts::GetContactDetailsByProxCard(Database::TDBTransaction &DBTransaction,AnsiString cardCode,TMMContactInfo &Info)
{
    bool RetVal = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE PROX_CARD = :PROX_CARD";
    IBInternalQuery->ParamByName("PROX_CARD")->AsString = cardCode;
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount)
    {
      int contactKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      GetContactDetails(DBTransaction,contactKey,Info);
      RetVal = true;
    }
    return RetVal;
}

int TDBContacts::GetContactByEmail(Database::TDBTransaction &DBTransaction,AnsiString Email)
{
   int RetVal = 0;
   if(Email == "" || Email == NULL)
     return 0;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE UPPER(EMAIL) = :EMAIL";
      IBInternalQuery->ParamByName("EMAIL")->AsString = Email.UpperCase();
      IBInternalQuery->ExecQuery();

	  if(IBInternalQuery->RecordCount)
      {
         RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      }
   }
	catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      throw;
   }

	return RetVal;
}

int TDBContacts::GetContactByEmail(Database::TDBTransaction &DBTransaction,AnsiString Email,int contactKey)
{
   int RetVal = 0;
   if(Email == "" || Email == NULL)
     return 0;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS WHERE UPPER(EMAIL) = :EMAIL and CONTACTS_KEY <> :RESTRICTED_CONTACTS_KEY";
      IBInternalQuery->ParamByName("EMAIL")->AsString = Email.UpperCase();
      IBInternalQuery->ParamByName("RESTRICTED_CONTACTS_KEY")->AsInteger = contactKey;
      IBInternalQuery->ExecQuery();

	  if(IBInternalQuery->RecordCount)
      {
         RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      }
   }
	catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      throw;
   }

	return RetVal;
}

AnsiString TDBContacts::GetContactProxCard(Database::TDBTransaction &DBTransaction,int ContactKey)
{
   AnsiString RetVal = 0;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT PROX_CARD FROM CONTACTS WHERE CONTACTS_KEY = :CONTACTS_KEY";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
      IBInternalQuery->ExecQuery();

	  if(IBInternalQuery->RecordCount)
      {
         RetVal = IBInternalQuery->FieldByName("PROX_CARD")->AsString;
      }
   }
   catch(Exception &E)
   {
	  TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      throw;
   }

	return RetVal;
}

int TDBContacts::GetContactByNamePhone(Database::TDBTransaction &DBTransaction,UnicodeString Name, UnicodeString Phone)
{
   int RetVal = 0;
   try
   {
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
      "SELECT "
         "CONTACTS_KEY "
      "FROM "
         "CONTACTS "
      "WHERE "
			"NAME = :NAME AND PHONE = :PHONE";
      IBInternalQuery->ParamByName("NAME")->AsString = Name;
      IBInternalQuery->ParamByName("PHONE")->AsString = Phone;
      IBInternalQuery->ExecQuery();

	  if(IBInternalQuery->RecordCount)
      {
         RetVal = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
      }
   }
	catch(Exception &E)
   {
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,E.Message);
      throw;
   }

	return RetVal;
}

void TDBContacts::GetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  " SELECT * FROM CONTACTS c LEFT JOIN MEMBERSHIP_SUBS_DETAILS m ON c.CONTACTS_KEY = m.CONTACTS_KEY "
          " WHERE c.CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();
	  if (IBInternalQuery->RecordCount)
	  {
		 Info.Name = IBInternalQuery->FieldByName("NAME")->AsString;
         Info.Surname = IBInternalQuery->FieldByName("LAST_NAME")->AsString;
		 Info.Title = IBInternalQuery->FieldByName("TITLE")->AsString;
		 Info.Sex = IBInternalQuery->FieldByName("SEX")->AsString;
		 Info.Initials = IBInternalQuery->FieldByName("INITIALS")->AsString;
		 Info.Phone = IBInternalQuery->FieldByName("PHONE")->AsString;
		 Info.Mobile = IBInternalQuery->FieldByName("MOBILE")->AsString;
		 Info.EMail = IBInternalQuery->FieldByName("EMAIL")->AsString;
		 Info.TabEnabled = IBInternalQuery->FieldByName("TAB_ENALBED")->AsString.UpperCase() == "F" ? false : true;
		 Info.PIN = IBInternalQuery->FieldByName("PIN")->AsString;
		 Info.AccessLevel = IBInternalQuery->FieldByName("ACCESS_LEVEL")->AsInt64;
		 Info.LocationAddress = IBInternalQuery->FieldByName("LOCATION_ADDRESS")->AsString;
		 Info.MailingAddress = IBInternalQuery->FieldByName("MAILING_ADDRESS")->AsString;
		 Info.Note = IBInternalQuery->FieldByName("NOTE")->AsString;
         Info.LastBirthdayProcessed =IBInternalQuery->FieldByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime;
         Info.ActivationDate =IBInternalQuery->FieldByName("ACTIVATION_DATE")->AsDateTime;
		 Info.PalmUserSettings.PriceAdjust = Info.AccessLevel.PriceAj; // AccessCheck(CheckPriceAj);
		 Info.PalmUserSettings.AdvanceUser = IBInternalQuery->FieldByName("Palm_Advanced_User")->AsString.UpperCase() == "F" ? false : true;
		 Info.PalmUserSettings.AutoDetails = IBInternalQuery->FieldByName("Palm_Auto_Options")->AsString.UpperCase() == "F" ? false : true;
		 Info.PalmUserSettings.LargeFont = IBInternalQuery->FieldByName("Palm_Large_Font")->AsString.UpperCase() == "F" ? false : true;
		 Info.PalmUserSettings.ScreenLock = IBInternalQuery->FieldByName("Palm_Screen_Lock")->AsString.UpperCase() == "F" ? false : true;
		 Info.PalmUserSettings.ClassicMode = IBInternalQuery->FieldByName("Palm_Classic_Mode")->AsString.UpperCase() == "F" ? false : true;
		 Info.PalmUserSettings.SoundLevel = static_cast <TPalmUserSettings::TPalmSoundLevel>
			 (IBInternalQuery->FieldByName("Palm_Sound_Level")->AsInteger);
		 Info.PalmUserSettings.GetAllTables = IBInternalQuery->FieldByName("Palm_Get_All_Tables")->AsString.UpperCase()
			 == "F" ? false : true;

		 Info.PalmTimeOut = IBInternalQuery->FieldByName("PALM_TIMEOUT")->AsInteger;
		 Info.PayRollID = IBInternalQuery->FieldByName("PAYROLL_ID")->AsString;
		 Info.StartTime1 = IBInternalQuery->FieldByName("START_TIME1")->AsDateTime;
		 Info.StartTime2 = IBInternalQuery->FieldByName("START_TIME2")->AsDateTime;
		 Info.StartTime3 = IBInternalQuery->FieldByName("START_TIME3")->AsDateTime;
		 Info.StartTime4 = IBInternalQuery->FieldByName("START_TIME4")->AsDateTime;
		 Info.StartTime5 = IBInternalQuery->FieldByName("START_TIME5")->AsDateTime;
		 Info.StartTime6 = IBInternalQuery->FieldByName("START_TIME6")->AsDateTime;
		 Info.DateOfBirth = IBInternalQuery->FieldByName("DATEOFBIRTH")->AsDateTime;
		 Info.CardCreationDate = IBInternalQuery->FieldByName("CARD_CREATION_DATE")->AsDateTime;
		 Info.LastModified = IBInternalQuery->FieldByName("LAST_MODIFIED")->AsDateTime;
		 Info.ProxStr = IBInternalQuery->FieldByName("PROX_CARD")->AsString;
		 Info.ContactID = IBInternalQuery->FieldByName("CONTACT_ID")->AsInteger;
		 Info.ExternalKey = IBInternalQuery->FieldByName("CONTACTS_3RDPARTY_KEY")->AsInteger;
		 Info.MembershipNumber = IBInternalQuery->FieldByName("MEMBER_NUMBER")->AsString;
		 Info.SiteID = IBInternalQuery->FieldByName("SITE_ID")->AsInteger;
		 Info.ContactKey = inContactKey;
		 Info.Alias = IBInternalQuery->FieldByName("KNOWN_AS")->AsString;
		 Info.Charges = IBInternalQuery->FieldByName("ACCOUNT_PROFILE")->AsInteger;
		 Info.LastVisit = IBInternalQuery->FieldByName("LAST_VISIT")->AsDateTime;
		 Info.HourlyRate = IBInternalQuery->FieldByName("HOURLY_RATE")->AsCurrency;
         Info.MemberCode = IBInternalQuery->FieldByName("MEMBER_CARD_CODE")->AsString;
         Info.MemberType = IBInternalQuery->FieldByName("MEMBER_TYPE")->AsInteger;
         Info.TierLevel = IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
         Info.IsFirstVisitRewarded = IBInternalQuery->FieldByName("IS_FIRSTVISIT_REWARDED")->AsString == 'T' ? true : false;
		 int PointsRules = IBInternalQuery->FieldByName("POINTS_RULES")->AsInteger;
         int PointsRulesSubs = IBInternalQuery->FieldByName("POINTS_RULES_SUBS")->AsInteger;
         TPointsRulesSetUtils().Expand(PointsRules, Info.Points.PointsRules);
         TPointsRulesSetUtils().ExpandSubs(PointsRulesSubs, Info.Points.PointsRulesSubs);
         Info.Points.ClearPoints();
         GetPointsBalances(DBTransaction,inContactKey,Info.Points);
         GetContactCards(DBTransaction,inContactKey,Info.Cards);
		 GetDiscountDetails(DBTransaction, Info.ContactKey, Info);
		 GetCurrentGroups(DBTransaction, inContactKey, Info);
         // loads the cloud uuid if already registered on the cloud
         Info.CloudUUID = TDBContacts::GetMemberCloudIdIfRegistered(DBTransaction, Info.ContactKey, Info.SiteID);

	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TDBContacts::GetPointsBalances(Database::TDBTransaction &DBTransaction,int ContactKey, TContactPoints & Points)
{
   try
   {
         TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        // Earned Held Points.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "SELECT distinct ADJUSTMENT_TYPE,ADJUSTMENT_SUBTYPE,EXPORTED_STATUS, SUM(ADJUSTMENT) TOTAL "
            "FROM POINTSTRANSACTIONS a "
            "WHERE CONTACTS_KEY = :CONTACTS_KEY "
            "group by ADJUSTMENT_TYPE,ADJUSTMENT_SUBTYPE,EXPORTED_STATUS";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
        IBInternalQuery->ExecQuery();

        // we are loading the points information from the database. At this point I (Nadun) have called the ClearBySource method on existing
        // points structure so there will be no duplicate point information will be left once the function is executed. Otherwise members points
        // were added again which causes points to double up
        if(!IBInternalQuery->Eof)
            Points.ClearBySource(pasDatabase);

        for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
        {

              TPointsTypePair Pair = TContactPoints::GetPointsType(IBInternalQuery->FieldByName("ADJUSTMENT_TYPE")->AsInteger,
                                                                          IBInternalQuery->FieldByName("ADJUSTMENT_SUBTYPE")->AsInteger);
              TPointsExportStatus Status = TContactPoints::getPointsStatus(IBInternalQuery->FieldByName("EXPORTED_STATUS")->AsInteger);
              TPointsType Type(pasDatabase,Pair,Status);
              Points.Load(Type, IBInternalQuery->FieldByName("TOTAL")->AsCurrency);
        }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

void TDBContacts::GetDiscountDetails(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT DISCOUNT_KEY" " FROM " "  CONTACTDISCOUNTS" " WHERE"
		  " CONTACTS_KEY = :CONTACTS_KEY ORDER BY CONTACTDISCOUNTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
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

bool TDBContacts::GetContactExists(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   bool RetVal = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT CONTACTS_KEY" " FROM " "  CONTACTS" " WHERE" " CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
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

int TDBContacts::GetOrCreateContact(Database::TDBTransaction &DBTransaction, int inContactKey,TContactType inContactType,TMMContactInfo &Info)
{
   int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction, eSystemProfiles, "Globals");
   TManagerVariable::Instance().GetProfileBool(DBTransaction,  GlobalProfileKey, vmUseMemberSubs, TGlobalSettings::Instance().UseMemberSubs);
   int RetVal = 0;
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  bool CreateContact = true;
	  if (inContactKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = " SELECT c.CONTACTS_KEY,c.POINTS_RULES,m.POINTS_RULES_SUBS FROM CONTACTS c LEFT JOIN MEMBERSHIP_SUBS_DETAILS m ON c.CONTACTS_KEY = m.CONTACTS_KEY"
         " WHERE c.CONTACTS_KEY = :CONTACTS_KEY AND c.CONTACT_TYPE = :CONTACT_TYPE ";
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = inContactType;
		 IBInternalQuery->ExecQuery();
		 if (inContactKey == IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger)
		 {
            if(TPaySubsUtility::IsLocalLoyalty())
            {
                int newRulesSubs = TPointsRulesSetUtils().CompressSubs(Info.Points.PointsRulesSubs);
                if(newRulesSubs == 0)
                {
                    int rulesSubs = IBInternalQuery->FieldByName("POINTS_RULES_SUBS")->AsInteger;
                    TPointsRulesSetUtils().ExpandSubs(rulesSubs,Info.Points.PointsRulesSubs);
                }
            }
			RetVal = inContactKey;
			CreateContact = false;
		 }
	  }
      if(inContactType == eThorMember && Info.ExternalKey == 0)
      {
         CreateContact = false;
      }

	  if (CreateContact)
	  {
         Info.ActivationDate = Now();
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTS, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 RetVal = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text =
			 "INSERT INTO CONTACTS (" "CONTACTS_KEY," "CONTACT_TYPE," "NAME, LAST_NAME," "PIN," "SITE_ID," "CARD_CREATION_DATE,"
			 "CONTACTS_3RDPARTY_KEY," "MEMBER_NUMBER, ACCOUNT_PROFILE,LAST_VISIT, LAST_BIRTHDAY_PROCESSED ,TIER_LEVEL, "
                         " ACTIVATION_DATE,MEMBER_TYPE,MAILING_ADDRESS,PHONE,MOBILE,TITLE,DATEOFBIRTH,MEMBER_CARD_CODE,IS_FIRSTVISIT_REWARDED,POINTS_RULES) " "VALUES (" ":CONTACTS_KEY, :CONTACT_TYPE, :NAME, :LAST_NAME, :PIN, :SITE_ID,"
			 ":CARD_CREATION_DATE, :CONTACTS_3RDPARTY_KEY, :MEMBER_NUMBER, :ACCOUNT_PROFILE, :LAST_VISIT, "
                         ":LAST_BIRTHDAY_PROCESSED ,:TIER_LEVEL ,:ACTIVATION_DATE,:MEMBER_TYPE,:MAILING_ADDRESS,:PHONE,:MOBILE,:TITLE,"
                         ":DATEOFBIRTH,:MEMBER_CARD_CODE,:IS_FIRSTVISIT_REWARDED,:POINTS_RULES);";
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = RetVal;
		 IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = inContactType;
		 IBInternalQuery->ParamByName("NAME")->AsString = Info.Name;
         IBInternalQuery->ParamByName("LAST_NAME")->AsString = Info.Surname;
		 IBInternalQuery->ParamByName("PIN")->AsString = "";
		 IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
		 IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = Info.CardCreationDate;
		 IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = 0;
		 IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsInteger = 0;
		 IBInternalQuery->ParamByName("ACCOUNT_PROFILE")->AsInteger = 0;
         IBInternalQuery->ParamByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime = Info.LastBirthdayProcessed;
         IBInternalQuery->ParamByName("LAST_VISIT")->AsDateTime = Info.LastVisit;
         IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = Info.TierLevel;
         IBInternalQuery->ParamByName("ACTIVATION_DATE")->AsDateTime = Info.ActivationDate;
         IBInternalQuery->ParamByName("MEMBER_TYPE")->AsInteger = Info.MemberType;
         IBInternalQuery->ParamByName("DATEOFBIRTH")->AsDateTime = Info.DateOfBirth;
         IBInternalQuery->ParamByName("TITLE")->AsString = Info.Title;
         IBInternalQuery->ParamByName("PHONE")->AsString = Info.Phone  ;
		 IBInternalQuery->ParamByName("MOBILE")->AsString = Info.Mobile ;
         IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = Info.MailingAddress;
         IBInternalQuery->ParamByName("MEMBER_CARD_CODE")->AsString = Info.MemberCode;
         IBInternalQuery->ParamByName("IS_FIRSTVISIT_REWARDED")->AsString = Info.IsFirstVisitRewarded ? 'T' : 'F';
         IBInternalQuery->ParamByName("POINTS_RULES")->AsInteger = Info.PointRule;
		 IBInternalQuery->ExecQuery();
         TDBContacts::InsertDetailstoMemberSubs(DBTransaction, RetVal,inContactType,Info);
         if(TPaySubsUtility::IsLocalLoyalty() && !TGlobalSettings::Instance().UseMemberSubs
            && TPointsRulesSetUtils().CompressSubs(Info.Points.PointsRulesSubs) == 0)
         {
            int pointRules = 0;
            pointRules |= eprAllowDiscounts;
            pointRules |= eprFinancial;
            TPointsRulesSetUtils().ExpandSubs(pointRules,Info.Points.PointsRulesSubs);
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
//-----------------------------------------------------------------------------
void TDBContacts::UpdateDetailstoMemberSubs(Database::TDBTransaction &DBTransaction, int inContactKey,TContactType inContactType,TMMContactInfo &Info)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
        "UPDATE MEMBERSHIP_SUBS_DETAILS SET"
        " POINTS_RULES_SUBS = :POINTS_RULES_SUBS, SUBS_PAID = :SUBS_PAID"
        " WHERE CONTACTS_KEY = :CONTACTS_KEY";
      if(!TPaySubsUtility::IsLocalLoyalty())
      {
          int pointRules = 0;
          if(!Info.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
            pointRules |= eprAllowDiscounts;
          if(!Info.Points.PointsRulesSubs.Contains(eprFinancial))
            pointRules |= eprFinancial;
          TPointsRulesSetUtils().ExpandSubs(pointRules,Info.Points.PointsRulesSubs);
      }
      IBInternalQuery->ParamByName("POINTS_RULES_SUBS" )->AsInteger = TPointsRulesSetUtils().CompressSubs(Info.Points.PointsRulesSubs);
      IBInternalQuery->ParamByName("CONTACTS_KEY" )->AsInteger = inContactKey;
      if(Info.Points.PointsRulesSubs.Contains(eprFinancial))
        IBInternalQuery->ParamByName("SUBS_PAID" )->AsString = "T";
      else
        IBInternalQuery->ParamByName("SUBS_PAID" )->AsString = "F";
      IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }

}
//-----------------------------------------------------------------------------
void TDBContacts::InsertDetailstoMemberSubs(Database::TDBTransaction &DBTransaction, int inContactKey,TContactType inContactType,TMMContactInfo &Info)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MEMBERSHIP_SUBS_KEY, 1) FROM RDB$DATABASE";
      IBInternalQuery->ExecQuery();
      int retValue = IBInternalQuery->Fields[0]->AsInteger;
      IBInternalQuery->Close();
      IBInternalQuery->SQL->Text =
        "INSERT INTO MEMBERSHIP_SUBS_DETAILS ( MEMBERSHIP_SUBS_KEY, CONTACTS_KEY,POINTS_RULES_SUBS,SUBS_PAID,SUBS_TYPE,ISLOCAL_MEMBER )"
        " VALUES (:MEMBERSHIP_SUBS_KEY, :CONTACTS_KEY,:POINTS_RULES_SUBS,:SUBS_PAID,:SUBS_TYPE,:ISLOCAL_MEMBER)";
      IBInternalQuery->ParamByName("MEMBERSHIP_SUBS_KEY")->AsInteger = retValue;
      IBInternalQuery->ParamByName("CONTACTS_KEY" )->AsInteger  = inContactKey;
      IBInternalQuery->ParamByName("POINTS_RULES_SUBS" )->AsInteger = TPointsRulesSetUtils().CompressSubs(Info.Points.PointsRulesSubs);
      if(TGlobalSettings::Instance().UseMemberSubs)
      {
          IBInternalQuery->ParamByName("SUBS_TYPE" )->AsString  = "";
          IBInternalQuery->ParamByName("SUBS_PAID" )->AsString  = "F";
              if(Info.Points.PointsRulesSubs.Contains(eprFinancial))
                Info.Points.PointsRulesSubs >> eprFinancial;
              if(!Info.Points.PointsRules.Contains(eprNoPointsPurchases))
                 Info.Points.PointsRules << eprNoPointsPurchases;
              if(!Info.Points.PointsRules.Contains(eprNoPointsRedemption))
                 Info.Points.PointsRules << eprNoPointsRedemption;
              if(!Info.Points.PointsRules.Contains(eprNeverEarnsPoints))
                 Info.Points.PointsRules << eprNeverEarnsPoints;
              if(Info.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
                 Info.Points.PointsRulesSubs >> eprAllowDiscounts;
      }
      else
      {
          IBInternalQuery->ParamByName("SUBS_TYPE" )->AsString  = "AUTO";
          IBInternalQuery->ParamByName("SUBS_PAID" )->AsString  = "T";
      }
      if(!TPaySubsUtility::IsLocalLoyalty())
      {
         IBInternalQuery->ParamByName("ISLOCAL_MEMBER" )->AsString  = "F";
      }
      else
      {
         IBInternalQuery->ParamByName("ISLOCAL_MEMBER" )->AsString  = "T";
      }

      IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }

}
//-----------------------------------------------------------------------------
void TDBContacts::SetContactDetails(Database::TDBTransaction &DBTransaction, int inContactKey,TContactType inContactType, TMMContactInfo &Info)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  inContactKey = GetOrCreateContact(DBTransaction, inContactKey,inContactType, Info);
	  Info.ContactKey = inContactKey;

	  if (Info.ProxStr != "")
	  {
      	 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT CONTACTS_KEY FROM CONTACTS  WHERE PROX_CARD = :PROX_CARD AND CONTACTS_KEY != :CONTACTS_KEY";
		 IBInternalQuery->ParamByName("PROX_CARD")->AsString = Info.ProxStr;
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsString = inContactKey;
		 IBInternalQuery->ExecQuery();
         if(IBInternalQuery->RecordCount > 0)
         {
             IBInternalQuery->Close();
             IBInternalQuery->SQL->Text = "UPDATE CONTACTS SET PROX_CARD = '' WHERE PROX_CARD = :PROX_CARD";
             IBInternalQuery->ParamByName("PROX_CARD")->AsString = Info.ProxStr;
             IBInternalQuery->ExecQuery();
         }
	  }
      UnicodeString surName = "";
      if((TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate) &&
         !TGlobalSettings::Instance().LoyaltyMateEnabled && Info.Surname == "")
          surName = GetLastNameForLocalCard(DBTransaction,Info.ContactKey);
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		"UPDATE "
			"CONTACTS "
		"SET "
			"NAME = :NAME, "
			"LAST_NAME = :LAST_NAME, "
			"TITLE = :TITLE, "
			"SEX = :SEX, "
			"INITIALS = :INITIALS, "
			"PHONE = :PHONE, "
			"MOBILE = :MOBILE, "
			"LOCATION_ADDRESS = :LOCATION_ADDRESS, "
			"MAILING_ADDRESS = :MAILING_ADDRESS, "
			"EMAIL = :EMAIL, "
			"PIN = :PIN, "
			"ACCESS_LEVEL = :ACCESS_LEVEL, "
			"TAB_ENALBED = :TAB_ENALBED, "
			"PALM_AUTO_OPTIONS = :PALM_AUTO_OPTIONS, "
			"PALM_LARGE_FONT = :PALM_LARGE_FONT, "
			"PALM_ADVANCED_USER = :PALM_ADVANCED_USER, "
			"Palm_Screen_Lock = :Palm_Screen_Lock, "
			"Palm_Classic_Mode = :Palm_Classic_Mode, "
			"Palm_Sound_Level = :Palm_Sound_Level,"
			"Palm_Get_All_Tables = :Palm_Get_All_Tables, "
			"PALM_TIMEOUT = :PALM_TIMEOUT, "
			"PAYROLL_ID = :PAYROLL_ID, "
			"DATEOFBIRTH = :DATEOFBIRTH, "
			"START_TIME1 = :START_TIME1, "
			"START_TIME2 = :START_TIME2, "
			"START_TIME3 = :START_TIME3, "
			"START_TIME4 = :START_TIME4, "
			"START_TIME5 = :START_TIME5, "
			"START_TIME6 = :START_TIME6, "
			"PROX_CARD = :PROX_CARD, "
			"CONTACT_ID = :CONTACT_ID, "
			"CONTACTS_3RDPARTY_KEY = :CONTACTS_3RDPARTY_KEY, "
			"MEMBER_NUMBER = :MEMBER_NUMBER, "
			"KNOWN_AS = :KNOWN_AS, "
			"NOTE = :NOTE, "
			"SITE_ID = :SITE_ID, "
			"ACCOUNT_PROFILE = :ACCOUNT_PROFILE, "
			"POINTS_RULES = :POINTS_RULES, "
			"LAST_VISIT = :LAST_VISIT, "
            "LAST_BIRTHDAY_PROCESSED = :LAST_BIRTHDAY_PROCESSED, "
            "TIER_LEVEL = :TIER_LEVEL,  "
			"HOURLY_RATE = :HOURLY_RATE, "
			"TOTAL_SPENT = :TOTAL_SPENT, "
			"EARNT_POINTS = :EARNT_POINTS, "
			"LOADED_POINTS = :LOADED_POINTS, "
			"CARD_CREATION_DATE = :CARD_CREATION_DATE, "
            "MEMBER_TYPE = :MEMBER_TYPE, "
            "MEMBER_CARD_CODE = :MEMBER_CARD_CODE, "
            "IS_FIRSTVISIT_REWARDED = :IS_FIRSTVISIT_REWARDED "
		"WHERE "
			"CONTACTS_KEY = :CONTACTS_KEY";

	  IBInternalQuery->ParamByName("NAME")->AsString = Info.Name.SubString(1, 50);
      if(surName != "")
        IBInternalQuery->ParamByName("LAST_NAME")->AsString = surName;
      else
        IBInternalQuery->ParamByName("LAST_NAME")->AsString = Info.Surname.SubString(1, 20);
	  IBInternalQuery->ParamByName("KNOWN_AS")->AsString = Info.Alias.SubString(1, 50);
	  IBInternalQuery->ParamByName("TITLE")->AsString = Info.Title.SubString(1, 10);
	  IBInternalQuery->ParamByName("SEX")->AsString = Info.Sex.SubString(1, 10);
	  IBInternalQuery->ParamByName("INITIALS")->AsString = Info.Initials.SubString(1, 5);
	  IBInternalQuery->ParamByName("PHONE")->AsString = Info.Phone.SubString(1, 25);
	  IBInternalQuery->ParamByName("MOBILE")->AsString = Info.Mobile.SubString(1, 25);
	  IBInternalQuery->ParamByName("LOCATION_ADDRESS")->AsString = Info.LocationAddress.SubString(1, 250);
	  IBInternalQuery->ParamByName("MAILING_ADDRESS")->AsString = Info.MailingAddress.SubString(1, 250);
	  IBInternalQuery->ParamByName("EMAIL")->AsString = Info.EMail.SubString(1, 256);
	  IBInternalQuery->ParamByName("PIN")->AsString = Info.PIN.SubString(1, PIN_Length);
	  IBInternalQuery->ParamByName("ACCESS_LEVEL")->AsInt64 = Info.AccessLevel;
	  IBInternalQuery->ParamByName("TAB_ENALBED")->AsString = Info.TabEnabled ? "T" : "F";

	  IBInternalQuery->ParamByName("Palm_Advanced_User")->AsString = Info.PalmUserSettings.AdvanceUser ? "T" : "F";
	  IBInternalQuery->ParamByName("Palm_Auto_Options")->AsString = Info.PalmUserSettings.AutoDetails ? "T" : "F";
	  IBInternalQuery->ParamByName("Palm_Large_Font")->AsString = Info.PalmUserSettings.LargeFont ? "T" : "F";
	  IBInternalQuery->ParamByName("Palm_Screen_Lock")->AsString = Info.PalmUserSettings.ScreenLock ? "T" : "F";
	  IBInternalQuery->ParamByName("Palm_Classic_Mode")->AsString = Info.PalmUserSettings.ClassicMode ? "T" : "F";
	  IBInternalQuery->ParamByName("Palm_Sound_Level")->AsInteger = static_cast <int> (Info.PalmUserSettings.SoundLevel);
	  IBInternalQuery->ParamByName("Palm_Get_All_Tables")->AsString = Info.PalmUserSettings.GetAllTables ? "T" : "F";

	  IBInternalQuery->ParamByName("PALM_TIMEOUT")->AsInteger = Info.PalmTimeOut;
	  IBInternalQuery->ParamByName("PAYROLL_ID")->AsString = Info.PayRollID.SubString(1, 15);
	  IBInternalQuery->ParamByName("DATEOFBIRTH")->AsDateTime = Info.DateOfBirth;
	  IBInternalQuery->ParamByName("START_TIME1")->AsDateTime = Info.StartTime1;
	  IBInternalQuery->ParamByName("START_TIME2")->AsDateTime = Info.StartTime2;
	  IBInternalQuery->ParamByName("START_TIME3")->AsDateTime = Info.StartTime3;
	  IBInternalQuery->ParamByName("START_TIME4")->AsDateTime = Info.StartTime4;
	  IBInternalQuery->ParamByName("START_TIME5")->AsDateTime = Info.StartTime5;
	  IBInternalQuery->ParamByName("START_TIME6")->AsDateTime = Info.StartTime6;
	  IBInternalQuery->ParamByName("PROX_CARD")->AsString = Info.ProxStr.SubString(1, 128);
	  IBInternalQuery->ParamByName("CONTACT_ID")->AsInteger = Info.ContactID;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("CONTACTS_3RDPARTY_KEY")->AsInteger = Info.ExternalKey;
	  IBInternalQuery->ParamByName("MEMBER_NUMBER")->AsString = Info.MembershipNumber.SubString(1, 10);
	  IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Info.SiteID;
	  IBInternalQuery->ParamByName("NOTE")->AsString = Info.Note.SubString(1, 50);
	  IBInternalQuery->ParamByName("POINTS_RULES")->AsInteger = TPointsRulesSetUtils().Compress(Info.Points.PointsRules);
      IBInternalQuery->ParamByName("LAST_BIRTHDAY_PROCESSED")->AsDateTime = Info.LastBirthdayProcessed;
      IBInternalQuery->ParamByName("LAST_VISIT")->AsDateTime = Info.LastVisit;
      IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = Info.TierLevel;
	  IBInternalQuery->ParamByName("ACCOUNT_PROFILE")->AsInteger = Info.Charges;
      IBInternalQuery->ParamByName("HOURLY_RATE")->AsCurrency = Info.HourlyRate;
      /* Only set for reporting purposes never read to be used*/
	  IBInternalQuery->ParamByName("EARNT_POINTS")->AsCurrency = Info.Points.getPointsBalance(ptstLoyalty);
	  IBInternalQuery->ParamByName("LOADED_POINTS")->AsCurrency = Info.Points.getPointsBalance(ptstAccount);
	  IBInternalQuery->ParamByName("TOTAL_SPENT")->AsCurrency = Info.Points.getPointsBalance();
      /* Needs to be set for if a replacement card is inserted into the system */
	  IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = Info.CardCreationDate;
      IBInternalQuery->ParamByName("MEMBER_TYPE")->AsInteger = Info.MemberType;
      IBInternalQuery->ParamByName("MEMBER_CARD_CODE")->AsString = Info.MemberCode;
      IBInternalQuery->ParamByName("IS_FIRSTVISIT_REWARDED")->AsString = Info.IsFirstVisitRewarded ? 'T' : 'F';
	  IBInternalQuery->ExecQuery();

      TDBContacts::UpdateDetailstoMemberSubs(DBTransaction, inContactKey,inContactType,Info);
      // try to match the current database points with the points in current info object. if they are different, add a sync record to database
      TContactPoints dbPoints;
      TDBContacts::GetPointsBalances(DBTransaction, Info.ContactKey, dbPoints);
     // sync the earned points
      Currency syncPoints = 0;

//      if(TGlobalSettings::Instance().LoyaltyMateEnabled)
//      {
////        syncPoints = Info.Points.getPointsBalance(ptstLoyalty) - dbPoints.getPointsBalanceFromDBWithLoyaltymate(ptstLoyalty);
//        syncPoints = Info.Points.getPointsBalance(ptstLoyalty) - dbPoints.getPointsBalanceFromDBWithLoyaltymate(ptstLoyalty);
//      }
//      else
//      {
//        syncPoints = Info.Points.getPointsBalance(ptstLoyalty) - dbPoints.getPointsBalance(ptstLoyalty);
//      }
      syncPoints = Info.Points.getPointsBalanceFromDBWithLoyaltymate(ptstLoyalty) - dbPoints.getPointsBalanceFromDBWithLoyaltymate(ptstLoyalty);

      if(syncPoints != 0)
      {
            TDBContacts::setPointsTransactionEntry(
                                DBTransaction,
                                Info.ContactKey,
                                Now(),
                                pttSync,
                                ptstLoyalty,
                                syncPoints,
                                pesExported,
                                Now());
      }
      //if(Info.Points.getPointsBalance(ptstLoyalty) != dbPoints.getPointsBalance(ptstLoyalty))
//      {
//            Currency cloud = Info.Points.getPointsBalance(ptstLoyalty);
//            Currency local = dbPoints.getPointsBalance(ptstLoyalty);
//            Currency syncPoints = Info.Points.getPointsBalance(ptstLoyalty) - dbPoints.getPointsBalance(ptstLoyalty);
//            TDBContacts::setPointsTransactionEntry(
//                                DBTransaction,
//                                Info.ContactKey,
//                                Now(),
//                                pttSync,
//                                ptstLoyalty,
//                                syncPoints,
//                                pesExported,
//                                Now());
//      }

      syncPoints = 0;
//      if(TGlobalSettings::Instance().LoyaltyMateEnabled)
//      {
//        syncPoints = Info.Points.getPointsBalance(ptstAccount) - dbPoints.getPointsBalanceFromDBWithLoyaltymate(ptstAccount);
//      }
//      else
//      {
//        syncPoints = Info.Points.getPointsBalance(ptstAccount) - dbPoints.getPointsBalance(ptstAccount);
//      }
       syncPoints = Info.Points.getPointsBalanceFromDBWithLoyaltymate(ptstAccount) - dbPoints.getPointsBalanceFromDBWithLoyaltymate(ptstAccount);
      // sync the loaded points
      if(syncPoints != 0)
      {
            TDBContacts::setPointsTransactionEntry(
                                DBTransaction,
                                Info.ContactKey,
                                Now(),
                                pttSync,
                                ptstAccount,
                                syncPoints,
                                pesExported,
                                Now());
      }
//      if(Info.Points.getPointsBalance(ptstAccount) != dbPoints.getPointsBalance(ptstAccount))
//      {
//            Currency cloud = Info.Points.getPointsBalance(ptstAccount);
//            Currency local = dbPoints.getPointsBalance(ptstAccount);
//            Currency syncPoints = Info.Points.getPointsBalance(ptstAccount) - dbPoints.getPointsBalance(ptstAccount);
//            TDBContacts::setPointsTransactionEntry(
//                                DBTransaction,
//                                Info.ContactKey,
//                                Now(),
//                                pttSync,
//                                ptstAccount,
//                                syncPoints,
//                                pesExported,
//                                Now());
//      }

	  if (static_cast <int> (Info.LastModified) != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "UPDATE CONTACTS SET LAST_MODIFIED = :LAST_MODIFIED WHERE CONTACTS_KEY = :CONTACTS_KEY";
		 IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Info.LastModified;
		 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		 IBInternalQuery->ExecQuery();
	  }

	  for (std::set <UnicodeString> ::iterator itCard = Info.CardsToAdd.begin(); itCard != Info.CardsToAdd.end(); advance(itCard, 1))
	  {
		 SetContactCard(DBTransaction, Info.ContactKey, *itCard);
	  }

	  for (std::set <UnicodeString> ::iterator itCard = Info.CardsToRemove.begin(); itCard != Info.CardsToRemove.end(); advance(itCard, 1))
	  {
		 DeleteContactCard(DBTransaction, *itCard);
	  }

	  DeleteContactDiscounts(DBTransaction, Info.ContactKey);
	  for (std::set <int> ::iterator itDiscounts = Info.AutoAppliedDiscounts.begin(); itDiscounts != Info.AutoAppliedDiscounts.end();
		 std::advance(itDiscounts, 1))
	  {
		 SetContactDiscounts(DBTransaction, Info.ContactKey, *itDiscounts);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message + " Contact Name : " + Info.Name + " Contact Type : " + IntToStr(inContactType)
		 + " Contact 3rdPartyKey : " + IntToStr(Info.ExternalKey));
	  throw;
   }
}

void TDBContacts::SetContactCard(Database::TDBTransaction &DBTransaction, int inContactKey, UnicodeString Card)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

      IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT CONTACTCARDS_KEY,CONTACTS_KEY FROM CONTACTCARDS WHERE SWIPE_CARD = :SWIPE_CARD";
	  IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = Card;
      /* remove any matching cards that belong to other contacts*/
      bool CardAlreadyExistsForContact = false;
      for(IBInternalQuery->ExecQuery(); !IBInternalQuery->Eof; IBInternalQuery->Next())
      {
          if(IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger != inContactKey)
          {
              DeleteContactCard(DBTransaction, Card, IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger);
          }
          else
          {
              CardAlreadyExistsForContact = true;
          }
      }

      if(!CardAlreadyExistsForContact)
      {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTCARDS, 1) FROM RDB$DATABASE";
          IBInternalQuery->ExecQuery();
          int RetVal = IBInternalQuery->Fields[0]->AsInteger;

          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text =
              "INSERT INTO CONTACTCARDS (" "CONTACTCARDS_KEY," "CONTACTS_KEY," "SWIPE_CARD,IS_ACTIVE) " "VALUES (" ":CONTACTCARDS_KEY,"
              ":CONTACTS_KEY," ":SWIPE_CARD,:IS_ACTIVE);";
          IBInternalQuery->ParamByName("CONTACTCARDS_KEY")->AsInteger = RetVal;
          IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
          IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = Card;
          IBInternalQuery->ParamByName("IS_ACTIVE")->AsString = 'T';
          IBInternalQuery->ExecQuery();
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);

   }
}

void TDBContacts::GetStaff(Database::TDBTransaction &DBTransaction, std::vector<TCommission> &inStaff)
{
	try
	{
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
			"select CONTACTS_KEY, NAME from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eStaff;
	  IBInternalQuery->ExecQuery();
	  int i = 1;
	  for(; !IBInternalQuery->Eof; IBInternalQuery->Next(), i++)
	  {
			TCommission temp(IBInternalQuery->FieldByName("NAME")->AsString,
								  IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger,
								  Now(),
								  Now().operator++(),
								  0,
								  true,
								  true,
								  i);
			inStaff.push_back(temp);
	  }
	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}

void TDBContacts::GetStaff(Database::TDBTransaction &DBTransaction, std::vector<TCommission> &inStaff, int ZedKey)
{
	try
	{
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
			"select CONTACTS_KEY, NAME from CONTACTS where CONTACT_TYPE = :CONTACT_TYPE";
	  IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = eStaff;
	  IBInternalQuery->ExecQuery();
	  int i = 1;
	  for(; !IBInternalQuery->Eof; IBInternalQuery->Next(), i++)
	  {
			TCommission temp(IBInternalQuery->FieldByName("NAME")->AsString,
								  IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger,
								  Now(),
								  Now().operator++(),
								  0,
								  true,
								  true,
								  i);
			inStaff.push_back(temp);
	  }

	  for(int i = 0; i < inStaff.size(); i++)
	  {
		  IBInternalQuery->Close();
		  IBInternalQuery->SQL->Text =
				"SELECT * FROM COMMISSION WHERE COMMISSION_KEY < 0 AND STAFF_KEY = :STAFF_KEY AND Z_KEY > :Z_KEY;";
		  IBInternalQuery->ParamByName("STAFF_KEY")->AsInteger = inStaff.at(i).GetNumber();
		  IBInternalQuery->ParamByName("Z_KEY")->AsInteger = ZedKey;
		  IBInternalQuery->ExecQuery();
		  if(IBInternalQuery->RecordCount)
		  {
			  inStaff.at(i).SetDateFrom(IBInternalQuery->FieldByName("DATEFROM")->AsDateTime);
			  inStaff.at(i).SetDateTo(IBInternalQuery->FieldByName("DATETO")->AsDateTime);
			  inStaff.at(i).SetCommission(IBInternalQuery->FieldByName("TOTALCOMMISSION")->AsCurrency);
			  inStaff.at(i).SetisDateFromNull(false);
			  inStaff.at(i).SetisDateToNull(false);
		  }



	  }
	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}

void TDBContacts::DeleteContactCard(Database::TDBTransaction &DBTransaction, UnicodeString Card, int inContactKey)
{
   try
   {
	  if (Card != "")
	  {
		 Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		 DBTransaction.RegisterQuery(IBInternalQuery);

		 IBInternalQuery->Close();
         if(inContactKey == 0)
         {
		 	IBInternalQuery->SQL->Text = "DELETE FROM CONTACTCARDS WHERE SWIPE_CARD = :SWIPE_CARD";
         }
         else
         {
             IBInternalQuery->SQL->Text = "DELETE FROM CONTACTCARDS WHERE SWIPE_CARD = :SWIPE_CARD AND CONTACTS_KEY = :CONTACTS_KEY";
	     	 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
         }
     	 IBInternalQuery->ParamByName("SWIPE_CARD")->AsString = Card;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TDBContacts::DeleteContactCards(Database::TDBTransaction &DBTransaction, int inContactKey)
{
   try
   {
	  if (inContactKey != 0)
	  {
		 Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		 DBTransaction.RegisterQuery(IBInternalQuery);

		 IBInternalQuery->Close();
         IBInternalQuery->SQL->Text = "DELETE FROM CONTACTCARDS WHERE CONTACTS_KEY = :CONTACTS_KEY";
	   	 IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TDBContacts::DeleteContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	try
	{
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "DELETE FROM CONTACTDISCOUNTS WHERE CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}

void TDBContacts::SetContactDiscounts(Database::TDBTransaction &DBTransaction, int inContactKey, int DiscountKey)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTDISCOUNTS, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  int RetVal = IBInternalQuery->Fields[0]->AsInteger;

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		  "INSERT INTO CONTACTDISCOUNTS (" "CONTACTDISCOUNTS_KEY," "CONTACTS_KEY," "DISCOUNT_KEY) " "VALUES ("
		  ":CONTACTDISCOUNTS_KEY," ":CONTACTS_KEY," ":DISCOUNT_KEY);";
	  IBInternalQuery->ParamByName("CONTACTDISCOUNTS_KEY")->AsInteger = RetVal;
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = DiscountKey;
	  IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}
//-----------------------------------------------------------------------
void TDBContacts::GetContactNameList(Database::TDBTransaction &DBTransaction, std::vector<ContactGroup> &vectorMembers, bool inProfile)
{

   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
			"Select * from CONTACTS where ACCOUNT_PROFILE = :ACCOUNT_PROFILE and CONTACT_TYPE = :CONTACT_TYPE";
		  IBInternalQuery->ParamByName("ACCOUNT_PROFILE")->AsInteger = inProfile;
          IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = 2;
	  IBInternalQuery->ExecQuery();
		 for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		 {
					ContactGroup ContactData;
        		ContactData.Name = IBInternalQuery->FieldByName("NAME")->AsString;
                	ContactData.Key  = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
                	vectorMembers.push_back(ContactData);
		 }


   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
	}
}

int TDBContacts::HasAccountProfile(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	try
	{
   	  int RetVal;
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT ACCOUNT_PROFILE FROM CONTACTS WHERE CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();

		  RetVal = IBInternalQuery->FieldByName("ACCOUNT_PROFILE")->AsInteger;
          return RetVal;
	}
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}

}

int TDBContacts::GetCurrentGroups(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
	int retval = false;
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		" SELECT GROUPS.NAME, GROUPS.GROUPS_KEY, GROUPS.TYPE, CONTACTGROUPS.CONTACTS_KEY FROM CONTACTGROUPS LEFT JOIN GROUPS ON CONTACTGROUPS.GROUPS_KEY = GROUPS.GROUPS_KEY "
		" WHERE CONTACTS_KEY = :CONTACTS_KEY AND STATUS = :STATUS ORDER BY TYPE, NAME ";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ParamByName("STATUS")->AsInteger = 0;
	  IBInternalQuery->ExecQuery();
	  for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
	  {
		ContactGroup temp;
		retval = true;
		temp.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		temp.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		temp.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		Info.currentGroups.push_back(temp);
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return retval;
}

int TDBContacts::GetAvailableGroups(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
	try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		" SELECT * FROM GROUPS WHERE STATUS = :STATUS ORDER BY TYPE, NAME";
	  IBInternalQuery->ParamByName("STATUS")->AsInteger = 0;
	  IBInternalQuery->ExecQuery();
	  for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
	  {
		ContactGroup temp;
		temp.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		temp.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		temp.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;
		Info.availableGroups.push_back(temp);
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

int TDBContacts::GetSummaGroup(Database::TDBTransaction &DBTransaction, int inContactKey ,TMMContactInfo &Info)
{
	try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		" SELECT GROUPS.TYPE, GROUPS.NAME, GROUPS.GROUPS_KEY FROM GROUPS LEFT JOIN CONTACTGROUPS ON GROUPS.GROUPS_KEY = CONTACTGROUPS.GROUPS_KEY WHERE CONTACTGROUPS.CONTACTS_KEY = :CONTACTS_KEY and CONTACTGROUPS.SUMMA_GROUP = 1 ";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	  IBInternalQuery->ExecQuery();
	  if(IBInternalQuery->RecordCount)
	  {

		Info.summagroup.Name = IBInternalQuery->FieldByName("NAME")->AsString;
		Info.summagroup.Key = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
		Info.summagroup.Type = IBInternalQuery->FieldByName("TYPE")->AsInteger;
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }


}

void TDBContacts::SetCurrentGroups(Database::TDBTransaction &DBTransaction, int inContactKey ,ContactGroup Info)
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);


	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CONTACTGROUPS_KEY, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  int RetVal = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" INSERT INTO CONTACTGROUPS(CONTACTGROUPS_KEY, CONTACTS_KEY, GROUPS_KEY) VALUES (:CONTACTGROUPS_KEY, :CONTACTS_KEY, :GROUPS_KEY);";
		IBInternalQuery->ParamByName("CONTACTGROUPS_KEY")->AsInteger = RetVal;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = Info.Key;
		IBInternalQuery->ExecQuery();


   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

void TDBContacts::RemoveCurrentGroups(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	try
   {
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
		"Delete from CONTACTGROUPS where CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	IBInternalQuery->ExecQuery();

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
}

void TDBContacts::SetSummaGroup(Database::TDBTransaction &DBTransaction, int inContactKey, TMMContactInfo &Info)
{
   try
   {
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
		"UPDATE CONTACTGROUPS SET SUMMA_GROUP = 1 WHERE GROUPS_KEY = :GROUPS_KEY and CONTACTS_KEY = :CONTACTS_KEY";
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = Info.summagroup.Key;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }


}

void TDBContacts::GetAllTypes(Database::TDBTransaction &DBTransaction, std::vector<int> &inGroupKeys)
{
   try
   {
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
		"Select distinct GROUPS_KEY from contactgroups;";
	IBInternalQuery->ExecQuery();
		for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			int Type = IBInternalQuery->FieldByName("GROUPS_KEY")->AsInteger;
			inGroupKeys.push_back(Type);
		}
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

void TDBContacts::RemoveDefaultGroup(Database::TDBTransaction &DBTransaction, int inContactKey)
{
	try
   {
   ContactGroup GroupKey;
   GroupKey.Key = TDBGroups::FindGroup(DBTransaction, "No Contacts Group");


	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->SQL->Text =
		"Delete from CONTACTGROUPS where CONTACTS_KEY = :CONTACTS_KEY and GROUPS_KEY = :GROUPS_KEY";
	IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
	IBInternalQuery->ParamByName("GROUPS_KEY")->AsInteger = GroupKey.Key;
	IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }

}

AnsiString TDBContacts::GetContactName(Database::TDBTransaction &DBTransaction, int ContactKey)
{
	AnsiString RetVal = "";
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "NAME " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("NAME")->AsString;
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);

   }
   return RetVal;
}
//----------------------------------------------------------------------------
bool TDBContacts::GetContactCards(Database::TDBTransaction &DBTransaction,int inContactsKey, std::set <UnicodeString> &Cards)
{
   bool RetVal = false;
   try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		// Contact Cards.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		 "select SWIPE_CARD from CONTACTCARDS where CONTACTS_KEY = :CONTACTS_KEY AND IS_ACTIVE='T' ORDER BY CONTACTCARDS_KEY";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactsKey;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
        	Cards.insert(IBInternalQuery->FieldByName("SWIPE_CARD")->AsString);
		}
        RetVal = Cards.size() > 0;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TDBContacts::ClearPIN(Database::TDBTransaction &DBTransaction, int inContactKey, TContactType Type)
{
   try
   {
   /* I pass this contact type in as well, this makes certian we are only clearing the pin for a certian type of member
    ( as this is currenlty called from the external membership Module ) for instance if the time comes where a staff
    member can also be a "Loyatly Member" then we do not want to clear the PIN all the time,
    if membership is applied via the handhelds. At this stage very unlikly to occur but makes a good double check. */

        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =
        	"UPDATE CONTACTS SET PIN = '' WHERE CONTACTS_KEY = :CONTACTS_KEY AND CONTACT_TYPE = :CONTACT_TYPE";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
        IBInternalQuery->ParamByName("CONTACT_TYPE")->AsInteger = Type;
        IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

AnsiString TDBContacts::GetPIN(Database::TDBTransaction &DBTransaction, int ContactKey)
{
	AnsiString RetVal = "";
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT " "PIN " "FROM " "CONTACTS " "WHERE " "CONTACTS_KEY = :CONTACTS_KEY";
	  IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
	  IBInternalQuery->ExecQuery();

	  if (IBInternalQuery->RecordCount)
	  {
		 RetVal = IBInternalQuery->FieldByName("PIN")->AsString;
	  }

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);

   }
   return RetVal;
}
//-------------------------------------------------------------------------
int TDBContacts::getNextPointTransactionID(Database::TDBTransaction &DBTransaction)
{
     Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
     DBTransaction.RegisterQuery(IBInternalQuery);

     IBInternalQuery->Close();
     IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_POINTSTRANSACTIONS, 1) FROM RDB$DATABASE";
     IBInternalQuery->ExecQuery();
     return IBInternalQuery->Fields[0]->AsInteger;
}
//-------------------------------------------------------------------------
void TDBContacts::setPointsTransactionEntry(
                                Database::TDBTransaction &DBTransaction,
                                int contactKey,
                                TDateTime timestamp,
                                TPointsTransactionType adjustmentType,
                                TPointsTransactionAccountType adjustmentSubType,
                                Currency adjustment,
                                TPointsExportStatus exportStatus,
                                TDateTime exportTimeStamp)
{
 if(adjustment != 0.0)
 {
     Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
     DBTransaction.RegisterQuery(IBInternalQuery);

     IBInternalQuery->Close();
     IBInternalQuery->SQL->Text = TDBContacts::getPointsTransactionInsertQuery();
     IBInternalQuery->ParamByName("POINTSTRANSACTIONS_KEY")->AsInteger = TDBContacts::getNextPointTransactionID(DBTransaction);
     IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
     IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = timestamp;
     IBInternalQuery->ParamByName("TIME_STAMP_EXPORTED")->AsDateTime = exportTimeStamp;
     IBInternalQuery->ParamByName("ADJUSTMENT_TYPE")->AsInteger = adjustmentType;
     IBInternalQuery->ParamByName("ADJUSTMENT_SUBTYPE")->AsInteger = adjustmentSubType;
     IBInternalQuery->ParamByName("EXPORTED_STATUS")->AsInteger = exportStatus;
     IBInternalQuery->ParamByName("ADJUSTMENT")->AsCurrency = adjustment;
     IBInternalQuery->ExecQuery();
 }
}
//-------------------------------------------------------------------------
UnicodeString TDBContacts::getPointsTransactionInsertQuery()
{
    UnicodeString query="INSERT INTO POINTSTRANSACTIONS "
			"("
				"POINTSTRANSACTIONS_KEY,"
				"CONTACTS_KEY,"
				"TIME_STAMP,"
				"TIME_STAMP_EXPORTED,"
				"ADJUSTMENT_TYPE,"
				"ADJUSTMENT_SUBTYPE,"
				"ADJUSTMENT,"
				"EXPORTED_STATUS"
                //,"
				//"MISC,"
				//"INVOICE_NUMBER"
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
				":EXPORTED_STATUS"
                //,"
				//":MISC,"
				//":INVOICE_NUMBER"
			");";

    return query;
}
//-------------------------------------------------------------------------
UnicodeString TDBContacts::GetMemberCloudIdIfRegistered(
										Database::TDBTransaction &DBTransaction,
                                        int contactKey,
                                        int siteId)
{
   UnicodeString result = "";
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT UUID FROM LOYALTYATTRIBUTES WHERE CONTACTS_KEY=:CONTACTS_KEY AND SITE_ID=:SITE_ID";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
      IBInternalQuery->ParamByName("SITE_ID")->AsInteger = siteId;
	  IBInternalQuery->ExecQuery();

      if(!IBInternalQuery->Eof)
      {
        result = IBInternalQuery->Fields[0]->AsString;
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return result;
}

UnicodeString TDBContacts::GetMemberCloudId(Database::TDBTransaction &DBTransaction,int contactKey)
{
   UnicodeString result = "";
   try
   {
	  TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT UUID FROM LOYALTYATTRIBUTES WHERE CONTACTS_KEY=:CONTACTS_KEY";
      IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = contactKey;
	  IBInternalQuery->ExecQuery();

      if(!IBInternalQuery->Eof)
      {
        result = IBInternalQuery->Fields[0]->AsString;
      }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	  throw;
   }
   return result;
}

void TDBContacts::UpdateMemberCardCodeToDB(Database::TDBTransaction &DBTransaction, TMMContactInfo &UserInfo,AnsiString memberCardCode)
{
  try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"CONTACTS "
		"SET "
        "MEMBER_CARD_CODE = :MEMBER_CARD_CODE "
		"WHERE "
		"CONTACTS_KEY = :CONTACTS_KEY";
		IBInternalQuery->ParamByName("MEMBER_CARD_CODE")->AsString = memberCardCode;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = UserInfo.ContactKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void TDBContacts::SaveCustomerAndNumber( Database::TDBTransaction &DBTransaction, TCustomer Customer )
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);


	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_HOTELCUSTOMER, 1) FROM RDB$DATABASE";
	  IBInternalQuery->ExecQuery();
	  int Key = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			" INSERT INTO HOTELCUSTOMER ( HOTELCUSTOMER_KEY, "
										  "CUSTOMER_NAME, "
										  "ROOM_NUMBER, "
										  "INVOICE_KEY ) "
							   " VALUES ( :HOTELCUSTOMER_KEY, "
										 ":CUSTOMER_NAME, "
										 ":ROOM_NUMBER, "
										 ":INVOICE_KEY );";

		IBInternalQuery->ParamByName("HOTELCUSTOMER_KEY")->AsInteger = Key;
		IBInternalQuery->ParamByName("CUSTOMER_NAME")->AsString = Customer.Name;
		IBInternalQuery->ParamByName("ROOM_NUMBER")->AsInteger = Customer.RoomNumber;
		IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = Customer.InvoiceKey;
		IBInternalQuery->ExecQuery();


   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

TCustomer TDBContacts::GetCustomerAndRoomNumber( Database::TDBTransaction &DBTransaction, const int InvoiceKey )
{
   try
   {
	  Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text =
		"SELECT * FROM HOTELCUSTOMER WHERE INVOICE_KEY = :INVOICE_KEY;";
	  IBInternalQuery->ParamByName("INVOICE_KEY")->AsInteger = InvoiceKey;
	  IBInternalQuery->ExecQuery();
      return TCustomer( IBInternalQuery->FieldByName("ROOM_NUMBER")->AsInteger,
						IBInternalQuery->FieldByName("INVOICE_KEY")->AsInteger,
						IBInternalQuery->FieldByName("CUSTOMER_NAME")->AsString );
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

TDateTime TDBContacts::GetCardCreationDate( Database::TDBTransaction &DBTransaction, int inContactKey )
{
	TDateTime RetVal = 0;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"SELECT "
				"CARD_CREATION_DATE "
			"FROM "
				"CONTACTS "
			"WHERE "
				"CONTACTS_KEY = :CONTACTS_KEY;";
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = inContactKey;
		IBInternalQuery->ExecQuery();
		return IBInternalQuery->FieldByName("CARD_CREATION_DATE")->AsDateTime;
	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}

void TDBContacts::SetCardCreationDate( Database::TDBTransaction &DBTransaction, int inContactKey, TDateTime inCreationDate )
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
			"UPDATE "
				"CONTACTS "
			"SET "
				"CARD_CREATION_DATE = :CARD_CREATION_DATE "
			"WHERE "
				"CONTACTS_KEY = :CONTACTS_KEY;";
		IBInternalQuery->ParamByName("CARD_CREATION_DATE")->AsDateTime = inCreationDate;
		IBInternalQuery->ParamByName("CONTACTS_KEY"		 )->AsInteger = inContactKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}

void TDBContacts::GetTierLevelAndMemberType(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT TIER_LEVEL,MEMBER_TYPE FROM CONTACTS WHERE CONTACTS_KEY = :CONTACTS_KEY";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           Member.TierLevel = IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
           Member.MemberType = IBInternalQuery->FieldByName("MEMBER_TYPE")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

int TDBContacts::GenerateNextMemberEmailNumber(Database::TDBTransaction &DBTransaction)
{
    int result = -1;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_LOYALTYMEMBERSHIPID, 1) FROM RDB$DATABASE";
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
      {
        result = IBInternalQuery->Fields[0]->AsInteger;
      }
    return result;
}

int TDBContacts::GetLoyaltyKey(Database::TDBTransaction &dBTransaction, int tabKey )
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		dBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
            "SELECT "
                " ORDERS.LOYALTY_KEY "
            "FROM "
                "TAB INNER JOIN ORDERS "
            "ON ORDERS.TAB_KEY = TAB.TAB_KEY  "
                "WHERE TAB.TAB_KEY = :TAB_KEY AND ORDERS.LOYALTY_KEY > 0 "
            "GROUP BY ORDERS.LOYALTY_KEY ";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
		    return IBInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger;
        }
        else
        {
            return 0;
        }
	}
	catch(Exception & E)
	{
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
	}
}

void TDBContacts::GetEarnedAndLoadedPointsBalances(Database::TDBTransaction &DBTransaction,int ContactKey, TContactPoints & Points)
{
   try
   {
         TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        // Earned Held Points.
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text ="Select LOADED_POINTS , EARNT_POINTS from Contacts where CONTACTS_KEY = :CONTACTS_KEY ";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
        IBInternalQuery->ExecQuery();

        // we are loading the points information from the database. At this point I (Nadun) have called the ClearBySource method on existing
        // points structure so there will be no duplicate point information will be left once the function is executed. Otherwise members points
        // were added again which causes points to double up
        if(!IBInternalQuery->Eof)
            Points.ClearBySource(pasDatabase);

            TPointsTypePair PairEarned = TContactPoints::GetPointsType(1,1);
            TPointsType Type(pasDatabase,PairEarned,pesNone);
            Points.Load(Type, IBInternalQuery->FieldByName("LOADED_POINTS")->AsCurrency);

            TPointsTypePair PairLoaded = TContactPoints::GetPointsType(2,1);
            TPointsType Type1(pasDatabase,PairLoaded,pesNone);
            Points.Load(Type, IBInternalQuery->FieldByName("EARNT_POINTS")->AsCurrency);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
   }
}

bool TDBContacts::IsFirstVisitRewarded(Database::TDBTransaction &DBTransaction,int ContactKey)
{
   bool RetVal = false;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "select IS_FIRSTVISIT_REWARDED from CONTACTS where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof)
     RetVal = IBInternalQuery->FieldByName("IS_FIRSTVISIT_REWARDED")->AsString == 'T' ? true : false;
   return RetVal;
}

void TDBContacts::SetFirstVisitRewardStatus(Database::TDBTransaction &DBTransaction,int ContactKey)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "Update CONTACTS set IS_FIRSTVISIT_REWARDED = 'T' where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
   IBInternalQuery->ExecQuery();
}
UnicodeString TDBContacts::GetLastNameForLocalCard(Database::TDBTransaction &DBTransaction,int ContactKey)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT LAST_NAME FROM CONTACTS where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactKey;
   IBInternalQuery->ExecQuery();
   if(IBInternalQuery->RecordCount > 0)
       return IBInternalQuery->FieldByName("LAST_NAME")->AsString;
   else
       return "";
}

