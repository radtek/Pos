//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddTab.h"
#include "MMTouchKeyboard.h"
#include "POSMain.h"
#include "CardSwipe.h"
#include "Membership.h"
#include "DBTab.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TForm * TfrmAddTab::WinOwner = NULL;

__fastcall TfrmAddTab::TfrmAddTab(TComponent* Owner)
	: TForm(Owner)
{
	AllowCancel = true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::CreateParams(Controls::TCreateParams &params)
{ 
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
//---------------------------------------------------------------------------
TfrmAddTab *TfrmAddTab::Create(TForm* Owner)
{
	WinOwner = Owner;
	return new TfrmAddTab(Owner);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::FormShow(TObject *Sender)
{
	FormResize(this);

	if(TabName == "")
   {
   	btnAddTab->Visible = false;
   }
   else
   {
		btnName->Caption = TabName;
   	btnAddTab->Visible = true;
   }
   
   if(!AllowCancel)
   {
      btnNewTabCancel->Enabled = false;
      btnNewTabCancel->Visible = false;
   }
   else
   {
      btnNewTabCancel->Enabled = true;
      btnNewTabCancel->Visible = true;
   }
}

void __fastcall TfrmAddTab::LoadDetails(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	if(inTabKey != 0)
	{
		TabKey = inTabKey;
		TMMTabType  TabType;
		TDBTab::GetTabDetails(DBTransaction,
							  TabKey,
							  TabName,
							  TabIDType,
							  TabIDNumber,
							  TabIDExpiry,
							  TabType);

		Label1->Caption = "Edit Tab";
		btnName->Caption = TabName;
		btnType->Caption = TabIDType;
		btnNumber->Caption = TabIDNumber;
		btnDate->Caption = TabIDExpiry;

		if(TDBTab::GetTabCard(DBTransaction,inTabKey) != "")
		{
			btnRecordCard->Caption = "Card On File";
			btnRecordCard->Color = clMaroon;
		}
		else
		{
				btnRecordCard->Caption = "Swipe Card";
				btnRecordCard->Color = clTeal;
		}
	}
	else
	{
		TabKey = 0;
		TabName	= "";
		TabIDType = "";
		TabIDNumber = "";
		TabIDExpiry = "";
		btnRecordCard->Caption = "Swipe Card";
		btnRecordCard->Color = clTeal;
		CardString = "";
		btnName->Caption = "";
		btnType->Caption = "";
		btnNumber->Caption = "";
		btnDate->Caption = "";
		Label1->Caption = "Add Tab";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::btnNewTabCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
      int Temp = Tag;
		Tag = Screen->Width;
    //	ScaleBy(Screen->Width, Temp);
   }

   Left				= (Screen->Width - Width) / 2;
   Top				= (Screen->Height - Height) / 2;  
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   AllowCancel = true;
	Label1->Caption = "Add a Tab";
//   btnName->Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::FormCreate(TObject *Sender)
{
   TabKey = 0;
   Label1->Caption = "Add a Tab";
   btnName->Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::btnNameClick(TObject *Sender)
{
 	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 25;
   frmTouchKeyboard->AllowCarriageReturn = false;
   frmTouchKeyboard->StartWithShiftDown = false;
   frmTouchKeyboard->KeyboardText = ((TTouchBtn *)Sender)->Caption;
   if (Sender == btnName) frmTouchKeyboard->Caption = "Enter Tab Name";
	else if (Sender == btnType) frmTouchKeyboard->Caption = "Enter ID Type";
   else if (Sender == btnNumber) frmTouchKeyboard->Caption = "Enter ID Number";
   else if (Sender == btnDate) frmTouchKeyboard->Caption = "Enter ID Expiry Date";
   if (frmTouchKeyboard->ShowModal() == mrOk)
   {
	  ((TTouchBtn *)Sender)->Caption =  frmTouchKeyboard->KeyboardText;
	  if (Sender == btnName)
	  {
		TabName = frmTouchKeyboard->KeyboardText;
		if (((TTouchBtn *)Sender)->Caption != "")
		{
			btnAddTab->Enabled = true;
			btnAddTab->Visible = true;
		}
		else
		{
			btnAddTab->Enabled = false;
		 }
	  }
		else if (Sender == btnType)
	  {
		TabIDType = frmTouchKeyboard->KeyboardText;
	  }
	  else if (Sender == btnNumber)
	  {
		TabIDNumber = frmTouchKeyboard->KeyboardText;
	  }
	  else if (Sender == btnDate)
	  {
		TabIDExpiry = frmTouchKeyboard->KeyboardText;
	  }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::btnAddTabClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::NumberBtnMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   AnsiString Temp1 = btnName->Caption;
   AnsiString Temp2 = ((TTouchBtn *)Sender)->Caption;
   btnName->Caption = Temp1 + Temp2;
   TabName = btnName->Caption;
   if (btnName->Caption != "")
   {
	  btnAddTab->Enabled = true;
	  btnAddTab->Visible = true;
   }
   else
   {
	  btnAddTab->Enabled = false;
   }
   btnAddTab->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTab::btnCLRMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   btnName->Caption = "";
   btnAddTab->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TfrmAddTab::btnRecordCardClick(TObject *Sender)
{
	std::auto_ptr<TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create<TfrmCardSwipe>(this));
	frmCardSwipe->ShowModal();
	if (frmCardSwipe->ModalResult == mrOk)
   {
		CardString = AnsiString(frmCardSwipe->SwipeString).SubString(1,80);
	  btnRecordCard->Caption = "Card On File";
	  btnRecordCard->Color = clMaroon;
	}
   else if (frmCardSwipe->ModalResult == mrAbort)
   {
		CardString = AnsiString(frmCardSwipe->SwipeString).SubString(1,80);
		btnRecordCard->Caption = "Swipe Card";
		btnRecordCard->Color = clTeal;
   }
}
//---------------------------------------------------------------------------

