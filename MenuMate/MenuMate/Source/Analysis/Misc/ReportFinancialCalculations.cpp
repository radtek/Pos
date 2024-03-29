//---------------------------------------------------------------------------
#pragma hdrstop
#include "ReportFinancialCalculations.h"
#include "DeviceRealTerminal.h"
#include "MMLogging.h"
#include "ManagerReports.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ReportFinancialCalculations::ReportFinancialCalculations()
{
}

ReportFinancialCalculations::~ReportFinancialCalculations()
{
}

TFinancialDetails ReportFinancialCalculations::GetFinancialDetails(Database::TDBTransaction &DBTransaction, TTransactionInfo &TransactionInfo,AnsiString DeviceName)
{
    TFinancialDetails financialDetails;

    GetBilledSalesDetail(DBTransaction, financialDetails, DeviceName);
    GetSavedSalesDetail(DBTransaction, financialDetails, DeviceName);
    GetBilledSalesQuantity(DBTransaction, financialDetails, DeviceName);
    GetSavedSalesQuantity(DBTransaction, financialDetails, DeviceName);
    GetLoyaltySalesDetail(DBTransaction, TransactionInfo, DeviceName);

    return financialDetails;
}

Currency ReportFinancialCalculations::GetTotalSalesTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName)
{
    Currency salesTax;
    TIBSQL *salesTaxQuery = DBTransaction.Query(DBTransaction.AddQuery());

    salesTaxQuery->SQL->Text = "SELECT "
                                    "SUM(TAX_VALUE) AS TAXSUM "
                                "FROM DAYARCORDERTAXES DAOT "
                                "INNER JOIN DAYARCHIVE DA ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                                "INNER JOIN DAYARCBILL DAB ON DA.ARCBILL_KEY = DAB.ARCBILL_KEY "
                                "WHERE TAX_TYPE = '0' AND DAB.TERMINAL_NAME = :Terminal_Name";

    salesTaxQuery->ParamByName("Terminal_Name")->AsString = deviceName;

    salesTaxQuery->ExecQuery();
    salesTax = salesTaxQuery->FieldByName("TAXSUM")->AsCurrency;
    salesTaxQuery->Close();

    return salesTax;
}

Currency ReportFinancialCalculations::GetTaxExemptSales(Database::TDBTransaction &DBTransaction)
{
	Currency TaxExemptTotal;
	Currency calctaxexempt;
	Currency servicecharge;
    Currency quantity;

    TIBSQL *qr = DBTransaction.Query(DBTransaction.AddQuery());
    TIBSQL *qr1 = DBTransaction.Query(DBTransaction.AddQuery());

    qr->SQL->Text = "SELECT DA.ARCHIVE_KEY, DA.PRICE, DA.QTY "
                    "FROM DAYARCHIVE DA "
                    "LEFT JOIN DAYARCORDERTAXES DAOT ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                    "WHERE DA.ARCHIVE_KEY NOT IN ( "
                        "SELECT ARCHIVE_KEY "
                        "FROM DAYARCORDERTAXES "
                        "WHERE (TAX_TYPE = 0 OR TAX_TYPE = 5) and TAX_VALUE <>0 "
                    ") "
                    "GROUP BY 1,2,3 ";
	qr->ExecQuery();

    while(!qr->Eof)
    {
        calctaxexempt = qr->FieldByName("PRICE")->AsCurrency;
        quantity = qr->FieldByName("QTY")->AsCurrency;
        TaxExemptTotal += calctaxexempt * quantity;

        // Get the ARCHIVE key for all tax exempt items
        qr1->SQL->Text = "SELECT TAX_VALUE "
                         "FROM DAYARCORDERTAXES "
                         "WHERE TAX_TYPE IN ('2','4') AND ARCHIVE_KEY=:ARCKEY ";

        qr1->ParamByName("ARCKEY")->AsInteger = qr->FieldByName("ARCHIVE_KEY")->AsInteger;
        qr1->ExecQuery();
        while(!qr1->Eof)
        {
            servicecharge += qr1->FieldByName("TAX_VALUE")->AsCurrency;
            qr1->Next();
        }
        qr1->Close();
        qr->Next();
    }

	qr->Close();

    if(TaxExemptTotal != 0) {
        TaxExemptTotal = TaxExemptTotal - servicecharge;
    }

    return TaxExemptTotal;
}

Currency ReportFinancialCalculations::GetServiceCharge(Database::TDBTransaction &DBTransaction, AnsiString deviceName)
{
    Currency servicecharge;
    TIBSQL *qr = DBTransaction.Query(DBTransaction.AddQuery());
    qr->SQL->Text = "SELECT "
                                    "SUM(TAX_VALUE) AS TAXSUM "
                                "FROM DAYARCORDERTAXES DAOT "
                                "INNER JOIN DAYARCHIVE DA ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                                "INNER JOIN DAYARCBILL DAB ON DA.ARCBILL_KEY = DAB.ARCBILL_KEY "
                                "WHERE TAX_TYPE = '2' AND DAB.TERMINAL_NAME = :Terminal_Name";

    qr->ParamByName("Terminal_Name")->AsString = deviceName;

    qr->ExecQuery();
    servicecharge = qr->FieldByName("TAXSUM")->AsCurrency;
    qr->Close();

    return servicecharge;
}

Currency ReportFinancialCalculations::GetServiceChargeTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName)
{
    Currency servicechargetax;
    TIBSQL *qr = DBTransaction.Query(DBTransaction.AddQuery());
    qr->SQL->Text = "SELECT "
                                    "SUM(TAX_VALUE) AS TAXSUM "
                                "FROM DAYARCORDERTAXES DAOT "
                                "INNER JOIN DAYARCHIVE DA ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                                "INNER JOIN DAYARCBILL DAB ON DA.ARCBILL_KEY = DAB.ARCBILL_KEY "
                                "WHERE TAX_TYPE = '3' AND DAB.TERMINAL_NAME = :Terminal_Name";

    qr->ParamByName("Terminal_Name")->AsString = deviceName;
    qr->ExecQuery();
    servicechargetax = qr->FieldByName("TAXSUM")->AsCurrency;

    qr->Close();
    return servicechargetax;
}

