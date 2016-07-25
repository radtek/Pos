//---------------------------------------------------------------------------

#ifndef PalmSetupH
#define PalmSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "TouchButton.h"
#include <ExtCtrls.hpp>
#include "TouchCheckBox.h"
//---------------------------------------------------------------------------
class TfrmPalmSetup : public TForm
{
__published:	// IDE-managed Components
	TPanel *GroupBox3;
	TTouchButton *btnModScan;
	TTouchButton *btnModClear;
	TTouchButton *btnModAutoProg;
	TTouchButton *btnModReset;
	TPanel *GroupBox6;
	TLabel *lbRetryCount;
	TLabel *lbIntervaltimeout;
	TLabel *lbTimeOut;
	TEdit *edRetry;
	TTouchButton *btnApplyTimeouts;
	TEdit *edInitailResponse;
	TEdit *edInterval;
	TLabel *Label2;
	TComboBox *cbReceiptPrinter;
	TLabel *Label1;
	TTouchButton *btnClose;
	TPanel *Panel1;
	TListBox *lbConnections;
	TTouchButton *TouchButton1;
   TTouchButton *TouchButton2;
	void __fastcall btnModScanClick(TObject *Sender);
	void __fastcall btnModClearClick(TObject *Sender);
	void __fastcall btnModAutoProgClick(TObject *Sender);
	void __fastcall btnModResetClick(TObject *Sender);
	void __fastcall btnApplyTimeoutsClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall cbReceiptPrinterChange(TObject *Sender);
	void __fastcall cbReceiptPrinterDropDown(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall TouchButton1Click(TObject *Sender);
   void __fastcall TouchButton2Click(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPalmSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPalmSetup *frmPalmSetup;
//---------------------------------------------------------------------------
#endif
