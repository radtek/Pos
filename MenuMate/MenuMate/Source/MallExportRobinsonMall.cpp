//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportRobinsonMall.h"
#include "GlobalSettings.h"
#include "MallExportDataManager.h"
#include "MallExportUpdateAdaptor.h"
#include "DeviceRealTerminal.h"
#include "Main.h"
#include "SelectDish.h"
#include <sstream>
#include <string>
#include <vector>
#include <ios>
#include <dirent.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportRobinsonMall::TMallExportRobinsonMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportRobinsonMall::~TMallExportRobinsonMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportRobinsonMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Delete this method if not needed
TExportResponse TMallExportRobinsonMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        UnicodeString Year = Now().FormatString("yyyy");
        UnicodeString MallPath = FFileURI;
        UnicodeString LocalPathFileName = "";
        UnicodeString LocalPathPrevName = "";
        UnicodeString LocalPath = BasePath + "\\RobinsonMall\\";
        UnicodeString FailedPath = LocalPath  + Year + "\\Failed Upload\\";
        UnicodeString FailedPathFileName = FailedPath;
        UnicodeString FailedPathPrevFileName = "";
        bool PrevZReportStatus = false;
        bool CurrentZReportStatus = false;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        DataQuery = getRobinsonQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename(LocalPathFileName, LocalPathPrevName, LocalPath, FailedPathFileName, FailedPathPrevFileName);
        if(TGlobalSettings::Instance().BatchNo == 1)
        {
            CreateOutputFormat(DataRead, DataToWrite, "", false);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);
            TGlobalSettings::Instance().ZCount += 1;
            SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);
        }

        TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mm/dd/yy");
        SaveStrVariable(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);

        CopyToMallLocation(FFileURI, LocalPathFileName, MallPath, LocalPathPrevName, FailedPath, FailedPathFileName, FailedPathPrevFileName);

        PrevZReportStatus = CheckPreviousZed();
        CurrentZReportStatus = CheckTodaysZed();

        if(PrevZReportStatus)
        {
            frmMain->EnablePOSBtn(true);
        }

        (CurrentZReportStatus) ? frmMain->EnablePOSBtn(false) : frmMain->EnablePOSBtn(true);

        if((Hour > 3) && (Hour < 9))
        {
            frmMain->EnablePOSBtn( false );
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
    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

TExportResponse TMallExportRobinsonMall::TransactionExport()
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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS

// Virtual methods
void TMallExportRobinsonMall::initTimingExportResponse( TExportResponse& outResponse )
{
    // TODO: Initiate the response for Robinson

    setExportResponse(
       false,               // Succesful = false
       emrExportNotDoneYet, // Result = emrExportNotDoneYet
       "",                  // Message
       "",                  // Description
       outResponse );
}
//---------------------------------------------------------------------------

TExportResponse TMallExportRobinsonMall::timingExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString Year = Now().FormatString("yyyy");
        UnicodeString LocalPath = BasePath + "\\RobinsonMall\\" + Year + "\\";
        UnicodeString TenantID = "";
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString FileExt = ".txt";
        UnicodeString DateFormat = "";
        UnicodeString LocalPathFileName = "";
        UnicodeString CheckDate = Now().FormatString("mmddyy");
        int BatchNo = TGlobalSettings::Instance().BatchNo;
        bool ZReportStatus = false;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        GetExportInfo(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);

        if((Hour > 3) && (Hour < 9))
        {
            frmMain->EnablePOSBtn(false);
        }
        else if ((Hour > 8) || (Hour < 4))
        {
            DateFormat = (Hour < 4) ? Yesterday.FormatString("mmdd") : Now().FormatString("mmdd");
            LocalPathFileName = LocalPath + TenantID + DateFormat + "." + TerminalNo + BatchNo + FileExt;

            if(!DirectoryExists(LocalPath))
            {
                frmMain->EnablePOSBtn(true);
            }
            else
            {
                CheckFile.open(LocalPathFileName.c_str(), std::ios_base::out | std::ios_base::in);

                if (!CheckFile.is_open())
                {
                    frmMain->EnablePOSBtn(true);
                }
            }
        }

        ZReportStatus = CheckPreviousExportFile(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);

        if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0) &&
            TGlobalSettings::Instance().FirstMallSet == false)
        {
            frmMain->EnablePOSBtn(false);
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
    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

// This function is for Creating Filename for Robinson Mall
TExportResponse TMallExportRobinsonMall::CreateFilename(UnicodeString &FileName, UnicodeString &PrevName,
                                                        UnicodeString LocalPath, UnicodeString &FailedPathFileName,
                                                        UnicodeString &FailedPathPrevFileName)
{
    TExportResponse result;

    try
    {
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString CheckDate = Now().FormatString("mmddyy");
        UnicodeString DateFormat = "";
        UnicodeString TenantID = "";
        UnicodeString CheckName = "";
        UnicodeString FileExt = ".txt";
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
        UnicodeString prevBatchNo = "";
        std::string TenantNameSTR = TenantName.t_str();
        bool ZReportStatus = false;
        int BatchNo = TGlobalSettings::Instance().BatchNo;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        // This condition is to get the last 4 digit of Tenant ID
        // This is required for the filename of the export file for
        // Robinson's Mall
        for(int i=0;i<TenantName.Length();i++)
        {
            if(i>=TenantName.Length() - 4)
            {
                TenantID = TenantID + TenantNameSTR[i];
            }
        }

        // This condition is only to check and correct if the terminal number
        // is only a single digit, if it is single digit it will add 0 at the
        // beginning, this is based on the example that Robinson's Mall
        if (TerminalNo.Length() == 1)
        {
            TerminalNo = "0" + TerminalNo;
        }

        LocalPath = CheckDir(LocalPath);
        LocalPath = CreateYearlyFolder(LocalPath);

        // This condition is to make sure what mode is being used and to get the
        // system date needed for text file filename
        if(TGlobalSettings::Instance().FirstMallSet != true && TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0)
        {
            ZReportStatus = CheckPreviousExportFile(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);
        }
        else
        {
            ZReportStatus = true;
            TGlobalSettings::Instance().FirstMallSet = false;
            SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
        }

        // This is robinsons requirement, so the text file coverage will be until 4 am
        // the next day
        if(Hour<4)
        {
            DateFormat = Yesterday.FormatString("mmdd");
        }
        else
        {
            DateFormat = (ZReportStatus) ? Now().FormatString("mmdd") : DateFormat;
        }

        // This is for checking if the file does already exist, if it is existing
        // this will create the same file with different batch no
        CheckName = LocalPath + TenantID + DateFormat + "." + TerminalNo + BatchNo + FileExt;
        CheckFile.open(CheckName.c_str(), std::ios_base::out | std::ios_base::in);

        if ((CheckFile.is_open()) && (ZReportStatus == true))
        {
            TGlobalSettings::Instance().BatchNo = TGlobalSettings::Instance().BatchNo + 1;
        }
        else
        {
            TGlobalSettings::Instance().BatchNo = 1;
        }
        CheckFile.close();

        BatchNo = TGlobalSettings::Instance().BatchNo;
        SaveIntVariable(vmBatchNo, TGlobalSettings::Instance().BatchNo);

        FileName = LocalPath + TenantID + DateFormat + "." + TerminalNo + BatchNo + FileExt;
        FFileURI = FFileURI + TenantID + DateFormat + "." + TerminalNo + BatchNo + FileExt;

        for(int i=0;i<TGlobalSettings::Instance().BatchNo;i++)
        {
            prevBatchNo = BatchNo - 1;
            CheckName = LocalPath + TenantID + DateFormat + "." + TerminalNo + prevBatchNo + FileExt;
            FailedPathPrevFileName = FailedPathFileName + TenantID + DateFormat + "." + TerminalNo + prevBatchNo + FileExt;
            CheckFile.open(CheckName.c_str(), std::ios_base::out | std::ios_base::in);

            if(CheckFile.is_open())
            {
                CheckFile.close();
                break;
            }
            CheckFile.close();
        }

        PrevName = CheckName;

        FailedPathFileName = FailedPathFileName + TenantID + DateFormat + "." + TerminalNo + BatchNo + FileExt;

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

// This function is for checking if the specified directory path is existing,
// if not, it will create local path called MallExport then create RobinsonMall
// Folder for ZedReport
UnicodeString TMallExportRobinsonMall::CheckDir(UnicodeString LocalPath)
{
    if(!DirectoryExists(LocalPath))
    {
        LocalPath = BasePath;
        if(!DirectoryExists(LocalPath))
        {
            CreateDir(LocalPath);
        }
        LocalPath = LocalPath + "\\RobinsonMall\\";
        if(!DirectoryExists(LocalPath))
        {
            CreateDir(LocalPath);
        }
    }
    return LocalPath;
}
//---------------------------------------------------------------------------

// This functions is for checking the previous EOD file dates
bool TMallExportRobinsonMall::CheckPreviousExportFile(UnicodeString LocalPath, UnicodeString TenantID, UnicodeString TerminalNo,
                                                      UnicodeString FileExt, UnicodeString &DateFormat)
{
    bool ZReportStatus = false;

    try
    {
        UnicodeString Filename = "";
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString Month = Yesterday.FormatString("mm");
        UnicodeString Day = Yesterday.FormatString("dd");
        UnicodeString Year = Yesterday.FormatString("yy");
        UnicodeString prevBatchNo = "";
        UnicodeString BatchNo = TGlobalSettings::Instance().BatchNo;

        DateFormat = Month + Day;

        for(int i=0;i<TGlobalSettings::Instance().BatchNo;i++)
        {
            Filename = LocalPath + TenantID + DateFormat + "." + TerminalNo + BatchNo + FileExt;
            CheckPreviousFile.open(Filename.c_str(), std::ios_base::out | std::ios_base::in);
            BatchNo = BatchNo - 1;

            if(CheckPreviousFile.is_open())
            {
                ZReportStatus = true;
                break;
            }
            else
            {
                ZReportStatus = false;
            }

            CheckPreviousFile.close();
        }
    }
    catch(Exception &ex)
    {
        //ignore the exception
    }

    return ZReportStatus;
}
//---------------------------------------------------------------------------

// This function is for saving the BatchNo to the DB required by Robinson Mall
void TMallExportRobinsonMall::SaveIntVariable(vmVariables vmVar, int CompName)
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

// This Function is for saving each string component into the DB for every function call
void TMallExportRobinsonMall::SaveStrVariable(vmVariables vmVar, UnicodeString CompName)
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

// This Function is for saving each bool component into the DB for every function call
void TMallExportRobinsonMall::SaveBoolVariable(vmVariables vmVar, bool CompName)
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
// This function is for getting query from the DB Table
UnicodeString TMallExportRobinsonMall::getRobinsonQuery()
{
    UnicodeString RobinsonQuery = "";
    RobinsonQuery = "SELECT * FROM ROBINSONS_MALLVIEW";

    return RobinsonQuery;
}
//---------------------------------------------------------------------------

// This function is for arranging the format of the output file
TExportResponse TMallExportRobinsonMall::CreateOutputFormat(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite,
                                                            UnicodeString PrevDate, bool PrevDateFlag)
{
    TExportResponse result;

    try
    {
        int i = 0;
        int RowNum = 0;
        UnicodeString ItemNo = "";
        UnicodeString Format = "\n";
        UnicodeString OutputValue = "";
        UnicodeString Zeroes = "";
        UnicodeString StrCount = "";
        std::ostringstream StreamIntToStr;
        std::string StringIntVal = "";

        std::map<UnicodeString, UnicodeString>::iterator it;

        for(i=0;i<DataRead.size();i++)
        {
            if(i==0)
            {
                it = DataRead.find("TenantNo");
            }
            else if(i==1)
            {
                it = DataRead.find("TerminalNo");
            }
            else if(i==2)
            {
                it = DataRead.find("TotalGrossSale");
            }
            else if(i==3)
            {
                it = DataRead.find("TotalSaleTax");
            }
            else if(i==4)
            {
                it = DataRead.find("TotalCancelled");
            }
            else if(i==5)
            {
                it = DataRead.find("TotalCancelledCount");
            }
            else if(i==6)
            {
                it = DataRead.find("TotalRegularDiscount");
            }
            else if(i==7)
            {
                it = DataRead.find("TotalRegularDiscountCount");
            }
            else if(i==8)
            {
                it = DataRead.find("TotalRefund");
            }
            else if(i==9)
            {
                it = DataRead.find("TotalRefundCount");
            }
            else if(i==10)
            {
                it = DataRead.find("TotalSeniorCitizenDiscount"); // Negative Adjustments
            }
            else if(i==11)
            {
                it = DataRead.find("TotalSeniorCitizenDiscountCount"); // Negative Adjustments Count
            }
            else if(i==12)
            {
                it = DataRead.find("TotalServiceCharge");
            }
            else if(i==13)
            {
                it = DataRead.find("TotalEndOfDayOld");
            }
            else if(i==14)
            {
                it = DataRead.find("GrandTotalOld");
            }
            else if(i==15)
            {
                it = DataRead.find("TotalEndOfDay");
            }
            else if(i==16)
            {
                it = DataRead.find("GrandTotal");
            }
            else if(i==17)
            {
                it = DataRead.find("Date");
            }
            else if(i==18)
            {
                it = DataRead.find("NoveltySales");
            }
            else if(i==19)
            {
                it = DataRead.find("MiscellaneusSales");
            }
            else if(i==20)
            {
                it = DataRead.find("LocalTaxes");
            }
            else if(i==21)
            {
                it = DataRead.find("ChargeCreditSales");
            }
            else if(i==22)
            {
                it = DataRead.find("ChargeCreditSalesTax");
            }
            else if(i==23)
            {
                it = DataRead.find("TaxExemptSales");
            }
            else if(i==24)
            {
                it = DataRead.find("PharmaSales");
            }
            else if(i==25)
            {
                it = DataRead.find("NonPharmaSales");
            }
            else if(i==26)
            {
                it = DataRead.find("DisabilityDiscount");
            }
            else if(i==27)
            {
                it = DataRead.find("SalesNotSubjectToRent");
            }
            else if(i==28)
            {
                it = DataRead.find("TotalReprintedTransc");
            }
            else if(i==29)
            {
                it = DataRead.find("TotalReprintedTranscCount");
            }

            if(i==17)
            {
                if(PrevDateFlag == true)
                {
                    StrCount = PrevDate;
                }
                else
                {
                    StrCount = GetDate((it->second));
                }
            }
            else
            {
                StrCount = (it->second);
            }

            RowNum = i+1;
            StreamIntToStr << RowNum;
            StringIntVal = StreamIntToStr.str();
            StreamIntToStr.str("");
            StreamIntToStr.clear();
            ItemNo = StringIntVal.c_str();
            if(ItemNo.Length() == 1)
            {
                ItemNo = "0" + ItemNo;
            }

            Zeroes = "";
            for(int j=0;j<(16-StrCount.Length());j++)
            {
                Zeroes += "0";
            }

            if(i==17)
            {
                if(PrevDateFlag == true)
                {
                    OutputValue = ItemNo + Zeroes + PrevDate + Format;
                }
                else
                {
                    OutputValue = ItemNo + Zeroes + GetDate((it->second)) + Format;
                }
            }
            else if (i==29)
            {
                OutputValue = ItemNo + Zeroes + (it->second);
            }
            else
            {
                OutputValue = ItemNo + Zeroes + (it->second) + Format;
            }
            DataToWrite.push_back(OutputValue.t_str());
            ItemNo = "";
        }

//        if(!DirectoryExists(FilePath))
//        {
//            MessageBox( "Sales file is not sent to RLC server.", "Please contact you POS vendor", MB_OK );
//        }
//        else
//        {
//            if(!PrevDateFlag)
//            {
//                MessageBox( "Sales file successfully sent to RLC server.", "", MB_OK );
//            }
//        }

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

// Get Date format for Robinson Export File
UnicodeString TMallExportRobinsonMall::GetDate(UnicodeString DateTime)
{
    AnsiString time = "";
    UnicodeString LastUsedDate = TGlobalSettings::Instance().LastTransactionDate;
    UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
    TDateTime Temp;
    TDateTime CurrentDate;
    TDateTime LastDate;
    int DayCount = 0;

    try
    {
        LastDate = StrToDateTime(LastUsedDate);
        CurrentDate = StrToDateTime(CheckDate);

        while(LastDate < CurrentDate)
        {
            DayCount++;
            Temp = Now() - DayCount;
            CheckDate = Temp.FormatString("mm/dd/yy");
            CurrentDate = StrToDateTime(CheckDate);
        }

        if(DayCount > 1)
        {
            TDateTime Yesterday = Now() - 1.0;
            time = Yesterday.FormatString("mm/dd/yyyy");
        }
        else
        {
            TDateTime date(DateTime);
            time = date.FormatString("mm/dd/yyyy");
        }
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// This method is used to get necessary information that the mall export for robinson
// will be use through the whole functionality
TExportResponse TMallExportRobinsonMall::GetExportInfo(UnicodeString &LocalPath, UnicodeString &TenantID,
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

        LocalPath = CheckDir(LocalPath);

        TenantName = TGlobalSettings::Instance().TenantNo;
        TenantNameSTR = TenantName.t_str();
        FileExt = ".txt";
        for(int i=0;i<TenantName.Length();i++)
        {
            if(i>=TenantName.Length() - 4)
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
    //::::::::::::::::::::::::::::::::::::::

    return result;

}
//---------------------------------------------------------------------------

// Setting of the zedtimer
void TMallExportRobinsonMall::SetZedTimer()
{
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    if(Hour < 4)
    {
        zedcheckTimer->Interval = ((10800000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
    else
    {
        zedcheckTimer->Interval = ((111600000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }

//    InitialMallSet = false;
}
//---------------------------------------------------------------------------

// This method is used to check the previous eod file, setting the timing for the next checking
// generation of export files for the days it was not used and for uploading unsent files
void TMallExportRobinsonMall::CheckPreviousEODFile()
{
    UnicodeString CheckDate = Now().FormatString("mmddyy");
    UnicodeString TenantID = "";
    UnicodeString FileExt = "";
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    UnicodeString Year = Now().FormatString("yyyy");
    UnicodeString DateFormat = "";
    UnicodeString LocalPath = BasePath + "\\RobinsonMall\\" + Year + "\\";
    UnicodeString FailedPath = LocalPath + "Failed Upload\\";
    bool ZReportStatus = false;
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    // Check the EOD in 9:00 AM, this is robison requirements
    zedcheckTimer->Interval = ((111600000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));

    GetExportInfo(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);

    ZReportStatus = CheckPreviousExportFile(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);
    GenerateFileForDaysPOSNotUsed(LocalPath, FailedPath, TerminalNo, CheckDate, TenantID, FileExt, DateFormat);
    UploadingUnsentFiles(FailedPath);

    if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0))
    {
        frmMain->ZForced();
    }
    else
    {
        frmMain->EnablePOSBtn(true);
    }
}

//---------------------------------------------------------------------------

// This method is used to copy the content of the export file. It is used for retaining
// the old values and then add the new value in the text file so it will not overwrite the file
TExportResponse TMallExportRobinsonMall::CopyToMallLocation(UnicodeString MallPathFileName, UnicodeString LocalPathFileName,
                                                            UnicodeString MallPath, UnicodeString LocalPathPrevName,
                                                            UnicodeString &FailedPath, UnicodeString FailedPathFileName,
                                                            UnicodeString FailedPathPrevFileName)
{
    TExportResponse result;

    try
    {
        bool SuccessUpload = false;
        UnicodeString FailedFile = "";

        if(TGlobalSettings::Instance().BatchNo != 1)
        {
            // Copy previous file to new file name if the file is resended
            CopyProcedure(LocalPathPrevName, LocalPathFileName);

            // Copy previous file to the mall path
            CopyProcedure(LocalPathPrevName, MallPathFileName);

            FailedFile = LocalPathPrevName;
        }
        else
        {
            // Copy the file from local path to the mall path
            CopyProcedure(LocalPathFileName, MallPathFileName);

            FailedFile = LocalPathFileName;
        }

        SuccessUpload = ExportUploadMessage(MallPath, FailedPath);

        if(!SuccessUpload)
        {
            // Copy files that failed to be sent to the failed upload folder
            CopyProcedure(FailedFile, FailedPathFileName);

            // If the file was resent and failed, remove the previous file
            std::remove(FailedPathPrevFileName.t_str());
        }
        else
        {
            std::remove(FailedPathPrevFileName.t_str());
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

// This is used for message prompting if the mall specified path is existing or not
bool TMallExportRobinsonMall::ExportUploadMessage(UnicodeString PathName, UnicodeString &FailedPath)
{
    bool result = true;

    if(!DirectoryExists(PathName))
    {
        MessageBox( "Sales file is not sent to RLC server. Please contact your POS vendor", "File was not sent!", MB_OK );
        result = false;
    }
    else
    {
        MessageBox( "Sales file successfully sent to RLC server.", "File has been sent!", MB_OK );
    }

    if(!DirectoryExists(FailedPath))
    {
        CreateDir(FailedPath);
    }

    return result;
}

//---------------------------------------------------------------------------

// This function is used to create yearly folder for robinson mall
// This is due for archiving purposes
UnicodeString TMallExportRobinsonMall::CreateYearlyFolder(UnicodeString LocalPath)
{
    UnicodeString Year = Now().FormatString("yyyy");

    LocalPath = LocalPath + Year;
    if(!DirectoryExists(LocalPath))
    {
        CreateDir(LocalPath);
    }

    return LocalPath + "\\";
}
//---------------------------------------------------------------------------

void TMallExportRobinsonMall::CopyProcedure(UnicodeString in, UnicodeString out)
{
    std::ifstream input(in.c_str());
    std::ofstream output(out.c_str());
    output << input.rdbuf();
}
//---------------------------------------------------------------------------

// This method generates the export file for the days it was not used, if the last day the pos
// was uses does not performed eod, this will generate that export file and the consecutive days
// that it was not used. The data will be zeroes if there are no information available
void TMallExportRobinsonMall::GenerateFileForDaysPOSNotUsed(UnicodeString LocalPath, UnicodeString FailedPath,
                                                            UnicodeString TerminalNo, UnicodeString CheckDate,
                                                            UnicodeString TenantID, UnicodeString FileExt,
                                                            UnicodeString DateFormat)
{
    try
    {
        TMallExportUpdateAdaptor exportUpdateAdaptor;

        UnicodeString LastUsedDate = TGlobalSettings::Instance().LastTransactionDate;
        UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
        UnicodeString MallPath = FilePath;
        UnicodeString LocalFileName = "";
        UnicodeString MallFileName = "";
        UnicodeString FailedPathFileName = "";
        UnicodeString DateParam = "";
        TDateTime PrevDate;
        TDateTime IncrementDate;
        TDateTime CurrentDate;
        TDateTime LastDate;
        int DayCount = 0;

        LastDate = StrToDateTime(LastUsedDate);
        CurrentDate = StrToDateTime(CheckDate);

        if(LastDate < CurrentDate)
        {
            while(CheckDate.Compare(LastUsedDate) != 0)
            {
                DayCount++;
                PrevDate = Now() - DayCount;
                CheckDate = PrevDate.FormatString("mm/dd/yy");
            }

            PrevDate = Now() - DayCount;

            if(DayCount > 1)
            {
                MessageBox( "Generating export files...successful!", "Sending unsent files", MB_OK );

                for(int i=0;i<DayCount-1;i++)
                {
                    IncrementDate = PrevDate + i;
                    DateParam = IncrementDate.FormatString("yyyy/mm/dd");
                    DateFormat = IncrementDate.FormatString("mmdd");
                    LocalFileName = LocalPath + TenantID + DateFormat + "." + TerminalNo + "1" + FileExt;
                    MallFileName = MallPath + TenantID + DateFormat + "." + TerminalNo + "1" + FileExt;
                    FailedPathFileName = FailedPath + TenantID + DateFormat + "." + TerminalNo + "1" + FileExt;
                    CheckFile.open(LocalFileName.c_str(), std::ios_base::out | std::ios_base::in);
                    if(!CheckFile.is_open())
                    {
                        DataQuery = getRobinsonQuery();
                        InputManager->ReadFromDB(DataQuery,DataRead);
                        CreateOutputFormat(DataRead, DataToWrite, DateParam, true);
                        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalFileName);
                        DataRead.clear();
                        DataToWrite.clear();
                        TGlobalSettings::Instance().ZCount += 1;
                        SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);
                        exportUpdateAdaptor.ResetExportTables();

                        if(!DirectoryExists(MallPath))
                        {
                            if(!DirectoryExists(FailedPath))
                            {
                                CreateDir(FailedPath);
                            }
                            CopyProcedure(LocalFileName, FailedPathFileName);
                        }
                        else
                        {
                            CopyProcedure(LocalFileName, MallFileName);
                        }
                    }
                    CheckFile.close();
                }
                TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mm/dd/yy");
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

// If the file was resent and successfully sent, remove the previous file
// from failed folder
void TMallExportRobinsonMall::UploadingUnsentFiles(UnicodeString FailedPath)
{
    try
    {
        std::vector<UnicodeString> files;
        UnicodeString CheckFailedPath = FailedPath;
        UnicodeString SendingFailedFiles = "";
        UnicodeString ReceivingFailedFiles = "";
        UnicodeString MallPath = FFileURI;
        UnicodeString fileExt = "";
        std::string getName = "";
        int fileCount = 0;

        if(DirectoryExists(MallPath))
        {
            DIR* dir = opendir(CheckFailedPath.t_str());

            while ( (entry = readdir(dir)) != NULL)
            {
                files.push_back(std::string(entry->d_name).c_str());
            }

            // Copy all the failed files in failed folder to the mall path
            for (int i = 0;i < files.size();i++)
            {
                getName = files.at(i).t_str();
                for (int i = getName.length() - 3 ; i<getName.length(); i++)
                {
                    fileExt = fileExt + getName[i];
                }
                if((fileExt == "txt") || (fileExt == "TXT"))
                {
                    SendingFailedFiles = FailedPath + files.at(i).t_str();
                    ReceivingFailedFiles = MallPath + files.at(i).t_str();

                    CopyProcedure(SendingFailedFiles, ReceivingFailedFiles);

                    std::remove(SendingFailedFiles.t_str());
                    SendingFailedFiles = "";
                    ReceivingFailedFiles = "";
                    fileCount++;
                }
                fileExt = "";
            }
            closedir(dir);

            if(fileCount != 0)
            {
                MessageBox( "Trying to send unsent files...successful!", "Sending unsent files", MB_OK );
            }
        }
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }
}
//---------------------------------------------------------------------------

// This method is used to set the Export Timing
TExportResponse TMallExportRobinsonMall::GenerateHourlyExport()
{
    TExportResponse result;

    try
    {
//        exportTimer->Interval = 60000 - (Seconds*1000);
        exportTimer->Interval = 1000;

        FTimingExportResponse = timingExport();


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
void TMallExportRobinsonMall::GetReportsList(TStringList* reportsList, UnicodeString folder)
{
    UnicodeString LocalPath = "";
    std::vector<UnicodeString> files;

    if(folder == "")
    {
        LocalPath = BasePath + "\\RobinsonMall\\";
    }
    else
    {
        LocalPath = BasePath + "\\RobinsonMall\\" + folder + "\\";
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
            if((fileExt == "txt") || (fileExt == "TXT"))
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
void TMallExportRobinsonMall::ResendReports(TStringList* resendReports, UnicodeString folder, bool status)
{

    UnicodeString Year = Now().FormatString("yyyy");
    UnicodeString LocalPath = BasePath + "\\RobinsonMall\\" + folder + "\\";
    UnicodeString MallPath = TGlobalSettings::Instance().MallPath;
    UnicodeString MallPathFiles = "";
    UnicodeString LocalPathFiles = "";

    if(status == false)
    {
        if(folder.Length() != 0)
        {
            if(!DirectoryExists(MallPath))
            {
                MessageBox( "Sales files was not sent to RLC server. Please check network connections!", "File was not sent!", MB_OK );
            }
            else
            {
                MessageBox( "Sales file successfully resend to RLC server.", "File has been sent!", MB_OK );
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
bool TMallExportRobinsonMall::CheckBackUpPath()
{
    UnicodeString LocalPath = BasePath + "\\RobinsonMall\\";

    return (DirectoryExists(LocalPath)) ? true : false;
}
//---------------------------------------------------------------------------

bool TMallExportRobinsonMall::CheckTodaysZed()
{
    unsigned short Hour = 0;
    unsigned short dummy = 0;
    TDateTime Yesterday = Now() - 1.0;
    UnicodeString Year = Now().FormatString("yyyy");
    UnicodeString LocalPath = BasePath + "\\RobinsonMall\\" + Year + "\\";
    UnicodeString TenantID = "";
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    UnicodeString FileExt = ".txt";
    UnicodeString DateFormat = "";
    UnicodeString LocalPathFileName = "";
    bool result = false;

    Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

    GetExportInfo(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);
    DateFormat = (Hour < 4) ? Yesterday.FormatString("mmdd") : Now().FormatString("mmdd");
    LocalPathFileName = LocalPath + TenantID + DateFormat + "." + TerminalNo + "1" + FileExt;
    CheckFile.open(LocalPathFileName.c_str(), std::ios_base::out | std::ios_base::in);

    result = (CheckFile.is_open()) ? true : false;
    return result;
}
//---------------------------------------------------------------------------

bool TMallExportRobinsonMall::CheckPreviousZed()
{
    UnicodeString Year = Now().FormatString("yyyy");
    UnicodeString LocalPath = BasePath + "\\RobinsonMall\\" + Year + "\\";
    UnicodeString TenantID = "";
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    UnicodeString FileExt = ".txt";
    UnicodeString DateFormat = "";
    bool result = false;

    GetExportInfo(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);
    result = CheckPreviousExportFile(LocalPath, TenantID, TerminalNo, FileExt, DateFormat);

    return result;
}
//---------------------------------------------------------------------------

bool TMallExportRobinsonMall::ScheduledExitOnSelectDish()
{
    bool result = false;
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&dummy,&dummy);

    if((Hour > 3) && (Hour < 9))
    {
        result = true;
    }

    return result;
}
//---------------------------------------------------------------------------
