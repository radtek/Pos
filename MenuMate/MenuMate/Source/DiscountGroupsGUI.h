//---------------------------------------------------------------------------

#ifndef DiscountGroupsGUIH
#define DiscountGroupsGUIH

#include "GenericGroupGUI.h"
#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"

// #include "DiscountTypesManager.h"
#include "DiscountGroupsManager.h"
//---------------------------------------------------------------------------
class TfrmDiscountGroupsGUI : public TfrmGenericGroupGUI
{
public:

    std::auto_ptr<TList> GroupList;
    UnicodeString GrpName;

    __fastcall TfrmDiscountGroupsGUI(TComponent* Owner);
    void AddGroup();
    void EditGroup();
    void DeleteGroup();
    void AssignGroups();
    void DisplayGroups();
    void ItemSelected();

private:

    void ClearDisplay();
    void CheckGroupExist(UnicodeString &FrmInput, UnicodeString &GrpName);

};
#endif
