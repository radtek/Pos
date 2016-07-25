//---------------------------------------------------------------------------

#ifndef DealsSetupH
#define DealsSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ZForm.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "IDeal.h"
#include "DealUtilities.h"
#include <vector>
#include "IDealManager.h"
//---------------------------------------------------------------------------
class TfrmDeals : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *MainPanel;
	TPanel *SidePanel;
	TPanel *pnlLabel;
	TTouchGrid *tgridSelection;
	TTouchBtn *btnOk;
	TTouchBtn *btnAddDeal;
	TTouchBtn *btnEditDeal;
	TTouchBtn *btnDeleteDeal;
	void __fastcall btnOkMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnAddDealMouseClick(TObject *Sender);
	void __fastcall btnEditDealMouseClick(TObject *Sender);
	void __fastcall btnDeleteDealMouseClick(TObject *Sender);
	void __fastcall tgridSelectionMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
private:	// User declarations


	void addDeal();
	void editDeal();
	void deleteDeal();

	void updateDealGrid();

	Database::TDBControl &DBControl;

	IDealManager* dealManager;
	int selectedDealIndex;
public:		// User declarations
    __fastcall TfrmDeals(TComponent* Owner, Database::TDBControl &inDBControl);
	__fastcall ~TfrmDeals();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
