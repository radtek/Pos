//---------------------------------------------------------------------------

#ifndef TipAmountH
#define TipAmountH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchNumpad.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
class TfrmTipAmount : public TZForm
{
    friend TZForm;
__published:	// IDE-managed Components
    TPanel *Panel1;
    TNumpadDisplay *lbTipDisplay;
    TTouchNumpad *numTipAmount;
    TTouchBtn *btnProcessTipOnReceipt;
    TTouchBtn *btnCancelTipping;
    void __fastcall btnCancelTippingMouseClick(TObject *Sender);
    void __fastcall btnProcessTipOnReceiptMouseClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

private:	// User declarations


public:		// User declarations
    __fastcall TfrmTipAmount(TComponent* Owner);
    double SelectedTipAmount;
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
