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
};
#endif
