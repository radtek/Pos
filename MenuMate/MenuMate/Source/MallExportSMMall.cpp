//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportSMMall.h"
#include "MallExportDataManager.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
#include <vector>
#include <ios>
#include <string>

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

// PUBLIC METHODS
TMallExportSMMall::TMallExportSMMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportSMMall::~TMallExportSMMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportSMMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Delete this method if not needed
TExportResponse TMallExportSMMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        DataRead.clear();
        DataToWrite.clear();
        TGlobalSettings::Instance().ZCount += 1;
        ZCounter = TGlobalSettings::Instance().ZCount;
        SaveZCount(vmZCount, TGlobalSettings::Instance().ZCount);
        DataQuery = getSMQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename();
        CreateOutputFormat(DataRead, DataToWrite);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);
        CreateOutputFlag();

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

TExportResponse TMallExportSMMall::TransactionExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        DataRead.clear();
        DataToWrite.clear();
        CreateLocalDir();
        DataQuery = getSMQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        ZCounter = 0;
        CreateOutputFormat(DataRead, DataToWrite);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);

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

// PROTECTED METHODS
// Virtual methods
void TMallExportSMMall::initTimingExportResponse( TExportResponse& outResponse )
{
    // TODO: Initiate the response for SM

    setExportResponse(
       false,               // Succesful = false
       emrExportNotDoneYet, // Result = emrExportNotDoneYet
       "",                  // Message
       "",                  // Description
       outResponse );
}
//---------------------------------------------------------------------------
// Delete this method if not needed
TExportResponse TMallExportSMMall::timingExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        DataRead.clear();
        DataToWrite.clear();
        DataQuery = getSMQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename();
        ZCounter = 0;
        CreateOutputFormat(DataRead, DataToWrite);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);

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

// This Function is for creating the SM Mall Export filename
void TMallExportSMMall::CreateFilename()
{
    UnicodeString FileName = "";
    UnicodeString DateFormat = "";
    UnicodeString FileExt = ".txt";

    DateFormat = Now().FormatString("mmddyyyy");
    FFileURI = CheckDir(FilePath);
    FFileURI = FFileURI + DateFormat + FileExt;
}
//---------------------------------------------------------------------------

// This function is for checking if the File Path specified in the settings is
// existing, if not, it will create a local Path in Menumate folder Called MallExport
// and then create another foler for SMMall for Zed Export
UnicodeString TMallExportSMMall::CheckDir(UnicodeString FilePath)
{
    if(!DirectoryExists(FilePath))
    {
        FilePath = ExtractFilePath(Application->ExeName) + MALLEXPORTFOLDERPATH;
        if(!DirectoryExists(FilePath))
        {
            CreateDir(FilePath);
        }
        FilePath = FilePath + "\\SMMall\\";
        if(!DirectoryExists(FilePath))
        {
            CreateDir(FilePath);
        }
    }
    return FilePath;
}
//---------------------------------------------------------------------------

// AFTER every transaction, the record must be updated with the RUNNING (cumulative)
// total - it means the text file is updated but not SENT, sending export file is only
// ZED and HOURLY, please inform me if you think this is not needed
void TMallExportSMMall::CreateLocalDir()
{
    UnicodeString FilePathLoc = ExtractFilePath(Application->ExeName) + MALLEXPORTFOLDERPATH;

    if(!DirectoryExists(FilePathLoc))
    {
        CreateDir(FilePathLoc);
    }
    FilePathLoc = FilePathLoc + "\\SMMallTransaction\\";
    if(!DirectoryExists(FilePathLoc))
    {
        CreateDir(FilePathLoc);
    }
    FFileURI = FilePathLoc + Now().FormatString("mmddyyyy") + ".txt";
}
//---------------------------------------------------------------------------

// This function is for getting query from the DB Table
UnicodeString TMallExportSMMall::getSMQuery()
{
    UnicodeString SMQuery = "";
    SMQuery = "SELECT * FROM SM_MALLVIEW";

    return SMQuery;
}
//---------------------------------------------------------------------------

