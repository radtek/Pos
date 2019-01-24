//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportMegaworldMall.h"
#include "MallExportDataManager.h"
#include "MallExportHourlyUpdate.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "Main.h"
#include "DbMegaworldMall.h"
#include <sstream>
#include <dirent.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportMegaworldMall::TMallExportMegaworldMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportMegaworldMall::~TMallExportMegaworldMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportMegaworldMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

TExportResponse TMallExportMegaworldMall::ZExport()
{
    TExportResponse result;
    //::::::::::::::::::::::::::::::::::::::
    try
    {
        UnicodeString LocalPath = BasePath + "\\MegaworldMall\\";
        UnicodeString LocalPathFileName = "";
        UnicodeString MallPath = FilePath;
        UnicodeString MallPathFileName = "";
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString TenantNo = TGlobalSettings::Instance().TenantNo;
        UnicodeString DateForMaxZed = "";
        UnicodeString DateForSecondMaxZed = "";
        TDateTime datevalue;
        TDateTime CurrentDateValue ;
        CurrentDateValue = Now();
        int MaxZed = 0;
        int SecondMaxZed = 0;
        bool BreakConSolidateDateForCurrentDate = true;
        GetMaxZedKeyAndSecondMaxZedKey(MaxZed ,SecondMaxZed);
        UnicodeString CurrentDateValueFormat =CurrentDateValue.FormatString("mm/dd/yyyy");
        GetFirstDateValueForMaxZedAndSecondMaxZed(datevalue,DateForMaxZed,DateForSecondMaxZed,MaxZed,SecondMaxZed);
        IsConsolidatedOrNot(BreakConSolidateDateForCurrentDate,MaxZed,SecondMaxZed);
        int zeroZCount;
        TDateTime zeroSaleDate;
        bool prepareDataForZeroSales = false;
        bool zeroZDoneBeforeAnySale = false;
        CheckTransactionDoneBeforeZed(zeroSaleDate, zeroZCount, zeroZDoneBeforeAnySale, prepareDataForZeroSales);

        if(!BreakConSolidateDateForCurrentDate)
        {
            if(prepareDataForZeroSales)
                TGlobalSettings::Instance().BatchNo = zeroZCount;
            else
                TGlobalSettings::Instance().BatchNo += 1;
        }
        else
        {
            bool IsDistinctDatePresent = false ;
            TDateTime MinimumDateForSameZed;
            int Zed_Key = 0;

            if(prepareDataForZeroSales)
                TGlobalSettings::Instance().BatchNo = zeroZCount;
            else if(zeroZDoneBeforeAnySale)
                TGlobalSettings::Instance().BatchNo += 1;
            else
                TGlobalSettings::Instance().BatchNo = zeroZCount + 1;

            TDbMegaWorldMall::GetMinimumAndMaxDateForCurrentZed(CurrentDateValue,Zed_Key);
            TDbMegaWorldMall::CheckDistinctDateInSameZed(IsDistinctDatePresent, MinimumDateForSameZed,Zed_Key);

            if(IsDistinctDatePresent)
            {
                datevalue = MinimumDateForSameZed;
            }
            else
            {
                TDateTime MaxDateForMaxZed;
                TDbMegaWorldMall::GetFirstDateFromArcMallExportTable(MaxZed,MaxDateForMaxZed);
                bool IsDateValueMatched = TDbMegaWorldMall::CheckIsDateValuePresentInHourlyTable(MaxZed,MaxDateForMaxZed);

                if(!IsDateValueMatched)
                    datevalue = Now()-1;
                else
                    datevalue = CurrentDateValue;
            }
        }

        SaveIntVariable(vmBatchNo, TGlobalSettings::Instance().BatchNo);


        if(prepareDataForZeroSales)
        {
            CreateFilename("H", MallPath, LocalPath, LocalPathFileName, MallPathFileName,zeroSaleDate);
            PrepareZeroSalesDateForHourly(zeroSaleDate);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
            CreateFilename("S", MallPath, LocalPath, LocalPathFileName, MallPathFileName,zeroSaleDate);
            PrepareDateForDaily(zeroSaleDate, false);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
            CreateFilename("D", MallPath, LocalPath, LocalPathFileName, MallPathFileName,zeroSaleDate);
            PrepareZeroSalesDiscounts(zeroSaleDate);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        }
        else
        {
            CreateFilename("H", MallPath, LocalPath, LocalPathFileName, MallPathFileName,datevalue);
            PrepareDateForHourly(datevalue);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
            CreateFilename("S", MallPath, LocalPath, LocalPathFileName, MallPathFileName,datevalue);
            PrepareDateForDaily(datevalue);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
            CreateFilename("D", MallPath, LocalPath, LocalPathFileName, MallPathFileName,datevalue);
            PrepareDiscounts(datevalue);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        }

        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);

        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );

    }

    catch( Exception& exc )
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }
    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

// Virtual methods
void TMallExportMegaworldMall::initTimingExportResponse( TExportResponse& outResponse )
{
    // TODO: Initiate the response for Ayala
    setExportResponse(
       false,               // Succesful = false
       emrExportNotDoneYet, // Result = emrExportNotDoneYet
       "",                  // Message
       "",                  // Description
       outResponse );
}
//---------------------------------------------------------------------------

TExportResponse TMallExportMegaworldMall::TransactionExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mm/dd/yy");
        SaveStrVariable(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);

        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );
    }
    catch( Exception& exc )
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }
    //::::::::::::::::::::::::::::::::::::::

    return result;
}

