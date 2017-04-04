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
#include "SalesTypeItemDetails.h"

//---------------------------------------------------------------------------
class TfrmMallSalesTypeAssignment : public TZForm
{
  friend TZForm;
__published:	// IDE-managed Components
	TPanel *panel9;
	TPanel *panel5;
	TTouchBtn *btnOk;
	TPanel *panel6;
	TTouchGrid *salesTypeList;
	TPanel *panel12;
	TPanel *panel7;
	TPanel *panel1;
	TPanel *panel4;
	TTouchGrid *assignedItemsBySalesTypeList;
	TPanel *panel2;
    TTouchBtn *btnCancel;
    TTouchGrid *itemList;
    TTouchBtn *btnAssignAll;
    TTouchBtn *btnRemoveAll;
    TTouchBtn *btnAddSalesType;
    TTouchBtn *btnEditSalesType;
    TTouchBtn *btnDeleteSalesType;

    ///It will Display Sales Type, All Items  and all buttons..
	void __fastcall FormShow(TObject *Sender);

    //It will highlight selected sales Type and will call a function which will enable disable items and will display assigned items in assignedItemsBySalesTypeList.
    void __fastcall salesTypeListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);

    //On Clicking Item List it will assign selected items to slected sales type and selected item will be added to assignedItemsBySalesTypeList. and in
    //itemList that item will be in disable state.
    void __fastcall itemListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);

    //It will Remove selected item from salected sales type , call a function which will enable disable items and will remove items from assignedItemsBySalesTypeList.
    void __fastcall assignedItemsBySalesTypeListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton);

    //Ok Button will call DB Method for saving it will pass map which will have all items which are eiether added or removed.
    void __fastcall btnOkMouseClick(TObject *Sender);

    //it will cancel all changes..
    void __fastcall btnCancelMouseClick(TObject *Sender);

    ///For Resizing Form According to screen resolution..
    void __fastcall FormResize(TObject *Sender);

    //This will Assign all items to selected sales type and will show those items in  assignedItemsBySalesTypeList and in itemList all assigned items
    //will be in disabled state..
    void __fastcall btnAssignAllMouseClick(TObject *Sender);

    //This will Remove all items from selected sales type and will remove those items from  assignedItemsBySalesTypeList and in itemList all assigned items
    //will be in enabled state..
    void __fastcall btnRemoveAllMouseClick(TObject *Sender);

    //For Adding new sales type. it will open a new form which will ask for code and name
    void __fastcall btnAddSalesTypeMouseClick(TObject *Sender);

    //This will call same form for editing sales type. for this sales type must be selected..
    void __fastcall btnEditSalesTypeMouseClick(TObject *Sender);

    ///Delete Selected sales type..
    void __fastcall btnDeleteSalesTypeMouseClick(TObject *Sender);


private:	// User declarations
    void DisplayItems();
    void DisplaySalesTypes();
    int SelectedSalesType;
    void DisplayAssignedItemBySalesType();
    void EnableDisableItemList();  //todo
    void AssignAllItems();
    void RemoveAllItems();

    //Multimap which will have all items relation with sales type. map will be loaded from db. first int param will be sales type id and inner map's first
    //Parameter is for item Key and another is for item name..
    std::map<int, std::map<int, UnicodeString> > assignedItemsDBState;

    //Multimap which will have all items relation with sales type which are either newly added or newly removed..
    // first int param will be sales type id and inner map's first Parameter is for item Key and another structure which will have item name and status of type
    // enum. status will be eiether assigned or removed..
    std::map<int, std::map<int, TItemDetails> > assignedRemovedItemsBySalesType;

    //Multimap which will have all items relation with sales type. map will be loaded from db. first int param will be sales type id and inner map's first
    //Parameter is for item Key and another is for item name..
    std::map<int, std::map<int, UnicodeString> > currentItemRelationsWithSalesType;

    //set for storing items key which are used means assigned to any sales type..
    std::set<int> alreadyAssignedItems;

    //   Insert in to assignedRemovedItemsBySalesType map according to item status
    void InsertIntoAssignedRemovedItemsBySalesTypeMap(int itemId, UnicodeString itemName, int itemStatus);

public:		// User declarations
	__fastcall TfrmMallSalesTypeAssignment(TComponent* Owner);
    __fastcall ~TfrmMallSalesTypeAssignment();
};
//---------------------------------------------------------------------------
#endif
