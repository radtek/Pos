#include "XServiceChargeSummaryDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "SecurityReference.h"
#include "ReportUtilities.h"

XServiceChargeSummaryDetailsReportSection::XServiceChargeSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmServiceChargeSummaryDetailsSection, dbTransaction, globalSettings)
{
    dataFormatUtilities = new DataFormatUtilities;
}

XServiceChargeSummaryDetailsReportSection::XServiceChargeSummaryDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmXReport, mmServiceChargeSummaryDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
    dataFormatUtilities = new DataFormatUtilities;
}


XServiceChargeSummaryDetailsReportSection::~XServiceChargeSummaryDetailsReportSection()
{
    delete dataFormatUtilities;
    delete _memberShip;
}

void XServiceChargeSummaryDetailsReportSection::GetOutput(TPrintout* printOut)
{
    try
    {
        AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
        Currency servcharge = 0;
        Currency servchargetax = 0;
        if(IsConsolidatedZed)
        {
            servcharge = reportCalculations->GetServiceCharge(*_dbTransaction, deviceName, *_startTime, *_endTime);
            servchargetax = reportCalculations->GetServiceChargeTax(*_dbTransaction, deviceName, *_startTime, *_endTime);
        }
        else
        {
            servcharge = reportCalculations->GetServiceCharge(*_dbTransaction, deviceName);
            servchargetax = reportCalculations->GetServiceChargeTax(*_dbTransaction, deviceName);
        }



        if (_globalSettings->ShowServiceChargeTaxWithServiceCharge && !_globalSettings->ShowServiceChargeTaxWithSalesTax)
        {
            servcharge = servcharge + servchargetax;
        }

        AddTitle(printOut, "Service Charge Summary");
        printOut->PrintFormat->NewLine();
        SetupPrintFormat(printOut->PrintFormat);

        printOut->PrintFormat->Line->Columns[0]->Text = "Service Charge Total:";
        printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(servcharge);
        printOut->PrintFormat->AddLine();

        if (!_globalSettings->ShowServiceChargeTaxWithServiceCharge && !_globalSettings->ShowServiceChargeTaxWithSalesTax)
        {
            printOut->PrintFormat->Line->Columns[0]->Text = "Service Charge Tax Total:";
            printOut->PrintFormat->Line->Columns[1]->Text = dataFormatUtilities->FormatMMReportCurrency(servchargetax);
            printOut->PrintFormat->AddLine();
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
void XServiceChargeSummaryDetailsReportSection::SetupPrintFormat(TPrintFormat*pf)
{
    pf->Line->ColCount = 2;
    pf->Line->Columns[0]->Alignment = taLeftJustify;
    pf->Line->Columns[1]->Alignment = taRightJustify;
    pf->Line->Columns[0]->Width = pf->Width * 2 / 3;
    pf->Line->Columns[1]->Width = pf->Width * 1 / 3;
    pf->Line->FontInfo.Reset();
}