//---------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                                         UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName,TDateTime &DateValue)
{
    TExportResponse result;

    try
    {
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString BatchNo = IntToStr(TGlobalSettings::Instance().BatchNo);
        UnicodeString TenantID = "";
        UnicodeString MonthValue = "";
        UnicodeString DayValue = "";
        TExportResponse result;

        GetExportInfo(MallPath, TenantID, TerminalNo, BatchNo, MonthValue, DayValue, DateValue);
        MallPath = CheckDir(MallPath);
        MallPathFileName = MallPath + mode + TenantID + TerminalNo + BatchNo + "." + MonthValue + DayValue;


        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );
    }
    catch( Exception& exc )
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }

    return result;
}

//---------------------------------------------------------------------------

UnicodeString TMallExportMegaworldMall::CheckDir(UnicodeString Path)
{
    if(!DirectoryExists(Path))
    {
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
    }
    return Path;
}
//---------------------------------------------------------------------------
void TMallExportMegaworldMall::SaveIntVariable(vmVariables vmVar, int CompName)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }
    TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TMallExportMegaworldMall::SaveStrVariable(vmVariables vmVar, UnicodeString CompName)
{
 		Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	    DBTransaction.StartTransaction();
	    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
	    if(GlobalProfileKey == 0)
	    {
		    GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
	    }
        TManagerVariable::Instance().SetProfileStr(DBTransaction, GlobalProfileKey, vmVar, CompName);
        DBTransaction.Commit();
}
//---------------------------------------------------------------------------

UnicodeString TMallExportMegaworldMall::RemoveDecimalValue(UnicodeString amountValue)
{
    UnicodeString result = "";
    std::string DataAmountSTR;
    DataAmountSTR = amountValue.t_str();

    for(int i=0;i<amountValue.Length();i++)
    {
        if(DataAmountSTR[i] != '.')
        {
            result = result + DataAmountSTR[i];
        }
    }
    return result;
}

//--------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                                        UnicodeString &TerminalNo, UnicodeString &BatchNo,
                                                        UnicodeString &MonthValue, UnicodeString &DayValue,
                                                        TDateTime DateValue)
{
    TExportResponse result;

    try
    {
        UnicodeString TenantNo = TGlobalSettings::Instance().TenantNo;
        UnicodeString CheckMonth = DateValue.FormatString("mm");
        UnicodeString CheckDay = DateValue.FormatString("dd");

        int Month = StrToInt(CheckMonth);
        std::string TenantNameSTR = TenantNo.t_str();

        Month = StrToInt(CheckMonth);

        for(int i=0;i<TenantNo.Length();i++)
        {
            if(i<4)
            {
                TenantID = TenantID + TenantNameSTR[i];
            }
        }

        if (TerminalNo.Length() == 1)
        {
            TerminalNo = "0" + TerminalNo;
        }

        // Month value must be 1-9 and A-C(10,11,12)
        for(int i=0;i<12;i++)
        {
            if(Month == 10)
            {
                MonthValue = "A";
                break;
            }
            else if(Month == 11)
            {
                MonthValue = "B";
                break;
            }
            else if(Month == 12)
            {
                MonthValue = "C";
                break;
            }
            else
            {
                MonthValue = IntToStr(Month);
            }
        }

        // Day should have at least 2 digit in format
        DayValue = (CheckDay.Length() == 1) ? "0" + CheckDay : CheckDay;

        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );
    }
    catch( Exception& exc )
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }

    return result;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// This method fixes the decimal point of a currency value
UnicodeString TMallExportMegaworldMall::FixDecimalPlaces(Currency AmountTotal)
{
    UnicodeString result = "";
    UnicodeString GetData = AmountTotal;
    std::string TempData = "";
    std::string value;
    std::string adecimal;
    bool decimal = false;

    TempData = GetData.t_str();

    for(int i=0;i<TempData.length();i++)
    {
        if((TempData.at(i) != '.') && (!decimal))
        {
            value = value + TempData.at(i);
        }
        else
        {
            decimal = true;
            if(TempData.at(i) != '.')
            {
                adecimal = adecimal + TempData.at(i);
            }
        }
    }

    if(adecimal.length() == 1)
    {
        adecimal = adecimal + "0";
    }
    GetData = value.c_str();

    if(!decimal)
    {
        result = GetData + ".00";
    }
    else
    {
        result = GetData + "." + adecimal.c_str();
    }

    return result;
}


