//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBTierLevel.h"
#include "MMLogging.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
TDBTierLevel::TDBTierLevel()
{
}
//---------------------------------------------------------------------------
TDBTierLevel::~TDBTierLevel()
{
}
//---------------------------------------------------------------------------
//Get birthday bonus poits for the tier level
int TDBTierLevel::GetBirthdayBonusPoints(Database::TDBTransaction &DBTransaction, int tierLevel)
{
   int birthdayBonus = 0;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        if(tierLevel > 0)
        {
          IBInternalQuery->SQL->Text =  "SELECT BIRTHDAY_BONUS FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
          IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = tierLevel;
          IBInternalQuery->ExecQuery();
          if(!IBInternalQuery->Eof)
             {
               birthdayBonus = IBInternalQuery->FieldByName("BIRTHDAY_BONUS")->AsInteger;
             }
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return birthdayBonus;
}
//---------------------------------------------------------------------------
//Get tier level greater than current tier level
int TDBTierLevel::GetNextTierLevel(Database::TDBTransaction &DBTransaction, int tierLevel)
{
   int nextTierLevel = 0;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT min(TIER_LEVEL) TIER_LEVEL FROM TIER_LEVELS  where TIER_LEVEL > :TIER_LEVEL";
        IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = tierLevel;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           nextTierLevel = IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return nextTierLevel;
}
//---------------------------------------------------------------------------
//Get points required for a tier level
int TDBTierLevel::GetPointsrequired(Database::TDBTransaction &DBTransaction, int tierLevel)
{
   int pointsRequired = 0;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT POINTS_REQUIRED FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
        IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = tierLevel;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           pointsRequired = IBInternalQuery->FieldByName("POINTS_REQUIRED")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return pointsRequired;
}
//---------------------------------------------------------------------------
int TDBTierLevel::GetTierLevelAsPerEarnedPoints(Database::TDBTransaction &DBTransaction, double earnedPoints)
{
   int tierLevel = 0;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =  "SELECT FIRST 1 TIER_LEVEL FROM TIER_LEVELS  where POINTS_REQUIRED <= :POINTS_REQUIRED "
                                      "ORDER BY POINTS_REQUIRED DESC ";
        IBInternalQuery->ParamByName("POINTS_REQUIRED")->AsFloat = earnedPoints;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           tierLevel = IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return tierLevel;
}
//---------------------------------------------------------------------------
AnsiString TDBTierLevel::GetTierLevelName(Database::TDBTransaction &DBTransaction, int tierLevel)
{
   AnsiString TierName = "";
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT TIER_NAME FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
        IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = tierLevel;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           TierName = IBInternalQuery->FieldByName("TIER_NAME")->AsString;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return TierName;
}
//---------------------------------------------------------------------------
//Delete a tier level from database
void TDBTierLevel::DeleteTierLevel(Database::TDBTransaction &DBTransaction, int tierKey)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "DELETE FROM TIER_LEVELS WHERE TIER_ID = :TIER_ID";
    IBInternalQuery->ParamByName("TIER_ID")->AsInteger = tierKey;
    IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
//save tier level in database
void TDBTierLevel::AddTierLevel(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "SELECT GEN_ID(GEN_TIER_LEVELS_ID, 1) FROM RDB$DATABASE";
    IBInternalQuery->ExecQuery();
    TierLevel->TierKey = IBInternalQuery->Fields[0]->AsInteger;
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "INSERT INTO TIER_LEVELS (TIER_LEVEL, TIER_NAME, POINTS_REQUIRED, "
                                 "BIRTHDAY_BONUS, PRICED_REDEMPTION_RATE, WEIGHED_REDEMPTION_WEIGHT, "
                                 "WEIGHED_REDEMPTION_POINTS, TIER_ID,SEND_MAIL_TO_HO,SEND_MAIL_TO_MEMBER,CHANGE_CARD,ALLOW_EARNT_REDEMPTION, "
                                 " CLOUD_ID) "
                                 "VALUES ( "
                                 ":TIER_LEVEL, "
                                 ":TIER_NAME, "
                                 ":POINTS_REQUIRED, "
                                 ":BIRTHDAY_BONUS, "
                                 ":PRICED_REDEMPTION_RATE, "
                                 ":WEIGHED_REDEMPTION_WEIGHT, "
                                 ":WEIGHED_REDEMPTION_POINTS, "
                                 ":TIER_ID, "
                                 ":SEND_MAIL_TO_HO, "
                                 ":SEND_MAIL_TO_MEMBER, "
                                 ":CHANGE_CARD, "
                                 ":ALLOW_EARNT_REDEMPTION, "
                                 ":CLOUD_ID )";
    IBInternalQuery->ParamByName("TIER_ID")->AsInteger = TierLevel->TierKey;
    IBInternalQuery->ParamByName("CLOUD_ID")->AsInteger = TierLevel->CloudId;
    IBInternalQuery->ParamByName("TIER_NAME")->AsString = TierLevel->Name;
    IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = TierLevel->Level;
    IBInternalQuery->ParamByName("POINTS_REQUIRED")->AsInteger = TierLevel->PointRequired;
    IBInternalQuery->ParamByName("BIRTHDAY_BONUS")->AsInteger = TierLevel->BirthDayBonus;
    IBInternalQuery->ParamByName("PRICED_REDEMPTION_RATE")->AsCurrency = TierLevel->PointRedemRate;
    IBInternalQuery->ParamByName("WEIGHED_REDEMPTION_WEIGHT")->AsCurrency = TierLevel->WeighedRedemWeight;
    IBInternalQuery->ParamByName("WEIGHED_REDEMPTION_POINTS")->AsCurrency = TierLevel->WeighedRedemPoint;
    IBInternalQuery->ParamByName("SEND_MAIL_TO_HO")->AsString = TierLevel->SendMailToHeadOffice == true ? 'T' : 'F' ;
    IBInternalQuery->ParamByName("SEND_MAIL_TO_MEMBER")->AsString = TierLevel->SendMailToMember== true ? 'T' : 'F' ;
    IBInternalQuery->ParamByName("CHANGE_CARD")->AsString = TierLevel->ChangeCard== true ? 'T' : 'F' ;
    IBInternalQuery->ParamByName("ALLOW_EARNT_REDEMPTION")->AsString = TierLevel->AllowEarntRedemption== true ? 'T' : 'F' ;
    IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
//update tier level data
void TDBTierLevel::UpdateTierLevel(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "UPDATE TIER_LEVELS SET "
                                  "TIER_NAME = :TIER_NAME, "
                                  "TIER_LEVEL = :TIER_LEVEL, "
                                  "POINTS_REQUIRED = :POINTS_REQUIRED, "
                                  "BIRTHDAY_BONUS =:BIRTHDAY_BONUS, "
                                  "PRICED_REDEMPTION_RATE = :PRICED_REDEMPTION_RATE, "
                                  "WEIGHED_REDEMPTION_WEIGHT = :WEIGHED_REDEMPTION_WEIGHT, "
                                  "WEIGHED_REDEMPTION_POINTS = :WEIGHED_REDEMPTION_POINTS, "
                                  "SEND_MAIL_TO_HO = :SEND_MAIL_TO_HO, "
                                  "SEND_MAIL_TO_MEMBER = :SEND_MAIL_TO_MEMBER, "
                                  "CHANGE_CARD = :CHANGE_CARD, "
                                  "ALLOW_EARNT_REDEMPTION = :ALLOW_EARNT_REDEMPTION, "
                                  "CLOUD_ID = :CLOUD_ID "
                                  "WHERE TIER_ID=:TIER_ID" ;
    IBInternalQuery->ParamByName("TIER_ID")->AsInteger = TierLevel->TierKey;
    IBInternalQuery->ParamByName("CLOUD_ID")->AsInteger = TierLevel->CloudId;
    IBInternalQuery->ParamByName("TIER_NAME")->AsString = TierLevel->Name;
    IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = TierLevel->Level;
    IBInternalQuery->ParamByName("POINTS_REQUIRED")->AsInteger = TierLevel->PointRequired;
    IBInternalQuery->ParamByName("BIRTHDAY_BONUS")->AsInteger = TierLevel->BirthDayBonus;
    IBInternalQuery->ParamByName("PRICED_REDEMPTION_RATE")->AsCurrency = TierLevel->PointRedemRate;
    IBInternalQuery->ParamByName("WEIGHED_REDEMPTION_WEIGHT")->AsCurrency = TierLevel->WeighedRedemWeight;
    IBInternalQuery->ParamByName("WEIGHED_REDEMPTION_POINTS")->AsCurrency = TierLevel->WeighedRedemPoint;
    IBInternalQuery->ParamByName("SEND_MAIL_TO_HO")->AsString = TierLevel->SendMailToHeadOffice == true ? 'T' : 'F' ;
    IBInternalQuery->ParamByName("SEND_MAIL_TO_MEMBER")->AsString = TierLevel->SendMailToMember== true ? 'T' : 'F' ;
    IBInternalQuery->ParamByName("CHANGE_CARD")->AsString = TierLevel->ChangeCard== true ? 'T' : 'F' ;
    IBInternalQuery->ParamByName("ALLOW_EARNT_REDEMPTION")->AsString = TierLevel->AllowEarntRedemption== true ? 'T' : 'F' ;
    IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
//Get all infromation of teir level
void TDBTierLevel::GetTierLevelInformation(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "SELECT * FROM TIER_LEVELS WHERE TIER_ID = :TIER_ID";
    IBInternalQuery->ParamByName("TIER_ID")->AsInteger = TierLevel->TierKey;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
     {
        TierLevel->CloudId = IBInternalQuery->FieldByName("CLOUD_ID")->AsInteger;
        TierLevel->Name =  IBInternalQuery->FieldByName("TIER_NAME")->AsString;
        TierLevel->Level =  IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
        TierLevel->PointRequired =  IBInternalQuery->FieldByName("POINTS_REQUIRED")->AsInteger;
        TierLevel->BirthDayBonus =  IBInternalQuery->FieldByName("BIRTHDAY_BONUS")->AsInteger;
        TierLevel->PointRedemRate =  IBInternalQuery->FieldByName("PRICED_REDEMPTION_RATE")->AsCurrency;
        TierLevel->WeighedRedemWeight =  IBInternalQuery->FieldByName("WEIGHED_REDEMPTION_WEIGHT")->AsInteger;
        TierLevel->WeighedRedemPoint =  IBInternalQuery->FieldByName("WEIGHED_REDEMPTION_POINTS")->AsInteger;
        TierLevel->SendMailToHeadOffice =  IBInternalQuery->FieldByName("SEND_MAIL_TO_HO")->AsString == 'T' ? true : false;
        TierLevel->SendMailToMember =  IBInternalQuery->FieldByName("SEND_MAIL_TO_MEMBER")->AsString== 'T' ? true : false;
        TierLevel->ChangeCard =  IBInternalQuery->FieldByName("CHANGE_CARD")->AsString== 'T' ? true : false;
        TierLevel->AllowEarntRedemption = IBInternalQuery->FieldByName("ALLOW_EARNT_REDEMPTION")->AsString == 'T' ? true : false;
     }
}
//---------------------------------------------------------------------------
void TDBTierLevel::GetTierLevelInformationByTierLevel(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "SELECT * FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
    IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = TierLevel->Level;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
     {
        TierLevel->Name =  IBInternalQuery->FieldByName("TIER_NAME")->AsString;
        TierLevel->CloudId = IBInternalQuery->FieldByName("CLOUD_ID")->AsInteger;
        TierLevel->TierKey =  IBInternalQuery->FieldByName("TIER_ID")->AsInteger;
        TierLevel->PointRequired =  IBInternalQuery->FieldByName("POINTS_REQUIRED")->AsInteger;
        TierLevel->BirthDayBonus =  IBInternalQuery->FieldByName("BIRTHDAY_BONUS")->AsInteger;
        TierLevel->PointRedemRate =  IBInternalQuery->FieldByName("PRICED_REDEMPTION_RATE")->AsCurrency;
        TierLevel->WeighedRedemWeight =  IBInternalQuery->FieldByName("WEIGHED_REDEMPTION_WEIGHT")->AsInteger;
        TierLevel->WeighedRedemPoint =  IBInternalQuery->FieldByName("WEIGHED_REDEMPTION_POINTS")->AsInteger;
        TierLevel->SendMailToHeadOffice =  IBInternalQuery->FieldByName("SEND_MAIL_TO_HO")->AsString == 'T' ? true : false;
        TierLevel->SendMailToMember =  IBInternalQuery->FieldByName("SEND_MAIL_TO_MEMBER")->AsString== 'T' ? true : false;
        TierLevel->ChangeCard =  IBInternalQuery->FieldByName("CHANGE_CARD")->AsString== 'T' ? true : false;
        TierLevel->AllowEarntRedemption = IBInternalQuery->FieldByName("ALLOW_EARNT_REDEMPTION")->AsString == 'T' ? true : false;
     }
}
//---------------------------------------------------------------------------
//check that tier level already exist
bool TDBTierLevel::CheckTierLevelExist(Database::TDBTransaction &DBTransaction, TTierLevel *TierLevel,bool IsEditMode)
{
    bool retVal = true;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "SELECT * FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
    IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = TierLevel->Level;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
       //if in edit mode then ckeck for tier id
       if(IsEditMode && TierLevel->TierKey == IBInternalQuery->FieldByName("TIER_ID")->AsInteger)
        {
          retVal = false;
        }
        else
        {
          retVal = true;
        }
    }
    else
    {
       retVal = false;
    }
    return retVal;
}
//---------------------------------------------------------------------------
//Get all tier levels from database
void TDBTierLevel::GetTierLevels(Database::TDBTransaction &DBTransaction,TStringList *TierLevels)
{
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->SQL->Text =  "SELECT * FROM TIER_LEVELS";
    IBInternalQuery->ExecQuery();
    if(IBInternalQuery->RecordCount && TierLevels != NULL)
	{
            for (;!IBInternalQuery->Eof; IBInternalQuery->Next())
            {
                int Index = TierLevels->Add(IBInternalQuery->FieldByName("TIER_NAME")->AsString);
                TierLevels->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("TIER_ID")->AsInteger;
            }
	}
}
//---------------------------------------------------------------------------
void TDBTierLevel::UpdateMemberTierLevel(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text =
	  "UPDATE CONTACTS SET TIER_LEVEL = :TIER_LEVEL, LAST_MODIFIED = :LAST_MODIFIED where CONTACTS_KEY = :CONTACTS_KEY ";
   IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
   IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = Member.TierLevel;
   IBInternalQuery->ParamByName("LAST_MODIFIED")->AsDateTime = Now();
   IBInternalQuery->ExecQuery();
}
//---------------------------------------------------------------------------
int TDBTierLevel::GetTierCloudId(Database::TDBTransaction &DBTransaction, int tierLevel)
{
 int tierCloudId = 0;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT CLOUD_ID FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
        IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = tierLevel;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
          tierCloudId = IBInternalQuery->FieldByName("CLOUD_ID")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return tierCloudId;
}
//---------------------------------------------------------------------------

int TDBTierLevel::GetTierLevelFromCloudId(Database::TDBTransaction &DBTransaction,int cloudId)
{
 int tierLevel = 0;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT TIER_LEVEL FROM TIER_LEVELS WHERE CLOUD_ID = :CLOUD_ID";
        IBInternalQuery->ParamByName("CLOUD_ID")->AsInteger = cloudId;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
          tierLevel = IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return tierLevel;
}

//---------------------------------------------------------------------------
void TDBTierLevel::GetTierLevelOfMember(Database::TDBTransaction &DBTransaction,TMMContactInfo &Member)
{
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT TIER_LEVEL FROM CONTACTS WHERE CONTACTS_KEY = :CONTACTS_KEY";
        IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Member.ContactKey;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           Member.TierLevel = IBInternalQuery->FieldByName("TIER_LEVEL")->AsInteger;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

bool TDBTierLevel::IsAllowEarnedRedemption(Database::TDBTransaction &DBTransaction,int tierLevel)
{
 bool retVal = false;
   try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "SELECT ALLOW_EARNT_REDEMPTION FROM TIER_LEVELS WHERE TIER_LEVEL = :TIER_LEVEL";
        IBInternalQuery->ParamByName("TIER_LEVEL")->AsInteger = tierLevel;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
          retVal = IBInternalQuery->FieldByName("ALLOW_EARNT_REDEMPTION")->AsString == 'T' ? true : false;
         }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return retVal;

}

void TDBTierLevel::ClearAll(Database::TDBTransaction &DBTransaction)
{
 try
   {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text =  "DELETE FROM TIER_LEVELS ";
        IBInternalQuery->ExecQuery();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
