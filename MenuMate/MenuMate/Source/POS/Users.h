//---------------------------------------------------------------------------

#ifndef UsersH
#define UsersH
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
#include "GroupGUI.h"
//---------------------------------------------------------------------------
class TfrmUsers : public TZForm
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
	void __fastcall FormShow(TObject *Sender);
	void __fastcall GroupListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton, int X, int Y);
	void __fastcall MembersGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall GroupMembersMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
	void __fastcall btnAllMouseClick(TObject *Sender);
	void __fastcall btnCloseMouseClick(TObject *Sender);
	void __fastcall AllSummaMouseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);


private:	// User declarations
	Database::TDBControl &DBControl;
        void DisplayGroups(void);
	void DisplayMembers(bool statechanged);
	std::vector<ContactGroup> vectorGroup;
        std::vector<UnicodeString> vectorMembers;
        std::vector<ContactGroup> groupmembersVector;
        TGridButton *pGroupButton;
        void DisplayGroupMembers(void);
        void InsertMemberToGroup(UnicodeString pMemberButton);

public:		// User declarations
	__fastcall TfrmUsers(TComponent* Owner, Database::TDBControl &inDBControl);

};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
