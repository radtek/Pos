//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "SelectCurrentMenu.h"
#include "math.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectCurrentMenu *frmSelectCurrentMenu;
//---------------------------------------------------------------------------
__fastcall TfrmSelectCurrentMenu::TfrmSelectCurrentMenu(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectCurrentMenu::FormShow(TObject *Sender)
{
	while (pnlMenus->ControlCount > 0)
   {
		delete pnlMenus->Controls[0];
   }
	int ColCount = 1;

	if(TDeviceRealTerminal::Instance().Menus->Current->Count != 0)
   {
		ColCount = sqrt((float)TDeviceRealTerminal::Instance().Menus->Current->Count);
   }

	if (ColCount * ColCount !=  TDeviceRealTerminal::Instance().Menus->Current->Count)
	{
		ColCount++;
	}

	SelectedMenuIndex = -1;
	int x=0, y=0;
	int ColNumber=0;
	int ButtonWidth = (Screen->Width / ColCount) / 2;
	int ButtonHeight = Screen->Width / 12;
	ClientWidth = ButtonWidth * ColCount + (Panel3->BorderWidth * 2);
   for (int i = 0 ; i < TDeviceRealTerminal::Instance().Menus->Current->Count ; i++)
	{
      TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->Current->MenuGet(i);
		TTouchBtn *NewButton		= new TTouchBtn(this);
		NewButton->Parent				= pnlMenus;
		NewButton->Caption			= Menu->MenuName;
		NewButton->Tag					= i;
		NewButton->Visible			= true;
		if (MenuIsCurrent(Menu->MenuKey))
		{
			NewButton->ButtonColor			= clGreen;
		}
		else
		{
			NewButton->ButtonColor			= clNavy;
		}
		NewButton->Font->Color		= clWhite;
        NewButton->Font->Name 		= "Arial";
        if(TGlobalSettings::Instance().ShowLargeFonts)
        {
               NewButton->Font->Size = 18;
        }
        NewButton->Font->Style		= TFontStyles() << fsBold;
		NewButton->OnMouseClick		= MenuClick;
		NewButton->Top					= y;
		NewButton->Left				= x;
		NewButton->Width				= ButtonWidth;
		NewButton->Height				= ButtonHeight;
		ClientHeight					= NewButton->Top + NewButton->Height + (Panel3->BorderWidth * 2);
		x									= x + NewButton->Width;

		if (++ColNumber >= ColCount)
		{
			ColNumber = 0;
			x = 0;
			y += NewButton->Height;
		}
	}

	// Cancel Button
   TTouchBtn *NewButton		= new TTouchBtn(this);
   NewButton->Parent				= pnlMenus;
   if(TGlobalSettings::Instance().ShowLargeFonts)
        {
               NewButton->Font->Size = 18;
        }
   NewButton->Caption			= "Cancel";
   NewButton->Tag					= -1;
   NewButton->Visible			= true;
	NewButton->ButtonColor		= clMaroon;
   NewButton->Font->Color		= clWhite;
   NewButton->Font->Name 		= "Arial";
	NewButton->OnMouseClick		= tbtnCancelClick;
   NewButton->Top					= y;
   NewButton->Left				= x;
   NewButton->Width				= ButtonWidth;
   NewButton->Height				= ButtonHeight;
   ClientHeight					= NewButton->Top + NewButton->Height + (Panel3->BorderWidth * 2);
   x									= x + NewButton->Width;
   if (++ColNumber >= ColCount)
   {
      ColNumber = 0;
      x = 0;
		y += NewButton->Height;
   }

	pnlMenus->Height = y + ButtonHeight;
	ClientHeight = pnlMenus->Height + 5;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool TfrmSelectCurrentMenu::MenuIsCurrent(int MenuKey)
{
   bool RetVal = false;
   if(TDeviceRealTerminal::Instance().Menus->VisibleMenu)
   {
      TListMenu *Menu = TDeviceRealTerminal::Instance().Menus->VisibleMenu;
      if (Menu->MenuKey == MenuKey)
      {
         RetVal = true;
      }
   }
	return RetVal;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectCurrentMenu::MenuClick(TObject *Sender)
{
	TTouchBtn *Button = (TTouchBtn *)Sender;
   SelectedMenuIndex = Button->Tag;
	Button->Color = clGreen;
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectCurrentMenu::tbtnCancelClick(TObject *Sender)
{
   SelectedMenuIndex = -1;
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

