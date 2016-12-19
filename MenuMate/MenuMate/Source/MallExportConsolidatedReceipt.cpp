//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportConsolidatedReceipt.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ManagerReports.h"
#include "MallExportIInput.h"
#include "MallExportIMall.h"
#include "MallExportInputFileDriver.h"
#include "MallExportDefines.h"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <vector>

#include <DateUtils.hpp>

//---------------------------------------------------------------------------

MallExportConsolidatedReceipt::MallExportConsolidatedReceipt(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmZReport, mmMallExportConsolidatedReceipt, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

MallExportConsolidatedReceipt::MallExportConsolidatedReceipt(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	: BaseReportSection(mmZReport, mmMallExportConsolidatedReceipt, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

MallExportConsolidatedReceipt::~MallExportConsolidatedReceipt()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void MallExportConsolidatedReceipt::GetOutput(TPrintout* printOut, TDateTime* startTime, TDateTime* endTime)
{
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL && TGlobalSettings::Instance().EnableConsolidatedReport)
    {
        TMallExportInputFileDriver InputManager;
        AnsiString Date = Now().FormatString("mm/dd/yyyy");
        UnicodeString GetQuery = "SELECT * FROM MALLEXPORT";
        std::map<UnicodeString, UnicodeString> DataRead;
        TDateTime DateVal;
        UnicodeString SDateValue = "";
        UnicodeString EDateValue = "";
    }
}


void MallExportConsolidatedReceipt::GetOutput(TPrintout* printOut)
{
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL && TGlobalSettings::Instance().EnableConsolidatedReport)
    {
        TMallExportInputFileDriver InputManager;
        AnsiString Date = Now().FormatString("mm/dd/yyyy");
        UnicodeString GetQuery = "SELECT * FROM MALLEXPORT";
        std::map<UnicodeString, UnicodeString> DataRead;
        TDateTime DateVal;
        UnicodeString SDateValue = "";
        UnicodeString EDateValue = "";

        Currency DailySales = 0;
        Currency VAT = 0;
        Currency VatableSales = 0;
        Currency NonVatableSales = 0;
        Currency SCDisc = 0;
        Currency VATExempt = 0;
        Currency ZeroRated = 0;
        Currency NetSales = 0;
        int TotalQty = GetSalesDetails();
        int TransactionCount = 0;

        Currency TempDailySales = 0;
        Currency TempVAT = 0;
        Currency TempVatableSales = 0;
        Currency TempNonVatableSales = 0;
        Currency TempSCDisc = 0;
        Currency TempVATExempt = 0;
        Currency TempZeroRated = 0;
        Currency TempNetSales = 0;
        int TempTotalQty = 0;
        int TempTransactionCount = 0;
        unsigned short Hour = 0;
        unsigned short Minutes = 0;
        unsigned short Seconds = 0;
        unsigned short dummy = 0;

        UnicodeString Paths = TGlobalSettings::Instance().ConsolidateReportPaths;
        UnicodeString DBPath = "";
        std::string DataBases = Paths.t_str();
        std::string DBList[10];
        int count = 0;

        InputManager.ReadFromDB(GetQuery,DataRead);

        std::map<UnicodeString, UnicodeString>::iterator it;

        for(int i=0;i<DataRead.size();i++)
        {
            switch(i)
            {
                case 0:
                    it = DataRead.find("TotalGrossSale");
                    DailySales = (it->second);
                    break;
                case 1:
                    it = DataRead.find("TotalSaleTax");
                    VAT = (it->second);
                    break;
                case 2:
                    it = DataRead.find("TaxInclusiveSales");
                    VatableSales = (it->second);
                    break;
                case 3:
                    it = DataRead.find("TaxExemptSales");
                    NonVatableSales = (it->second);
                    break;
                case 4:
                    it = DataRead.find("TotalSeniorCitizenDiscount");
                    SCDisc = (it->second);
                    break;
                case 5:
                    it = DataRead.find("OtherCharges");
                    VATExempt = (it->second);
                    break;
                case 6:
                    it = DataRead.find("NonVatSales");
                    ZeroRated = (it->second);
                    break;
                case 7:
                    it = DataRead.find("DailySales");
                    NetSales = (it->second);
                    break;
                case 8:
                    it = DataRead.find("TransactionCount");
                    TransactionCount = StrToInt(it->second);
                    break;
                default:
                    break;
           }
        }

        for(int i=0;i< Paths.Length();i++)
        {

            if(DataBases[i] == ',' || DataBases[i] == NULL)
            {
                DBPath = DBPath + "\\" + "MENUMATE.FDB";
                DBList[count] = DBPath.t_str();
                count++;
                DBPath = "";
                continue;
            }
            DBPath = DBPath + DataBases[i];

        }

        for(int i=0;i<count;i++)
        {
            UnicodeString PathName = DBList[i].c_str();
            UnicodeString STime = "06:00:00";
            UnicodeString ETime = "06:00:00";
            TDateTime SDate;
            TDateTime EDate;

            auto_ptr<TIBDatabase>    db(new TIBDatabase(NULL));
            auto_ptr<TIBTransaction> tr(new TIBTransaction(NULL));
            auto_ptr<TIBSQL>         qr(new TIBSQL(NULL));

            qr->Database    = db.get();
            qr->Transaction = tr.get();

            tr->AddDatabase(db.get());

            db->DBParamByDPB[isc_dpb_user_name] = "sysdba";
            db->DBParamByDPB[isc_dpb_password]  = "masterkey";
            db->DatabaseName = PathName;
            db->LoginPrompt = false;
            db->Open();

            Now().DecodeTime(&Hour,&Minutes,&Seconds,&dummy);

            DateVal = (Hour >= 6) ? Now().CurrentDate() : Now().CurrentDate() - 1;

            SDate = DateVal;
            DateVal = DateVal + 1;
            EDate = DateVal;

            SDate = SDate + StrToTime(STime);
            EDate = EDate + StrToTime(ETime);

            qr->Close();
            qr->SQL->Text = "SELECT FIRST 1 SUM(GROSS_SALES) as GROSS_SALES, SUM(VAT_SALES) as VAT_SALES, "
                            "SUM(TAX_INCLUSIVE_SALES) as TAX_INCLUSIVE_SALES, SUM(VATEXEMPT_SALES) as VATEXEMPT_SALES, "
                            "SUM(SCDISCOUNT_AMOUNT) AS SCDISCOUNT_AMOUNT, SUM(OTHER_CHARGES) as OTHER_CHARGES, "
                            "SUM(NONVAT_SALES) as NONVAT_SALES, SUM(DAILY_SALES) as DAILY_SALES, "
                            "SUM(TRANSACTION_COUNT) as TRANSACTION_COUNT FROM ARCMALLEXPORT "
                            "WHERE TRANSACTIONDATE >= :STARTDATE AND TRANSACTIONDATE < :ENDDATE";
            tr->StartTransaction();

            qr->ParamByName("STARTDATE")->AsDateTime = SDate;
            qr->ParamByName("ENDDATE")->AsDateTime = EDate;

            SDateValue = SDate.FormatString("mm/dd/yyyy hh:nn:ss");
            EDateValue = EDate.FormatString("mm/dd/yyyy hh:nn:ss");
            qr->ExecQuery();

            if(!qr->Eof)
            {
                TempDailySales = qr->FieldByName("GROSS_SALES")->AsCurrency;
                TempVAT = qr->FieldByName("VAT_SALES")->AsCurrency;
                TempVatableSales = qr->FieldByName("TAX_INCLUSIVE_SALES")->AsCurrency;
                TempNonVatableSales = qr->FieldByName("VATEXEMPT_SALES")->AsCurrency;
                TempSCDisc = qr->FieldByName("SCDISCOUNT_AMOUNT")->AsCurrency;
                TempVATExempt = qr->FieldByName("OTHER_CHARGES")->AsCurrency;
                TempZeroRated = qr->FieldByName("NONVAT_SALES")->AsCurrency;
                TempNetSales = qr->FieldByName("DAILY_SALES")->AsCurrency;
                TempTransactionCount = qr->FieldByName("TRANSACTION_COUNT")->AsInteger;
            }
            tr->Commit();

            qr->Close();
            qr->SQL->Text = "SELECT SUM(QTY) AS QTY FROM ARCHIVE "
                            "WHERE TIME_STAMP_BILLED >= :STARTDATE AND TIME_STAMP_BILLED < :ENDDATE";
            tr->StartTransaction();

            qr->ParamByName("STARTDATE")->AsDateTime = SDate;
            qr->ParamByName("ENDDATE")->AsDateTime = EDate;
            qr->ExecQuery();

            if(!qr->Eof)
            {
                TempTotalQty = qr->FieldByName("QTY")->AsInteger;
            }
            tr->Commit();

            DailySales += TempDailySales;
            VAT += TempVAT;
            VatableSales += TempVatableSales;
            NonVatableSales += TempNonVatableSales;
            SCDisc += TempSCDisc;
            VATExempt += TempVATExempt;
            ZeroRated += TempZeroRated;
            NetSales += TempNetSales;
            TotalQty += TempTotalQty;
            TransactionCount += TempTransactionCount;
        	db->Close();
        }

        DataRead.clear();

        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

        printOut->PrintFormat->PartialCut();

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;

        TDeviceRealTerminal::Instance().LoadHdrFtr();
        TStringList *textContent = TGlobalSettings::Instance().ZedHeader.get();

        for (int i = 0; i < textContent->Count; i++)
        {
            printOut->PrintFormat->Line->Columns[0]->Text = textContent->Strings[i];
            printOut->PrintFormat->AddLine();
            if(i==(textContent->Count - 1))
            {
                printOut->PrintFormat->NewLine();
                printOut->PrintFormat->Line->Columns[0]->Text = "CONSOLIDATED REPORT Z-READ";
                printOut->PrintFormat->AddLine();
            }
        }
        printOut->PrintFormat->Line->FontInfo.Reset();

        AddTitleWithColumn(printOut, "DESCRIPTION","QTY/AMOUNT");

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
	    printOut->PrintFormat->Line->FontInfo.Reset();

    	printOut->PrintFormat->Line->Columns[0]->Text = "DAILY SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(DailySales);
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "VAT";
    	printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(VAT);
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "VATABLE SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(VatableSales);
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "NON-VATABLE SALES";
    	printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(NonVatableSales);
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "LESS SC DISC";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(SCDisc);
       	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "VAT EXEMPT";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(VATExempt);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "ZERO RATED";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(ZeroRated);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "NET SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(NetSales);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL QTY. SOLD";
	    printOut->PrintFormat->Line->Columns[1]->Text = IntToStr(TotalQty);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TRANSACTION COUNT";
	    printOut->PrintFormat->Line->Columns[1]->Text = IntToStr(TransactionCount);
    	printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	    printOut->PrintFormat->Line->ColCount = 1;
    	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	    printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
    	printOut->PrintFormat->Line->Columns[0]->Text = "";
	    printOut->PrintFormat->Line->Columns[0]->Line();
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->FontInfo.Reset();
        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;

        printOut->PrintFormat->Line->Columns[0]->Text = Date;
        printOut->PrintFormat->AddLine();
    }
    else if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        TMallExportInputFileDriver InputManager;
        AnsiString Date = Now().FormatString("mm/dd/yyyy");
        UnicodeString GetQuery = "SELECT * FROM MALLEXPORT";
        std::map<UnicodeString, UnicodeString> DataRead;

        UnicodeString TransactionDate = "";
        UnicodeString MerchantCode = "";
        UnicodeString MerchantName = "";
        Currency OldGrandTotal = 0;
        Currency NewGrandTotal = 0;
        Currency DailySales = 0;
        Currency GrandTotalDiscount = 0;
        Currency TotalDiscountSCD = 0;
        Currency TotalDiscountPWD = 0;
        Currency TotalDiscountGPC = 0;
        Currency TotalDiscountVIP = 0;
        Currency TotalDiscountEMP = 0;
        Currency TotalDiscountREG = 0;
        Currency TotalDiscountOTHERS = 0;
        Currency TotalRefund = 0;
        Currency TotalCancel = 0;
        Currency TotalVAT = 0;
        Currency TotalVATIncSales = 0;
        Currency TotalVATExSales = 0;
        UnicodeString BeginningOR = "";
        UnicodeString EndingOR = "";
        UnicodeString DocumentCount = "";
        UnicodeString CustomerCount = "";
        UnicodeString SCDCount = "";
        Currency LocalTax = 0;
        Currency ServiceCharge = 0;
        Currency TotalNonVatSale = 0;
        Currency RawGross = 0;
        Currency DailyLocalTax = 0;
        UnicodeString WorkStationNumber = "";
        Currency TotalCash = 0;
        Currency TotalCard = 0;
        Currency TotalOthers = 0;

        TDateTime DateTime;

        InputManager.ReadFromDB(GetQuery,DataRead);

        std::map<UnicodeString, UnicodeString>::iterator it;

        for(int i=0;i<DataRead.size();i++)
        {
            switch(i)
            {
                case 0:
                    it = DataRead.find("TransactionDate");
                    DateTime = StrToDateTime(it->second);
                    TransactionDate = DateTime.FormatString("mm/dd/yyyy");
                    break;
                case 1:
                    it = DataRead.find("TenantNo");
                    MerchantCode = (it->second);
                    break;
                case 2:
                    it = DataRead.find("MallCode");
                    MerchantName = (it->second);
                    break;
                case 3:
                    it = DataRead.find("GrandTotalOld");
                    OldGrandTotal = (it->second);
                    break;
                case 4:
                    it = DataRead.find("GrandTotal");
                    NewGrandTotal = (it->second);
                    break;
                case 5:
                    it = DataRead.find("DailySales");
                    DailySales = (it->second);
                    break;
                case 6:
                    it = DataRead.find("TotalDiscount");
                    GrandTotalDiscount = (it->second);
                    break;
                case 7:
                    it = DataRead.find("TotalSeniorCitizenDiscount");
                    TotalDiscountSCD = (it->second);
                    break;
                case 8:
                    it = DataRead.find("DisabilityDiscount");
                    TotalDiscountPWD = (it->second);
                    break;
                case 9:
                    it = DataRead.find("GPCDiscount");
                    TotalDiscountGPC = (it->second);
                    break;
                case 10:
                    it = DataRead.find("VIPDiscount");
                    TotalDiscountVIP = (it->second);
                    break;
                case 11:
                    it = DataRead.find("EmployeeDiscount");
                    TotalDiscountEMP = (it->second);
                    break;
                case 12:
                    it = DataRead.find("TotalRegularDiscount");
                    TotalDiscountREG = (it->second);
                    break;
                case 13:
                    it = DataRead.find("DiscountG1");
                    TotalDiscountOTHERS = (it->second);
                    break;
                case 14:
                    it = DataRead.find("TotalRefund");
                    TotalRefund = (it->second);
                    break;
                case 15:
                    it = DataRead.find("TotalCancelled");
                    TotalCancel = (it->second);
                    break;
                case 16:
                    it = DataRead.find("TotalSaleTax");
                    TotalVAT = (it->second);
                    break;
                case 17:
                    it = DataRead.find("TaxInclusiveSales");
                    TotalVATIncSales = (it->second);
                    break;
                case 18:
                    it = DataRead.find("TaxExemptSales");
                    TotalVATExSales = (it->second);
                    break;
                case 19:
                    it = DataRead.find("BeginningInvoiceNo");
                    BeginningOR = (it->second);
                    break;
                case 20:
                    it = DataRead.find("EndingInvoiveNo");
                    EndingOR = (it->second);
                    break;
                case 21:
                    it = DataRead.find("TransactionCount");
                    DocumentCount = (it->second);
                    break;
                case 22:
                    it = DataRead.find("FineDiningCustomerCount");
                    CustomerCount = (it->second);
                    break;
                case 23:
                    it = DataRead.find("TotalSeniorCitizenDiscountCount");
                    SCDCount = (it->second);
                    break;
                case 24:
                    it = DataRead.find("LocalTaxes");
                    LocalTax = (it->second);
                    break;
                case 25:
                    it = DataRead.find("TotalServiceCharge");
                    ServiceCharge = (it->second);
                    break;
                case 26:
                    it = DataRead.find("NonVatSales");
                    TotalNonVatSale = (it->second);
                    break;
                case 27:
                    it = DataRead.find("TotalGrossSale");
                    RawGross = (it->second);
                    break;
                case 28:
                    it = DataRead.find("LocalTaxExemptDailySales");
                    DailyLocalTax = (it->second);
                    break;
                case 29:
                    it = DataRead.find("TerminalNo");
                    WorkStationNumber = (it->second);
                    break;
                case 30:
                    it = DataRead.find("CashSales");
                    TotalCash = (it->second);
                    break;
                case 31:
                    it = DataRead.find("CardSales");
                    TotalCard = (it->second);
                    break;
                case 32:
                    it = DataRead.find("OtherSales");
                    TotalOthers = (it->second);
                    break;
                default:
                    break;
            }
        }

        DataRead.clear();

        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;

        printOut->PrintFormat->PartialCut();

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 1;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;

        printOut->PrintFormat->NewLine();
        printOut->PrintFormat->Line->Columns[0]->Text = "Z-READING";
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Reset();

        printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
        printOut->PrintFormat->Line->ColCount = 2;
        printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 2 / 3;
        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
        printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
        printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;

        printOut->PrintFormat->NewLine();
        printOut->PrintFormat->Line->Columns[0]->Text = "DATE";
	    printOut->PrintFormat->Line->Columns[1]->Text = Date;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->NewLine();
        printOut->PrintFormat->Line->Columns[0]->Text = MerchantCode;
	    printOut->PrintFormat->Line->Columns[1]->Text = MerchantName;
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Reset();

        AddTitleWithColumn(printOut, "DESCRIPTION","DESCRIPTION");

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
	    printOut->PrintFormat->Line->FontInfo.Reset();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TRANSACTION DATE";
	    printOut->PrintFormat->Line->Columns[1]->Text = TransactionDate;
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "MERCHANT CODE";
    	printOut->PrintFormat->Line->Columns[1]->Text = MerchantCode;
	    printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "MERCHANT NAME";
    	printOut->PrintFormat->Line->Columns[1]->Text = MerchantName;
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "OLD GRAND TOTAL";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(OldGrandTotal);
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "NEW GRAND TOTAL";
    	printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(NewGrandTotal);
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "DAILY SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(DailySales);
       	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "GRAND TOTAL DISCOUNT";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(GrandTotalDiscount);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-SENIORS";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountSCD);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-PWD";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountPWD);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-GPC";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountGPC);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-VIP";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountVIP);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-EMP";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountEMP);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-REG";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountREG);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL DISCOUNT-OTHERS";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalDiscountOTHERS);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL REFUND";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalRefund);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL CANCELED";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalCancel);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL VAT";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalVAT);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL VAT INCLUSIVE SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalVATIncSales);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL VAT EXCLUSIVE SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalVATExSales);
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "BEGINNING O.R";
    	printOut->PrintFormat->Line->Columns[1]->Text = BeginningOR;
	    printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "ENDING O.R";
    	printOut->PrintFormat->Line->Columns[1]->Text = EndingOR;
	    printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "DOCUMENT COUNT";
    	printOut->PrintFormat->Line->Columns[1]->Text = DocumentCount;
	    printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "CUSTOMER COUNT";
    	printOut->PrintFormat->Line->Columns[1]->Text = CustomerCount;
	    printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "SENIOR CITIZEN COUNT";
    	printOut->PrintFormat->Line->Columns[1]->Text = SCDCount;
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "LOCAL TAX";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(LocalTax);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "SERVICE CHARGE";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(ServiceCharge);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL NON-VAT SALE";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalNonVatSale);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "RAW GROSS";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(RawGross);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "DAILY LOCAL TAX";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(DailyLocalTax);
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "WORKSTATION NUMBER";
    	printOut->PrintFormat->Line->Columns[1]->Text = WorkStationNumber;
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL PAYMENT-CASH";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalCash);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL PAYMENT-CARD";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalCard);
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL PAYMENT-OTHERS";
	    printOut->PrintFormat->Line->Columns[1]->Text = FixDecimalPlaces(TotalOthers);
    	printOut->PrintFormat->AddLine();
    	printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	    printOut->PrintFormat->Line->ColCount = 1;
    	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	    printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
    	printOut->PrintFormat->Line->Columns[0]->Text = "";
	    printOut->PrintFormat->Line->Columns[0]->Line();
    	printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Reset();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
	    printOut->PrintFormat->Line->FontInfo.Reset();

    	printOut->PrintFormat->Line->Columns[0]->Text = "Prepared By:";
    	printOut->PrintFormat->AddLine();
    	printOut->PrintFormat->Line->Columns[0]->Text = "Checked By:";
    	printOut->PrintFormat->AddLine();
    	printOut->PrintFormat->Line->Columns[0]->Text = "Approved By:";
    	printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	    printOut->PrintFormat->Line->ColCount = 1;
    	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	    printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
    	printOut->PrintFormat->Line->Columns[0]->Text = "";
	    printOut->PrintFormat->Line->Columns[0]->Line();
    	printOut->PrintFormat->AddLine();
    }
}

