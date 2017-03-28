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
    if(Editing)
    {
        UnicodeString salesCode = "";
        UnicodeString salesTypeName = "";
        TDBSalesTypeAssignment::LoadSelectedSalesType(SalesTypeId, salesCode, salesTypeName);
        SalesTypeInfoPointers[0] = salesTypeName;
        SalesTypeInfoPointers[1] = salesCode;
        btnName->Caption = btnName->Caption + " " + salesTypeName;
        btnCode->Caption = btnCode->Caption + " " + salesCode;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddSalesType::btnOkMouseClick(TObject *Sender)
{
    SalesTypeInfoPointers[0] = SalesTypeInfoPointers[0].Trim();
    SalesTypeInfoPointers[1] = SalesTypeInfoPointers[1].Trim();
    if (SalesTypeInfoPointers[0] != "" && SalesTypeInfoPointers[1] != "")
    {
        ModalResult = mrOk;
        bool isCodeExist = TDBSalesTypeAssignment::IsSalesTypeCodeExist(SalesTypeInfoPointers[1]);

        if(Editing)
        {   MessageBox(SalesTypeId, "Error", MB_OK + MB_ICONERROR);
            TDBSalesTypeAssignment::UpdateSalesType(SalesTypeId, SalesTypeInfoPointers[0], SalesTypeInfoPointers[1]);
            ModalResult = mrOk;
        }
        else if (!isCodeExist)
        {
            TDBSalesTypeAssignment::SaveSalesType(SalesTypeInfoPointers[0], SalesTypeInfoPointers[1]);
            ModalResult = mrOk;
        }
        else
        {
            MessageBox("Entered Code Already Exist.", "Error", MB_OK + MB_ICONERROR);
        }
    }
    else if(SalesTypeInfoPointers[0] == "")
    {
        MessageBox("Please Enter Sales Type Name", "Error", MB_OK + MB_ICONERROR);
    }
    else if(SalesTypeInfoPointers[1] == "")
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
            frmTouchKeyboard->KeyboardText = SalesTypeInfoPointers[0];
            frmTouchKeyboard->MaxLength = 25;
            break;
        case 1:
            Caption = "Enter Sales Type Code";
            frmTouchKeyboard->KeyboardText = SalesTypeInfoPointers[1];
            frmTouchKeyboard->MaxLength = 5;
            break;
        default:
            break;
    }
    frmTouchKeyboard->Caption = Caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
         SalesTypeInfoPointers[btn->Tag] =  frmTouchKeyboard->KeyboardText;
		 DisplaySalesTypeInfoFromPointers();
	}
}
//-----------------------------------------------------------------------------------------------------------
void TfrmAddSalesType::DisplaySalesTypeInfoFromPointers()
{
     btnName->Caption = "Name: " + SalesTypeInfoPointers[0].Trim();
     btnCode->Caption = "Code: " + SalesTypeInfoPointers[1].Trim();
}
