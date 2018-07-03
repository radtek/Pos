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
            {
                if(IBInternalQuery->FieldByName("REVENUECODE")->AsInteger != NULL)
                    ic->RevenueCode = IBInternalQuery->FieldByName("REVENUECODE")->AsInteger;
                else
                    ic->RevenueCode = 0;
            }
            else
            {
                ic->RevenueCode = 0;
            }
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

