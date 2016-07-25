//---------------------------------------------------------------------------


#pragma hdrstop

#include "PaymentTypeGroupsGUI.h"
#include "PaymentTypeGroupAssignment.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TfrmPaymentTypesGroupsGUI::TfrmPaymentTypesGroupsGUI(TComponent* Owner)
    : TfrmGenericGroupGUI(Owner)
{
    GroupList.reset(new TList());
    Panel3->Caption = "Payment Groups";
}
//---------------------------------------------------------------------------
void TfrmPaymentTypesGroupsGUI::AddGroup()
{
 	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 25;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = true;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter Payment Group Name";

	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		if (frmTouchKeyboard->KeyboardText != "")
		{
            CheckGroupExist(frmTouchKeyboard->KeyboardText, GrpName);
            if (frmTouchKeyboard->KeyboardText != GrpName)
            {
                TPaymentTypeGroupsManager PaymentGroups;
                TPaymentTypeGroup group(0, frmTouchKeyboard->KeyboardText, false);
                PaymentGroups.AddPaymentTypeGroup(&group);
            }
		}
		else
		{
			MessageBox("A blank group is not allowed.", "Warning", MB_ICONWARNING + MB_OK);
		}
	}
	DisplayGroups();
}
//---------------------------------------------------------------------------
void TfrmPaymentTypesGroupsGUI::EditGroup()
{
    TPaymentTypeGroup* group = (TPaymentTypeGroup*) GroupList->Items[tgridGroups->Row];

 	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
 	frmTouchKeyboard->MaxLength = 25;
 	frmTouchKeyboard->AllowCarriageReturn = false;
 	frmTouchKeyboard->StartWithShiftDown = true;
 	frmTouchKeyboard->KeyboardText = group->Name;
 	frmTouchKeyboard->Caption = "Edit the name of this Group";

	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		if (frmTouchKeyboard->KeyboardText != "")
		{
            if(group->Name != frmTouchKeyboard->KeyboardText)
            {
                // no need to check for group exists or update the db if we dont change the name of an existing group
                CheckGroupExist(frmTouchKeyboard->KeyboardText, GrpName);

                if (frmTouchKeyboard->KeyboardText != GrpName)
                {
                    group->Name = frmTouchKeyboard->KeyboardText;
                    TPaymentTypeGroupsManager PaymentGroups;
                    PaymentGroups.UpdatePaymentTypeGroup(group);
                }
            }
        }
        else
        {
			MessageBox("A blank group is not allowed.", "Warning", MB_ICONWARNING + MB_OK);
        }
    }
	DisplayGroups();
}
//---------------------------------------------------------------------------
void TfrmPaymentTypesGroupsGUI::DeleteGroup()
{
    TPaymentTypeGroup* group = (TPaymentTypeGroup*) GroupList->Items[tgridGroups->Row];

    if (MessageBox("Are you sure you wish to delete this group?", "Warning", MB_ICONWARNING + MB_YESNO) == ID_YES)
	{
        TPaymentTypeGroupsManager PaymentGroups;
        PaymentGroups.DeletePaymentTypeGroup(group->DBKey);
	}
    DisplayGroups();
    ItemSelected(); // This functions is for enabling and disabling buttons depending if the item selected is a default item or not
}
//---------------------------------------------------------------------------
void TfrmPaymentTypesGroupsGUI::AssignGroups()
{
    std::auto_ptr <TfrmPaymentTypeGroupAssignment> PaymentTypeGroupAssignment(new TfrmPaymentTypeGroupAssignment(this));
    PaymentTypeGroupAssignment->ShowModal();
}
//---------------------------------------------------------------------------
void TfrmPaymentTypesGroupsGUI::DisplayGroups()
{
    ClearDisplay();
    TPaymentTypeGroupsManager PaymentGroups;
    PaymentGroups.LoadAllPaymentTypeGroups(GroupList.get());
    TPaymentTypeGroup* group;
    tgridGroups->RowCount = GroupList->Count;

	for (int i = 0; i < GroupList->Count; i++)
	{
        group = (TPaymentTypeGroup*) GroupList->Items[i];
		tgridGroups->Cols[0]->Add(group->Name);
	}
}
//---------------------------------------------------------------------------

void TfrmPaymentTypesGroupsGUI::ClearDisplay()
{
   tgridGroups->Cols[0]->Clear();
   GroupList->Clear();
}
//---------------------------------------------------------------------------

void TfrmPaymentTypesGroupsGUI::ItemSelected()
{
    TPaymentTypeGroup* group = (TPaymentTypeGroup*) GroupList->Items[tgridGroups->Row];
    tgridGroups->SetFocus();

    ButtonStatus(!group->IsDefault);
}
//---------------------------------------------------------------------------

void TfrmPaymentTypesGroupsGUI::CheckGroupExist(UnicodeString &FrmInput, UnicodeString &GrpName)
{
    TPaymentTypeGroup* itemGroup = (TPaymentTypeGroup*) GroupList->Items[0];
    for (int i = 0; i < GroupList->Count; i++)
    {
        itemGroup = (TPaymentTypeGroup*) GroupList->Items[i];
        GrpName = itemGroup->Name;
        if (FrmInput == GrpName)
        {
            MessageBox("Group already exists.", "Warning", MB_ICONWARNING + MB_OK);
            break;
        }
    }
}
//---------------------------------------------------------------------------
