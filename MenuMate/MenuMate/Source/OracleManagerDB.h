//---------------------------------------------------------------------------

#ifndef OracleManagerDBH
#define OracleManagerDBH
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
class TOracleManagerDB
{
    public:
        TOracleManagerDB();
        ~TOracleManagerDB();
        int GetSequenceNumber(Database::TDBTransaction &DBTransaction);
        int GetCheckNumber(Database::TDBTransaction &DBTransaction);
        AnsiString GetVersionNumber(Database::TDBTransaction &DBTransaction);
        TIBSQL* LoadMeals(Database::TDBTransaction &DBTransaction);
        TIBSQL* LoadRevenueCodes(Database::TDBTransaction &DBTransaction);
};
#endif
