// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef PalmMate
#include "Palm.h"
#endif

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "SelectMember.h"
#include "MMMessageBox.h"
#include "MMLogging.h"
#include "EditCustomer.h"
#include "ManagerVariable.h"
#include "Membership.h"
#include "DBContacts.h"

#include <cctype>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchKeyboard"
#pragma link "TouchBtn"
#pragma link "TouchNumpad"
#pragma link "TouchGrid"
#pragma link "TouchPages"
#pragma resource "*.dfm"

char MemButtonChar[24][2] =
{
   {
	  '~', '`'
   },
   {
	  '!', '1'
   },
   {
	  '@', '2'
   },
   {
	  '#', '3'
   },
   {
	  '$', '4'
   },
   {
	  '%', '5'
   },
   {
	  '^', '6'
   },
   {
	  '&', '7'
   },
   {
	  '*', '8'
   },
   {
	  '(', '9'
   },
   {
	  ')', '0'
   },
   {
	  '_', '-'
   },
   {
	  '+', '='
   },
   {
	  '{', '['
   },
   {
	  '}', ']'
   },
   {
	  '|', '\\'
   },
   {
	  ':', ';'
   },
   {
	  '"', '\''
   },
   {
	  '<', ','
   },
   {
	  '>', '.'
   },
   {
	  '?', '/'
   }
};

