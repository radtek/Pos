//---------------------------------------------------------------------------

#ifndef MallSalesTypeAssignmentH
#define MallSalesTypeAssignmentH
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
class TfrmMallSalesTypeAssignment : public TZForm
{
  friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel9;
	TPanel *Panel5;
	TTouchBtn *btnClose;
	TPanel *Panel6;
	TTouchGrid *GroupList;
	TPanel *Panel12;
	TPanel *Panel7;
	TPanel *Panel1;
	TPanel *Panel4;
	TTouchGrid *GroupMembers;
	TPanel *Panel2;
    TTouchBtn *btinGGA_Cancel;
    TTouchGrid *MembersGrid;
    TTouchBtn *btnAssignAll;
    TTouchBtn *btnRemoveAll;
    TTouchBtn *btnAddSalesType;
    TTouchBtn *btnEditSalesType;
    TTouchBtn *btnDeleteSalesType;
	void __fastcall FormShow(TObject *Sender);
    void __fastcall GroupListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
    void __fastcall MembersGridMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
          TGridButton *GridButton);
    void __fastcall GroupMembersMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);
    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall btinGGA_CancelMouseClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall btnAssignAllMouseClick(TObject *Sender);
    void __fastcall btnRemoveAllMouseClick(TObject *Sender);
    void __fastcall btnAddSalesTypeMouseClick(TObject *Sender);
    void __fastcall btnEditSalesTypeMouseClick(TObject *Sender);
    void __fastcall btnDeleteSalesTypeMouseClick(TObject *Sender);


private:	// User declarations
	    std::vector<ContactGroup> vectorGroup;
        std::vector<UnicodeString> vectorMembers;
        std::vector<ContactGroup> groupmembersVector;
        TGridButton *pGroupButton;
        void DisplayGroupMembers(void);
        void InsertMemberToGroup(UnicodeString pMemberButton);
        std::map<int, std::vector<int> > assignedItems;
        void DisplayItems();
        void DisplaySalesTypes();
        int SelectedGroup;

public:		// User declarations
	__fastcall TfrmMallSalesTypeAssignment(TComponent* Owner);
    __fastcall ~TfrmMallSalesTypeAssignment();


};
//---------------------------------------------------------------------------
#endif
