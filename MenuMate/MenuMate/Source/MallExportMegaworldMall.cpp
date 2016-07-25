//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportMegaworldMall.h"
#include "MallExportDataManager.h"
#include "MallExportHourlyUpdate.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "Main.h"
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

// Delete this method if not needed
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
        TDateTime DateValue = CheckLastZed();
        UnicodeString LastZD = TGlobalSettings::Instance().LastZedDate;
        UnicodeString DateVal = DateValue.FormatString("mm/dd/yy");
        TDateTime Yesterday = Now() - 1.0;
        Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];

        unsigned short Hour = 0;
        unsigned short Minutes = 0;
        unsigned short Seconds = 0;
        unsigned short dummy = 0;

        Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

        TGlobalSettings::Instance().ZCount += 1;
        SaveIntVariable(vmZCount, TGlobalSettings::Instance().ZCount);

        if(Hour < 6 || gridBtn->Enabled == false)
        {
            LastZD = Yesterday.FormatString("mm/dd/yy");
        }

        if(TGlobalSettings::Instance().LastZedDate == "")
        {
            TGlobalSettings::Instance().BatchNo = 1;
        }
        else if(DateVal.Compare(LastZD) != 0)
        {
            TGlobalSettings::Instance().BatchNo = 1;
        }
        else
        {
            TGlobalSettings::Instance().BatchNo += 1;
        }
        SaveIntVariable(vmBatchNo, TGlobalSettings::Instance().BatchNo);

        // Zed Report Generation
        // Mall Path
        DataQuery = getMegaworldQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename("S", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
        CreateOutputFormatZed(DataRead, DataToWrite, MallPathFileName);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        // Local Path
//        InputManager->ReadFromDB(DataQuery,DataRead);
//        CreateFilename("S", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
//        CreateOutputFormatZed(DataRead, DataToWrite, LocalPathFileName);
//        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);


        // Hourly Report Gereration
        // Mall Path
        CreateFilename("H", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
        CreateOutputFormatHourly(DataRead, DataToWrite, MallPathFileName);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        // Local Path
//        CreateFilename("H", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
//        CreateOutputFormatHourly(DataRead, DataToWrite, LocalPathFileName);
//        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);

        // Discount Report Gereration
        // Mall Path
        CreateFilename("D", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
        CreateOutputFormatDiscount(DataRead, DataToWrite, MallPathFileName);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        // Local Path
//        CreateFilename("D", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
//        CreateOutputFormatDiscount(DataRead, DataToWrite, LocalPathFileName);
//        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);

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
            if(Hour < 6)
            {
                TGlobalSettings::Instance().LastZedDate = Yesterday.FormatString("mm/dd/yy");
                TGlobalSettings::Instance().LastTransactionDate = Yesterday.FormatString("mm/dd/yy");
            }
            else
            {
                TGlobalSettings::Instance().LastZedDate = Now().FormatString("mm/dd/yy");
                TGlobalSettings::Instance().LastTransactionDate = Now().FormatString("mm/dd/yy");
            }
            SaveStrVariable(vmLastZedDate, TGlobalSettings::Instance().LastZedDate);
            SaveStrVariable(vmLastTransactionDate, TGlobalSettings::Instance().LastTransactionDate);
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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS

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

// This function is for getting query from the DB Table for ZED
UnicodeString TMallExportMegaworldMall::getMegaworldQuery()
{
    UnicodeString MegaworldQuery = "";

    MegaworldQuery = "SELECT * FROM MEGAWORLD_MALLVIEW";

    return MegaworldQuery;
}
//---------------------------------------------------------------------------

TExportResponse TMallExportMegaworldMall::CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                                         UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName)
{
    TExportResponse result;

    try
    {
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString BatchNo = IntToStr(TGlobalSettings::Instance().BatchNo);
        UnicodeString TenantID = "";
        UnicodeString MonthValue = "";
        UnicodeString DayValue = "";
        TDateTime DateValue = CheckLastZed();

        GetExportInfo(MallPath, TenantID, TerminalNo, BatchNo, MonthValue, DayValue, DateValue);

        MallPath = CheckDir(MallPath);
//        LocalPath = CheckDir(LocalPath);

        MallPathFileName = MallPath + mode + TenantID + TerminalNo + BatchNo + "." + MonthValue + DayValue;
//        LocalPathFileName = LocalPath + mode + TenantID + TerminalNo + BatchNo + "." + MonthValue + DayValue;

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

// This function is for saving the integer global variable
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

// This Function is for saving each string component into the DB for every function call
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

TExportResponse TMallExportMegaworldMall::CreateOutputFormatZed(std::map<UnicodeString, UnicodeString> DataRead,
                                                                std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName)
{
    int i = 0;
    int RowNum = 0;
    UnicodeString ItemNo = "";
    std::ostringstream StreamIntToStr;
    std::string StringIntVal = "";
    UnicodeString OutputValue = "";
    UnicodeString SalesTypeID = "";
    UnicodeString DataAmount = "";
    UnicodeString FinalValue = "";
    UnicodeString Format = "\n";
    TDateTime DateValue = CheckLastZed();

    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];

    std::map<UnicodeString, UnicodeString>::iterator it;
    std::vector<UnicodeString> AvailableSalesType;
    DataToWrite.clear();

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        DBTransaction.StartTransaction();
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());
        TIBSQL* query1 = DBTransaction.Query(DBTransaction.AddQuery());
        query->Close();

        query->SQL->Text = "SELECT STIR.SALES_TYPE_KEY FROM SALES_TYPE_ITEMS_RELATION STIR "
                           "GROUP BY STIR.SALES_TYPE_KEY";
        query->ExecQuery();

        while(!query->Eof)
        {
	    	int SalesTypeDB = query->FieldByName("SALES_TYPE_KEY")->AsInteger;
            query1->Close();
            query1->SQL->Text = "SELECT ST.SALES_TYPE_NAME FROM SALES_TYPE ST WHERE ST.SALES_TYPE_KEY = :SALES_KEY";
	    	query1->ParamByName("SALES_KEY")->AsInteger = SalesTypeDB;
            query1->ExecQuery();
            UnicodeString SalesTypeValue = "SalesType" + query1->FieldByName("SALES_TYPE_NAME")->AsString;
            AvailableSalesType.push_back(SalesTypeValue.t_str());
            query->Next();
        }
        DBTransaction.Commit();


        for(i=0;i<DataRead.size();i++)
        {
            switch(i)
            {
                case 0:
                    it = DataRead.find("TenantNo");
                    break;
                case 1:
                    it = DataRead.find("TerminalNo");
                    break;
                case 2:
                    it = DataRead.find("TransactionDate");
                    break;
                case 3:
                    it = DataRead.find("GrandTotalOld");
                    break;
                case 4:
                    it = DataRead.find("GrandTotal");
                    break;
                case 5:
                    it = DataRead.find("TotalGrossSale");
                    break;
                case 6:
                    it = DataRead.find("TaxExemptSales");
                    break;
                case 7:
                    it = DataRead.find("TotalSeniorCitizenDiscount");
                    break;
                case 8:
                    it = DataRead.find("TotalRegularDiscount");
                    break;
                case 9:
                    it = DataRead.find("TotalRefund");
                    break;
                case 10:
                    it = DataRead.find("TotalSaleTax");
                    break;
                case 11:
                    it = DataRead.find("TotalServiceCharge");
                    break;
                case 12:
                    it = DataRead.find("DailySales");
                    break;
                case 13:
                    it = DataRead.find("CashSales");
                    break;
                case 14:
                    it = DataRead.find("CardSales");
                    break;
                case 15:
                    it = DataRead.find("OtherSales");
                    break;
                case 16:
                    it = DataRead.find("TotalCancelled");
                    break;
                case 17:
                    it = DataRead.find("FineDiningCustomerCount");
                    break;
                case 18:
                    it = DataRead.find("CurrentEODCounter");
                    break;
                case 19:
                    it = DataRead.find("TransactionCount");
                    break;
                case 20:
                    it = DataRead.find("SalesTypeFood");
                    SalesTypeID = "01";
                    break;
                case 21:
                    it = DataRead.find("SalesTypeNon-Food");
                    SalesTypeID = "02";
                    break;
                case 22:
                    it = DataRead.find("SalesTypeGroceries");
                    SalesTypeID = "03";
                    break;
                case 23:
                    it = DataRead.find("SalesTypeMedicines");
                    SalesTypeID = "04";
                    break;
                case 24:
                    it = DataRead.find("SalesTypeOthers");
                    SalesTypeID = "05";
                    break;
                default:
                    break;
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

            if((i >= 3 && i <= 16) || (i >= 20 && i <= 24))
            {
                Currency value = 0;
                DataAmount = (it->second);
                value = StrToCurr(DataAmount);
                DataAmount = FixDecimalPlaces(value);
                // Remove if decimal value should be shown
//                FinalValue = DataAmount;
                FinalValue = RemoveDecimalValue(DataAmount);
            }
            else
            {
                FinalValue = (it->second);
            }

            if(i == 2)
            {
                TDateTime Yesterday = Now() - 1;
                if(Hour < 6 || gridBtn->Enabled == false)
                {
                    OutputValue = ItemNo + DateValue.FormatString("mmddyyyy") + Format;
                }
                else
                {
                    OutputValue = ItemNo + Now().FormatString("mmddyyyy") + Format;
                }
                // Uncomment if date should be based on the MallExportTable
//                TDateTime DateonDB = StrToDateTime(it->second);
//                UnicodeString DateToData = DateonDB.FormatString("mmddyyyy");
//                OutputValue = ItemNo + DateToData + Format;
            }
            else if(i >= 20)
            {
                for(int j=0; j<AvailableSalesType.size();j++)
                {
                    if(AvailableSalesType.at(j).t_str() == (it->first))
                    {
                        OutputValue = "21" + SalesTypeID + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                        OutputValue = "22" + FinalValue;
                        OutputValue = (AvailableSalesType.size() == (j+1)) ? OutputValue : OutputValue + Format;
                        DataToWrite.push_back(OutputValue.t_str());
                    }
                }
            }
            else
            {
                OutputValue = ItemNo + FinalValue + Format;
            }
            FinalValue = "";

            if(i < 20)
            {
                DataToWrite.push_back(OutputValue.t_str());
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

// This method creates the hourly data for hourly export file
TExportResponse TMallExportMegaworldMall::CreateOutputFormatHourly(std::map<UnicodeString, UnicodeString> DataRead,
                                                                   std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName)
{
    UnicodeString OutputValue = "";
    UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;
    UnicodeString TenantName = TGlobalSettings::Instance().TenantNo;
    UnicodeString DateValue = Now().FormatString("mmddyyyy");
    Currency AmountSum = 0;
    int TransactionTotal = 0;
    int TransactionSum = 0;
    int PatronCount = 0;
    int PatronSum = 0;
    UnicodeString Amount = "";
    UnicodeString TotalValue = "";

    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    Touchcontrols::TGridButton *gridBtn = frmMain->tgridMenu->Buttons[0][0];

    DataToWrite.clear();

    TExportResponse result;

    try
    {
        if(Hour < 6 || gridBtn->Enabled == false)
        {
            TDateTime DateTemp = CheckLastZed();
            DateValue = DateTemp.FormatString("mmddyyyy");
        }

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
//                TempAmount = Amount;
                // Remove if decimal value should be shown
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
        // Remove if decimal value should be shown
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

// This method will generate the data for hourly export file
int TMallExportMegaworldMall::GetHourlyData(UnicodeString &TerminalName, UnicodeString &TenantName,
                                                      UnicodeString &DateValue, UnicodeString &TimeValue,
                                                      int &TransactionTotal, UnicodeString &Amount, int &PatronCount)
{
    Currency AmountValue = 0;

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

    query->SQL->Text = "SELECT SUM(AMOUNT_VALUE) AS AMOUNTSUM, SUM(TRANSACTION_COUNT) as TRANSSUM, SUM(PATRON_COUNT) AS PATRONSUM FROM MALLEXPORT_HOURLY "
                       "WHERE TIME_VALUE=:TIMEVALUE AND TERMINAL_NAME=:TERMINALNAME";
    DBTransaction.StartTransaction();
    query->ParamByName("TIMEVALUE")->AsString = TimeValue;
    query->ParamByName("TERMINALNAME")->AsString = TerminalName;
    query->ExecQuery();

    AmountValue = RoundToNearest(query->FieldByName("AMOUNTSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    TransactionTotal = RoundToNearest(query->FieldByName("TRANSSUM")->AsCurrency, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    PatronCount = query->FieldByName("PATRONSUM")->AsInteger;

    Amount = FixDecimalPlaces(AmountValue);

    DBTransaction.Commit();
    query->Close();

    return TransactionTotal;
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
//---------------------------------------------------------------------------

TExportResponse TMallExportMegaworldMall::CreateOutputFormatDiscount(std::map<UnicodeString, UnicodeString> DataRead,
                                                                     std::vector<UnicodeString> &DataToWrite, UnicodeString MallPathFileName)
{
    int i = 0;
    TDateTime Check = CheckLastZed();
    UnicodeString OutputValue = "";
    UnicodeString DateVal = Check.FormatString("mm/dd/yyyy");
    UnicodeString HourVal = TGlobalSettings::Instance().LastZedTime;
    UnicodeString DiscountName = "";
    UnicodeString DiscountDesc = "";
    UnicodeString DiscountTotal = "";
    UnicodeString Format = ",";
    Currency DiscountValue = 0;
    TDateTime Today = StrToDateTime(DateVal) + StrToTime(HourVal);

    DataToWrite.clear();

    TGlobalSettings::Instance().LastZedTime = Now().FormatString("hh:nn:ss");
    SaveStrVariable(vmLastZedTime, TGlobalSettings::Instance().LastZedTime);

    TExportResponse result;

    try
    {
        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

        query->SQL->Text = "SELECT ARCORDERDISCOUNTS.NAME, ARCORDERDISCOUNTS.DESCRIPTION, SUM(ARCORDERDISCOUNTS.DISCOUNTED_VALUE) AS DISC_VAL FROM ARCORDERDISCOUNTS INNER JOIN "
                           "ARCHIVE ON ARCORDERDISCOUNTS.ARCHIVE_KEY = ARCHIVE.ARCHIVE_KEY WHERE "
                           "ARCHIVE.TIME_STAMP >= :START_DATE GROUP BY ARCORDERDISCOUNTS.NAME, ARCORDERDISCOUNTS.DESCRIPTION";
        DBTransaction.StartTransaction();
        query->ParamByName("START_DATE")->AsDateTime = Today;
        query->ExecQuery();

        while(!query->Eof)
        {
            DiscountName = query->FieldByName("NAME")->AsString;
            DiscountDesc = query->FieldByName("DESCRIPTION")->AsString;
            DiscountValue = fabs(query->FieldByName("DISC_VAL")->AsCurrency);

            DiscountTotal = FixDecimalPlaces(DiscountValue);
            // Remove if decimal value should be shown
//            DiscountTotal = RemoveDecimalValue(DiscountValue);

            OutputValue = DiscountName + Format + DiscountDesc + Format + DiscountTotal + "\n";
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

// Checking of previous file will happen in 00:00 AM if the POS will not be exited
void TMallExportMegaworldMall::SetZedTimer()
{
    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    if(Hour < 6)
    {
        zedcheckTimer->Interval = ((3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
    else
    {
        zedcheckTimer->Interval = ((86400000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
}
//---------------------------------------------------------------------------

// Call from MallExporTIMall
void TMallExportMegaworldMall::CheckPreviousEODFile()
{
    UnicodeString CheckDate = Now().FormatString("mm/dd/yy");
    UnicodeString CurrentDate = Now().FormatString("mm/dd/yyyy");
    TDateTime Check = CheckLastZed();
    UnicodeString DateFormat = Check.FormatString("mm/dd/yyyy");
    bool ZReportStatus = false;

    unsigned short Hour = 0;
    unsigned short Minutes = 0;
    unsigned short Seconds = 0;
    unsigned short dummy = 0;

    Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

    // Checking of previous file will happen in 00:00 AM if the POS will not be exited
    if(Hour < 6)
    {
        zedcheckTimer->Interval = ((3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }
    else
    {
        zedcheckTimer->Interval = ((86400000 - (Hour*3600000)) + (3540000 - (Minutes*60000)) + (59000 - (Seconds*1000)));
    }

    if(DateFormat.Compare(CurrentDate) != 0)
    {
        ZReportStatus = false;
    }
    else
    {
        ZReportStatus = true;
    }

    if(Check < Now() && Hour >= 6)
    {
        if( (!ZReportStatus) && (TGlobalSettings::Instance().FirstMallDate.Compare(CheckDate) != 0))
        {
            frmMain->ZForced();
        }
    }
}
//---------------------------------------------------------------------------

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

        // TenantName format is the first 4 Character of the Tenant Name
        for(int i=0;i<TenantNo.Length();i++)
        {
            if(i<4)
            {
                TenantID = TenantID + TenantNameSTR[i];
            }
        }

        // Teminal number must be at least 2 digit
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

// This method checks if the prev zed report was not yet generated
// and returns the correspondent datetime
TDateTime TMallExportMegaworldMall::CheckLastZed()
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
