//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectActiveMenus.h"
#include "TouchBtn.h"
#include "math.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

TForm * TfrmSelectActiveMenus::WinOwner = NULL;
//---------------------------------------------------------------------------
__fastcall TfrmSelectActiveMenus::TfrmSelectActiveMenus(TComponent* Owner,TManagerMenus *inManagerMenus,Database::TDBTransaction &inDBTransaction)
	: TForm(Owner) , DBTransaction(inDBTransaction)
{
	ManagerMenus = inManagerMenus;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmSelectActiveMenus::CreateParams(Controls::TCreateParams &params)
{
	TForm::CreateParams( params );
	if( WinOwner )
	{
		params.WndParent = WinOwner->Handle;
		WinOwner = NULL;
	}
}
//---------------------------------------------------------------------------
TfrmSelectActiveMenus *TfrmSelectActiveMenus::Create(TForm* Owner,TManagerMenus *inManagerMenus,Database::TDBTransaction &inDBTransaction)
{
	WinOwner = Owner;
	return new TfrmSelectActiveMenus(Owner,inManagerMenus,inDBTransaction);
}

void __fastcall TfrmSelectActiveMenus::FormShow(TObject *Sender)
{
	while (pnlMenus->ControlCount > 0)
	{
		delete pnlMenus->Controls[0];
	}

	std::auto_ptr<TStringList> MenuList(new TStringList);
	ManagerMenus->GetMenuList(DBTransaction,MenuList.get());

	int ColCount = sqrt(MenuList->Count);
	if (ColCount * ColCount != MenuList->Count)
	{
		ColCount++;
	}
	int x=0, y=0;
	int ColNumber=0;
	int ButtonWidth = (Screen->Width / ColCount) / 2;
	int ButtonHeight = Screen->Width / 12;
	ClientWidth = ButtonWidth * ColCount + (Panel3->BorderWidth * 2);

	for (int i = 0; i < MenuList->Count; i++)
	{
		TTouchBtn *NewButton			= new TTouchBtn(this);
		NewButton->Parent				= pnlMenus;
		NewButton->Caption				= MenuList->Strings[i];
		NewButton->Tag					= (int)MenuList->Objects[i];
		NewButton->Visible				= true;
		if (MenuIsActive(NewButton->Tag))
		{
			NewButton->ButtonColor			= clGreen;
		}
		else
		{
			NewButton->ButtonColor			= clMaroon;
		}
		NewButton->Font->Color		= clWhite;
		NewButton->Font->Name 		= "Arial";

        if(TGlobalSettings::Instance().ShowLargeFonts)
              {
               NewButton->Font->Size = 18;
              }
          else
              {
		NewButton->Font->Size 		= 14;
              }


		NewButton->Font->Style		= TFontStyles() << fsBold;

		NewButton->OnMouseClick		= MenuClick;
		NewButton->Top					= y;
		NewButton->Left				= x;
		NewButton->Width				= ButtonWidth;
		NewButton->Height				= ButtonHeight;
		ClientHeight					= NewButton->Top + NewButton->Height + Panel1->Height + (Panel3->BorderWidth * 2);
		x									= x + NewButton->Width;

		if (++ColNumber >= ColCount)
		{
			ColNumber = 0;
			x = 0;
			y += NewButton->Height;
		}
	}
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
	TTouchBtn *Button = (TTouchBtn *)Sender;
	if (Button->ButtonColor == clGreen)
	{
		Button->ButtonColor = clMaroon;
	}
	else
	{
		Button->ButtonColor = clGreen;
	}
}
//---------------------------------------------------------------------------



void __fastcall TfrmSelectActiveMenus::btnMinimiseMouseClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectActiveMenus::TouchBtn1MouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------


