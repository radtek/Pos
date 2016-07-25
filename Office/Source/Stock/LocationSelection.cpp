//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LocationSelection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLocationSelection *frmLocationSelection;
//---------------------------------------------------------------------------
__fastcall TfrmLocationSelection::TfrmLocationSelection(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLocationSelection::FormShow(TObject *Sender)
{
// SelectedCategoryList =new TList;
	lstLocation->Items->Clear();

	 //	btnOk->Visible				= true;
	  //	btnCancel->Visible		= true;
	  //	btnClose->Visible			= false;
 
	    LoadTree();
	lstLocation->SetFocus();
}
void TfrmLocationSelection::LoadTree()
{
	lstLocation->Items->BeginUpdate();



	if (!Transaction1->InTransaction) Transaction1->StartTransaction();
	sqlLocation->Close();

         for (sqlLocation->ExecQuery(); !sqlLocation->Eof; sqlLocation->Next())
            {
              lstLocation->Items->Add(sqlLocation->FieldByName("Name")->AsString);
            }
	Transaction1->Commit();
	lstLocation->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLocationSelection::btnOkClick(TObject *Sender)
{

int selected_item= lstLocation->ItemIndex;
   if(selected_item>=0)
   {
Location= lstLocation->Items->Strings[selected_item];
ModalResult = mrOk;
}
 else
   {  Location="";
      Application->MessageBox("Please select   Stock Location.", "Error", MB_ICONERROR + MB_OK);
      return;
   }  
 /*
SelectedLocationList =new TStringList;
  if(lstLocation->SelCount > 0)
  {
       for (int Index = 0; Index < lstLocation->Count; ++Index)
       {
         if (lstLocation->Selected[Index])
         {
            AnsiString NewItemName1=      lstLocation->Items->Strings[Index];
            SelectedLocationList->Add(NewItemName1 );
            ModalResult = mrOk;
         }
       }
  }
  else
   {
      Application->MessageBox("Please select atleast one Stock Location.", "Error", MB_ICONERROR + MB_OK);
      return;
   }  */
}
//---------------------------------------------------------------------------
void __fastcall TfrmLocationSelection::btnCancelClick(TObject *Sender)
{
ModalResult = mrCancel;

}
//---------------------------------------------------------------------------