//---------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::PrepareDateForHourly(TDateTime DateFieldInHourlyData)
{
    TExportResponse result;
    UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
    UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
    UnicodeString DateValue = DateFieldInHourlyData.FormatString("mmddyyyy");
    Currency AmountSum = 0;
    int TransactionTotal = 0;
    int TransactionSum = 0;
    int PatronCount = 0;
    int PatronSum = 0;
    UnicodeString Amount = "";
    UnicodeString TotalValue = "";
    UnicodeString OutputValue = "";
    TDateTime CurrentdateTime = Now();
    UnicodeString CurrentdateTimeFormat = CurrentdateTime.FormatString("mm/dd/yyyy");
    try
    {
        OutputValue = "01" + TenantName + "\n" +
                      "02" + TerminalNum + "\n" +
                      "03" + DateValue + "\n";
        DataToWrite.push_back(OutputValue.t_str());
        for(int i=0;i<24;i++)
        {
            UnicodeString Time = i;
            Time = (Time.Length() == 1) ? "0" + Time : Time;

            GetHourlyData(TerminalNum, TenantName, DateValue, Time, TransactionTotal, Amount, PatronCount);

            if(TransactionTotal == 0)
            {
                continue;
            }
            else
            {
                UnicodeString TempAmount = "";
                TempAmount = RemoveDecimalValue(Amount);
                OutputValue = "04" + Time + "\n" +
                              "05" + TempAmount + "\n" +
                              "06" + TransactionTotal + "\n" +
                              "07" + PatronCount + "\n";
                DataToWrite.push_back(OutputValue.t_str());
            }
            AmountSum += StrToCurr(Amount);
            TransactionSum += TransactionTotal;
            PatronSum += PatronCount;
        }
        TotalValue = FixDecimalPlaces(AmountSum);
        TotalValue = RemoveDecimalValue(TotalValue);
        OutputValue = "08" + TotalValue + "\n" +
                      "09" + TransactionSum + "\n" +
                      "10" + PatronSum;
        DataToWrite.push_back(OutputValue.t_str());

        setExportResponse(
               true,               // Succesful = false
               emrExportSuccesful, // Result = emrExportSuccesful
               "",                 // Message
               "",                 // Description
               result );

    }

    catch(Exception &ex)
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }

    return result;

}
//---------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::PrepareDateForDaily(TDateTime DateFieldInDailyData, bool prepareZeroDateForDaily)
{

    UnicodeString SalesTypeID = "";
    UnicodeString TenantID = "";
    UnicodeString FOOD = "SalesTypeFood";
    UnicodeString NONFOOD = "SalesTypeNon-Food";
    UnicodeString GROCERIES = "SalesTypeGroceries";
    UnicodeString MEDICINES = "SalesTypeMedicines";
    UnicodeString OTHERS = "SalesTypeOthers";
    UnicodeString tempTenant = "";
    UnicodeString tempTerminal = "";
    std::string TenantNameSTR;
    TExportResponse result;
    std::vector<UnicodeString> AvailableSalesType;
    DataToWrite.clear();
    try
    {
        bool BreakConSolidateDateForCurrentDate = true;
        Currency oldgrandtotal = 0;
        Currency Newgrandtotal = 0;
        Currency grosssales = 0;
        Currency vatexemptSales=0;
        Currency ScdAmount = 0;
        Currency RegdiscountAmount = 0;
        Currency RefundAmount = 0;
        Currency VatSales = 0;
        Currency SchargeAmount = 0;
        Currency DailySales = 0;
        Currency CashSales = 0;
        Currency CardSales = 0;
        Currency OtherSales = 0;
        Currency VoidAmount = 0;
        Currency salestype_food = 0;
        Currency salestype_Nonfood = 0;
        Currency salestype_Groceries = 0;
        Currency salestype_Medicines = 0;
        Currency salestype_others = 0;

        int FineDineCustCount = 0;
        int EodCounter = 0;
        int TransactionCount = 0;


        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query0 = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());


        UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
        UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
        UnicodeString DateValue = DateFieldInDailyData.FormatString("mmddyyyy");

        OutputValue = "01" + TenantName + "\n" +
                      "02" + TerminalNum + "\n" +
                      "03" + DateValue + "\n";
        DataToWrite.push_back(OutputValue.t_str());


        int MaxZedKey;
        int SecondMaxZedKey;
        TDateTime DateValueForSecondMaxZed ;
        GetMaxZedKeyAndSecondMaxZedKey(MaxZedKey ,SecondMaxZedKey);
        IsConsolidatedOrNot(BreakConSolidateDateForCurrentDate,MaxZedKey,SecondMaxZedKey);
        GetMaxZKeyFromArcMallExport(MaxZedKey);
        GetOldAndNewGrandTotal(MaxZedKey,oldgrandtotal,Newgrandtotal);
        EodCounter = TDbMegaWorldMall::GetCurrentControlNumber(MaxZedKey);
        int BatchId = 1;

        if((!BreakConSolidateDateForCurrentDate || TGlobalSettings::Instance().BatchNo > 1) && prepareZeroDateForDaily)
        {
            BatchId = TGlobalSettings::Instance().BatchNo;
        }

        for(int i = 0;i < BatchId; i++)
        {
           query->Close();
           query->SQL->Text = "SELECT  *  FROM ARCMALLEXPORT A "
                              "WHERE A.Z_KEY > :MIN_ZED And A.Z_KEY <= :MAX_ZED ";

           query->ParamByName("MIN_ZED")->AsInteger = MaxZedKey-i-1;
           query->ParamByName("MAX_ZED")->AsInteger = MaxZedKey-i;
           query->ExecQuery();

           if(!query->Eof)
           {
                grosssales += query->FieldByName("GROSS_SALES")->AsCurrency;
                vatexemptSales += query->FieldByName("VATEXEMPT_SALES")->AsCurrency;
                ScdAmount += query->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
                RegdiscountAmount += query->FieldByName("REGDISCOUNT_AMOUNT")->AsCurrency;
                RefundAmount += query->FieldByName("REFUND_AMOUNT")->AsCurrency;
                VatSales += query->FieldByName("VAT_SALES")->AsCurrency;
                SchargeAmount += query->FieldByName("SCHARGE_AMOUNT")->AsCurrency;
                DailySales += query->FieldByName("DAILY_SALES")->AsCurrency;
                CashSales += query->FieldByName("CASH_SALES")->AsCurrency;
                CardSales += query->FieldByName("CARD_SALES")->AsCurrency;
                OtherSales += query->FieldByName("OTHER_SALES")->AsCurrency;
                VoidAmount += query->FieldByName("VOID_AMOUNT")->AsCurrency;
                FineDineCustCount += query->FieldByName("FINEDINECUST_COUNT")->AsInteger;
                TransactionCount += query->FieldByName("TRANSACTION_COUNT")->AsInteger;
                salestype_food += query->FieldByName("SALESTYPE_FOOD")->AsCurrency;
                salestype_Nonfood += query->FieldByName("SALESTYPE_NONFOOD")->AsCurrency;
                salestype_Groceries += query->FieldByName("SALESTYPE_GROCERIES")->AsCurrency;
                salestype_Medicines += query->FieldByName("SALESTYPE_MEDICINES")->AsCurrency;
                salestype_others += query->FieldByName("SALESTYPE_OTHERS")->AsCurrency;
            }
       }

       WriteDataAccordingToDifferentSalesTotal("04",oldgrandtotal);
       WriteDataAccordingToDifferentSalesTotal("05",Newgrandtotal);
       WriteDataAccordingToDifferentSalesTotal("06",grosssales);
       WriteDataAccordingToDifferentSalesTotal("07",vatexemptSales);
       WriteDataAccordingToDifferentSalesTotal("08",ScdAmount);
       WriteDataAccordingToDifferentSalesTotal("09",RegdiscountAmount);
       WriteDataAccordingToDifferentSalesTotal("10",RefundAmount);
       WriteDataAccordingToDifferentSalesTotal("11",VatSales);
       WriteDataAccordingToDifferentSalesTotal("12",SchargeAmount);
       WriteDataAccordingToDifferentSalesTotal("13",DailySales);
       WriteDataAccordingToDifferentSalesTotal("14",CashSales);
       WriteDataAccordingToDifferentSalesTotal("15",CardSales);
       WriteDataAccordingToDifferentSalesTotal("16",OtherSales);
       WriteDataAccordingToDifferentSalesTotal("17",VoidAmount);

       WriteDataAccordingToDifferentCountType("18", FineDineCustCount);
       WriteDataAccordingToDifferentCountType("19", EodCounter);
       WriteDataAccordingToDifferentCountType("20", TransactionCount);

       query0->Close();
       query0->SQL->Text = "SELECT STIR.SALES_TYPE_KEY FROM SALES_TYPE_ITEMS_RELATION STIR "
                            "GROUP BY STIR.SALES_TYPE_KEY";
       query0->ExecQuery();

        while(!query0->Eof)
        {
            int SalesTypeDB = query0->FieldByName("SALES_TYPE_KEY")->AsInteger;

            query1->Close();
            query1->SQL->Text = "SELECT ST.SALES_TYPE_NAME FROM SALES_TYPE ST WHERE ST.SALES_TYPE_KEY = :SALES_KEY";
            query1->ParamByName("SALES_KEY")->AsInteger = SalesTypeDB;
            query1->ExecQuery();
            UnicodeString SalesTypeValue = "SalesType" + query1->FieldByName("SALES_TYPE_NAME")->AsString;
            AvailableSalesType.push_back(SalesTypeValue.t_str());
            query0->Next();
        }
        DBTransaction.Commit();

        int SalesTypeSize = AvailableSalesType.size();

        for(int j=0; j < SalesTypeSize; j++)
        {
            if(AvailableSalesType.at(j) == FOOD)
                WriteDataAccordingToSalesType("2101", salestype_food, j, SalesTypeSize);

            else if(AvailableSalesType.at(j) == NONFOOD)
                WriteDataAccordingToSalesType("2102", salestype_Nonfood, j, SalesTypeSize);

            else if(AvailableSalesType.at(j) == GROCERIES)
                WriteDataAccordingToSalesType("2103", salestype_Groceries, j, SalesTypeSize);

            else if(AvailableSalesType.at(j) == MEDICINES)
                WriteDataAccordingToSalesType("2104", salestype_Medicines, j, SalesTypeSize);

            else if(AvailableSalesType.at(j) == OTHERS)
                WriteDataAccordingToSalesType("2105", salestype_others, j, SalesTypeSize );

        }
        setExportResponse(
               true,               // Succesful = false
               emrExportSuccesful, // Result = emrExportSuccesful
               "",                 // Message
               "",                 // Description
               result );
    }
    catch(Exception &ex)
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }

    return result;
}

