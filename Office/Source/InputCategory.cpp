//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "InputCategory.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmInputCategory *frmInputCategory;
//---------------------------------------------------------------------------
__fastcall TfrmInputCategory::TfrmInputCategory(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmInputCategory::btnOkClick(TObject *Sender)
{
   CategoryName = tEditCategory->Text;
   GlCode = tEditGlCode->Text;
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInputCategory::btnCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInputCategory::FormShow(TObject *Sender)
{
  tEditCategory->Text = CategoryName;
  tEditGlCode->Text =  GlCode;
}
//---------------------------------------------------------------------------