// This function is for creating output flag required by sm mall to indicate that
// the Z-reading is already initiated
void TMallExportSMMall::CreateOutputFlag()
{
    UnicodeString FlagFormat = "";
    UnicodeString FlagExtName = ".flg";

    FFileURI = CheckDir(FilePath);
    FlagFormat = FFileURI + "Z" + Now().FormatString("yymmdd") + FlagExtName;
    OutputFlag.open(FlagFormat.c_str(), std::ios_base::out);
    OutputFlag.close();
}
//---------------------------------------------------------------------------

// This function is for arranging the format of the output file
TExportResponse TMallExportSMMall::CreateOutputFormat(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite)
{
    TExportResponse result;

    try
    {

        int i = 0;
        UnicodeString Format = ",";
        UnicodeString OutputValue = "";

        std::map<UnicodeString, UnicodeString>::iterator it;

        for(i=0;i<DataRead.size();i++) {
            if(i==0) {
                it = DataRead.find("MallCode");
            } else if(i==1) {
                it = DataRead.find("TenantNo");
            } else if(i==2) {
                it = DataRead.find("ClassCode");
            } else if(i==3) {
                it = DataRead.find("TradeCode");
            } else if(i==4) {
                it = DataRead.find("OutletNumber");
            } else if(i==5) {
                it = DataRead.find("GrandTotal");
            } else if(i==6) {
                it = DataRead.find("GrandTotalOld");
            } else if(i==7) {
                it = DataRead.find("SalesType");
            } else if(i==8) {
//                it = DataRead.find("TotalGrossSale"); // Department Sum
                it = DataRead.find("DepartmentSum");
            } else if(i==9) {
                it = DataRead.find("TotalRegularDiscount");
            } else if(i==10) {
                it = DataRead.find("EmployeeDiscount");
            } else if(i==11) {
                it = DataRead.find("TotalSeniorCitizenDiscount");
            } else if(i==12) {
                it = DataRead.find("VIPDiscount");
            } else if(i==13) {
                it = DataRead.find("DisabilityDiscount");
            } else if(i==14) {
                it = DataRead.find("GPCDiscount");
            } else if(i==15) {
                it = DataRead.find("DiscountG1");
            } else if(i==16) {
                it = DataRead.find("DiscountG2");
            } else if(i==17) {
                it = DataRead.find("DiscountG3");
            } else if(i==18) {
                it = DataRead.find("DiscountG4");
            } else if(i==19) {
                it = DataRead.find("DiscountG5");
            } else if(i==20) {
                it = DataRead.find("DiscountG6");
            } else if(i==21) {
                it = DataRead.find("TotalSaleTax");
            } else if(i==22) {
//                it = DataRead.find("LocalTaxes");
                it = DataRead.find("OtherTax");   // OtherTax is intended for SM
            } else if(i==23) {
                it = DataRead.find("Adjustments");
            } else if(i==24) {
                it = DataRead.find("PositiveAdjustments");
            } else if(i==25) {
                it = DataRead.find("NegativeAdjustments");
            } else if(i==26) {
                it = DataRead.find("PositiveAdjustmentsNonTax");
            } else if(i==27) {
                it = DataRead.find("NegativeAdjustmentsNonTax");
            } else if(i==28) {
                it = DataRead.find("DailySales");
            } else if(i==29) {
                it = DataRead.find("TotalCancelled");
            } else if(i==30) {
                it = DataRead.find("TotalRefund");
            } else if(i==31) {
                it = DataRead.find("TaxInclusiveSales");
            } else if(i==32) {
                it = DataRead.find("TaxExemptSales");
            } else if(i==33) {
                it = DataRead.find("ChargeCreditSales");
            } else if(i==34) {
                it = DataRead.find("CashSales");
            } else if(i==35) {
                it = DataRead.find("GiftChequeSales");
            } else if(i==36) {
                it = DataRead.find("DebitCardSales");
            } else if(i==37) {
                it = DataRead.find("OtherTender");
            } else if(i==38) {
                it = DataRead.find("TotalMastercard");
            } else if(i==39) {
                it = DataRead.find("TotalVisa");
            } else if(i==40) {
                it = DataRead.find("TotalAmericanExpress");
            } else if(i==41) {
                it = DataRead.find("TotallDiners");
            } else if(i==42) {
                it = DataRead.find("TotalJCB");
            } else if(i==43) {
                it = DataRead.find("TotalOtherCards");
            } else if(i==44) {
                it = DataRead.find("TotalServiceCharge");
            } else if(i==45) {
                it = DataRead.find("OtherCharges");
            } else if(i==46) {
                it = DataRead.find("FirstTransaction");
            } else if(i==47) {
                it = DataRead.find("LastTransaction");
            } else if(i==48) {
                it = DataRead.find("TransactionCount");
            } else if(i==49) {
                it = DataRead.find("BeginningInvoiceNo");
            } else if(i==50) {
                it = DataRead.find("EndingInvoiveNo");
            } else if(i==51) {
                it = DataRead.find("CashTransactions");
            } else if(i==52) {
                it = DataRead.find("GiftCardChequeTransactions");
            } else if(i==53) {
                it = DataRead.find("DebitCardTransactions");
            } else if(i==54) {
                it = DataRead.find("OtherTenderTransctions");
            } else if(i==55) {
                it = DataRead.find("MasterCardTransactions");
            } else if(i==56) {
                it = DataRead.find("VisaTransactions");
            } else if(i==57) {
                it = DataRead.find("AmericanExpressTransactions");
            } else if(i==58) {
                it = DataRead.find("DinersTransactions");
            } else if(i==59) {
                it = DataRead.find("JCBTransactions");
            } else if(i==60) {
                it = DataRead.find("OtherCardTransactions");
            } else if(i==61) {
                it = DataRead.find("TerminalNo");
            } else if(i==62) {
                it = DataRead.find("SerialNumber");
            } else if(i==63) {
                // it = DataRead.find("ZCount");
                // No function since it does not exist in the table
                // only a flag that changes after zed report
            } else if(i==64) {
                it = DataRead.find("TransactionTime");
            } else if(i==65) {
                it = DataRead.find("TransactionDate");
            }

            if(((i>=51) || (i<=60)) && (((it->second) == "0") || ((it->second) == ""))){
                OutputValue = "0000" + Format;
            } else if(i==63) {
                OutputValue = ZCounter;
                OutputValue = OutputValue + Format;
            } else if(i==64) {
                OutputValue = GetTime((it->second)) + Format;
            } else if(i==65) {
                OutputValue = GetDate((it->second));
            } else if(((it->second) == "0") || ((it->second) == "")){
                OutputValue = "0.00" + Format;
            } else {
                OutputValue = (it->second) + Format;
            }
            DataToWrite.push_back(OutputValue.t_str());
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

// Saving ZCount Global Variable to the DB
void TMallExportSMMall::SaveZCount(vmVariables vmVar, int CompName)
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

// Get Time format for SM Export File
UnicodeString TMallExportSMMall::GetTime(UnicodeString DateTime)
{
    AnsiString time = "";

    try
    {
        TDateTime date(DateTime);
        time = date.FormatString("hhnnss");
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// Get Date format for SM Export File
UnicodeString TMallExportSMMall::GetDate(UnicodeString DateTime)
{
    AnsiString time = "";

    try
    {
        TDateTime date(DateTime);
        time = date.FormatString("mmddyyyy");
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// This function is for checking the ZFlag for SM Mall
TExportResponse TMallExportSMMall::CheckZFlag()
{
    TExportResponse result;

    try
    {
        UnicodeString Hour = Now().FormatString("hh");
        UnicodeString Minutes = Now().FormatString("nn");
        UnicodeString Seconds = Now().FormatString("ss");
        UnicodeString FlagName = "Z" + Now().FormatString("yymmdd") + ".flg";
        UnicodeString CheckFile = "";
        UnicodeString CheckFilePath = "";
        UnicodeString CheckDate = Now().FormatString("mmddyy");

        CheckFilePath = CheckDir(FilePath);

        CheckFile = CheckFilePath + FlagName;

        CheckFlag.open(CheckFile.c_str(), std::ios_base::out | std::ios_base::in);

        if(CheckFlag.is_open())
        {
            if(TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) == 0)
            {
                FTimingExportResponse = timingExport();
            }
            // Set timing to generate export file on the next day, exactly (00:00:00)
            exportTimer->Interval = ((82800000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
        } else {
            exportTimer->Interval = 60000 - (Seconds*1000);
        }

        if((Minutes == "00") && (!CheckFlag.is_open()))
        {
            FTimingExportResponse = timingExport();
        }
        CheckFlag.close();

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

