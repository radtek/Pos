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
UnicodeString TDBAdyen::GetCompanyName()
{
    UnicodeString CompanyName = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.COMPANY FROM REGISTRATION a where a.TERMINAL_NAME = :TERMINAL_NAME";
        IBInternalQuery->Close();
        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
        IBInternalQuery->ExecQuery();
        if(!IBInternalQuery->Eof)
         {
           CompanyName = IBInternalQuery->FieldByName("COMPANY")->AsString;
         }
         DBTransaction.Commit();
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
    return CompanyName;
}
//---------------------------------------------------------------------------
void TDBAdyen::SetRefundTransaction(bool isRefund)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TGlobalSettings::Instance().AdyenRecoveryTransactionIsRefund = isRefund;
        TManagerVariable::Instance().SetDeviceBool(DBTransaction,vmAdyenRecoveryTransactionIsRefund,TGlobalSettings::Instance().AdyenRecoveryTransactionIsRefund);
        DBTransaction.Commit();
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
}
//---------------------------------------------------------------------------
