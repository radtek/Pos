//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleManagerDB.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
TOracleManagerDB::TOracleManagerDB()
{
}
//---------------------------------------------------------------------------
TOracleManagerDB::~TOracleManagerDB()
{
}
//---------------------------------------------------------------------------
int TOracleManagerDB::GetSequenceNumber(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *GeneratorQuery= DBTransaction.Query(DBTransaction.AddQuery());
    GeneratorQuery->Close();
    GeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_ORACLESEQNUMBER, 1) FROM RDB$DATABASE";
    GeneratorQuery->ExecQuery();
    return GeneratorQuery->Fields[0]->AsInteger;
}
//---------------------------------------------------------------------------
int TOracleManagerDB::GetCheckNumber(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *GeneratorQuery= DBTransaction.Query(DBTransaction.AddQuery());
    GeneratorQuery->Close();
    GeneratorQuery->SQL->Text = "SELECT GEN_ID(GEN_ORACLECHECKNUMBER, 1) FROM RDB$DATABASE";
    GeneratorQuery->ExecQuery();
    return GeneratorQuery->Fields[0]->AsInteger;
}
//---------------------------------------------------------------------------
