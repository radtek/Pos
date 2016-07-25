//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectPaymentTypeAndValue.h"
#include "Message.h"
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchNumpad"
#pragma link "TouchBtn"
#pragma resource "*.dfm"
//TfrmSelectPaymentTypeAndValue *frmSelectPaymentTypeAndValue;
//---------------------------------------------------------------------------
__fastcall TfrmSelectPaymentTypeAndValue::TfrmSelectPaymentTypeAndValue(TComponent* Owner)
        : TZForm(Owner)
{
  SelectedPayment = "";
  NumericResult = 0;
  InitialValue = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectPaymentTypeAndValue::btnOkMouseClick(TObject *Sender)
{
  NumericResult = QtyDisplay->Numeric();
  SelectedPayment = "";
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------



void __fastcall TfrmSelectPaymentTypeAndValue::btnPaymentTypeMouseClick(TObject *Sender)

{
  std::auto_ptr <TfrmMessage> SelectPaymentType(TfrmMessage::Create <TfrmMessage> (this, TDeviceRealTerminal::Instance().DBControl));
  SelectPaymentType->MessageType = ePaymentTypeDisplay;
  if (SelectPaymentType->ShowModal() == mrOk)
   {
      SelectedPayment = SelectPaymentType->TextResult;
      if(SelectedPayment == "")
      {
        btnPaymentType->Caption = "Payment Type";
      }
      else
      {
        btnPaymentType->Caption = SelectedPayment;
        ModalResult = mrOk;
      }

   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectPaymentTypeAndValue::btnCancelMouseClick(TObject *Sender)

{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectPaymentTypeAndValue::FormShow(TObject *Sender)
{
  QtyDisplay->SetNumeric(InitialValue);
  if(SelectedPayment != "")
  {
    btnPaymentType->Caption = SelectedPayment;
  }
}
//---------------------------------------------------------------------------

