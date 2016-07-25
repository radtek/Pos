//---------------------------------------------------------------------------
#ifndef DealLevelsEditorH
#define DealLevelsEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Grids.hpp>
#include "IStoredDealLevel.h"
#include <memory>
#include "DealTypeDefs.h"
//---------------------------------------------------------------------------
class TfrmDealLevels : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TStringGrid *sgDisplay;
	TPanel *pnlLabel;
	TPanel *pnlMinor;
	TTouchBtn *tbtnOk;
	TTouchBtn *tbtnAdd;
	TTouchBtn *tbtnDelete;
	TTouchBtn *tbtnEdit;
	void __fastcall tbtnOkMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall sgDisplayDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall tbtnDeleteMouseClick(TObject *Sender);
	void __fastcall sgDisplaySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall tbtnEditMouseClick(TObject *Sender);
	void __fastcall tbtnAddMouseClick(TObject *Sender);

private:	// User declarations
	__fastcall TfrmDealLevels(TComponent* Owner, Database::TDBControl &inDBControl);

	Database::TDBControl &DBControl;

	void add(Currency initQty = 0, Currency initPrice = 0);
	void edit();
	void remove();
	void updateDisplay();
	StoredDealLevelSet::iterator getDealLevelFromSelection();
	Currency currencyTouchPad(UnicodeString inCaption, Currency inInitValue = 0);

	StoredDealLevelSet DealLevelsList;
public:		// User declarations
	StoredDealLevelSet GetDealLevels();

	void SetDealLevels (StoredDealLevelSet inDealLevels);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDealLevels *frmDealLevels;
//---------------------------------------------------------------------------
#endif
