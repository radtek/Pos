//---------------------------------------------------------------------------

#ifndef PalmRegistrationH
#define PalmRegistrationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmPalmRegistration : public TForm
{
__published:	// IDE-managed Components
	TPanel *GroupBox4;
	TListBox *lbPalms;
	TTouchBtn *btnRemovePalm;
	TTouchBtn *btnClose;
	TTouchBtn *btnUp;
	TTouchBtn *btnDown;
	TTouchBtn *tbtnReregister;
	void __fastcall btnRemovePalmClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnDownMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbtnReregisterClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:
public:		// User declarations
	void UpdateDisplay();	// User declarations
	__fastcall TfrmPalmRegistration(TComponent* Owner);
	UnicodeString PalmName;
	int LastRegSyncNumber;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPalmRegistration *frmPalmRegistration;
//---------------------------------------------------------------------------
#endif
