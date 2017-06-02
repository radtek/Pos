//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerPMSCodes.h"
#include "MMLogging.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerPMSCodes::TManagerPMSCodes()
{
}
//----------------------------------------------------------------------------
TManagerPMSCodes::~TManagerPMSCodes()
{
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetTaxCodesDetails(Database::TDBTransaction &DBTransaction, TStringGrid * StringGrid,
                                std::map<int,TaxCodesDetails> &TaxCodesMap)
{
    GetTaxCodesFromDB(DBTransaction,TaxCodesMap);
    PopulateTaxesToGrid(TaxCodesMap,StringGrid);
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetRevenueCodesDetails(Database::TDBTransaction &DBTransaction, TStringGrid * StringGrid,
                        std::map<int,AnsiString> &RevenueCodesMap)
{
    GetRevenueCodesFromDB(DBTransaction,RevenueCodesMap);
    PopulateRevenuesToGrid(RevenueCodesMap,StringGrid);
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetTaxCodesFromDB(Database::TDBTransaction &DBTransaction,std::map<int,TaxCodesDetails> TaxCodesMap)
{
      TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      IBInternalQuery->SQL->Text =
                       "SELECT * FROM TAXCODESDETAILS";
      IBInternalQuery->ExecQuery();
      for(;!IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
      }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetRevenueCodesFromDB(Database::TDBTransaction &DBTransaction,
                                             std::map<int,AnsiString> &RevenueCodesMap)
{
    TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->SQL->Text =
               "SELECT * FROM REVENUECODEDETAILS ORDER BY REVENUECODE";
    SelectQuery->ExecQuery();
    RevenueCodesMap.clear();
    for(;!SelectQuery->Eof; SelectQuery->Next())
    {
       RevenueCodesMap[SelectQuery->FieldByName("REVENUECODE")->AsInteger] =
       SelectQuery->FieldByName("REVENUECODE_DESCRIPTION")->AsString;
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::PopulateTaxesToGrid(std::map<int,TaxCodesDetails> &TaxCodesMap,TStringGrid * StringGrid)
{

}
//----------------------------------------------------------------------------
void TManagerPMSCodes::PopulateRevenuesToGrid(std::map<int,AnsiString> &RevenueCodeMap,TStringGrid * StringGrid)
{
   if(RevenueCodeMap.size() < 2)
   {
	   StringGrid->RowCount = 2;
   }
   else
   {
		StringGrid->RowCount = RevenueCodeMap.size() + 1;
//        MessageBox("New size defined","",MB_OK);
   }
    std::map<int,AnsiString>::iterator it;
    int i = 0;
    int Index = 0;
    for(std::map <int,AnsiString>::iterator iter = RevenueCodeMap.begin(); iter != RevenueCodeMap.end(); ++iter)
    {
        Index = StringGrid->Cols[0]->Add(iter->first);
//        MessageBox(Index,"Index",MB_OK);
        StringGrid->Cols[0]->Objects[Index] = (TObject*)iter->first;
        Index = StringGrid->Cols[1]->Add(iter->second);
//        MessageBox(Index,"Index2",MB_OK);
        StringGrid->Cols[1]->Objects[Index] = (TObject*)iter->first;
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::SaveRevenueCodesToDB(Database::TDBTransaction &DBTransaction,
                                            std::map<int,AnsiString> &RevenueCodeMap)
{
    try
    {
        TIBSQL *InsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
        InsertQuery->SQL->Text =
                   "INSERT INTO REVENUECODEDETAILS ( REVENUECODE, REVENUECODE_DESCRIPTION ) VALUES "
                   "( :REVENUECODE, :REVENUECODE_DESCRIPTION ) ";
        std::map<int,AnsiString>::iterator it = RevenueCodeMap.begin();
        InsertQuery->ParamByName("REVENUECODE")->AsInteger = it->first;
        InsertQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = it->second;
        InsertQuery->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::DeleteRevenueCode(Database::TDBTransaction &DBTransaction,int key)
{
    try
    {
        TIBSQL *DeleteQuery = DBTransaction.Query(DBTransaction.AddQuery());
        DeleteQuery->SQL->Text =
                   "DELETE FROM REVENUECODEDETAILS WHERE REVENUECODE = :REVENUECODE";
        DeleteQuery->ParamByName("REVENUECODE")->AsInteger = key;
        DeleteQuery->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::EditRevenueCode(Database::TDBTransaction &DBTransaction,int key, int newCode, AnsiString newDescription)
{
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->SQL->Text =
                   " UPDATE REVENUECODEDETAILS SET REVENUECODE = :REVENUECODE, REVENUECODE_DESCRIPTION = :REVENUECODE_DESCRIPTION "
                   " WHERE REVENUECODE = :REVENUECODEOLD";
        UpdateQuery->ParamByName("REVENUECODEOLD")->AsInteger = key;
        UpdateQuery->ParamByName("REVENUECODE")->AsInteger = newCode;
        UpdateQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = newDescription;
        UpdateQuery->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------