// ---------------------------------------------------------------------------
__fastcall TfrmSelectMember::TfrmSelectMember(TComponent* Owner) : TZForm(Owner)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectMember::FormCreate(TObject *Sender)
{
   StartWithShiftDown = false;
   LastSearchIndex = 0;
   Password = "";
   pnlList->Height = Panel4->Height;
   MemberIncrementalSearch = false;

}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::FormShow(TObject *Sender)
{
   FormResize(this);

   SelectedUser.Clear();

   tbtnOk->Enabled = false;
   tbtnCancel->Enabled = true;
   tbtnCancel->Visible = true;
   tlistMembers->Visible = true;



   edSearch->Text = SearchString;
   LastSearchIndex = 0;
   SelectionVisible = false;

   btnAddUser->Enabled = !TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ReadOnlyInterface;
   btnEditUser->Enabled = !TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ReadOnlyInterface;
   MemberIncrementalSearch = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->EnableIncrementalSearch;

   SearchType = TDeviceRealTerminal::Instance().ManagerMembership->DefaultSearchType;
   if (SearchType == eSearchByPhone)
   {
	  tbtnSearchByGuestClick(Sender);
   }
   else if (SearchType == eSearchByNumber)
   {
	  tbtnSearchByNumberClick(Sender);
   }
   else
   {
	  tbtnSearchByNameClick(Sender);
   }
   edSearch->SetFocus();



}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::FormClose(TObject *Sender, TCloseAction &Action)
{
   StartWithShiftDown = false;
   MustHaveValidMember = false;
   Filter = eMemberAll;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectMember::FormResize(TObject *Sender)
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

   Panel4->Height = pnlKeyboard->Top;
   Panel8->Height = Panel3->Height;
   pnlList->Width = Panel4->Left -this->BorderWidth;
   pnlList->Left = this->BorderWidth;
   pnlList->Top = this->BorderWidth;

   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;

   // tlistMembers->ItemHeight = btnSearch->Height;
   if (TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay == eMemKeypad)
   {
	  pnlList->Height = Panel4->Height;
   }
   else
   {
	  pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   }
   Realign();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectMember::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(this);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectMember::btnOkClick(TObject *Sender)
{
    LoadSelectedMemberInfoFromDB();
    ModalResult = mrOk;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   if (MustHaveValidMember && SelectedUser.ContactKey == 0)
   {
	  MessageBox("A Valid Member must be selected", "Please Select a Member", MB_OK + MB_ICONINFORMATION);
	  CanClose = false;
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tbtnCancelClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tbtnClearClick(TObject *Sender)
{
   lbMemberText->Clear();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::edSearchKeyPress(TObject *Sender, char &Key)
{
   if (Key == VK_RETURN)
   {
	  QuickSearch(edSearch->Text);
	  edSearch->SelectAll();
	  DisplayData(SelectedUser);
   }
   else
   {
	  LastSearchIndex = 0;
   }
}
// ---------------------------------------------------------------------------

void TfrmSelectMember::QuickSearch(AnsiString SearchString)
{
   SearchString = edSearch->Text;
   bool ItemFound = false;
   for (int i = LastSearchIndex; (i < tlistMembers->Count) && !ItemFound; i++)
   {
	  if (UpperCase(tlistMembers->Items->Strings[i]).Pos(UpperCase(SearchString)) != 0)
	  {
		 tlistMembers->Selected[i] = true;
		 // tgridMembers->SetVisibleRow(tgridMembers->Row(tgridMembers->Buttons[i][0]));
		 LastSearchIndex = i + 1;
		 ItemFound = true;
		 SetSelectedMember(int(tlistMembers->Items->Objects[i]), false);
	  }
   }

   if (!ItemFound && LastSearchIndex > 0)
   {
	  LastSearchIndex = 0;
	  for (int i = LastSearchIndex; (i < tlistMembers->Items->Count) && !ItemFound; i++)
	  {
		 if (UpperCase(tlistMembers->Items->Strings[i]).Pos(UpperCase(SearchString)) != 0)
		 {
			tlistMembers->Selected[i] = true;
			// tgridMembers->Buttons[i][0]->Latched = true;
			// tgridMembers->SetVisibleRow(tgridMembers->Row(tgridMembers->Buttons[i][0]));
			LastSearchIndex = i + 1;
			ItemFound = true;
			SetSelectedMember(int(tlistMembers->Items->Objects[i]), false);
		 }
	  }
   }


}

void __fastcall TfrmSelectMember::tbtnSearchByNameClick(TObject *Sender)
{
   SearchType = eSearchByName;
   tbtnSearchByName->ButtonColor = clGreen;
   tbtnSearchByNumber->ButtonColor = clMaroon;
   tbtnSearchByGuest->ButtonColor = clMaroon;
   TDeviceRealTerminal::Instance().ManagerMembership->DefaultSearchType = SearchType;
   RefreshDisplay();
   edSearch->SetFocus();
   TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemKeypad;
   pnlList->Height = Panel4->Height;
   UpdateInterface();

}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tbtnSearchByNumberClick(TObject *Sender)
{
   SearchType = eSearchByNumber;
   tbtnSearchByName->ButtonColor = clMaroon;
   tbtnSearchByNumber->ButtonColor = clGreen;
   tbtnSearchByGuest->ButtonColor = clMaroon;
   TDeviceRealTerminal::Instance().ManagerMembership->DefaultSearchType = SearchType;
   RefreshDisplay();
   edSearch->SetFocus();
   TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemNumPad;
   pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   UpdateInterface();

}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tbtnSearchByGuestClick(TObject *Sender)
{
   SearchType = eSearchByPhone;
   tbtnSearchByName->ButtonColor = clMaroon;
   tbtnSearchByNumber->ButtonColor = clMaroon;
   tbtnSearchByGuest->ButtonColor = clGreen;
   TDeviceRealTerminal::Instance().ManagerMembership->DefaultSearchType = SearchType;
   RefreshDisplay();
   edSearch->SetFocus();
   TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemNumPad;
   pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   UpdateInterface();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::CardSwipe(TMessage& Message)
{
   AnsiString Data = *((AnsiString*)Message.WParam);

   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();

   TMMContactInfo Info;
   Info.CardStr = Data.SubString(1, 80);
   int SearchKey = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactByCard(DBTransaction, Info);
   if (SearchKey != 0)
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, SearchKey, Info);
	  if (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ContactType == eERSMember)
	  {
		 SearchKey = Info.ExternalKey;
	  }
   }
   else
   {
	  int SearchKey = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactByProx(DBTransaction, Data.SubString(1, 80));
	  if (SearchKey != 0)
	  {
		 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, SearchKey, Info);
		 if (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ContactType == eERSMember)
		 {
			SearchKey = Info.ExternalKey;
		 }
	  }
   }
   DBTransaction.Commit();

   if (SearchKey != 0)
   {
	  SetSelectedMember(SearchKey, false);
	  bool ItemFound = false;
	  for (int i = LastSearchIndex; (i < tlistMembers->Count) && !ItemFound; i++)
	  {
		 if (int(tlistMembers->Items->Objects[i]) == SearchKey)
		 {
			tlistMembers->Selected[i] = true;
			// tgridMembers->Buttons[i][0]->Latched = true;
			// tgridMembers->SetVisibleRow(tgridMembers->Row(tgridMembers->Buttons[i][0]));
			ItemFound = true;
		 }
	  }
   }
}

void TfrmSelectMember::RefreshDisplay()
{
	/* Emphasizing that this is a reference?! */
	TDeviceRealTerminal &ref_drtrm = TDeviceRealTerminal::Instance();

	TIBSQL *members_to_list;
	TContactType memsys_ctype = ref_drtrm.ManagerMembership->MembershipSystem->ContactType;
	unsigned int sel_mbr = 0;
	Database::TDBTransaction trans(ref_drtrm.DBControl);

	ref_drtrm.RegisterTransaction(trans);

	ref_drtrm.ManagerMembership->MembershipSystem->FastContactListing(&members_to_list, trans, memsys_ctype, SearchType, Filterlist);

	tlistMembers->Items->BeginUpdate();
	tlistMembers->Clear();

	btnEditUser->Enabled = members_to_list->RecordCount > 0;
	if (btnEditUser->Enabled)
    {
		unsigned int ukey;

      for ( ; !members_to_list->Eof; members_to_list->Next())
      {
			ukey = members_to_list->FieldByName("k")->AsInteger;

			if ((ukey == Last3rdPartyMemberKey && memsys_ctype == eERSMember) || ukey == LastMemberKey)
				sel_mbr = ukey;

			tlistMembers->AddItem(ref_drtrm.ManagerMembership->MembershipSystem->FastFormatMemberName
            (TStringTools::Instance()->StripSpecialChars_ANSI(members_to_list->FieldByName("n")->AsString + " "+ members_to_list->FieldByName("l")->AsString)),
			   reinterpret_cast<TObject *>(ukey));
		}
	}

	trans.Commit();

	tlistMembers->Items->EndUpdate();
	tlistMembers->Refresh();
	tlistMembers->Realign();
	tlistMembers->SetFocus();

	SetSelectedMember(sel_mbr, 0);
}

void TfrmSelectMember::DisplayData(TMMContactInfo &TempUserInfo)
{
   if (SelectedUser.ContactKey != 0)
   {
	  lbMemberText->Clear();
	  lbMemberText->Items->Add("Member Number : " + TStringTools::Instance()->StripSpecialChars_UC(TempUserInfo.MembershipNumber));
	  lbMemberText->Items->Add("Member Name   : " + TStringTools::Instance()->StripSpecialChars_UC(TempUserInfo.Name +" " +TempUserInfo.Surname));

	  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	  DBTransaction.StartTransaction();
	  std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
	  DBTransaction.Commit();
	  if (Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckViewMembersDetials))
	  {
		 lbMemberText->Items->Add("Phone : " + TStringTools::Instance()->StripSpecialChars_UC(TempUserInfo.Phone));
		 lbMemberText->Items->Add("Mobile : " + TStringTools::Instance()->StripSpecialChars_UC(TempUserInfo.Mobile));
		 lbMemberText->Items->Add("Addr : " + TStringTools::Instance()->StripSpecialChars_UC(TempUserInfo.LocationAddress));
	  }
   }
   else
   {
	  lbMemberText->Clear();
   }
}

void __fastcall TfrmSelectMember::tbtnFullSearchClick(TObject *Sender)
{
   /*
   SearchString = edSearch->Text;
   bool ItemFound = false;
   for (int i = LastSearchIndex; (i < List->RowCount) && !ItemFound; i++)
   {
   if(UpperCase(List->Cells[0][i]).Pos(UpperCase(SearchString)) != 0)
   {
   List->Row = i;
   LastSearchIndex = i+1;
   ItemFound = true;
   }
   }
   edSearch->SelectAll();
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();
   DisplayData(DBTransaction);
   DBTransaction.Commit();


   for (int i=0; i<pnlLastPicked->ControlCount; i++)
   {
   TTouchButton *UnSelected = (TTouchButton * )pnlLastPicked->Controls[i];
   UnSelected->Color = clMaroon;
   } */
}
// ---------------------------------------------------------------------------

int TfrmSelectMember::SetSelectedMember(int inSelectedMember, bool filter)
{


   if (inSelectedMember != 0)
   {
      SelectedUser.Clear();
	  tpDetails->ActivePage = tsDetails;
	  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	  TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	  DBTransaction.StartTransaction();
	  int ContactKey = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactKey(DBTransaction, inSelectedMember);
	  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, ContactKey, SelectedUser);
	  DisplayData(SelectedUser);

	  tbtnOk->Enabled = true;
	  tbtnOk->Caption = "Select\r" + SelectedUser.Name + " "+SelectedUser.Surname;

	  SelectionVisible = true;
	  DBTransaction.Commit();
   }
   else
   {
	  tbtnOk->Caption = "Ok";
	  tbtnOk->Enabled = false;
	  lbMemberText->Clear();
   }

   return SelectedUser.ContactKey;
}

