#include "ConsolidatedZedReport.h"
#include "GlobalSettings.h"
#include "Comms.h"
#include "PrinterPhysical.h"
#include "ShowPrintout.h"

ConsolidatedZedReport::ConsolidatedZedReport() : EndOfDayReport()
{
	_closeTill = true;
}

AnsiString ConsolidatedZedReport::GetReportName()
{
    return "Zed Report";
}

int ConsolidatedZedReport::DisplayAndPrint(TMemoryStream* memoryStream)
{
    int retValue = 0;
    TPrintout* printOut = SetupPrintOutInstanceForConsolidatedZed();
    if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
    {
       printOut->PrintFormat->PartialCut();
    }

    TForm* currentForm = Screen->ActiveForm;
    if(printOut->ContinuePrinting)
    {
        if(memoryStream)
        {
            printOut->PrintToStream(memoryStream);
        }
        SkipZedProcess = false;
    }
    else
    {
        SkipZedProcess = true;
    }
    return retValue;

}