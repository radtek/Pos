//---------------------------------------------------------------------------


#pragma hdrstop

#include "TaxProfileDBAccessManager_MM.h"

//---------------------------------------------------------------------------

TaxProfileMap TTaxProfileDBAccessManager_MM::LoadTaxProfiles(Database::TDBTransaction &transaction)
{
   TaxProfileMap profileMap;

   TIBSQL* selectQuery = transaction.Query(transaction.AddQuery());
   selectQuery->SQL->Text = "SELECT PROFILE_KEY,NAME,RATE,TYPE,PRIORITY,SELECTABLE FROM TAXPROFILES WHERE SELECTABLE = 'T' ";
   selectQuery->ExecQuery();

   if(selectQuery->RecordCount > 0)
   {
      while(!selectQuery->Eof)
      {
         TaxProfile *taxProfile = new TaxProfile(selectQuery->FieldByName("PROFILE_KEY")->AsInteger);
         getTaxProfileFromQuery(selectQuery, taxProfile);

         TaxProfileKey key = taxProfile->GetUniqueProfileKeyPair();

         if(profileMap.find(key) == profileMap.end())
         {
            profileMap.insert(std::pair<TaxProfileKey, TaxProfile*>(key, taxProfile));
         }
         selectQuery->Next();
      }
   }

   return profileMap;
}

bool TTaxProfileDBAccessManager_MM::InsertTaxProfile(
                              Database::TDBTransaction &transaction,
                              TaxProfile taxProfile,
                              int &taxProfileKey)
{
   bool status = false;
   int nextTaxProfileKey = TGeneratorManager::GetNextGeneratorKey(transaction,"GEN_TAXPROFILES");
   TIBSQL* insertQuery = transaction.Query(transaction.AddQuery());
   insertQuery->SQL->Text = "INSERT INTO TAXPROFILES(PROFILE_KEY,NAME,RATE,TYPE,PRIORITY,SELECTABLE) VALUES(:PROFILE_KEY,:NAME,:RATE,:TYPE,:PRIORITY,:SELECTABLE)";

   insertQuery->ParamByName("PROFILE_KEY")->AsInteger = nextTaxProfileKey;
   insertQuery->ParamByName("NAME")->AsString = taxProfile.taxProfileName; //todo: substring to db field lengths
   insertQuery->ParamByName("RATE")->AsDouble = taxProfile.taxPercentage;
   insertQuery->ParamByName("TYPE")->AsInteger = taxProfile.taxProfileType;
   //insertQuery->ParamByName("TAX_CODE")->AsInteger = taxProfile.taxCode;
   insertQuery->ParamByName("PRIORITY")->AsInteger = taxProfile.taxPriority;
   insertQuery->ParamByName("SELECTABLE")->AsString = taxProfile.taxSelectable ? "T" : "F";

   insertQuery->ExecQuery();

   if(insertQuery->RowsAffected > 0)
   {
      taxProfileKey = nextTaxProfileKey;
      status = true;
   }

   return status;
}

bool TTaxProfileDBAccessManager_MM::UpdateTaxProfile(
                              Database::TDBTransaction &transaction,
                              TaxProfile taxProfile)
{
   bool status = false;

   if( taxProfile.GetTaxProfileStatus() == Deleted )
   {
      status = DisableTaxProfile( transaction, taxProfile.GetTaxProfileDBKey());
   }
   else
   {
      // check this tax profile against the db to check whether the rate and type has been changed
      // if either of them changed, then we need to update the references as needed

      bool needRefUpdate = false;
      TaxProfile dbProfile = LoadTaxProfileFromKey( transaction, taxProfile.GetTaxProfileDBKey());

      needRefUpdate = dbProfile.taxPercentage != taxProfile.taxPercentage
                        || dbProfile.taxProfileType != taxProfile.taxProfileType;

      if( needRefUpdate )
      {
         // set a new tax profile, disable the current one and update the refs for menu items
         int newTaxProfileKey = 0;
         status = InsertTaxProfile(transaction, taxProfile, newTaxProfileKey);

         status = status && UpdateTaxProfileReferences(
                                 transaction,
                                 taxProfile.GetTaxProfileDBKey(),
                                 newTaxProfileKey);
         if(status)
         {
            //we have successfully updated any references to this tax profile. we can safely disable it now
            status = DisableTaxProfile(transaction, taxProfile.GetTaxProfileDBKey());
         }
      }
      else
      {
         // we update only the name and priority of existing tax profiles
         updateTaxProfilePartial( transaction, &taxProfile );
      }
   }

   return status;
}

int TTaxProfileDBAccessManager_MM::GetTaxProfileKey(
                              Database::TDBTransaction &transaction,
                              AnsiString inName,
                              Currency inRate,
                              __int32 inType)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text =
        "SELECT "
        "   PROFILE_KEY "
        "FROM "
        "   TAXPROFILES "
        "WHERE "
        "   NAME = :NAME AND "
        "   RATE = :RATE AND "
        "   TYPE = :TYPE AND "
        "   SELECTABLE = :SELECTABLE";
    query->ParamByName("NAME")->AsString   = inName;
    query->ParamByName("RATE")->AsCurrency = inRate;
    query->ParamByName("TYPE")->AsInteger  = inType;
    query->ParamByName("SELECTABLE")->AsString = "T";

    query->ExecQuery();

    int taxProfileKey = 0;

    if( query->RecordCount > 0 )
    {
        taxProfileKey = query->FieldByName("PROFILE_KEY")->AsInteger;
    }

    return taxProfileKey;
}