//---------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::PrepareDiscounts(TDateTime DateFieldInDailyData)
{

        TMegaworldDiscount Discounted_data;
        int MaxZedKey;
        int SecondMaxZedKey;
        TDateTime DateValueForSecondMaxZed ;
        UnicodeString DiscountName = "";
        UnicodeString DiscountDesc = "";
        Currency DiscountTotal = 0;
        UnicodeString discountname = "";
        UnicodeString discountdesc = "";
        Currency discounttotal = 0;
        UnicodeString Format = ",";
        Currency DiscountValue = 0;
        UnicodeString OutputValue = "";
        TExportResponse result;

        std::map<pair<UnicodeString,UnicodeString>, Currency> StoreAllValue;
        std::map<pair<UnicodeString,UnicodeString>, Currency>::iterator itrmap;
        StoreAllValue.clear();

        std::vector<TMegaworldDiscount> AvailableDiscountName;
        std::vector<TMegaworldDiscount>::iterator itr;
        AvailableDiscountName.clear() ;
        DataToWrite.clear();

        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

        GetMaxZedKeyAndSecondMaxZedKey(MaxZedKey ,SecondMaxZedKey);
        bool BreakConSolidateDateForCurrentDate;
        IsConsolidatedOrNot(BreakConSolidateDateForCurrentDate,MaxZedKey,SecondMaxZedKey);
        GetMaxZKeyFromArcMallExport(MaxZedKey);
        try
        {

            if(!BreakConSolidateDateForCurrentDate  || TGlobalSettings::Instance().BatchNo > 1)
            {

                for(int i = 0;i<TGlobalSettings::Instance().BatchNo;i++)
                {
                   query->Close();

                   query->SQL->Text =  "SELECT A.NAME,A.DESCRIPTION,SUM(A.DISCOUNTED_VALUE) AS DISC_VAL "
                                        "FROM ARCORDERDISCOUNTS A inner join ARCHIVE B on "
                                        "A.ARCHIVE_KEY = B.ARCHIVE_KEY inner join ARCBILL C on "
                                        "C.ARCBILL_KEY = B.ARCBILL_KEY "
                                        "WHERE C.Z_KEY > :MIN_ZED And C.Z_KEY <= :MAX_ZED "
                                        "GROUP BY A.NAME,A.DESCRIPTION ";

                   query->ParamByName("MIN_ZED")->AsInteger = MaxZedKey-i-1;
                   query->ParamByName("MAX_ZED")->AsInteger = MaxZedKey-i;
                   query->ExecQuery();
                   while(!query->Eof)
                   {

                       DiscountName = query->FieldByName("NAME")->AsString;
                       DiscountDesc = query->FieldByName("DESCRIPTION")->AsString;
                       DiscountTotal = fabs(query->FieldByName("DISC_VAL")->AsCurrency);
                        Discounted_data.Discount_Name = query->FieldByName("NAME")->AsString;
                        Discounted_data.Description = query->FieldByName("DESCRIPTION")->AsString;
                        Discounted_data.Discount_Value = fabs(query->FieldByName("DISC_VAL")->AsCurrency);

                       if(AvailableDiscountName.size() == 0)
                       {
                         AvailableDiscountName.push_back(Discounted_data);
                       }
                       else
                       {
                         bool isAdded = false;
                          for (itr = AvailableDiscountName.begin(); itr!=AvailableDiscountName.end(); itr++)
                          {

                             if(itr->Discount_Name == DiscountName && itr->Description == DiscountDesc)
                             {


                                itr->Discount_Value += DiscountTotal;
                                DiscountTotal = itr->Discount_Value;

                               isAdded = true;
                               break;
                             }
                          }
                          if(!isAdded)
                          {

                                 AvailableDiscountName.push_back(Discounted_data);
                          }
                       }
                       query->Next();

                      StoreAllValue[make_pair(DiscountName,DiscountDesc)] = DiscountTotal;
                   }

             }
             for (itrmap = StoreAllValue.begin(); itrmap!=StoreAllValue.end(); itrmap++)
             {
                DiscountName = (itrmap->first).first;
                DiscountDesc = (itrmap->first).second;
                DiscountTotal = itrmap->second;
                OutputValue= DiscountName + Format + DiscountDesc + Format + DiscountTotal + "\n";
                DataToWrite.push_back(OutputValue.t_str());
             }
            }
            else
            {
                  query->Close();

                  query->SQL->Text =  "SELECT A.NAME,A.DESCRIPTION,SUM(A.DISCOUNTED_VALUE) AS DISC_VAL "
                                        "FROM ARCORDERDISCOUNTS A inner join ARCHIVE B on "
                                        "A.ARCHIVE_KEY = B.ARCHIVE_KEY inner join ARCBILL C on "
                                        "C.ARCBILL_KEY = B.ARCBILL_KEY "
                                        "WHERE C.Z_KEY = :MAX_ZED "
                                        "GROUP BY A.NAME,A.DESCRIPTION ";

                   query->ParamByName("MAX_ZED")->AsInteger = MaxZedKey;


                   query->ExecQuery();
                   DiscountName = query->FieldByName("NAME")->AsString;

                   while(!query->Eof)
                   {
                        DiscountName = query->FieldByName("NAME")->AsString;
                        DiscountDesc = query->FieldByName("DESCRIPTION")->AsString;
                        DiscountValue = fabs(query->FieldByName("DISC_VAL")->AsCurrency);
                        DiscountTotal = FixDecimalPlaces(DiscountValue);
                        OutputValue= DiscountName + Format + DiscountDesc + Format + DiscountTotal + "\n";
                        DataToWrite.push_back(OutputValue.t_str());
                        query->Next();
                    }

            }
            DBTransaction.Commit();
            setExportResponse(
               true,               // Succesful = false
               emrExportSuccesful, // Result = emrExportSuccesful
               "",                 // Message
               "",                 // Description
               result );
        }
        catch(Exception &ex)
        {
            setExportResponse(
               false,           // Succesful = false
               emrExportFailed, // Result = emrExportFailed
               "",              // Message
               "",              // Description
               result );
        }
        return result;
}
//---------------------------------------------------------------------------

