//---------------------------------------------------------------------------
#ifndef MMMessageBoxH
#define MMMessageBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------

#define MB_ACCEPTDECLINE 0x0F
class TfrmMessageBox : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TImage *imgHand;
	TLabel *lbeText;
	TPanel *pnlOkCancel;
	TTouchBtn *btnOCOk;
	TTouchBtn *btnOCCancel;
	TBevel *Bevel1;
	TImage *imgQuestion;
	TImage *imgExclamation;
	TPanel *pnlOk;
	TTouchBtn *btnOOk;
	TPanel *pnlYesNo;
	TTouchBtn *btnYNYes;
	TTouchBtn *btnYNNo;
	TPanel *pnlAbortRetryIgnore;
	TTouchBtn *btnARIRetry;
	TTouchBtn *btnARIAbort;
	TTouchBtn *btnARIIgnore;
	TPanel *pnlRetryCancel;
	TTouchBtn *btnRCRetry;
	TTouchBtn *btnRCCancel;
	TPanel *pnlYesNoCancel;
	TTouchBtn *btnYNCYes;
	TTouchBtn *btnYNCNo;
	TTouchBtn *btnYNCCancel;
	TPanel *pnlAcceptDecline;
	TTouchBtn *tbtnAccepted;
	TTouchBtn *tbtnDeclined;
	TPanel *pnlCustom;
	TTouchBtn *tbtnAffim;
	TTouchBtn *tbtnCancel;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnYesClick(TObject *Sender);
	void __fastcall btnNoClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnIgnoreClick(TObject *Sender);
	void __fastcall btnRetryClick(TObject *Sender);
	void __fastcall btnAbortClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbtnAcceptedClick(TObject *Sender);
	void __fastcall tbtnDeclinedClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
	__fastcall TfrmMessageBox(TComponent* Owner);
public:		// User declarations

	int Flags;	
   int RetVal;
};

int MessageBox(UnicodeString Text, UnicodeString Caption, int Flags);
int CustomMessageBox(UnicodeString Text, UnicodeString Caption, int Flags,UnicodeString AffimCaption,UnicodeString CancelCaption);
//---------------------------------------------------------------------------
#endif