Currency ReportFinancialCalculations::GetLocalTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName)
{
    Currency localtax;
    TIBSQL *qr = DBTransaction.Query(DBTransaction.AddQuery());
    qr->SQL->Text = "SELECT "
                                    "SUM(TAX_VALUE) AS TAXSUM "
                                "FROM DAYARCORDERTAXES DAOT "
                                "INNER JOIN DAYARCHIVE DA ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                                "INNER JOIN DAYARCBILL DAB ON DA.ARCBILL_KEY = DAB.ARCBILL_KEY "
                                "WHERE TAX_TYPE = '4' AND DAB.TERMINAL_NAME = :Terminal_Name";

    qr->ParamByName("Terminal_Name")->AsString = deviceName;

    qr->ExecQuery();
    localtax = qr->FieldByName("TAXSUM")->AsCurrency;

    qr->Close();
    return localtax;
}

Currency ReportFinancialCalculations::GetProfitTax(Database::TDBTransaction &DBTransaction, AnsiString deviceName)
{
    Currency localtax;
    TIBSQL *qr = DBTransaction.Query(DBTransaction.AddQuery());
    qr->SQL->Text = "SELECT "
                                    "SUM(TAX_VALUE) AS TAXSUM "
                                "FROM DAYARCORDERTAXES DAOT "
                                "INNER JOIN DAYARCHIVE DA ON DAOT.ARCHIVE_KEY = DA.ARCHIVE_KEY "
                                "INNER JOIN DAYARCBILL DAB ON DA.ARCBILL_KEY = DAB.ARCBILL_KEY "
                                "WHERE TAX_TYPE = '5' AND DAB.TERMINAL_NAME = :Terminal_Name";

    qr->ParamByName("Terminal_Name")->AsString = deviceName;

    qr->ExecQuery();
    localtax = qr->FieldByName("TAXSUM")->AsCurrency;

    qr->Close();
    return localtax;
}

Currency ReportFinancialCalculations::GetDiscountsAndSurcharges(Database::TDBTransaction &DBTransaction)
{
	Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
	TIBSQL *qr = tr.Query(tr.AddQuery());
	Currency discountsandsurcharges;

    qr->SQL->Text = "SELECT SUM(DAD.DISCOUNTED_VALUE) AS DISCSUM "
                    "FROM DAYARCORDERDISCOUNTS DAD "
                    "JOIN DAYARCORDERTAXES DAT ON DAD.ARCHIVE_KEY = DAT.ARCHIVE_KEY "
                    "WHERE DAT.TAX_TYPE = '0' AND DAT.TAX_VALUE > 0";

	tr.StartTransaction();

	qr->ExecQuery();

    discountsandsurcharges = qr->FieldByName("DISCSUM")->AsCurrency;

	tr.Commit();
	qr->Close();

	return discountsandsurcharges;
}

