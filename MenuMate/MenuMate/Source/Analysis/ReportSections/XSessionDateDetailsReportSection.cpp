#include "XSessionDateDetailsReportSection.h"
#include "DeviceRealTerminal.h"
#include "Analysis.h"

XSessionDateDetailsReportSection::XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings)
	:BaseReportSection(mmXReport, mmSessionDateDetailsSection, dbTransaction, globalSettings)
{
}

XSessionDateDetailsReportSection::XSessionDateDetailsReportSection(Database::TDBTransaction* dbTransaction, TGlobalSettings* globalSettings, TDateTime* startTime, TDateTime* endTime)
	:BaseReportSection(mmConsolidatedZReport, mmSessionDateDetailsSection, dbTransaction, globalSettings, startTime, endTime)
{
}


XSessionDateDetailsReportSection::~XSessionDateDetailsReportSection()
{
}

void XSessionDateDetailsReportSection::GetOutput(TPrintout* printOut)
{
   ShowSessionDate(printOut);
}

void XSessionDateDetailsReportSection::ShowSessionDate(TPrintout* printOut)
{
    try
    {
        if(IsConsolidatedZed)
        {
            AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
            printOut->PrintFormat->Line->Columns[0]->Text =deviceName ;
            printOut->PrintFormat->AddLine();
            const TMMContactInfo &staff_member = TfrmAnalysis::GetLastAuthenticatedUser();
            printOut->PrintFormat->Line->Columns[0]->Text =staff_member.Name;
            printOut->PrintFormat->AddLine();
        }
        else
        {
            IReportSectionDisplayStrategy* reportSectionDisplayStrategy = GetReportSectionStrategy();

            if (reportSectionDisplayStrategy)
            {
                //Call the strategy to build the section..
                reportSectionDisplayStrategy->BuildSection(printOut);
            }
            if(TGlobalSettings::Instance().ShowSessionDateInZed && !IsConsolidatedZed)
            {
                AnsiString deviceName = TDeviceRealTerminal::Instance().ID.Name;
                printOut->PrintFormat->Line->Columns[0]->Text =deviceName ;
                printOut->PrintFormat->AddLine();
                const TMMContactInfo &staff_member = TfrmAnalysis::GetLastAuthenticatedUser();
                printOut->PrintFormat->Line->Columns[0]->Text =staff_member.Name;
                printOut->PrintFormat->AddLine();
                DataCalculationUtilities* dataCalculationUtilities = new DataCalculationUtilities;
                int value = dataCalculationUtilities->GetZedNumber(*_dbTransaction);
                value += 1;
                printOut->PrintFormat->Line->Columns[0]->Text = "#" + IntToStr(value);
                printOut->PrintFormat->AddLine();
            }
        }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}
