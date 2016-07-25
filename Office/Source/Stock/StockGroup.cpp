//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "StockData.h"
#include "MMData.h"
#include "Utilities.h"
#include "StockGroup.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmStockGroup *frmStockGroup;
//---------------------------------------------------------------------------
__fastcall TfrmStockGroup::TfrmStockGroup(TComponent* Owner)
: TForm(Owner),
frmAddStock(new TfrmAddStock(NULL)),
frmAddStockCategoryGroup(new TfrmAddStockCategoryGroup(NULL)),
ignorePendingDeletions(true)
{
	DoubleBuffered = true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmStockGroup::FormShow(TObject *Sender)
{
  // SelectedCategoryList =new TList;
	lstCategory->Items->Clear();
	if (StockMode == smSelect)
	{
		btnOk->Visible				= true;
		btnCancel->Visible		= true;
	  	btnClose->Visible			= false;
	}
	else if (StockMode == smEdit)
	{
		btnOk->Visible				= false;
		btnCancel->Visible		= false;
		btnClose->Visible			= true;
	}
	    LoadTree();
	lstCategory->SetFocus();
}


//---------------------------------------------------------------------------
void TfrmStockGroup::LoadTree()
{
	lstCategory->Items->BeginUpdate();



	if (!Transaction->InTransaction) Transaction->StartTransaction();
	qrStockCat->Close();
    if(Mode=="All" && Location!="")
    {
        qrStockCatAll->ParamByName("LOCATION")->AsString =Location;

         for (qrStockCatAll->ExecQuery(); !qrStockCatAll->Eof; qrStockCatAll->Next())
            {
              lstCategory->Items->Add(qrStockCatAll->FieldByName("Stock_Category")->AsString);
            }
    }
    else{
     qrStockCat->ParamByName("SUPPLIER_KEY")->AsInteger =ContactKey;
	for (qrStockCat->ExecQuery(); !qrStockCat->Eof; qrStockCat->Next())
	{
      lstCategory->Items->Add(qrStockCat->FieldByName("Stock_Category")->AsString);
	}
    }
	Transaction->Commit();
	lstCategory->Items->EndUpdate();
}

void __fastcall TfrmStockGroup::btnCancelClick(TObject *Sender)
{
		ModalResult = mrCancel;
}
//---------------------------------------------------------------------------



  // end CopySelec

void __fastcall TfrmStockGroup::btnOkClick(TObject *Sender)
{
  SelectedCategoryList =new TStringList;
  if(lstCategory->SelCount > 0)
  {
       for (int Index = 0; Index < lstCategory->Count; ++Index)
       {
         if (lstCategory->Selected[Index])
         {
            AnsiString NewItemName1=      lstCategory->Items->Strings[Index];
            SelectedCategoryList->Add(NewItemName1 );
            ModalResult = mrOk;
         }
       }
  }
  else
   {
      Application->MessageBox("Please select atleast one Stock Category.", "Error", MB_ICONERROR + MB_OK);
      return;
   }

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


