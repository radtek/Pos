//---------------------------------------------------------------------------


#pragma hdrstop

#include "DiscountGroupsGUI.h"
#include "DiscountGroupsAssignment.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

__fastcall TfrmDiscountGroupsGUI::TfrmDiscountGroupsGUI(TComponent* Owner)
    : TfrmGenericGroupGUI(Owner)
{
    GroupList.reset(new TList());
    Panel3->Caption = "Discount Groups";
}
//---------------------------------------------------------------------------
void TfrmDiscountGroupsGUI::AddGroup()
{
 	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 25;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = true;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = "Enter Discount Group Name";

	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		if (frmTouchKeyboard->KeyboardText != "")
		{
            CheckGroupExist(frmTouchKeyboard->KeyboardText, GrpName);
            if (frmTouchKeyboard->KeyboardText != GrpName)
            {
                TDiscountGroupsManager DiscountGroups;
                TDiscountGroup group(0, frmTouchKeyboard->KeyboardText, false);
                DiscountGroups.AddDiscountGroups(&group);
            }
		}
		else
		{
			MessageBox("A blank group is not allowed.", "Warning", MB_ICONWARNING + MB_OK);
		}
	}
	DisplayGroups();
};
//---------------------------------------------------------------------------
void TfrmDiscountGroupsGUI::EditGroup()
{
    TDiscountGroup* group = (TDiscountGroup*) GroupList->Items[tgridGroups->Row];

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
                    TDiscountGroupsManager DiscountGroups;
                    DiscountGroups.UpdateDiscountGroups(group);
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
void TfrmDiscountGroupsGUI::DeleteGroup()
{
    TDiscountGroup* group = (TDiscountGroup*) GroupList->Items[tgridGroups->Row];

    if (MessageBox("Are you sure you wish to delete this group?", "Warning", MB_ICONWARNING + MB_YESNO) == ID_YES)
	{
        TDiscountGroupsManager DiscountGroups;
        DiscountGroups.DeleteDiscountGroups(group->DBKey);
	}
    DisplayGroups();
    ItemSelected(); // This functions is for enabling and disabling buttons depending if the item selected is a default item or not
}
//---------------------------------------------------------------------------
void TfrmDiscountGroupsGUI::AssignGroups()
{
    std::auto_ptr <TfrmDiscountGroupsAssignment> DiscountGroupsAssignment(new TfrmDiscountGroupsAssignment(this));
    DiscountGroupsAssignment->ShowModal();
};
//---------------------------------------------------------------------------
void TfrmDiscountGroupsGUI::DisplayGroups()
{
    ClearDisplay();
    TDiscountGroupsManager DiscountGroups;
    DiscountGroups.LoadAllDiscountGroups(GroupList.get());
    TDiscountGroup* group;
    tgridGroups->RowCount = GroupList->Count;

	for (int i = 0; i < GroupList->Count; i++)
	{
        group = (TDiscountGroup*) GroupList->Items[i];
		tgridGroups->Cols[0]->Add(group->Name);
	}
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsGUI::ClearDisplay()
{;
   tgridGroups->Cols[0]->Clear();
   GroupList->Clear();
};
//---------------------------------------------------------------------------

void TfrmDiscountGroupsGUI::ItemSelected()
{
    TDiscountGroup* group = (TDiscountGroup*) GroupList->Items[tgridGroups->Row];
    tgridGroups->SetFocus();

    ButtonStatus(!group->IsDefault);
}
//---------------------------------------------------------------------------

void TfrmDiscountGroupsGUI::CheckGroupExist(UnicodeString &FrmInput, UnicodeString &GrpName)
{
    TDiscountGroup* itemGroup = (TDiscountGroup*) GroupList->Items[0];
    for (int i = 0; i < GroupList->Count; i++)
    {
        itemGroup = (TDiscountGroup*) GroupList->Items[i];
        GrpName = itemGroup->Name;
        if (FrmInput == GrpName)
        {
            MessageBox("Group already exists.", "Warning", MB_ICONWARNING + MB_OK);
            break;
        }
    }
};
//---------------------------------------------------------------------------
