//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBTax.h"
#include "MMLogging.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

std::vector<TTaxProfile> TDBTax::GetAllProfilesOfType(Database::TDBTransaction &DBTransaction, int type)
{
    std::vector<TTaxProfile> taxProfiles = std::vector<TTaxProfile>();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "SELECT "
            "   PROFILE_KEY,"
            "   NAME,"
            "   RATE "
            "FROM "
            "   TAXPROFILES "
            "WHERE "
            "   TYPE = :TYPE "
            "AND "
            "   SELECTABLE = :SELECTABLE;";
        IBInternalQuery->ParamByName("TYPE")->AsInteger = type;
        IBInternalQuery->ParamByName("SELECTABLE")->AsString = "T";
        IBInternalQuery->ExecQuery();

        for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
            TTaxProfile taxProfile;

            taxProfile.ProfileKey = IBInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
            taxProfile.Name       = IBInternalQuery->FieldByName("NAME")->AsString;
            taxProfile.Rate       = IBInternalQuery->FieldByName("RATE")->AsCurrency;
            taxProfile.Type       = type;

            taxProfiles.push_back(taxProfile);
        }
    }
    catch(Exception &Err)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
        throw;
    }

    return taxProfiles;
}



