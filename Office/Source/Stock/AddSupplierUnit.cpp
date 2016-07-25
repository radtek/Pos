//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddSupplierUnit.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmAddSupplierUnit *frmAddSupplierUnit;
//---------------------------------------------------------------------------
__fastcall TfrmAddSupplierUnit::TfrmAddSupplierUnit(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSupplierUnit::FormShow(TObject *Sender)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	cbUnits->Items->Clear();
	for (qrSupplierUnits->Open(); !qrSupplierUnits->Eof; qrSupplierUnits->Next())
	{
		cbUnits->Items->Add(qrSupplierUnits->FieldByName("Supplier_Unit")->AsString);
	}
	qrSupplierUnits->Close();
	if (Transaction->InTransaction) Transaction->Commit();
	lbeTitle->Caption = SupplierName;
	GroupBox1->Caption = "Supplier Unit (" + ItemName + ")";
	cbUnits->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSupplierUnit::btnOkClick(TObject *Sender)
{
	if (cbUnits->Text == "")
	{
		Application->MessageBox("Please select a unit.", "Error", MB_ICONERROR);
	}
	else
	{
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSupplierUnit::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
