//---------------------------------------------------------------------------

#ifndef PocketVoucherH
#define PocketVoucherH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ZForm.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmPocketVoucher : public TZForm
{
__published:	// IDE-managed Components
	TPanel *Panel8;
	TPanel *Panel1;
	TPanel *Panel5;
	TPanel *Panel6;
	TPanel *Panel4;
	TPanel *pnlLable;
	TPanel *Panel2;
	//TTouchBtn *tbtnVoucherCode;
	TEdit *edVoucherCode;
    TTouchBtn *tbtnOk;
    TTouchBtn *tBtnCancel;
	//void __fastcall tbtnVoucherCodeMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall edVoucherCodeMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbtnOkMouseClick(TObject *Sender);
	void __fastcall tBtnCancelMouseClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
   AnsiString FixedVoucher;
   void CapturedAllData();
public:		// User declarations
	__fastcall TfrmPocketVoucher(TComponent* Owner);
   AnsiString VoucherCode();
};
//---------------------------------------------------------------------------
#endif