bool TMallExportMegaworldMall::IsConsolidatedOrNot(bool &IsBreakConSolidateDateForCurrentDate , int MaxZedKey ,int SecondMaxZed)
{
       TDateTime DateValue;
       UnicodeString DateForMaxZed = "";
       UnicodeString DateForSecondMaxZed = "";
       GetFirstDateValueForMaxZedAndSecondMaxZed(DateValue,DateForMaxZed,DateForSecondMaxZed,MaxZedKey,SecondMaxZed) ;
       if(DateForMaxZed == DateForSecondMaxZed)
       {
        IsBreakConSolidateDateForCurrentDate = false;
       }
       else
       {

        IsBreakConSolidateDateForCurrentDate = true;
       }
}
//---------------------------------------------------------------------------
void TMallExportMegaworldMall::GetMaxZedKeyAndSecondMaxZedKey(int &maxzed, int &maxzed2)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* MaxZedQuery = DBTransaction.Query(DBTransaction.AddQuery());
    MaxZedQuery->Close();

    MaxZedQuery->SQL->Text =  "SELECT MAX(a.Z_KEY) Z_KEY "
                        "FROM ARCMALLEXPORTHOURLY a " ;

    MaxZedQuery->ExecQuery();

    maxzed = MaxZedQuery->Fields[0]->AsInteger;

    MaxZedQuery->Close();

    MaxZedQuery->SQL->Text =  "SELECT MAX(a.Z_KEY) "
                        "FROM ARCMALLEXPORTHOURLY a "
                        "WHERE a.Z_KEY < :Max_Zed_Key ";

    MaxZedQuery->ParamByName("Max_Zed_Key")->AsInteger = maxzed;
    MaxZedQuery->ExecQuery();

    maxzed2 = MaxZedQuery->Fields[0]->AsInteger;

    MaxZedQuery->Close();
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TMallExportMegaworldMall::GetOldAndNewGrandTotal(int maxzed, Currency &oldgrandtotal , Currency &newgrandtotal)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* IBQuery = DBTransaction.Query(DBTransaction.AddQuery());

    IBQuery->Close();

    IBQuery->SQL->Text =  "SELECT a.OLD_GRANDTOTAL,a.NEW_GRANDTOTAL "
                        "FROM ARCMALLEXPORT a "
                        "WHERE a.Z_KEY = :MaxZed ";

    IBQuery->ParamByName("MaxZed")->AsInteger = maxzed;

    IBQuery->ExecQuery();
    if(IBQuery->RecordCount)
    {
        oldgrandtotal = IBQuery->Fields[0]->AsCurrency;
        newgrandtotal = IBQuery->Fields[1]->AsCurrency;
    }

    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void TMallExportMegaworldMall::GetFirstDateValueForMaxZedAndSecondMaxZed(TDateTime &DateValue, UnicodeString &MaxZedDate,UnicodeString &MinZedDate,int MaxZedKey,int MinZedKey)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();

    query->SQL->Text =  "SELECT FIRST 1 a.DATE_VALUE "
                        "FROM ARCMALLEXPORTHOURLY a "
                        "WHERE a.Z_KEY = :Max_Zed "
                        "ORDER BY a.AME_HOURLY_KEY ";

    query->ParamByName("Max_Zed")->AsInteger = MaxZedKey;
    query->ExecQuery();

    DateValue = query->Fields[0]->AsDate;
    MaxZedDate = DateValue.FormatString("mm/dd/yyyy");

    query->Close();

    query->SQL->Text =  "SELECT FIRST 1 a.DATE_VALUE "
                        "FROM ARCMALLEXPORTHOURLY a "
                        "WHERE a.Z_KEY = :Second_Max_Zed "
                        "ORDER BY a.AME_HOURLY_KEY ";

    query->ParamByName("Second_Max_Zed")->AsInteger = MinZedKey;
    query->ExecQuery();

    DateValue = query->Fields[0]->AsDate;
    MinZedDate = DateValue.FormatString("mm/dd/yyyy");

    DBTransaction.Commit();

}
//---------------------------------------------------------------------------
int TMallExportMegaworldMall::GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                      UnicodeString &DateValue, UnicodeString &TimeValue,
                                                      int &TransactionTotal, UnicodeString &Amount, int &PatronCount)
{
    Currency AmountValue = 0;
    int transactioncount = 0;
    int Patroncount = 0;
    TDateTime Currentime = Now();
    UnicodeString PresentDateTime = Currentime.FormatString("mmddyyyy");
    bool BreakConSolidateDateForCurrentDate = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    int MaxZedKey;
    int SecondMaxZedKey;
    TDateTime DateValueForHourly ;
    GetMaxZedKeyAndSecondMaxZedKey(MaxZedKey ,SecondMaxZedKey);
    IsConsolidatedOrNot(BreakConSolidateDateForCurrentDate,MaxZedKey,SecondMaxZedKey);
    GetMaxZKeyFromArcMallExport(MaxZedKey);
    if(!BreakConSolidateDateForCurrentDate  || TGlobalSettings::Instance().BatchNo > 1)
    {
      for(int i = 0;i<TGlobalSettings::Instance().BatchNo;i++)
      {
          query->Close();
          query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM, SUM(PATRON_COUNT) AS PATRONSUM FROM ARCMALLEXPORTHOURLY A "
                           "WHERE A.TIME_VALUE=:TIMEVALUE AND A.Z_KEY > :MIN_ZED And A.Z_KEY <= :MAX_ZED ";


           query->ParamByName("TIMEVALUE")->AsString = TimeValue;
           query->ParamByName("MIN_ZED")->AsInteger = MaxZedKey-i-1;
           query->ParamByName("MAX_ZED")->AsInteger = MaxZedKey-i;

          query->ExecQuery();


          transactioncount += query->FieldByName("TRANSSUM")->AsInteger;
          AmountValue += RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
          Patroncount += query->FieldByName("PATRONSUM")->AsInteger;

      }

    }
    else
    {
             query->Close();

              query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM, SUM(PATRON_COUNT) AS PATRONSUM FROM ARCMALLEXPORTHOURLY A "
                       "WHERE A.TIME_VALUE=:TIMEVALUE AND A.Z_KEY = :MAX_ZED ";


              query->ParamByName("TIMEVALUE")->AsString = TimeValue;
              query->ParamByName("MAX_ZED")->AsInteger = MaxZedKey;


              query->ExecQuery();
              AmountValue = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
              transactioncount = query->FieldByName("TRANSSUM")->AsInteger;

              Patroncount = query->FieldByName("PATRONSUM")->AsInteger;

    }

    Amount = FixDecimalPlaces(AmountValue);
    TransactionTotal= transactioncount;
    PatronCount=Patroncount ;
    DBTransaction.Commit();
    query->Close();

    return TransactionTotal;

}

