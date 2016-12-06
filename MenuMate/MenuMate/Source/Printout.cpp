// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Printout.h"
#include "ManagerPhysicalPrinter.h"
#include "MMLogging.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TPrintout::TPrintout()
{
   PrintFormat = new TPrintFormat;
   PrintedSuccessfully = false;
   Status = poPending;
   BlindBalanceUsed = true;
   CashDenominationUsed = true;
}

__fastcall TPrintout::~TPrintout()
{
   delete PrintFormat;
}

void __fastcall TPrintout::SetPrinter(TPrinterPhysical Printer)
{
   FPrinter = Printer;
   PrintFormat->EmulationMode = Printer.EmulationMode;
   PrintFormat->NormalWidth = Printer.NormalCharPerLine;
   PrintFormat->BoldWidth = Printer.BoldCharPerLine;
   PrintFormat->CutterGap = Printer.CutterGap;
   PrintFormat->TopMargin = Printer.KitchenTopMargin;
   PrintFormat->AlwaysUseFullCut = Printer.AlwaysUseFullCut;
   PrintFormat->KanjiPrinter = Printer.KanjiPrinter;
   PrintFormat->PrinterCodePage = Printer.PrinterCodePage;

   PrintFormat->PrinterInitCmd = Printer.PrinterInitCmd;
   PrintFormat->PrinterCodePageOn = Printer.PrinterCodePageOn;
   PrintFormat->PrinterCodePageOff = Printer.PrinterCodePageOff;

   PrintFormat->SolidLineChar = Printer.SingleLineChar;
   PrintFormat->DoubleLineChar = Printer.DoubleLineChar;
   PrintFormat->GraphicPrinterSettings = Printer.Graphics;
}

bool TPrintout::PrintToFile(UnicodeString FileName)
{
   PrintFormat->GetText()->SaveToFile(FileName);
   return true;
}

bool TPrintout::PrintToStream(TStream *Stream)
{
   PrintFormat->GetText()->SaveToStream(Stream);
   return true;
}

// ---------------------------------------------------------------------------
bool TPrintout::OpenDrawer()
{
   try
   {
	  if (Printer.PhysicalPrinterKey == 0)
	  {
		 Status = poNoPhysicalPrinter;
	  }
	  if (Status == poPending && Printer.Type == ptWindows_Printer)
	  {
         if (PrintFormat->OpenDrawer(Printer.UNCName()))
         {
            Status = poSuccess;
         }
         else
         {
            Status = poFailed;
         }
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing to Failed " + Err.Message);
	  Status = poFailed;
   }
   return(Status == poSuccess);
}
// ---------------------------------------------------------------------------

bool TPrintout::Print(UnicodeString JobName)
{
   try
   {
	  if (Printer.PhysicalPrinterKey == 0)
	  {
		 Status = poNoPhysicalPrinter;
	  }

	  if (Status == poPending)
	  {
		 if (Printer.Type == ptWindows_Printer)
		 {
            if (PrintFormat->Print(Printer.UNCName(), JobName))
            {
               Status = poSuccess;
            }
            else
            {
               Status = poFailed;
            }
		 }
		 else
		 {
			Status = poSuccess;
		 }
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing to Failed " + Err.Message);
	  Status = poFailed;
   }
   return(Status == poSuccess);
}

bool TPrintout::PrintToPrinterRaw(TStringList *Text, UnicodeString PrinterName)
{
   return PrintFormat->Print(PrinterName, "", Text->Text);
}

// ---------------------------------------------------------------------------
bool TPrintout::PrintToPrinterStream(TMemoryStream *Data, UnicodeString PrinterName)
{
   Data->Position = 0;
   const char* ptr = (const char*)Data->Memory;
   AnsiString StreamData(ptr, (unsigned int)(Data->Size));
   return PrintFormat->Print(PrinterName, "", StreamData);
}

bool TPrintout::PrintToStrings(TStrings *Strings)
{
   if (Strings)
   {
	  Strings->AddStrings(PrintFormat->GetText());
   }
   return true;
}
// ---------------------------------------------------------------------------

