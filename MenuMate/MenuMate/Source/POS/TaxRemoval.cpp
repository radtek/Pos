//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TaxRemoval.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

__fastcall TfrmTaxRemoval::TfrmTaxRemoval(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl)
{

}

void __fastcall TfrmTaxRemoval::btnOkMouseClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TfrmTaxRemoval::FormShow(TObject *Sender)
{
    if(TfrmTaxRemoval::Caption == "Select Discounts")
    {
        tgTaxList->ColCount = 1;
        tgTaxList->RowCount = TaxList->Count;

        for(int i=0; i< TaxList->Count;i++)
        {
            tgTaxList->Buttons[i][0]->Caption = TaxList->Strings[i];
            tgTaxList->Buttons[i][0]->Tag = (int)TaxList->Objects[i];
            if(RemovedTaxes->IndexOf(TaxList->Strings[i]) >=0)
            {
                tgTaxList->Buttons[i][0]->Color = clGreen;
            }
            else
            {
                tgTaxList->Buttons[i][0]->Color = clGray;
            }
        }
    }
    else
    {
        tgTaxList->ColCount = 1;
        tgTaxList->RowCount = TaxList->Count;

        for(int i=0; i< TaxList->Count;i++)
        {
            tgTaxList->Buttons[i][0]->Caption = TaxList->Strings[i];
            tgTaxList->Buttons[i][0]->Tag = (int)TaxList->Objects[i];
            if(RemovedTaxes->IndexOf(TaxList->Strings[i]) >=0)
            {
                tgTaxList->Buttons[i][0]->Color = clGray;
            }
            else
            {
                tgTaxList->Buttons[i][0]->Color = clGreen;
            }
        }
    }
}

void __fastcall TfrmTaxRemoval::btnCancelMouseClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTaxRemoval::tgTaxListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{

 if(TfrmTaxRemoval::Caption == "Select Discounts")
 {
     if(GridButton->Color == clGray)
     {
       GridButton->Color = clGreen;
       RemovedTaxes->AddObject(GridButton->Caption, (TObject *)GridButton->Tag);
     }
   else
     {
        GridButton->Color = clGray;
        int index = RemovedTaxes->IndexOf(GridButton->Caption);
        if(index >= 0)
        {
           RemovedTaxes->Delete(index);
        }
     }
  }
  else
  {
    if(GridButton->Color == clGreen)
     {
       GridButton->Color = clGray;
       RemovedTaxes->AddObject(GridButton->Caption, (TObject *)GridButton->Tag);
     }
   else
     {
        GridButton->Color = clGreen;
        int index = RemovedTaxes->IndexOf(GridButton->Caption);
        if(index >= 0)
        {
           RemovedTaxes->Delete(index);
        }
     }
  }
}
//---------------------------------------------------------------------------

