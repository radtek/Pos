//---------------------------------------------------------------------------
#ifndef SplitPaymentH
#define SplitPaymentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmSplitPayment : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel3;
	TPanel *Panel6;
	TPanel *Panel10;
	TPanel *Panel15;
	TPanel *Panel20;
	TPanel *Panel21;
	TPanel *Panel22;
	TPanel *Panel23;
	TPanel *Panel24;
	TPanel *Panel25;
	TPanel *Panel26;
	TPanel *Panel27;
	TPanel *lbeCLR;
	TPanel *Panel5;
	TLabel *stEnteredValue;
	TPanel *Panel1;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *lbeTotalDue;
	TLabel *lbeDivisionsLeft;
	TTouchBtn *btnRedivide;
	TTouchBtn *btnNumberOfPayments;
	TTouchBtn *TouchButton3;
	TTouchBtn *TouchButton4;
	TTouchBtn *tbtnCancel;
        TTouchBtn *btnDelayAll;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnNumberOfPaymentsClick(TObject *Sender);
	void __fastcall TouchButton2Click(TObject *Sender);
	void __fastcall TouchButton3MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TouchButton4MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnRedivideClick(TObject *Sender);
	void __fastcall Panel6MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall lbeCLRMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall btnDelayAllMouseClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmSplitPayment(TComponent* Owner);
	void CalculatePayment();
public:		// User declarations

   int TabKey;
   Currency TotalDue;
//   int DivideBy;
   int DivisionsLeft;
   int TotalDivisions;
   int NumberOfPayments;
   Currency PaymentAmount;
   bool IsDelayedPayment;
   bool DelayAll;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSplitPayment *frmSplitPayment;
//---------------------------------------------------------------------------
#endif
