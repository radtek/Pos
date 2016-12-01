#include "XTaxSummaryDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"


XTaxSummaryDetailsReportSection::XTaxSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmTaxSummaryDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
    dataCalculationUtilities = new DataCalculationUtilities;
    reportCalculations = new ReportFinancialCalculations;
}


XTaxSummaryDetailsReportSection::~XTaxSummaryDetailsReportSection()
{
    delete dataFormatUtilities;
    delete dataCalculationUtilities;
    delete reportCalculations;
}

void XTaxSummaryDetailsReportSection::GetOutput(TPrintout* printOut)
{
    if(!printOut->BlindBalanceUsed)
        return;
    AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
    const Currency todays_earnings = dataCalculationUtilities->GetTotalEarnings(*_dbTransaction, deviceName);
    Currency taxExemptSales = 0;
    Currency salesTax = reportCalculations->GetTotalSalesTax(*_dbTransaction, deviceName);
    Currency serviceCharge = reportCalculations->GetServiceCharge(*_dbTransaction, deviceName);
    Currency serviceChargeTax = reportCalculations->GetServiceChargeTax(*_dbTransaction, deviceName);
    Currency localTax = reportCalculations->GetLocalTax(*_dbTransaction, deviceName);
    Currency profitTax = reportCalculations->GetProfitTax(*_dbTransaction, deviceName);
    Currency discountAndSurcharge = reportCalculations->GetDiscountsAndSurcharges(*_dbTransaction);
    Currency zeroratedsales = reportCalculations->GetZeroRatedSales(*_dbTransaction, deviceName);
    Currency totaldiscount = reportCalculations->GetTotalDiscountValue(*_dbTransaction, deviceName);



    taxExemptSales = RoundToNearest(reportCalculations->GetTaxExemptSales(*_dbTransaction, deviceName), 0.01, _globalSettings->MidPointRoundsDown);

    if(_globalSettings->ReCalculateTaxPostDiscount)
    {
        discountAndSurcharge = 0;
    }
    const Currency taxSales = 0;

    if(_globalSettings->UseBIRFormatInXZReport)
    {

        taxSales = (((todays_earnings - discountAndSurcharge) - (taxExemptSales -zeroratedsales)  - serviceCharge - serviceChargeTax) - (salesTax + localTax + profitTax)) - zeroratedsales;
    }
    else
    {
       taxSales = (((todays_earnings - discountAndSurcharge) - taxExemptSales - serviceCharge - serviceChargeTax) - (salesTax + localTax + profitTax));
    }


    if(_globalSettings->ShowServiceChargeTaxWithSalesTax && !_globalSettings->ShowServiceChargeTaxWithServiceCharge)
    {
        salesTax += serviceChargeTax;
    }

    if(_globalSettings->UseBIRFormatInXZReport)
    {
        sales_tax.clear();
        GetDifferentTotalSalesTax(*_dbTransaction, deviceName);
        dataCalculationUtilities->PrinterFormatinTwoSections(printOut); //SetPrinterFormatToMiddle(printOut);

        printOut->PrintFormat->Line->Columns[0]->Text = "";
        printOut->PrintFormat->Line->Columns[1]->Text = "";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "VATable Sales";
        printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(taxSales);
        printOut->PrintFormat->AddLine();

        if(sales_tax.size() > 0)
        {
            for (std::vector<TSalesTax>::iterator it = sales_tax.begin(); it != sales_tax.end(); it++)
            {
                printOut->PrintFormat->Line->Columns[1]->Text = FloatToStr((it->Rate)) + "% VAT";
                if(it->TaxSum < 0)
                {
                   printOut->PrintFormat->Line->Columns[2]->Text = "(" + dataFormatUtilities->FormatMMReportCurrency(abs(it->TaxSum)) + ")";
                }
                else
                {
                    printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(it->TaxSum);//salesTax);
                }
                printOut->PrintFormat->AddLine();
            }
        }
        else
        {
            printOut->PrintFormat->Line->Columns[1]->Text = FloatToStr(0) + "% VAT";
            printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(0.00);
            printOut->PrintFormat->AddLine();
        }
        printOut->PrintFormat->Line->Columns[1]->Text = "VAT Exempt Sales";
        if((taxExemptSales - zeroratedsales) < 0)
        {
           printOut->PrintFormat->Line->Columns[2]->Text ="(" + dataFormatUtilities->FormatMMReportCurrency(fabs(taxExemptSales - zeroratedsales)) + ")";
        }
        else
        {
            printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(taxExemptSales - zeroratedsales);
        }
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Zero-Rated Sales";
        printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(zeroratedsales);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[1]->Text = "Total Discount";
        if(totaldiscount < 0)
        {
           printOut->PrintFormat->Line->Columns[2]->Text = "(" +dataFormatUtilities->FormatMMReportCurrency(fabs(totaldiscount)) + ")";
        }
        else
        {
            printOut->PrintFormat->Line->Columns[2]->Text = dataFormatUtilities->FormatMMReportCurrency(totaldiscount);
        }
        printOut->PrintFormat->AddLine();
        printOut->PrintFormat->Line->Columns[1]->Text = "";
        printOut->PrintFormat->Line->Columns[2]->Text = "";
        printOut->PrintFormat->AddLine();
    }
    else
    {
        AddTitle(printOut, "Tax Summary");
        printOut->PrintFormat->NewLine();

        IReportSectionDisplayTraits* reportSectionDisplayTraits = GetTextFormatDisplayTrait();

        if(reportSectionDisplayTraits)
        {
            reportSectionDisplayTraits->ApplyTraits(printOut);
        }

        printOut->PrintFormat->Line->Columns[1]->Width = printOut->PrintFormat->Width * 1 / 3;
        printOut->PrintFormat->Line->FontInfo.Reset();


        printOut->PrintFormat->Line->Columns[0]->Text = "Sales Tax Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(salesTax);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Tax Exempt Sales Total";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(taxExemptSales);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Taxable Sales Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(taxSales);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Local Tax Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(localTax);
        printOut->PrintFormat->AddLine();

        printOut->PrintFormat->Line->Columns[0]->Text = "Profit Tax Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(profitTax);
        printOut->PrintFormat->AddLine();

    }
}

