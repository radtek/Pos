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
#include "TouchButton.h"
//---------------------------------------------------------------------------
class TfrmPalmRegistration : public TForm
{
__published:	// IDE-managed Components
	TPanel *GroupBox4;
	TListBox *lbPalms;
	TTouchButton *btnAddPalm;
	TTouchButton *btnRemovePalm;
	TTimer *tmRegisterPalm;
	TTouchButton *btnClose;
	TTouchButton *btnUp;
	TTouchButton *btnDown;
   TTouchButton *tbtnReregister;
	void __fastcall btnAddPalmClick(TObject *Sender);
	void __fastcall btnRemovePalmClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tmRegisterPalmTimer(TObject *Sender);
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
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPalmRegistration(TComponent* Owner);
   AnsiString PalmDisplayName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPalmRegistration *frmPalmRegistration;
//---------------------------------------------------------------------------
#endif
