//---------------------------------------------------------------------------

#ifndef SetupGlCodesH
#define SetupGlCodesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmSetupGlCodes : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TTouchBtn *btnPointPurchased;
    TTouchBtn *btnPointSpent;
    TTouchBtn *btnVoucherPurchased;
    TTouchBtn *btnServiceCharge;
    TTouchBtn *btnServiceChargeTax;
    TTouchBtn *btnOk;
    TTouchBtn *btnCancel;
    TTouchBtn *btnRounding;
    TTouchBtn *btnTabAmountReceived;
    TTouchBtn *btnTabAmountRefunded;
    TTouchBtn *btnFloatGlCode;
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall btnPointPurchasedMouseClick(TObject *Sender);
    void __fastcall btnPointSpentMouseClick(TObject *Sender);
    void __fastcall btnVoucherPurchasedMouseClick(TObject *Sender);
    void __fastcall btnServiceChargeMouseClick(TObject *Sender);
    void __fastcall btnServiceChargeTaxMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnRoundingMouseClick(TObject *Sender);
    void __fastcall btnTabAmountReceivedClick(TObject *Sender);
    void __fastcall btnTabAmountRefundedClick(TObject *Sender);
    void __fastcall btnFloatGlCodeClick(TObject *Sender);

private:	// User declarations
   void ShowKeyBoard(AnsiString KeyBoardCaption,AnsiString& KeyBoardText);
public:		// User declarations
    __fastcall TfrmSetupGlCodes(TComponent* Owner);
    AnsiString PointsPurchased;
    AnsiString PointsSpent;
    AnsiString VoucherPurchased;
    AnsiString ServiceCharge;
    AnsiString ServiceChargeTax;
    AnsiString Rounding;
    AnsiString TabDepositCreditReceived;
    AnsiString TabDepositCreditRefunded;
    AnsiString FloatGLCode;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSetupGlCodes *frmSetupGlCodes;
//---------------------------------------------------------------------------
#endif

