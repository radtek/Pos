//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Users.h"
#include "DBGroups.h"
#include "Contact.h"
#include "DBContacts.h"
#include "MMMessageBox.h"
#include "MMContactInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmUsers::TfrmUsers(TComponent* Owner, Database::TDBControl &inDBControl)
	: TZForm(Owner), DBControl(inDBControl)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmUsers::FormShow(TObject *Sender)
{
DisplayGroups();
DisplayMembers(false);
     	MembersGrid->Enabled = false;
        GroupMembers->Enabled = false;
}
//---------------------------------------------------------------------------
void TfrmUsers::DisplayGroups(void)
{

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;

  //      DBGroups.SortGroups(DBTransaction);

        DBGroups.GetGroupsList(DBTransaction, vectorGroup);
        GroupList->RowCount = vectorGroup.size();
	DBTransaction.Commit();
        for (int i = 0; i < vectorGroup.size() ; i++)
	{
        	GroupList->Buttons[i][0]->Caption = vectorGroup.at(i).Name;
      	 	GroupList->Buttons[i][1]->Caption = vectorGroup.at(i).Type;
      	 	GroupList->Buttons[i][0]->Tag = vectorGroup.at(i).Key;
                GroupList->Buttons[i][1]->Enabled = false;
	}

}
//-------------------------------------------------------------------------

void TfrmUsers::DisplayMembers(bool statechanged)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TContact Contact;



if(statechanged)
{
	for(int i = 0; i < vectorMembers.size(); i++)
        {
		MembersGrid->Buttons[i][0]->Enabled = true;
        }
        vectorMembers.clear();


}


        Contact.GetContactNameList(DBTransaction, vectorMembers, 2);
        MembersGrid->RowCount = vectorMembers.size();
	DBTransaction.Commit();


        for(int i = 0; i < vectorMembers.size(); i++)
        {
		MembersGrid->Buttons[i][0]->Caption = vectorMembers.at(i);
                for(int j = 0; j < groupmembersVector.size(); j++)
                {

                	if(vectorMembers.at(i) == groupmembersVector.at(j).Name)
                        {

                        	MembersGrid->Buttons[i][0]->Enabled = false;
                        }
                }

        }

}
//---------------------------------------------------------------------------

void __fastcall TfrmUsers::GroupListMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TDBGroups DBGroups;
        TContact Contact;
        GroupMembers->Enabled = true;
        Contact.Enabled = true;
        MembersGrid->Enabled = true;
        GridButton->LatchedColor = clNavy;
        GridButton->LatchedFontColor = clWhite;
        btnAll->Enabled = true;

        groupmembersVector.clear();

        pGroupButton = GridButton;

        DBGroups.GetContacts_ContactsKeys(DBTransaction, GridButton->Tag, groupmembersVector);
        GroupMembers->RowCount = groupmembersVector.size();

        for(int i = 0; i < groupmembersVector.size(); i++)
        {
        	groupmembersVector.at(i).Name = Contact.GetContactName(DBTransaction, groupmembersVector.at(i).Key);
        	GroupMembers->Buttons[i][0]->Caption =  groupmembersVector.at(i).Name;
        }

        DBTransaction.Commit();
        DisplayMembers(true);
}
//---------------------------------------------------------------------------

void TfrmUsers::DisplayGroupMembers(void)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TDBGroups DBGroups;
        TContact Contact;
        Contact.Enabled = true;

        MembersGrid->Enabled = true;

        groupmembersVector.clear();

        //GroupButton = GridButton;

        DBGroups.GetContacts_ContactsKeys(DBTransaction, pGroupButton->Tag, groupmembersVector);
        GroupMembers->RowCount = groupmembersVector.size();


        for(int i = 0; i < groupmembersVector.size(); i++)
        {
        	groupmembersVector.at(i).Name = Contact.GetContactName(DBTransaction, groupmembersVector.at(i).Key);
        	GroupMembers->Buttons[i][0]->Caption =  groupmembersVector.at(i).Name;
        }
        DBTransaction.Commit();


}
//---------------------------------------------------------------------------

