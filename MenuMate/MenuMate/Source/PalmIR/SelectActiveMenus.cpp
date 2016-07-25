//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "SelectActiveMenus.h"
#include "Data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma resource "*.dfm"
TfrmSelectActiveMenus *frmSelectActiveMenus;
//---------------------------------------------------------------------------
__fastcall TfrmSelectActiveMenus::TfrmSelectActiveMenus(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::FormDestroy(TObject *Sender)
{
	while (pnlMenus->ControlCount > 0)
   {
		delete pnlMenus->Controls[0];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectActiveMenus::FormShow(TObject *Sender)
{
	FormResize(NULL);
	while (pnlMenus->ControlCount > 0)
   {
		delete pnlMenus->Controls[0];
	}
	MenuName = "";
	{
		int ColCount = sqrt(frmData->IBQuery->RecordCount);
		if (ColCount * ColCount != frmData->IBQuery->RecordCount)
		{
			ColCount++;
		}
		int x=0, y=0;
		int ColNumber=0, RowNumber=0;
		int ButtonWidth = (Screen->Width / ColCount) / 2;
		int ButtonHeight = Screen->Width / 12;
		ClientWidth = ButtonWidth * ColCount + (Panel3->BorderWidth * 2);
		for (frmData->IBQuery->First(); !frmData->IBQuery->Eof; frmData->IBQuery->Next())
		{
			TTouchButton *NewButton		= new TTouchButton(this);
			NewButton->Parent				= pnlMenus;
			NewButton->Caption			= frmData->IBQuery->FieldByName("MENU_NAME")->AsString;
			NewButton->Tag					= frmData->IBQuery->FieldByName("MENU_KEY")->AsInteger;
			NewButton->Visible			= true;
			if (MenuIsActive(frmData->IBQuery->FieldByName("MENU_KEY")->AsInteger))
			{
				NewButton->Color			= clGreen;
				MenuName						= NewButton->Caption;
			}
			else
			{
				NewButton->Color			= clMaroon;
			}
			NewButton->Font->Color		= clWhite;
			NewButton->OnClick			= MenuClick;
			NewButton->Top					= y;
			NewButton->Left				= x;
			NewButton->Width				= ButtonWidth;
			NewButton->Height				= ButtonHeight;
			ClientHeight					= NewButton->Top + NewButton->Height + Panel1->Height + (Panel3->BorderWidth * 2);
			x									= x + NewButton->Width;

			if (++ColNumber >= ColCount)
			{
				ColNumber = 0;
				RowNumber++;
				x = 0;
				y += NewButton->Height;
			}
		}
	}
	btnCancel->Left = Panel1->ClientWidth - Panel3->BorderWidth - btnCancel->Width;
	btnOk->Left = btnCancel->Left - btnOk->Width - btnNone->Left;
}
//---------------------------------------------------------------------------
bool TfrmSelectActiveMenus::MenuIsActive(int MenuKey)
{
	for ( int i = 0 ; i < TDeviceRealTerminal::Instance().Menus.Current->Count; i ++)
	{
      TListMenu * ThisMenu = TDeviceRealTerminal::Instance().Menus.Current->MenuGet(i);
		if (ThisMenu->MenuKey == MenuKey) return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::MenuClick(TObject *Sender)
{
	TTouchButton *ButtonPressed = (TTouchButton *)Sender;
	for (int i=0; i<pnlMenus->ControlCount; i++)
   {
		TTouchButton *Button = (TTouchButton *)pnlMenus->Controls[i];
		if (Button == ButtonPressed)
		{
			Button->Color = clGreen;
			MenuName = Button->Caption;
		}
		else
		{
			Button->Color = clMaroon;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;

		ScaleBy(Screen->Width, Temp);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::btnOkClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::btnNoneClick(TObject *Sender)
{
	MenuName = "";
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------


