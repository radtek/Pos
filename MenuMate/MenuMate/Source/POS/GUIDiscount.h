//---------------------------------------------------------------------------
#ifndef GUIDiscountH
#define GUIDiscountH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "Discount.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
enum TAdjustmentValueType {avtNone,avtDiscount,avtSurcharge,avtPoints};
//---------------------------------------------------------------------------

class TfrmDiscount : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel11;
	TPanel *pnlOptions;
	TPanel *btnCancel;
	TPanel *pnlOk;
	TPanel *pnlToggle;
	TPanel *Panel3;
	TPanel *Panel6;
	TPanel *Panel10;
	TPanel *Panel15;
	TPanel *Panel20;
	TPanel *Panel21;
	TPanel *Panel22;
	TPanel *Panel23;
	TPanel *pnl2;
	TPanel *pnl3;
	TPanel *Panel26;
	TPanel *pnl00;
	TPanel *btnCLR;
	TPanel *Panel1;
	TLabel *lbeEnteredValue;
	TPanel *Panel5;
	TTouchBtn *tbToggleAmount;
	void __fastcall NumberMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnCLRMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall pnlOkClick(TObject *Sender);
	void __fastcall pnlToggleClick(TObject *Sender);
	void __fastcall tbToggleAmountClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
   __fastcall TfrmDiscount(TComponent* Owner);
   Currency wrkCurAmount;
   double wrkPercAmount;
   bool PercentDecimal;
   TAdjustmentValueType ForcedType;
   void HandlePaymnetSurcharge();
   void HandleNormalDiscount();
public:		// User declarations
   TDiscountMode Mode;
   Currency CURInitial;
   double PERCInitial;
   Currency TotalValue;		// To calculate percentage
   Currency CURResult;
   double PERCResult;
   TAdjustmentValueType ValueType;
   TAdjustmentValueType ValueTypeBeforeSetPrice;
   void ForceType(TAdjustmentValueType inValueType);
   bool IsNewDiscount;
   bool IsComboDiscount;
   bool IsPaymentSurcharge;
};
//---------------------------------------------------------------------------
#endif
