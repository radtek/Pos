//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddSupplierBarcode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAddSupplierBarcode *frmAddSupplierBarcode;
//---------------------------------------------------------------------------
__fastcall TfrmAddSupplierBarcode::TfrmAddSupplierBarcode(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TfrmAddSupplierBarcode::SetDataSet(TDataSet *theDataSet)
{
	dsSupplierUnits->DataSet = theDataSet;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSupplierBarcode::dbgSupplierUnitsDblClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