void MallExportConsolidatedReceipt::AddTitleWithColumn(TPrintout* printOut, AnsiString title1, AnsiString title2)
{
	printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	printOut->PrintFormat->Line->ColCount = 1;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
	printOut->PrintFormat->Line->Columns[0]->Text = "";
	printOut->PrintFormat->Line->Columns[0]->Line();
    printOut->PrintFormat->SolidLineChar = '-';
    printOut->PrintFormat->AddLine();

	printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
    printOut->PrintFormat->Line->FontInfo.Bold = true;
	printOut->PrintFormat->Line->ColCount = 2;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width * 2 / 3;
	printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;
	printOut->PrintFormat->Line->Columns[1]->Alignment = taRightJustify;
	printOut->PrintFormat->Line->Columns[0]->Text = title1;
	printOut->PrintFormat->Line->Columns[1]->Text = title2;
	printOut->PrintFormat->AddLine();

    printOut->PrintFormat->Line->FontInfo.Height = fsDoubleSize;
	printOut->PrintFormat->Line->ColCount = 1;
	printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
	printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
	printOut->PrintFormat->Line->Columns[0]->Text = "";
	printOut->PrintFormat->Line->Columns[0]->Line();
	printOut->PrintFormat->AddLine();
}

int MallExportConsolidatedReceipt::GetSalesDetails()
{
    int result = 0;
    TTransactionInfo TransactionInfo;
    AnsiString DeviceName = TDeviceRealTerminal::Instance().ID.Name;

    TransactionInfo = TTransactionInfoProcessor::Instance().GetTransactionInfo(*_dbTransaction, DeviceName);


    TFinancialDetails FinancialDetails =  reportFinancialCalculations->GetFinancialDetails(*_dbTransaction,TransactionInfo,DeviceName);

    bool showTaxAndServiceCharge = _globalSettings->RevenueFiguresAreTaxAndServiceChargeInclusive;

    TCategoryGroups::iterator itCatGroups;
    for (itCatGroups = FinancialDetails.TotalSales.Details.begin(); itCatGroups != FinancialDetails.TotalSales.Details.end();
    itCatGroups++)
    {
        TCategoryGroupDetails CategoryGroupDetails = itCatGroups->second;
        TCategoryTotals::iterator itCategoryTotals;
        for (itCategoryTotals = CategoryGroupDetails.Details.begin(); itCategoryTotals != CategoryGroupDetails.Details.end();
        itCategoryTotals++)
        {
//            result = dataFormatUtilities->FormatMMReportCurrency( itCategoryTotals->second.Totals.Qty );
            result = itCategoryTotals->second.Totals.Qty;
        }
    }

    return result;
}

UnicodeString MallExportConsolidatedReceipt::FixDecimalPlaces(Currency AmountTotal)
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
#pragma package(smart_init)
