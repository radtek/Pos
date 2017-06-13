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

};
#endif
