//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerReportExport.h"
#include "MallExportRegenerateReport.h"
#include "MallExportOutputDBDriver.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TManagerReportExport::ExportReport()
{
    std::auto_ptr<TfrmMallExportRegenerateReport> MallExportRegenerateReport(TfrmMallExportRegenerateReport::Create<TfrmMallExportRegenerateReport>(Screen->ActiveForm));
    MallExportRegenerateReport->btnGenerate->Visible = false;
    MallExportRegenerateReport->btnLoadPath->Visible = false;
    MallExportRegenerateReport->sbAllTerminals->Visible = true;
    MallExportRegenerateReport->sbThisTerminal->Visible = true;
    MallExportRegenerateReport->btnOk->Caption = "Generate Report";
    MallExportRegenerateReport->Caption = "Generate Report";
    TModalResult result = MallExportRegenerateReport->ShowModal();
    TDateTime SDate = MallExportRegenerateReport->SDate;
    TDateTime EDate = MallExportRegenerateReport->EDate;
    isAllTerminalSelected = MallExportRegenerateReport->isAllTerminalsSelected;
    UnicodeString reportExportPath = MallExportRegenerateReport->edLocationPath->Text;
    if(result == mrOk)
    {
        CreateFileExportPath(reportExportPath);
        PrepareDataForCSVFile(SDate, EDate, reportExportPath);
    }
}
//---------------------------------------------------------------------------------------------------------
void TManagerReportExport::CreateFileExportPath(UnicodeString &reportExportPath)
{
    UnicodeString Format = ".csv";
     //Check if directory not exist than create it.
    if (!DirectoryExists(reportExportPath))
    {
        CreateDir(reportExportPath);
    }

    //CSV will be created with Following Name
    reportExportPath = reportExportPath + Now().FormatString("ddmmyyyyhhnnss") + Format;
}
//--------------------------------------------------------------------------------------------------------
void TManagerReportExport::PrepareDataForCSVFile(TDateTime SDate, TDateTime EDate, UnicodeString reportExportPath)
{
    //Register the database transaction..
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        std::vector<UnicodeString> dataToWrite;
        THavanaReport* report = new THavanaReport();
        dataToWrite =  report->CreateHeaderFormat(dbTransaction, SDate, EDate, isAllTerminalSelected);

        //Use Federal Mall's Function of printing Header
        TMallExportOutputDBDriver  exportCSV;
        exportCSV.GenerateTXT(dataToWrite, reportExportPath, "ZTXTHEADER");

        //Clear DataToWrite Vector
        dataToWrite.erase (dataToWrite.begin(),dataToWrite.end());

        //Load all data into vector
        dataToWrite =  report->LoadDataFromDB(dbTransaction, SDate, EDate, isAllTerminalSelected);

        //calling existing mall function for insert Data into CSV
        exportCSV.GenerateTXT(dataToWrite, reportExportPath, "ZTXTDATA");

        //Commit the transaction as we have completed all the transactions
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