void ReportFinancialCalculations::GetBilledSalesDetail(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName)
{
    AnsiString TerminalCondition = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
      {
         TerminalCondition = " DAYARCHIVE.TERMINAL_NAME = '" + DeviceName + "' AND ";
      }
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT CATEGORYGROUPS.NAME,CATEGORYGROUPS.CATEGORYGROUPS_KEY,"
			" ARCCATEGORIES.CATEGORY,ARCCATEGORIES.CATEGORY_KEY, DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME, "
			" SUM (SALES_TAX.TAX_TOTAL) SALES_TAX_TOTAL,"
			" SUM (SC_TAX.TAX_TOTAL) SC_TAX_TOTAL,"
			" SUM (SC.SC_TOTAL) SC_TOTAL,"
			" SUM (LOCAL_TAX.LOCAL_TAX_TOTAL) LOCAL_TAX_TOTAL,"
            " SUM (PROFIT_TAX.PROFIT_TAX_TOTAL) PROFIT_TAX_TOTAL,";
            if(TGlobalSettings::Instance().RevenueFiguresAreDiscountInclusive)
            {
                    IBInternalQuery->SQL->Text =IBInternalQuery->SQL->Text+
                    " SUM ((DAYARCHIVE.PRICE * DAYARCHIVE.QTY))  TOTAL,"
                     " SUM ((abs(DAYARCHIVE.BASE_PRICE) * DAYARCHIVE.QTY)+(DAYARCHIVE.DISCOUNT_WITHOUT_TAX)) RAWTOTAL,";
            }
            else
            {
                    IBInternalQuery->SQL->Text =IBInternalQuery->SQL->Text+
                    " SUM ((DAYARCHIVE.PRICE * DAYARCHIVE.QTY)-(DAYARCHIVE.DISCOUNT))  TOTAL,"
                    " SUM (abs(DAYARCHIVE.BASE_PRICE) * DAYARCHIVE.QTY) RAWTOTAL,";
            }
             IBInternalQuery->SQL->Text =IBInternalQuery->SQL->Text+

			" SUM (DAYARCHIVE.COST * DAYARCHIVE.QTY) COST,"
			" SUM (DAYARCHIVE.QTY) TOTALQTY"
            " FROM CATEGORYGROUPS"
			" INNER JOIN ARCCATEGORIES ON CATEGORYGROUPS.CATEGORYGROUPS_KEY = ARCCATEGORIES.CATEGORYGROUPS_KEY"
			" INNER JOIN DAYARCHIVE ON ARCCATEGORIES.CATEGORY_KEY = DAYARCHIVE.CATEGORY_KEY"
			" LEFT JOIN DAYARCBILL ON DAYARCHIVE.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY"
			" LEFT JOIN  (SELECT DAYARCORDERDISCOUNTS.ARCHIVE_KEY, "
			" MIN(CASE WHEN DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME = 'Complimentary' THEN coalesce(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME,0) END) AS DISCOUNT_GROUPNAME  "
			" FROM DAYARCORDERDISCOUNTS  WHERE (COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME, 0)= 'Complimentary') group by  DAYARCORDERDISCOUNTS.ARCHIVE_KEY )  DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY   "
	        " LEFT JOIN (SELECT ARCHIVE_KEY, SUM(TAX_VALUE) TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 0 GROUP BY ARCHIVE_KEY) SALES_TAX ON DAYARCHIVE.ARCHIVE_KEY = SALES_TAX.ARCHIVE_KEY"
			" LEFT JOIN (SELECT ARCHIVE_KEY, SUM(TAX_VALUE) TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 3 GROUP BY ARCHIVE_KEY) SC_TAX ON DAYARCHIVE.ARCHIVE_KEY = SC_TAX.ARCHIVE_KEY"
			" LEFT JOIN (SELECT ARCHIVE_KEY, SUM(TAX_VALUE) SC_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 2 GROUP BY ARCHIVE_KEY) SC ON DAYARCHIVE.ARCHIVE_KEY = SC.ARCHIVE_KEY"
			" LEFT JOIN (SELECT ARCHIVE_KEY, SUM(TAX_VALUE) LOCAL_TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 4 GROUP BY ARCHIVE_KEY) LOCAL_TAX ON DAYARCHIVE.ARCHIVE_KEY = LOCAL_TAX.ARCHIVE_KEY"
            " LEFT JOIN (SELECT ARCHIVE_KEY, SUM(TAX_VALUE) PROFIT_TAX_TOTAL FROM DAYARCORDERTAXES WHERE TAX_TYPE = 5 GROUP BY ARCHIVE_KEY) PROFIT_TAX ON DAYARCHIVE.ARCHIVE_KEY = PROFIT_TAX.ARCHIVE_KEY"
			" WHERE (COALESCE(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME, 0)<> 'Non-Chargeable') AND "	+ TerminalCondition +
			" ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + ") "
			" GROUP BY CATEGORYGROUPS.NAME,CATEGORYGROUPS.CATEGORYGROUPS_KEY, ARCCATEGORIES.CATEGORY, ARCCATEGORIES.CATEGORY_KEY,DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME ";
    IBInternalQuery->ExecQuery();
    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	{
                // Retrive Grand Totals.
                TCategoryGroupDetails TotalSalesCategoryGroup = FinancialDetails.TotalSales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
                TCatTotal TotalSalesDetailTotals = TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];
                // Retrive Category Group Totals.
                TCategoryGroupDetails CategoryGroup = FinancialDetails.BilledSales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
                if(IBInternalQuery->FieldByName("DISCOUNT_GROUPNAME")->AsString == "Complimentary")
                {
                        //TODO:
                        CategoryGroup = FinancialDetails.ComplimentarySales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
                }
                TCatTotal DetailTotals = CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];

                DetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                DetailTotals.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                DetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                DetailTotals.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                DetailTotals.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                DetailTotals.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                DetailTotals.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                DetailTotals.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                DetailTotals.Totals.TaxContent = DetailTotals.Totals.SalesTaxContent + DetailTotals.Totals.ServiceChargeTaxContent;
                DetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;

                CategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                CategoryGroup.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                CategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                CategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
                CategoryGroup.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                CategoryGroup.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                CategoryGroup.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                CategoryGroup.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                CategoryGroup.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                CategoryGroup.Totals.TaxContent = CategoryGroup.Totals.SalesTaxContent + CategoryGroup.Totals.ServiceChargeTaxContent;

                TotalSalesDetailTotals.Category_Key = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
                TotalSalesDetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                TotalSalesDetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
                TotalSalesDetailTotals.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                TotalSalesDetailTotals.Totals.TaxContent = TotalSalesDetailTotals.Totals.SalesTaxContent + TotalSalesDetailTotals.Totals.ServiceChargeTaxContent;

                TotalSalesCategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                TotalSalesCategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
                TotalSalesCategoryGroup.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                TotalSalesCategoryGroup.Totals.TaxContent = TotalSalesCategoryGroup.Totals.SalesTaxContent + TotalSalesCategoryGroup.Totals.ServiceChargeTaxContent;
                TotalSalesCategoryGroup.Category_Group_Name = IBInternalQuery->FieldByName("NAME")->AsString;
                TotalSalesCategoryGroup.Category_Group_Key = IBInternalQuery->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;

                if(IBInternalQuery->FieldByName("DISCOUNT_GROUPNAME")->AsString == "Complimentary")
                {
                        FinancialDetails.ComplimentarySales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
                        FinancialDetails.ComplimentarySales.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.ComplimentarySales.Totals.TaxContent = FinancialDetails.BilledSales.Totals.SalesTaxContent + FinancialDetails.BilledSales.Totals.ServiceChargeTaxContent;
                }
                else
                {
                        FinancialDetails.BilledSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
                        FinancialDetails.BilledSales.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                        FinancialDetails.BilledSales.Totals.TaxContent = FinancialDetails.BilledSales.Totals.SalesTaxContent + FinancialDetails.BilledSales.Totals.ServiceChargeTaxContent;
                }

                FinancialDetails.TotalSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
                FinancialDetails.TotalSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
                FinancialDetails.TotalSales.Totals.SalesTaxContent += IBInternalQuery->FieldByName("SALES_TAX_TOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.ServiceChargeTaxContent += IBInternalQuery->FieldByName("SC_TAX_TOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.ServiceChargeContent += IBInternalQuery->FieldByName("SC_TOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.LocalTaxContent += IBInternalQuery->FieldByName("LOCAL_TAX_TOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.ProfitTaxContent += IBInternalQuery->FieldByName("PROFIT_TAX_TOTAL")->AsCurrency;
                FinancialDetails.TotalSales.Totals.TaxContent = FinancialDetails.TotalSales.Totals.SalesTaxContent + FinancialDetails.TotalSales.Totals.ServiceChargeTaxContent;

                // Store Category Group Totals.
                //TODO:
                CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = DetailTotals;
                if(IBInternalQuery->FieldByName("DISCOUNT_GROUPNAME")->AsString == "Complimentary")
                {
                        //TODO:
                    FinancialDetails.ComplimentarySales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = CategoryGroup;
                }
                else
                {
                        //TODO:
                     FinancialDetails.BilledSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = CategoryGroup;
                }
                // Store Grand Totals.
                //TODO:
                TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = TotalSalesDetailTotals;
                //TODO:
                FinancialDetails.TotalSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = TotalSalesCategoryGroup;
      }
    IBInternalQuery->Close();
}

