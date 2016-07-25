//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddStockCategoryGroup.h"
#include "StockData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmAddStockCategoryGroup *frmAddStockCategoryGroup;
//---------------------------------------------------------------------------
__fastcall TfrmAddStockCategoryGroup::TfrmAddStockCategoryGroup(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStockCategoryGroup::FormShow(TObject *Sender)
{
	if (StockGroupType == gtCategory)
	{
		lbeTitle->Caption	= "Enter the category name and GL Code";
	}
	else
	{
		lbeTitle->Caption	= "Enter the group name and GL Code";
	}
	edName->Text			= Name;
	edGLCode->Text			= GLCode;

	edName->SetFocus();
	edName->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStockCategoryGroup::btnOkClick(TObject *Sender)
{
	Name		= edName->Text;
	GLCode	= edGLCode->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStockCategoryGroup::BitBtn2Click(TObject *Sender)
{
	if (Application->MessageBox("This will update missing GL codes for previous stock transactions for export purposes.\r"
			"Continue? (This will occur immediately)\r\r"
			"Warning: This will only succedd if the Category Name hasn't changed.",
			"Continue?", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();
		try
		{
			Screen->Cursor = crHourGlass;
			qrUpdateInvoiceGLCode->Close();
			qrUpdateInvoiceGLCode->ParamByName("GLCode")->AsString			= edGLCode->Text;
			qrUpdateInvoiceGLCode->ParamByName("StockCategory")->AsString	= edName->Text;
			qrUpdateInvoiceGLCode->ExecQuery();

			Transaction->Commit();
		}
		__finally
		{
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------