bool TTaxProfileDBAccessManager_MM::CheckTaxProfileNameTypePair(
                              Database::TDBTransaction &transaction,
                              AnsiString inName,
                              __int32 inType)
{
    TIBSQL* checkQuery = transaction.Query(transaction.AddQuery());
    checkQuery->SQL->Text =
        "SELECT "
        "   PROFILE_KEY "
        "FROM "
        "   TAXPROFILES "
        "WHERE "
        "   NAME = :NAME AND "
        "   TYPE = :TYPE AND "
        "   SELECTABLE = :SELECTABLE";
    checkQuery->ParamByName("NAME")->AsString  = inName;
    checkQuery->ParamByName("TYPE")->AsInteger = inType;
    checkQuery->ParamByName("SELECTABLE")->AsString = "T";

    checkQuery->ExecQuery();

    return checkQuery->RecordCount > 0;
}

TaxProfile TTaxProfileDBAccessManager_MM::LoadTaxProfileFromKey(
                              Database::TDBTransaction &transaction,
                              int taxProfileDBKey)
{
    TIBSQL* selectQuery = transaction.Query(transaction.AddQuery());
    selectQuery->SQL->Text =
        "SELECT "
        "   NAME,"
        "   RATE,"
        "   TYPE,"
        //"   TAX_CODE,"
        "   PRIORITY,"
        "   SELECTABLE "
        "FROM "
        "   TAXPROFILES "
        "WHERE "
        "   PROFILE_KEY = :PROFILE_KEY ";
    selectQuery->ParamByName("PROFILE_KEY")->AsInteger = taxProfileDBKey;
    selectQuery->ExecQuery();

    TaxProfile taxProfile;

    if(selectQuery->RecordCount > 0)
    {
        taxProfile = TaxProfile(taxProfileDBKey);
        getTaxProfileFromQuery(selectQuery, &taxProfile);
    }

    return taxProfile;
}

int TTaxProfileDBAccessManager_MM::GetTaxProfileGeneratorValue(Database::TDBTransaction &transaction)
{
   int value = 0;

   TIBSQL* selectQuery = transaction.Query(transaction.AddQuery());
   selectQuery->SQL->Text = "SELECT GEN_ID(GEN_TAXPROFILES,0) FROM RDB$DATABASE";
   selectQuery->ExecQuery();

   value = selectQuery->Fields[0]->AsInteger;

   return value;
}

void TTaxProfileDBAccessManager_MM::SetTaxProfileGeneratorValue(Database::TDBTransaction &transaction, int val)
{
    TIBSQL* query = transaction.Query(transaction.AddQuery());
    query->SQL->Text = "SET GENERATOR GEN_TAXPROFILES TO " + IntToStr(val);
    query->ExecQuery();
}

bool TTaxProfileDBAccessManager_MM::DisableTaxProfile(
            Database::TDBTransaction &transaction,
            int taxProfileKey)
{
   TIBSQL* updateQuery = transaction.Query(transaction.AddQuery());
   updateQuery->SQL->Text = "UPDATE TAXPROFILES SET SELECTABLE=:SELECTABLE WHERE PROFILE_KEY=:PROFILE_KEY";
   updateQuery->ParamByName("PROFILE_KEY")->AsInteger = taxProfileKey;
   updateQuery->ParamByName("SELECTABLE")->AsString = "F";

   updateQuery->ExecQuery();
   return updateQuery->RowsAffected > 0;
}

bool TTaxProfileDBAccessManager_MM::UpdateTaxProfileReferences(
                                 Database::TDBTransaction &transaction,
                                 int oldTaxProfileKey,
                                 int newTaxProfileKey)
{
   TIBSQL* updateQuery = transaction.Query(transaction.AddQuery());
   updateQuery->SQL->Text = "UPDATE TAXPROFILES_ITEMSIZE SET PROFILE_KEY=:NEW_PROFILE_KEY WHERE PROFILE_KEY=:OLD_PROFILE_KEY";
   updateQuery->ParamByName("NEW_PROFILE_KEY")->AsInteger = newTaxProfileKey;
   updateQuery->ParamByName("OLD_PROFILE_KEY")->AsInteger = oldTaxProfileKey;

   updateQuery->ExecQuery();
   return true;
}

void TTaxProfileDBAccessManager_MM::getTaxProfileFromQuery(TIBSQL* query, TaxProfile* taxProfile)
{
   //this method will only assign properties other than the DB key. as it assumes the db key to be set already
   taxProfile->taxProfileName = query->FieldByName("NAME")->AsString;
   taxProfile->taxPercentage  = query->FieldByName("RATE")->AsDouble;
   taxProfile->taxProfileType = query->FieldByName("TYPE")->AsInteger;
   //taxProfile->taxCode        = query->FieldByName("TAX_CODE")->AsInteger;
   taxProfile->taxPriority    = query->FieldByName("PRIORITY")->AsInteger;
   taxProfile->taxSelectable  = query->FieldByName("SELECTABLE")->AsString == "T" ? true : false;
}

bool TTaxProfileDBAccessManager_MM::updateTaxProfilePartial( Database::TDBTransaction &transaction, TaxProfile* taxProfile)
{
   bool status = false;

   TIBSQL* updateQuery = transaction.Query(transaction.AddQuery());
   updateQuery->SQL->Text = "UPDATE TAXPROFILES SET NAME=:NAME, PRIORITY=:PRIORITY WHERE PROFILE_KEY=:PROFILE_KEY";
   updateQuery->ParamByName("PROFILE_KEY")->AsInteger = taxProfile->GetTaxProfileDBKey();
   updateQuery->ParamByName("NAME")->AsString = taxProfile->taxProfileName;
   updateQuery->ParamByName("PRIORITY")->AsInteger = taxProfile->taxPriority;
   //updateQuery->ParamByName("TAX_CODE")->AsInteger = taxProfile->taxCode;

   updateQuery->ExecQuery();

   status = updateQuery->RowsAffected > 0;
}

#pragma package(smart_init)

