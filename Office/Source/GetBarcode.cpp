//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GetBarcode.h"
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmGetBarcode *frmGetBarcode;
//---------------------------------------------------------------------------
__fastcall TfrmGetBarcode::TfrmGetBarcode(TComponent* Owner)
	: TForm(Owner)
{
	MMBarcodeReader = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::FormShow(TObject *Sender)
{
	if (CurrentConnection.UseSerialBarcodeReader)
	{
		if (MMBarcodeReader == NULL)
		{
			try
			{
				MMBarcodeReader = new TMMBarcodeReader;
				MMBarcodeReader->Open(CurrentConnection.SerialBarcodeReaderPort);
			}
			catch (Exception &E)
			{
				MMBarcodeReader = NULL;
				Application->MessageBox(AnsiString("Error initializing barcode reader!\r\r" + E.Message).c_str(), "Error", MB_ICONERROR);
			}
		}

	}
	else
	{
		if (MMBarcodeReader)
		{
			MMBarcodeReader->Close();
			delete MMBarcodeReader;
			MMBarcodeReader = NULL;
		}
	}
	edBarcode->Enabled		= false;
	edBarcode->Text			= "";
	btnManual->Caption		= "Manual";
	btnManual->Kind			= bkCustom;
	btnManual->ModalResult	= mrNone;
	btnManual->Glyph->Assign(NULL);
	lbeTitle->Caption			= "Waiting for barcode...";
	Done							= false;
	tiSwipe->Enabled			= false;
	Swiping						= false;
	TempBarcode					= "";
	edDummy->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::FormHide(TObject *Sender)
{
	if (MMBarcodeReader)
	{
		MMBarcodeReader->Close();
		delete MMBarcodeReader;
		MMBarcodeReader = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::WMCardSwipe(TMessage& Message)
{
	Barcode = *((AnsiString *)Message.WParam);
	ModalResult	= mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::btnManualClick(TObject *Sender)
{
	if (edBarcode->Enabled)
	{
		if (edBarcode->Text == "")
		{
			edBarcode->Enabled		= false;
			lbeTitle->Caption			= "Waiting for barcode...";
			btnManual->Caption		= "Manual";
			btnManual->Kind			= bkCustom;
			btnManual->ModalResult	= mrNone;
			btnManual->Glyph->Assign(NULL);
		}
		else
		{
			Barcode		= edBarcode->Text;
			ModalResult	= mrOk;
		}
	}
	else
	{
		btnManual->Kind		= bkOK;
		lbeTitle->Caption		= "Enter barcode";
		edBarcode->Enabled	= true;
		edBarcode->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::FormKeyPress(TObject *Sender, char &Key)
{
	if (!edBarcode->Enabled)
	{
		if (isprint (Key))
		{
			TempBarcode += Key;
			Swiping = true;
			if (!tiSwipe->Enabled)
			{
				tiSwipe->Enabled = true;
			}
		}
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::tiSwipeTimer(TObject *Sender)
{
   if(Swiping == true)
   {
      Swiping = false;
	}
   else
   {
		Done			= true;
		Barcode		= TempBarcode;
		ModalResult	= mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGetBarcode::btnCancelClick(TObject *Sender)
{
	tiSwipe->Enabled	= false;
	Swiping				= false;
	TempBarcode			= "";
	Barcode				= "";
	Done					= true;
	ModalResult			= mrCancel;
}
//---------------------------------------------------------------------------

