//----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dialog.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
//TfrmMenuLoad *frmMenuLoad;
//----------------------------------------------------------------------------
__fastcall TfrmMenuLoad::TfrmMenuLoad(TComponent *Owner)
	: TForm(Owner)
{
}
//----------------------------------------------------------------------------
TButtonResult TfrmMenuLoad::Execute()
{
	btnLoadDB->Enabled		= ShowDBLoad;
	ButtonPressed				= brNone;
	ShowModal();
	return ButtonPressed;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuLoad::btnLoadDBClick(TObject *Sender)
{
	ButtonPressed				= brLoadFromDB;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuLoad::Button2Click(TObject *Sender)
{
	ButtonPressed				= brLoadFromFile;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuLoad::btnNewMenuClick(TObject *Sender)
{
	ButtonPressed				= brNewMenu;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuLoad::btnCancelClick(TObject *Sender)
{
	ButtonPressed				= brNone;
	Close();
}
//---------------------------------------------------------------------------

