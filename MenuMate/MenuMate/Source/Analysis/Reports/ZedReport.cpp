#include "ZedReport.h"
#include "GlobalSettings.h"
#include "Comms.h"
#include "PrinterPhysical.h"
#include "ShowPrintout.h"

ZedReport::ZedReport() : EndOfDayReport()
{
	_closeTill = true;
}

AnsiString ZedReport::GetReportName()
{
    return "Zed Report";
}

int ZedReport::DisplayAndPrint(TMemoryStream* memoryStream)
{
    int retValue = 0;
    TPrintout* printOut = SetupPrintOutInstance();

    printOut->PrintFormat->PartialCut();

    TForm* currentForm = Screen->ActiveForm;
    if(printOut->BlindBalanceUsed)
    {
        std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (currentForm));

        printOut->PrintToStream(frmShowPrintout->CurrentPrintout.get());

        if (TGlobalSettings::Instance().EnableBlindBalances)
        {
            frmShowPrintout->btnCancel->Visible = false;
        }
        else
        {
            frmShowPrintout->btnCancel->Visible = true;
            frmShowPrintout->btnCancel->Caption = "Cancel Zed";
        }
        frmShowPrintout->btnClose->Caption = "Close Till";
        frmShowPrintout->btnClosePrint->Caption = "Close Till and Print Zed";

        frmShowPrintout->Execute();

        if(memoryStream)
        {
            printOut->PrintToStream(memoryStream);
        }
        SkipZedProcess = false;
        retValue = frmShowPrintout->ExitCode;
    }
    else
    {
        SkipZedProcess = true;
    }
    return retValue;

}