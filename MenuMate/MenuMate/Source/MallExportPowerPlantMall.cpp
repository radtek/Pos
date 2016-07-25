//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportPowerPlantMall.h"
#include "MallExportDataManager.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "MallExportUpdateAdaptor.h"
#include "Main.h"
#include <string>
#include <vector>
#include <ios>
#include <windows.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportPowerPlantMall::TMallExportPowerPlantMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportPowerPlantMall::~TMallExportPowerPlantMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportPowerPlantMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Delete this method if not needed
TExportResponse TMallExportPowerPlantMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        bool PrevZReportStatus = false;

        CreateFilename("ZED", prevFName);

        RIdentifier = "99";
        DataQuery = getPowerPlantQuery("ZED");
        InputManager->ReadFromDB(DataQuery,DataRead);
        if(TGlobalSettings::Instance().BatchNo != 1)
        {
            CopyOutputContent(FileURI, prevFName);
        }
        else
        {
            TGlobalSettings::Instance().PPlantCount += 1;
            SaveIntVariable(vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
            CreateOutputFormatZed(DataRead, DataToWrite, FileURI, RIdentifier);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);
            DataRead.clear();
            // Clear content of *.sal file
            std::ofstream(FileURI.c_str(), std::ios_base::out).close();

            RIdentifier = "95";
            TGlobalSettings::Instance().PPlantCount += 1;
            SaveIntVariable(vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
            DataQuery = getPowerPlantQuery("GT");
            InputManager->ReadFromDB(DataQuery,DataRead);
            CreateOutputFormatZed(DataRead, DataToWrite, FileURI, RIdentifier);
            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);
        }

        TGlobalSettings::Instance().ZCount += 1;
        SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);

        TGlobalSettings::Instance().PPlantFlag = 1;
        SaveIntVariable(vmPPlantFlag, TGlobalSettings::Instance().PPlantFlag);
        TGlobalSettings::Instance().PPlantCount = 0;
        SaveIntVariable(vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
        TGlobalSettings::Instance().PPlantHourly = 0;
        SaveIntVariable(vmPPlantHourly, TGlobalSettings::Instance().PPlantHourly);
        TGlobalSettings::Instance().PPlantPrevTime = 0;
        SaveIntVariable(vmPPlantPrevTime, TGlobalSettings::Instance().PPlantPrevTime);
        // Delete PowerPlant.txt and temp.txt file
        std::remove((ExtractFilePath(Application->ExeName) + "PowerPlant.txt").t_str());
        std::remove((ExtractFilePath(Application->ExeName) + "temp.txt").t_str());

        PauseExportTiming();

        PrevZReportStatus = CheckPreviousZed();

        if(PrevZReportStatus)
        {
            frmMain->EnablePOSBtn(true);
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

// This method generates the first text file and data that will be added in the text file.
// For each transacation it will update the text file (in ExtractFilePath) for the current
// hour. Then if the current hour has passed, it will generate another data to be appended in
// the previous data.
// This method also checks if the previous data of the previous hour is not generated, it will
// generate the previous data along with the current data. So let say I have a transaction for
// 13:00, but the timing missed the 00 minutes, when a transaction is performed for 14:00
// this method will generate the data for 13:00 and 14:00
TExportResponse TMallExportPowerPlantMall::TransactionExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        UnicodeString Time = Now().FormatString("hh");
        int PreviousTime = TGlobalSettings::Instance().PPlantPrevTime;
        int CurrentTime = Time.ToInt();

        TGlobalSettings::Instance().PPlantFlag = 0;
        SaveIntVariable(vmPPlantFlag, TGlobalSettings::Instance().PPlantFlag);

        RIdentifier = "01";
        DataQuery = getPowerPlantQuery("TRANSACTION");
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename("TRANSACTION", prevFName);

        if((CurrentTime != PreviousTime) && (TGlobalSettings::Instance().PPlantHourly == 1))
        {
            TGlobalSettings::Instance().PPlantCount += 1;
            SaveIntVariable(vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
        }

        TGlobalSettings::Instance().PPlantPrevTime = CurrentTime;
        SaveIntVariable(vmPPlantPrevTime, TGlobalSettings::Instance().PPlantPrevTime);

        CreateOutputFormatGeneric(DataRead, DataToWrite, "TRANSACTION", FileURI, RIdentifier);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);

        TGlobalSettings::Instance().PPlantHourly = 1;
        SaveIntVariable(vmPPlantHourly, TGlobalSettings::Instance().PPlantHourly);

        // Delete temp.txt file
        std::remove((ExtractFilePath(Application->ExeName) + "temp.txt").t_str());

        ResumeExportTiming();

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
void TMallExportPowerPlantMall::initTimingExportResponse( TExportResponse& outResponse )
{
    // TODO:

    setExportResponse(
       false,               // Succesful = false
       emrExportNotDoneYet, // Result = emrExportNotDoneYet
       "",                  // Message
       "",                  // Description
       outResponse );
}
//---------------------------------------------------------------------------

// This method checks if the data of the previous hour is created or not. If the data
// is created, this method will just increment the line count in preperation for the current hour.
// If the data is not created, that means there are not transaction occurred in the previous hour
// so the data will be created for that hour but the all data is just zero
TExportResponse TMallExportPowerPlantMall::timingExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        UnicodeString Minutes = Now().FormatString("nn");

        if(Minutes == "00")
        {
            if(TGlobalSettings::Instance().PPlantHourly == 0)
            {
                TGlobalSettings::Instance().PPlantFlag = 0;
                SaveIntVariable(vmPPlantFlag, TGlobalSettings::Instance().PPlantFlag);

                RIdentifier = "01";
                DataQuery = getPowerPlantQuery("TIMING");
                InputManager->ReadFromDB(DataQuery,DataRead);
                CreateFilename("TIMING", prevFName);
                CreateOutputFormatGeneric(DataRead, DataToWrite, "TIMING", FileURI, RIdentifier);
                TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FileURI);
            }

            TGlobalSettings::Instance().PPlantHourly = 0;
            SaveIntVariable(vmPPlantHourly, TGlobalSettings::Instance().PPlantHourly);
            TGlobalSettings::Instance().PPlantCount += 1;
            SaveIntVariable(vmPPlantCount, TGlobalSettings::Instance().PPlantCount);
        }

        // Delete temp.txt file
        std::remove((ExtractFilePath(Application->ExeName) + "temp.txt").t_str());

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

// This function is for getting query from the DB Table for ZED, GT(Grand Total) and Generic files
UnicodeString TMallExportPowerPlantMall::getPowerPlantQuery(UnicodeString mode)
{
    UnicodeString PowerPlantQuery = "";

    if(mode == "ZED")
    {
        PowerPlantQuery = "SELECT * FROM POWERPLANT_ZED_MALLVIEW";
    }
    else if(mode == "GT")
    {
        PowerPlantQuery = "SELECT * FROM POWERPLANT_GT_MALLVIEW";
    }
    else if((mode == "TIMING") || (mode == "TRANSACTION"))
    {
        PowerPlantQuery = "SELECT * FROM POWERPLANT_GENERIC_MALLVIEW";
    }

    return PowerPlantQuery;
}
//---------------------------------------------------------------------------

TExportResponse TMallExportPowerPlantMall::CreateFilename(UnicodeString mode, AnsiString &prevFName)
{
    TExportResponse result;

    try
    {
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString CheckDate = Now().FormatString("mmddyy");
        UnicodeString DateFormat = "";
        UnicodeString FileName = "";
        UnicodeString CheckName = "";
        UnicodeString FileExt = ".sal";
        UnicodeString BatchNo = TGlobalSettings::Instance().BatchNo;
        UnicodeString MallPath = "";
        UnicodeString prevBatchNo = "";
        bool ZReportStatus = false;
        int prevBatch = 0;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        if(BatchNo.Length() == 1)
        {
            BatchNo = "0" + BatchNo;
        }

        if(mode == "ZED")
        {
            MallPath = FFileURI;
        }
        else
        {
            MallPath = ExtractFilePath(Application->ExeName);
        }

        MallPath = CheckDir(MallPath);

        // This condition is to make sure what mode is being used and to get the
        // system date needed for text file filename
        if(mode == "ZED")
        {
            // This condition is used to determine if the system will check previous eod
            // file or not. When a mall is set, first mall set will become 1 and the system
            // will not look if there is a previous zed report performed
            if(TGlobalSettings::Instance().FirstMallSet != true && TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0)
            {
                ZReportStatus = CheckPreviousExportFile(MallPath, FileExt, DateFormat);
            }
            else
            {
                ZReportStatus = true;
                TGlobalSettings::Instance().FirstMallSet = false;
                SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
            }

            if(Hour<3)
            {
                DateFormat = Yesterday.FormatString("mmddyy");
            }
            else
            {
                DateFormat = (ZReportStatus) ? Now().FormatString("mmddyy") : DateFormat;
            }

            // This is for checking if the file does already exist, if it is existing
            // this will create the same file with different batch no
            CheckName = MallPath + DateFormat + BatchNo + FileExt;
            CheckFile.open(CheckName.c_str(), std::ios_base::out | std::ios_base::in);

            if ((CheckFile.is_open()) && (ZReportStatus == true))
            {
                if(TGlobalSettings::Instance().PPlantFlag == 1)
                {
                    TGlobalSettings::Instance().BatchNo = TGlobalSettings::Instance().BatchNo + 1;
                }
                else
                {
                    TGlobalSettings::Instance().PPlantFlag = 1;
                }
            }
            else
            {
                TGlobalSettings::Instance().BatchNo = 1;
                TGlobalSettings::Instance().PPlantFlag = 1;
            }
            SaveIntVariable(vmPPlantFlag, TGlobalSettings::Instance().PPlantFlag);

            CheckFile.close();

            BatchNo = TGlobalSettings::Instance().BatchNo;
            SaveIntVariable(vmBatchNo, TGlobalSettings::Instance().BatchNo);

            for(int i=0;i<TGlobalSettings::Instance().BatchNo;i++)
            {
                if(BatchNo.Length() == 1)
                {
                    BatchNo = "0" + BatchNo;
                    prevBatchNo = "0" + (BatchNo - 1);
                }
                CheckName = MallPath + DateFormat + prevBatchNo + FileExt;
                CheckFile.open(CheckName.c_str(), std::ios_base::out | std::ios_base::in);

                if(CheckFile.is_open())
                {
                    CheckFile.close();
                    break;
                }
            }

            FileName = MallPath + DateFormat + BatchNo + FileExt;
            prevFName = MallPath + DateFormat + prevBatchNo + FileExt;
        }
        else
        {
            DateFormat = "";
            FileName = MallPath + "PowerPlant.txt";
            prevFName = FileName;
        }

        FFileURI = FileName;

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

// This function is for checking the specified path for PowerPlant Mall is existing
// if not, it will create a local folder called MallExport then create a folder
// called PowerPlantMall for ZedReport and for HourlyExport
UnicodeString TMallExportPowerPlantMall::CheckDir(UnicodeString MallPath)
{
    if(!DirectoryExists(MallPath))
    {
        MallPath = ExtractFilePath(Application->ExeName) + MALLEXPORTFOLDERPATH;
        if(!DirectoryExists(MallPath))
        {
            CreateDir(MallPath);
        }
        MallPath = MallPath + "\\PowerPlantMall\\";
        if(!DirectoryExists(MallPath))
        {
            CreateDir(MallPath);
        }
    }
    return MallPath;
}
//---------------------------------------------------------------------------

// This functions is for checking the previous EOD file
bool TMallExportPowerPlantMall::CheckPreviousExportFile(UnicodeString MallPath, UnicodeString FileExt, UnicodeString &DateFormat)
{
    try
    {
        UnicodeString Filename = "";
        bool ZReportStatus = true;
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString Month = Yesterday.FormatString("mm");
        UnicodeString Day = Yesterday.FormatString("dd");
        UnicodeString Year = Yesterday.FormatString("yy");

        DateFormat = Month + Day + Year;

        Filename = MallPath + DateFormat + "01" + FileExt;
        CheckPreviousFile.open(Filename.c_str(), std::ios_base::out | std::ios_base::in);

        ZReportStatus = (CheckPreviousFile.is_open()) ? true : false;
        CheckPreviousFile.close();

        return ZReportStatus;
    }
    catch(Exception &ex)
    {
        return false;
    }
}
//---------------------------------------------------------------------------

// Get Time format for PowerPlant Export File
UnicodeString TMallExportPowerPlantMall::GetTime(UnicodeString DateTime, UnicodeString mode)
{
    AnsiString time = "";

    try
    {
        TDateTime date(DateTime);
        UnicodeString Minutes = Now().FormatString("nn");
        if(mode == "TIMING")
        {
            if(Minutes == "00")
            {
                int inittime = StrToInt(Now().FormatString("hh"));
                if(inittime == 1)
                {
                    inittime = 24;
                }
                else
                {
                    inittime -= 1;
                }
                time = IntToStr(inittime);
                if(time.Length() == 1)
                {
                    time = "0" + time;
                }
                time = time + ":00";
           }
           else
           {
                time = date.FormatString("hh") + ":00";
           }
        }
        else if (mode == "TRANSACTION")
        {
            TDateTime date(DateTime);
            time = date.FormatString("hh") + ":00";
        }
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// Get Date format for PowerPlant Export File
UnicodeString TMallExportPowerPlantMall::GetDate(UnicodeString DateTime)
{
    AnsiString time = "";

    try
    {
        TDateTime date(DateTime);
        time = date.FormatString("mm/dd/yyyy");
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( time );
}
//---------------------------------------------------------------------------

// This function is for saving the integer global variable
void TMallExportPowerPlantMall::SaveIntVariable(vmVariables vmVar, int CompName)
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

// This function is for saving the bool global variable
void TMallExportPowerPlantMall::SaveBoolVariable(vmVariables vmVar, bool CompName)
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

// This function is for arranging the format of the output file for ZED and GT(Grand Total) files
TExportResponse TMallExportPowerPlantMall::CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead,
                                                                 std::vector<UnicodeString> &DataToWrite,
                                                                 AnsiString FileURI,
                                                                 UnicodeString RIdentifier)
{
    TExportResponse result;

    try
    {
        int i = 0;
        UnicodeString Format = " ";
        UnicodeString OutputValue = "";
        DataToWrite.clear();
        UnicodeString GetData = "";
        std::string TempData = "";
        UnicodeString TempPath = ExtractFilePath(Application->ExeName) + "PowerPlant.txt";

        std::map<UnicodeString, UnicodeString>::iterator it;

        if(RIdentifier == "99")
        {
            // For RIdentifier
            OutputValue = RIdentifier + Format;
            DataToWrite.push_back(OutputValue.t_str());
            for(i=0;i<DataRead.size();i++)
            {
                if(i==0)
                {
                    it = DataRead.find("TenantNo");
                }
                else if(i==1)
                {
                    it = DataRead.find("TransactionDate"); // Will be overridden
                }
                else if((i>=2) && (i<=6))
                {
                    (it->second) = CalculateZed((i+2),TempPath);
                }
                else if(i==7)
                {
                    it = DataRead.find("MallCode"); // Should be Tenant No.
                }
                else if((i>=8) && (i<=12))
                {
                    (it->second) = CalculateZed((i+2),TempPath);
                }

                if ((i==0) || (i==7))
                {
                    OutputValue = "\"" + (it->second) + "\"" + Format;
                }
                else if (i==1)
                {
                    OutputValue = GetDate(it->second) + Format;
                }
                else if (((i>=2) && (i<=5)) || (i==9) || (i==11) || (i==12))
                {
                    // The purpose of this condition is to get make the value have a decimal places
                    GetData = (it->second);
                    TempData = GetData.t_str();
                    std::string result;
                    std::string adecimal;
                    bool decimal = false;
                    for(int j=0;j<TempData.length();j++)
                    {
                        if((TempData.at(j) != '.') && (!decimal))
                        {
                            result = result + TempData.at(j);
                        }
                        else
                        {
                            decimal = true;
                            if(TempData.at(j) != '.')
                            {
                                adecimal = adecimal + TempData.at(j);
                            }
                        }
                    }
                    if(adecimal.length() == 1)
                    {
                        adecimal = adecimal + "0";
                    }
                    GetData = result.c_str();
                    if(!decimal)
                    {
                        OutputValue = GetData + ".00";
                    }
                    else
                    {
                        OutputValue = GetData + "." + adecimal.c_str();
                    }
                    if(i!=12)
                    {
                        OutputValue = OutputValue + Format;
                    }
                }
                else if (((i==6) || (i==8) || (i==10)) && (((it->second) == "0") || ((it->second) == "")))
                {
                    OutputValue = "0" + Format;
                }
                else if(((it->second) == "0") || ((it->second) == ""))
                {
                    OutputValue = "0.00" + Format;
                }
                else
                {
                    OutputValue = (it->second) + Format;
                }
                DataToWrite.push_back(OutputValue.t_str());
            }
        }
        else if (RIdentifier == "95")
        {
            OutputValue = RIdentifier + Format;
            DataToWrite.push_back(OutputValue.t_str());
            for(i=0;i<DataRead.size();i++)
            {
                if(i==0)
                {
                    it = DataRead.find("TenantNo");
                }
                else if(i==1)
                {
                    it = DataRead.find("TransactionDate"); // Will be overridden
                }
                else if(i==2)
                {
                    it = DataRead.find("GrandTotalOld");
                }
                else if(i==3)
                {
                    it = DataRead.find("GrandTotal");
                }

                if (i==0)
                {
                    OutputValue = "\"" + (it->second) + "\"" + Format;
                }
                else if (i==1)
                {
                    OutputValue = GetDate(it->second) + Format;
                }
                else if(((it->second) == "0") || ((it->second) == ""))
                {
                    OutputValue = "0.00" + Format;
                }
                else
                {
                    OutputValue = (it->second) + Format;
                }
                DataToWrite.push_back(OutputValue.t_str());
            }
            OutputValue = TGlobalSettings::Instance().ZCount;
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

// This function is for arranging the format of the output file for GENERIC export files
TExportResponse TMallExportPowerPlantMall::CreateOutputFormatGeneric(std::map<UnicodeString, UnicodeString> DataRead,
                                                                     std::vector<UnicodeString> &DataToWrite,
                                                                     UnicodeString mode,
                                                                     AnsiString FileURI,
                                                                     UnicodeString RIdentifier)
{
    TExportResponse result;

    try
    {
        int i = 0;
        UnicodeString Format = " ";
        UnicodeString OutputValue = "";
        DataToWrite.clear();
        UnicodeString GetData = "";
        std::string TempData = "";

        std::map<UnicodeString, UnicodeString>::iterator it;

        // For RIdentifier
        OutputValue = RIdentifier + Format;
        DataToWrite.push_back(OutputValue.t_str());
        for(i=0;i<DataRead.size();i++)
        {
            if(i==0)
            {
               it = DataRead.find("TenantNo");
            }
            else if(i==1)
            {
               it = DataRead.find("TransactionDate"); // Will be overridden
            }
            else if(i==2)
            {
               it = DataRead.find("TransactionTime");
            }
            else if(i==3)
            {
               it = DataRead.find("TotalGrossSale");
            }
            else if(i==4)
            {
               it = DataRead.find("TotalSaleTax");
            }
            else if(i==5)
            {
               it = DataRead.find("TotalDiscount");
            }
            else if(i==6)
            {
               it = DataRead.find("TotalServiceCharge");
            }
            else if(i==7)
            {
               it = DataRead.find("TransactionCount");
            }
            else if(i==8)
            {
               it = DataRead.find("MallCode"); // Should be Tenant No.
            }
            else if(i==9)
            {
               it = DataRead.find("TotalRefundCount");
            }
            else if(i==10)
            {
               it = DataRead.find("TotalRefund");
            }
            else if(i==11)
            {
               it = DataRead.find("TotalCancelledCount");
            }
            else if(i==12)
            {
               it = DataRead.find("TotalCancelled");
            }
            else if(i==13)
            {
               it = DataRead.find("TotalSeniorCitizenDiscount");
            }

            if ((i==0) || (i==8))
            {
                OutputValue = "\"" + (it->second) + "\"" + Format;
            }
            else if (i==1)
            {
                OutputValue = GetDate(it->second) + Format;
            }
            else if (i==2)
            {
                OutputValue = GetTime(it->second, mode) + Format;
            }
            else if (((i==7) || (i==9) || (i==11)) && (((it->second) == "0") || ((it->second) == "")))
            {
                OutputValue = "0" + Format;
            }
            else if(((it->second) == "0") || ((it->second) == ""))
            {
                OutputValue = "0.00" + Format;
            }
            else if(i==13)
            {
                OutputValue = (it->second);
            }
            else
            {
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

// This method is used to copy the content of the export file. It is used for retaining
// the old values and then add the new value in the text file so it will not overwrite the file
TExportResponse TMallExportPowerPlantMall::CopyOutputContent(AnsiString FileURI, AnsiString prevPath)
{
    TExportResponse result;

    try
    {
        std::ifstream FirstFile(prevPath.c_str());
        std::ofstream SecondFile(FileURI.c_str());

        SecondFile << FirstFile.rdbuf();

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

// This method is used to get the total of the required field from the sum of hourly reports
Currency TMallExportPowerPlantMall::CalculateZed(int number, AnsiString TempPath)
{
    Currency result = 0;

    try
    {
        int loop = 0;
        int ContentCount = 0;
        int ArrayCount = 0;
        std::string words;
        std::string sentence;

        TempFile.open(TempPath.c_str());

        if(TempFile.is_open())
        {
            std::ifstream myfile(TempPath.c_str());
            loop = TGlobalSettings::Instance().PPlantCount - 1;
            ContentCount = 0;
            std::string items[100];
            while(std::getline(myfile,line))
            {
                ArrayCount = 0;
                if(ContentCount<loop)
                {
                    sentence = line.c_str();
                    for(int i=0;i<sentence.length();i++)
                    {
                        if(sentence.at(i) == ' ')
                        {
                            items[ArrayCount] = words;
                            words = "";
                            ArrayCount++;
                        }
                        else
                        {
                            words += sentence.at(i);
                        }
                        if(i == (sentence.length()-1))
                        {
                            items[ArrayCount] = words;
                            words = "";
                            ArrayCount++;
                        }
                    }
                }
                ContentCount++;
                for(int j=0;j<ArrayCount;j++)
                {
                    if(j == number)
                    {
                        result += StrToCurr(items[j].c_str());
                    }
                }
            }
        }
        TempFile.close();
    }
    catch( Exception& exc )
    {
        //ignore the exception
    }

    return result;
}
//---------------------------------------------------------------------------

// This method is used to set the Export Timing
TExportResponse TMallExportPowerPlantMall::GenerateHourlyExport()
{
    TExportResponse result;

    try
    {
        UnicodeString Minutes = Now().FormatString("nn");
        unsigned short Seconds = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&dummy,&dummy,&Seconds,&dummy);

        exportTimer->Interval = 60000 - (Seconds*1000);

        // This condition is for generation of export for every hour
        if(Minutes == "00") {
            TMallExportUpdateAdaptor exportUpdateAdaptor;
            FTimingExportResponse = timingExport();
            exportUpdateAdaptor.ResetExportTables();
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
TExportResponse TMallExportPowerPlantMall::GetExportInfo(UnicodeString &MallPath, UnicodeString &TenantID,
                                                         UnicodeString &TerminalNo, UnicodeString &FileExt,
                                                         UnicodeString &DateFormat)
{
    TExportResponse result;

    try
    {
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString Day = Yesterday.FormatString("dd");
        UnicodeString Month = Yesterday.FormatString("mm");
        UnicodeString Year = Yesterday.FormatString("yy");

        TenantID = "";
        TerminalNo = "";

        MallPath = CheckDir(MallPath);
        FileExt = ".sal";

        DateFormat = Month + Day + Year;

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

void TMallExportPowerPlantMall::SetZedTimer()
{
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    if(Hour < 3)
    {
        zedcheckTimer->Interval = ((7200000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
    else
    {
        zedcheckTimer->Interval = ((93600000 - (Hour*3600000)) + (3600000 - (Minutes*60000)) + (60000 - (Seconds*1000)));
    }

    TGlobalSettings::Instance().FirstMallSet = false;
    SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
}//---------------------------------------------------------------------------

void TMallExportPowerPlantMall::CheckPreviousEODFile()
{
    UnicodeString CheckDate = Now().FormatString("mmddyy");
    UnicodeString TenantID = "";
    UnicodeString FileExt = "";
    UnicodeString MallPath = FFileURI;
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    UnicodeString DateFormat = "";
    bool ZReportStatus = false;
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    zedcheckTimer->Interval = ((93600000 - (Hour*3600000)) + (3600000 - (Minutes*60000)) + (60000 - (Seconds*1000)));

    GetExportInfo(MallPath, TenantID, TerminalNo, FileExt, DateFormat);

    if (TerminalNo.Length() == 1)
    {
        TerminalNo = "0" + TerminalNo;
    }

    ZReportStatus = CheckPreviousExportFile(MallPath, FileExt, DateFormat);

    if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0))
    {
        frmMain->ZForced();
    }
}

//---------------------------------------------------------------------------

bool TMallExportPowerPlantMall::CheckPreviousZed()
{
//    UnicodeString MallPath = FFileURI;
    UnicodeString FileExt = ".txt";
    UnicodeString DateFormat = "";
    UnicodeString TenantID = "";
    UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
    bool result = false;

    GetExportInfo(FilePath, TenantID, TerminalNo, FileExt, DateFormat);
    result = CheckPreviousExportFile(FilePath, FileExt, DateFormat);

    return result;
}
//---------------------------------------------------------------------------
