//---------------------------------------------------------------------------

#ifndef DbMegaworldMallH
#define DbMegaworldMallH
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------

class TDbMegaWorldMall
{
public:

    static bool CheckValueExistInOtherSalesField(int Zed_key);
    static int GetCountForSalesTypeValue(int Zed_key,bool &Foodtype,bool &NonFoodType, bool &GroceriesType, bool &MedicinesType ,bool &OtherType) ;
    static Currency GetTotalSalesAmountFromArcMallExport(int Zed_key);
    static void UpdateSaleTypeValueInArcMallExport(int Zed_key,Currency DivisibleAmount,bool Foodtype,bool NonFoodType, bool GroceriesType, bool MedicinesType ,bool OtherType) ;

    static int GetCurrentControlNumber(int MaxZed) ;
    static void GetMinimumAndMaxDateForCurrentZed(TDateTime DateValue, int &Zedkey) ;
    static void CheckDistinctDateInSameZed(bool &IsDistinctDatePresentForSameZed , TDateTime &DateValue, int Zedkey );
    static void GetFirstDateFromArcMallExportTable(int MaxZed, TDateTime &MaxDateForMaxZed) ;
    static bool CheckIsDateValuePresentInHourlyTable(int MaxZed,TDateTime MaxDateForMaxZed) ;

   __fastcall TDbMegaWorldMall();
};
#endif
