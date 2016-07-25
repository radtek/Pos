//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectMenu.h"
#include "MMData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmSelectMenu *frmSelectMenu;
//---------------------------------------------------------------------------
__fastcall TfrmSelectMenu::TfrmSelectMenu(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectMenu::FormShow(TObject *Sender)
{
	if (!qrMenu->Transaction->InTransaction)
	{
		qrMenu->Transaction->StartTransaction();
	}
	qrMenu->Active = true;

	DBGrid1->Columns->Items[0]->Width = DBGrid1->ClientWidth;
	DBGrid1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectMenu::DBGrid1DblClick(TObject *Sender)
{
	Application->ProcessMessages();
	Types::TPoint MyPos = Mouse->CursorPos;
	MyPos = DBGrid1->ScreenToClient(MyPos);
	TGridCoord MyGridCoord = DBGrid1->MouseCoord(MyPos.x, MyPos.y);
	if (MyGridCoord.Y > -1)
	{
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectMenu::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	MenuKey = qrMenu->FieldByName("Menu_Key")->AsInteger;
	if (qrMenu->Transaction->InTransaction)
	{
		qrMenu->Transaction->Commit();
	}
}
//---------------------------------------------------------------------------

