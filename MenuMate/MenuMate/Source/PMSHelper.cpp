//---------------------------------------------------------------------------


#pragma hdrstop

#include "PMSHelper.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
bool TPMSHelper::LoadRevenueCodes(std::map<int,TRevenueCodeDetails> &RevenueCodesMap, Database::TDBTransaction &DBTransaction)
{
    bool retValue = false;
    RevenueCodesMap.clear();
  
    TIBSQL* queryRevenue = GetRevenueCodesQuery(DBTransaction);
    for(;!queryRevenue->Eof;queryRevenue->Next())
    {
        TRevenueCodeDetails codeDetails;
        codeDetails.IsDefault = queryRevenue->FieldByName("ISDEFAULT_REVENUECODE")->AsString == "T" ?
                                true : false;
        codeDetails.RevenueCodeDescription = queryRevenue->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
        RevenueCodesMap[queryRevenue->FieldByName("REVENUECODE")->AsInteger] =
            codeDetails;
        retValue = true;
    }
    return retValue;
}
//---------------------------------------------------------------------------
TIBSQL* TPMSHelper::GetRevenueCodesQuery(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *SelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->Close();
    SelectQuery->SQL->Text = "SELECT * FROM REVENUECODEDETAILS";
    SelectQuery->ExecQuery();
    return SelectQuery;
}
//----------------------------------------------------------------------------
void TPMSHelper::GetRevenueCode(TList *Orders)//(int _itemKey, UnicodeString _sizeName)
{
    int revenueCode = 0;
    Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
    transaction.StartTransaction();
    try
    {
		TIBSQL *IBInternalQuery = transaction.Query(transaction.AddQuery());
        for(int i = 0; i < Orders->Count; i++)
        {
            TItemComplete *ic = (TItemComplete*)Orders->Items[i];
            IBInternalQuery->Close();
            IBInternalQuery->SQL->Text =
                " Select a.REVENUECODE FROM ITEMSIZE a WHERE a.ITEM_KEY = :ITEM_KEY AND a.SIZE_NAME = :SIZE_NAME" ;
            IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = ic->ItemKey;
            IBInternalQuery->ParamByName("SIZE_NAME")->AsString = ic->Size;
            IBInternalQuery->ExecQuery();
            if(IBInternalQuery->RecordCount)
                ic->RevenueCode = IBInternalQuery->FieldByName("REVENUECODE")->AsInteger;
            else
                ic->RevenueCode = 0;
        }
        transaction.Commit();
    }
    catch(Exception &ex)
    {
        transaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }
}
//----------------------------------------------------------------------------
void TPMSHelper::LoadPMSPaymentTypes(std::map<int, TPMSPaymentType> &PMSPaymentTypesMapContainer)
{
    //InitializePMSDefaultPayment();
    Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
    transaction.StartTransaction();
    try
    {
        std::auto_ptr<TManagerPMSCodes> pmsCodes(new TManagerPMSCodes());
        pmsCodes->GetPMSPaymentTypeFromDB(transaction,PMSPaymentTypesMapContainer);
        transaction.Commit();
    }
    catch(Exception &ex)
    {
        transaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ex.Message);
    }
}
//---------------------------------------------------------------------------
void TPMSHelper::InitializePMSDefaultPayment()
{
    if(DefaultPaymentInitRequired())
    {
        InitDefaultPaymentInDB();
    }
}

//---------------------------------------------------------------------------
bool TPMSHelper::DefaultPaymentInitRequired()
{
    bool retValue = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text =
                   "SELECT * FROM PMSPAYMENTSCONFIG WHERE PMS_PAYTYPE_NAME like '%Default Payment Category%'";
        SelectQuery->ExecQuery();
        UnicodeString value = SelectQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString;
        if(value.Trim() == "")
        {
            retValue = true;
        }
        else
        {
            retValue = false;
        }
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
    }
    return retValue;
}

//---------------------------------------------------------------------------
void TPMSHelper::InitDefaultPaymentInDB()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->Close();
        UpdateQuery->SQL->Text =
                   "UPDATE PMSPAYMENTSCONFIG SET PMS_PAYTYPE_CODE = :PMS_PAYTYPE_CODE "
                   "WHERE PMS_PAYTYPE_NAME like '%Default Payment Category%'";

        UpdateQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString = TDeviceRealTerminal::Instance().BasePMS->DefaultPaymentCategory;
        UpdateQuery->ExecQuery();
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction.Rollback();
    }
}

//---------------------------------------------------------------------------