void ReportFinancialCalculations::GetSavedSalesDetail(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName)
{
    AnsiString TerminalCondition = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        TerminalCondition = " TERMINAL_NAME = '" + DeviceName + "' AND ";
    }

    TDateTime PrevZedTime = GetPreviousZedTime(DBTransaction);
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

    IBInternalQuery->SQL->Text = "SELECT " " CATEGORYGROUPS.NAME," " ARCCATEGORIES.CATEGORY,";
                                 if(TGlobalSettings::Instance().RevenueFiguresAreDiscountInclusive)
                                 {
                                        IBInternalQuery->SQL->Text =IBInternalQuery->SQL->Text+
                                        " SUM ((ORDERS.ZED_PRICE * ORDERS.QTY)) TOTAL,"
                                        " SUM ((ORDERS.BASE_PRICE * ORDERS.QTY)+(ORDERS.DISCOUNT_WITHOUT_TAX))RAWTOTAL, ";
                                 }
                                 else
                                 {
                                        IBInternalQuery->SQL->Text =IBInternalQuery->SQL->Text+
                                        " SUM ((ORDERS.ZED_PRICE * ORDERS.QTY)-(ORDERS.DISCOUNT)) TOTAL, "
                                        " SUM ((ORDERS.BASE_PRICE * ORDERS.QTY))RAWTOTAL, ";
			                     }
                                  IBInternalQuery->SQL->Text =IBInternalQuery->SQL->Text+
                                 " SUM (ORDERS.COST * ORDERS.QTY) COST,"
                                 " SUM (ORDERS.QTY) TOTALQTY,"
                                 " 0.00 AS TAXTOTAL"
                                 " FROM" " CATEGORYGROUPS" " INNER JOIN ARCCATEGORIES ON CATEGORYGROUPS.CATEGORYGROUPS_KEY = ARCCATEGORIES.CATEGORYGROUPS_KEY"
                                 " INNER JOIN ORDERS ON ARCCATEGORIES.CATEGORY_KEY = ORDERS.CATEGORY_KEY"
                                 " WHERE " + TerminalCondition +
                                 " ORDERS.TIME_STAMP > :TIME_STAMP" " AND ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr
                                 (CreditNonExistingOrder) + ") " " GROUP BY CATEGORYGROUPS.NAME, ARCCATEGORIES.CATEGORY";
    IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = PrevZedTime;

    IBInternalQuery->ExecQuery();

    for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
    {
        // Retrive Grand Totals.
        TCategoryGroupDetails TotalSalesCategoryGroup = FinancialDetails.TotalSales.Details
        [IBInternalQuery->FieldByName("NAME")->AsString];
        TCatTotal TotalSalesDetailTotals = TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];
        // Retrive Category Group Totals.
        TCategoryGroupDetails CategoryGroup = FinancialDetails.SavedSales.Details[IBInternalQuery->FieldByName("NAME")->AsString];
        TCatTotal DetailTotals = CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString];

        DetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
        DetailTotals.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
        DetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
        DetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
        DetailTotals.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

        CategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
        CategoryGroup.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
        CategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
        CategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
        CategoryGroup.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;


        TotalSalesDetailTotals.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
        TotalSalesDetailTotals.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
        TotalSalesDetailTotals.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
        TotalSalesDetailTotals.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
        TotalSalesDetailTotals.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

        TotalSalesCategoryGroup.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
        TotalSalesCategoryGroup.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
        TotalSalesCategoryGroup.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
        TotalSalesCategoryGroup.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
        TotalSalesCategoryGroup.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

        FinancialDetails.SavedSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
        FinancialDetails.SavedSales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
        FinancialDetails.SavedSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
        FinancialDetails.SavedSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
        FinancialDetails.SavedSales.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;

        FinancialDetails.TotalSales.Totals.Total += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
        FinancialDetails.TotalSales.Totals.RawTotal += IBInternalQuery->FieldByName("RAWTOTAL")->AsCurrency;
        FinancialDetails.TotalSales.Totals.Cost += IBInternalQuery->FieldByName("COST")->AsCurrency;
        FinancialDetails.TotalSales.Totals.Qty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;
        //TODO:
        FinancialDetails.TotalSales.Totals.TaxContent += IBInternalQuery->FieldByName("TAXTOTAL")->AsCurrency;
        // Store Category Group Totals.
        //TODO:
        CategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = DetailTotals;
        //TODO:
        FinancialDetails.SavedSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = CategoryGroup;
        // Store Grand Totals.
        //TODO:
        TotalSalesCategoryGroup.Details[IBInternalQuery->FieldByName("CATEGORY")->AsString] = TotalSalesDetailTotals;
        //TODO:
        FinancialDetails.TotalSales.Details[IBInternalQuery->FieldByName("NAME")->AsString] = TotalSalesCategoryGroup;
    }
    IBInternalQuery->Close();
}

