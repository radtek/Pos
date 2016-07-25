//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportAlphalandMall.h"
#include "MallExportDataManager.h"
#include "MallExportIMall.h"
#include "ManagerVariable.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "ReportUtilities.h"
#include "ReportFinancialCalculations.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportAlphalandMall::TMallExportAlphalandMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportAlphalandMall::~TMallExportAlphalandMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportAlphalandMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Export End Of Day File
TExportResponse TMallExportAlphalandMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        UnicodeString MallPath = FilePath;
        UnicodeString MallPathFileName = "";
        UnicodeString LocalPath = BasePath + "\\AlphalandMall\\";
        UnicodeString LocalPathFileName = "";

        DataRead.clear();
        DataToWrite.clear();
        TGlobalSettings::Instance().ZCount += 1;
        SaveZCount(vmZCount, TGlobalSettings::Instance().ZCount);
        DataQuery = getAlphalandQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFilename("ZED", MallPath, LocalPath, LocalPathFileName, MallPathFileName);
        CreateOutputFormat(DataRead, DataToWrite);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);

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

// Export Every Transaction File
TExportResponse TMallExportAlphalandMall::TransactionExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        UnicodeString MallPath = FilePath;
        UnicodeString MallPathFileName = "";
        UnicodeString LocalPath = BasePath + "\\AlphalandMall\\";
        UnicodeString LocalPathFileName = "";

        DataRead.clear();
        DataToWrite.clear();
        CreateFilename("TRANS", MallPath, LocalPath, LocalPathFileName, MallPathFileName);

        CreateOutputFormatTransaction(DataRead, DataToWrite);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, MallPathFileName);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);

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
void TMallExportAlphalandMall::initTimingExportResponse( TExportResponse& outResponse )
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

