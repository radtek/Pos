//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <locale>
#include "CardSwipe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------

__fastcall TfrmCardSwipe::TfrmCardSwipe(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCardSwipe::FormShow(TObject *Sender)
{
   Done = false;
   tiSwipe->Enabled = false;
	Swiping = false;
   SwipeString = "";
   Animate1->Active = true;
	Animate1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCardSwipe::CardSwipe(TMessage& Message)
{
	UnicodeString Data = *((UnicodeString *)Message.WParam);
	SwipeString = Data;
	Done = true;
	tbOtherOpt->Visible = true;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardSwipe::BitBtn1Click(TObject *Sender)
{
	tiSwipe->Enabled = false;
	Swiping = false;
	SwipeString = "";
	Done = true;
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardSwipe::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   Animate1->Active = false;
   tbOtherOpt->Caption = "Clear Card";
   tbOtherOpt->Visible = true;
	tbtnCancel->Caption = "Cancel";

}
//---------------------------------------------------------------------------

void __fastcall TfrmCardSwipe::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
	CanClose = Done;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardSwipe::tbOtherOptClick(TObject *Sender)
{
	tiSwipe->Enabled = false;
	Swiping = false;
	SwipeString = "";
	Done = true;
	ModalResult = mrAbort;
}
//---------------------------------------------------------------------------

