// ---------------------------------------------------------------------------
#ifndef MMTouchNumpadH
#define MMTouchNumpadH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

// ---------------------------------------------------------------------------
enum TPadMode
{
	pmCurrency, pmNumber, pmPIN, pmDecimal, pmSTR, pmWeight
};

// ---------------------------------------------------------------------------
class TfrmTouchNumpad : public TZForm
{
	friend TZForm;
__published: // IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel11;
	TPanel *pnlOptions;
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
	TTouchBtn *btnSurcharge;
	TTouchBtn *btnDiscount;
	TTouchBtn *btnCancel;
	TTouchBtn *btnOk;

	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnNumberMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnCLRMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnDiscountClick(TObject *Sender);
	void __fastcall btnSurchargeClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall btnOkMouseClick(TObject *Sender);

protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)END_MESSAGE_MAP(TComponent)

private: // User declarations
	__fastcall TfrmTouchNumpad(TComponent* Owner);

	Currency wrkPayAmount;
	int wrkIntAmount;
	UnicodeString wrkStrAmount;
    UnicodeString wrkNumStrAmount;

    int MaxLength;

public: // User declarations

	Currency CURResult;
	int INTResult;
	UnicodeString STRResult;
    UnicodeString NUMSTRResult;

	Currency CURInitial;
	int INTInitial;
	UnicodeString StrInitial;
    UnicodeString NUMSTRInitial;
	bool ForcedNegitive;
	bool PreSelect;
	int BtnExit;
	TPadMode Mode;
	TFloatFormat Format;
    void SetMaxLengthValue(int Length);

	void __fastcall UpdateDisplay();
};
// ---------------------------------------------------------------------------
#endif
