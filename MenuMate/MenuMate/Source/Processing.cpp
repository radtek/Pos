//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Processing.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmProcessing::TfrmProcessing(TComponent* Owner)
   : TZForm(Owner)
{
   ShowProgress = false;
   CanCancel = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::FormShow(TObject *Sender)
{
   Closeable = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::FormDeactivate(TObject *Sender)
{
   if (!Closeable && Visible)
   {
      SetFocus();
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
	CanClose = Closeable;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::Show()
{
	ProgressBar->Position = 0;
	btnCancel->Enabled = true;
	if (CanCancel)
	{
		btnCancel->Visible = true;
		ClientWidth = btnCancel->Left + btnCancel->Width + ProgressBar->Left;
	}
	else
	{
		btnCancel->Visible = false;
		ClientWidth = ProgressBar->Left * 2 + ProgressBar->Width;
	}
	ProgressBar->Visible = ShowProgress;
	TForm::Show();
	Update();
	Cancelled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProcessing::Close()
{
   Closeable = true;
   TForm::Close();
}
//---------------------------------------------------------------------------
void TfrmProcessing::SetMessage(UnicodeString Message)
{
	Label1->Caption = Message;
	Update();
}

UnicodeString TfrmProcessing::GetMessage()
{
	return Label1->Caption;
}

void TfrmProcessing::SetTitle(UnicodeString inTitle)
{
	Label2->Caption = inTitle;
}

UnicodeString TfrmProcessing::GetTitle()
{
   return Label2->Caption;
}

//---------------------------------------------------------------------------
void TfrmProcessing::SetPosition(int iPos)
{
	ProgressBar->Position = iPos;
}
//---------------------------------------------------------------------------
void TfrmProcessing::SetMax(int iMax)
{
	ProgressBar->Max = iMax;
}
//---------------------------------------------------------------------------

void __fastcall TfrmProcessing::btnCancelClick(TObject *Sender)
{
	Cancelled = true;
}
//---------------------------------------------------------------------------

__fastcall TfrmProcessing::~TfrmProcessing()
{
   Close();
}
