//---------------------------------------------------------------------------
#ifndef PatronCountH
#define PatronCountH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include "ManagerPatron.h"
#include "TouchBtn.h"
#include "MM_DBCore.h"
#include "ZForm.h"
//---------------------------------------------------------------------------

class TfrmPatronCount : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel11;
	TPanel *pnlOptions;
	TPanel *Panel3;
	TPanel *Panel6;
	TPanel *Panel10;
	TPanel *Panel15;
	TPanel *Panel20;
	TPanel *Panel21;
	TPanel *Panel22;
	TPanel *Panel23;
	TPanel *pnl2;
	TPanel *pnl3;
	TPanel *Panel26;
	TPanel *pnl00;
	TPanel *btnCLR;
	TPanel *Panel1;
	TLabel *lbeEnteredValue;
	TPanel *Panel5;
	TTouchBtn *btnOk;
	TPanel *Panel4;
	TPanel *Panel7;
	TTouchGrid *btnPartonTypes;
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnNumberMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnCLRMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnPartonTypesMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall btnOkMouseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmPatronCount(TComponent* Owner,Database::TDBControl &inDBControl);
	
	int	 	wrkIntAmount;
	Database::TDBControl &DBControl;
public:		// User declarations


	std::vector<TPatronType> Patrons;
	std::vector<TPatronType> InitPatrons;

	TGridButton *CurrentButton;
};
//---------------------------------------------------------------------------
#endif
