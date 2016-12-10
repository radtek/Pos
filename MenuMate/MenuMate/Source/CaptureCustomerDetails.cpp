//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CaptureCustomerDetails.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"
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
    FormResize(Sender);
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::FormResize(TObject *Sender)
{
	PanelDetails->Left = (Screen->Width - PanelDetails->Width) / 2;
	PanelDetails->Top  = (Screen->Height - PanelDetails->Height) / 2;
}
//--------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::btnOKClick(TObject *Sender)
{
    if (edCustomerName->Text.Trim()  == "")
    {
      MessageBox("You must enter Customer Name.", "Error", MB_OK + MB_ICONERROR);
    }
    else if(edAddress->Text.Trim()  == "" )
    {
        MessageBox("Enter Customer Address", "Error", MB_OK + MB_ICONERROR);
    }
    else if(edTIN->Text.Trim() == "")
    {
       MessageBox("You must enter Tin No.", "Error", MB_ICONERROR);
    }
    else if(edSCPWDNO->Text.Trim() == "")
    {
       MessageBox("You must enter SC/PWD ID.", "Error", MB_ICONERROR);
    }
    else
    {
        customerDetails.CustomerName = edCustomerName->Text;
        customerDetails.Address = edAddress->Text;
        customerDetails.TinNo = edTIN->Text;
        customerDetails.BusinessStyle = edBusinessStyle->Text;
        customerDetails.SC_PWD_ID = edSCPWDNO->Text;
        ModalResult = mrOk;
    }
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::btnCancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//----------------------------------------------------------------------------------------------
void __fastcall TfrmCaptureCustomerDetails::CaptureCustomerDetails(TObject *Sender)
{
    TTouchBtn *btn = reinterpret_cast<TTouchBtn*>(Sender);

    if(btn->Tag > 6)
        return;

    AnsiString Caption = "";
    AnsiString KeyboardText = "";
    std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
    frmTouchKeyboard->AllowCarriageReturn = false;
    frmTouchKeyboard->StartWithShiftDown = false;
    frmTouchKeyboard->MaxLength = 50;
    frmTouchKeyboard->AllowCarriageReturn = true;

     switch(btn->Tag)
    {
        case 0:
            Caption = "Enter Customer Name";
            frmTouchKeyboard->KeyboardText = edCustomerName->Text;
            frmTouchKeyboard->MaxLength = 75;
            break;
        case 1:
            Caption = "Enter Customer Address";
            frmTouchKeyboard->KeyboardText = edAddress->Text;
            frmTouchKeyboard->MaxLength = 200;
            break;
        case 2:
            Caption = "Enter Tin No";
            frmTouchKeyboard->KeyboardText = edTIN->Text;;
            break;
        case 3:
            Caption = "Enter Business Style";
            frmTouchKeyboard->KeyboardText = edBusinessStyle->Text;
            break;
        case 4:
            Caption = "Enter SCD/PWD ID#";
            frmTouchKeyboard->KeyboardText = edSCPWDNO->Text;
            break;
        default:
            break;
    }
    frmTouchKeyboard->Caption = Caption;
    if (frmTouchKeyboard->ShowModal() == mrOk)
    {
        const char* line = frmTouchKeyboard->KeyboardText.t_str();
        AnsiString keyBoardVal = "";
        for(int i = 0; line[i] != '\0'; i++)
        {
            if(line[i] == '\n')
            {
                keyBoardVal += ' ';
            }
            else if(line[i] != '\r')
            {
                keyBoardVal += char(line[i]);
            }
         }
         CustomerInfoPointers[btn->Tag] =  keyBoardVal;
         customerDetails.CustomerName =  CustomerInfoPointers[0].Trim();
         customerDetails.Address = CustomerInfoPointers[1].Trim();
         customerDetails.TinNo =  CustomerInfoPointers[2].Trim();
         customerDetails.BusinessStyle = CustomerInfoPointers[3].Trim();
         customerDetails.SC_PWD_ID =  CustomerInfoPointers[4];
		 DisplayCustomerData();
	}
}
//-------------------------------------------------------------------------------------------------
void TfrmCaptureCustomerDetails::DisplayCustomerData()
{
    edCustomerName->Text = CustomerInfoPointers[0].Trim();
    edAddress->Text = CustomerInfoPointers[1].Trim();
    edTIN->Text = CustomerInfoPointers[2].Trim();
    edBusinessStyle->Text = CustomerInfoPointers[3].Trim();
    edSCPWDNO->Text = CustomerInfoPointers[4];
}



