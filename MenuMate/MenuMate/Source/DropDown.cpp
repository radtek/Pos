//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DropDown.h"
#include "MMLogging.h"
#include "Enum.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
#define NUMBER_OF_MSG_IN_VIEW 10

TfrmDropDownFunc *frmDropDownFunc;
//---------------------------------------------------------------------------
__fastcall TfrmDropDownFunc::TfrmDropDownFunc(TComponent* Owner)
	: TZForm(Owner), CreateCancel(true)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


void __fastcall TfrmDropDownFunc::FormDestroy(TObject *Sender)
{
	while (sbMessages->ControlCount > 0)
   {
		delete sbMessages->Controls[0];
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDropDownFunc::btnMsgDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
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

void __fastcall TfrmDropDownFunc::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
     //	ScaleBy(Screen->Width, Temp);
   }
   this->Height = Screen->Height - 40;
   Left				= (Screen->Width - Width) / 2;
	Top				= (Screen->Height - Height) / 2;
   pnlCourses->Width = ClientWidth - pnlCourses->Left - pnlCourses->Left;
   pnlCourses->Height = ClientHeight - pnlCourses->Left - pnlCourses->Left;
   btnMsgUp->Top = pnlCourses->Height - btnMsgUp->Height - pnlCourses->Left;
   btnMsgDown->Top = pnlCourses->Height - btnMsgDown->Height - pnlCourses->Left;
   btnMsgUp->Left = pnlCourses->Left;
   btnMsgDown->Left = pnlCourses->Width - btnMsgDown->Width - pnlCourses->Left;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDropDownFunc::pnlCoursesResize(TObject *Sender)
{
   sbMessages->Align = alClient;
   sbMessages->Align = alNone;
   sbMessages->Width += 20;//sbCourses->VertScrollBar->Size;
   sbMessages->Height = btnMsgUp->Top - pnlCourses->Left;//sbCourses->VertScrollBar->Size;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDropDownFunc::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmDropDownFunc::BtnCloseClick(TObject *Sender)
{
   ModalResult = mrCancel;
}


__fastcall TExeBtn::TExeBtn(Classes::TComponent* AOwner) : TTouchBtn(AOwner)//TCustomStaticText(AOwner)
{
   TextResult = "";
   Title = "";
   Font->Color 		= clBlack;
	Font->Name 			= "Arial";
	Font->Size 			= 12;
	Font->Style			= TFontStyles() << fsBold;
	BevelInner	 		= bvNone;
   Selected = false;
}

void __fastcall TfrmDropDownFunc::FormShow(TObject *Sender)
{
	FormResize(NULL);
	SetWindowPos(Handle, HWND_TOP	, Left, Top, Width, Height, 0);
   int ButtonWidth;
	int x = pnlCourses->Left;
	int y = pnlCourses->Left;
	const int PixelGap = pnlCourses->Left;
	ButtonWidth = pnlCourses->Width - (pnlCourses->Left * 2);

	if(CreateCancel)
	{
		CreateCancelBtn();
		y += sbMessages->Height / NUMBER_OF_MSG_IN_VIEW + PixelGap;
	}

	try
   {
		for ( std::map<AnsiString,TFunctionEvent>::const_iterator Btn = Buttons.begin();
            Btn != Buttons.end();
            Btn++)
      {
			TExeBtn *NewButton = new TExeBtn(this);
         NewButton->Parent = sbMessages;
         NewButton->Visible = true;
         NewButton->Font->Color = clWhite;
         NewButton->Title =  Btn->first;
         NewButton->FunctionToCall = Btn->second;
			NewButton->TextResult = Btn->first;
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

void TfrmDropDownFunc::Clear()
{
	while (sbMessages->ControlCount > 0)
   {
		delete sbMessages->Controls[0];
   }
}

void TfrmDropDownFunc::AddButton(AnsiString ButtonName,TFunctionEvent inClick)
{
	Buttons[ButtonName] = inClick;
}

void __fastcall TfrmDropDownFunc::BtnExeClick(TObject *Sender)
{
	TExeBtn *Button = (TExeBtn *)Sender;
	FunctionToCall = Button->FunctionToCall;
   ModalResult = mrOk;
}

void TfrmDropDownFunc::CreateCancelBtn()
{
   TExeBtn *CloseButton = new TExeBtn(this);
	CloseButton->Parent = sbMessages;
	CloseButton->Visible = true;
	CloseButton->Font->Color = clWhite;
	CloseButton->Title =  "Cancel";
	CloseButton->Caption = CloseButton->Title;
	CloseButton->TextResult = "";
	CloseButton->Left 		= pnlCourses->Left;
	CloseButton->Top 			= pnlCourses->Left;
	CloseButton->Height 		= sbMessages->Height / NUMBER_OF_MSG_IN_VIEW;
	CloseButton->Width  = pnlCourses->Width - (pnlCourses->Left * 2);
	CloseButton->ButtonColor 	= clGreen;
	CloseButton->OnMouseClick = BtnCloseClick;
}


void __fastcall TfrmDropDownFunc::btnMsgUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
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

