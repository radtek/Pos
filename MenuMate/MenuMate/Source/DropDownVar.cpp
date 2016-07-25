//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DropDownVar.h"
#include "MMLogging.h"
#include "Enum.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#define NUMBER_OF_MSG_IN_VIEW 8

TfrmDropDownVar *frmDropDownVar;
//---------------------------------------------------------------------------
__fastcall TfrmDropDownVar::TfrmDropDownVar(TComponent* Owner)
	: TZForm(Owner)
{
	ShowCloseButton = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


void __fastcall TfrmDropDownVar::FormDestroy(TObject *Sender)
{
	while (sbMessages->ControlCount > 0)
	{
		delete sbMessages->Controls[0];
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmDropDownVar::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
   	//ScaleBy(Screen->Width, Temp);
   }
   this->Height = Screen->Height;
   Left				= (Screen->Width - Width) / 2;
   Top				= (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------


void __fastcall TfrmDropDownVar::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmDropDownVar::BtnCloseClick(TObject *Sender)
{
   ModalResult = mrCancel;
}


__fastcall TExeBtnVar::TExeBtnVar(Classes::TComponent* AOwner) : TTouchBtn(AOwner)//TCustomStaticText(AOwner)
{
	TextResult = "";
	Result = -1;
   Title = "";
   Font->Color 		= clBlack;
	Font->Name 			= "Arial";
	Font->Style			= TFontStyles() << fsBold;
	BevelInner	 		= bvNone;
   Selected = false;
}

void __fastcall TfrmDropDownVar::FormShow(TObject *Sender)
{
	FormResize(NULL);

   int ButtonWidth;
	const int PixelGap = 5;
   int x = PixelGap;
	int y = pnlCourses->Left;
   ButtonWidth = sbMessages->Width - (PixelGap * 2) - 20;

	if(ShowCloseButton)
	{
		TExeBtnVar *CloseButton = new TExeBtnVar(this);
		CloseButton->Parent = sbMessages;
		CloseButton->Visible = true;
		CloseButton->Font->Color = clWhite;
		CloseButton->Title =  "Cancel";
		CloseButton->Caption = CloseButton->Title;
		CloseButton->TextResult = "";
		CloseButton->Result = -1;
		CloseButton->Left 		= x;
		CloseButton->Top 			= y;
		CloseButton->Height 		= sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
		CloseButton->Width  = ButtonWidth;
		CloseButton->ButtonColor 	= clGreen;
		CloseButton->OnMouseClick = BtnCloseClick;

		y += CloseButton->Height + PixelGap;
	}

   try
   {
		for ( std::vector<TddvBtn>::const_iterator Btn = Buttons.begin();
            Btn != Buttons.end();
            Btn++)
      {
			TExeBtnVar *NewButton = new TExeBtnVar(this);
         NewButton->Parent = sbMessages;
         NewButton->Visible = true;
         NewButton->Font->Color = clWhite;
			NewButton->Title =  Btn->ButtonCaption;
			NewButton->TextResult = Btn->ButtonCaption;
			NewButton->Result = Btn->ReturnVal;
			NewButton->ButtonColor = clMaroon;
			NewButton->Caption = NewButton->Title;
         NewButton->Left = x;
         NewButton->Top	= y;
         NewButton->Height	= sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
			NewButton->Width 	= ButtonWidth;
			NewButton->OnMouseClick = BtnExeClick;
         y += NewButton->Height + PixelGap;
      }
   }
   catch(Exception &E)
   {
      TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
   }

}

void TfrmDropDownVar::Clear()
{
	while (sbMessages->ControlCount > 0)
   {
		delete sbMessages->Controls[0];
   }
}

void TfrmDropDownVar::AddButton(TddvBtn Btn)
{
	Buttons.push_back(Btn);
}

void __fastcall TfrmDropDownVar::BtnExeClick(TObject *Sender)
{
	TExeBtnVar *Button = (TExeBtnVar *)Sender;
	Result = Button->Result;
   ModalResult = mrOk;
}


void __fastcall TfrmDropDownVar::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	ShowCloseButton = true;	
}
//---------------------------------------------------------------------------


TddvBtn::TddvBtn()
{
	ButtonCaption = "";
	ReturnVal = 0;
}

TddvBtn::TddvBtn(AnsiString BtnName, int RetVal)
{
	ButtonCaption = BtnName;
	ReturnVal = RetVal;
}
void __fastcall TfrmDropDownVar::btnMsgUpMouseClick(TObject *Sender)
{
	int StartScrollPos = sbMessages->VertScrollBar->Position;
	int ButtonHeight = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
	int EndScrollPos = StartScrollPos - sbMessages->ClientHeight + ButtonHeight;
	if (EndScrollPos < 0) EndScrollPos = 0;

	for (int i=StartScrollPos; i>= EndScrollPos; i-=8)
	{
		sbMessages->VertScrollBar->Position = i;
		sbMessages->Update();
	}
	sbMessages->VertScrollBar->Position = EndScrollPos;

}
//---------------------------------------------------------------------------

void __fastcall TfrmDropDownVar::btnMsgDownMouseClick(TObject *Sender)
{
   int StartScrollPos = sbMessages->VertScrollBar->Position;
   int ButtonHeight = sbMessages->ClientHeight / NUMBER_OF_MSG_IN_VIEW;
   int EndScrollPos = StartScrollPos + sbMessages->ClientHeight - ButtonHeight;
   if (EndScrollPos > sbMessages->VertScrollBar->Range) EndScrollPos = sbMessages->VertScrollBar->Range;

   for (int i=StartScrollPos; i<= EndScrollPos; i+=8)
   {
		sbMessages->VertScrollBar->Position = i;
	  sbMessages->Update();
   }
   sbMessages->VertScrollBar->Position = EndScrollPos;
}
//---------------------------------------------------------------------------

