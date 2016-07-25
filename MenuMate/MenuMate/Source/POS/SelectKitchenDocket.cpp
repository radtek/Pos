//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "SelectKitchenDocket.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "DocketManager.h"



#define RECEIPT_DELIMITER "#####"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmSelectKitchenDocket *frmSelectKitchenDocket;
//---------------------------------------------------------------------------


void __fastcall TfrmSelectKitchenDocket::FormCreate(TObject *Sender)
{
	FormResize(this);
}

//---------------------------------------------------------------------------

__fastcall TfrmSelectKitchenDocket::TfrmSelectKitchenDocket(TComponent* Owner)
	: TZForm(Owner)
{
	ManagerDockets->Date = Date();

   calReceipt->Date = ManagerDockets->Date;
}

__fastcall TfrmSelectKitchenDocket::~TfrmSelectKitchenDocket()
{
	ManagerDockets->Close(); 
}
//---------------------------------------------------------------------------
void TfrmSelectKitchenDocket::Execute()
{
	if (!ManagerDockets->ReceiptsExist())
	{
		MessageBox("There are no dockets to print for this terminal.",
										"Warning", MB_ICONWARNING + MB_OK);

	}
   CurrentDate = Date();
	ShowModal();
}
//---------------------------------------------------------------------------
void TfrmSelectKitchenDocket::ShowReceipt()
{
	TStringList *Lines = new TStringList;
	try
	{
		ManagerDockets->Get(Lines);
                memReceipt->Lines->Assign(Lines);

	}
	__finally
	{
		delete Lines;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectKitchenDocket::btnPrintClick(TObject *Sender)
{
	ManagerDockets->Print();
}
//---------------------------------------------------------------------------
/*
void TfrmSelectReceipt::PrintReceipt(TStringList *Receipt)
{
	if (TComms::Instance().ReceiptPrinter == NULL)			throw Exception("No receipt printer selected.");

	TPrintFormat *pPrinter							= new TPrintFormat;
	try
	{
		pPrinter->PrintRaw(ManagerDockets->Receipt, TComms::Instance().ReceiptPrinter);
	}
	__finally
	{
		delete pPrinter;
	}
}
*/
//---------------------------------------------------------------------------
void __fastcall TfrmSelectKitchenDocket::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectKitchenDocket::calReceiptClick(TObject *Sender)
{
	CurrentDate = calReceipt->Date;
}
//---------------------------------------------------------------------------


void TfrmSelectKitchenDocket::SortTables()
{
}

void __fastcall TfrmSelectKitchenDocket::sbAllClick(TObject *Sender)
{
	ManagerDockets->Filter = eAllTill;
	ManagerDockets->Open();
	ShowReceipt();

   btnNext->Enabled = false;
   btnNext->Color = clMaroon;

	if(!ManagerDockets->IsEmpty())
   {
      btnPrev->Enabled = true;
      btnPrev->Color = clNavy;
      ShowReceipt();
   }
   else
   {
		btnPrev->Enabled = false;
      btnPrev->Color = clMaroon;
   }
}
//---------------------------------------------------------------------------




void __fastcall TfrmSelectKitchenDocket::FormResize(TObject *Sender)
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
//---------------------------------------------------------------------------


void __fastcall TfrmSelectKitchenDocket::FormShow(TObject *Sender)
{
	FormResize(this);
	SetWindowPos(Handle, HWND_TOP, Left, Top, Width, Height, 0);	   
}
//---------------------------------------------------------------------------



void __fastcall TfrmSelectKitchenDocket::TouchButton1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Caption = "Enter the Docket Number";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->Mode = pmNumber;
	frmTouchNumpad->INTInitial = 0;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		ManagerDockets->Find(frmTouchNumpad->INTResult);
		ShowReceipt();
	}
}
//---------------------------------------------------------------------------
void TfrmSelectKitchenDocket::SetCurrentDate(TDateTime SelectedDate)
{
	ManagerDockets->Date = SelectedDate;
	btnNext->Enabled = false;
	btnNext->Color = clMaroon;

	if(!ManagerDockets->IsEmpty())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
	}
	ShowReceipt();	
}




void __fastcall TfrmSelectKitchenDocket::sbThisTerminalClick(TObject *Sender)
{
	ManagerDockets->Filter = eThisTill;
	ManagerDockets->Open();
	ShowReceipt();

	btnNext->Enabled = false;
	btnNext->Color = clMaroon;

	if(!ManagerDockets->IsEmpty())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
		ShowReceipt();
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectKitchenDocket::SpeedButton1Click(TObject *Sender)
{
	ManagerDockets->Filter = eActiveDockets;
	ManagerDockets->Open();
	ShowReceipt();

	btnNext->Enabled = false;
	btnNext->Color = clMaroon;

	if(!ManagerDockets->IsEmpty())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
		ShowReceipt();
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
	}
}
//---------------------------------------------------------------------------



void __fastcall TfrmSelectKitchenDocket::memReceiptMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Shift.Contains(ssCtrl))
   {
      TStringList *Lines = new TStringList;
      try
      {
         ManagerDockets->GetWithSymbols(Lines);
         memReceipt->Lines->Assign(Lines);
      }
      __finally
      {
         delete Lines;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectKitchenDocket::btnPrevMouseClick(TObject *Sender)
{
	ManagerDockets->Next();
	if(!ManagerDockets->Eof())
	{
		btnNext->Enabled = true;
		btnNext->Color = clNavy;
	}
	else
	{
		btnPrev->Enabled = false;
		btnPrev->Color = clMaroon;
		btnNext->Enabled = true;
		btnNext->Color = clNavy;
	}
	ShowReceipt();
}
//---------------------------------------------------------------------------


void __fastcall TfrmSelectKitchenDocket::btnNextMouseClick(TObject *Sender)
{
	ManagerDockets->Prev();
	if(!ManagerDockets->Bof())
	{
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	else
	{
		btnNext->Enabled = false;
		btnNext->Color = clMaroon;
		btnPrev->Enabled = true;
		btnPrev->Color = clNavy;
	}
	ShowReceipt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectKitchenDocket::btnBillDownMouseClick(TObject *Sender)
{
	memReceipt->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectKitchenDocket::btnBillUpMouseClick(TObject *Sender)
{
	memReceipt->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------

