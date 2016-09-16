//---------------------------------------------------------------------------

#ifndef TierLevelsH
#define TierLevelsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "ZForm.h"
#include "ContactGroup.h"
#include "MM_DBCore.h"
#include <Grids.hpp>
//---------------------------------------------------------------------------
#define TIERNAME "TierName"

//---------------------------------------------------------------------------
class TfrmTierLevel : public TZForm
{
__published:	// IDE-managed Components
    TPanel *pnlMajor;
    TTouchGrid *tgridSelection;
    TPanel *pnlMinor;
    TTouchBtn *btnClose;
    TTouchBtn *btnAdd;
    TTouchBtn *btnEdit;
    TTouchBtn *btnDelete;
    TStaticText *lbeHeader;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall btnAddMouseClick(TObject *Sender);
    void __fastcall btnEditMouseClick(TObject *Sender);
    void __fastcall btnDeleteMouseClick(TObject *Sender);
    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall tgridSelectionMouseClick(TObject *Sender, TMouseButton Button,
                                             TShiftState Shift, TGridButton *GridButton);
private:	// User declarations
    Database::TDBControl &DBControl;
    void PopulateTierLevels();
    int SelectedTierKey;
    void DeleteTier();
public:		// User declarations
	__fastcall TfrmTierLevel(TComponent* Owner, Database::TDBControl &inDBControl);
        __fastcall ~TfrmTierLevel();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
