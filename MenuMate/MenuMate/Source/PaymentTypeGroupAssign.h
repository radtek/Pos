//---------------------------------------------------------------------------

#ifndef PaymentTypeGroupAssignH
#define PaymentTypeGroupAssignH
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
#include <CheckLst.hpp>
#include "TouchGrid.h"
#include "PaymentGroupGUI.h"
//---------------------------------------------------------------------------
class TfrmAssign : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel9;
	TPanel *Panel5;
	TTouchBtn *btnClose;
	TPanel *Panel6;
	TTouchGrid *GroupList;
	TPanel *Panel12;
	TPanel *Panel7;
	TTouchGrid *MembersGrid;
	TTouchBtn *btnAll;
	TPanel *Panel1;
	TPanel *Panel4;
	TTouchGrid *GroupMembers;
	TTouchBtn *btnSumma;
	TPanel *Panel2;

void __fastcall btnCloseMouseClick(TObject *Sender);

private:	// User declarations
	static TForm *WinOwner;

public:		// User declarations
	static TfrmAssign *Create(TForm* Owner);

    __fastcall TfrmAssign(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAssign *frmAssign;
//---------------------------------------------------------------------------
#endif
