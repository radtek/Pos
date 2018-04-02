//---------------------------------------------------------------------------


#pragma hdrstop

#include "OracleManagerDB.h"
#include "DeviceRealTerminal.h"

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
AnsiString TOracleManagerDB::GetVersionNumber(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->Close();
    SelectQuery->SQL->Text = "SELECT VERSION_NUMBER FROM VERSIONHISTORY ORDER BY VERSION_NUMBER DESC";
    SelectQuery->ExecQuery();
    return SelectQuery->FieldByName("VERSION_NUMBER")->AsString;
}
//---------------------------------------------------------------------------
TIBSQL* TOracleManagerDB::LoadMeals(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *SelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->Close();
    SelectQuery->SQL->Text = "SELECT MEALIDENTIFIER,STARTTIME,ENDTIME,ISDEFAULT_SERVINGTIME FROM SERVINGTIMESDETAILS";
    SelectQuery->ExecQuery();
    return SelectQuery;
}
//----------------------------------------------------------------------------
TIBSQL* TOracleManagerDB::LoadRevenueCodes(Database::TDBTransaction &DBTransaction)
{
    TIBSQL *SelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->Close();
    SelectQuery->SQL->Text = "SELECT * FROM REVENUECODEDETAILS";
    SelectQuery->ExecQuery();
    return SelectQuery;
}
//----------------------------------------------------------------------------
void TOracleManagerDB::GetRevenueCode(TList *Orders)//(int _itemKey, UnicodeString _sizeName)
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
