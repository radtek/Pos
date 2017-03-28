//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddSalesType.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmAddSalesType::TfrmAddSalesType(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSalesType::FormShow(TObject *Sender)
{
    //Todo
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSalesType::btnOkMouseClick(TObject *Sender)
{
    if (SalesTypeInfoPointers[0].Trim() != "" && SalesTypeInfoPointers[1].Trim() != "")
    {
        ModalResult = mrOk;
        TDBSalesTypeAssignment::SaveSalesType(SalesTypeInfoPointers[0], SalesTypeInfoPointers[1]);
    }
    else if(SalesTypeInfoPointers[0].Trim() == "")
    {
        MessageBox("Please Enter Sales Type Name", "Error", MB_OK + MB_ICONERROR);
    }
    else if(SalesTypeInfoPointers[1].Trim() == "")
    {
        MessageBox("Please Enter Sales Type Code", "Error", MB_OK + MB_ICONERROR);
    }
}
//----------------------------------------------------------------------------
void __fastcall TfrmAddSalesType::btnCancelMouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//----------------------------------------------------------------------------
void __fastcall TfrmAddSalesType::AddSalesTypeDetails(TObject *Sender)
{
    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);

    if(btn->Tag > 1)
        return;

    AnsiString Caption = "";
    AnsiString KeyboardText = "";
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;


     switch(btn->Tag)
    {
        case 0:
            Caption = "Enter SalesType Name";
            //frmTouchKeyboard->KeyboardText = Info.EMail;
            frmTouchKeyboard->MaxLength = 25;
            break;
        case 1:
            Caption = "Enter Sales Type Code";
            //frmTouchKeyboard->KeyboardText = Info.Name;
            frmTouchKeyboard->MaxLength = 5;
            break;
        default:
            break;
    }
    frmTouchKeyboard->Caption = Caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
         SalesTypeInfoPointers[btn->Tag] =  frmTouchKeyboard->KeyboardText;
         //Info.EMail=  SalesTypeInfoPointers[0].Trim();
        // Info.Name=  SalesTypeInfoPointers[1].Trim();
		 DisplaySalesTypeInfoFromPointers();
	}
}
//-----------------------------------------------------------------------------------------------------------
void TfrmAddSalesType::DisplaySalesTypeInfoFromPointers()
{
     btnName->Caption = btnName->Caption + " " + SalesTypeInfoPointers[0].Trim();
     btnCode->Caption = btnCode->Caption + " " + SalesTypeInfoPointers[1].Trim();
}
