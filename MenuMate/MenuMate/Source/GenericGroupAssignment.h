//---------------------------------------------------------------------------

#ifndef GenericGroupAssignmentH
#define GenericGroupAssignmentH
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
class TfrmGenericGroupAssignment : public TZForm
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


private:	// User declarations
	    std::vector<ContactGroup> vectorGroup;
        std::vector<UnicodeString> vectorMembers;
        std::vector<ContactGroup> groupmembersVector;
        TGridButton *pGroupButton;
        void DisplayGroupMembers(void);
        void InsertMemberToGroup(UnicodeString pMemberButton);

public:		// User declarations
	__fastcall TfrmGenericGroupAssignment(TComponent* Owner);
    __fastcall ~TfrmGenericGroupAssignment();
    virtual void DisplayGroups(){};
    virtual void DisplayTypes(){};
    virtual void DisplayItemsInGroups(int GroupKey){};
    virtual void SaveAssignment(){};
    virtual void AddToGroup(int selectedGroupKey, int TypeKey){};
    virtual void GroupSelected(TGridButton* GridButton){};
    virtual void TypeSelected(TGridButton* GridButton){};
    virtual void RemoveTypefromGroup(int TypeKey){};
    virtual void CleanUpMemory(){};
    virtual void AssignAllItems(){};
    virtual void RemoveAllItems(){};
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