void XTaxSummaryDetailsReportSection::GetDifferentTotalSalesTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName)
{
    TIBSQL *salesTaxQuery = DBTransaction.Query(DBTransaction.AddQuery());

    salesTaxQuery->SQL->Text = "SELECT "
                                    "SUM(TAX_VALUE) AS TAXSUM, DTax.RATE "
                                "FROM DAYARCORDERTAXES DAOT "
                                "INNER JOIN DAYARCHIVE DA ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                                "INNER JOIN DAYARCBILL DAB ON DA.ARCBILL_KEY = DAB.ARCBILL_KEY "
                                "INNER JOIN TAXPROFILES DTax on DTax.TYPE = DAOT.TAX_TYPE "
                                "WHERE TAX_TYPE = '0' AND DAB.TERMINAL_NAME = :Terminal_Name and DAOT.TAX_NAME = DTax.NAME "
                                "group by "
                                "DTax.RATE, "
                                "DAOT.TAX_TYPE ";

    salesTaxQuery->ParamByName("Terminal_Name")->AsString = deviceName;

    salesTaxQuery->ExecQuery();
    while(!salesTaxQuery->Eof)
    {
        TSalesTax sales;
        sales.Rate = salesTaxQuery->FieldByName("RATE")->AsDouble;
        sales.TaxSum = salesTaxQuery->FieldByName("TAXSUM")->AsCurrency;
        sales_tax.push_back(sales);
        salesTaxQuery->Next();
    }
    salesTaxQuery->Close();
}


