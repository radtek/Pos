//---------------------------------------------------------------------------

#ifndef ProcessSecurityQueriesH
#define ProcessSecurityQueriesH

#include "MM_DBCore.h"
//---------------------------------------------------------------------------
class TProcessSecurityQueries
{
public:
   TProcessSecurityQueries(Database::TDBTransaction &DBTransaction);
   int GetKey();
   Database::TcpIBSQL DuplicateCheck;
   Database::TcpIBSQL Insertor;
private:
   Database::TDBTransaction &fDBTransaction;
   Database::TcpIBSQL fGenerator;
};
#endif