TDateTime ReportFinancialCalculations::GetPreviousZedTime(Database::TDBTransaction &DBTransaction)
{
    DataCalculationUtilities dataCalcUtils;
    return dataCalcUtils.GetPreviousZedTimeForTerminal(
                                                DBTransaction,
                                                TDeviceRealTerminal::Instance().ID.Name);
}

void ReportFinancialCalculations::GetBilledSalesQuantity(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName)
{
    AnsiString DayArcbillCondition = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
      {
	DayArcbillCondition = " DAYARCBILL.TERMINAL_NAME = '" + DeviceName + "' AND ";
      }
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "SELECT COUNT (DISTINCT DAYARCBILL.ARCBILL_KEY) QTYTOTAL " "FROM DAYARCBILL "
    "LEFT JOIN DAYARCHIVE ON DAYARCHIVE.ARCBILL_KEY = DAYARCBILL.ARCBILL_KEY "
    " WHERE " + DayArcbillCondition +
    " ( DAYARCHIVE.ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    DAYARCHIVE.ORDER_TYPE = " + IntToStr
    (CreditNonExistingOrder) + ") ";
    IBInternalQuery->ExecQuery();
    if (IBInternalQuery->RecordCount != 0)
    {
            FinancialDetails.BilledSales.Totals.Qty = IBInternalQuery->FieldByName("QTYTOTAL")->AsInteger;
    }
}

