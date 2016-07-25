//---------------------------------------------------------------------------

#ifndef SystemConfigLiteH
#define SystemConfigLiteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <TouchBtn.h>
#include "TouchControls.h"

//---------------------------------------------------------------------------
class TfrmSystemConfig : public TForm
{
__published:	// IDE-managed Components
   TPanel *pButtons;
   TTouchBtn *tbOk;
   TTouchBtn *tbInterbase;
   TTouchBtn *tbDBLocation;
   TPanel *Panel1;
	TTouchBtn *tbtnExit;
	TLabel *Label8;
	TComboBox *cbBarCodePort;
	TCheckBox *cbAutoCloseWhenBumped;
	TTouchBtn *tbUnsSannedWarning;
	TLabel *Label1;
   TCheckBox *cbRemovedWhenScanned;
   TCheckBox *cbDisplayDynamicOrders;
   TCheckBox *cbConfirmBeforeBumping;
   TCheckBox *cbRemovedWhenClosed;
	TTouchBtn *TouchBtn1;
   void __fastcall tbInterbaseClick(TObject *Sender);
   void __fastcall tbtnExitClick(TObject *Sender);
   void __fastcall tbOkClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall tbDBLocationClick(TObject *Sender);
   void __fastcall cbBarCodePortChange(TObject *Sender);
   void __fastcall cbAutoCloseWhenBumpedClick(TObject *Sender);
   void __fastcall tbUnsSannedWarningClick(TObject *Sender);
   void __fastcall cbRemovedWhenScannedClick(TObject *Sender);
   void __fastcall cbDisplayDynamicOrdersClick(TObject *Sender);
   void __fastcall cbConfirmBeforeBumpingClick(TObject *Sender);
   void __fastcall cbRemovedWhenClosedClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmSystemConfig(TComponent* Owner);
   bool RequestClose;
   int MinutesWarning;   
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