int TfrmSelectMember::SelectMember(std::vector <TQuickMember> &LastAccessed, eMemberDisplayFilter inFilter)
{
   int RetVal = 0;
   try
   {
	  tgridHistory->ColCount = 0;
	  tgridHistory->RowCount = 0;

	  tgridHistory->VertScrollDistance = (tgridHistory->Height / 4) * 6;
	  tgridHistory->DefaultRowHeight = tgridHistory->Height / 4;

	  tgridHistory->ColCount = 0;
	  tgridHistory->RowCount = LastAccessed.size();

	  for (int i = 0; i < LastAccessed.size(); i++)
	  {
		 tgridHistory->Buttons[i][0]->Caption = LastAccessed[i].Name;
		 tgridHistory->Buttons[i][0]->Tag = LastAccessed[i].Key;
	  }

	  Filter = inFilter;
	  RetVal = this->ShowModal();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   return RetVal;
}

void __fastcall TfrmSelectMember::btnAddUserClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);

   TMMContactInfo UserInfo;
   DBTransaction.StartTransaction();

   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   TLoginSuccess Result;

    if(TGlobalSettings::Instance().EnablePhoneOrders && TDeviceRealTerminal::Instance().ManagerMembership->IsPOFMemberSelectionEnabled())
        Result = lsAccepted;
    else
        Result = Staff->Login(this, DBTransaction, UserInfo, CheckAccountManager);

   DBTransaction.Commit();

   if (Result == lsAccepted)
   {
	  TMMContactInfo ContactInfo;
	  TDeviceRealTerminal::Instance().ManagerMembership->AddMember(ContactInfo);
	  RefreshDisplay();
	  switch(SearchType)
	  {
	  case eSearchByNumber:
		 edSearch->Text = ContactInfo.MembershipNumber;
		 break;
	  case eSearchByName:
		 edSearch->Text = ContactInfo.Name;
		 break;
	  case eSearchByPhone:
		 edSearch->Text = ContactInfo.Phone;
		 break;
	  }
	  QuickSearch(edSearch->Text);
	  edSearch->SelectAll();
	  DisplayData(SelectedUser);

        if(ContactInfo.ContactKey > 0)
        {
            // Closes the form if the phone orders are enabled and setup for quicker member creation
            CloseFormIfPhoneOrdersEnabled();
        }
   }
   else if (Result == lsDenied)
   {
	  MessageBox("You do not have access to the loyalty settings.", "Error", MB_OK + MB_ICONERROR);
   }
   else if (Result == lsPINIncorrect)
   {
	  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::btnEditUserClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   TMMContactInfo UserInfo;

   DBTransaction.StartTransaction();
   bool prevCharges = SelectedUser.Charges;

   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   TLoginSuccess Result;

    if(TGlobalSettings::Instance().EnablePhoneOrders  && TDeviceRealTerminal::Instance().ManagerMembership->IsPOFMemberSelectionEnabled())
        Result = lsAccepted;
    else
        Result = Staff->Login(this, DBTransaction, UserInfo, CheckAccountManager);

    DBTransaction.Commit();
   if (Result == lsAccepted)
   {
	  TMMContactInfo ContactInfo;
	  ContactInfo.ContactKey = SelectedUser.ContactKey;
	  DBTransaction.StartTransaction();
	  TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction, ContactInfo);
	  DBTransaction.Commit();
	  RefreshDisplay();
   }
   else if (Result == lsDenied)
   {
	  MessageBox("You do not have access to the loyalty settings.", "Error", MB_OK + MB_ICONERROR);
   }
   else if (Result == lsPINIncorrect)
   {
	  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
   }


}
// ---------------------------------------------------------------------------

