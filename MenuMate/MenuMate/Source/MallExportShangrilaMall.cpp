//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportShangrilaMall.h"
#include "MallExportDataManager.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "MallExportUpdateAdaptor.h"
#include "Main.h"
#include <string>
#include <vector>
#include <ios>
#include <windows.h>
#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportShangrilaMall::TMallExportShangrilaMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportShangrilaMall::~TMallExportShangrilaMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportShangrilaMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS

// Virtual methods
void TMallExportShangrilaMall::initTimingExportResponse( TExportResponse& outResponse )
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
TExportResponse TMallExportShangrilaMall::timingExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
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

TExportResponse TMallExportShangrilaMall::TransactionExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mmddyyyy");
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
TExportResponse TMallExportShangrilaMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        bool PrevZReportStatus = false;
        TDateTime Yesterday = Now() - 1.0;
        Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];

        DataRead.clear();

        CreateFilename();
        RIdentifier = "01";
        CreateOutputFormatHourly(DataToWrite, RIdentifier);
        DataRead.clear();

        RIdentifier = "99";
        DataQuery = getSharngrilaQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateOutputFormatZed(DataRead, DataToWrite, RIdentifier);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FFileURI);

        if(gridBtn->Enabled == false)
        {
            frmMain->EnablePOSBtn(true);
            TGlobalSettings::Instance().LastZedDate = Yesterday.FormatString("mm/dd/yy");
            SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
        }
        else
        {
            TGlobalSettings::Instance().LastZedDate = Now().FormatString("mm/dd/yy");
            SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
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

// This function is for getting query from the DB Table for ZED
UnicodeString TMallExportShangrilaMall::getSharngrilaQuery()
{
    UnicodeString ShangrilaQuery = "";

    ShangrilaQuery = "SELECT * FROM SHANGRILA_MALLVIEW";

    return ShangrilaQuery;
}
//---------------------------------------------------------------------------

TExportResponse TMallExportShangrilaMall::CreateFilename()
{
    TExportResponse result;

    try
    {
        TDateTime Yesterday = Now() - 1.0;
        UnicodeString CheckDate = Now().FormatString("mmddyy");
        UnicodeString DateFormat = "";
        UnicodeString FileName = "";
        UnicodeString CheckName = "";
        UnicodeString FileExt = ".txt";
        UnicodeString MallPath = "";
        bool ZReportStatus = false;
        int prevBatch = 0;
        unsigned short Hour = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

        MallPath = FFileURI;

        MallPath = CheckDir(MallPath);

        // This condition is used to determine if the system will check previous eod
        // file or not. When a mall is set, first mall set will become 1 and the system
        // will not look if there is a previous zed report performed
        if(TGlobalSettings::Instance().FirstMallSet != true && TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0)
        {
            ZReportStatus = PerformedLastZed();
        }
        else
        {
            ZReportStatus = true;
            TGlobalSettings::Instance().FirstMallSet = false;
            SaveBoolVariable(vmFirstMallSet, TGlobalSettings::Instance().FirstMallSet);
        }

        if(Hour<6)
        {
            DateFormat = Yesterday.FormatString("mmddyyyy");
        }
        else
        {
            DateFormat = (ZReportStatus) ? Now().FormatString("mmddyyyy") : TGlobalSettings::Instance().LastTransactionDate;
        }

        FileName = MallPath + DateFormat + FileExt;

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
// called Shangrila for ZedReport and for HourlyExport
UnicodeString TMallExportShangrilaMall::CheckDir(UnicodeString MallPath)
{
    if(!DirectoryExists(MallPath))
    {
		MessageBox("Mall Path do not Exist. Please check specified Mall Path in settings", "Error",
		MB_OK + MB_ICONWARNING);
    }
    return MallPath;
}
//---------------------------------------------------------------------------

// This function is for saving the bool global variable
void TMallExportShangrilaMall::SaveBoolVariable(vmVariables vmVar, bool CompName)
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

// This function is for saving the integer global variable
void TMallExportShangrilaMall::SaveIntVariable(vmVariables vmVar, int CompName)
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
void TMallExportShangrilaMall::SaveStrVariable(vmVariables vmVar, UnicodeString CompName)
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

// This method creates the hourly data for hourly export file
TExportResponse TMallExportShangrilaMall::CreateOutputFormatHourly(std::vector<UnicodeString> &DataToWrite, UnicodeString RIdentifier)
{
    UnicodeString Format = " ";
    UnicodeString OutputValue = "";

    UnicodeString MallCode = TGlobalSettings::Instance().TenantNo;
    UnicodeString SalesDept = TGlobalSettings::Instance().BranchCode;
    UnicodeString TenantName = "";
    UnicodeString DateValue = "";
    UnicodeString TimeValue = "";
    UnicodeString AmountValue = 0;
    UnicodeString TaxValue = 0;
    UnicodeString AdjValue = 0;
    UnicodeString SChargeValue = 0;
    int TransactionTotal = 0;

    std::string TimeCount[12];
    int val = 0;
    bool flag = false;

    RIdentifier = "01";
    UnicodeString Amount = "";

    unsigned short Hour = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&dummy,&dummy,&dummy);

    DataToWrite.clear();

    std::map<UnicodeString, UnicodeString>::iterator it;

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

        query->SQL->Text = "SELECT MALLCODE, DATE_VALUE, TIME_VALUE, SUM(AMOUNT_VALUE) AMOUNT_VALUE, "
                           "SUM(VAT_SALES) VAT_SALES, SUM(TOTALDISCOUNT) TOTALDISCOUNT, "
                           "SUM(SCHARGE_AMOUNT) SCHARGE_AMOUNT, SUM(TRANSACTION_COUNT) TRANSACTION_COUNT "
                           "FROM MALLEXPORT_HOURLY WHERE MALLCODE=:MALLCODE GROUP BY MALLCODE, DATE_VALUE, TIME_VALUE";
        DBTransaction.StartTransaction();
        query->ParamByName("MALLCODE")->AsString = MallCode;
        query->ExecQuery();

        while(!query->Eof)
        {
            TenantName = query->FieldByName("MALLCODE")->AsString;
            DateValue = query->FieldByName("DATE_VALUE")->AsString;
            TimeValue = query->FieldByName("TIME_VALUE")->AsString;
            AmountValue = FixDecimalPlaces(RoundToNearest(query->FieldByName("AMOUNT_VALUE")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
            TaxValue = FixDecimalPlaces(RoundToNearest(query->FieldByName("VAT_SALES")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
            AdjValue = FixDecimalPlaces(RoundToNearest(query->FieldByName("TOTALDISCOUNT")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
            SChargeValue = FixDecimalPlaces(RoundToNearest(query->FieldByName("SCHARGE_AMOUNT")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
            TransactionTotal = query->FieldByName("TRANSACTION_COUNT")->AsCurrency;

            TimeCount[val] = TimeValue.t_str();
            val++;

            DataRead[TimeValue] = GetHourlyFormat(TenantName, DateValue, TimeValue, AmountValue, TaxValue, AdjValue, SChargeValue, TransactionTotal, SalesDept);

            query->Next();
        }
        DBTransaction.Commit();
        query->Close();

        for(int i=0;i<12;i++)
        {
            UnicodeString Time = "";
            UnicodeString Data = "";

            switch(i)
            {
                case 0:
                    Time = "10";
                    break;
                case 1:
                    Time = "11";
                    break;
                case 2:
                    Time = "12";
                    break;
                case 3:
                    Time = "13";
                    break;
                case 4:
                    Time = "14";
                    break;
                case 5:
                    Time = "15";
                    break;
                case 6:
                    Time = "16";
                    break;
                case 7:
                    Time = "17";
                    break;
                case 8:
                    Time = "18";
                    break;
                case 9:
                    Time = "19";
                    break;
                case 10:
                    Time = "20";
                    break;
                case 11:
                    Time = "21";
                    break;
                default:
                    break;
            }

            for(int i=0;i<val;i++)
            {
                if(TimeCount[i].c_str() == Time)
                {
                    it = DataRead.find(Time);
                    OutputValue = (it->second);
                    flag = true;
                    break;
                }
                else
                {
                    flag = false;
                }
            }

            if(!flag)
            {
                TransactionTotal = 0;
                OutputValue = GetHourlyFormat(MallCode, DateValue, Time, AmountValue, TaxValue, AdjValue, SChargeValue, TransactionTotal, SalesDept);
            }

            OutputValue = RIdentifier + Format + OutputValue;

            DataToWrite.push_back(OutputValue.t_str());

            TMallExportDataManager::ExportFile(OutputManager, DataToWrite, FFileURI);
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

// This method will generate the output format for hourly export file
UnicodeString TMallExportShangrilaMall::GetHourlyFormat(UnicodeString TenantName, UnicodeString DateValue, UnicodeString TimeValue,
                                                        UnicodeString AmountValue, UnicodeString TaxValue, UnicodeString AdjValue, UnicodeString SChargeValue,
                                                        int TransactionTotal, UnicodeString SalesDept)
{
    UnicodeString result = "";
    UnicodeString Format = " ";
    UnicodeString TimeData = "";

    TimeData = GetRegTime(TimeValue);
    DateValue = FixDate(DateValue);

    if(TransactionTotal != 0)
    {
        result = "\"" + TenantName + "\"" + Format + DateValue + Format + TimeData + Format +
                 AmountValue + Format + TaxValue + Format + AdjValue + Format + SChargeValue + Format +
                 TransactionTotal + Format + "\"" + SalesDept + "\"" + "\n";
    }
    else
    {
        result = "\"" + TenantName + "\"" + Format + DateValue + Format + TimeData + Format +
                 "0.00" + Format + "0.00" + Format + "0.00" + Format + "0.00" + Format +
                 "0" + Format + "\"" + SalesDept + "\"" + "\n";
    }

    return result;
}
//---------------------------------------------------------------------------

// This function is for arranging the format of the output file for ZED
TExportResponse TMallExportShangrilaMall::CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite,
                                                                UnicodeString RIdentifier)
{
    TExportResponse result;

    try
    {
        int i = 0;
        UnicodeString Format = " ";
        UnicodeString OutputValue = "";
        UnicodeString GetData = "";
        UnicodeString CheckDate = Now().FormatString("mmddyy");

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
                it = DataRead.find("TransactionDate"); // For Date
            }
            else if(i==2)
            {
                it = DataRead.find("TransactionDate"); // For Time
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
                it = DataRead.find("MallCode");
            }


            if ((i==0) || (i==8))
            {
                OutputValue = "\"" + (it->second) + "\"" + Format;
            }
            else if (i==1)
            {
                if(PerformedLastZed())
                {
                    OutputValue = Now().FormatString("mm/dd/yyyy") + Format;
                }
                else
                {
                    OutputValue = GetDate(it->second) + Format;
                }
            }
            else if (i==2)
            {
                OutputValue = "09:00" + Format;
//                OutputValue = GetTime(it->second) + Format;
            }
            else if (i>=3 && i<=7)
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

// Get Time format
UnicodeString TMallExportShangrilaMall::GetTime(UnicodeString DateTime)
{
    AnsiString time = "";
    UnicodeString DateVal = "";
    int TimeComp = 0;

    try
    {
        TDateTime date(DateTime);
        time = date.FormatString("hh");
        TimeComp = StrToInt(time);

        if(TimeComp >= 0 && TimeComp < 6)
        {
            DateVal = "09:00";
        }
        else if(TimeComp < 10 && TimeComp > 5)
        {
            DateVal = "10:00";
        }
        else
        {
            DateVal = GetRegTime(DateTime);
        }
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return DateVal;
}
//---------------------------------------------------------------------------

// Get Date format
UnicodeString TMallExportShangrilaMall::GetDate(UnicodeString DateTime)
{
    AnsiString time = "";
    AnsiString date = "";
    UnicodeString DateVal = "";
    int TimeComp = 0;

    try
    {
        TDateTime adate(DateTime);
        time = adate.FormatString("hh");
        DateVal = adate.FormatString("mm/dd/yyyy");
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return DateVal;
}
//---------------------------------------------------------------------------

UnicodeString TMallExportShangrilaMall::GetRegTime(UnicodeString DateTime)
{
    AnsiString time = "";
    int TimeComp = 0;
    UnicodeString TimeVal = "";

    TDateTime date(DateTime);
    time = date.FormatString("hh");
    TimeComp = StrToInt(time);

    if(TimeComp < 10)
    {
        TimeVal = "10:00";
    }
    else if(TimeComp == 13)
    {
        TimeVal = "01:00";
    }
    else if(TimeComp == 14)
    {
        TimeVal = "02:00";
    }
    else if(TimeComp == 15)
    {
        TimeVal = "03:00";
    }
    else if(TimeComp == 16)
    {
        TimeVal = "04:00";
    }
    else if(TimeComp == 17)
    {
        TimeVal = "05:00";
    }
    else if(TimeComp == 18)
    {
        TimeVal = "06:00";
    }
    else if(TimeComp == 19)
    {
        TimeVal = "07:00";
    }
    else if(TimeComp == 20)
    {
        TimeVal = "08:00";
    }
//    else if((TimeComp >= 21 && TimeComp <= 23) || (TimeComp == 0))
    else if((TimeComp >= 21 && TimeComp <= 23))
    {
        TimeVal = "09:00";
    }
    else
    {
        TimeVal = IntToStr(TimeComp) + ":00";
    }

    return TimeVal;
}
//---------------------------------------------------------------------------

UnicodeString TMallExportShangrilaMall::FixDate(UnicodeString DateValue)
{
    std::string DateSTR = "";
    UnicodeString Date = DateValue;
    UnicodeString Month = "";
    UnicodeString Day = "";
    UnicodeString Year = "";

    UnicodeString Filename = "";
    TDateTime Yesterday = Now() - 1.0;
    UnicodeString FileExt = ".txt";
    UnicodeString DateFormat = "";
    UnicodeString MallPath = FilePath;

    DateSTR = Date.t_str();

    for(int i=0;i<Date.Length();i++)
    {
        if(i<=3)
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

    if(DateValue == "")
    {
        Month = Yesterday.FormatString("mm");
        Day = Yesterday.FormatString("dd");
        Year = Yesterday.FormatString("yyyy");
        DateFormat = Month + Day + Year;

        DateValue = (PerformedLastZed()) ? Now().FormatString("mm/dd/yyyy") : Month + "/" + Day + "/" + Year;
    }
    else
    {
        DateValue = Month + "/" + Day + "/" + Year;
    }

    return DateValue;
}
//---------------------------------------------------------------------------

UnicodeString TMallExportShangrilaMall::FixDecimalPlaces(Currency AmountTotal)
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

void TMallExportShangrilaMall::CheckPreviousEODFile()
{
    UnicodeString CheckDate = Now().FormatString("mmddyy");
    UnicodeString TenantID = "";
    UnicodeString FileExt = ".txt";
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

//    ZReportStatus = PerformedLastZed();

    YesterdayDate = Yesterday.FormatString("mm/dd/yy");

    if(LastZedDate == "")
    {
        ZReportStatus = false;
    }
    else if(YesterdayDate.Compare(LastZedDate) != 0)
    {
        if(CurrentDate.Compare(LastZedDate) != 0)
        {
            ZReportStatus = false;
        }
        else
        {
            ZReportStatus = (StrToInt(LastZedTime) > 5) ? false : true;
        }
    }
    else
    {
        ZReportStatus = (StrToInt(LastZedTime) > 5) ? true : false;
    }

    if(Hour >= 6)
    {
//        if(!ZReportStatus)
        if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0))
        {
            frmMain->ZForced();
        }
    }
}
//---------------------------------------------------------------------------

// This Function checks if there are data in the DB so we can detect if zed is peformed in the previous
// time the POS was used
bool TMallExportShangrilaMall::PerformedLastZed()
{
    bool flag = true;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
    query->Close();

    query->SQL->Text = "SELECT * FROM MALLEXPORT_HOURLY";
    DBTransaction.StartTransaction();
    query->ExecQuery();

    flag = (query->RecordCount == 0) ? true : false;

    return flag;
}
//---------------------------------------------------------------------------
