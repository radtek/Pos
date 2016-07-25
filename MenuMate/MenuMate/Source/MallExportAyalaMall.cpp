//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportAyalaMall.h"
#include "MallExportDataManager.h"
#include "MallExportHourlyUpdate.h"
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
TMallExportAyalaMall::TMallExportAyalaMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportAyalaMall::~TMallExportAyalaMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportAyalaMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Delete this method if not needed
TExportResponse TMallExportAyalaMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        TMallExportUpdateAdaptor exportUpdateAdaptor;
        UnicodeString LocalPath = BasePath + "\\AyalaMall\\";
        UnicodeString LocalPathGen = BasePath + "\\AyalaMall\\";
        UnicodeString LocalPathFileName = "";
        UnicodeString MallPath = FilePath;
        UnicodeString MallPathGen = FilePath;
        UnicodeString MallPathFileName = "";
        TDateTime Yesterday = Now() - 1.0;
        Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];

        UnicodeString CheckDate = Now().FormatString("mmddyy");
        UnicodeString TenantID = "";
        UnicodeString FileExt = "";
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString DateFormat = "";
        UnicodeString GenMallPath = FilePath;
        bool ZReportStatus = false;
        unsigned short Hour = 0;
        unsigned short Minutes = 0;
        unsigned short Seconds = 0;
        unsigned short dummy = 0;

        if(gridBtn->Enabled == false)
        {
            Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

            GetExportInfo(MallPathGen, TenantID, TerminalNo, FileExt, DateFormat);
            ZReportStatus = CheckPreviousExportFile(MallPathGen, TenantID, FileExt, DateFormat, "ZED");
            GenerateFileForDaysPOSNotUsed(LocalPathGen, GenMallPath, TerminalNo, CheckDate, TenantID, FileExt, DateFormat);

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

        // For DBF File
        DataQuery = getAyalaQuery("ZED");
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename("ZED", MallPath, LocalPath, LocalPathFileName, MallPathFileName, ".dbf");
        // For Mall Path
        CreateHeaderFormatZed(MallPathFileName,DataToWrite, "DBF");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZHEADER");
        DataToWrite.clear();
        // For Local Path Header
        // CreateHeaderFormatZed(LocalPathFileName,DataToWrite, "DBF");
        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName, "ZHEADER");
        // DataToWrite.clear();

        // For Mall Path Data
        CreateOutputFormatZed(DataRead, DataToWrite, MallPathFileName, "", "", false, "DBF");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZDATA");
        DataToWrite.clear();
        // For Local Path Data
        // InputManager->ReadFromDB(DataQuery,DataRead);
        // CreateOutputFormatZed(DataRead, DataToWrite, LocalPathFileName, "", "", false, "DBF");
        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName, "ZDATA");
        // DataToWrite.clear();

        // For TXT File
        LocalPathFileName = "";
        MallPathFileName = "";
        DataQuery = getAyalaQuery("ZED");
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename("ZED", MallPath, LocalPath, LocalPathFileName, MallPathFileName, ".txt");

        // For Mall Path Header
        CreateHeaderFormatZed(MallPathFileName,DataToWrite, "TXT");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTHEADER");
        DataToWrite.clear();
        // For Local Path Header
        // CreateHeaderFormatZed(LocalPathFileName,DataToWrite, "TXT");
        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName, "ZTXTHEADER");
        // DataToWrite.clear();

        // For Mall Path Data
        CreateOutputFormatZed(DataRead, DataToWrite, MallPathFileName, "", "", false, "TXT");
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "ZTXTDATA");
        DataToWrite.clear();
        // For Local Path Data
        // InputManager->ReadFromDB(DataQuery,DataRead);
        // CreateOutputFormatZed(DataRead, DataToWrite, LocalPathFileName, "", "", false, "TXT");
        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName, "ZTXTDATA");
        // DataToWrite.clear();

        TGlobalSettings::Instance().ZCount += 1;
        SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);

        HourlyDetailedExportOnZed();

        if(gridBtn->Enabled == false)
        {
            frmMain->EnablePOSBtn(true);
            TGlobalSettings::Instance().LastZedDate = Yesterday.FormatString("mm/dd/yy");
            SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
            TGlobalSettings::Instance().LastTransactionDate = Yesterday.FormatString("mm/dd/yy");
            SaveStrVariable(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);
        }
        else
        {
            TGlobalSettings::Instance().LastZedDate = Now().FormatString("mm/dd/yy");
            SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
            TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mm/dd/yy");
            SaveStrVariable(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);
        }

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
void TMallExportAyalaMall::initTimingExportResponse( TExportResponse& outResponse )
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