//---------------------------------------------------------------------------
void TMallExportMegaworldMall::CheckTransactionDoneBeforeZed(TDateTime &dateValueField, int &zeroZCount, bool &zeroZDoneBeforeAnySale, bool &prepareDataForZeroSales)
{
    try
    {
        int maxZKeyFromArcMallExportHourly;
        int maxZKeyFromZed;
        int zCount;
        int zCount2;
        TDateTime maxZDateFromZed;
        TDateTime maxZDateFromArcMallExportHourly;
        UnicodeString minZedDate = "";
        UnicodeString maxZedDate = "";
        TDateTime zDate;
        LoadMaxZkeyAndDateFromZed(maxZKeyFromZed, maxZDateFromZed);
        maxZedDate =  maxZDateFromZed.FormatString("mm/dd/yyyy");
        LoadMaxZkeyAndDateFromArcMallExportHourly(maxZKeyFromArcMallExportHourly, maxZDateFromArcMallExportHourly);
        minZedDate = maxZDateFromArcMallExportHourly.FormatString("mm/dd/yyyy");
        zeroZCount = GetZCountForZedsDoneBeforeAnySale(maxZKeyFromArcMallExportHourly);
        zeroZDoneBeforeAnySale = CheckIfZeroZedDoneBeforeAnySale(maxZKeyFromArcMallExportHourly);

        if((maxZKeyFromZed > maxZKeyFromArcMallExportHourly) && !(SameStr(maxZedDate,minZedDate)))
        {
            dateValueField = maxZDateFromZed;
            prepareDataForZeroSales = true;
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//---------------------------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::PrepareZeroSalesDateForHourly(TDateTime DateFieldInHourlyData)
{
    TExportResponse result;
    UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
    UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
    UnicodeString DateValue = DateFieldInHourlyData.FormatString("mmddyyyy");
    UnicodeString OutputValue;
    try
    {
        OutputValue = "01" + TenantName + "\n" + "02" + TerminalNum + "\n" + "03" + DateValue + "\n" + "04" + "0" + "\n" +
                      "05" + "00" + "\n" + "06" + "00" + "\n" + "07" + "0" + "\n" + "08" + "00" + "\n" + "09" + "00" + "\n" + "10" + "0";
        DataToWrite.push_back(OutputValue.t_str());

        setExportResponse(
               true,               // Succesful = false
               emrExportSuccesful, // Result = emrExportSuccesful
               "",                 // Message
               "",                 // Description
               result );

    }

    catch(Exception &ex)
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }

    return result;

}
//---------------------------------------------------------------------------
TExportResponse TMallExportMegaworldMall::PrepareZeroSalesDiscounts(TDateTime DateFieldInDailyData)
{
    UnicodeString DiscountName = "";
    UnicodeString DiscountDesc = "";
    Currency DiscountTotal = 0;
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";
    TExportResponse result;
    DataToWrite.clear();
    try
    {
        OutputValue = DiscountName + Format + DiscountDesc + Format + DiscountTotal + "\n";
        DataToWrite.push_back(OutputValue.t_str());


        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );
    }
    catch(Exception &ex)
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }
    return result;
}
//---------------------------------------------------------------------------
void TMallExportMegaworldMall::GetMaxZKeyFromArcMallExport(int &maxzed)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL* MaxZedQuery = DBTransaction.Query(DBTransaction.AddQuery());
        MaxZedQuery->Close();

        MaxZedQuery->SQL->Text =  "SELECT MAX(a.Z_KEY) Z_KEY "
                                  "FROM ARCMALLEXPORT a " ;

        MaxZedQuery->ExecQuery();

        maxzed = MaxZedQuery->Fields[0]->AsInteger;

        MaxZedQuery->Close();
        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//----------------------------------------------------------------------------
