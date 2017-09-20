//---------------------------------------------------------------------------
#ifndef SelectTableH
#define SelectTableH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "enum.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <set>
#include "MezzanineDetails.h"

#include "MM_DBCore.h"
#include "ZForm.h"
//---------------------------------------------------------------------------

class TfrmSelectTable : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TTouchGrid *tgridTables;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall stCancelClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall tgridTablesMouseClick(TObject *Sender,
    TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
    TTimer *tiUpdateTableReq;
    void __fastcall tiUpdateTableReqTimer(TObject *Sender);
private:	// User declarations
//   bool TableScrollUp;
	__fastcall TfrmSelectTable(TComponent* Owner,Database::TDBControl &IBDatabase);
	Database::TDBControl &IBDatabase;
    void SetTablesColor(Database::TDBTransaction &DBTransaction);
    void SetColorAsPerNoServiceTime(TGridButton *GridButton,TDateTime OrderTime);
    void UpdateFloorPlanSheet();
    TfrmSelectTable *frmSelectTable;
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)

public:		// User declarations

   int SelectedTabContainerNumber;
   AnsiString SelectedTabContainerName;
   AnsiString SelectedPartyName;
   bool ShowAll;
   bool ChangingName;
   //For Mezzanine
   int TableMode;
   std::map<int, TMezzanineTable > MezzanineTables;
   std::set<int> AssignedMezzanineTable;
};
//---------------------------------------------------------------------------
class TTableSelector : public TObject
{
private:
public:

	int TableNo;
   AnsiString TableName;
   AnsiString PartyName;
};
//---------------------------------------------------------------------------
#endif
