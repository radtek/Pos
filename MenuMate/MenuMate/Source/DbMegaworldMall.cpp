//---------------------------------------------------------------------------


#pragma hdrstop

#include "DbMegaworldMall.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TDbMegaWorldMall::TDbMegaWorldMall()
{
}

bool TDbMegaWorldMall::CheckValueExistInOtherSalesField(int Zed_key)
{
    Currency GiftCard_Vale = 0;
    Currency Check_Value = 0;
    bool IsValueNotZeroInGC_CheckSales = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =  "SELECT COALESCE(SUM(a.GIFT_CARD),0)GIFT_CARD,COALESCE(SUM(a.CHECK_SALES),0)CHECK_SALES  "
                                      "FROM ARCMALLEXPORTHOURLY a "
                                      "WHERE a.Z_KEY = :Zed_Value ";

        IBInternalQuery->ParamByName("Zed_Value")->AsInteger = Zed_key;
        IBInternalQuery->ExecQuery();

        GiftCard_Vale = IBInternalQuery->FieldByName("GIFT_CARD")->AsCurrency;
        Check_Value = IBInternalQuery->FieldByName("CHECK_SALES")->AsCurrency;

        if(GiftCard_Vale>0)
        {
          IsValueNotZeroInGC_CheckSales = true;
        }
        else if(Check_Value>0)
        {
          IsValueNotZeroInGC_CheckSales = true;
        }

        DBTransaction.Commit();
    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }

    return IsValueNotZeroInGC_CheckSales;
}
//-------------------------------------------------------------------------------
int TDbMegaWorldMall::GetCountForSalesTypeValue(int Zed_key,bool &Foodtype,bool &NonFoodType, bool &GroceriesType, bool &MedicinesType ,bool &OtherType)
{
    int count = 0;
    Currency Salestype_food = 0;
    Currency Salestype_nonfood = 0;
    Currency Salestype_groceries = 0;
    Currency Salestype_medicines = 0;
    Currency Salestype_others = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =  "SELECT a.SALESTYPE_FOOD,a.SALESTYPE_NONFOOD,a.SALESTYPE_GROCERIES,a.SALESTYPE_MEDICINES,a.SALESTYPE_OTHERS "
                                      "FROM ARCMALLEXPORT a "
                                      "WHERE a.Z_KEY = :Zed_Value ";

        IBInternalQuery->ParamByName("Zed_Value")->AsInteger = Zed_key;
        IBInternalQuery->ExecQuery();

        Salestype_food = IBInternalQuery->FieldByName("SALESTYPE_FOOD")->AsCurrency;
        if(Salestype_food > 0)
        {
          Foodtype = true;
          count++;
        }
        Salestype_nonfood = IBInternalQuery->FieldByName("SALESTYPE_NONFOOD")->AsCurrency;
        if(Salestype_nonfood > 0)
        {
          NonFoodType = true;
          count++;
        }
        Salestype_groceries = IBInternalQuery->FieldByName("SALESTYPE_GROCERIES")->AsCurrency;
        if(Salestype_groceries > 0)
        {
          GroceriesType = true;
          count++;
        }
        Salestype_medicines = IBInternalQuery->FieldByName("SALESTYPE_MEDICINES")->AsCurrency;
        if(Salestype_medicines > 0)
        {
          MedicinesType = true;
          count++;
        }
        Salestype_others = IBInternalQuery->FieldByName("SALESTYPE_OTHERS")->AsCurrency;
        if(Salestype_others > 0)
        {
             OtherType = true;
             count++;

        }
        DBTransaction.Commit();
    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }
    return count;
}
//-------------------------------------------------------------------------------
Currency TDbMegaWorldMall::GetTotalSalesAmountFromArcMallExport(int Zed_key)
{
    Currency Cashsales = 0;
    Currency othersales = 0;
    Currency cardsales = 0;
    Currency voidamount = 0;
    Currency TotalAmount = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =  "SELECT a.CASH_SALES , a.OTHER_SALES ,a.CARD_SALES,a.VOID_AMOUNT "
                                      "FROM ARCMALLEXPORT a "
                                      "WHERE a.Z_KEY = :Zed_Value ";

        IBInternalQuery->ParamByName("Zed_Value")->AsInteger = Zed_key;
        IBInternalQuery->ExecQuery();

        Cashsales = IBInternalQuery->FieldByName("CASH_SALES")->AsCurrency;
        othersales = IBInternalQuery->FieldByName("OTHER_SALES")->AsCurrency;
        cardsales = IBInternalQuery->FieldByName("CARD_SALES")->AsCurrency;
        voidamount = IBInternalQuery->FieldByName("VOID_AMOUNT")->AsCurrency;

        TotalAmount = Cashsales + othersales + cardsales + voidamount;
        DBTransaction.Commit();
    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }

    return TotalAmount;
}
//-------------------------------------------------------------------------------
void TDbMegaWorldMall::UpdateSaleTypeValueInArcMallExport(int Zed_key,Currency DivisibleAmount,bool Foodtype,bool NonFoodType, bool GroceriesType, bool MedicinesType ,bool OtherType)
{

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());


        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "UPDATE ARCMALLEXPORT SET ARCMALLEXPORT.SALESTYPE_FOOD = :SALESTYPEFOOD, "
                                     "ARCMALLEXPORT.SALESTYPE_NONFOOD = :SALESTYPENONFOOD, "
                                     "ARCMALLEXPORT.SALESTYPE_GROCERIES = :SALESTYPEGROCERIES, "
                                     "ARCMALLEXPORT.SALESTYPE_MEDICINES = :SALESTYPEMEDICINES, "
                                     "ARCMALLEXPORT.SALESTYPE_OTHERS = :SALESTYPEOTHERS "
                                     "WHERE ARCMALLEXPORT.Z_KEY  =:Z_VALUE ";

        if(Foodtype)
        {
          IBInternalQuery->ParamByName("SALESTYPEFOOD")->AsCurrency = DivisibleAmount;
        }
        else
        {
         IBInternalQuery->ParamByName("SALESTYPEFOOD")->AsCurrency = 0;
        }

        if(NonFoodType)
        {
          IBInternalQuery->ParamByName("SALESTYPENONFOOD")->AsCurrency = DivisibleAmount;
        }
        else
        {
         IBInternalQuery->ParamByName("SALESTYPENONFOOD")->AsCurrency = 0;
        }
        if(GroceriesType)
        {
          IBInternalQuery->ParamByName("SALESTYPEGROCERIES")->AsCurrency = DivisibleAmount;
        }
        else
        {
         IBInternalQuery->ParamByName("SALESTYPEGROCERIES")->AsCurrency = 0;
        }
        if(MedicinesType)
        {
          IBInternalQuery->ParamByName("SALESTYPEMEDICINES")->AsCurrency = DivisibleAmount;
        }
        else
        {
         IBInternalQuery->ParamByName("SALESTYPEMEDICINES")->AsCurrency = 0;
        }
        if(OtherType)
        {
          IBInternalQuery->ParamByName("SALESTYPEOTHERS")->AsCurrency = DivisibleAmount;
        }
        else
        {
         IBInternalQuery->ParamByName("SALESTYPEOTHERS")->AsCurrency = 0;
        }
        IBInternalQuery->ParamByName("Z_VALUE")->AsInteger = Zed_key;

        IBInternalQuery->ExecQuery();

        DBTransaction.Commit();

    }
    catch(Exception & E)
    {
        DBTransaction.Rollback();
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        TManagerLogs::Instance().AddLastError(EXCEPTIONLOG);
    }

}

