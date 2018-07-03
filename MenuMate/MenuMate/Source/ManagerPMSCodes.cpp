//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerPMSCodes.h"
#include "MMLogging.h"
#include "MMMessageBox.h"
#include "GeneratorManager.h"
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
                        std::map<int,TRevenueCodeDetails> &RevenueCodesMap)
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
void TManagerPMSCodes::UpdateItemSizes(Database::TDBTransaction &DBTransaction,int newValue,int key)
{
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->SQL->Text =
                   "Update ITEMSIZE SET REVENUECODE = :REVENUECODENEW "
                   "WHERE REVENUECODE =:REVENUECODEOLD ";
        UpdateQuery->ParamByName("REVENUECODEOLD")->AsInteger = key;
        UpdateQuery->ParamByName("REVENUECODENEW")->AsInteger = newValue;
        UpdateQuery->ExecQuery();
    }
    catch(Exception &Exc)
    {
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetRevenueCodesFromDB(Database::TDBTransaction &DBTransaction,
                                             std::map<int,TRevenueCodeDetails> &RevenueCodesMap)
{
    TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->SQL->Text =
               "SELECT * FROM REVENUECODEDETAILS ORDER BY REVENUECODE";
    SelectQuery->ExecQuery();
    RevenueCodesMap.clear();
    for(;!SelectQuery->Eof; SelectQuery->Next())
    {
       TRevenueCodeDetails revenueCodeDetail;
       if(SelectQuery->FieldByName("ISDEFAULT_REVENUECODE")->AsString == "F")
           revenueCodeDetail.IsDefault = false;
       else
           revenueCodeDetail.IsDefault = true;
       revenueCodeDetail.RevenueCodeDescription = SelectQuery->FieldByName("REVENUECODE_DESCRIPTION")->AsString;

       RevenueCodesMap[SelectQuery->FieldByName("REVENUECODE")->AsInteger] =
                                                           revenueCodeDetail;
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::PopulateTaxesToGrid(std::map<int,TaxCodesDetails> &TaxCodesMap,TStringGrid * StringGrid)
{

}
//----------------------------------------------------------------------------
void TManagerPMSCodes::PopulateRevenuesToGrid(std::map<int,TRevenueCodeDetails> &RevenueCodeMap,TStringGrid * StringGrid)
{
   if(RevenueCodeMap.size() < 2)
   {
	   StringGrid->RowCount = 2;
   }
   else
   {
		StringGrid->RowCount = RevenueCodeMap.size() + 1;
   }
    std::map<int,AnsiString>::iterator it;
    int i = 0;
    int Index = 0;
    for(std::map <int,TRevenueCodeDetails>::iterator iter = RevenueCodeMap.begin(); iter != RevenueCodeMap.end(); ++iter)
    {
        AnsiString value = "";
        Index = StringGrid->Cols[0]->Add(iter->first);
        StringGrid->Cols[0]->Objects[Index] = (TObject*)iter->first;
        value = iter->second.RevenueCodeDescription;
        if(iter->second.IsDefault)
            value += "(default)";
        Index = StringGrid->Cols[1]->Add(value);
        StringGrid->Cols[1]->Objects[Index] = (TObject*)iter->first;
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::SaveRevenueCodesToDB(Database::TDBTransaction &DBTransaction,
                                            std::map<int,TRevenueCodeDetails> &RevenueCodeMap)
{
    try
    {
        TIBSQL *InsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
        InsertQuery->SQL->Text =
                   "INSERT INTO REVENUECODEDETAILS ( REVENUECODE, REVENUECODE_DESCRIPTION, ISDEFAULT_REVENUECODE ) VALUES "
                   "( :REVENUECODE, :REVENUECODE_DESCRIPTION, :ISDEFAULT_REVENUECODE ) ";
        std::map<int,TRevenueCodeDetails>::iterator it = RevenueCodeMap.begin();
        InsertQuery->ParamByName("REVENUECODE")->AsInteger = it->first;
        InsertQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = it->second.RevenueCodeDescription;
        if(it->second.IsDefault)
           InsertQuery->ParamByName("ISDEFAULT_REVENUECODE")->AsString = "T";
        else
           InsertQuery->ParamByName("ISDEFAULT_REVENUECODE")->AsString = "F";
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
    bool isDefault = false;
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text =
                   "SELECT * FROM REVENUECODEDETAILS WHERE REVENUECODE = :REVENUECODE";
        SelectQuery->ParamByName("REVENUECODE")->AsInteger = key;
        SelectQuery->ExecQuery();
        if(SelectQuery->FieldByName("ISDEFAULT_REVENUECODE")->AsString == "T")
           isDefault = true;
        TIBSQL *DeleteQuery = DBTransaction.Query(DBTransaction.AddQuery());
        DeleteQuery->Close();
        DeleteQuery->SQL->Text =
                   "DELETE FROM REVENUECODEDETAILS WHERE REVENUECODE = :REVENUECODE";
        DeleteQuery->ParamByName("REVENUECODE")->AsInteger = key;
        DeleteQuery->ExecQuery();
        UpdateItemSizes(DBTransaction,0,key);
        if(isDefault)
        {
            TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
            UpdateQuery->Close();
            for(std::map<int,TRevenueCodeDetails>::iterator itrev = RevenueCodesMap.begin();
               itrev != RevenueCodesMap.end(); advance(itrev,1))
            {
                if(itrev->first != key)
                {
                    UpdateQuery->SQL->Text = "UPDATE REVENUECODEDETAILS SET ISDEFAULT_REVENUECODE = :ISDEFAULT_REVENUECODE "
                    "WHERE REVENUECODE = :REVENUECODE";
                    UpdateQuery->ParamByName("ISDEFAULT_REVENUECODE")->AsString = "T";
                    UpdateQuery->ParamByName("REVENUECODE")->AsInteger = itrev->first;
                    UpdateQuery->ExecQuery();
                    break;
                }
            }
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//-----------------------------------------------------------------------------
void TManagerPMSCodes::EditRevenueCode(Database::TDBTransaction &DBTransaction,int key, int newCode, TRevenueCodeDetails codeDetails)
{
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->SQL->Text =
                   " UPDATE REVENUECODEDETAILS SET REVENUECODE = :REVENUECODE, REVENUECODE_DESCRIPTION = :REVENUECODE_DESCRIPTION "
                   " WHERE REVENUECODE = :REVENUECODEOLD";
        UpdateQuery->ParamByName("REVENUECODEOLD")->AsInteger = key;
        UpdateQuery->ParamByName("REVENUECODE")->AsInteger = newCode;
        UpdateQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = codeDetails.RevenueCodeDescription;
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
                   " INSERT INTO SERVINGTIMESDETAILS ( SERVINGTIMES_KEY, MEALIDENTIFIER, STARTTIME, ENDTIME, ISDEFAULT_SERVINGTIME) "
                   " VALUES ( :SERVINGTIMES_KEY, :MEALIDENTIFIER, :STARTTIME, :ENDTIME, :ISDEFAULT_SERVINGTIME ) " ;
        InsertQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = mealKey;
        InsertQuery->ParamByName("MEALIDENTIFIER")->AsInteger = atoi(mealDetails.MealName.c_str());
        InsertQuery->ParamByName("STARTTIME")->AsDateTime = mealDetails.StartTime;
        InsertQuery->ParamByName("ENDTIME")->AsDateTime = mealDetails.EndTime;
        InsertQuery->ParamByName("ISDEFAULT_SERVINGTIME")->AsString = mealDetails.IsDefault ? "T" : "F";
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
                   " Select SERVINGTIMES_KEY,MEALIDENTIFIER,STARTTIME,ENDTIME, ISDEFAULT_SERVINGTIME FROM SERVINGTIMESDETAILS " ;

        SelectQuery->ExecQuery();
        TimeSlots.clear();
        for(;!SelectQuery->Eof; SelectQuery->Next())
        {
           TTimeSlots meals;
           meals.key = SelectQuery->FieldByName("SERVINGTIMES_KEY")->AsInteger;
           meals.MealName = SelectQuery->FieldByName("MEALIDENTIFIER")->AsInteger;
           meals.StartTime = SelectQuery->FieldByName("STARTTIME")->AsDateTime;
           meals.EndTime = SelectQuery->FieldByName("ENDTIME")->AsDateTime;
           meals.IsDefault = SelectQuery->FieldByName("ISDEFAULT_SERVINGTIME")->AsString == "T" ? true : false;
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
        value += iter->IsDefault ? "(default)" : "";
        Index = StringGrid->Cols[1]->Add(value);
        StringGrid->Cols[1]->Objects[Index] = (TObject*)iter->key;
    }

}
//----------------------------------------------------------------------------
void TManagerPMSCodes::DeleteMealDetails(Database::TDBTransaction &DBTransaction,int key)
{
    bool isDefault = false;
    TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
    SelectQuery->Close();
    SelectQuery->SQL->Text =
               "SELECT * FROM SERVINGTIMESDETAILS WHERE SERVINGTIMES_KEY = :SERVINGTIMES_KEY";
    SelectQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = key;
    SelectQuery->ExecQuery();
    if(SelectQuery->FieldByName("ISDEFAULT_SERVINGTIME")->AsString == "T")
       isDefault = true;

    TIBSQL *DeleteQuery = DBTransaction.Query(DBTransaction.AddQuery());
    DeleteQuery->SQL->Text =
               " DELETE FROM SERVINGTIMESDETAILS WHERE SERVINGTIMES_KEY = :SERVINGTIMES_KEY";
    DeleteQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = key;
    DeleteQuery->ExecQuery();

    if(isDefault)
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->Close();
        for(std::vector<TTimeSlots>::iterator itTime = TimeSlots.begin();
           itTime != TimeSlots.end(); advance(itTime,1))
        {
            if(itTime->key != key)
            {
                UpdateQuery->SQL->Text = "UPDATE SERVINGTIMESDETAILS SET ISDEFAULT_SERVINGTIME = :ISDEFAULT_SERVINGTIME "
                "WHERE SERVINGTIMES_KEY = :SERVINGTIMES_KEY";
                UpdateQuery->ParamByName("ISDEFAULT_SERVINGTIME")->AsString = "T";
                UpdateQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = itTime->key;
                UpdateQuery->ExecQuery();
                break;
            }
        }
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::EditMeal(Database::TDBTransaction &DBTransaction,TTimeSlots slots)
{
    TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
    UpdateQuery->SQL->Text =
               " UPDATE SERVINGTIMESDETAILS SET MEALIDENTIFIER = :MEALIDENTIFIER, STARTTIME = :STARTTIME, ENDTIME = :ENDTIME "
               " WHERE SERVINGTIMES_KEY = :SERVINGTIMES_KEY";
    UpdateQuery->ParamByName("SERVINGTIMES_KEY")->AsInteger = slots.key;
    UpdateQuery->ParamByName("MEALIDENTIFIER")->AsInteger = atoi(slots.MealName.c_str());
    UpdateQuery->ParamByName("STARTTIME")->AsDateTime = slots.StartTime;
    UpdateQuery->ParamByName("ENDTIME")->AsDateTime = slots.EndTime;
    UpdateQuery->ExecQuery();
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::SetPMSPaymentType(Database::TDBTransaction &DBTransaction,TPMSPaymentType pmsPayment, bool isNewPaymentType, bool isMMPayType)
{
    if(isNewPaymentType)
    {
        InsertIntoPMSPaymentsConfig(DBTransaction,pmsPayment,isMMPayType);// Insert Query
    }
    else
    {
        //UpdatePMSPaymentType(DBTransaction,pmsPayment);// Update Query
        UpdatePMSPaymentConfig(DBTransaction,pmsPayment);
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::InsertIntoPMSPaymentsConfig(Database::TDBTransaction &DBTransaction,TPMSPaymentType pmsPayment, bool isMMPayType)
{
    UnicodeString field = "";
    UnicodeString param = "";
    if(isMMPayType)
    {
        field = "PMS_MM_PAYTYPELINK, ";
        param = ":PMS_MM_PAYTYPELINK, ";
    }
    try
    {

      TIBSQL *InsertQuery = DBTransaction.Query(DBTransaction.AddQuery());
      InsertQuery->Close();
      InsertQuery->SQL->Text =
                 " INSERT INTO PMSPAYMENTSCONFIG "
                 " (PMS_PAYTYPE_ID, PMS_PAYTYPE_NAME, PMS_PAYTYPE_CODE,"
                 " PMS_PAYTYPE_CATEGORY ," +field+ "IS_ELECTRONICPAYMENT) VALUES"
                 " (:PMS_PAYTYPE_ID, :PMS_PAYTYPE_NAME, :PMS_PAYTYPE_CODE,"
                 " :PMS_PAYTYPE_CATEGORY ,"+ param +":IS_ELECTRONICPAYMENT)";
      InsertQuery->ParamByName("PMS_PAYTYPE_ID")->AsInteger           = TGeneratorManager::GetNextGeneratorKey(DBTransaction,"GEN_PMSPAYTYPEID");
      //GetPMSPayTypeIDFromGenerator(DBTransaction);
      InsertQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString          = pmsPayment.PMSPayTypeName;
      InsertQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString          = pmsPayment.PMSPayTypeCode;
      InsertQuery->ParamByName("PMS_PAYTYPE_CATEGORY")->AsString      = pmsPayment.PMSPayTypeCategory;
      if(isMMPayType)
          InsertQuery->ParamByName("PMS_MM_PAYTYPELINK")->AsInteger     = pmsPayment.PMSMMPayTypeLink;
      InsertQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString       = pmsPayment.isElectronicPayment ? "T" : "F";
      InsertQuery->ExecQuery();
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
int TManagerPMSCodes::GetPMSPayTypeIDFromGenerator(Database::TDBTransaction &DBTransaction)
{
    int key = 0;
    TIBSQL *GenQuery =  DBTransaction.Query(DBTransaction.AddQuery());
    GenQuery->Close();
    GenQuery->SQL->Text = "SELECT GEN_ID(GEN_PMSPAYTYPEID, 1) FROM RDB$DATABASE";
    GenQuery->ExecQuery();
    key = GenQuery->Fields[0]->AsInteger;
    return key;
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::UpdatePMSPaymentConfig(Database::TDBTransaction &DBTransaction,TPMSPaymentType pmsPayment)
{
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->SQL->Text =
                   " UPDATE PMSPAYMENTSCONFIG "
                   " SET"
                   " PMS_PAYTYPE_NAME               = :PMS_PAYTYPE_NAME, "
                   " PMS_PAYTYPE_CATEGORY           = :PMS_PAYTYPE_CATEGORY, "
                   " IS_ELECTRONICPAYMENT           = :IS_ELECTRONICPAYMENT  "
                   " WHERE  PMS_MM_PAYTYPELINK      = :PMS_MM_PAYTYPELINK ";

        UpdateQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString = pmsPayment.PMSPayTypeName;
        UpdateQuery->ParamByName("PMS_PAYTYPE_CATEGORY")->AsString = pmsPayment.PMSPayTypeCategory;
        if(pmsPayment.isElectronicPayment)
            UpdateQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "T";
        else
            UpdateQuery->ParamByName("IS_ELECTRONICPAYMENT")->AsString = "F";
        UpdateQuery->ParamByName("PMS_MM_PAYTYPELINK")->AsInteger = pmsPayment.PMSMMPayTypeLink;
        UpdateQuery->ExecQuery();
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetPMSPaymentTypeDetails(Database::TDBTransaction &DBTransaction,TStringGrid * StringGrid,std::map<int,
                                                                             TPMSPaymentType> &PMSPaymentTypeMap)
{
    GetPMSPaymentTypeFromDB(DBTransaction,PMSPaymentTypeMap);
    PopulatePaymenyTypeToGrid(PMSPaymentTypeMap,StringGrid);
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::GetPMSPaymentTypeFromDB(Database::TDBTransaction &DBTransaction,std::map<int, TPMSPaymentType> &PMSPaymentTypeMap)
{
    try
    {
        TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
        SelectQuery->Close();
        SelectQuery->SQL->Text =
                   " SELECT * FROM  PMSPAYMENTSCONFIG ORDER BY PMS_PAYTYPE_CATEGORY";
        SelectQuery->ExecQuery();
        PMSPaymentTypeMap.clear();
        for(; !SelectQuery->Eof; SelectQuery->Next())
        {
           TPMSPaymentType pmsPayment;
           pmsPayment.PMSPayTypeID                  = SelectQuery->FieldByName("PMS_PAYTYPE_ID")->AsInteger;
           pmsPayment.PMSPayTypeName                = SelectQuery->FieldByName("PMS_PAYTYPE_NAME")->AsString;
           pmsPayment.PMSPayTypeCode                = SelectQuery->FieldByName("PMS_PAYTYPE_CODE")->AsString;
           pmsPayment.PMSPayTypeCategory            = SelectQuery->FieldByName("PMS_PAYTYPE_CATEGORY")->AsInteger;
           pmsPayment.PMSMMPayTypeLink              = SelectQuery->FieldByName("PMS_MM_PAYTYPELINK")->AsInteger == NULL ? 0 : SelectQuery->FieldByName("PMS_MM_PAYTYPELINK")->AsInteger;
           pmsPayment.isElectronicPayment           = SelectQuery->FieldByName("IS_ELECTRONICPAYMENT")->AsString == "T" ? true : false;
           PMSPaymentTypeMap[pmsPayment.PMSPayTypeID]
                                                    = pmsPayment;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::PopulatePaymenyTypeToGrid(std::map<int, TPMSPaymentType> PMSPaymentTypeMap,TStringGrid * StringGrid)
{
    try
    {
       if(PMSPaymentTypeMap.size() < 2)
       {
           StringGrid->RowCount = 2;
       }
       else
       {
            StringGrid->RowCount = PMSPaymentTypeMap.size() + 1;
       }
        std::map<int,TPMSPaymentType>::iterator it;
        int i = 0;
        int Index = 0;
        for(std::map <int,TPMSPaymentType>::iterator iter = PMSPaymentTypeMap.begin(); iter != PMSPaymentTypeMap.end(); ++iter)
        {
            AnsiString value = "";
            Index = StringGrid->Cols[0]->Add(iter->second.PMSPayTypeName);
            StringGrid->Cols[0]->Objects[Index] = (TObject*)iter->first;
            value = iter->second.PMSPayTypeCode;
            Index = StringGrid->Cols[1]->Add(value);
            StringGrid->Cols[1]->Objects[Index] = (TObject*)iter->first;
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::DeletePMSPaymentType(Database::TDBTransaction &DBTransaction,int key)
{
    try
    {
        TIBSQL *DeleteQuery = DBTransaction.Query(DBTransaction.AddQuery());
        DeleteQuery->SQL->Text =
                   " DELETE FROM PMSPAYMENTSCONFIG WHERE PMS_PAYTYPE_ID = :PMS_PAYTYPE_ID";
        DeleteQuery->ParamByName("PMS_PAYTYPE_ID")->AsInteger = key;
        DeleteQuery->ExecQuery();
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------
void TManagerPMSCodes::UpdatePMSPaymentType(Database::TDBTransaction &DBTransaction,TPMSPaymentType pmsPaymentType)
{
    try
    {
        TIBSQL *UpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
        UpdateQuery->SQL->Text =
                   " UPDATE PMSPAYMENTSCONFIG "
                   " SET "
                   " PMS_PAYTYPE_NAME       = :PMS_PAYTYPE_NAME, "
                   " PMS_PAYTYPE_CODE       = :PMS_PAYTYPE_CODE "
                   " WHERE PMS_PAYTYPE_ID   = :PMS_PAYTYPE_ID2";

        UpdateQuery->ParamByName("PMS_PAYTYPE_NAME")->AsString = pmsPaymentType.PMSPayTypeName;
        UpdateQuery->ParamByName("PMS_PAYTYPE_CODE")->AsString = pmsPaymentType.PMSPayTypeCode;
        UpdateQuery->ParamByName("PMS_PAYTYPE_ID2")->AsInteger = pmsPaymentType.PMSPayTypeID;
        UpdateQuery->ExecQuery();
        if(pmsPaymentType.PMSPayTypeCategory == 0)
        {
            TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmPMSPaymentCategory,pmsPaymentType.PMSPayTypeCode);
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//----------------------------------------------------------------------------



