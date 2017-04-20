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
    try
    {
        TPrintout* printOut = SetupPrintOutInstance();
        if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
           printOut->PrintFormat->PartialCut();
        }

        TForm* currentForm = Screen->ActiveForm;
        if(printOut->ContinuePrinting)
        {
            std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (currentForm));

            printOut->PrintToStream(frmShowPrintout->CurrentPrintout.get());

            if (TGlobalSettings::Instance().EnableBlindBalances)
            {
                frmShowPrintout->btnCancel->Visible = false;
                TBorderIcons tempBI = frmShowPrintout->BorderIcons;
                tempBI >> biSystemMenu;
                frmShowPrintout->BorderIcons = tempBI;
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
    }
    catch (Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return retValue;
}