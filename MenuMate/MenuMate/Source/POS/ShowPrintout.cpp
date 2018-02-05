// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "ShowPrintout.h"
#include "Comms.h"
#include "PrintFormat.h"
#include "MMMessageBox.h"
#include "Printout.h"
#include "ReportUtilities.h"

#define RECEIPT_DELIMITER "#####"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmShowPrintout::TfrmShowPrintout(TForm *Owner) : TZForm(Owner), CurrentPrintout(new TMemoryStream)
{
   ExitCode = 0;
}

__fastcall TfrmShowPrintout::~TfrmShowPrintout()
{
}

// ---------------------------------------------------------------------------
void TfrmShowPrintout::Execute()
{
   if (!PrintoutExist())
   {
	  MessageBox("There are no Z reports to print. To view old Z reports, view the files in the 'ZDir' Directory ", "Warning",
		 MB_ICONWARNING + MB_OK);
	  return;
   }
   else
   {
	  ShowPrintout();
	  ShowModal();
   }
}

// ---------------------------------------------------------------------------
void TfrmShowPrintout::ShowPrintout()
{
   TStringList *Lines = new TStringList;
   try
   {
	  CurrentPrintout->Position = 0;
	  AnsiString TrimmedLine;
	  char *Line = (char*)CurrentPrintout->Memory;
	  for (int j = 0; j < CurrentPrintout->Size; j++)
	  {
		 if (Line[j] == ESC)
		 {
			if (Line[j + 1] == PRINT_MODE)
			   j += 2;
			else if (Line[j + 1] == COLOUR_MODE)
			   j += 2;
			else if (Line[j + 1] == STAR_OPEN_DRAWER)
			   j += 4;
			else if (Line[j + 1] == STAR_CUT_MODE)
			   j += 2;
			else if (Line[j + 1] == STAR_UNDERLINE)
			   j += 2;
			else if (Line[j + 1] == STAR_FONT_NORMAL_HEIGHT)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_DOUBLE_HEIGHT)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_A)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_B)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_INTENSITY_NORMAL)
			   j += 1;
			else if (Line[j + 1] == STAR_FONT_INTENSITY_BOLD)
			   j += 1;
		 }
		 else if (Line[j] == STAR_FONT_NORMAL_WIDTH || Line[j] == STAR_FONT_DOUBLE_WIDTH)
		 {
			// Single Character command just step over it.
		 }
		 else if (Line[j] == GS)
		 {
			if (Line[j + 1] == CUT_MODE)
			   j += 2;
			else if (Line[j + 1] == PRINT_GRAPHIC)
			   j += 2;
		 }
		 else if (Line[j] == ::CR)
		 {
			if (Line[j + 1] == ::LF)
			   j += 1;
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		 }
		 else if (Line[j] == ::LF)
		 {
			if (Line[j + 1] == ::CR)
			   j += 1;
			Lines->Add(TrimmedLine);
			TrimmedLine = "";
		 }
		 else
		 {
			if (Line[j] == char(196))
			{
			   TrimmedLine += "-";
			}
			else if (Line[j] == char(205))
			{
			   TrimmedLine += "=";
			}
			else
			{
			   TrimmedLine += Line[j];
			}
		 }
	  }
	  if (TrimmedLine != "")
		 Lines->Add(TrimmedLine);

	  memReceipt->Lines->Assign(Lines);
   }
   __finally
   {
	  delete Lines;
   }
}

// ---------------------------------------------------------------------------

bool TfrmShowPrintout::PrintoutExist()
{
   return(CurrentPrintout->Size != 0);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmShowPrintout::btnCancelClick(TObject *Sender)
{
   ExitCode = 0;
   Close();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmShowPrintout::FormShow(TObject *Sender)
{
   FormResize(this);
   SetWindowPos(Handle, HWND_TOP, Left, Top, Width, Height, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmShowPrintout::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height < 1.4)
          {
	  	ScaleBy(Screen->Width, Temp);
          }
   }
   ClientWidth = Panel1->Width;
   ClientHeight = Panel1->Height;
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmShowPrintout::btnClosePrintClick(TObject *Sender)
{
   UnicodeString deviceName = TDeviceRealTerminal::Instance().ID.Name;
   TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);

   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
   {
	  MessageBox("Please select a receipt printer from Setup first.", "Print error", MB_OK + MB_ICONERROR);
   }
   else
   {
	  ExitCode = 2;
	  Close();

   }

}
// ---------------------------------------------------------------------------

void __fastcall TfrmShowPrintout::btnCloseClick(TObject *Sender)
{
   UnicodeString deviceName = TDeviceRealTerminal::Instance().ID.Name;
   TTransactionInfoProcessor::Instance().RemoveEntryFromMap(deviceName);
   ExitCode = 1;
   Close();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmShowPrintout::btnBillDownAutoRepeat(TObject *Sender)
{
   memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmShowPrintout::btnBillUpAutoRepeat(TObject *Sender)
{
   memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
// ---------------------------------------------------------------------------