void ReportFinancialCalculations::GetSavedSalesQuantity(Database::TDBTransaction &DBTransaction,TFinancialDetails &FinancialDetails,AnsiString DeviceName)
{
    AnsiString TerminalCondition = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
      {
         TerminalCondition = " TERMINAL_NAME = '" + DeviceName + "' AND ";
      }
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text =
    "select count(distinct(time_stamp)) QTY  from orders "
    " WHERE " + TerminalCondition +
    " ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR ORDER_TYPE = " + IntToStr
    (CreditNonExistingOrder) + ") ";
    IBInternalQuery->ExecQuery();
    if (IBInternalQuery->RecordCount != 0)
    {
            FinancialDetails.SavedSales.Totals.Qty = IBInternalQuery->FieldByName("QTY")->AsInteger;
    }
    FinancialDetails.TotalSales.Totals.Qty = FinancialDetails.SavedSales.Totals.Qty + FinancialDetails.BilledSales.Totals.Qty;
}

void ReportFinancialCalculations::GetLoyaltySalesDetail(Database::TDBTransaction &DBTransaction,TTransactionInfo &TransactionInfo,AnsiString DeviceName)
{
    AnsiString TerminalCondition = "";
    if(!TGlobalSettings::Instance().EnableDepositBagNum)
      {
         TerminalCondition = " TERMINAL_NAME = '" + DeviceName + "' AND ";
      }
    TDateTime PrevZedTime = GetPreviousZedTime(DBTransaction);
    Currency LoyaltyTotal = 0;
    float LoyaltyTotalQty = 0;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "select " " SUM (DAYARCHIVE.PRICE) TOTAL,"
    " SUM (DAYARCHIVE.COST * DAYARCHIVE.QTY) COST," " SUM (DAYARCHIVE.QTY) TOTALQTY " " FROM " "DAYARCHIVE "
    " WHERE " + TerminalCondition +
    " LOYALTY_KEY != 0 " " AND LOYALTY_KEY IS NOT NULL " " AND ( ORDER_TYPE = " + IntToStr(NormalOrder)
    + " " " OR    ORDER_TYPE = " + IntToStr(CreditNonExistingOrder) + ") ";
    IBInternalQuery->ExecQuery();

    LoyaltyTotal += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
    LoyaltyTotalQty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;

    IBInternalQuery->Close();
    IBInternalQuery->SQL->Text = "select " " SUM (ORDERS.ZED_PRICE) TOTAL,"
    " SUM (ORDERS.COST *ORDERS.QTY) COST," " SUM (ORDERS.QTY) TOTALQTY " " FROM ORDERS "
    " WHERE " + TerminalCondition +
    " LOYALTY_KEY != 0 " " AND LOYALTY_KEY IS NOT NULL "
    " AND ORDERS.TIME_STAMP > :TIME_STAMP" " AND ( ORDER_TYPE = " + IntToStr(NormalOrder) + " " " OR    ORDER_TYPE = " + IntToStr
    (CreditNonExistingOrder) + ") ";
    IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = PrevZedTime;
    IBInternalQuery->ExecQuery();

    LoyaltyTotal += IBInternalQuery->FieldByName("TOTAL")->AsCurrency;
    LoyaltyTotalQty += IBInternalQuery->FieldByName("TOTALQTY")->AsFloat;

    TCalculatedTotals LoyaltySalesTotal(etcTotalLoyaltySales, LoyaltyTotal,0,0, LoyaltyTotalQty);
    TransactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalLoyaltySales]] = LoyaltySalesTotal;
    IBInternalQuery->Close();
}