void TMallExportMegaworldMall::LoadMaxZkeyAndDateFromZed(int &maxZKeyFromZed, TDateTime &maxZDateFromZed)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL* MaxZKeyFromZedQuery = DBTransaction.Query(DBTransaction.AddQuery());
        MaxZKeyFromZedQuery->Close();

        MaxZKeyFromZedQuery->SQL->Text =  "SELECT FIRST 1 a.Z_KEY, a.TIME_STAMP "
                                          "FROM ZEDS a "
                                          "ORDER BY a.Z_KEY DESC " ;
        MaxZKeyFromZedQuery->ExecQuery();
        maxZKeyFromZed = MaxZKeyFromZedQuery->Fields[0]->AsInteger;
        maxZDateFromZed = MaxZKeyFromZedQuery->Fields[1]->AsDate;

        MaxZKeyFromZedQuery->Close();
        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//----------------------------------------------------------------------------
void TMallExportMegaworldMall::LoadMaxZkeyAndDateFromArcMallExportHourly(int &maxZKeyFromArcMallExportHourly, TDateTime &maxZDateFromArcMallExportHourly)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {

        TIBSQL* MaxZKeyFromArcMallExportHourly = DBTransaction.Query(DBTransaction.AddQuery());
        MaxZKeyFromArcMallExportHourly->Close();

        MaxZKeyFromArcMallExportHourly->SQL->Text =  "SELECT FIRST 1 a.Z_KEY, a.DATE_VALUE "
                                                     "FROM ARCMALLEXPORTHOURLY a "
                                                     "ORDER BY a.Z_KEY DESC " ;
        MaxZKeyFromArcMallExportHourly->ExecQuery();
        maxZKeyFromArcMallExportHourly = MaxZKeyFromArcMallExportHourly->Fields[0]->AsInteger;
        maxZDateFromArcMallExportHourly = MaxZKeyFromArcMallExportHourly->Fields[1]->AsDate;

        MaxZKeyFromArcMallExportHourly->Close();
        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//----------------------------------------------------------------------------
int TMallExportMegaworldMall::GetZCountForZedsDoneBeforeAnySale(int maxZKeyFromArcMallExportHourly)
{
    int zCount = 0;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL* ZCountFromZedQuery = DBTransaction.Query(DBTransaction.AddQuery());
        ZCountFromZedQuery->Close();

        ZCountFromZedQuery->SQL->Text =  "SELECT COUNT(a.Z_KEY) "
                                         "FROM ZEDS a "
                                         "WHERE a.TIME_STAMP >= :CURRENT_DATE AND a.Z_KEY > :MAX_ZED_FROM_HOURLY " ;
        ZCountFromZedQuery->ParamByName("CURRENT_DATE")->AsDate = Now().CurrentDate();
        ZCountFromZedQuery->ParamByName("MAX_ZED_FROM_HOURLY")->AsInteger = maxZKeyFromArcMallExportHourly;
        ZCountFromZedQuery->ExecQuery();
        zCount = ZCountFromZedQuery->Fields[0]->AsInteger;

        ZCountFromZedQuery->Close();
        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return zCount;
}
//----------------------------------------------------------------------------
bool TMallExportMegaworldMall::CheckIfZeroZedDoneBeforeAnySale(int maxZKeyFromArcMallExportHourly)
{
    bool zeroZDoneBeforeAnySale = false;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        int zCount;
        TIBSQL* ZCountFromZedQuery = DBTransaction.Query(DBTransaction.AddQuery());
        ZCountFromZedQuery->Close();

        ZCountFromZedQuery->SQL->Text =  "SELECT COUNT(a.Z_KEY) "
                                         "FROM ZEDS a "
                                         "WHERE a.TIME_STAMP >= :CURRENT_DATE AND a.Z_KEY < :MAX_ZED_FROM_HOURLY AND a.TERMINAL_EARNINGS = '0' " ;
        ZCountFromZedQuery->ParamByName("CURRENT_DATE")->AsDate = Now().CurrentDate();
        ZCountFromZedQuery->ParamByName("MAX_ZED_FROM_HOURLY")->AsInteger = maxZKeyFromArcMallExportHourly;
        ZCountFromZedQuery->ExecQuery();
        zCount = ZCountFromZedQuery->Fields[0]->AsInteger;

        ZCountFromZedQuery->Close();
        if(zCount>0)
            zeroZDoneBeforeAnySale = true;

        DBTransaction.Commit();
    }
    catch(Exception &E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
    return zeroZDoneBeforeAnySale;

}
//----------------------------------------------------------------------------------------------
void TMallExportMegaworldMall::WriteDataAccordingToSalesType(UnicodeString saleTypeCode, Currency SaleTypaAmount, int index, int salesTypeSize)
{
    OutputValue = saleTypeCode + "\n";
    DataToWrite.push_back(OutputValue.t_str());
    OutputValue = "22" + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(SaleTypaAmount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2));
    OutputValue = (salesTypeSize == (index+1)) ? OutputValue : OutputValue + "\n";
    DataToWrite.push_back(OutputValue.t_str());
}
//----------------------------------------------------------------------------------------------
void  TMallExportMegaworldMall::WriteDataAccordingToDifferentSalesTotal(UnicodeString salesTotalCode, Currency salesTotalAmount)
{
    OutputValue = salesTotalCode + megaworldExport->RemoveDecimalValue(CurrToStrF((RoundToNearest(salesTotalAmount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown)), ffFixed, 2)) + "\n";
    DataToWrite.push_back(OutputValue.t_str());
}
//----------------------------------------------------------------------------------------------
void  TMallExportMegaworldMall::WriteDataAccordingToDifferentCountType(UnicodeString countTypeCode, int countTotal)
{
    OutputValue = countTypeCode + IntToStr(countTotal) + "\n";
    DataToWrite.push_back(OutputValue.t_str());
}
