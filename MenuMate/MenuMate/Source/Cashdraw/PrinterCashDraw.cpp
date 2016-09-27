//---------------------------------------------------------------------------


#pragma hdrstop

#include "PrinterCashDraw.h"

#include "DeviceRealControl.h"
#include "MM_DBCore.h"
#include "MMLogging.h"
#include "ManagerPhysicalPrinter.h"
#include "VirtualPrinterManager.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
// TPrinterCashDraw
//---------------------------------------------------------------------------

/**
 * Open the cash draw using the selected printer.
 */
void TPrinterCashDraw::OpenDraw()
{
    TPrinterPhysical Printer;
    if(LoadPrinter(Printer))
    {
        TPrintout *Printout = new TPrintout;
        Printout->Printer = Printer;
        try
        {
            makeLogFile("Opening cash drawer Class - TPrinterCashDraw");
            SendOpenDrawCommands(Printout);
        }
        __finally
        {
            delete Printout;
        }
    }
}

//---------------------------------------------------------------------------
// TCashDrawReceiptPrinter
//---------------------------------------------------------------------------

/**
 * Create a controller for a cash draw attached to the receipt printer.
 *
 * @param inReceiptPrinter The receipt printer to be used to open the cash draw.
 */
TCashDrawReceiptPrinter::TCashDrawReceiptPrinter(TPrinterPhysical& inReceiptPrinter)
{
    ReceiptPrinter = inReceiptPrinter;
}

/**
 * Load the receipt printer.
 *
 * @param Printer Reference variable to load the receipt printer into.
 */
bool TCashDrawReceiptPrinter::LoadPrinter(TPrinterPhysical& Printer)
{
    bool receiptPrinterSelected = ReceiptPrinter.PhysicalPrinterKey != 0;
    if(receiptPrinterSelected)
    {
        Printer = ReceiptPrinter;
    }
    else
    {
        TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"Error Opening Cash Drawer. No Receipt Printer Selected");
    }
    return receiptPrinterSelected;
}

/**
 * Send print instruction to the receipt printer to open the attached cash draw.
 *
 * @param Printout Receipt printer printout to send command to.
 */
void TCashDrawReceiptPrinter::SendOpenDrawCommands(TPrintout* Printout)
{
    Printout->OpenDrawer();
}

//---------------------------------------------------------------------------
// TCashDrawCashDrawPrinter
//---------------------------------------------------------------------------

/**
 * Create a controller for a cash draw using a virtual printer.
 *
 * @param inPortLength
 */
TCashDrawCashDrawPrinter::TCashDrawCashDrawPrinter(int inPortLength)
{
    PortLength = inPortLength;
}

/**
 * Load the cash draw printer.
 *
 * @param Printer Reference variable to load the cash draw printer into.
 */
bool TCashDrawCashDrawPrinter::LoadPrinter(TPrinterPhysical& Printer)
{
    Database::TDBTransaction& dbTransaction = Database::TDBTransaction(TDeviceRealControl::ActiveInstance().DBControl);
    TPrinterVirtual *PrinterVirtual = TManagerVirtualPrinter::GetVirtualPrinterByName(dbTransaction, "Cashdrawer Printer");
    bool receiptPrinterSelected = PrinterVirtual != NULL;
    if(receiptPrinterSelected)
    {
        TManagerPhysicalPrinter ManagerPhysicalPrinter;
        TPrinterPhysical PhysicalPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(dbTransaction, PrinterVirtual->PhysicalPrinterKey);
        receiptPrinterSelected = PhysicalPrinter.PhysicalPrinterKey != 0;
        if(receiptPrinterSelected)
        {
            Printer = PhysicalPrinter;
        }
        else
        {
            TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Unable to Locate 'Cashdrawer Printer' as a Physical windows printer");
        }
    }
    else
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Unable to Locate 'Cashdrawer Printer' as a Virtual printer");
    }
    return  receiptPrinterSelected;
}

/**
 * Format and send print instuctions to cash draw printer to open the cash draw.
 *
 * @param Printout Cash draw printer printout to send command to.
 */
void TCashDrawCashDrawPrinter::SendOpenDrawCommands(TPrintout* Printout)
{
    Printout->PrintFormat->EmulationMode			   = Printout->Printer.EmulationMode;
    Printout->PrintFormat->NormalWidth				   = Printout->Printer.NormalCharPerLine;
    Printout->PrintFormat->BoldWidth				   = Printout->Printer.BoldCharPerLine;
    Printout->PrintFormat->CutterGap				   = Printout->Printer.CutterGap;
    Printout->PrintFormat->TopMargin				   = Printout->Printer.TopMargin;
    Printout->PrintFormat->AlwaysUseFullCut			   = Printout->Printer.AlwaysUseFullCut;
    Printout->PrintFormat->KanjiPrinter				   = Printout->Printer.KanjiPrinter;

    Printout->PrintFormat->Line->FontInfo.Height	   = fsDoubleSize;
    Printout->PrintFormat->Line->ColCount			   = 1;
    Printout->PrintFormat->Line->Columns[0]->Width	   = Printout->PrintFormat->Width;
    Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
    Printout->PrintFormat->Line->Columns[0]->Text	   = "Open Drawer Open Drawer Open Drawer";
    for (int i = 0; i < PortLength ; i++)
    {
        Printout->PrintFormat->AddLine();
        Printout->PrintFormat->AddLine();
        Printout->PrintFormat->AddLine();
        Printout->PrintFormat->AddLine();
        Printout->PrintFormat->AddLine();
    }
    Printout->Print();
}

//---------------------------------------------------------------------------
void TPrinterCashDraw::makeLogFile(UnicodeString str)
{
    AnsiString fileName = ExtractFilePath(Application->ExeName) + "EFTPOSCashDrawer_Logs.txt" ;
    TStrings * List = new TStringList();
    if (FileExists(fileName) )
    {
      List->LoadFromFile(fileName);
    }
    List->Add(" "+ str +  "\n");
    List->SaveToFile(fileName );
}
