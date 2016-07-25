//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectCategory.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmSelectCategory *frmSelectCategory;
//---------------------------------------------------------------------------
__fastcall TfrmSelectCategory::TfrmSelectCategory(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectCategory::lbCategoryDblClick(TObject *Sender)
{
	ModalResult = mrOk;	
}
//---------------------------------------------------------------------------

