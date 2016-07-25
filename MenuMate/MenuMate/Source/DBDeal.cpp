//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBDeal.h"
#include "IStoredDeal.h"
#include "IStoredDealLevel.h"
#include "Deal.h"
#include "DealLevel.h"

DBDeal::DBDeal()
    :DBControl(TDeviceRealTerminal::Instance().DBControl)
{
}

//---------------------------------------------------------------------------
DBDeal::~DBDeal()
{
}

//---------------------------------------------------------------------------
bool DBDeal::Save(IStoredDeal *inItem)
{
    bool result = false;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    try
    {
		result = Save(DBTransaction, inItem);
    }
    catch(Exception &err)
	{
		logError(__FUNC__, ERRORLOG, err.Message);
    }

    DBTransaction.Commit();

    return result;
}
//---------------------------------------------------------------------------

bool DBDeal::Save(Database::TDBTransaction &DBTransaction, IStoredDeal *inItem)
{
	inItem->SetDealID(generateDealKey(DBTransaction));

    TIBSQL *internalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
    internalQuery->SQL->Text = getDealInsertQuery();

    internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();
    internalQuery->ParamByName("CATEGORY_KEY")->AsInteger = inItem->GetCategoryKey();
    internalQuery->ParamByName("DEAL_NAME")->AsString = inItem->GetName();
    internalQuery->ParamByName("PRIORITY")->AsInteger = inItem->GetPriority();

	internalQuery->ExecQuery();
	if(internalQuery->RowsAffected == 0)
	{
		throw new Exception("Failed to insert a deal into database");
	}

	StoredDealLevelSet dealLevels = inItem->GetStoredDealLevels();

	for(StoredDealLevelSet::iterator it = dealLevels.begin(); it != dealLevels.end(); it++)
	{
		IStoredDealLevel* dealLevel = *it;
		dealLevel->SetDealLevelID(generateDealLevelKey(DBTransaction));

		internalQuery->SQL->Text = getDealLevelInsertQuery();

		internalQuery->ParamByName("DEALLEVEL_KEY")->AsInteger = dealLevel->GetDealLevelID();
		internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();
		internalQuery->ParamByName("QTY")->AsCurrency = dealLevel->GetQty();
		internalQuery->ParamByName("PRICE")->AsCurrency = dealLevel->GetPrice();

		internalQuery->ExecQuery();
	}

    return true;
}
//---------------------------------------------------------------------------

bool DBDeal::Update(IStoredDeal *inItem)
{
    bool result = false;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    try
    {
        result = Update(
                    DBTransaction,
                    inItem);
    }
    catch(Exception &err)
    {
        logError(
                __FUNC__,
                ERRORLOG,
                err.Message);
    }

    DBTransaction.Commit();

    return result;
}
//---------------------------------------------------------------------------

bool DBDeal::Update(Database::TDBTransaction &DBTransaction, IStoredDeal *inItem)
{
    TIBSQL *internalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
	internalQuery->SQL->Text = getDealUpdateQuery();

	internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();
    internalQuery->ParamByName("CATEGORY_KEY")->AsInteger = inItem->GetCategoryKey();
	internalQuery->ParamByName("DEAL_NAME")->AsString = inItem->GetName();
	internalQuery->ParamByName("PRIORITY")->AsInteger = inItem->GetPriority();

	internalQuery->ExecQuery();
	if(internalQuery->RowsAffected == 0)
	{
		throw new Exception("Failed to update a deal in database");
	}

	internalQuery->SQL->Text = getDealLevelDeleteQuery();

	internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();

	internalQuery->ExecQuery();

	StoredDealLevelSet dealLevels = inItem->GetStoredDealLevels();

	for(StoredDealLevelSet::iterator it = dealLevels.begin(); it != dealLevels.end(); it++)
	{
		IStoredDealLevel* dealLevel = *it;
		if (dealLevel->GetDealLevelID() == 0)
		{
			dealLevel->SetDealLevelID(generateDealLevelKey(DBTransaction));
		}

		internalQuery->SQL->Text = getDealLevelInsertQuery();

		internalQuery->ParamByName("DEALLEVEL_KEY")->AsInteger = dealLevel->GetDealLevelID();
		internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();
		internalQuery->ParamByName("QTY")->AsCurrency = dealLevel->GetQty();
		internalQuery->ParamByName("PRICE")->AsCurrency = dealLevel->GetPrice();

		internalQuery->ExecQuery();
	}

    return true;
}
//---------------------------------------------------------------------------

