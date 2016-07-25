//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EftChequeVerify.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
#include "Eftpos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmChequeVerfiy::TfrmChequeVerfiy(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	}
	ClientWidth = Panel2->Left + Panel2->Width + Panel27->Left;
	ClientHeight = Panel27->Height + (Panel27->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::FormShow(TObject *Sender)
{
	FormResize(NULL);
 	edChqAmount->Text = FloatToStrF(ChequeAmount, ffCurrency, 15, 2);
	edChqSerial->Text = "";
	edChqBranch->Text = "";
	edChqAccount->Text = "";
	if(EftPos->ChequeSerialMaxLength == 0){edChqSerial->Enabled = false;} else {edChqSerial->Enabled = true;}
	if(EftPos->ChequeBranchMaxLength == 0){edChqBranch->Enabled = false;} else {edChqBranch->Enabled = true;}
	if(EftPos->ChequeAccountMaxLength == 0){edChqAccount->Enabled = false;} else {edChqAccount->Enabled = true;}
}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::btnOkClick(TObject *Sender)
{
	ModalResult = mrOk;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::Panel1Click(TObject *Sender)
{
	ModalResult = mrCancel;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::edChqSerialMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool Abort = false;
   bool Valid = false;
	do
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = EftPos->ChequeSerialMaxLength;
      frmTouchKeyboard->AllowCarriageReturn = false;
      frmTouchKeyboard->StartWithShiftDown = false;
      frmTouchKeyboard->KeyboardText = edChqSerial->Text;
		frmTouchKeyboard->Caption = "Enter Cheque Serial Number (6-" + IntToStr( EftPos->ChequeSerialMaxLength) +" digits)";
      if (frmTouchKeyboard->ShowModal() == mrOk)
      {
         edChqSerial->Text = frmTouchKeyboard->KeyboardText;
      }
      else
      {
      	Abort = true;
      }

      Valid = EftPos->CheckChequeSerial(edChqSerial->Text);
      if(!Valid)
      {
	      MessageBox(EftPos->ResultText, "Invalid Input", MB_OK + MB_ICONINFORMATION);
         edChqSerial->Text = "";
      }

   }
   while(Abort == false && Valid == false);

}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::edChqBranchMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool Abort = false;
   bool Valid = false;
	do
   {
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = EftPos->ChequeBranchMaxLength;
      frmTouchKeyboard->AllowCarriageReturn = false;
      frmTouchKeyboard->StartWithShiftDown = false;
      frmTouchKeyboard->KeyboardText = edChqBranch->Text;
      frmTouchKeyboard->Caption = "Enter Cheque Branch Number";
      if (frmTouchKeyboard->ShowModal() == mrOk)
      {
         edChqBranch->Text = frmTouchKeyboard->KeyboardText;
      }
      else
      {
      	Abort = true;
      }

      Valid = EftPos->CheckChequeBranch(edChqBranch->Text);
      if(!Valid)
      {
	      MessageBox(EftPos->ResultText, "Invalid Input", MB_OK + MB_ICONINFORMATION);
         edChqBranch->Text = "";
      }

   }
   while(Abort == false && Valid == false);

}
//---------------------------------------------------------------------------
void __fastcall TfrmChequeVerfiy::edChqAccountMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool Abort = false;
   bool Valid = false;
	do
	{
	  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
		frmTouchKeyboard->MaxLength = EftPos->ChequeAccountMaxLength;
      frmTouchKeyboard->AllowCarriageReturn = false;
      frmTouchKeyboard->StartWithShiftDown = false;
      frmTouchKeyboard->KeyboardText = edChqAccount->Text;
      frmTouchKeyboard->Caption = "Enter Cheque Account";
      if (frmTouchKeyboard->ShowModal() == mrOk)
      {
         edChqAccount->Text = frmTouchKeyboard->KeyboardText;
      }
      else
      {
      	Abort = true;
      }

      Valid = EftPos->CheckChequeAccount(edChqAccount->Text);
      if(!Valid)
      {
	      MessageBox(EftPos->ResultText, "Invalid Input", MB_OK + MB_ICONINFORMATION);
         edChqAccount->Text = "";
      }

   }
   while(Abort == false && Valid == false);
}
//---------------------------------------------------------------------------
