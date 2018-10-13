//---------------------------------------------------------------------------

#ifndef SystemConfigH
#define SystemConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"

#include "enum.h"
#include <ExtCtrls.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include "touchgrid.h"
#include "MMSettings.h"

//---------------------------------------------------------------------------
class TfrmSystemConfig : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel3;
	TPanel *pButtons;
	TTouchGrid *tgridButtons;
	TPanel *tpnlBtns;
	TTouchBtn *tbtnOk;
	TTouchBtn *tbtnQuit;
   void __fastcall btnExit(TObject *Sender);
   void __fastcall tbOkClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall tgridButtonsMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);
private:	// User declarations
	__fastcall TfrmSystemConfig(TComponent* Owner);

	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);

public:		// User declarations
   static TfrmSystemConfig *Create(TForm* Owner);

   bool RequestClose;
   void EnableMenuMate();
   void EnableMembership();
   void EnableGeneralLedgerMembership();
   void EnableStock();
};

//---------------------------------------------------------------------------
#endif
