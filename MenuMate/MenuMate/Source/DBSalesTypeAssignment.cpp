//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBSalesTypeAssignment.h"

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
        query->SQL->Text =  "SELECT MST.SALES_TYPE_ID, MST.SALES_TYPE_NAME FROM MALL_SALES_TYPE MST ORDER BY 1 ASC ";
        query->ExecQuery();

        while(!query->Eof)
        {
            salesTypeMap.insert( std::pair<int, UnicodeString >(query->FieldByName("SALES_TYPE_ID")->AsInteger, query->FieldByName("SALES_TYPE_NAME")->AsString ));
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
//------------------------------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::SaveSalesType(UnicodeString name, UnicodeString code)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        //Increment Generator for inserting date into db since it is primary key.
        TIBSQL* incrementGenerator = dbTransaction.Query(dbTransaction.AddQuery());
        incrementGenerator->Close();
        incrementGenerator->SQL->Text = "SELECT GEN_ID(GEN_MALLSALES_TYPE, 1) FROM RDB$DATABASE";
        incrementGenerator->ExecQuery();

        //insert new sales type into Db..
        TIBSQL* insertQuery = dbTransaction.Query(dbTransaction.AddQuery());
        insertQuery->Close();
        insertQuery->SQL->Text =  "INSERT INTO MALL_SALES_TYPE  VALUES(:SALES_TYPE_ID, :SALES_TYPE_CODE, :SALES_TYPE_NAME) ";

        insertQuery->ParamByName("SALES_TYPE_ID")->AsInteger = incrementGenerator->Fields[0]->AsInteger;
        insertQuery->ParamByName("SALES_TYPE_CODE")->AsString = code;
        insertQuery->ParamByName("SALES_TYPE_NAME")->AsString = name;
        insertQuery->ExecQuery();

        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        dbTransaction.Rollback();
	}
}
//-------------------------------------------------------------------------------------------------------------
bool TDBSalesTypeAssignment::IsSalesTypeCodeExist(UnicodeString code)
{
    bool isPresent = false;
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TIBSQL* selectQuery = dbTransaction.Query(dbTransaction.AddQuery());
        selectQuery->Close();
        selectQuery->SQL->Text =  "SELECT A.SALES_TYPE_ID FROM MALL_SALES_TYPE A WHERE A.SALES_TYPE_CODE = :SALES_TYPE_CODE ";

        selectQuery->ParamByName("SALES_TYPE_CODE")->AsString = code;
        selectQuery->ExecQuery();

        if(selectQuery->RecordCount)
            isPresent = true;

        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        dbTransaction.Rollback();
	}
    return isPresent;
}
//----------------------------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::DeleteSalesType(int id)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TIBSQL* selectQuery = dbTransaction.Query(dbTransaction.AddQuery());
        selectQuery->Close();
        selectQuery->SQL->Text =  "DELETE FROM MALL_SALES_TYPE a WHERE A.SALES_TYPE_ID = :SALES_TYPE_ID ";

        selectQuery->ParamByName("SALES_TYPE_ID")->AsInteger = id;
        selectQuery->ExecQuery();

        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        dbTransaction.Rollback();
	}
}
//----------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::LoadSelectedSalesType(int id, UnicodeString &code, UnicodeString &name)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TIBSQL* selectQuery = dbTransaction.Query(dbTransaction.AddQuery());
        selectQuery->Close();
        selectQuery->SQL->Text =  "SELECT MST.SALES_TYPE_CODE, MST.SALES_TYPE_NAME FROM MALL_SALES_TYPE MST WHERE MST.SALES_TYPE_ID = :SALES_TYPE_ID ";

        selectQuery->ParamByName("SALES_TYPE_ID")->AsInteger = id;
        selectQuery->ExecQuery();

        if(selectQuery->RecordCount)
        {
            code = selectQuery->FieldByName("SALES_TYPE_CODE")->AsString;
            name = selectQuery->FieldByName("SALES_TYPE_NAME")->AsString;
        }

        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//------------------------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::UpdateSalesType(int id, UnicodeString name, UnicodeString code)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TIBSQL* selectQuery = dbTransaction.Query(dbTransaction.AddQuery());
        selectQuery->Close();
        selectQuery->SQL->Text =  "UPDATE MALL_SALES_TYPE MST  SET SALES_TYPE_NAME = :SALES_TYPE_NAME, SALES_TYPE_CODE = :SALES_TYPE_CODE  "
                                    "WHERE MST.SALES_TYPE_ID = :SALES_TYPE_ID ";

        selectQuery->ParamByName("SALES_TYPE_ID")->AsInteger = id;
        selectQuery->ParamByName("SALES_TYPE_NAME")->AsString = name;
        selectQuery->ParamByName("SALES_TYPE_CODE")->AsString = code;
        selectQuery->ExecQuery();

        
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        dbTransaction.Rollback();
	}
}
//----------------------------------------------------------------------------------------------------
std::map<int, std::map<int, UnicodeString> > TDBSalesTypeAssignment::LoadAssignedItemsBySalesType()
{
    std::map<int, std::map<int, UnicodeString> > getAssignedsalesTypeItemsMap;
    try
    {
        //Register the database transaction..
        Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
        dbTransaction.StartTransaction();

        TIBSQL* query = dbTransaction.Query(dbTransaction.AddQuery());
        query->SQL->Text =  " SELECT a.ITEM_ID, a.SALES_TYPE_ID, b.ITEM_NAME FROM MALL_SALES_TYPE_ITEMS_RELATION a INNER JOIN ITEM b "
                             "ON a.ITEM_ID = b.ITEM_ID ORDER BY 2 ASC ";
        query->ExecQuery();

        while(!query->Eof)
        {
            getAssignedsalesTypeItemsMap[query->FieldByName("SALES_TYPE_ID")->AsInteger].insert(std::make_pair(query->FieldByName("ITEM_ID")->AsInteger, query->FieldByName("ITEM_NAME")->AsString));
            query->Next();
        }
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return getAssignedsalesTypeItemsMap;
}
//----------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::SaveItemRelationWithSalesType(std::map<int, std::map<int, TItemDetails> > modifieldItemsWithSalesType)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        std::map<int, std::map<int, TItemDetails> >::iterator outerit;
        std::map <int, TItemDetails>::iterator innerit;

        //outer loop is for sales id and inner is for item id
        for (outerit = modifieldItemsWithSalesType.begin(); outerit != modifieldItemsWithSalesType.end(); ++outerit)
        {
            for (innerit = outerit->second.begin(); innerit != outerit->second.end(); ++innerit)
            {
                //checking TITemDetails 's enum property..
                if(innerit->second.ItemStatus == eAssigned)
                {
                    InsertRecordInToDB(dbTransaction, outerit->first, innerit->first);
                }
                else if(innerit->second.ItemStatus == eRemoved)
                {
                    DeleteRecordFromDB(dbTransaction, outerit->first, innerit->first);
                }
            }
        }
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        dbTransaction.Rollback();
	}
}
//-----------------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::InsertRecordInToDB(Database::TDBTransaction &dbTransaction, int saleTypeId, int itemId)
{
    try
    {
        TIBSQL* incrementGenerator = dbTransaction.Query(dbTransaction.AddQuery());
        incrementGenerator->Close();
        incrementGenerator->SQL->Text = "SELECT GEN_ID(GEN_MALLSALES_TYPE_ITEMS_REL, 1) FROM RDB$DATABASE";
        incrementGenerator->ExecQuery();

        TIBSQL* insertQuery = dbTransaction.Query(dbTransaction.AddQuery());
        insertQuery->Close();
        insertQuery->SQL->Text =  "INSERT INTO MALL_SALES_TYPE_ITEMS_RELATION  VALUES (:STI_ID, :ITEM_ID, :SALES_TYPE_ID) ";

        insertQuery->ParamByName("STI_ID")->AsInteger = incrementGenerator->Fields[0]->AsInteger;
        insertQuery->ParamByName("ITEM_ID")->AsInteger = itemId;
        insertQuery->ParamByName("SALES_TYPE_ID")->AsInteger = saleTypeId;
        insertQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}
}
//---------------------------------------------------------------------------------------------------------
void TDBSalesTypeAssignment::DeleteRecordFromDB(Database::TDBTransaction &dbTransaction, int saleTypeId, int itemId)
{
    try
    {
        TIBSQL* deleteQuery = dbTransaction.Query(dbTransaction.AddQuery());
        deleteQuery->Close();
        deleteQuery->SQL->Text =  "DELETE FROM MALL_SALES_TYPE_ITEMS_RELATION a WHERE a.ITEM_ID = :ITEM_ID  and a.SALES_TYPE_ID = :SALES_TYPE_ID ";

        deleteQuery->ParamByName("ITEM_ID")->AsInteger = itemId;
        deleteQuery->ParamByName("SALES_TYPE_ID")->AsInteger = saleTypeId;
        deleteQuery->ExecQuery();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
	}
}

