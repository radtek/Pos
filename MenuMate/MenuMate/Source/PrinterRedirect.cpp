//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

#include "PrinterRedirect.h"
#include "Comms.h"
#include "MMMessageBox.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmPrinterRedirect::TfrmPrinterRedirect(TComponent* Owner,Database::TDBTransaction &inDBTransaction)
	: TZForm(Owner) , DBTransaction(inDBTransaction)
{
	ShowAll = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterRedirect::FormShow(TObject *Sender)
{
	FormResize(Sender);
	GetPrinterList();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterRedirect::FormResize(TObject *Sender)
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
	Left				= (Screen->Width - Width) / 2;
	Top				= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

void TfrmPrinterRedirect::GetPrinterList()
{
	lbPrinters->Items->Clear();
	TManagerPhysicalPrinter ManagerPhysicalPrinter;
	if(ShowAll)
	{
		ManagerPhysicalPrinter.DBGetPrinterList(DBTransaction,lbPrinters->Items);
	}
	else
	{
		ManagerPhysicalPrinter.DBGetValidPrinterList(DBTransaction,TDeviceRealTerminal::Instance().ID.DeviceKey,lbPrinters->Items);
	}
}

void __fastcall TfrmPrinterRedirect::tbCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterRedirect::btnOkClick(TObject *Sender)
{
	if(lbPrinters->ItemIndex > -1)
	{
		SelectedPrinterKey = ((int)(lbPrinters->Items->Objects[lbPrinters->ItemIndex]));
		ModalResult = mrOk;
	}
	else
	{
		MessageBox("You must select a printer, Or Cancel.", "Printing Error",MB_OK + MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