//-------------------------------------------------------------------------------
int TDbMegaWorldMall::GetCurrentControlNumber(int maxzed )
{
    int CurrentZedNumber  = 0;
    try
    {

        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL* IBQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBQuery->Close();

        IBQuery->SQL->Text =  "SELECT a.EODNEW "
                            "FROM ARCMALLEXPORT a "
                            "WHERE a.Z_KEY = :MaxZed ";

        IBQuery->ParamByName("MaxZed")->AsInteger = maxzed;

        IBQuery->ExecQuery();
        if(IBQuery->RecordCount)
        {
            CurrentZedNumber = IBQuery->Fields[0]->AsInteger;
        }

        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

    return CurrentZedNumber;
}

//---------------------------------------------------------------------------
void TDbMegaWorldMall::GetMinimumAndMaxDateForCurrentZed(TDateTime CurrentDatevalue, int &Zedkey)
{
   try
   {
       int Count = 0;
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
       query->Close();


              query->SQL->Text = "SELECT MAX(a.Z_KEY)Z_KEY "
                                 "FROM ARCMALLEXPORTHOURLY a "
                                 "WHERE extract(DAY from (a.DATE_VALUE)) = :day_startday "
                                 "AND extract(MONTH from (a.DATE_VALUE)) = :month_startday "
                                 "AND extract(YEAR from (a.DATE_VALUE))  = :year_startday ";


               query->ParamByName("day_startday")->AsString = CurrentDatevalue.FormatString("dd");

               query->ParamByName("month_startday")->AsString = CurrentDatevalue.FormatString("mm");

               query->ParamByName("year_startday")->AsString =CurrentDatevalue.FormatString("yyyy");

               query->ExecQuery();

        if(query->RecordCount)
        {
            Zedkey = query->Fields[0]->AsInteger;
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

}
//---------------------------------------------------------------------------
void TDbMegaWorldMall::CheckDistinctDateInSameZed(bool &IsDistinctDatePresentForSameZed, TDateTime &Min_Temp_DateValue,int Zedkey)
{
   try
   {
       TDateTime Max_tempdatevalue;
       UnicodeString Max_DateValue = "";
       UnicodeString Min_DateValue = "";
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
       query->Close();


       query->SQL->Text = "SELECT MAX(a.DATE_VALUE) "
                          "FROM ARCMALLEXPORTHOURLY a "
                          "WHERE a.Z_KEY = :ZED_KEY ";

       query->ParamByName("ZED_KEY")->AsInteger = Zedkey;

       query->ExecQuery();

        if(query->RecordCount)
        {
            Max_tempdatevalue = query->Fields[0]->AsDate;
            Max_DateValue = Max_tempdatevalue.FormatString("mm/dd/yyyy");
        }

        query->Close();

        query->SQL->Text = "SELECT MIN(a.DATE_VALUE) "
                          "FROM ARCMALLEXPORTHOURLY a "
                          "WHERE a.Z_KEY = :ZED_KEY ";

        query->ParamByName("ZED_KEY")->AsInteger = Zedkey;

        query->ExecQuery();

        if(query->RecordCount)
        {
            Min_Temp_DateValue = query->Fields[0]->AsDate;
            Min_DateValue = Min_Temp_DateValue.FormatString("mm/dd/yyyy");
        }
        if(Max_DateValue != Min_DateValue )
        {
          IsDistinctDatePresentForSameZed = true;
        }

        else
        {
          IsDistinctDatePresentForSameZed = false;
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

}

//---------------------------------------------------------------------------
void TDbMegaWorldMall::GetFirstDateFromArcMallExportTable(int Max_Zed_Key ,TDateTime &Max_datevalue)
{
   try
   {
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
       query->Close();


       query->SQL->Text =  "SELECT CAST(MAX(a.DATE_VALUE) AS date)DATE_VALUE "
                           "FROM ARCMALLEXPORT a "
                           "WHERE a.Z_KEY = :Max_Zed_Key ";

       query->ParamByName("Max_Zed_Key")->AsInteger = Max_Zed_Key ;

       query->ExecQuery();
        if(query->RecordCount)
        {
            Max_datevalue = query->Fields[0]->AsDate;
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

}

//---------------------------------------------------------------------------
bool TDbMegaWorldMall::CheckIsDateValuePresentInHourlyTable(int Max_Zed_Key ,TDateTime Max_datevalue )
{
   UnicodeString max_date_Arcmallexport = "";
   UnicodeString Date_ArcMallExport_Hourly = "";
   max_date_Arcmallexport = Max_datevalue.FormatString("mm/dd/yyyy");
   bool CheckDatePresent = false;
   try
   {
       TDateTime Hourly_datevalue;
       Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
       DBTransaction.StartTransaction();
       TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
       query->Close();


       query->SQL->Text =  "SELECT CAST(a.DATE_VALUE AS date)Date_value "
                           "FROM ARCMALLEXPORTHOURLY a "
                           "WHERE a.Z_KEY = :Max_Zed_Key "
                           "GROUP BY 1 ";

      query->ParamByName("Max_Zed_Key")->AsInteger = Max_Zed_Key ;
      query->ExecQuery();
      if(query->RecordCount)
      {

         Hourly_datevalue = query->Fields[0]->AsDate;
         Date_ArcMallExport_Hourly = Hourly_datevalue.FormatString("mm/dd/yyyy");
         if(max_date_Arcmallexport == Date_ArcMallExport_Hourly)
         {
           CheckDatePresent = true;
         }
         else
         {
           CheckDatePresent = false;
         }
      }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}

    return CheckDatePresent;
}

