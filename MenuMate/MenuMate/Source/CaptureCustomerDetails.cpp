//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CaptureCustomerDetails.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmCaptureCustomerDetails *frmCaptureCustomerDetails;
//---------------------------------------------------------------------------
__fastcall TfrmCaptureCustomerDetails::TfrmCaptureCustomerDetails(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::FormCreate(TObject *Sender)
{
}
//------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::FormShow(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edCustomerNameClick(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edAddressClick(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edTinClick(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edBusinessStyleClick(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::edSCPWDNOClick(TObject *Sender)
{
    //UnicodeString pwdNo = edSCPWDNO->Text;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::btnOKClick(TObject *Sender)
{
    SeniorCustomerDetails customerDetails;
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
