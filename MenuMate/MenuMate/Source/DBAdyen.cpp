//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBAdyen.h"
#include "MMLogging.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
TDBAdyen::TDBAdyen()
{
}
//---------------------------------------------------------------------------
TDBAdyen::~TDBAdyen()
{
}
//---------------------------------------------------------------------------
UnicodeString TDBAdyen::GetServiceID()
{
    UnicodeString retValue = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *GeneratorQuery = DBTransaction.Query(DBTransaction.AddQuery());
        GeneratorQuery->Close();
        GeneratorQuery->SQL->Clear();
        GeneratorQuery->SQL->Text ="SELECT GEN_ID(GEN_ADYENSERVICEID, 1) FROM RDB$DATABASE ";

        GeneratorQuery->ExecQuery();
        int val = GeneratorQuery->Fields[0]->AsInteger;
        retValue = IntToStr(val);
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
    }
    return retValue;
}
//---------------------------------------------------------------------------
UnicodeString TDBAdyen::GetTransactionID()
{
    UnicodeString retValue = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *GeneratorQuery = DBTransaction.Query(DBTransaction.AddQuery());
        GeneratorQuery->Close();
        GeneratorQuery->SQL->Clear();
        GeneratorQuery->SQL->Text ="SELECT GEN_ID(GEN_ADYENTRANSACTIONID, 1) FROM RDB$DATABASE ";

        GeneratorQuery->ExecQuery();
        int val = GeneratorQuery->Fields[0]->AsInteger;
        retValue = IntToStr(val);
        DBTransaction.Commit();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
    }
    return retValue;
}
//---------------------------------------------------------------------------