void ReportFinancialCalculations::GetSummaSalesDetail(Database::TDBTransaction &DBTransaction,TTransactionInfo &TransactionInfo,AnsiString DeviceName)
{
   //Summa Gross Sales.
    TCalculatedTotals TotalRawSales(etcTotalRawSales, 0,0,0,0);
    GetSummaGrossSalesTotal(DBTransaction,DeviceName,TotalRawSales);
    TransactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalRawSales]] = TotalRawSales;

    //Summa Net sales.
    TCalculatedTotals TotalGrossSales(etcTotalGrossSales, 0,0,0,0);
    GetSummaNetSalesTotal(DBTransaction,DeviceName,TotalGrossSales);
    TransactionInfo.CalculatedTotals[eStrCalculatedTotals[etcTotalGrossSales]] = TotalGrossSales;
}

void ReportFinancialCalculations::GetSummaGrossSalesTotal(Database::TDBTransaction &DBTransaction,AnsiString DeviceName, TCalculatedTotals &BaseSales)
{ // Sales Excluding Discounts
	try
	{
		// The Total Including Discounts.
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM (DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) GROSS_TOTAL FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' "
		" AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal = IBInternalQuery->FieldByName("GROSS_TOTAL")->AsCurrency;
		}

		// The Total Qty.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT COUNT(DAYARCBILL.ARCBILL_KEY) FROM DAYARCBILL where DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Qty = IBInternalQuery->FieldByName("COUNT")->AsCurrency;
		}

		// Add Surcharges to the total.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT SUM (DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) DISCOUNT_TOTAL "
		"FROM DAYARCHIVE "
		"INNER JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"WHERE DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and "
		" DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0 AND DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal += IBInternalQuery->FieldByName("DISCOUNT_TOTAL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
                                            "SUM((CAST((CAST(((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY ) AS NUMERIC(17, 4)) * 100) AS NUMERIC(17, 4)) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_EXCL "
                                        "FROM DAYARCHIVE "
                                        "LEFT JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
                                        "WHERE DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME AND (DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' OR DAYARCHIVE.DISCOUNT = 0)";

        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total = IBInternalQuery->FieldByName("TOTAL_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
                                            "SUM(CAST((CAST(((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) AS NUMERIC(17, 4)) / (DAYARCHIVE.GST_PERCENT + 100.0)) AS NUMERIC(17, 4))) TOTAL_SURCHARGE_EXCL "
                                        "FROM DAYARCORDERDISCOUNTS "
                                        "INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
                                        "WHERE  DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME AND "
                                        "((DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' AND DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0) OR DAYARCHIVE.DISCOUNT = 0)";

        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total += IBInternalQuery->FieldByName("TOTAL_SURCHARGE_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		/*IBInternalQuery->SQL->Text = "SELECT "
		" SUM( ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) - ((((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_TAX "
		" FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'   "
		" or DAYARCHIVE.DISCOUNT = 0 and DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";*/

        IBInternalQuery->SQL->Text = "SELECT "
                                            "SUM(((CAST(DAYARCHIVE.PRICE AS NUMERIC(17, 4)) - CAST(DAYARCHIVE.DISCOUNT AS NUMERIC(17, 4))) * CAST(DAYARCHIVE.QTY AS NUMERIC(17, 4))) - "
                                                 "(CAST(DAYARCORDERTAXES.TAX_VALUE AS NUMERIC(17, 4)))) TOTAL_TAX "
                                        "FROM DAYARCHIVE "
                                        "LEFT JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
                                        "LEFT JOIN DAYARCORDERTAXES ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERTAXES.ARCHIVE_KEY AND DAYARCORDERTAXES.TAX_TYPE = 0 "
                                        "WHERE DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME AND "
                                        "(DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' OR DAYARCHIVE.DISCOUNT = 0)";

        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent = IBInternalQuery->FieldByName("TOTAL_TAX")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE -  ((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0) ) TOTAL_SURCHARGE_TAX "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		" WHERE (( DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' and  "
		" DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE > 0) "
		"or DAYARCHIVE.DISCOUNT = 0) AND DAYARCHIVE.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent += IBInternalQuery->FieldByName("TOTAL_SURCHARGE_TAX")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}

void ReportFinancialCalculations::GetSummaNetSalesTotal(Database::TDBTransaction &DBTransaction,AnsiString DeviceName,TCalculatedTotals &BaseSales)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT SUM ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) GROSS_TOTAL FROM DAYARCHIVE  "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal = IBInternalQuery->FieldByName("GROSS_TOTAL")->AsCurrency;
		}

		// The Total Qty.
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT COUNT(DAYARCBILL.ARCBILL_KEY) FROM DAYARCBILL where DAYARCBILL.TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Qty = IBInternalQuery->FieldByName("COUNT")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT SUM (DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) TOTAL_ADJUSTMENTS "
		"FROM DAYARCHIVE "
		"INNER JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.RawTotal += IBInternalQuery->FieldByName("TOTAL_ADJUSTMENTS")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
                                            "SUM((CAST((((CAST(DAYARCHIVE.PRICE AS NUMERIC(17, 4)) - CAST(DAYARCHIVE.DISCOUNT AS NUMERIC(17, 4))) * CAST(DAYARCHIVE.QTY AS NUMERIC(17, 4))) * 100) AS NUMERIC(17, 4)) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_EXCL "
                                        "FROM DAYARCHIVE "
                                        "LEFT JOIN DAYARCORDERDISCOUNTS ON DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY "
                                        "WHERE TERMINAL_NAME = :TERMINAL_NAME AND "
                                        "DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable' OR DAYARCHIVE.DISCOUNT = 0";

        IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total = IBInternalQuery->FieldByName("TOTAL_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( (((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_ADJUSTMENTS_EXCL "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.Total += IBInternalQuery->FieldByName("TOTAL_ADJUSTMENTS_EXCL")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( ((DAYARCHIVE.PRICE - DAYARCHIVE.DISCOUNT) * DAYARCHIVE.QTY) - (((DAYARCHIVE.PRICE * DAYARCHIVE.QTY) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0)) ) TOTAL_TAX "
		" FROM DAYARCHIVE "
		"left join DAYARCORDERDISCOUNTS on  "
		"DAYARCHIVE.ARCHIVE_KEY = DAYARCORDERDISCOUNTS.ARCHIVE_KEY  "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent = IBInternalQuery->FieldByName("TOTAL_TAX")->AsCurrency;
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT "
		" SUM( DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE -  ((DAYARCORDERDISCOUNTS.DISCOUNTED_VALUE) * 100) / (DAYARCHIVE.GST_PERCENT + 100.0) ) TOTAL_ADJUSTMENTS_TAX "
		" FROM DAYARCORDERDISCOUNTS "
		" INNER JOIN DAYARCHIVE ON DAYARCORDERDISCOUNTS.ARCHIVE_KEY = DAYARCHIVE.ARCHIVE_KEY "
		"where DAYARCORDERDISCOUNTS.DISCOUNT_GROUPNAME <> 'Non-Chargeable'  "
		" or DAYARCHIVE.DISCOUNT = 0 AND "
		"TERMINAL_NAME = :TERMINAL_NAME";
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = DeviceName;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount > 0)
		{
			BaseSales.TaxContent += IBInternalQuery->FieldByName("TOTAL_ADJUSTMENTS_TAX")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
		throw;
	}
}
