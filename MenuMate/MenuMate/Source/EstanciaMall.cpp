//---------------------------------------------------------------------------


#pragma hdrstop
#include "EstanciaMall.h"
#include "MallExportData.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include <Math.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

TEstanciaMall::TEstanciaMall()
{
}
void TEstanciaMall::PrepareDataForDatabase(TPaymentTransaction &paymentTransaction, int arcBillKey)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    TMallExportData mallExportData;
    TMallExportSalesData salesData;
    int terminalNumber;
    std::list<TMallExportSettings>::iterator it;

    //01 load Tenant code Row
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;

    for(it = TGlobalSettings::Instance().mallInfo.MallSettings.begin(); it != TGlobalSettings::Instance().mallInfo.MallSettings.end(); it++)
    {
        if(it->ControlName == "edTenantNo1")
        {
            salesData.DataValue = it->Value;
        }
        else if(it->ControlName == "edTerminalNo1")
        {
            terminalNumber = StrToInt(it->Value);
        }
    }
    salesData.Field = "Tenant Code";
    salesData.DataValueType = "UnicodeString";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //02 Load pos Terminal Number Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = terminalNumber;
    salesData.Field = "POS Terminal Number";
    salesData.DataValueType = "Integer";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //03 Load pos Terminal Number Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
    salesData.Field = "Date (mmddyyyy)";
    salesData.DataValueType = "TDateTime";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //04 Load Transaction date Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
    salesData.Field = "Date (mmddyyyy)";
    salesData.DataValueType = "TDateTime";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //05 Load Old Accumulated Sale Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
    salesData.Field = "Old Accumulated Sales";
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

     //06 Total Gross Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = Now().FormatString("mmddyyyy");
    salesData.Field = "New Accumulated Sales";
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //07 Total Gross Amount Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal + fabs(paymentTransaction.Money.TotalAdjustment);
    salesData.Field = "Total Gross Amount";
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);

    //08 Total Deductions Row;
    salesData.MallExportSalesId = GenerateSaleKey(dbTransaction);
    salesData.MallKey = TGlobalSettings::Instance().mallInfo.MallId;
    salesData.DataValue = paymentTransaction.Money.RoundedGrandTotal + fabs(paymentTransaction.Money.TotalAdjustment);
    salesData.Field = "Total Gross Amount";
    salesData.DataValueType = "Currency";
    salesData.enumType = 10;
    salesData.DateCreated = Now();
    salesData.CreatedBy = TDeviceRealTerminal::Instance().User.Name;
    salesData.ArcBillKey = arcBillKey;
    //Now push salesdata to Mallexportdata 's list;
    mallExportData.SalesData.push_back(salesData);



}
//-----------------------------------------------------------------------------------------------
void TEstanciaMall::PrepareDataForExport()
{
///todo

}
//-----------------------------------------------------------------------------------------------
void TEstanciaMall::CreateExportMedium()
{
///todo
}
//----------------------------------------------------------------------------------------------
long TEstanciaMall::GenerateSaleKey(Database::TDBTransaction &dbTransaction)
{

    Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	dbTransaction.RegisterQuery(IBInternalQuery);
    long saleKey;
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_MALLEXPORT_SALE_KEY, 1) FROM RDB$DATABASE";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            saleKey = IBInternalQuery->Fields[0]->AsInteger;
    }
     catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return saleKey;
}


