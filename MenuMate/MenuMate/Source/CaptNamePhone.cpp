//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MMMessageBox.h"
#include "CaptNamePhone.h"
#include "MMTouchKeyboard.h"
#include "MMTouchNumpad.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchKeyboard"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmCaptNamePhone::TfrmCaptNamePhone(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptNamePhone::FormShow(TObject *Sender)
{
   edCustomerName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptNamePhone::btnOkMouseClick(TObject *Sender)
{
    if((edCustomerName->Text != ""))
    {
        ModalResult = mrOk;
    }
    else
    {
        if(MessageBox("Please Enter Customer Name", "Capture Customer Info", MB_OK + MB_ICONWARNING) == IDOK)
        {
           CapturedAllData();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptNamePhone::TouchBtn1MouseClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void TfrmCaptNamePhone::CapturedAllData()
{
  if(edCustomerName->Text == "")
  {
     edCustomerName->SetFocus();
  }
  else if(edCustomerPhone->Text == "")
  {
    edCustomerPhone->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptNamePhone::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if(Key == VK_RETURN)
  {
    CapturedAllData();
    btnOkMouseClick(btnOk);
  }
  else if(Key == VK_TAB)
  {
     edCustomerPhone->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmCaptNamePhone::TouchKeyboard1Click(TObject *Sender, char Char)
{
  TextBoxValue(Char);
  if(Char == '\t')
  {
    edCustomerPhone->SetFocus();
  }
  else if(Char == '\r')
  {
    btnOk->SetFocus();
    btnOkMouseClick(btnOk);
  }
}
//---------------------------------------------------------------------------
void TfrmCaptNamePhone::TextBoxValue(char valueChar)
{
  if(edCustomerName->Focused())
  {
    PopulateTextBox(valueChar, edCustomerName);
  }
  else if(edCustomerPhone->Focused())
  {
    PopulateTextBox(valueChar, edCustomerPhone);
  }
}

void TfrmCaptNamePhone::PopulateTextBox(char valueChar, TEdit *edTextbox)
{
   PostMessage(edTextbox->Handle, WM_CHAR, valueChar, 0);
   edTextbox->SetFocus();
}
