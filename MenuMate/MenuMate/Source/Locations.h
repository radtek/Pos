//---------------------------------------------------------------------------

#ifndef LocationsH
#define LocationsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "MM_DBCore.h"
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmLocations : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
   TPanel *pnl1;
   TLabel *Label17;
   TPanel *pnl2;
	TTouchBtn *tbtnAddLoc;
	TTouchBtn *TouchBtn2;
	TTouchBtn *tbtnDelete;
	TStringGrid *List;
	TTouchBtn *btnUp;
	TTouchBtn *btnDown;
	TTouchBtn *TouchBtn1;
   void __fastcall AddClick(TObject *Sender);
   void __fastcall EditLocClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ExitClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
	void __fastcall btnUpMouseClick(TObject *Sender);
	void __fastcall btnDownMouseClick(TObject *Sender);
	void __fastcall TbtnDeleteClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmLocations(TComponent* Owner,Database::TDBControl &inIBDatabase);
	Database::TDBControl &IBDatabase;
	void RefreshList();
public:		// User declarations

   UnicodeString SelectedLocation;
   int SelectedLocationKey;
};

#endif
