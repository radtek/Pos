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
void TManagerPMSCodes::InsertTimeSlots(Database::TDBTransaction &DBTransaction,TTimeSlots mealDetails)
{
    try
    {
        int mealKey = 0;
        TIBSQL *GenQuery =  DBTransaction.Query(DBTransaction.AddQuery());
        GenQuery->Close();
        GenQuery->SQL->Text = "SELECT GEN_ID(GEN_RUNPROGRAMS, 1) FROM RDB$DATABASE";
        GenQuery->ExecQuery();
        mealKey = GenQuery->Fields[0]->AsInteger;

        TIBSQL *InsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
        InsertQuery->SQL->Text =
                   " INSERT INTO SERVINGTIMESDETAILS ( SERVINGTIMES_KEY, MEALNAME, STARTTIME, ENDTIME) "
                   " VALUES ( :SERVINGTIMES_KEY, :MEALNAME, :STARTTIME, :ENDTIME ) " ;
        InsertQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = mealKey;
        InsertQuery->ParamByName("MEALNAME")->AsString = mealDetails.MealName;
        InsertQuery->ParamByName("STARTTIME")->AsDateTime = mealDetails.StartTime;
        InsertQuery->ParamByName("ENDTIME")->AsDateTime = mealDetails.EndTime;
        InsertQuery->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::GetMealDetails(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid,std::vector<TTimeSlots> &TimeSlots)
{
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->SQL->Text =
                   " Select SERVINGTIMES_KEY,MEALNAME,STARTTIME,ENDTIME FROM SERVINGTIMESDETAILS " ;

        SelectQuery->ExecQuery();
        TimeSlots.clear();
        for(;!SelectQuery->Eof; SelectQuery->Next())
        {
           TTimeSlots meals;
           meals.key = SelectQuery->FieldByName("SERVINGTIMES_KEY")->AsInteger;
           meals.MealName = SelectQuery->FieldByName("MEALNAME")->AsString;
           meals.StartTime = SelectQuery->FieldByName("STARTTIME")->AsDateTime;
           meals.EndTime = SelectQuery->FieldByName("ENDTIME")->AsDateTime;
           TimeSlots.push_back(meals);
        }
        PopulateMealsToGrid(StringGrid, TimeSlots);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::PopulateMealsToGrid(TStringGrid * StringGrid,std::vector<TTimeSlots> TimeSlots)
{
   if(TimeSlots.size() < 2)
   {
	   StringGrid->RowCount = 2;
   }
   else
   {
		StringGrid->RowCount = TimeSlots.size() + 1;
   }
    std::map<int,AnsiString>::iterator it;
    int i = 0;
    int Index = 0;
    for(std::vector<TTimeSlots>::iterator iter = TimeSlots.begin(); iter != TimeSlots.end(); ++iter)
    {
        Index = StringGrid->Cols[0]->Add(iter->MealName);
        StringGrid->Cols[0]->Objects[Index] = (TObject*)iter->key;
        AnsiString value = iter->StartTime.TimeString();
        value += " to ";
        value += iter->EndTime.TimeString();
        Index = StringGrid->Cols[1]->Add(value);
        StringGrid->Cols[1]->Objects[Index] = (TObject*)iter->key;
    }

}
//----------------------------------------------------------------------------
void TManagerPMSCodes::DeleteMealDetails(Database::TDBTransaction &DBTransaction,int key)
{
    TIBSQL *DeleteQuery = DBTransaction.Query(DBTransaction.AddQuery());
    DeleteQuery->SQL->Text =
               " DELETE FROM SERVINGTIMESDETAILS WHERE SERVINGTIMES_KEY = :SERVINGTIMES_KEY";
    DeleteQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = key;
    DeleteQuery->ExecQuery();
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::EditMeal(Database::TDBTransaction &DBTransaction,TTimeSlots slots)
{
    TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
    UpdateQuery->SQL->Text =
               " UPDATE SERVINGTIMESDETAILS SET MEALNAME = :MEALNAME, STARTTIME = :STARTTIME, ENDTIME = :ENDTIME "
               " WHERE SERVINGTIMES_KEY = :SERVINGTIMES_KEY";
    UpdateQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = slots.key;
    UpdateQuery->ParamByName("MEALNAME")->AsString = slots.MealName;
    UpdateQuery->ParamByName("STARTTIME")->AsDateTime = slots.StartTime;
    UpdateQuery->ParamByName("ENDTIME")->AsDateTime = slots.EndTime;
    UpdateQuery->ExecQuery();
}
//----------------------------------------------------------------------------



