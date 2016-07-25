//---------------------------------------------------------------------------

#ifndef GroupGUIH
#define GroupGUIH
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
enum {GROUPNAME, GROUPNUMBER};
//---------------------------------------------------------------------------

class TfrmGroupGUI : public TZForm
{
__published:	// IDE-managed Components
	TPanel *pnlMajor;
	TPanel *pnlGroups;
	TStringGrid *tgridGroups;
	TPanel *pnlMinor;
	TTouchBtn *btnClose;
	TTouchBtn *btnAdd;
	TTouchBtn *btnEdit;
	TTouchBtn *btnDelete;
	TTouchBtn *btnUsers;
	TPanel *Panel3;
        void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnAddMouseClick(TObject *Sender);
	void __fastcall btnEditMouseClick(TObject *Sender);
        void __fastcall btnUsersMouseClick(TObject *Sender);
	void __fastcall btnDeleteMouseClick(TObject *Sender);
	void __fastcall btnCloseMouseClick(TObject *Sender);



private:	// User declarations
	Database::TDBControl &DBControl;
        void DisplayGroups(void);
        int DisplayNumpad(void);
  //	Database::TDBControl &IBDatabase;
public:		// User declarations
	__fastcall TfrmGroupGUI(TComponent* Owner, Database::TDBControl &inDBControl);
        __fastcall ~TfrmGroupGUI();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