TExportResponse TMallExportAyalaMall::TransactionExport()
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
// Delete this method if not needed
TExportResponse TMallExportAyalaMall::HourlyDetailedExportOnZed()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        TMallExportHourlyUpdate exportHourlyUpdate;
        UnicodeString LocalPath = BasePath + "\\AyalaMall\\";
        UnicodeString LocalPathFileName = "";
        UnicodeString MallPath = FilePath;
        UnicodeString MallPathFileName = "";
        UnicodeString FileNameBackUp = "";
        UnicodeString PrevDate = "";
        UnicodeString PrevDateTomorrow = "";
        UnicodeString TXTPrevDate = "";
        UnicodeString TXTPrevDateTomorrow = "";
        TDateTime Date;
        bool PrevDateFlag = false;

        // For DBF File
        CreateFilename("TIMING", MallPath, LocalPath, LocalPathFileName, MallPathFileName, ".dbf");

        // For Mall Path Header
        CheckCurrentFile.open(MallPathFileName.c_str());
        if(!CheckPreviousZedDateTime())
        {
            DataToWrite.clear();
            CreateHeaderFormatHourly(MallPathFileName, DataToWrite, "DBF");
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "HHEADER");
        }
        CheckCurrentFile.close();

        // For Local Path Header
        // CheckCurrentFile.open(LocalPathFileName.c_str());
        // if(!CheckPreviousZedDateTime())
        // {
        //     DataToWrite.clear();
        //     CreateHeaderFormatHourly(MallPathFileName, DataToWrite, "DBF");
        //     TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName, "HHEADER");
        // }
        // CheckCurrentFile.close();

        Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];
        if(gridBtn->Enabled == false)
        {
            Date = Now() - 1.0;
            PrevDate = Date.FormatString("yyyymmdd");
            TXTPrevDate = Date.FormatString("mm/dd/yyyy");
            Date = Now();
            PrevDateTomorrow = Date.FormatString("yyyymmdd");
            TXTPrevDateTomorrow = Date.FormatString("mm/dd/yyyy");
            PrevDateFlag = true;
        }
        DataRead.clear();

        // For Mall Path Data
        CreateOutputFormatHourly(DataRead, DataToWrite, MallPathFileName, PrevDate, PrevDateTomorrow, "", PrevDateFlag, "DBF");
        // For Local Path Data
        // CreateOutputFormatHourly(DataRead, DataToWrite, LocalPathFileName, PrevDate, PrevDateTomorrow, "", PrevDateFlag, "DBF");

        MallPathFileName = "";
        LocalPathFileName = "";
        // For TXT File
        CreateFilename("TIMING", MallPath, LocalPath, LocalPathFileName, MallPathFileName, ".txt");

        // For Mall Path Header
        CheckCurrentFile.open(MallPathFileName.c_str());
        if(!CheckPreviousZedDateTime())
        {
            DataToWrite.clear();
            CreateHeaderFormatHourly(MallPathFileName, DataToWrite, "TXT");
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "HTXTHEADER");
        }
        CheckCurrentFile.close();

        // For Local Path Header
        // CheckCurrentFile.open(LocalPathFileName.c_str());
        // if(!CheckPreviousZedDateTime())
        // {
        //     DataToWrite.clear();
        //     CreateHeaderFormatHourly(MallPathFileName, DataToWrite, "TXT");
        //     TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName, "HTXTHEADER");
        // }
        // CheckCurrentFile.close();

        DataRead.clear();

        // For TXT
        // For Mall Path Data
        CreateOutputFormatHourly(DataRead, DataToWrite, MallPathFileName, TXTPrevDate, TXTPrevDateTomorrow, "", PrevDateFlag, "TXT");
        // For Local Path Data
        // CreateOutputFormatHourly(DataRead, DataToWrite, LocalPathFileName, TXTPrevDate, TXTPrevDateTomorrow, "", PrevDateFlag, "TXT");

        exportHourlyUpdate.ResetHourlyExportTablesOnZed();

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

