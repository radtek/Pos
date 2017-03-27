//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBSalesTypeAssignment.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

std::map<int, UnicodeString> TDBSalesTypeAssignment::LoadAllItems()
{
    std::map<int, UnicodeString> itemMap;
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
        query->SQL->Text = "SELECT ITEM_KEY, ITEM_NAME FROM ITEM";
        query->ExecQuery();

        while(!query->Eof)
        {
            itemMap.insert( std::pair<int, UnicodeString >(query->FieldByName("ITEM_KEY")->AsInteger, query->FieldByName("ITEM_NAME")->AsString ));
            query->Next();
        }
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

    return itemMap;
}
//------------------------------------------------------------------------------------------------------------------------
std::map<int, UnicodeString> TDBSalesTypeAssignment::LoadAllSalesTypes()
{
    std::map<int, UnicodeString> salesTypeMap;
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
        query->SQL->Text =  "SELECT MST.SALES_TYPE_KEY, MST.SALES_TYPE_NAME, MST.DEFAULT_SALES_TYPE "
                            "FROM MALL_SALES_TYPE MST ";
        query->ExecQuery();

        while(!query->Eof)
        {
            salesTypeMap.insert( std::pair<int, UnicodeString >(query->FieldByName("SALES_TYPE_KEY")->AsInteger, query->FieldByName("SALES_TYPE_NAME")->AsString ));
            query->Next();
        }
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

    return salesTypeMap;
}
