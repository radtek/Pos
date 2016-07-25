//---------------------------------------------------------------------------

#ifndef ChitListH
#define ChitListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "DBChitFwd.h"
#include "GlobalSettings.h"
//---------------------------------------------------------------------------
class TfrmChitList : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTouchGrid *ChitListGrid;
	TPanel *Panel2;
	TTouchBtn *BtnCancel;
	void __fastcall BtnCancelMouseClick(TObject *Sender);
	void __fastcall ChitListGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	UnicodeString chitName;
	chit_option_name_vector_t ChitList;
	void DisplayChits();
public:		// User declarations
	__property UnicodeString SelectedChitName = { read=chitName };
	__fastcall TfrmChitList(TComponent* Owner, chit_option_name_vector_t &ChitNames);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmChitList *frmChitList;
//---------------------------------------------------------------------------
#endif
