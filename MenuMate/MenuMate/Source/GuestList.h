//---------------------------------------------------------------------------

#ifndef GuestList
#define GuestList
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
#include "SiHotDataObjects.h"
//---------------------------------------------------------------------------
class TfrmGuestList : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTouchGrid *GuestListGrid;
	TPanel *Panel2;
	TTouchBtn *BtnCancel;
	void __fastcall BtnCancelMouseClick(TObject *Sender);
	void __fastcall GuestListGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	UnicodeString AccountNumber;
	void DisplayGuests();

public:		// User declarations
	__property UnicodeString SelectedAccountNumber = { read=AccountNumber };
	__fastcall TfrmGuestList(TComponent* Owner);
    std::vector<TSiHotAccounts> GuestAccounts;
    TSiHotAccounts selectedGuestDetails;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGuestList *frmGuestList;
//---------------------------------------------------------------------------
#endif