// This method create file names for TRANSACTION and EOD
TExportResponse TMallExportAlphalandMall::CreateFilename(UnicodeString mode, UnicodeString MallPath, UnicodeString LocalPath,
                                                         UnicodeString &LocalPathFileName, UnicodeString &MallPathFileName)
{
    TExportResponse result;

    try
    {
        UnicodeString CheckDate = Now().FormatString("yymmdd");
        UnicodeString AlphaCode = "TMS";
        UnicodeString OutputType = ".EOD";
        UnicodeString TerminalNo = TGlobalSettings::Instance().TerminalNo;
        UnicodeString Invoice = "";
        UnicodeString InvoiceNumber = "";
        UnicodeString TerminalNum = "";
        UnicodeString FileExt = ".txt";
        UnicodeString ZeroesTerminal = "";
        UnicodeString ZeroesInvoice = "";
        UnicodeString ItemNo = "";
        UnicodeString StrCount = "";
        std::ostringstream StreamIntToStr;
        std::string StringIntVal = "";

        std::map<UnicodeString, UnicodeString>::iterator it;

        Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
        TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

        TerminalNum = TGlobalSettings::Instance().TerminalNo;

        query->SQL->Text = "SELECT * FROM MALLEXPORT_TRANSACTION WHERE TERMINAL_NAME=:TERMINAL_NAME "
                           "ORDER BY ME_TRANSACTION_KEY ASC";
        DBTransaction.StartTransaction();
        query->ParamByName("TERMINAL_NAME")->AsString = TerminalNum;
        query->ExecQuery();

        if(Invoice == "")
        {
            Invoice = "0";
        }

        while(!query->Eof)
        {
            Invoice = query->FieldByName("INVOICE_NUMBER")->AsString;
            query->Next();
        }
        DBTransaction.Commit();
        query->Close();

        StreamIntToStr << StrToInt(TGlobalSettings::Instance().TerminalNo);
        StringIntVal = StreamIntToStr.str();
        ItemNo = StringIntVal.c_str();
        TerminalNo = ItemNo;

        ZeroesTerminal = "";
        for(int j=0;j<(3-ItemNo.Length());j++)
        {
            ZeroesTerminal += "0";
        }

        StreamIntToStr.str("");
        StreamIntToStr.clear();

        StreamIntToStr << StrToInt(Invoice);
        StringIntVal = StreamIntToStr.str();
        ItemNo = StringIntVal.c_str();

        ZeroesInvoice = "";
        for(int j=0;j<(8-ItemNo.Length());j++)
        {
            ZeroesInvoice += "0";
        }

        TerminalNo = ZeroesTerminal + TerminalNo;
        InvoiceNumber = ZeroesInvoice + Invoice;


        MallPath = CheckDir(MallPath);

        LocalPath = CheckDir(LocalPath);

        if(mode == "ZED")
        {
            LocalPathFileName = LocalPath + AlphaCode + CheckDate + TerminalNo + OutputType;
            MallPathFileName = MallPath + AlphaCode + CheckDate + TerminalNo + OutputType;
        }
        else
        {
            LocalPathFileName = LocalPath + AlphaCode + CheckDate + InvoiceNumber + "." + TerminalNo;
            MallPathFileName = MallPath + AlphaCode + CheckDate + InvoiceNumber + "." + TerminalNo;
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

// This function is for getting query from the DB Table
UnicodeString TMallExportAlphalandMall::getAlphalandQuery()
{
    UnicodeString AlphalandQuery = "";
    AlphalandQuery = "SELECT * FROM ALPHALAND_ZED_MALLVIEW";

    return AlphalandQuery;
}
//---------------------------------------------------------------------------

// Saving ZCount Global Variable to the DB
void TMallExportAlphalandMall::SaveZCount(vmVariables vmVar, int CompName)
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

// This function is for arranging the format of the output file for EOD
TExportResponse TMallExportAlphalandMall::CreateOutputFormat(std::map<UnicodeString, UnicodeString> DataRead, std::vector<UnicodeString> &DataToWrite)
{
    TExportResponse result;

    try
    {

        int i = 0;
        UnicodeString Format = "\t";
        UnicodeString OutputValue = "";
        TDateTime FormatDate;

        UnicodeString Zeroes = "";
        UnicodeString ItemNo = "";
        UnicodeString StrCount = "";
        UnicodeString Number = "";
        UnicodeString TerminalName = "";
        std::ostringstream StreamIntToStr;
        std::string StringIntVal = "";

        std::map<UnicodeString, UnicodeString>::iterator it;

        for(i=0;i<DataRead.size();i++) {
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
                it = DataRead.find("TransactionDate");
            }
            else if(i==3)
            {
                it = DataRead.find("BeginningInvoiceNo");
            }
            else if(i==4)
            {
                it = DataRead.find("EndingInvoiveNo");
            }
            else if(i==5)
            {
                it = DataRead.find("TotalGrossSale");
            }
            else if(i==6)
            {
                it = DataRead.find("TotalSaleTax");
            }
            else if(i==7)
            {
                it = DataRead.find("NonVatSales");
            }
            else if(i==8)
            {
                it = DataRead.find("TaxExemptSales");
            }
            else if(i==9)
            {
                it = DataRead.find("TotalCancelled");
            }
            else if(i==10)
            {
                it = DataRead.find("TotalCancelledCount");
            }
            else if(i==11)
            {
                it = DataRead.find("TotalDiscount");
            }
            else if(i==12)
            {
                it = DataRead.find("DiscountCount");
            }
            else if(i==13)
            {
                it = DataRead.find("TotalRefund");
            }
            else if(i==14)
            {
                it = DataRead.find("TotalRefundCount");
            }
            else if(i==15)
            {
                it = DataRead.find("TotalSeniorCitizenDiscount");
            }
            else if(i==16)
            {
                it = DataRead.find("TotalSeniorCitizenDiscountCount");
            }
            else if(i==17)
            {
                it = DataRead.find("TotalServiceCharge");
            }
            else if(i==18)
            {
                it = DataRead.find("CardSales");
            }
            else if(i==19)
            {
                it = DataRead.find("CashSales");
            }
            else if(i==20)
            {
                it = DataRead.find("CheckSales");
            }
            else if(i==21)
            {
                it = DataRead.find("GiftChequeSales");
            }
            else if(i==22)
            {
                it = DataRead.find("EPaySales");
            }
            else if(i==23)
            {
                it = DataRead.find("OtherSales");
            }
            else if(i==24)
            {
                it = DataRead.find("NoSales");
            }
            else if(i==25)
            {
                it = DataRead.find("PrevEODCounter");
            }
            else if(i==26)
            {
                it = DataRead.find("GrandTotalOld");
            }
            else if(i==27)
            {
                it = DataRead.find("CurrentEODCounter");
            }
            else if(i==28)
            {
                it = DataRead.find("GrandTotal");
            }
            else if(i==29)
            {
                it = DataRead.find("TransactionCount");
            }

            OutputValue = (it->second) + Format;


            if(i==1)
            {
                TerminalName = TGlobalSettings::Instance().TerminalNo;
                StreamIntToStr << StrToInt(TerminalName);
                StringIntVal = StreamIntToStr.str();
                Number = StringIntVal.c_str();
                TerminalName = Number;
                OutputValue = AddZeroesToValue(3,StrToInt(TerminalName));
                OutputValue = OutputValue + TerminalName + Format;
            }
            else if(i==2)
            {
                FormatDate = StrToDateTime(it->second);
                OutputValue = FormatDate.FormatString("yyyy-mm-dd") + Format;
            }
            else if((i==3) || (i==4))
            {
                OutputValue = AddZeroesToValue(8,StrToInt(it->second));
                OutputValue = OutputValue + (it->second) + Format;
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

// This function is for checking the specified path for the Mall is existing
// if not, it will create a local folder called MallExport then create a folder
UnicodeString TMallExportAlphalandMall::CheckDir(UnicodeString Path)
{
    if(!DirectoryExists(Path))
    {
        Path = BasePath;
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
        Path = Path + "\\AlphalandMall\\";
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
    }
    return Path;
}
//---------------------------------------------------------------------------

// This function is used to create yearly folder for the mall
UnicodeString TMallExportAlphalandMall::CreateYearlyFolder(UnicodeString Path, UnicodeString Year)
{
    Path = Path + Year;
    if(!DirectoryExists(Path))
    {
        CreateDir(Path);
    }

    return Path + "\\";
}
//---------------------------------------------------------------------------

// This method creates the data for transaction export file
TExportResponse TMallExportAlphalandMall::CreateOutputFormatTransaction(std::map<UnicodeString, UnicodeString> DataRead,
                                                                        std::vector<UnicodeString> &DataToWrite)
{
    UnicodeString OutputValue = "";
    UnicodeString Format = "\t";
    UnicodeString DateTime = Now().FormatString("hh:nn:ss");

    UnicodeString DateValue = "";
    UnicodeString TenantName = "";
    UnicodeString CurrentDateValue = "";
    UnicodeString TimeValue = "";
    UnicodeString TerminalName = "";
    int InvoiceNumber = 0;
    UnicodeString GrossSales = "";
    UnicodeString VatSales = "";
    UnicodeString NonVatSales = "";
    UnicodeString VatExemptSales = "";
    UnicodeString SCDiscount = "";
    UnicodeString EMPDiscount = "";
    UnicodeString PRODiscount = "";
    UnicodeString OTHERDiscount = "";
    UnicodeString RefundAmount = "";
    UnicodeString ServiceChargeAmount = "";
    UnicodeString OtherSurcharge = "";
    UnicodeString NetSales = "";
    UnicodeString OldGrandTotal = "";
    UnicodeString NewGrandTotal = "";
    UnicodeString ZCount = "";
    UnicodeString CashSales = "";
    UnicodeString CardSales = "";
    UnicodeString CheckSales = "";
    UnicodeString GCSales = "";
    UnicodeString EPaySales = "";
    UnicodeString OtherSales = "";
    UnicodeString CancelAmount = "";
    UnicodeString DepositAmount = "";
    UnicodeString AppliedDepositAmount = "";
    int VatSalesCount = 0;
    int NonVatSalesCount = 0;
    int VatExemptSalesCount = 0;
    int SCDiscountCount = 0;
    int EMPDiscountCount = 0;
    int PRODiscountCount = 0;
    int OTHERDiscountCount = 0;
    int RefundCount = 0;
    int ServiceChargeCount = 0;
    int OtherSurchargeCount = 0;
    int CashSalesCount = 0;
    int CardSalesCount = 0;
    int CheckSalesCount = 0;
    int GCSalesCount = 0;
    int EPaySalesCount = 0;
    int OtherSalesCount = 0;
    int CancelCount = 0;
    int TransVoidCount = 0;
    int DepositCount = 0;
    int AppliedDepositCount = 0;
    int PatronCount = 0;
    int TransactionCount = 0;
    int NoSale = 0;
    UnicodeString SalesFlag = "";
    UnicodeString VatPercentage = "";
    UnicodeString ItemNo = "";
    std::ostringstream StreamIntToStr;
    std::string StringIntVal = "";


    Currency AmountValue = 0;
    int TransactionTotal = 0;
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

        UnicodeString TerminalNum = TGlobalSettings::Instance().TerminalNo;

        query->SQL->Text = "SELECT * FROM MALLEXPORT_TRANSACTION WHERE TERMINAL_NAME=:TERMINAL_NAME AND TIME_VALUE = :TIME_VALUE "
                           "ORDER BY ME_TRANSACTION_KEY ASC";
        DBTransaction.StartTransaction();
        query->ParamByName("TERMINAL_NAME")->AsString = TerminalNum;
        query->ParamByName("TIME_VALUE")->AsString = DateTime;
        query->ExecQuery();

        while(!query->Eof)
        {
            DateValue = query->FieldByName("DATE_VALUE")->AsString;
            OutputValue = DateValue + Format;
            DataToWrite.push_back(OutputValue.t_str());

            TenantName = query->FieldByName("TENANT_NAME")->AsString;
            OutputValue = TenantName + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CurrentDateValue = query->FieldByName("CURRENT_DATE_VALUE")->AsString;
            OutputValue = CurrentDateValue + Format;
            DataToWrite.push_back(OutputValue.t_str());

            TimeValue = query->FieldByName("TIME_VALUE")->AsString;
            OutputValue = TimeValue + Format;
            DataToWrite.push_back(OutputValue.t_str());

            TerminalName = query->FieldByName("TERMINAL_NAME")->AsString;
            StreamIntToStr << StrToInt(TerminalName);
            StringIntVal = StreamIntToStr.str();
            ItemNo = StringIntVal.c_str();
            TerminalName = ItemNo;
            OutputValue = AddZeroesToValue(3,StrToInt(TerminalName)) + TerminalName + Format;
            DataToWrite.push_back(OutputValue.t_str());

            InvoiceNumber = query->FieldByName("INVOICE_NUMBER")->AsInteger;
            OutputValue = AddZeroesToValue(8,InvoiceNumber) + InvoiceNumber + Format;
            DataToWrite.push_back(OutputValue.t_str());

            GrossSales = query->FieldByName("GROSS_SALES")->AsString;
            OutputValue = CheckValue(GrossSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            VatSales = query->FieldByName("VAT_SALES")->AsString;
            OutputValue = CheckValue(VatSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            NonVatSales = query->FieldByName("NONVAT_SALES")->AsString;
            OutputValue = CheckValue(NonVatSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            VatExemptSales = query->FieldByName("VATEXEMPT_SALES")->AsString;
            OutputValue = CheckValue(VatExemptSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            SCDiscount = query->FieldByName("SCDISCOUNT_AMOUNT")->AsString;
            OutputValue = CheckValue(SCDiscount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            EMPDiscount = query->FieldByName("EMPDISCOUNT_AMOUNT")->AsString;
            OutputValue = CheckValue(EMPDiscount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            PRODiscount = query->FieldByName("PRODISCOUNT_AMOUNT")->AsString;
            OutputValue = CheckValue(PRODiscount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OTHERDiscount = query->FieldByName("OTHERDISCOUNT_AMOUNT")->AsString;
            OutputValue = CheckValue(OTHERDiscount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            RefundAmount = query->FieldByName("REFUND_AMOUNT")->AsString;
            OutputValue = CheckValue(RefundAmount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            ServiceChargeAmount = query->FieldByName("SCHARGE_AMOUNT")->AsString;
            OutputValue = CheckValue(ServiceChargeAmount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OtherSurcharge = query->FieldByName("OTHER_SURCHARGE")->AsString;
            OutputValue = CheckValue(OtherSurcharge) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            NetSales = query->FieldByName("NET_SALES")->AsString;
            OutputValue = CheckValue(NetSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OldGrandTotal = query->FieldByName("OLD_GRANDTOTAL")->AsString;
            OutputValue = CheckValue(OldGrandTotal) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            NewGrandTotal = query->FieldByName("NEW_GRANDTOTAL")->AsString;
            OutputValue = CheckValue(NewGrandTotal) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            ZCount = query->FieldByName("ZCOUNT")->AsString;
            OutputValue = ZCount + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CashSales = query->FieldByName("CASH_SALES_AMOUNT")->AsString;
            OutputValue = CheckValue(CashSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CardSales = query->FieldByName("CARD_SALES_AMOUNT")->AsString;
            OutputValue = CheckValue(CardSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CheckSales = query->FieldByName("CHECK_SALES_AMOUNT")->AsString;
            OutputValue = CheckValue(CheckSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            GCSales = query->FieldByName("GC_SALES_AMOUNT")->AsString;
            OutputValue = CheckValue(GCSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            EPaySales = query->FieldByName("EPAY_SALES_AMOUNT")->AsString;
            OutputValue = CheckValue(EPaySales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OtherSales = query->FieldByName("OTHER_SALES_AMOUNT")->AsString;
            OutputValue = CheckValue(OtherSales) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CancelAmount = query->FieldByName("VOID_AMOUNT")->AsString;
            OutputValue = CheckValue(CancelAmount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            DepositAmount = query->FieldByName("DEPOSIT_AMOUNT")->AsString;
            OutputValue = CheckValue(DepositAmount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            AppliedDepositAmount = query->FieldByName("APPLIED_DEPOSIT_AMOUNT")->AsString;
            OutputValue = CheckValue(AppliedDepositAmount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            VatSalesCount = query->FieldByName("VAT_SALES_COUNT")->AsInteger;
            OutputValue = IntToStr(VatSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            NonVatSalesCount = query->FieldByName("NONVAT_SALES_COUNT")->AsInteger;
            OutputValue = IntToStr(NonVatSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            VatExemptSalesCount = query->FieldByName("VATEXEMPT_SALES_COUNT")->AsInteger;
            OutputValue = IntToStr(VatExemptSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            SCDiscountCount = query->FieldByName("SCDISCOUNT_COUNT")->AsInteger;
            OutputValue = IntToStr(SCDiscountCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            EMPDiscountCount = query->FieldByName("EMPDISCOUNT_COUNT")->AsInteger;
            OutputValue = IntToStr(EMPDiscountCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            PRODiscountCount = query->FieldByName("PRODISCOUNT_COUNT")->AsInteger;
            OutputValue = IntToStr(PRODiscountCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OTHERDiscountCount = query->FieldByName("OTHERDISCOUNT_COUNT")->AsInteger;
            OutputValue = IntToStr(OTHERDiscountCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            RefundCount = query->FieldByName("REFUND_COUNT")->AsInteger;
            OutputValue = IntToStr(RefundCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            ServiceChargeCount = query->FieldByName("SCHARGE_COUNT")->AsInteger;
            OutputValue = IntToStr(ServiceChargeCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OtherSurchargeCount = query->FieldByName("OTHER_SURCHARGE_COUNT")->AsInteger;
            OutputValue = IntToStr(OtherSurchargeCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CashSalesCount = query->FieldByName("CASH_COUNT")->AsInteger;
            OutputValue = IntToStr(CashSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CardSalesCount = query->FieldByName("CARD_COUNT")->AsInteger;
            OutputValue = IntToStr(CardSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CheckSalesCount = query->FieldByName("CHECK_COUNT")->AsInteger;
            OutputValue = IntToStr(CheckSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            GCSalesCount = query->FieldByName("GC_COUNT")->AsInteger;
            OutputValue = IntToStr(GCSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            EPaySalesCount = query->FieldByName("EPAY_COUNT")->AsInteger;
            OutputValue = IntToStr(EPaySalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            OtherSalesCount = query->FieldByName("OTHER_COUNT")->AsInteger;
            OutputValue = IntToStr(OtherSalesCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            CancelCount = query->FieldByName("VOID_COUNT")->AsInteger;
            OutputValue = IntToStr(CancelCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            TransVoidCount = query->FieldByName("TRANS_VOID_COUNT")->AsInteger;
            OutputValue = IntToStr(TransVoidCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            DepositCount = query->FieldByName("DEPOSIT_COUNT")->AsInteger;
            OutputValue = IntToStr(DepositCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            AppliedDepositCount = query->FieldByName("APPLIED_DEPOSIT_COUNT")->AsInteger;
            OutputValue = IntToStr(AppliedDepositCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            PatronCount = query->FieldByName("PATRON_COUNT")->AsInteger;
            OutputValue = IntToStr(PatronCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            TransactionCount = query->FieldByName("TRANSACTION_COUNT")->AsInteger;
            OutputValue = IntToStr(TransactionCount) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            NoSale = query->FieldByName("NOSALE_COUNT")->AsInteger;
            OutputValue = IntToStr(NoSale) + Format;
            DataToWrite.push_back(OutputValue.t_str());

            SalesFlag = query->FieldByName("SALES_FLAG")->AsString;
            OutputValue = SalesFlag + Format;
            DataToWrite.push_back(OutputValue.t_str());

            VatPercentage = query->FieldByName("VAT_PERCENTAGE")->AsString;
            OutputValue = CheckValue(VatPercentage);
            DataToWrite.push_back(OutputValue.t_str());

            query->Next();
        }
        DBTransaction.Commit();
        query->Close();


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

// This method add zero in the front of data for formating purposes
UnicodeString TMallExportAlphalandMall::AddZeroesToValue(int ZeroCount, int Value)
{
    UnicodeString Zeroes = "";
    UnicodeString ItemNo = "";
    UnicodeString StrCount = "";

    std::ostringstream StreamIntToStr;
    std::string StringIntVal = "";

    StreamIntToStr << Value;
    StringIntVal = StreamIntToStr.str();
    ItemNo = StringIntVal.c_str();

    Zeroes = "";
    for(int j=0;j<(ZeroCount-ItemNo.Length());j++)
    {
        Zeroes += "0";
    }

    return Zeroes;
}
//---------------------------------------------------------------------------

// This method checks if the currency data has a decimal or not.
// If there is a decimal value it will do nothing but if there is
// no decimal value, this method will add .00 in the end of the value
UnicodeString TMallExportAlphalandMall::CheckValue(Currency Amount)
{
    UnicodeString result = "";
    std::string StrValue = "";
    bool Flag = false;


    if(Amount == 0)
    {
        result = "0.00";
    }
    else
    {
        result = RoundToNearest(Amount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
        StrValue = result.t_str();
        for(int i=0; i<StrValue.length();i++)
        {
            if(StrValue[i] == '.')
            {
                Flag = true;
            }
        }

        if(!Flag)
        {
            result = result + ".00";
        }
    }

    return result;
}
//---------------------------------------------------------------------------
