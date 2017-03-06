#include "XReport.h"
#include "Comms.h"
#include "PrinterPhysical.h"
#include "ShowPrintout.h"

XReport::XReport() : EndOfDayReport()
{
	_closeTill = false;
}

AnsiString XReport::GetReportName()
{
    return "X Report";
}

int XReport::DisplayAndPrint(TMemoryStream* memoryStream)
{
    try
    {
        TPrintout* printOut = SetupPrintOutInstance();

        if(!TGlobalSettings::Instance().UseBIRFormatInXZReport)
        {
            printOut->PrintFormat->Line->FontInfo.Height = fsNormalSize;
            printOut->PrintFormat->Line->ColCount = 1;
            printOut->PrintFormat->Line->Columns[0]->Width = printOut->PrintFormat->Width;
            printOut->PrintFormat->Line->Columns[0]->Alignment = taCenter;
            printOut->PrintFormat->Add("");
            printOut->PrintFormat->Add("Preliminary (X) Till Not Closed Off");
            printOut->PrintFormat->PartialCut();
        }

        TForm* currentForm = Screen->ActiveForm;
        std::auto_ptr <TfrmShowPrintout> (frmShowPrintout)(TfrmShowPrintout::Create <TfrmShowPrintout> (currentForm));
        printOut->PrintToStream(frmShowPrintout->CurrentPrintout.get());

        frmShowPrintout->btnCancel->Caption = "Close";
        frmShowPrintout->btnClose->Visible = false;
        frmShowPrintout->btnClosePrint->Caption = "Print";

        frmShowPrintout->Execute();

        if(memoryStream)
        {
            printOut->PrintToStream(memoryStream);
        }

        return frmShowPrintout->ExitCode;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }
}

void XReport::PopulateXReportForEJournal(TMemoryStream* memoryStream)
{
    TPrintout* printOut = SetupPrintOutInstance();
    if(memoryStream)
    {
        printOut->PrintToStream(memoryStream);
    }
}
