#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

#include "VirtualPrintersInterface.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "PrinterRedirect.h"
#include "ManagerPhysicalPrinter.h"
#include "Devices.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmVirtualPrintersInterface *frmVirtualPrintersInterface;
//---------------------------------------------------------------------------
__fastcall TfrmVirtualPrintersInterface::TfrmVirtualPrintersInterface(TComponent* Owner,Database::TDBControl &inDBControl)
	: TZForm(Owner) , DBControl(inDBControl)
{
	Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfrmVirtualPrintersInterface::pnlAddClick(TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	AnsiString ButtonTitle = frmTouchKeyboard->KeyboardText;
	frmTouchKeyboard->MaxLength = 25;
	frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = true;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter Virtual Printer Name e.g Bar,Restaurant";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		int VPrinterKey = TManagerVirtualPrinter::DBCheckVirtualPrinterByName(DBTransaction,frmTouchKeyboard->KeyboardText);
		if(VPrinterKey == 0)
		{
			TPrinterVirtual PrinterVirtual;
			PrinterVirtual.VirtualPrinterName = frmTouchKeyboard->KeyboardText;
			if(GetNewPrinter("Select Printer",PrinterVirtual))
			{
				PrinterVirtual.VirtualPrinterKey = TManagerVirtualPrinter::DBAddPrinter(DBTransaction,PrinterVirtual);
			}
		}
		else
		{
			MessageBox("A Printer with this name already exits on this Till?", "Error",MB_ICONWARNING + MB_OK);
		}
		DBTransaction.Commit();
	}
	RefreshList();
}
//---------------------------------------------------------------------------
void __fastcall TfrmVirtualPrintersInterface::pnlEditClick(TObject *Sender)
{
	if(lbVirtualPrinters->ItemIndex >= 0)
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = 25;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = true;
		frmTouchKeyboard->KeyboardText = lbVirtualPrinters->Items->Strings[lbVirtualPrinters->ItemIndex];
		frmTouchKeyboard->Caption = "Rename this Virtual Printer";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			bool StartTrans = false;
			try
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				TManagerVirtualPrinter::RenamePrinter(DBTransaction,(int)(lbVirtualPrinters->Items->Objects[lbVirtualPrinters->ItemIndex]),frmTouchKeyboard->KeyboardText);
				DBTransaction.Commit();
			}
			catch(Exception &E)
			{
				TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
			}
		}
		RefreshList();
	}
	else
	{
		MessageBox("Please Select a Printer to Rename.", "Warning",	MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmVirtualPrintersInterface::pnlDeleteClick(TObject *Sender)
{
	if(lbVirtualPrinters->ItemIndex >= 0)
	{
		if(MessageBox("Are you sure wish to delete this Printer?", "Warning",
			MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
		{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				TManagerVirtualPrinter::DeletePrinter(DBTransaction,(int)(lbVirtualPrinters->Items->Objects[lbVirtualPrinters->ItemIndex]));
				DBTransaction.Commit();
		}
		RefreshList();
	}
	else
	{
		MessageBox("Please Select a Printer to Delete?", "Warning",	MB_OK);
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmVirtualPrintersInterface::FormShow(TObject *Sender)
{
	FormResize(this);
	RefreshList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmVirtualPrintersInterface::FormResize(TObject *Sender)
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
	ClientWidth = pnl2->Left + pnl2->Width + pnl1->Left;
	ClientHeight = pnl1->Height + (pnl1->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------
void TfrmVirtualPrintersInterface::RefreshList()
{
	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TManagerVirtualPrinter::InitialisePrinters(DBTransaction,TDeviceRealTerminal::Instance().ID.DeviceKey);
		TManagerVirtualPrinter::GetVirtualPrinterList(DBTransaction,lbVirtualPrinters->Items);
		DBTransaction.Commit();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void __fastcall TfrmVirtualPrintersInterface::btnOkClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

bool TfrmVirtualPrintersInterface::GetNewPrinter(AnsiString Message,TPrinterVirtual &PrinterVirtual)
{
	bool RetVal = false;
	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		std::auto_ptr<TfrmPrinterRedirect> frmPrinterRedirect( TfrmPrinterRedirect::Create<TfrmPrinterRedirect>(this,DBTransaction));
		frmPrinterRedirect->lbCaption->Caption = Message;
		frmPrinterRedirect->ShowAll = true;
		if(frmPrinterRedirect->ShowModal() == mrOk)
		{
			TManagerPhysicalPrinter ManagerPhysicalPrinter;
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,frmPrinterRedirect->SelectedPrinterKey);
			PrinterVirtual.PhysicalPrinterKey = PrinterPhysical.PhysicalPrinterKey;
			RetVal = true;
		}
		DBTransaction.Commit();		
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}


void __fastcall TfrmVirtualPrintersInterface::pnlReassignClick(
		TObject *Sender)
{
	if(lbVirtualPrinters->ItemIndex >= 0)
	{
		int VirtualPrinterKey = (int)(lbVirtualPrinters->Items->Objects[lbVirtualPrinters->ItemIndex]);
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr<TfrmPrinterRedirect> frmPrinterRedirect( TfrmPrinterRedirect::Create<TfrmPrinterRedirect>(this,DBTransaction));
		frmPrinterRedirect->lbCaption->Caption = "Select Printer";
		if(frmPrinterRedirect->ShowModal() == mrOk)
		{
			bool Assign = false;
			TManagerPhysicalPrinter ManagerPhysicalPrinter;
			TPrinterPhysical PrinterPhysical = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,frmPrinterRedirect->SelectedPrinterKey);
			{
				TManagerVirtualPrinter::DBReAssignPrinter(DBTransaction,VirtualPrinterKey,PrinterPhysical.PhysicalPrinterKey);
			}
		}
		DBTransaction.Commit();
	}
	else
	{
		MessageBox("Please Select a Printer to Assign To...?", "Warning",	MB_OK);
	}
}
//---------------------------------------------------------------------------