void TfrmSelectMember::UpdateInterface()
{
   if (TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay == eMemKeypad)
   {
	  tbtnInterface->Caption = "NumPad";
	  TouchNumpad1->Visible = false;
	  TouchKeyboard1->Visible = true;

   }
   else
   {
	  tbtnInterface->Caption = "Keyboard";
	  TouchNumpad1->Visible = true;
	  TouchKeyboard1->Visible = false;
   }
}

void __fastcall TfrmSelectMember::edSearchChange(TObject *Sender)
{
   if (MemberIncrementalSearch)
   {
	  LastSearchIndex = 0;
	  QuickSearch(edSearch->Text);
	  DisplayData(SelectedUser);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::TouchKeyboard1Click(TObject *Sender, char Char)
{
   PostMessage(edSearch->Handle, WM_CHAR, Char, 0);
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::btnSearchMouseClick(TObject *Sender)
{
   PostMessage(edSearch->Handle, WM_CHAR, '\r', 0);
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::TouchNumpad2Click(TObject *Sender, TNumpadKey Key)
{
   if (Key <= nk9)
   {
	  PostMessage(edSearch->Handle, WM_CHAR, char(Key + 0x30), 0);
   }
   else if (Key == nkCustom)
   {
	  edSearch->Text = "";
   }
   else // nkClear
   {
	  edSearch->Text = "";
   }
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tbtnInterfaceMouseClick(TObject *Sender)
{
   if (TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay == eMemKeypad)
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemNumPad;
	  pnlList->Height = Panel4->Height + pnlKeyboard->Height;
   }
   else
   {
	  TDeviceRealTerminal::Instance().ManagerMembership->DefaultInterfaceDisplay = eMemKeypad;
	  pnlList->Height = Panel4->Height;
   }
   Realign();
   UpdateInterface();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::btnClearMouseClick(TObject *Sender)
{
   edSearch->Text = "";
   edSearch->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tgridHistoryMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
   int ContactKey = GridButton->Tag;

   bool ItemFound = false;
   for (int i = 0; i < tlistMembers->Items->Count; i++)
   {
	  if (int(tlistMembers->Items->Objects[i]) == ContactKey)
	  {
		 LastSearchIndex = i + 1;
		 ItemFound = true;
		 tlistMembers->Selected[i] = true;
		 // tgridMembers->SetVisibleRow(i);
		 // tgridMembers->Buttons[i][0]->Latched = true;
	  }
   }

   if (ItemFound)
   {
	  SetSelectedMember(ContactKey, false);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::tlistMembersClick(TObject *Sender)
{
   if (!MemberIncrementalSearch)
   {
	  edSearch->SelectAll();
   }

	SetSelectedMember(int(tlistMembers->Items->Objects[tlistMembers->ItemIndex]), false);

}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::TouchBtn2AutoRepeat(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   // while (ScrollCount < List->VisibleRowCount-1)
   while (ScrollCount < (int(tlistMembers->Height / tlistMembers->ItemHeight) - 1))
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(tlistMembers->Handle, WM_VSCROLL, SB_LINEUP, 0L);
		 ScrollCount++;
		 if (tlistMembers->TopIndex == 0)
		 {
			break;
		 }
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::TouchBtn1AutoRepeat(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   while (ScrollCount < (int(tlistMembers->Height / tlistMembers->ItemHeight) - 1))
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(tlistMembers->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
		 ScrollCount++;
		 /* if (tlistMembers->TopIndex == tlistMembers->RowCount - tlistMembers->VisibleRowCount)
		 {
		 break;
		 } */
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::btnDeleteMouseClick(TObject *Sender)
{
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   TMMContactInfo UserInfo;

   DBTransaction.StartTransaction();

   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   TLoginSuccess Result = Staff->Login(this, DBTransaction, UserInfo, CheckAccountManager);
   DBTransaction.Commit();
   if (Result == lsAccepted)
   {
	  TMMContactInfo ContactInfo;
	  ContactInfo.ContactKey = SelectedUser.ContactKey;
	  DBTransaction.StartTransaction();
	  TDeviceRealTerminal::Instance().ManagerMembership->DeleteMember(DBTransaction, ContactInfo);
	  DBTransaction.Commit();
	  SetSelectedMember(0, false);
	  RefreshDisplay();
   }
   else if (Result == lsDenied)
   {
	  MessageBox("You do not have access to the loyalty settings.", "Error", MB_OK + MB_ICONERROR);
   }
   else if (Result == lsPINIncorrect)
   {
	  MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectMember::Panel8Resize(TObject *Sender)
{
   TouchBtn1->Width = (Panel8->Width / 2) - (Panel6->Width / 2);
}
//----------------------------------------------------------------------------

void TfrmSelectMember::SetFilterlist(bool temp)
{
	Filterlist = temp;
}
//----------------------------------------------------------------------------

TMMContactInfo TfrmSelectMember::getMemberInfo(void)
{
	return MemberInfo;
}
//----------------------------------------------------------------------------

void TfrmSelectMember::CloseFormIfPhoneOrdersEnabled()
{
    if(TGlobalSettings::Instance().EnablePhoneOrders
        && TDeviceRealTerminal::Instance().ManagerMembership->IsPOFMemberSelectionEnabled()
        && SelectedUser.ContactKey != 0)
    {
        LoadSelectedMemberInfoFromDB();
        ModalResult = mrOk;
    }
}

void TfrmSelectMember::LoadSelectedMemberInfoFromDB()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();

    int MemberKey;
    if(tlistMembers->ItemIndex != -1)
    {
        MemberKey = int(tlistMembers->Items->Objects[tlistMembers->ItemIndex]);
        TDBContacts::GetContactDetails(DBTransaction, MemberKey, MemberInfo);
    }
}
