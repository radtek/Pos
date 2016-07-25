//---------------------------------------------------------------------------


#pragma hdrstop

#include "ProcessSecurityQueries.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TProcessSecurityQueries::TProcessSecurityQueries(Database::TDBTransaction &DBTransaction) : fDBTransaction
	(DBTransaction), fGenerator(new TIBSQL(NULL)), DuplicateCheck(new TIBSQL(NULL)), Insertor(new TIBSQL(NULL))
{
   fGenerator->SQL->Text = "SELECT GEN_ID(GEN_SECURITY_KEY, 1) FROM RDB$DATABASE";
   DBTransaction.RegisterQuery(fGenerator);

   Insertor->SQL->Text =
	   "INSERT INTO SECURITY (" "SECURITY_KEY," "SECURITY_REF," "SECURITY_EVENT," "FROM_VAL," "TO_VAL," "NOTE," "TERMINAL_NAME,"
	   "USER_KEY," "TIME_STAMP)" "VALUES (" ":SECURITY_KEY," ":SECURITY_REF," ":SECURITY_EVENT," ":FROM_VAL," ":TO_VAL," ":NOTE,"
	   ":TERMINAL_NAME," ":USER_KEY," ":TIME_STAMP)";
   DBTransaction.RegisterQuery(Insertor);

   DuplicateCheck->SQL->Text = "SELECT " "SECURITY_REF " "FROM " "SECURITY " "WHERE " "SECURITY_REF = :SECURITY_REF "
	   "AND SECURITY_EVENT = :SECURITY_EVENT";
   DBTransaction.RegisterQuery(DuplicateCheck);
}

int TProcessSecurityQueries::GetKey()
{
   fGenerator->Close();
   fGenerator->ExecQuery();
   return fGenerator->Fields[0]->AsInteger;
}
