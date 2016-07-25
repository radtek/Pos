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

//---------------------------------------------------------------------------

MallExportConsolidatedReceipt::MallExportConsolidatedReceipt(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	: BaseReportSection(mmZReport, mmMallExportConsolidatedReceipt, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
}

MallExportConsolidatedReceipt::~MallExportConsolidatedReceipt()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
}

void MallExportConsolidatedReceipt::GetOutput(TPrintout* printOut)
{
    if(TGlobalSettings::Instance().MallIndex == AYALAMALL)
    {
        TMallExportInputFileDriver InputManager;
        AnsiString Date = Now().FormatString("mm/dd/yyyy");
        UnicodeString GetQuery = "SELECT * FROM MALLEXPORT";
        std::map<UnicodeString, UnicodeString> DataRead;

        UnicodeString DailySales = "";
        UnicodeString VAT = "";
        UnicodeString VatableSales = "";
        UnicodeString NonVatableSales = "";
        UnicodeString SCDisc = "";
        UnicodeString VATExempt = "";
        UnicodeString ZeroRated = "";
        UnicodeString NetSales = "";
        UnicodeString TotalQty = GetSalesDetails();
        UnicodeString TransactionCount = "";

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
                    TransactionCount = (it->second);
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
	    printOut->PrintFormat->Line->Columns[1]->Text = DailySales;
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "VAT";
    	printOut->PrintFormat->Line->Columns[1]->Text = VAT;
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "VATABLE SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = VatableSales;
    	printOut->PrintFormat->AddLine();

	    printOut->PrintFormat->Line->Columns[0]->Text = "NON-VATABLE SALES";
    	printOut->PrintFormat->Line->Columns[1]->Text = NonVatableSales;
	    printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "LESS SC DISC";
	    printOut->PrintFormat->Line->Columns[1]->Text = SCDisc;
       	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "VAT EXEMPT";
	    printOut->PrintFormat->Line->Columns[1]->Text = VATExempt;
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "ZERO RATED";
	    printOut->PrintFormat->Line->Columns[1]->Text = ZeroRated;
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "NET SALES";
	    printOut->PrintFormat->Line->Columns[1]->Text = NetSales;
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TOTAL QTY. SOLD";
	    printOut->PrintFormat->Line->Columns[1]->Text = TotalQty;
    	printOut->PrintFormat->AddLine();

    	printOut->PrintFormat->Line->Columns[0]->Text = "TRANSACTION COUNT";
	    printOut->PrintFormat->Line->Columns[1]->Text = TransactionCount;
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

AnsiString MallExportConsolidatedReceipt::GetSalesDetails()
{
    UnicodeString result = "";
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
            result = dataFormatUtilities->FormatMMReportCurrency( itCategoryTotals->second.Totals.Qty );
        }
    }

    return result;
}


#pragma package(smart_init)
