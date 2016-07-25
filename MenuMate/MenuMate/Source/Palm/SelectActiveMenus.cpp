//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "SelectActiveMenus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmSelectActiveMenus *frmSelectActiveMenus;
//---------------------------------------------------------------------------
__fastcall TfrmSelectActiveMenus::TfrmSelectActiveMenus(TManagerMenus *inManagerMenus,
		Database::TDBTransaction &inDBTransaction, TMenuType menuType) :

	TForm(static_cast<TComponent*>(NULL)),
	DBTransaction(inDBTransaction),
	ManagerMenus(inManagerMenus),
	MenuType(menuType)
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
		std::auto_ptr<TStringList> MenuList(new TStringList);
		ManagerMenus->GetMenuList(DBTransaction,MenuList.get(), MenuType);

		int ColCount = sqrt(MenuList->Count);
		if (ColCount * ColCount != MenuList->Count)
		{
			ColCount++;
		}
		int x=0, y=0;
		int ColNumber=0, RowNumber=0;
		int ButtonWidth = (Screen->Width / ColCount) / 2;
		int ButtonHeight = Screen->Width / 12;
		ClientWidth = ButtonWidth * ColCount + (Panel3->BorderWidth * 2);
		for (int i = 0; i < MenuList->Count; i++)
		{
			TTouchBtn *NewButton		= new TTouchBtn(this);
			NewButton->Parent			= pnlMenus;
			NewButton->Caption			= MenuList->Strings[i];
			NewButton->Tag				= (int)MenuList->Objects[i];
			NewButton->Visible			= true;
			if (MenuIsActive(NewButton->Tag))
			{
				NewButton->ButtonColor	= clGreen;
				MenuName				= NewButton->Caption;
			}
			else
			{
				NewButton->ButtonColor	= clMaroon;
			}
			NewButton->Font->Color		= clWhite;
			NewButton->OnMouseClick		= MenuClick;
			NewButton->Top			  	= y;
			NewButton->Left				= x;
			NewButton->Width		  	= ButtonWidth;
			NewButton->Height		  	= ButtonHeight;
			ClientHeight			  	= NewButton->Top + NewButton->Height + Panel1->Height + (Panel3->BorderWidth * 2);
			x						  	= x + NewButton->Width;

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
	for (int i = 0 ; i < ManagerMenus->Current->Count ; i++)
   {
      TListMenu *Menu = ManagerMenus->Current->MenuGet(i);
		if (Menu->MenuKey == MenuKey) return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::MenuClick(TObject *Sender)
{
	TTouchBtn *ButtonPressed = (TTouchBtn *)Sender;
	for (int i=0; i<pnlMenus->ControlCount; i++)
	{
		TTouchBtn *Button = (TTouchBtn *)pnlMenus->Controls[i];
		if (Button == ButtonPressed)
		{
			Button->ButtonColor = clGreen;
			MenuName = Button->Caption;
		}
		else
		{
			Button->ButtonColor = clMaroon;
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


