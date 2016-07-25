//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectImportType.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmSelectImportType *frmSelectImportType;
//---------------------------------------------------------------------------
__fastcall TfrmSelectImportType::TfrmSelectImportType(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectImportType::btnImportStockClick(TObject *Sender)
{
  IsImportStock = true;
  ModalResult = mrOk ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectImportType::btnImportSupplierClick(TObject *Sender)
{
   IsImportStock = false;
   ModalResult = mrOk ;
}
//--------------------------------------------------------------------------

