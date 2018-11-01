//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBAdyen.h"
#include "MMLogging.h"
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
UnicodeString TDBAdyen::GetInvoiceNumber(int arcBillKey)
{
    UnicodeString InvoiceNumber = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.INVOICE_NUMBER FROM DAYARCBILL a WHERE a.ARCBILL_KEY = :ARCBILL_KEY";
        IBInternalQuery->Close();
        IBInternalQuery->ParamByName("ARCBILL_KEY")->AsInteger = arcBillKey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           InvoiceNumber = IBInternalQuery->FieldByName("INVOICE_NUMBER")->AsString;
        }
        DBTransaction.Commit();
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
    return InvoiceNumber;
}
//---------------------------------------------------------------------------
UnicodeString TDBAdyen::GetMerchantAccount(UnicodeString invoiceNumber)
{
    UnicodeString MerchantAccount = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.MERCHANT_ID FROM EFTPOSREFRENECE a  WHERE a.INVOICE_NO = :INVOICE_NO";
        IBInternalQuery->Close();
        IBInternalQuery->ParamByName("INVOICE_NO")->AsString = invoiceNumber;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           MerchantAccount = IBInternalQuery->FieldByName("MERCHANT_ID")->AsString;
        }
        DBTransaction.Commit();
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
	}
    return MerchantAccount;
}
//-----------------------------------------------------------------
void TDBAdyen::UpdateEFTPOSSettleField(Database::TDBTransaction &DBTransaction, UnicodeString invoiceNumber)
{
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "UPDATE EFTPOSREFRENECE a SET a.IS_SETTLED = :IS_SETTLED WHERE a.INVOICE_NO = :INVOICE_NO ";
        IBInternalQuery->Close();
        IBInternalQuery->ParamByName("IS_SETTLED")->AsString = "T";
        IBInternalQuery->ParamByName("INVOICE_NO")->AsString = invoiceNumber;
        IBInternalQuery->ExecQuery();
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
        throw;
	}
}
//---------------------------------------------------------------------------
bool TDBAdyen::IsTipFromReceiptAlreadyAdded(int arcBillKey)
{
    bool isTipAlreadyAdded =  false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        UnicodeString invoiceNumber = GetInvoiceNumber(arcBillKey);
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.IS_SETTLED FROM EFTPOSREFRENECE a WHERE a.INVOICE_NO = :INVOICE_NO AND a.IS_SETTLED = :IS_SETTLED ";
        IBInternalQuery->Close();
        IBInternalQuery->ParamByName("INVOICE_NO")->AsString = invoiceNumber;
        IBInternalQuery->ParamByName("IS_SETTLED")->AsString = "T";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            isTipAlreadyAdded = true;

        DBTransaction.Commit();
    }
    catch(Exception & E)
	{
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return isTipAlreadyAdded;
}
