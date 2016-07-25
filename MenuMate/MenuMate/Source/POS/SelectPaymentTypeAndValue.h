//---------------------------------------------------------------------------

#ifndef SelectPaymentTypeAndValueH
#define SelectPaymentTypeAndValueH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchControls.h"
#include "TouchNumpad.h"
#include "TouchBtn.h"
//---------------------------------------------------------------------------
class TfrmSelectPaymentTypeAndValue : public TZForm
{
  friend TZForm;
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *pnlOptions;
        TPanel *Panel3;
        TPanel *Panel4;
        TNumpadDisplay *QtyDisplay;
        TTouchNumpad *TouchNumpad1;
        TTouchBtn *btnOk;
        TTouchBtn *btnCancel;
        TTouchBtn *btnPaymentType;
        void __fastcall btnOkMouseClick(TObject *Sender);
        void __fastcall btnPaymentTypeMouseClick(TObject *Sender);
        void __fastcall btnCancelMouseClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmSelectPaymentTypeAndValue(TComponent* Owner);
        AnsiString SelectedPayment;
        Currency NumericResult;
        Currency InitialValue;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectPaymentTypeAndValue *frmSelectPaymentTypeAndValue;
//---------------------------------------------------------------------------
#endif