TExportResponse TMallExportAyalaMall::CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                                     UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName,
                                                     UnicodeString FileExt)
{
    TExportResponse result;

    try
    {
        UnicodeString CheckDate = Now().FormatString("mmddyy");
        UnicodeString CheckEOYFolder = Now().FormatString("mmdd");
        UnicodeString DateFormat = Now().FormatString("mmdd");
        UnicodeString TenantID = "";
        UnicodeString TenantName = TGlobalSettings::Instance().BranchCode; // Changes is based on TenantNo vs MallCode
        UnicodeString ContractNum = TGlobalSettings::Instance().SerialNo;
        UnicodeString NameFormat = "";
        UnicodeString Year = "";
        TDateTime FolderYear;
        std::string TenantNameSTR = TenantName.t_str();
        bool ZReportStatus = false;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        if((CheckEOYFolder == "0101") && (Hour < 6))
        {
            FolderYear = Now() - 1.0;
            Year = FolderYear.FormatString("yyyy");
        }
        else
        {
            Year = Now().FormatString("yyyy");
        }

        for(int i=0;i<TenantName.Length();i++)
        {
            if(i<3)
            {
                TenantID = TenantID + TenantNameSTR[i];
            }
        }

        MallPath = CheckDir(MallPath);
        MallPath = CreateYearlyFolder(MallPath, Year);

        LocalPath = CheckDir(LocalPath);
        LocalPath = CreateYearlyFolder(LocalPath, Year);

        TenantID = (FileExt == ".dbf") ? TenantID : ContractNum;

        // This condition is to create different date format for the mall export
        // filename depending on the mode
        if(mode == "ZED")
        {
            if(TGlobalSettings::Instance().FirstMallSet == false && TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0)
            {
                ZReportStatus = CheckPreviousExportFile(MallPath, TenantID, FileExt, DateFormat,"ZED");
                DateFormat = getDateFormat(ZReportStatus, DateFormat);
            }
            else
            {
                ZReportStatus = true;
                DateFormat = getDateFormat(ZReportStatus, DateFormat);
                TGlobalSettings::Instance().FirstMallSet = false;
                SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
            }
        }
        else if (mode == "TIMING")
        {
            if(TGlobalSettings::Instance().FirstMallSet == false && TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0)
            {
                ZReportStatus = CheckPreviousExportFile(MallPath, TenantID, FileExt, DateFormat,"TIMING");
                DateFormat = getDateFormat(ZReportStatus, DateFormat) + "H";
            }
            else
            {
                TGlobalSettings::Instance().LastZedTime = Now().FormatString("hh");
                SaveStrVariable(vmLastZedTime, TGlobalSettings::Instance().LastZedTime);
                ZReportStatus = true;
                DateFormat = getDateFormat(ZReportStatus, DateFormat) + "H";
                TGlobalSettings::Instance().FirstMallSet = false;
                SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
            }
        }
        else
        {
            DateFormat = "";
        }

        NameFormat = (FileExt == ".dbf") ? TenantID : ContractNum;


        LocalPathFileName = LocalPath + NameFormat + DateFormat + FileExt;
        MallPathFileName = MallPath + NameFormat + DateFormat + FileExt;

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

UnicodeString TMallExportAyalaMall::getDateFormat(bool ZReportStatus, UnicodeString DateFormat)
{
    TDateTime Yesterday = Now() - 1.0;
    unsigned short Hour = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

    if(Hour < 6)
    {
        DateFormat = Yesterday.FormatString("mmdd");
    }
    else
    {
        if(TGlobalSettings::Instance().FirstMallSet == true)
        {
            DateFormat = Now().FormatString("mmdd");
        }
        else
        {
            DateFormat = (!ZReportStatus) ? DateFormat : Now().FormatString("mmdd");
        }
    }

    return DateFormat;
}
//---------------------------------------------------------------------------

// This function is for checking the specified path for Ayala Mall is existing
// if not, it will create a local folder called MallExport then create a folder
// called AyalaMall for ZedReport and for HourlyExport
UnicodeString TMallExportAyalaMall::CheckDir(UnicodeString Path)
{
    if(!DirectoryExists(Path))
    {
        Path = BasePath;
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
        Path = Path + "\\AyalaMall\\";
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
    }
    return Path;
}
//---------------------------------------------------------------------------

// This functions is for checking the previous EOD file dates
bool TMallExportAyalaMall::CheckPreviousExportFile(UnicodeString MallPath, UnicodeString TenantID,
                                                   UnicodeString FileExt, UnicodeString &DateFormat, UnicodeString mode)
{
    bool ZReportStatus = true;

    try
    {
        UnicodeString Filename = "";
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString Month = Yesterday.FormatString("mm");
        UnicodeString Day = Yesterday.FormatString("dd");

        DateFormat = Month + Day;

        if(mode == "ZED")
        {
            Filename = MallPath + TenantID + DateFormat + FileExt;
        }
        else
        {
            Filename = MallPath + TenantID + DateFormat + "H" + FileExt;
        }
        CheckPreviousFile.open(Filename.c_str(), std::ios_base::out | std::ios_base::in);

        if(!CheckPreviousFile.is_open())
        {
            // ZReportStatus = true; // Old Imprelentation
            ZReportStatus = false;
        }
        else
        {
//             ZReportStatus = CheckPreviousZedDateTime(); // Old Implementation
            ZReportStatus = true;
        }
        CheckPreviousFile.close();
    }
    catch(Exception &ex)
    {
        //ignore the exception
    }

    return ZReportStatus;
}
//---------------------------------------------------------------------------

// This function is used to create yearly folder for ayala mall as part of
// requirements based on the additional information in the text file in jira
UnicodeString TMallExportAyalaMall::CreateYearlyFolder(UnicodeString Path, UnicodeString Year)
{
    Path = Path + Year;
    if(!DirectoryExists(Path))
    {
        CreateDir(Path);
    }

    return Path + "\\";
}
//---------------------------------------------------------------------------

// This function is for getting query from the DB Table for ZED and HOURLY files
UnicodeString TMallExportAyalaMall::getAyalaQuery(UnicodeString mode)
{
    UnicodeString AyalaQuery = "";

    if(mode == "ZED")
    {
        AyalaQuery = "SELECT * FROM AYALA_ZED_MALLVIEW";
    }

    return AyalaQuery;
}

//---------------------------------------------------------------------------

// This method is used to create the header names for Hourly Export
// This method is separated to prevent multiple pos create multiple header files
void TMallExportAyalaMall::CreateHeaderFormatZed(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite,
                                                 UnicodeString FileFormat)
{
    if(!CreateColumn.is_open())
    {
        UnicodeString Store = "";
        UnicodeString Format = ",";
        if(FileFormat == "DBF")
        {
            std::string InsertColumn;
            std::string Column[21] = { "TRANDATE", "OLDGT", "NEWGT", "DLYSALE", "TOTDISC",
                                       "TOTREF", "TOTCAN", "VAT", "TENTNAME", "BEGINV",
                                       "ENDINV", "BEGOR", "ENDOR", "TRANCNT", "LOCALTX",
                                       "SERVCHARGE", "NOTAXSALE", "RAWGROSS", "DLYLOCTAX",
                                       "OTHERS", "TERMNUM" };
            for(int i=0;i<21;i++)
            {
                Store = Column[i].c_str();
                DataToWrite.push_back(Store.t_str());
            }
        }
        else
        {
            Store = "TRANDATE" + Format + "OLDGT" + Format + "NEWGT" + Format + "DLYSALE" + Format +
                    "TOTDISC" + Format + "TOTREF" + Format + "TOTCAN" + Format + "VAT" + Format +
                    "TENTNAME" + Format + "BEGINV" + Format + "ENDINV" + Format + "BEGOR" + Format +
                    "ENDOR" + Format + "TRANCNT" + Format + "LOCALTX" + Format + "SERVCHARGE" + Format +
                    "NOTAXSALE" + Format + "RAWGROSS" + Format + "DLYLOCTAX" + Format + "OTHERS" + Format + "TERMNUM" + "\n";
            DataToWrite.push_back(Store.t_str());
        }
    }
}
//---------------------------------------------------------------------------

// This function is for arranging the format of the output file for bith ZED and HOURLY export files
TExportResponse TMallExportAyalaMall::CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead,
                                                 std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName,
                                                 UnicodeString PrevDate, UnicodeString PrevDateYesterday, bool PrevDateFlag,
                                                 UnicodeString FileFormat)
{
    int i = 0;
    UnicodeString OldGT = "";
    UnicodeString NewGT = "";
    UnicodeString Format = "";
    UnicodeString OutputValue = "";
    UnicodeString Zeroes = "";
    UnicodeString StrCount = "";
    UnicodeString OutVal = "";
    TDateTime Yesterday = Now() - 1.0;
    unsigned short Hour = 0;
    unsigned short dummy = 0;

    Format = (FileFormat == "DBF") ? "                    " : ",";

    Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

    std::map<UnicodeString, UnicodeString>::iterator it;

    TExportResponse result;

    try
    {
        for(i=0;i<DataRead.size();i++)
        {
            switch(i)
            {
                case 0:
                    it = DataRead.find("TransactionDate");
                    break;
                case 1:
                    it = DataRead.find("GrandTotalOld");
                    break;
                case 2:
                    it = DataRead.find("GrandTotal");
                    break;
                case 3:
                    it = DataRead.find("DailySales");
                    break;
                case 4:
                    it = DataRead.find("TotalDiscount");
                    break;
                case 5:
                    it = DataRead.find("TotalRefund");
                    break;
                case 6:
                    it = DataRead.find("TotalCancelled");
                    break;
                case 7:
                    it = DataRead.find("TotalSaleTax");
                    break;
                case 8:
                    it = DataRead.find("MallCode"); // Should be Tenant No.
                    // (it->second) = TGlobalSettings::Instance().BranchCode;
                    break;
                case 9:
                    it = DataRead.find("BeginningInvoiceNo");
                    break;
                case 10:
                    it = DataRead.find("EndingInvoiveNo");
                    break;
                case 11:
                    // it = DataRead.find("BeginningORNo");
                    it = DataRead.find("BeginningInvoiceNo");
                    break;
                case 12:
                    // it = DataRead.find("EndingORNo");
                    it = DataRead.find("EndingInvoiveNo");
                    break;
                case 13:
                    it = DataRead.find("TransactionCount");
                    break;
                case 14:
                    it = DataRead.find("LocalTaxes");
                    break;
                case 15:
                    it = DataRead.find("TotalServiceCharge");
                    break;
                case 16:
                    it = DataRead.find("TaxExemptSales");
                    break;
                case 17:
                    it = DataRead.find("TotalGrossSale");
                    break;
                case 18:
                    it = DataRead.find("LocalTaxExemptDailySales");
                    break;
                case 19:
                    it = DataRead.find("FineDiningCustomerCount");
                    break;
                case 20:
                    // it = DataRead.find("TerminalNo");
                    (it->second) = TGlobalSettings::Instance().TerminalNo;
                    break;
                default:
                    break;
            }

            if((i>=9) && (i<=12))
            {
                StrCount = (it->second);
            }

            Zeroes = "";
            for(int j=0;j<(6-StrCount.Length());j++)
            {
                Zeroes += "0";
            }

            if(i==0)
            {
                if(PrevDateFlag == true)
                {
                    OutputValue = (Hour > 5) ? PrevDate : PrevDateYesterday;
                }
                else
                {
                    Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];
                    if(gridBtn->Enabled == false)
                    {
                        if(FileFormat == "DBF")
                        {
                            OutputValue = Yesterday.FormatString("yyyymmdd");
                        }
                        else
                        {
                            OutputValue = Yesterday.FormatString("mm/dd/yyyy");
                        }
                    }
                    else
                    {
                        if(FileFormat == "DBF")
                        {
                            OutputValue = (Hour > 5) ? Now().FormatString("yyyymmdd") : Yesterday.FormatString("yyyymmdd");
                        }
                        else
                        {
                            OutputValue = (Hour > 5) ? Now().FormatString("mm/dd/yyyy") : Yesterday.FormatString("mm/dd/yyyy");
                        }
                    }
                }

                OutputValue = OutputValue + Format;
            }
            else if(i==1)
            {
                OldGT = (it->second);
                OutputValue = (it->second) + Format;
            }
            else if(i==2)
            {
                NewGT = (it->second);
                OutputValue = (it->second) + Format;
            }
            else if(i==8)
            {
                if(FileFormat == "DBF")
                {
                    OutputValue = (it->second) + Format;
                }
                else
                {
//                    UnicodeString MallCodeTemp = (it->second);
//                    UnicodeString MallCodeVal = "";
//                    std::string MallCodeSTR = MallCodeTemp.t_str();
//                    for(int i=0;i<MallCodeTemp.Length();i++)
//                    {
//                        if(i>3)
//                        {
//                            MallCodeVal = MallCodeVal + MallCodeSTR[i];
//                        }
//                    }
//                    OutputValue = MallCodeVal + Format;
                    OutputValue = TGlobalSettings::Instance().TenantNo + Format;
                }
            }
            else if((i>=9) && (i<=12))
            {
                if(OldGT.Compare(NewGT) != 0)
                {
                    if(FileFormat == "DBF")
                    {
                        OutputValue = Zeroes + (it->second) + Format;
                    }
                    else
                    {
                        OutputValue = (it->second) + Format;
                    }
                }
                else
                {
                    if(FileFormat == "DBF")
                    {
                        OutputValue = "0";
                    }
                    else
                    {
                        OutputValue = "0" + Format;
                    }
                }
            }
            else if(((it->second) == "0") || ((it->second) == ""))
            {
                if(i==13)
                {
                    OutputValue = "0" + Format;
                }
                else
                {
                    OutputValue = "0.00" + Format;
                }
            }
            else if(i==(DataRead.size() - 1))
            {
                if(FileFormat == "DBF")
                {
                    OutputValue = (it->second) + Format;
                }
                else
                {
                    OutputValue = (it->second);
                }
            }
            else
            {
                OutputValue = (it->second) + Format;
            }

            if(FileFormat == "DBF")
            {
                DataToWrite.push_back(OutputValue.t_str());
            }
            else
            {
                OutVal += OutputValue;
            }
        }

        if(FileFormat == "TXT")
        {
            OutVal += "\n";
            DataToWrite.push_back(OutVal.t_str());
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

// This method is used to create the header names for Hourly Export
// This method is separated to prevent multiple pos create multiple header files
void TMallExportAyalaMall::CreateHeaderFormatHourly(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite,
                                                    UnicodeString FileFormat)
{
    if(!OutputFile.is_open())
    {
        UnicodeString Store = "";
        UnicodeString Format = ",";
        if(FileFormat == "DBF")
        {
            std::string InsertColumn;
            std::string Column[6] = { "TRANDATE", "HOUR", "SALES", "TRANCNT", "TENTNAME", "TERMNUM"};

            for(int i=0;i<6;i++)
            {
                Store = Column[i].c_str();
                DataToWrite.push_back(Store.t_str());
            }
        }
        else
        {
            OutputFile.open(MallPathFileName.c_str());
            if(!OutputFile.is_open())
            {
                Store = "TRANDATE" + Format + "HOUR" + Format + "SALES" + Format + "TRANCNT" + Format +
                        "TENTNAME" + Format + "TERMNUM" + "\n";
                DataToWrite.push_back(Store.t_str());
            }
            else
            {
                DataToWrite.clear();
            }
            OutputFile.close();
        }
    }
}
//---------------------------------------------------------------------------

// This method creates the hourly data for hourly export file
TExportResponse TMallExportAyalaMall::CreateOutputFormatHourly(std::map<UnicodeString, UnicodeString> DataRead,
                            std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName,
                            UnicodeString PrevDate, UnicodeString PrevDateTomorrow, UnicodeString PrevDateYesterday,
                            bool PrevDateFlag, UnicodeString FileFormat)
{
    UnicodeString Format = "";
    UnicodeString OutputValue = "";

    UnicodeString TerminalNum = "";
    UnicodeString TerminalName = "";
    UnicodeString TenantName = "";
    UnicodeString DateValue = "";
    UnicodeString TimeValue = "";
    Currency AmountValue = 0;
    int TransactionTotal = 0;
    UnicodeString Amount = "";
    UnicodeString OutVal = "";

    unsigned short Hour = 0;
    unsigned short dummy = 0;

    Format = (FileFormat == "DBF") ? "                    " : ",";

    Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

    DataToWrite.clear();

    std::map<UnicodeString, UnicodeString>::iterator it;

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

        TerminalNum = TGlobalSettings::Instance().TerminalNo;

        query->SQL->Text = "SELECT * FROM MALLEXPORT_HOURLY WHERE TERMINAL_NAME=:TERMINALNAME "
                           "ORDER BY DATE_VALUE, TIME_VALUE ASC";
        DBTransaction.StartTransaction();
        query->ParamByName("TERMINALNAME")->AsString = TerminalNum;
        query->ExecQuery();

        while(!query->Eof)
        {
            TenantName = query->FieldByName("TENANT_NAME")->AsString;
            DateValue = query->FieldByName("DATE_VALUE")->AsString;
            TimeValue = query->FieldByName("TIME_VALUE")->AsString;
            AmountValue = query->FieldByName("AMOUNT_VALUE")->AsCurrency;

            TenantName = (FileFormat == "TXT") ? TGlobalSettings::Instance().TenantNo : TenantName;

            DataRead[TimeValue] = GetHourlyFormat(TerminalNum, TenantName, DateValue, TimeValue);

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
                DataRead[Time] = GetHourlyFormat(TerminalNum, TenantName, DateValue, TimeValue);
            }

            it = DataRead.find(Time);
            OutputValue = (it->second);

            if((i>=18) && (i<=23))
            {
                if(PrevDateFlag == true)
                {
                    DateValue = (Hour < 6) ? PrevDate : PrevDateTomorrow;
                }
                else
                {
                    if(FileFormat == "DBF")
                    {
                        DateValue = (Hour < 6) ? Now().FormatString("yyyymmdd") : Tomorrow.FormatString("yyyymmdd");
                    }
                    else
                    {
                        DateValue = (Hour < 6) ? Now().FormatString("mm/dd/yyyy") : Tomorrow.FormatString("mm/dd/yyyy");
                    }
                }
            }
            else
            {
                if(PrevDateFlag == true)
                {
                    DateValue = (Hour > 5) ? PrevDate : PrevDateYesterday;
                }
                else
                {
                    if(FileFormat == "DBF")
                    {
                        DateValue = (Hour > 5) ? Now().FormatString("yyyymmdd") : Yesterday.FormatString("yyyymmdd");
                    }
                    else
                    {
                        DateValue = (Hour > 5) ? Now().FormatString("mm/dd/yyyy") : Yesterday.FormatString("mm/dd/yyyy");
                    }
                }
            }

            if(TenantName == "")
            {
                TenantName = TGlobalSettings::Instance().BranchCode;
            }

            GetHourlyData(TerminalNum, TenantName, DateValue, Time, TransactionTotal, Amount);

            for(int i=0;i<6;i++)
            {
                if(i==0)
                {
                    OutputValue = DateValue + Format;
                }
                else if(i==1)
                {
                    OutputValue = Time + ":00" + Format;
                }
                else if(i==2)
                {
                    if(TransactionTotal != 0)
                    {
                        OutputValue = Amount + Format;
                    }
                    else
                    {
                        OutputValue = "0.00" + Format;
                    }
                }
                else if(i==3)
                {
                    if(FileFormat == "DBF")
                    {
                        if(TransactionTotal != 0)
                        {
                            OutputValue = TransactionTotal;
                        }
                        else
                        {
                            OutputValue = "0";
                        }
                    }
                    else
                    {
                        if(TransactionTotal != 0)
                        {
                            OutputValue = TransactionTotal + ",";
                        }
                        else
                        {
                            OutputValue = "0" + Format;
                        }
                    }
                }
                else if(i==4)
                {
                    if(FileFormat == "DBF")
                    {
                        OutputValue = TenantName + Format;
                    }
                    else
                    {
//                        UnicodeString MallCodeTemp = TenantName;
//                        UnicodeString MallCodeVal = "";
//                        std::string MallCodeSTR = MallCodeTemp.t_str();
//                        for(int i=0;i<MallCodeTemp.Length();i++)
//                        {
//                            if(i>3)
//                            {
//                                MallCodeVal = MallCodeVal + MallCodeSTR[i];
//                            }
//                        }
//                        OutputValue = MallCodeVal + Format;
                        OutputValue = TGlobalSettings::Instance().TenantNo + Format;
                    }
                }
                else
                {
                    if(FileFormat == "DBF")
                    {
                        OutputValue = TerminalNum + Format;
                    }
                    else
                    {
                        OutputValue = TerminalNum + "\n";
                    }
                }

                if(FileFormat == "DBF")
                {
                    DataToWrite.push_back(OutputValue.t_str());
                }
                else
                {
                    OutVal += OutputValue;
                }
            }

            if(FileFormat == "DBF")
            {
                TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "HDATA");
                DataToWrite.clear();
            }
            else
            {
                DataToWrite.push_back(OutVal.t_str());
                OutVal = "";
                TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName, "HTXTDATA");
                DataToWrite.clear();
            }
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

// Get Time format for Ayala Export File
UnicodeString TMallExportAyalaMall::GetTime(UnicodeString DateTime)
{
    AnsiString time = "";

    try
    {
        TDateTime date(DateTime);
        time = date.FormatString("hh") + ":" + date.FormatString("nn");
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// Get Date format for Ayala Export File
UnicodeString TMallExportAyalaMall::GetDate(UnicodeString DateTime)
{
    AnsiString time = "";
    UnicodeString LastUsedDate = TGlobalSettings::Instance().LastTransactionDate;
    UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
    TDateTime Temp;
    TDateTime CurrentDate;
    TDateTime LastDate;
    TDateTime Yesterday;
    int DayCount = 0;

    try
    {
        LastDate = StrToDateTime(LastUsedDate);
        CurrentDate = StrToDateTime(CheckDate);

        while(LastDate<CurrentDate)
        {
            DayCount++;
            Temp = Now() - DayCount;
            CheckDate = Temp.FormatString("mm/dd/yy");
            CurrentDate = StrToDateTime(CheckDate);
         }

        if(DayCount >= 1)
        {
            Yesterday = Now() - DayCount;
            time = Yesterday.FormatString("yyyymmdd");
        }
        else
        {
            TDateTime date(DateTime);
            time = date.FormatString("yyyymmdd");
            if(time == "18991230")
            {
                time = Now().FormatString("yyyymmdd");
            }
        }
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// This function is for saving the integer global variable
void TMallExportAyalaMall::SaveIntVariable(vmVariables vmVar, int CompName)
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

void TMallExportAyalaMall::SaveBoolVariable(vmVariables vmVar, bool CompName)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }
    TManagerVariable::Instance().SetProfileBool(DBTransaction, GlobalProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------

// This Function is for saving each string component into the DB for every function call
void TMallExportAyalaMall::SaveStrVariable(vmVariables vmVar, UnicodeString CompName)
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

TExportResponse TMallExportAyalaMall::GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                                    UnicodeString &TerminalNo, UnicodeString &FileExt,
                                                    UnicodeString &DateFormat)
{
    TExportResponse result;

    try
    {
        std::string TenantNameSTR = "";
        UnicodeString TenantName = TGlobalSettings::Instance().BranchCode;
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString Day = Yesterday.FormatString("dd");
        UnicodeString Month = Yesterday.FormatString("mm");
        UnicodeString Year = Now().FormatString("yyyy");

        MallPath = CheckDir(MallPath);
        MallPath = CreateYearlyFolder(MallPath, Year);

        TenantNameSTR = TenantName.t_str();
        FileExt = ".dbf";
        for(int i=0;i<TenantName.Length();i++)
        {
            if(i<3)
            {
                TenantID = TenantID + TenantNameSTR[i];
            }
        }

        if (TerminalNo.Length() == 1)
        {
            TerminalNo = "0" + TerminalNo;
        }

        DateFormat = Month + Day;

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

// Checking of previous file will happen in 6:00 AM if the POS will not be exited
void TMallExportAyalaMall::SetZedTimer()
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

void TMallExportAyalaMall::CheckPreviousEODFile()
{
    UnicodeString CheckDate = Now().FormatString("mmddyy");
    UnicodeString TenantID = "";
    UnicodeString FileExt = "";
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    UnicodeString DateFormat = "";
    UnicodeString MallPath = FilePath;
    UnicodeString LastZedDate = TGlobalSettings::Instance().LastZedDate;
    UnicodeString LastZedTime = TGlobalSettings::Instance().LastZedTime;
    UnicodeString CurrentDate = Now().FormatString("mm/dd/yy");
    UnicodeString YesterdayDate = "";
    TDateTime Yesterday = Now() - 1.0;
    bool ZReportStatus = false;
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    // Checking of previous file will happen in 6:00 AM if the POS will not be exited
    zedcheckTimer->Interval = ((108000000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));

    GetExportInfo(MallPath, TenantID, TerminalNo, FileExt, DateFormat);
    ZReportStatus = CheckPreviousExportFile(MallPath, TenantID, FileExt, DateFormat, "ZED");

    YesterdayDate = Yesterday.FormatString("mm/dd/yy");

    // Old Implementation
    // if(LastZedDate == "")
    // {
    //     ZReportStatus = false;
    // }
    // else if(YesterdayDate.Compare(LastZedDate) != 0)
    // {
    //     if(CurrentDate.Compare(LastZedDate) != 0)
    //     {
    //         ZReportStatus = false;
    //     }
    //     else
    //     {
    //         ZReportStatus = (StrToInt(LastZedTime) > 5) ? true : false;
    //     }
    // }
    // else
    // {
    //     ZReportStatus = (StrToInt(LastZedTime) > 5) ? true : false;
    // }

    if(Hour >= 6)
    {
        if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0))
        {
            frmMain->ZForced();
        }
    }
}
//---------------------------------------------------------------------------

// This method is used to copy the content of the export file. It is used for retaining
// the old values and then add the new value in the text file so it will not overwrite the file
TExportResponse TMallExportAyalaMall::CopyToBackUpPath(UnicodeString LocalPath, UnicodeString LocalPathFileName,
                                                       UnicodeString MallPath, UnicodeString MallPathFileName)
{
    TExportResponse result;

    try
    {
        UnicodeString LocalOutput = "";
        UnicodeString Year = Now().FormatString("yyyy");
        if(!DirectoryExists(MallPath))
        {
            MessageBox( "Sales files was not sent to server. Please check network connections!", "File was not sent!", MB_OK );
        }
        else
        {
            LocalPath = CheckDir(LocalPath);
            LocalPath = CreateYearlyFolder(LocalPath, Year);
            LocalOutput = LocalPath + LocalPathFileName;
            // Copy the file from local path to the mall path
            CopyProcedure(MallPathFileName, LocalOutput);
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

// This is used for resending the data that can be performed by the user in the settings
// This method generate all the export files that is located in the backup folder
void TMallExportAyalaMall::GetReportsList(TStringList* reportsList, UnicodeString folder)
{
    UnicodeString LocalPath = "";
    std::vector<UnicodeString> files;

    if(folder == "")
    {
        LocalPath = BasePath + "\\AyalaMall\\";
    }
    else
    {
        LocalPath = BasePath + "\\AyalaMall\\" + folder + "\\";
    }

    DIR* dir = opendir(LocalPath.t_str());

    std::string getName;
    UnicodeString fileExt = "";

    while ( (entry = readdir(dir)) != NULL)
    {
        files.push_back(std::string(entry->d_name).c_str());
    }

    for (int i = 0;i < files.size();i++)
    {
        getName = files.at(i).t_str();
        if((files.at(i) == ".") || (files.at(i) == ".."))
        {
            continue;
        }
        else if(((files.at(i)).Length() > 2) && ((files.at(i)).Length() < 5))
        {
            reportsList->Add(files.at(i).t_str());
        }
        else
        {
            for (int i = getName.length() - 3 ; i<getName.length(); i++)
            {
                fileExt = fileExt + getName[i];
            }
            if((fileExt == "dbf") || (fileExt == "DBF"))
            {
                reportsList->Add(files.at(i).t_str());
            }
            fileExt = "";
        }
    }
}
//---------------------------------------------------------------------------

// This method is used to resend the reports, it when the user select a file/multiple file
// the system will check if that files does exists in the backup data and then send it to
// the mall path if the path is available
void TMallExportAyalaMall::ResendReports(TStringList* resendReports, UnicodeString folder, bool status)
{

    UnicodeString Year = Now().FormatString("yyyy");
    UnicodeString LocalPath = BasePath + "\\AyalaMall\\" + folder + "\\";
    UnicodeString MallPath = TGlobalSettings::Instance().MallPath;
    UnicodeString MallPathFiles = "";
    UnicodeString LocalPathFiles = "";

    if(status == false)
    {
        if(folder.Length() != 0)
        {
            if(!DirectoryExists(MallPath))
            {
                MessageBox( "Sales files was not sent to server. Please check network connections!", "File was not sent!", MB_OK );
            }
            else
            {
                MessageBox( "Sales file successfully resend to server.", "File has been sent!", MB_OK );
                MallPath = MallPath + folder + "\\";
                if(!DirectoryExists(MallPath))
                {
                    CreateDir(MallPath);
                }
            }

            for(int i=0; i<resendReports->Count;i++)
            {
                MallPathFiles = MallPath + resendReports->Strings[i];
                LocalPathFiles = LocalPath + resendReports->Strings[i];
                if(DirectoryExists(MallPath))
                {
                    CopyProcedure(LocalPathFiles, MallPathFiles);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

// This method copy a file from one place to another
void TMallExportAyalaMall::CopyProcedure(UnicodeString in, UnicodeString out)
{
    std::ifstream input(in.c_str());
    std::ofstream output(out.c_str());
    output << input.rdbuf();
}
//---------------------------------------------------------------------------

// This method checks the Local Path if it is already exists or not
// This is used in resend function
bool TMallExportAyalaMall::CheckBackUpPath()
{
    UnicodeString LocalPath = BasePath + "\\AyalaMall\\";

    return (DirectoryExists(LocalPath)) ? true : false;
}
//---------------------------------------------------------------------------

// This method will generate the output format for hourly export file
UnicodeString TMallExportAyalaMall::GetHourlyFormat(UnicodeString TerminalName, UnicodeString TenantName,
                                                    UnicodeString DateValue, UnicodeString TimeValue)
{
    int TransactionTotal = 0;
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";
    UnicodeString Amount = "";

    GetHourlyData(TerminalName, TenantName, DateValue, TimeValue, TransactionTotal, Amount);

    if(TransactionTotal != 0)
    {
        result = DateValue + TimeValue + ":00" + Amount + TransactionTotal + TenantName + TerminalName;
    }
    else
    {
        result = DateValue + TimeValue + ":00" + "0.00" + "0" + TenantName + TerminalName;
    }

    return result;
}
//---------------------------------------------------------------------------

// This method will generate the data for hourly export file
UnicodeString TMallExportAyalaMall::GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                  UnicodeString &DateValue, UnicodeString &TimeValue,
                                                  int &TransactionTotal, UnicodeString &Amount)
{
    Currency AmountTotal = 0;
    UnicodeString result = "";
    UnicodeString Format = "";

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

    query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM FROM MALLEXPORT_HOURLY "
                       "WHERE TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME";
    DBTransaction.StartTransaction();
    query->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query->ParamByName("TERMINALNAME")->AsString = TerminalName;
    query->ExecQuery();

    AmountTotal = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    TransactionTotal = RoundToNearest(query->FieldByName("TRANSSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);

    Amount = FixDecimalPlaces(AmountTotal);

    DBTransaction.Commit();
    query->Close();

    return result;
}
//---------------------------------------------------------------------------

// This method fixes the decimal point of a currency value
UnicodeString TMallExportAyalaMall::FixDecimalPlaces(Currency AmountTotal)
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

// This will check if the previous zed report is already exists or not
bool TMallExportAyalaMall::CheckPreviousZed()
{
    UnicodeString MallPath = FilePath;
    UnicodeString TenantID = "";
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    UnicodeString FileExt = ".dbf";
    UnicodeString DateFormat = "";
    bool result = false;

    GetExportInfo(MallPath, TenantID, TerminalNo, FileExt, DateFormat);
    result = CheckPreviousExportFile(MallPath, TenantID, FileExt, DateFormat, "ZED");

    return result;
}
//---------------------------------------------------------------------------

// This method generates the export file for the days it was not used, if the last day the pos
// was uses does not performed eod, this will generate that export file and the consecutive days
// that it was not used. The data will be zeroes if there are no information available
void TMallExportAyalaMall::GenerateFileForDaysPOSNotUsed(UnicodeString LocalPath, UnicodeString MallPath,
                                                         UnicodeString TerminalNo, UnicodeString CheckDate,
                                                         UnicodeString TenantID, UnicodeString FileExt,
                                                         UnicodeString DateFormat)
{
    try
    {
        TMallExportUpdateAdaptor exportUpdateAdaptor;
        TMallExportHourlyUpdate exportHourlyUpdate;

        UnicodeString LastUsedDate = TGlobalSettings::Instance().LastTransactionDate;
        UnicodeString LastZedDate = TGlobalSettings::Instance().LastZedDate;
        UnicodeString LastZedTime = TGlobalSettings::Instance().LastZedTime;
        UnicodeString ContractNum = TGlobalSettings::Instance().SerialNo;
        UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
        UnicodeString LocalFileName = "";
        UnicodeString MallFileName = "";
        UnicodeString LocalFileNameH = "";
        UnicodeString MallFileNameH = "";
        UnicodeString LocalFileNameTXT = "";
        UnicodeString MallFileNameTXT = "";
        UnicodeString LocalFileNameHTXT = "";
        UnicodeString MallFileNameHTXT = "";
        UnicodeString DateParam = "";
        UnicodeString DateParamTomorrow = "";
        UnicodeString DateParamYesterday = "";
        UnicodeString DateParamTXT = "";
        UnicodeString DateParamTomorrowTXT = "";
        UnicodeString DateParamYesterdayTXT = "";
        UnicodeString MPYear = "";
        UnicodeString LPYear = "";
        UnicodeString InitMallPath = MallPath;
        UnicodeString InitLocalPath = LocalPath;
        UnicodeString DateComp = "";
        TDateTime PrevDate;
        TDateTime IncrementDate;
        TDateTime DateDataTomorrow;
        TDateTime DateDataYesterday;
        TDateTime Yesterday = Now() - 2.0;
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
                    DateParamTXT = IncrementDate.FormatString("mm/dd/yyyy");
                    DateParamTomorrowTXT = DateDataTomorrow.FormatString("mm/dd/yyyy");
                    DateParamYesterdayTXT = DateDataYesterday.FormatString("mm/dd/yyyy");
                    DateFormat = IncrementDate.FormatString("mmdd");
                    MPYear = IncrementDate.FormatString("yyyy");
                    LPYear = IncrementDate.FormatString("yyyy");
                    MallPath = "";
                    MallPath = InitMallPath;
                    MallPath = CheckDir(MallPath);
                    MallPath = CreateYearlyFolder(MallPath, MPYear);
                    LocalPath = "";
                    LocalPath = InitLocalPath;
                    LocalPath = CheckDir(LocalPath);
                    LocalPath = CreateYearlyFolder(LocalPath, LPYear);
                    LocalFileName = LocalPath + TenantID + DateFormat + FileExt;
                    MallFileName = MallPath + TenantID + DateFormat + FileExt;
                    LocalFileNameH = LocalPath + TenantID + DateFormat + "H" + FileExt;
                    MallFileNameH = MallPath + TenantID + DateFormat + "H" + FileExt;

                    LocalFileNameTXT = LocalPath + ContractNum + DateFormat + ".txt";
                    MallFileNameTXT = MallPath + ContractNum + DateFormat + ".txt";
                    LocalFileNameHTXT = LocalPath + ContractNum + DateFormat + "H" + ".txt";
                    MallFileNameHTXT = MallPath + ContractNum + DateFormat + "H" + ".txt";

                    CheckFile.open(MallFileName.c_str(), std::ios_base::out | std::ios_base::in);

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

                        DataRead.clear();
                        DataToWrite.clear();
                        DataQuery = getAyalaQuery("ZED");
                        InputManager->ReadFromDB(DataQuery,DataRead);
                        // For Mall Path Header
                        CreateHeaderFormatZed(MallFileName,DataToWrite, "DBF");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileName, "ZHEADER");
                        DataToWrite.clear();
                        // For Local Path Header
                        // CreateHeaderFormatZed(LocalFileName,DataToWrite, "DBF");
                        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileName, "ZHEADER");
                        // DataToWrite.clear();

                        // For Mall Path
                        CreateOutputFormatZed(DataRead, DataToWrite, MallFileName, DateParam, DateParamYesterday, true, "DBF");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileName, "ZDATA");
                        DataToWrite.clear();
                        // For Local Path Data
                        // InputManager->ReadFromDB(DataQuery,DataRead);
                        // if(!DirectoryExists(LocalPath))
                        // {
                        //     CreateDir(LocalPath);
                        // }
                        // CreateOutputFormatZed(DataRead, DataToWrite, LocalFileName, DateParam, DateParamYesterday, true, "DBF");
                        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileName, "ZDATA");

                        DataRead.clear();
                        DataToWrite.clear();


                        // For TXT File
                        DataQuery = getAyalaQuery("ZED");
                        InputManager->ReadFromDB(DataQuery,DataRead);

                        // For Mall Path Header
                        CreateHeaderFormatZed(MallFileNameTXT,DataToWrite, "TXT");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameTXT, "ZTXTHEADER");
                        DataToWrite.clear();
                        // For Local Path Header
                        // CreateHeaderFormatZed(LocalFileNameTXT,DataToWrite, "TXT");
                        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileNameTXT, "ZTXTHEADER");
                        // DataToWrite.clear();

                        // For Mall Path Data
                        CreateOutputFormatZed(DataRead, DataToWrite, MallFileNameTXT, DateParamTXT, DateParamYesterdayTXT, true, "TXT");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameTXT, "ZTXTDATA");
                        DataToWrite.clear();
                        // For Local Path Data
                        // CreateOutputFormatZed(DataRead, DataToWrite, LocalFileNameTXT, DateParamTXT, DateParamYesterdayTXT, true, "TXT");
                        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileNameTXT, "ZTXTDATA");
                        // DataToWrite.clear();

                        TGlobalSettings::Instance().ZCount += 1;
                        SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);

                        exportUpdateAdaptor.ResetExportTables();

                        TGlobalSettings::Instance().LastZedDate = IncrementDate.FormatString("mm/dd/yy");
                        SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
                        TGlobalSettings::Instance().LastZedTime = Now().FormatString("hh");
                        SaveStrVariable(vmLastZedTime, TGlobalSettings::Instance().LastZedTime);
                    }
                    CheckFile.close();

                    CheckFile.open(MallFileNameH.c_str());

                    if(!CheckPreviousZedDateTime())
                    {
                        DataToWrite.clear();
                        // For Mall Path
                        CreateHeaderFormatHourly(MallFileNameH, DataToWrite, "DBF");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameH, "HHEADER");
                        DataToWrite.clear();
                        // For Local Path
                        // CreateHeaderFormatHourly(LocalFileNameH, DataToWrite, "DBF");
                        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileNameH, "HHEADER");
                        // DataToWrite.clear();

                        // For Mall Path
                        CreateOutputFormatHourly(DataRead, DataToWrite, MallFileNameH, DateParam, DateParamTomorrow, DateParamYesterday, true, "DBF");
                        if(!DirectoryExists(LocalPath))
                        {
                            CreateDir(LocalPath);
                        }
                        // For Local Path
                        // CreateOutputFormatHourly(DataRead, DataToWrite, LocalFileNameH, DateParam, DateParamTomorrow, DateParamYesterday, true, "DBF");
                        // DataToWrite.clear();

                        // For TXT File
                        // For Mall Path
                        CreateHeaderFormatHourly(MallFileNameHTXT, DataToWrite, "TXT");
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallFileNameHTXT, "HTXTHEADER");
                        DataToWrite.clear();
                        // For Local Path
                        // CreateHeaderFormatHourly(LocalFileNameHTXT, DataToWrite, "TXT");
                        // TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileNameHTXT, "HTXTHEADER");
                        // DataToWrite.clear();

                        // For Mall Path
                        CreateOutputFormatHourly(DataRead, DataToWrite, MallFileNameHTXT, DateParamTXT, DateParamTomorrowTXT, DateParamYesterdayTXT, true, "TXT");
                        DataToWrite.clear();
                        // For Local Path
                        // CreateOutputFormatHourly(DataRead, DataToWrite, LocalFileNameHTXT, DateParamTXT, DateParamTomorrowTXT, DateParamYesterdayTXT, true, "TXT");
                        // DataToWrite.clear();

                        exportHourlyUpdate.ResetHourlyExportTablesOnZed();
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
bool TMallExportAyalaMall::CheckPreviousZedDateTime()
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
    int DayCount = 0;

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


