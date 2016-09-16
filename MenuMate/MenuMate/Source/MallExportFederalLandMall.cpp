//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportFederalLandMall.h"
#include "MallExportDataManager.h"
#include "MallExportHourlyUpdate.h"
#include "MallExportOtherDetailsUpdate.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "Main.h"
#include <string>
#include <vector>
#include <ios>
#include <windows.h>
#include <dirent.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportFederalLandMall::TMallExportFederalLandMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportFederalLandMall::~TMallExportFederalLandMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportFederalLandMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Delete this method if not needed
TExportResponse TMallExportFederalLandMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        TMallExportUpdateAdaptor exportUpdateAdaptor;
        UnicodeString MallPath = FilePath;
        UnicodeString MallPathFileName = "";
        UnicodeString Query = "";
        Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];
        UnicodeString MonthValue = "";
        UnicodeString DayValue = "";
        UnicodeString FileYear = "";
        UnicodeString MerchantCode = TGlobalSettings::Instance().TenantNo;
        UnicodeString FileName = "";
        UnicodeString Format = ".csv";
        UnicodeString CheckDate = Now().FormatString("mmddyy");
        UnicodeString DateFormat = "";
        TDateTime Yesterday = Now() - 1.0;
        TDateTime DateValue;
        unsigned short Hour = 0;
        unsigned short Minutes = 0;
        unsigned short Seconds = 0;
        unsigned short dummy = 0;

        if(gridBtn->Enabled == false)
        {
            Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

            GenerateFileForDaysPOSNotUsed(MallPath, MerchantCode, CheckDate, Format, DateFormat);

            Database::TDBTransaction DBTransaction(
            TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();

            UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

            TTransactionInfo TransactionInfo;

            TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(DBTransaction, DeviceName);


            TMallExportUpdateAdaptor exportUpdateAdaptor;
            TFinancialDetails FinancialDetails = reportFinancialCalculations->GetFinancialDetails(DBTransaction, TransactionInfo, DeviceName);
            exportUpdateAdaptor.UpdateExportTablesOnZed(FinancialDetails, &DBTransaction);
        }


        // For Daily Sales
        DataQuery = getFederalLandQuery("S");
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename("S", MallPath, MallPathFileName);
        CreateHeaderFormat(MallPathFileName,DataToWrite, "S");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
        DataToWrite.clear();

        CreateOutputFormatModeS(DataRead, DataToWrite);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
        DataToWrite.clear();

        // For Daily Sales Discount
        CreateFilename("D", MallPath, MallPathFileName);
        CreateHeaderFormat(MallPathFileName,DataToWrite, "D");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
        DataToWrite.clear();

        Query = getFederalLandQuery("D");
        CreateOutputFormatModeD(DataRead, DataToWrite, Query);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
        DataToWrite.clear();

        // For Dailu Sales Payment
        CreateFilename("P", MallPath, MallPathFileName);
        CreateHeaderFormat(MallPathFileName,DataToWrite, "P");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
        DataToWrite.clear();

        Query = getFederalLandQuery("P");
        CreateOutputFormatModeP(DataRead, DataToWrite, Query);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
        DataToWrite.clear();

        // For Dailu Sales Refund Cancel
        CreateFilename("R", MallPath, MallPathFileName);
        CreateHeaderFormat(MallPathFileName,DataToWrite, "R");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
        DataToWrite.clear();

        Query = getFederalLandQuery("R");
        CreateOutputFormatModeR(DataRead, DataToWrite, Query);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
        DataToWrite.clear();

        // For Hourly Sales
        CreateFilename("H", MallPath, MallPathFileName);
        CreateHeaderFormat(MallPathFileName,DataToWrite, "H");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
        DataToWrite.clear();

        Query = getFederalLandQuery("H");
        CreateOutputFormatModeH(DataRead, DataToWrite, MallPathFileName, Query);
        DataToWrite.clear();

        frmMain->EnablePOSBtn(true);
        TGlobalSettings::Instance().LastZedTime = Now().FormatString("hh");
        SaveStrVariable(vmLastZedTime, TGlobalSettings::Instance().LastZedTime);

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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS

// Virtual methods
void TMallExportFederalLandMall::initTimingExportResponse( TExportResponse& outResponse )
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

TExportResponse TMallExportFederalLandMall::CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString &MallPathFileName)
{
    TExportResponse result;

    try
    {
        UnicodeString MerchantCode = TGlobalSettings::Instance().TenantNo;
        UnicodeString MonthValue = "";
        UnicodeString DayValue = "";
        UnicodeString Format = ".csv";
        UnicodeString FileYear = Now().FormatString("yy");
        TDateTime DateValue = CheckLastZed();

        UnicodeString CheckEOYFolder = Now().FormatString("mmdd");
        UnicodeString Year = "";
        TDateTime FolderYear;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        if((CheckEOYFolder == "0101") && (Hour < 6))
        {
            FolderYear = Now() - 1.0;
            Year = FolderYear.FormatString("yyyy");
            FileYear = FolderYear.FormatString("yy");
        }
        else
        {
            Year = Now().FormatString("yyyy");
            FileYear = Now().FormatString("yy");
        }

        GetExportInfo(MonthValue, DayValue, DateValue);

        MallPath = MallPath + Year + "\\";

        MallPath = CheckDir(MallPath, DateValue);

        MallPathFileName = MallPath + MerchantCode + MonthValue + DayValue + FileYear + mode + Format;

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

TExportResponse TMallExportFederalLandMall::GetExportInfo(UnicodeString &MonthValue, UnicodeString &DayValue,
                                                          TDateTime DateValue)
{
    TExportResponse result;

    try
    {
        UnicodeString CheckMonth = DateValue.FormatString("mm");
        UnicodeString CheckDay = DateValue.FormatString("dd");

        int Month = StrToInt(CheckMonth);
        int Day = StrToInt(CheckDay);

        Month = StrToInt(CheckMonth);

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

        switch(Day)
        {
            case 10:
                DayValue = "A";
                break;
            case 11:
                DayValue = "B";
                break;
            case 12:
                DayValue = "C";
                break;
            case 13:
                DayValue = "D";
                break;
            case 14:
                DayValue = "E";
                break;
            case 15:
                DayValue = "F";
                break;
            case 16:
                DayValue = "G";
                break;
            case 17:
                DayValue = "H";
                break;
            case 18:
                DayValue = "I";
                break;
            case 19:
                DayValue = "J";
                break;
            case 20:
                DayValue = "K";
                break;
            case 21:
                DayValue = "L";
                break;
            case 22:
                DayValue = "M";
                break;
            case 23:
                DayValue = "N";
                break;
            case 24:
                DayValue = "O";
                break;
            case 25:
                DayValue = "P";
                break;
            case 26:
                DayValue = "Q";
                break;
            case 27:
                DayValue = "R";
                break;
            case 28:
                DayValue = "S";
                break;
            case 29:
                DayValue = "T";
                break;
            case 30:
                DayValue = "U";
                break;
            case 31:
                DayValue = "V";
                break;
            default:
                DayValue = IntToStr(Day);
                break;
        }

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

// This method checks if the prev zed report was not yet generated
// and returns the correspondent datetime
TDateTime TMallExportFederalLandMall::CheckLastZed()
{
    TDateTime result;
    UnicodeString Value = "";
    UnicodeString OutValue = "";
    UnicodeString Month = "";
    UnicodeString Day = "";
    UnicodeString Year = "";
    std::string ValueSTR = "";

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

    query->SQL->Text = "SELECT FIRST 1 * FROM MALLEXPORT_HOURLY";
    DBTransaction.StartTransaction();
    query->ExecQuery();

    if(!query->Eof)
    {
        Value = query->FieldByName("DATE_VALUE")->AsString;
        ValueSTR = Value.t_str();

        for(int i=0;i<Value.Length();i++)
        {
            if(i<4)
            {
                Year = Year + ValueSTR[i];
            }
            else if(i==4 || i==5)
            {
                Month = Month + ValueSTR[i];
            }
            else
            {
                Day = Day + ValueSTR[i];
            }
        }
        OutValue = Month + "/" + Day + "/" + Year;
        result = StrToDateTime(OutValue);
    }
    else
    {
        result = Now();
    }
    DBTransaction.Commit();

    return result;
}
//---------------------------------------------------------------------------

UnicodeString TMallExportFederalLandMall::CheckDir(UnicodeString Path, TDateTime DateValue)
{
    UnicodeString Month = DateValue.FormatString("mm");
    if(!DirectoryExists(Path))
    {
        CreateDir(Path);
    }
    Path = Path + Month + "\\";
    if(!DirectoryExists(Path))
    {
        CreateDir(Path);
    }
    return Path;
}
//---------------------------------------------------------------------------

// This method is used to create the header names for Hourly Export
// This method is separated to prevent multiple pos create multiple header files
void TMallExportFederalLandMall::CreateHeaderFormat(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite, UnicodeString Status)
{
    UnicodeString Store = "";
    UnicodeString Format = ",";
    std::ofstream CreateColumn;
    if(!CreateColumn.is_open())
    {
        if(Status == "S")
        {
            Store = "fdtTrnsctn,fvcMrchntCd,fvcMrcntDsc,fnmGrndTtlOld,fnmGrndTtlNew" + Format +
                    "fnmGTDlySls,fnmGTDscnt,fnmGTDscntSNR,fnmGTDscntPWD" + Format +
                    "fnmGTDscntGPC,fnmGTDscntVIP,fnmGTDscntEMP,fnmGTDscntREG" + Format +
                    "fnmGTDscntOTH,fnmGTRfnd,fnmGTCncld,fnmGTSlsVAT,fnmGTVATSlsInclsv" + Format +
                    "fnmGTVATSlsExclsv,fnmOffclRcptBeg,fnmOffclRcptEnd,fnmGTCntDcmnt,fnmGTCntCstmr" + Format +
                    "fnmGTCntSnrCtzn,fnmGTLclTax,fnmGTSrvcChrg,fnmGTSlsNonVat,fnmGTRwGrss,fnmGTLclTaxDly" + Format +
                    "fvcWrksttnNmbr,fnmGTPymntCSH,fnmGTPymntCRD,fnmGTPymntOTH" + "\n";
                    DataToWrite.push_back(Store.t_str());
        }
        else if(Status == "D")
        {
            Store = "fdtTrnsctn,fvcMrchntCd,fvcDscntCd,fvcDscntPrcntg,fnmDscnt" + Format +
                    "fnmCntDcmnt,fnmCntCstmr,fnmCntSnrCtzn" + "\n";
                    DataToWrite.push_back(Store.t_str());
        }
        else if(Status == "P")
        {
            Store = "fdtTrnsctn,fvcMrchntCd,fvcPymntCd,fvcPymntDsc,fvcPymntCdCLSCd" + Format +
                    "fvcPymntCdCLSDsc,fnmPymnt" + "\n";
                    DataToWrite.push_back(Store.t_str());
        }
        else if(Status == "R")
        {
            Store = "fdtTrnsctn,fvcMrchntCd,fvcRfndCncldCd,fvcRfndCncldRsn,fnmAmt" + Format +
                    "fnmCntDcmnt,fnmCntCstmr,fnmCntSnrCtzn" + "\n";
                    DataToWrite.push_back(Store.t_str());
        }
        else if(Status == "H")
        {
            Store = "fdtTrnsctn,fvcMrchntCd,fvcHRLCd,fnmDlySls,fnmCntDcmnt" + Format +
                    "fnmCntCstmr,fnmCntSnrCtzn" + "\n";
                    DataToWrite.push_back(Store.t_str());
        }
    }
    CreateColumn.close();
}
//---------------------------------------------------------------------------

// This function is for getting query from the DB Table for ZED and HOURLY files
UnicodeString TMallExportFederalLandMall::getFederalLandQuery(UnicodeString mode)
{
    UnicodeString FederalLandQuery = "";

    if(mode == "S")
    {
        FederalLandQuery = "SELECT * FROM FEDERALLAND_MALLVIEW";
    }
    else if(mode == "D")
    {
//        FederalLandQuery = "SELECT DATE_VALUE, TENANT_NAME, DISCOUNT_TYPE, DISCOUNT_PERC, DISCOUNT_AMOUNT, TRANSACTION_COUNT, "
//                           "FINEDINECUST_COUNT, SCDISCOUNT_COUNT FROM MALLEXPORTOTHERDETAILS "
//                           "WHERE TRANSACTION_CODE = 'DISC'";

        FederalLandQuery = "SELECT TENANT_NAME, DISCOUNT_PERC, DISCOUNT_TYPE, SUM(DISCOUNT_AMOUNT) AS DISCOUNT_AMOUNT, SUM(TRANSACTION_COUNT) AS TRANSACTION_COUNT, "
                           "SUM(FINEDINECUST_COUNT) AS FINEDINECUST_COUNT, SUM(SCDISCOUNT_COUNT) AS SCDISCOUNT_COUNT FROM MALLEXPORTOTHERDETAILS "
                           "WHERE TRANSACTION_CODE = 'DISC' GROUP BY TENANT_NAME, DISCOUNT_PERC, DISCOUNT_TYPE";
    }
    else if(mode == "P")
    {
//        FederalLandQuery = "SELECT DATE_VALUE, TENANT_NAME, PAYMENT_CODE, PAYMENT_CODE_DESC, "
//                           "PAYMENT_CLASS, PAYMENT_CLASS_DESC, PAYMENT_AMOUNT FROM MALLEXPORTOTHERDETAILS "
//                           "WHERE TRANSACTION_CODE = 'PAYMENT' AND REFUND_CANCEL_REASON = ''";

        FederalLandQuery = "SELECT TENANT_NAME, PAYMENT_CODE, PAYMENT_CODE_DESC, "
                           "PAYMENT_CLASS, PAYMENT_CLASS_DESC, SUM(PAYMENT_AMOUNT) AS PAYMENT_AMOUNT FROM MALLEXPORTOTHERDETAILS "
                           "WHERE TRANSACTION_CODE = 'PAYMENT' AND REFUND_CANCEL_REASON = '' GROUP BY TENANT_NAME, PAYMENT_CODE, PAYMENT_CODE_DESC, "
                           "PAYMENT_CLASS, PAYMENT_CLASS_DESC";
    }
    else if(mode == "R")
    {
//        FederalLandQuery = "SELECT DATE_VALUE, TENANT_NAME, TRANSACTION_CODE, REFUND_CANCEL_REASON, "
//                           "REFUND_CANCEL_AMOUNT, TRANSACTION_COUNT, FINEDINECUST_COUNT, SCDISCOUNT_COUNT "
//                           "FROM MALLEXPORTOTHERDETAILS WHERE TRANSACTION_CODE = 'CNCLD' OR TRANSACTION_CODE = 'RFND'";

        FederalLandQuery = "SELECT TENANT_NAME, TRANSACTION_CODE, REFUND_CANCEL_REASON, "
                           "SUM(REFUND_CANCEL_AMOUNT) AS REFUND_CANCEL_AMOUNT, SUM(TRANSACTION_COUNT) AS TRANSACTION_COUNT, SUM(FINEDINECUST_COUNT) AS FINEDINECUST_COUNT, SUM(SCDISCOUNT_COUNT) AS SCDISCOUNT_COUNT "
                           "FROM MALLEXPORTOTHERDETAILS WHERE TRANSACTION_CODE = 'CNCLD' OR TRANSACTION_CODE = 'RFND' GROUP BY TENANT_NAME, TRANSACTION_CODE, REFUND_CANCEL_REASON ";
    }
    else if(mode == "H")
    {
        FederalLandQuery = "SELECT * FROM MALLEXPORT_HOURLY WHERE TERMINAL_NAME=:TERMINALNAME "
                           "ORDER BY DATE_VALUE, TIME_VALUE ASC";
    }

    return FederalLandQuery;
}
//---------------------------------------------------------------------------

TExportResponse TMallExportFederalLandMall::CreateOutputFormatModeS(std::map<UnicodeString, UnicodeString> DataRead,
                                                 std::vector<UnicodeString> &DataToWrite)
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";
    UnicodeString TempData = "";

    std::map<UnicodeString, UnicodeString>::iterator it;

    TExportResponse result;

    try
    {
        for(int i=0;i<DataRead.size();i++)
        {
            switch(i)
            {
                case 0:
                    it = DataRead.find("TransactionDate");
                    break;
                case 1:
                    it = DataRead.find("TenantNo");
                    break;
                case 2:
                    it = DataRead.find("MallCode");
                    break;
                case 3:
                    it = DataRead.find("GrandTotalOld");
                    break;
                case 4:
                    it = DataRead.find("GrandTotal");
                    break;
                case 5:
                    it = DataRead.find("DailySales");
                    break;
                case 6:
                    it = DataRead.find("TotalDiscount");
                    break;
                case 7:
                    it = DataRead.find("TotalSeniorCitizenDiscount");
                    break;
                case 8:
                    it = DataRead.find("DisabilityDiscount");
                    break;
                case 9:
                    it = DataRead.find("GPCDiscount");
                    break;
                case 10:
                    it = DataRead.find("VIPDiscount");
                    break;
                case 11:
                    it = DataRead.find("EmployeeDiscount");
                    break;
                case 12:
                    it = DataRead.find("TotalRegularDiscount");
                    break;
                case 13:
                    it = DataRead.find("DiscountG1");
                    break;
                case 14:
                    it = DataRead.find("TotalRefund");
                    break;
                case 15:
                    it = DataRead.find("TotalCancelled");
                    break;
                case 16:
                    it = DataRead.find("TotalSaleTax");
                    break;
                case 17:
                    it = DataRead.find("TaxInclusiveSales");
                    break;
                case 18:
                    it = DataRead.find("TaxExemptSales");
                    break;
                case 19:
                    it = DataRead.find("BeginningInvoiceNo");
                    break;
                case 20:
                    it = DataRead.find("EndingInvoiveNo");
                    break;
                case 21:
                    it = DataRead.find("TransactionCount");
                    break;
                case 22:
                    it = DataRead.find("FineDiningCustomerCount");
                    break;
                case 23:
                    it = DataRead.find("TotalSeniorCitizenDiscountCount");
                    break;
                case 24:
                    it = DataRead.find("LocalTaxes");
                    break;
                case 25:
                    it = DataRead.find("TotalServiceCharge");
                    break;
                case 26:
                    it = DataRead.find("NonVatSales");
                    break;
                case 27:
                    it = DataRead.find("TotalGrossSale");
                    break;
                case 28:
                    it = DataRead.find("LocalTaxExemptDailySales");
                    break;
                case 29:
                    it = DataRead.find("TerminalNo");
                    break;
                case 30:
                    it = DataRead.find("CashSales");
                    break;
                case 31:
                    it = DataRead.find("CardSales");
                    break;
                case 32:
                    it = DataRead.find("OtherSales");
                    break;
                default:
                    break;
            }

            if(i==0)
            {
                TDateTime DateTime;
                DateTime = StrToDateTime(it->second);
                TempData = DateTime.FormatString("mm/dd/yyyy") + Format;
            }
            else if((i>=3 && i<=18) || (i>=24 && i<=28) || (i>=30 && i<=31))
            {
                TempData = "" + FixDecimalPlaces(StrToCurr(it->second)) + "" + Format;
            }
            else if(i == (DataRead.size() - 1))
            {
                TempData = FixDecimalPlaces(StrToCurr(it->second)) + "\n";
            }
            else
            {
                TempData = (it->second) + Format;
            }

            OutputValue += TempData;
        }
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

TExportResponse TMallExportFederalLandMall::CreateOutputFormatModeD(std::map<UnicodeString, UnicodeString> DataRead,
                                                 std::vector<UnicodeString> &DataToWrite, UnicodeString Query)
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";

    TDateTime Date;
    UnicodeString DateValue = "";
    UnicodeString TenantName = "";
    UnicodeString DiscountTypeCode = "";
    Currency DiscountPerc = 0;
    Currency DiscountAmount = 0;
    int DocumentCount = 0;
    int CustomerCount = 0;
    int SCDCount = 0;

    DataToWrite.clear();

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());
        DBTransaction.StartTransaction();

        query1->SQL->Text = "SELECT FIRST 1 DATE_VALUE FROM MALLEXPORTOTHERDETAILS WHERE TRANSACTION_CODE = 'DISC'";
        query1->ExecQuery();
        if(!query1->Eof)
        {
            Date = query1->FieldByName("DATE_VALUE")->AsDateTime;
            DateValue = Date.FormatString("mm/dd/yyyy");
        }
        query1->Close();
//        query->SQL->Text = "SELECT DATE_VALUE, TENANT_NAME, DISCOUNT_TYPE, DISCOUNT_PERC, DISCOUNT_AMOUNT, TRANSACTION_COUNT, "
//                           "FINEDINECUST_COUNT, SCDISCOUNT_COUNT FROM MALLEXPORTOTHERDETAILS "
//                           "WHERE TRANSACTION_CODE = 'DISC'";
        query->SQL->Text = Query;
        query->ExecQuery();

        while(!query->Eof)
        {
            TenantName = query->FieldByName("TENANT_NAME")->AsString;
            DiscountTypeCode = query->FieldByName("DISCOUNT_TYPE")->AsString;
            DiscountPerc = query->FieldByName("DISCOUNT_PERC")->AsCurrency;
            DiscountAmount = query->FieldByName("DISCOUNT_AMOUNT")->AsCurrency;
            DocumentCount = query->FieldByName("TRANSACTION_COUNT")->AsInteger;
            CustomerCount = query->FieldByName("FINEDINECUST_COUNT")->AsInteger;
            SCDCount = query->FieldByName("SCDISCOUNT_COUNT")->AsInteger;

            OutputValue = DateValue + Format + TenantName + Format + DiscountTypeCode + Format +
                          FixDecimalPlaces(DiscountPerc) + Format + "" + FixDecimalPlaces(DiscountAmount) + "" + Format + DocumentCount + Format +
                          CustomerCount + Format + SCDCount + "\n";
            DataToWrite.push_back(OutputValue.t_str());
            query->Next();
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

TExportResponse TMallExportFederalLandMall::CreateOutputFormatModeP(std::map<UnicodeString, UnicodeString> DataRead,
                                                 std::vector<UnicodeString> &DataToWrite, UnicodeString Query)
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";

    TDateTime Date;
    UnicodeString DateValue = "";
    UnicodeString TenantName = "";
    UnicodeString PaymentCode = "";
    UnicodeString PaymentCodeDesc = "";
    UnicodeString PaymentClass = "";
    UnicodeString PaymentClassDesc = "";
    Currency PaymentAmount = 0;

    std::map<UnicodeString, UnicodeString>::iterator it;

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());
        DBTransaction.StartTransaction();

        query1->SQL->Text = "SELECT FIRST 1 DATE_VALUE FROM MALLEXPORTOTHERDETAILS WHERE TRANSACTION_CODE = 'PAYMENT' ";
        query1->ExecQuery();
        if(!query1->Eof)
        {
            Date = query1->FieldByName("DATE_VALUE")->AsDateTime;
            DateValue = Date.FormatString("mm/dd/yyyy");
        }
        query1->Close();

//        query->SQL->Text = "SELECT DATE_VALUE, TENANT_NAME, PAYMENT_CODE, PAYMENT_CODE_DESC, "
//                           "PAYMENT_CLASS, PAYMENT_CLASS_DESC, PAYMENT_AMOUNT FROM MALLEXPORTOTHERDETAILS "
//                           "WHERE TRANSACTION_CODE = 'PAYMENT' AND REFUND_CANCEL_REASON = ''";
        query->SQL->Text = Query;
        query->ExecQuery();

        while(!query->Eof)
        {
            TenantName = query->FieldByName("TENANT_NAME")->AsString;
            PaymentCode = query->FieldByName("PAYMENT_CODE")->AsString;
            PaymentCodeDesc = query->FieldByName("PAYMENT_CODE_DESC")->AsString;
            PaymentClass = query->FieldByName("PAYMENT_CLASS")->AsString;
            PaymentClassDesc = query->FieldByName("PAYMENT_CLASS_DESC")->AsString;
            PaymentAmount = query->FieldByName("PAYMENT_AMOUNT")->AsCurrency;

            OutputValue = DateValue + Format + TenantName + Format + PaymentCode + Format +
                          PaymentCodeDesc + Format + PaymentClass + Format + PaymentClassDesc +
                          Format + "" + FixDecimalPlaces(PaymentAmount) + "" + "\n";
            DataToWrite.push_back(OutputValue.t_str());
            query->Next();
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

TExportResponse TMallExportFederalLandMall::CreateOutputFormatModeR(std::map<UnicodeString, UnicodeString> DataRead,
                                                 std::vector<UnicodeString> &DataToWrite, UnicodeString Query)
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";

    TDateTime Date;
    UnicodeString DateValue = "";
    UnicodeString TenantName = "";
    UnicodeString RCCode = "";
    UnicodeString RCReason = "";
    Currency RCAmount = 0;
    int DocumentCount = 0;
    int CustomerCount = 0;
    int SCDCount = 0;

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());
        DBTransaction.StartTransaction();

        query1->SQL->Text = "SELECT FIRST 1 DATE_VALUE FROM MALLEXPORTOTHERDETAILS WHERE TRANSACTION_CODE = 'RFND'";
        query1->ExecQuery();
        if(!query1->Eof)
        {
            Date = query1->FieldByName("DATE_VALUE")->AsDateTime;
            DateValue = Date.FormatString("mm/dd/yyyy");
        }
        query1->Close();

//        query->SQL->Text = "SELECT DATE_VALUE, TENANT_NAME, TRANSACTION_CODE, REFUND_CANCEL_REASON, "
//                           "REFUND_CANCEL_AMOUNT, TRANSACTION_COUNT, FINEDINECUST_COUNT, SCDISCOUNT_COUNT "
//                           "FROM MALLEXPORTOTHERDETAILS WHERE TRANSACTION_CODE = 'CNCLD' OR TRANSACTION_CODE = 'RFND'";
        query->SQL->Text = Query;
        query->ExecQuery();

        while(!query->Eof)
        {
            TenantName = query->FieldByName("TENANT_NAME")->AsString;
            RCCode = query->FieldByName("TRANSACTION_CODE")->AsString;
            RCReason = query->FieldByName("REFUND_CANCEL_REASON")->AsString;
            RCAmount = query->FieldByName("REFUND_CANCEL_AMOUNT")->AsCurrency;
            DocumentCount = query->FieldByName("TRANSACTION_COUNT")->AsInteger;
            CustomerCount = query->FieldByName("FINEDINECUST_COUNT")->AsInteger;
            SCDCount = query->FieldByName("SCDISCOUNT_COUNT")->AsInteger;

            OutputValue = DateValue + Format + TenantName + Format + RCCode + Format +
                          RCReason + Format + "" + FixDecimalPlaces(RCAmount) + "" + Format + DocumentCount + Format +
                          CustomerCount + Format + SCDCount + "\n";
            DataToWrite.push_back(OutputValue.t_str());
            query->Next();
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

TExportResponse TMallExportFederalLandMall::CreateOutputFormatModeH(std::map<UnicodeString, UnicodeString> DataRead,
                                                 std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName, UnicodeString Query)
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";
    UnicodeString TempVal = "";

    UnicodeString TerminalNum = "";
    UnicodeString TerminalName = "";
    UnicodeString TenantName = "";
    UnicodeString DateValue = Now().FormatString("yyyymmdd");
    UnicodeString TimeValue = "";
    UnicodeString MinuteValue = "";
    int SCDiscountCount = 0;
    int PatronCount = 0;
    Currency AmountValue = 0;
    int TransactionTotal = 0;
    UnicodeString Amount = "";

    std::map<UnicodeString, UnicodeString>::iterator it;

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

        TerminalNum = TGlobalSettings::Instance().TerminalNo;

//        query->SQL->Text = "SELECT * FROM MALLEXPORT_HOURLY WHERE TERMINAL_NAME=:TERMINALNAME "
//                           "ORDER BY DATE_VALUE, TIME_VALUE ASC";
        query->SQL->Text = Query;

        DBTransaction.StartTransaction();
        query->ParamByName("TERMINALNAME")->AsString = TerminalNum;
        query->ExecQuery();

        while(!query->Eof)
        {
            TenantName = query->FieldByName("MALLCODE")->AsString;
            DateValue = query->FieldByName("DATE_VALUE")->AsString;
            TimeValue = query->FieldByName("TIME_VALUE")->AsString;
            AmountValue = query->FieldByName("AMOUNT_VALUE")->AsCurrency;
            MinuteValue = query->FieldByName("MINUTE_VALUE")->AsString;
            SCDiscountCount = query->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
            PatronCount = query->FieldByName("PATRON_COUNT")->AsInteger;

            DataRead[TimeValue] = GetHourlyFormat(TerminalNum, TenantName, DateValue, TimeValue, PatronCount, MinuteValue, SCDiscountCount);

            query->Next();
        }
        DBTransaction.Commit();
        query->Close();

        for(int i=0;i<24;i++)
        {
            UnicodeString Time = "";
            UnicodeString Data = "";
            TDateTime Tomorrow = Now() + 1.0;
            TDateTime Yesterday = Now() - 1.0;

            switch(i)
            {
                case 0:
                    Time = "06";
                    break;
                case 1:
                    Time = "07";
                    break;
                case 2:
                    Time = "08";
                    break;
                case 3:
                    Time = "09";
                    break;
                case 4:
                    Time = "10";
                    break;
                case 5:
                    Time = "11";
                    break;
                case 6:
                    Time = "12";
                    break;
                case 7:
                    Time = "13";
                    break;
                case 8:
                    Time = "14";
                    break;
                case 9:
                    Time = "15";
                    break;
                case 10:
                    Time = "16";
                    break;
                case 11:
                    Time = "17";
                    break;
                case 12:
                    Time = "18";
                    break;
                case 13:
                    Time = "19";
                    break;
                case 14:
                    Time = "20";
                    break;
                case 15:
                    Time = "21";
                    break;
                case 16:
                    Time = "22";
                    break;
                case 17:
                    Time = "23";
                    break;
                case 18:
                    Time = "00";
                    break;
                case 19:
                    Time = "01";
                    break;
                case 20:
                    Time = "02";
                    break;
                case 21:
                    Time = "03";
                    break;
                case 22:
                    Time = "04";
                    break;
                case 23:
                    Time = "05";
                    break;
                default:
                    break;
            }

            if(query->RecordCount == 0)
            {
                DataRead[Time] = GetHourlyFormat(TerminalNum, TenantName, DateValue, TimeValue, PatronCount, MinuteValue, SCDiscountCount);
            }

            it = DataRead.find(Time);
            TempVal = (it->second);


            if(TenantName == "")
            {
                TenantName = TGlobalSettings::Instance().TenantNo;
            }

            GetHourlyData(TerminalNum, TenantName, DateValue, Time, TransactionTotal, Amount, MinuteValue, SCDiscountCount, PatronCount);

            for(int i=0;i<7;i++)
            {
                if(i==0)
                {
                    std::string DateSTR = "";
                    UnicodeString DateVal = DateValue;
                    UnicodeString Day = "";
                    UnicodeString Month = "";
                    UnicodeString Year = "";

                    DateSTR = DateVal.t_str();
                    for(int i=0;i<DateVal.Length();i++)
                    {
                        if(i<4)
                        {
                            Year = Year + DateSTR[i];
                        }
                        else if(i>=4 && i<=5)
                        {
                            Month = Month + DateSTR[i];
                        }
                        else
                        {
                            Day = Day + DateSTR[i];
                        }
                    }

                    TempVal = Month + "/" + Day + "/" + Year + Format;
                }
                else if(i==1)
                {
                    TempVal = TenantName + Format;
                }
                else if(i==2)
                {
                    TempVal = Time + ":" + MinuteValue + Format;
                }
                else if(i==3)
                {
                    TempVal = "" + Amount + "" + Format;
                }
                else if(i==4)
                {
                    TempVal = IntToStr(TransactionTotal) + Format;
                }
                else if(i==5)
                {
                    TempVal = IntToStr(PatronCount) + Format;
                }
                else
                {
                    TempVal = IntToStr(SCDiscountCount) + "\n";
                }
                OutputValue += TempVal;
            }
            DataToWrite.push_back(OutputValue.t_str());
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "HTXTDATA");
            DataToWrite.clear();
            OutputValue = "";
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

// This method fixes the decimal point of a currency value
UnicodeString TMallExportFederalLandMall::FixDecimalPlaces(Currency AmountTotal)
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
        adecimal = adecimal + "000";
    }
    else if(adecimal.length() == 2)
    {
        adecimal = adecimal + "00";
    }
    else if(adecimal.length() == 3)
    {
        adecimal = adecimal + "0";
    }
    GetData = value.c_str();

    if(!decimal)
    {
        result = GetData + ".0000";
    }
    else
    {
        result = GetData + "." + adecimal.c_str();
    }

    return result;
}
//---------------------------------------------------------------------------

// This method will generate the output format for hourly export file
UnicodeString TMallExportFederalLandMall::GetHourlyFormat(UnicodeString TerminalName, UnicodeString TenantName,
                                                    UnicodeString DateValue, UnicodeString TimeValue, int PatronCount,
                                                    UnicodeString MinuteValue, int SCDiscountCount)
{
    int TransactionTotal = 0;
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";
    UnicodeString Amount = "";

    GetHourlyData(TerminalName, TenantName, DateValue, TimeValue, TransactionTotal, Amount, MinuteValue, SCDiscountCount, PatronCount);

    if(TransactionTotal != 0)
    {
        result = DateValue + TenantName + TimeValue + ":" + MinuteValue + Amount + TransactionTotal + PatronCount + SCDiscountCount;
    }
    else
    {
        result = DateValue + TenantName + TimeValue + ":" + "00" + "0" + "0" + "0" + "0";
    }

    return result;
}
//---------------------------------------------------------------------------

// This method will generate the data for hourly export file
UnicodeString TMallExportFederalLandMall::GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                  UnicodeString &DateValue, UnicodeString &TimeValue,
                                                  int &TransactionTotal, UnicodeString &Amount, UnicodeString &MinuteValue,
                                                  int &SCDiscountCount, int &PatronCount)
{
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());

    query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM, SUM(SCDISCOUNT_COUNT) as SCCOUNT, SUM(PATRON_COUNT) as PATRON_COUNT "
                       "FROM MALLEXPORT_HOURLY WHERE TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME";
    DBTransaction.StartTransaction();
    query->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query->ParamByName("TERMINALNAME")->AsString = TerminalName;
    query->ExecQuery();

    AmountTotal = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    TransactionTotal = RoundToNearest(query->FieldByName("TRANSSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    SCDiscountCount = query->FieldByName("SCCOUNT")->AsInteger;
    PatronCount = query->FieldByName("PATRON_COUNT")->AsInteger;

    Amount = FixDecimalPlaces(AmountTotal);

    DBTransaction.Commit();
    query->Close();

    query1->SQL->Text = "SELECT MINUTE_VALUE FROM MALLEXPORT_HOURLY "
                       "WHERE TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME ORDER BY MINUTE_VALUE DESC";
    DBTransaction.StartTransaction();
    query1->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query1->ParamByName("TERMINALNAME")->AsString = TerminalName;
    query1->ExecQuery();

    MinuteValue = query1->FieldByName("MINUTE_VALUE")->AsString;

    DBTransaction.Commit();
    query1->Close();

    return result;
}
//---------------------------------------------------------------------------

// This Function is for saving each string component into the DB for every function call
void TMallExportFederalLandMall::SaveStrVariable(vmVariables vmVar, UnicodeString CompName)
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

// Checking of previous file will happen in 6:00 AM if the POS will not be exited
void TMallExportFederalLandMall::SetZedTimer()
{
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    if(Hour < 6)
    {
        zedcheckTimer->Interval = ((18000000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
    else
    {
        zedcheckTimer->Interval = ((104400000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
}
//---------------------------------------------------------------------------

void TMallExportFederalLandMall::CheckPreviousEODFile()
{
    UnicodeString MerchantCode = TGlobalSettings::Instance().TenantNo;
    UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
    bool ZReportStatus = false;
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    UnicodeString MonthValue = "";
    UnicodeString DayValue = "";
    TDateTime DateValue;
    DateValue = Now() - 1.0;

    UnicodeString Format = ".csv";
    UnicodeString FileYear = DateValue.FormatString("yy");
    UnicodeString MallPath = FilePath;
    UnicodeString CheckEOYFolder = DateValue.FormatString("mmdd");
    UnicodeString Year = DateValue.FormatString("yyyy");
    UnicodeString Month = DateValue.FormatString("mm");
    UnicodeString FileName = MallPath + Year + "\\" + Month + "\\";

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    // Checking of previous file will happen in 6:00 AM if the POS will not be exited
    zedcheckTimer->Interval = ((108000000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));

    GetExportInfo(MonthValue, DayValue, DateValue);

    FileName = FileName + MerchantCode + MonthValue + DayValue + FileYear + "S" + Format;

    CheckPreviousFile.open(FileName.c_str(), std::ios_base::out | std::ios_base::in);

    if(!CheckPreviousFile.is_open())
    {
        ZReportStatus = false;
    }
    else
    {
        ZReportStatus = true;
    }
    CheckPreviousFile.close();

    if(Hour >= 6)
    {
        if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0))
        {
            frmMain->ZForced();
        }
    }
}
//---------------------------------------------------------------------------

// This method generates the export file for the days it was not used, if the last day the pos
// was uses does not performed eod, this will generate that export file and the consecutive days
// that it was not used. The data will be zeroes if there are no information available
void TMallExportFederalLandMall::GenerateFileForDaysPOSNotUsed(UnicodeString MallPath, UnicodeString MerchantCode,
                                       UnicodeString CheckDate, UnicodeString Format, UnicodeString DateFormat)
{
    try
    {
        TMallExportUpdateAdaptor exportUpdateAdaptor;
        TMallExportHourlyUpdate exportHourlyUpdate;
        TMallExportOtherDetailsUpdate exportOtherDetailsUpdate;

        UnicodeString LastUsedDate = TGlobalSettings::Instance().LastTransactionDate;
        UnicodeString LastZedDate = TGlobalSettings::Instance().LastZedDate;
        UnicodeString LastZedTime = TGlobalSettings::Instance().LastZedTime;
        UnicodeString ContractNum = TGlobalSettings::Instance().SerialNo;
        UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
        UnicodeString MallFileNameS = "";
        UnicodeString MallFileNameD = "";
        UnicodeString MallFileNameP = "";
        UnicodeString MallFileNameR = "";
        UnicodeString MallFileNameH = "";
        UnicodeString DateParam = "";
        UnicodeString DateParamTomorrow = "";
        UnicodeString DateParamYesterday = "";
        UnicodeString MPYear = "";
        UnicodeString InitMallPath = MallPath;
        UnicodeString DateComp = "";
        UnicodeString MonthValue = "";
        UnicodeString DayValue = "";
        UnicodeString FileYear = "";
        UnicodeString Month = "";
        UnicodeString Query = "";

        TDateTime PrevDate;
        TDateTime IncrementDate;
        TDateTime DateDataTomorrow;
        TDateTime DateDataYesterday;
//        TDateTime Yesterday = Now() - 2.0;
        TDateTime Today = Now() - 1.0;
        TDateTime CurrentDate;
        TDateTime LastDate;
        int DayCount = 0;
        int i = 0;

        DateComp = (LastZedDate == "") ? LastUsedDate : LastZedDate;
        LastDate = (LastZedDate == "") ? StrToDateTime(LastUsedDate) : StrToDateTime(LastZedDate);
        CurrentDate = StrToDateTime(CheckDate);

        if(LastDate < CurrentDate)
        {
            while(CheckDate.Compare(DateComp) != 0)
            {
                DayCount++;
                PrevDate = Now() - DayCount;
                CheckDate = PrevDate.FormatString("mm/dd/yy");
            }

            PrevDate = Now() - DayCount;

            i = (LastZedTime == "") ? 0 : 1;

            int a = 1;

            if(DayCount > 1)
            {
                for(;i<DayCount-1;i++)
                {
                    IncrementDate = PrevDate + i;
                    DateParam = IncrementDate.FormatString("yyyymmdd");
                    DateDataTomorrow = PrevDate + i + 1;
                    DateParamTomorrow = DateDataTomorrow.FormatString("yyyymmdd");
                    DateDataYesterday = PrevDate + i - 1;
                    DateParamYesterday = DateDataYesterday.FormatString("yyyymmdd");

                    FileYear = IncrementDate.FormatString("yy");
                    MPYear = IncrementDate.FormatString("yyyy");
                    MallPath = "";
                    MallPath = InitMallPath;

                    if(!DirectoryExists(MallPath))
                    {
                        CreateDir(MallPath);
                    }
                    MallPath = MallPath + MPYear;
                    if(!DirectoryExists(MallPath))
                    {
                        CreateDir(MallPath);
                    }
                    MallPath = MallPath + "\\";
                    Month = IncrementDate.FormatString("mm");
                    MallPath = MallPath + Month;
                    if(!DirectoryExists(MallPath))
                    {
                        CreateDir(MallPath);
                    }
                    MallPath = MallPath + "\\";

                    GetExportInfo(MonthValue, DayValue, IncrementDate);

                    MallFileNameS = MallPath + MerchantCode + MonthValue + DayValue + FileYear + "S" + Format;
                    MallFileNameD = MallPath + MerchantCode + MonthValue + DayValue + FileYear + "D" + Format;
                    MallFileNameP = MallPath + MerchantCode + MonthValue + DayValue + FileYear + "P" + Format;
                    MallFileNameR = MallPath + MerchantCode + MonthValue + DayValue + FileYear + "R" + Format;
                    MallFileNameH = MallPath + MerchantCode + MonthValue + DayValue + FileYear + "H" + Format;

                    CheckFile.open(MallFileNameS.c_str(), std::ios_base::out | std::ios_base::in);

                    if(!CheckPreviousZedDateTime())
                    {
                        if(a >= 2)
                        {
                            Database::TDBTransaction DBTransaction(
                            TDeviceRealTerminal::Instance().DBControl);
                            DBTransaction.StartTransaction();
                            UnicodeString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

                            TTransactionInfo TransactionInfo;

                            TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(DBTransaction, DeviceName);

                            TMallExportUpdateAdaptor exportUpdateAdaptor;
                            TFinancialDetails FinancialDetails = reportFinancialCalculations->GetFinancialDetails(DBTransaction, TransactionInfo, DeviceName);
                            exportUpdateAdaptor.UpdateExportTablesOnZed(FinancialDetails, &DBTransaction);
                        }
                        a++;

                        // For Daily Sales
                        DataQuery = getFederalLandQuery("S");
                        InputManager->ReadFromDB(DataQuery,DataRead);
//                        CreateFilename("S", MallPath, MallPathFileName);
                        CreateHeaderFormat(MallFileNameS,DataToWrite, "S");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameS, "ZTXTHEADER");
                        DataToWrite.clear();

                        CreateOutputFormatModeS(DataRead, DataToWrite);
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameS, "ZTXTDATA");
                        DataToWrite.clear();

                        // For Daily Sales Discount
//                        CreateFilename("D", MallPath, MallPathFileName);
                        CreateHeaderFormat(MallFileNameD,DataToWrite, "D");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameD, "ZTXTHEADER");
                        DataToWrite.clear();

                        Query = getFederalLandQuery("D");
                        CreateOutputFormatModeD(DataRead, DataToWrite, Query);
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameD, "ZTXTDATA");
                        DataToWrite.clear();

                        // For Dailu Sales Payment
//                        CreateFilename("P", MallPath, MallPathFileName);
                        CreateHeaderFormat(MallFileNameP,DataToWrite, "P");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameP, "ZTXTHEADER");
                        DataToWrite.clear();

                        Query = getFederalLandQuery("P");
                        CreateOutputFormatModeP(DataRead, DataToWrite, Query);
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameP, "ZTXTDATA");
                        DataToWrite.clear();

                        // For Dailu Sales Refund Cancel
//                        CreateFilename("R", MallPath, MallPathFileName);
                        CreateHeaderFormat(MallFileNameR,DataToWrite, "R");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameR, "ZTXTHEADER");
                        DataToWrite.clear();

                        Query = getFederalLandQuery("R");
                        CreateOutputFormatModeR(DataRead, DataToWrite, Query);
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameR, "ZTXTDATA");
                        DataToWrite.clear();

                        // For Hourly Sales
//                        CreateFilename("H", MallPath, MallPathFileName);
                        CreateHeaderFormat(MallFileNameH,DataToWrite, "H");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameH, "ZTXTHEADER");
                        DataToWrite.clear();

                        Query = getFederalLandQuery("H");
                        CreateOutputFormatModeH(DataRead, DataToWrite, MallFileNameH, Query);
                        DataToWrite.clear();

                        TGlobalSettings::Instance().ZCount += 1;
                        SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);

                        exportUpdateAdaptor.ResetExportTables();
                        exportHourlyUpdate.ResetHourlyExportTablesOnZed();
                        exportOtherDetailsUpdate.ResetOtherDetailsExportTablesOnZed();

                        TGlobalSettings::Instance().LastZedDate = IncrementDate.FormatString("mm/dd/yy");
                        SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
                        TGlobalSettings::Instance().LastZedTime = Now().FormatString("hh");
                        SaveStrVariable(vmLastZedTime, TGlobalSettings::Instance().LastZedTime);
                    }
                    CheckFile.close();
                }
                TGlobalSettings::Instance().LastTransactionDate = Today.FormatString("mm/dd/yy");
                SaveStrVariable(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);
            }
        }
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }
}
//---------------------------------------------------------------------------

// This will check if the previous zed date time
bool TMallExportFederalLandMall::CheckPreviousZedDateTime()
{
    UnicodeString LastZedDate = TGlobalSettings::Instance().LastZedDate;
    UnicodeString LastZedTime = TGlobalSettings::Instance().LastZedTime;
    UnicodeString CurrentDate = Now().FormatString("mm/dd/yy");
    UnicodeString ZedTime = Now().FormatString("hh");
    bool result = false;
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    if(LastZedDate == "")
    {
        result = false;
    }
    else if(CurrentDate.Compare(LastZedDate) != 0)
    {
        result = false;
    }
    else
    {
        if(StrToInt(LastZedTime) == StrToInt(ZedTime))
        {
            result = false;
        }
        else
        {
            result = (StrToInt(LastZedTime) > 5) ? true : false;
        }
    }
    return result;
}
//---------------------------------------------------------------------------

// This function is for saving the integer global variable
void TMallExportFederalLandMall::SaveIntVariable(vmVariables vmVar, int CompName)
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

#pragma package(smart_init)
