//---------------------------------------------------------------------------


#pragma hdrstop

#include "WaiterAppZedReport.h"
#include "GlobalSettings.h"
#include "Comms.h"
#include "PrinterPhysical.h"
#include "ShowPrintout.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



WaiterAppZedReport::WaiterAppZedReport() : EndOfDayReport()
{
	_closeTill = true;
}

AnsiString WaiterAppZedReport::GetReportName()
{
    return "Zed Report";
}

int WaiterAppZedReport::DisplayAndPrint(TMemoryStream* memoryStream)
{
    int retValue = 1;
    TPrintout* printOut = SetupPrintOutInstance();
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