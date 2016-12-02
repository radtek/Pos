//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CaptureCustomerDetails.h"
#include "MMTouchKeyboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmCaptureCustomerDetails::TfrmCaptureCustomerDetails(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::FormShow(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edCustomerNameClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 150;
	frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edCustomerName->Text;
    frmTouchKeyboard->Caption = "Enter Customer Name";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        edCustomerName->Text = frmTouchKeyboard->KeyboardText;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edAddressClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 150;
	frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edAddress->Text;
    frmTouchKeyboard->Caption = "Enter Customer Address";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        edAddress->Text = frmTouchKeyboard->KeyboardText;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edTinClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 150;
	frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edTIN->Text;
    frmTouchKeyboard->Caption = "Enter Tin No";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        edTIN->Text = frmTouchKeyboard->KeyboardText;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edBusinessStyleClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 150;
	frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edBusinessStyle->Text;
    frmTouchKeyboard->Caption = "Enter Business Style";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        edBusinessStyle->Text = frmTouchKeyboard->KeyboardText;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edSCPWDNOClick(TObject *Sender)
{
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 150;
	frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edSCPWDNO->Text;
    frmTouchKeyboard->Caption = "Enter SCD/PWD ID#";
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        edSCPWDNO->Text = frmTouchKeyboard->KeyboardText;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::btnOKClick(TObject *Sender)
{
    customerDetails.CustomerName = edCustomerName->Text;
    customerDetails.Address = edAddress->Text;
    customerDetails.TinNo = edTIN->Text;
    customerDetails.BusinessStyle = edBusinessStyle->Text;
    customerDetails.SC_PWD_ID = edSCPWDNO->Text;
    ModalResult = mrOk;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::btnCancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//----------------------------------------------------------------------------------------------