bool DBDeal::Delete(IStoredDeal *inItem)
{
    bool result = false;

    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    try
    {
        result = Delete(
                    DBTransaction,
                    inItem);
    }
    catch(Exception &err)
    {
        logError(
                __FUNC__,
                ERRORLOG,
                err.Message);
    }

    DBTransaction.Commit();
    return result;
}
//---------------------------------------------------------------------------

bool DBDeal::Delete(Database::TDBTransaction &DBTransaction, IStoredDeal *inItem)
{
    TIBSQL *internalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
    internalQuery->SQL->Text = getDealDeleteQuery();

    internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();
    internalQuery->ExecQuery();

	if(internalQuery->RowsAffected == 0)
	{
		throw new Exception("Failed to remove a deal from database");
	}

	internalQuery->SQL->Text = getDealLevelDeleteQuery();

	internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItem->GetDealID();
	internalQuery->ExecQuery();

    return true;
}
//---------------------------------------------------------------------------

void DBDeal::Load(int inItemID, IStoredDeal *outDeal)
{
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    try
    {
        TIBSQL *internalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
        internalQuery->SQL->Text = getDealSelectSingleQuery();

        internalQuery->ParamByName("DEAL_KEY")->AsInteger = inItemID;
        internalQuery->ExecQuery();

        if(internalQuery->RecordCount == 0)
            throw new Exception("Deal not found in the database");

        loadSingleDealFromQuery( internalQuery, outDeal );
    }
    catch(Exception &err)
    {
        logError(
                __FUNC__,
                ERRORLOG,
                err.Message);
    }

    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void DBDeal::LoadAll(StoredDealVector &allDeals)
{
	Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    try
    {
		TIBSQL *internalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
        internalQuery->SQL->Text = getDealSelectAllQuery();

		internalQuery->ExecQuery();

		while(!internalQuery->Eof)
		{
			int dealKey 		   = internalQuery->FieldByName("DEAL_KEY"	  )->AsInteger;
			UnicodeString dealName = internalQuery->FieldByName("DEAL_NAME"	  )->AsString;
			int categoryKey 	   = internalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
			int priority 		   = internalQuery->FieldByName("PRIORITY"	  )->AsInteger;

			IStoredDeal *deal = new TDeal(dealName, priority, categoryKey, dealKey);

			deal->SetStoredDealLevels(loadDealLevelsForDeal(DBTransaction, dealKey));

			allDeals.push_back(deal);
			internalQuery->Next();
		}
    }
    catch(Exception &err)
	{
		logError(__FUNC__, ERRORLOG, err.Message);
    }

    DBTransaction.Commit();

}
//---------------------------------------------------------------------------

StoredDealLevelSet DBDeal::loadDealLevelsForDeal(Database::TDBTransaction &DBTransaction, int inDealKey)
{
	TIBSQL *internalQuery = DBTransaction.Query( DBTransaction.AddQuery() );
	internalQuery->SQL->Text = getDealLevelSelectOnDealKeyQuery();

	internalQuery->ParamByName("DEAL_KEY")->AsInteger = inDealKey;
	internalQuery->ExecQuery();

    StoredDealLevelSet dealLevels;

	while(!internalQuery->Eof)
	{
		int dealLevelKey = internalQuery->FieldByName("DEALLEVEL_KEY")->AsInteger;
		Currency qty 	 = internalQuery->FieldByName("QTY"			 )->AsCurrency;
		Currency price 	 = internalQuery->FieldByName("PRICE"	  	 )->AsCurrency;

		IStoredDealLevel* dealLevel = new TDealLevel(qty, price, dealLevelKey);
		dealLevels.insert(dealLevel);
		internalQuery->Next();
	}

	return dealLevels;
}
//---------------------------------------------------------------------------

void DBDeal::logError(UnicodeString Function, UnicodeString Type, UnicodeString Msg, UnicodeString Terminal)
{
	TManagerLogs::Instance().Add(Function, Type, Msg);
}
//---------------------------------------------------------------------------

int DBDeal::generateDealKey(Database::TDBTransaction &DBTransaction)
{
	TIBSQL *internalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	internalQuery->SQL->Text = getNewDealKeyQuery();
	internalQuery->ExecQuery();

	return internalQuery->Fields[0]->AsInteger;
}
//---------------------------------------------------------------------------

int DBDeal::generateDealLevelKey(Database::TDBTransaction &DBTransaction)
{
	TIBSQL *internalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	internalQuery->SQL->Text = getNewDealLevelKeyQuery();
	internalQuery->ExecQuery();

	return internalQuery->Fields[0]->AsInteger;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getNewDealKeyQuery()
{
	UnicodeString query = "SELECT GEN_ID(GEN_DEAL, 1) FROM RDB$DATABASE";
    return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealInsertQuery()
{
    UnicodeString query =
        "INSERT INTO DEAL"
            "("
                "DEAL_KEY,"
			    "CATEGORY_KEY,"
			    "DEAL_NAME,"
			    "PRIORITY"
            ")"
            "VALUES"
            "("
                ":DEAL_KEY,"
			    ":CATEGORY_KEY,"
			    ":DEAL_NAME,"
			    ":PRIORITY"
            ")";
    return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealUpdateQuery()
{
    UnicodeString query =
		"UPDATE DEAL "
			"SET "
				"CATEGORY_KEY = :CATEGORY_KEY,"
				"DEAL_NAME = :DEAL_NAME,"
				"PRIORITY = :PRIORITY "
            "WHERE "
                "DEAL_KEY = :DEAL_KEY";
    return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealDeleteQuery()
{
    UnicodeString query =
		"DELETE FROM DEAL "
			"WHERE "
				"DEAL_KEY = :DEAL_KEY";
    return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealSelectSingleQuery()
{
    UnicodeString query =
        "SELECT "
            " DEAL_KEY,"
            " CATEGORY_KEY, "
            " DEAL_NAME, "
            " PRIORITY "
        " FROM DEAL"
        " WHERE"
            " DEAL_KEY = :DEAL_KEY";
    return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealSelectAllQuery()
{
    UnicodeString query =
        "SELECT "
            " DEAL_KEY,"
            " CATEGORY_KEY, "
            " DEAL_NAME, "
            " PRIORITY "
        " FROM DEAL";
    return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getNewDealLevelKeyQuery()
{
	UnicodeString query = "SELECT GEN_ID(GEN_DEALLEVEL, 1) FROM RDB$DATABASE";
	return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealLevelInsertQuery()
{
	UnicodeString query =
		"INSERT INTO DEALLEVEL"
            "("
				"DEALLEVEL_KEY,"
				"DEAL_KEY,"
				"QTY,"
				"PRICE"
			") "
			"VALUES "
            "("
				":DEALLEVEL_KEY,"
				":DEAL_KEY,"
				":QTY,"
			    ":PRICE"
            ")";
	return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealLevelUpdateQuery()
{
	UnicodeString query =
		"UPDATE DEAL "
			"SET "
				"DEAL_KEY = :DEAL_KEY,"
				"QTY      = :QTY,"
				"PRICE    = :PRICE "
			"WHERE "
                "DEALLEVEL_KEY = :DEALLEVEL_KEY";
	return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealLevelDeleteQuery()
{
	UnicodeString query =
		"DELETE FROM DEALLEVEL "
            "WHERE "
                "DEAL_KEY = :DEAL_KEY";
	return query;
}
//---------------------------------------------------------------------------

UnicodeString DBDeal::getDealLevelSelectOnDealKeyQuery()
{
    UnicodeString query =
        "SELECT "
            "DEALLEVEL_KEY,"
			"DEAL_KEY,"
			"QTY,"
			"PRICE "
		"FROM "
			"DEALLEVEL "
		"WHERE "
			"DEAL_KEY = :DEAL_KEY";
    return query;
}
//---------------------------------------------------------------------------

void DBDeal::loadSingleDealFromQuery( TIBSQL* internalQuery, IStoredDeal *outDeal)
{

}
//---------------------------------------------------------------------------

TList DBDeal::populateDealsCollectionFromQuery( TIBSQL* internalQuery, TList *allDeals)
{
    allDeals->Clear();


}
//---------------------------------------------------------------------------

#pragma package(smart_init)
