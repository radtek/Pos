//---------------------------------------------------------------------------

#ifndef PaymentTypeGroupsGUIH
#define PaymentTypeGroupsGUIH

#include "GenericGroupGUI.h"
#include "MMTouchNumpad.h"
#include "MMTouchKeyboard.h"
#include "MMMessageBox.h"

#include "PaymentTypesManager.h"
#include "PaymentTypeGroupsManager.h"

//---------------------------------------------------------------------------
class TfrmPaymentTypesGroupsGUI : public TfrmGenericGroupGUI
{
public:

    std::auto_ptr<TList> GroupList;
    UnicodeString GrpName;

    __fastcall TfrmPaymentTypesGroupsGUI(TComponent* Owner);
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