void __fastcall TfrmUsers::MembersGridMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{
	InsertMemberToGroup(GridButton->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TfrmUsers::GroupMembersMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton)
{

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBGroups DBGroups;
	TContact Contact;
	TDBContacts DBContacts;
	TMMContactInfo Info;
	ContactGroup GroupKey;

	int ContactKey = Contact.GetContactKeyByName(DBTransaction, GridButton->Caption);
		DBGroups.DeleteContactGroup(DBTransaction, pGroupButton->Tag, ContactKey);

		if(!TDBContacts::GetCurrentGroups(DBTransaction, ContactKey, Info))
		{
			ContactGroup GroupKey;
			GroupKey.Key = TDBGroups::FindGroup(DBTransaction, "No Contacts Group");
			DBContacts.SetCurrentGroups(DBTransaction, ContactKey, GroupKey);
        }


	DBTransaction.Commit();
        DisplayGroupMembers();
        DisplayMembers(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmUsers::btnAllMouseClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TContact Contact;
	TDBGroups  DBGroups;
	int contactkey;
	int groupkey = DBGroups.GetOrCreateGroup(DBTransaction, pGroupButton->Caption);






	for(int i = 0; i < vectorMembers.size(); i++)
	{
		contactkey = Contact.GetContactKeyByName(DBTransaction, vectorMembers.at(i));
		if(!DBGroups.ContactGroupExists(DBTransaction, contactkey, groupkey))
		{
			InsertMemberToGroup(vectorMembers.at(i));
		}
	}


}
//---------------------------------------------------------------------------

void TfrmUsers::InsertMemberToGroup(UnicodeString pMemberButton)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TDBGroups DBGroups;
        TContact Contact;
        TDBContacts DBContacts;
        std::vector<ContactGroup> groups;
        std::vector<ContactGroup> temp;

        int ContactKey = Contact.GetContactKeyByName(DBTransaction, pMemberButton);
        int Type = DBGroups.GetGroupType(DBTransaction,pGroupButton->Caption);
        DBGroups.GetGroupsfromType(DBTransaction,groups, Type);


		ContactGroup GroupKey;
		GroupKey.Key = TDBGroups::FindGroup(DBTransaction, "No Contacts Group");


        for(int i = 0; i < groups.size(); i++)
        {
			if(DBContacts.HasAccountProfile(DBTransaction, ContactKey))
        	{
				DBGroups.GetContacts_GroupsKeys(DBTransaction, ContactKey, temp);

				if(temp.size())
				{
					if(temp.at(0).Key != GroupKey.Key)
					{
						MessageBox( pMemberButton + " Cannot be in more than 1 group", "Error", 0);
						break;
					}
				}
			}



        	if(!Type)
			{
				TDBContacts::RemoveDefaultGroup(DBTransaction, ContactKey);
				DBGroups.SetContactGroup(DBTransaction, ContactKey, pGroupButton->Caption);
				DBTransaction.Commit();
				break;
			}

        	else if(DBGroups.ContactGroupExists(DBTransaction, ContactKey, groups.at(i).Key))
			{
				if(MessageBox( pMemberButton + " is already in a group with the same group number."
					" Would " + pMemberButton + " like to be removed from their previous group?", "Warning",
					MB_ICONWARNING + MB_YESNO) == ID_NO)
				{
					break;
				}
				else
				{
					DBGroups.DeleteContactGroup(DBTransaction,groups.at(i).Key, ContactKey);
				}

			}
			if(i == (groups.size()-1))
			{
				TDBContacts::RemoveDefaultGroup(DBTransaction, ContactKey);
				DBGroups.SetContactGroup(DBTransaction, ContactKey, pGroupButton->Caption);
				DBTransaction.Commit();
			}
		}





        DisplayGroupMembers();
        DisplayMembers(true);
}


void __fastcall TfrmUsers::btnCloseMouseClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------


void __fastcall TfrmUsers::AllSummaMouseClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
        TDBGroups DBGroups;

	for(int i = 0; i < groupmembersVector.size(); i++)
	{
        	if(DBGroups.SummaGroupExists(DBTransaction, groupmembersVector.at(i).Key))
                {
                	if(MessageBox(groupmembersVector.at(i).Name + " is already in a summa group, would you like to make this the new Summa group?", "", MB_YESNO) == ID_YES)
                        {
                        	DBGroups.RemoveSummaGroup(DBTransaction, groupmembersVector.at(i).Key);
                                DBGroups.SetSummaGroup(DBTransaction, pGroupButton->Tag, groupmembersVector.at(i).Key);
                        }
                }
                else
                {
        		DBGroups.SetSummaGroup(DBTransaction, pGroupButton->Tag, groupmembersVector.at(i).Key);
                }
	}
        DBTransaction.Commit();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUsers::FormResize(TObject *Sender)
{
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height < 1.4)
          {
	  	ScaleBy(Screen->Width, Temp);
          }
   }
}
//---------------------------------------------------------------------------

