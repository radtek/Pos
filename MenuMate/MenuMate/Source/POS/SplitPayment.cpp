//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SplitPayment.h" 
#include "MMTouchNumpad.h"
#include "POSMain.h"
#include "Rounding.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmSplitPayment *frmSplitPayment;

#pragma warn -par

//---------------------------------------------------------------------------
__fastcall TfrmSplitPayment::TfrmSplitPayment(TComponent* Owner)
	: TZForm(Owner)
{
	Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::FormShow(TObject *Sender)
{
   btnDelayAll->Visible = IsDelayedPayment;
   DelayAll = false;
   NumberOfPayments = 1;
   if (DivisionsLeft == 0)
   {
	  DivisionsLeft = 1;
      btnRedivideClick(NULL);
   }
	lbeTotalDue->Caption = CurrToStrF(TotalDue, ffCurrency, CurrencyDecimals);
	CalculatePayment();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::btnNumberOfPaymentsClick(TObject *Sender)
{
	DivisionsLeft -= NumberOfPayments;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::TouchButton2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::TouchButton3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   NumberOfPayments++;
   if (NumberOfPayments > DivisionsLeft) NumberOfPayments = DivisionsLeft;
   CalculatePayment();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::TouchButton4MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   NumberOfPayments--;
   if (NumberOfPayments < 1) NumberOfPayments = 1;
   CalculatePayment();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::btnRedivideClick(TObject *Sender)
{
   std::auto_ptr<TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create<TfrmTouchNumpad>(this));
	frmTouchNumpad->Mode = pmNumber;
   frmTouchNumpad->Caption = "How many ways do you wish to split the bill?";
   frmTouchNumpad->btnSurcharge->Caption = "Ok";
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->btnSurcharge->Visible = true;
   frmTouchNumpad->INTInitial = DivisionsLeft;
	if (frmTouchNumpad->ShowModal() == mrOk)
   {
      if(TotalDivisions == 0)
      {
         TotalDivisions = frmTouchNumpad->INTResult;
      }
      else
      {
         TotalDivisions = (TotalDivisions - DivisionsLeft) + frmTouchNumpad->INTResult;
      }

      DivisionsLeft = frmTouchNumpad->INTResult;
      if (NumberOfPayments > DivisionsLeft)
      {
      	NumberOfPayments = DivisionsLeft;
      }
      CalculatePayment();
   }
   else
   {
		ModalResult = mrCancel;
		PostMessage(Handle, WM_CLOSE, 0, 0);
   }
}
//---------------------------------------------------------------------------
void TfrmSplitPayment::CalculatePayment()
{
   btnNumberOfPayments->Caption = "Pay for " + IntToStr(NumberOfPayments);
   lbeDivisionsLeft->Caption = IntToStr(DivisionsLeft);
   if (DivisionsLeft == 0)
   {
      PaymentAmount = 0;
   }
   else if (DivisionsLeft == NumberOfPayments)
   {
      PaymentAmount = TotalDue;
   }
   else
   {
      PaymentAmount = RoundToNearest(TotalDue * NumberOfPayments / DivisionsLeft, MIN_CURRENCY_VALUE, true);
   }
   stEnteredValue->Color = clHighlight;
   stEnteredValue->Font->Color = clHighlightText;
   stEnteredValue->Caption = CurrToStrF(PaymentAmount, ffCurrency, 2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::Panel6MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TPanel *Btn = (TPanel *)Sender;
	if (stEnteredValue->ParentColor)
   {
      if (long(double(PaymentAmount)) / 100000 != 0)
      {
         return;
      }
	  if (Btn->Caption == "00")
      {
         PaymentAmount = double(PaymentAmount) * 100;
         stEnteredValue->Caption = FloatToStrF(PaymentAmount, ffCurrency, 18, CurrencyDecimals);
      }
      else
      {
         PaymentAmount = double(PaymentAmount) * 10;
         PaymentAmount += double(StrToCurr(Btn->Caption) / 100);
         stEnteredValue->Caption = FloatToStrF(PaymentAmount, ffCurrency, 18, CurrencyDecimals);
      }
   }
   else
   {
   	if (Btn->Caption == "00")
      {
      	PaymentAmount = 0;
         stEnteredValue->Caption = FloatToStrF(PaymentAmount, ffCurrency, 18, CurrencyDecimals);
      }
      else
      {
      	PaymentAmount = StrToCurr(Btn->Caption) / 100;
      	stEnteredValue->Caption = FloatToStrF(PaymentAmount, ffCurrency, 18, CurrencyDecimals);
      }
      stEnteredValue->ParentColor = true;
      stEnteredValue->Font->Color = clBlack;
   }
}
//---------------------------------------------------------------------------


void __fastcall TfrmSplitPayment::lbeCLRMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	PaymentAmount = 0;
	stEnteredValue->Caption = FloatToStrF(PaymentAmount, ffCurrency, 18, CurrencyDecimals);
   stEnteredValue->ParentColor = true;
   stEnteredValue->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSplitPayment::btnDelayAllMouseClick(TObject *Sender)
{
  DelayAll = true;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
#pragma warn .par
