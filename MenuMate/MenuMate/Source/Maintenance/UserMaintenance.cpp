// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "UserMaintenance.h"
#include "Main.h"
#include "NewUser.h"
#include "EditCustomer.h"
#include "MMMessageBox.h"
#include "Invoice.h"
#include "Membership.h"
#include "SmartCardAPI.h"
#include "DBTab.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmUserMaintenance::TfrmUserMaintenance(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner),
DBControl(inDBControl)
{
   Color = clBlack;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::imgExitClick(TObject *Sender)
{
   Close();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::FormCreate(TObject *Sender)
{
   TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.RegisterForEvent(OnSmartCardInserted);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmUserMaintenance::FormDestroy(TObject *Sender)
{
   TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.DeregisterForEvent(OnSmartCardInserted);
}

void __fastcall TfrmUserMaintenance::FormShow(TObject *Sender)
{
   FormResize(this);

   Top = 0;
   Left = 0;
   Width = Screen->Width;
   Height = Screen->Height;

   if (UserMode == umStaff)
   {
	  pnlLabel->Caption = "Users";
   }
   else if (UserMode == umLoyalty)
   {
	  pnlLabel->Caption = "Customers";
   }

   Database::TDBTransaction DBTransaction(DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();
   LoadUserList(DBTransaction);
   DBTransaction.Commit();

   if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->Enabled && TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
   {
	  // TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->Refresh();
	  OnSmartCardInserted(NULL);
   }
}

// ---------------------------------------------------------------------------
void TfrmUserMaintenance::LoadUserList(Database::TDBTransaction &DBTransaction)
{
   int Row = sgList->Row;
   sgList->RowCount = 1;
   sgList->Cells[0][0] = "";
   sgList->Objects[0][0] = NULL;
   TStringList *List = NULL;
   try
   {
	  std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

	  List = new TStringList;

	  if (UserMode == umStaff)
	  {
		 Staff->GetContactList(DBTransaction, List, "%Name", "NAME");
	  }
	  else if (UserMode == umLoyalty)
	  {
		 TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactList(DBTransaction, List, "%Name (%Number)", "NAME");
	  }

	  for (int i = 0; i < List->Count; i++)
	  {
		 AnsiString Name = List->Strings[i];
		 std::auto_ptr <TStringList> FixCase(new TStringList);
		 FixCase->Delimiter = ' ';
		 FixCase->DelimitedText = Name;
		 Name = "";
		 // Uppercase the First Letter of the Name.
		 for (int i = 0; i < FixCase->Count; i++)
		 {
			AnsiString Data = FixCase->Strings[i];
			Name += " " + Data.SubString(1, 1).UpperCase() + Data.SubString(2, Data.Length() - 1).LowerCase();
		 }

		 sgList->Cells[0][i] = Name;
		 sgList->Objects[0][i] = List->Objects[i];
		 sgList->RowCount++;
	  }
   }
   __finally
   {
	  delete List;
   }
   sgList->RowCount--;
   if (Row < sgList->RowCount)
   {
	  sgList->Row = Row;
   }
   else
   {
	  sgList->Row = 0;
   }
   sgList->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::FormResize(TObject *Sender)
{
 //  if (Tag != Screen->Width)
 //{
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height < 1.4)
         {
	  	ScaleBy(Screen->Width, Temp);
          }
  // }
   sgList->DefaultRowHeight = sgList->ClientHeight / 13;
   sgList->DefaultColWidth = sgList->ClientWidth;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::CardSwipe(TMessage& Message)
{
   AnsiString Data = *((AnsiString*)Message.WParam);
   AnsiString ContactName;
   TMMContactInfo Info;
   Info.CardStr = Data.SubString(1, 80);

   Database::TDBTransaction DBTransaction(DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();

   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

   if (UserMode == umStaff)
   {
	  ContactName = Staff->GetContactNameByCard(DBTransaction, Info);
   }
   else if (UserMode == umLoyalty)
   {
	  ContactName = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactNameByCard(DBTransaction, Info);
   }

   LoadUserList(DBTransaction);
   DBTransaction.Commit();

   for (int i = 0; i < sgList->RowCount; i++)
   {
	  if (sgList->Cells[0][i].UpperCase() == ContactName.UpperCase())
	  {
		 sgList->Row = i;
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::sgListDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   TCanvas *pCanvas = ((TStringGrid*)Sender)->Canvas;
   pCanvas->FillRect(Rect);
   UnicodeString Caption = sgList->Cells[ACol][ARow];
   TRect DrawRegRect(Rect.Left + 2, Rect.Top, Rect.Right - 2, Rect.Bottom);

   int TextHeight = DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &DrawRegRect, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX,
	  NULL) + 4;

   TRect CentredRect = DrawRegRect;
   int RowHeight = Rect.Bottom - Rect.Top;
   if (RowHeight > TextHeight)
   {
	  CentredRect.Top += (RowHeight - TextHeight) / 2;
	  CentredRect.Bottom = CentredRect.Top + TextHeight;
   }
   DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &CentredRect, DT_WORDBREAK | DT_NOPREFIX, NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::btnEditUserClick(TObject *Sender)
{
   if ((int)sgList->Objects[0][sgList->Row] != 0)
   {
	  if (UserMode == umStaff)
	  {
		 std::auto_ptr <TfrmNewUser> frmNewUser(TfrmNewUser::Create <TfrmNewUser> (this, DBControl));
		 frmNewUser->User_Key = (int)sgList->Objects[0][sgList->Row];
		 frmNewUser->Editing = true;
		 frmNewUser->Caption = "Editing User";
		 frmNewUser->ShowModal();
		 Database::TDBTransaction DBTransaction(DBControl);
		 TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		 DBTransaction.StartTransaction();
		 LoadUserList(DBTransaction);
		 DBTransaction.Commit();
	  }
	  else if (UserMode == umLoyalty)
	  {
		 Database::TDBTransaction DBTransaction(DBControl);
		 TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		 DBTransaction.StartTransaction();

		 TMMContactInfo ContactInfo;
		 ContactInfo.ContactKey = (int)sgList->Objects[0][sgList->Row];
		 if (TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction, ContactInfo) == mrOk)
		 {
			LoadUserList(DBTransaction);
		 }
		 DBTransaction.Commit();
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::btnAddUserClick(TObject *Sender)
{
   if (UserMode == umStaff)
   {
	  std::auto_ptr <TfrmNewUser> frmNewUser(TfrmNewUser::Create <TfrmNewUser> (this, DBControl));
	  frmNewUser->Editing = false;
	  frmNewUser->Caption = "New User";
	  frmNewUser->ShowModal();
	  Database::TDBTransaction DBTransaction(DBControl);
	  TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	  DBTransaction.StartTransaction();
	  LoadUserList(DBTransaction);
	  DBTransaction.Commit();
   }
   else if (UserMode == umLoyalty)
   {

	  if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["ReadOnly"])
	  {
		 MessageBox("You must Add,Edit and Delete Members from your 3rd Party Membership software.", "Warning", MB_ICONWARNING + MB_OK);
	  }
	  else
	  {
		 if (!TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
		 {
			MessageBox("Membership is disabled.", "Warning", MB_ICONWARNING + MB_OK);
		 }
		 else
		 {
			TMMContactInfo ContactInfo;
			ContactInfo.ContactKey = (int)sgList->Objects[0][sgList->Row];
			if (TDeviceRealTerminal::Instance().ManagerMembership->AddMember(ContactInfo) == mrOk)
			{
			   Database::TDBTransaction DBTransaction(DBControl);
			   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			   DBTransaction.StartTransaction();
			   LoadUserList(DBTransaction);
			   DBTransaction.Commit();
			}
		 }
	  }
   }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmUserMaintenance::btnDeleteUserClick(TObject *Sender)
{

   if ((int)sgList->Objects[0][sgList->Row] != 0)
   {
	  Database::TDBTransaction DBTransaction(DBControl);
	  TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	  DBTransaction.StartTransaction();

	  std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

	  if (UserMode == umStaff)
	  {
		 if (MessageBox("Are you sure you wish to remove this user? Their staff hours will be kept and will still appear on all reports.",
			   "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
		 {
			int ContactKey = (int)sgList->Objects[0][sgList->Row];
			if (Invoice->GetOutStanding(DBTransaction, ContactKey) || TDBTab::GetOutStanding(DBTransaction, ContactKey))
			{
			   MessageBox("You must clear all outstanding Invoices and Tabs before this person can be removed.", "Warning",
				  MB_ICONWARNING + MB_OK);
			}
			else
			{
			   Staff->DeleteContact(DBTransaction, ContactKey);
			}
			LoadUserList(DBTransaction);
		 }
	  }
	  else if (UserMode == umLoyalty)
	  {
		 if (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ReadOnlyInterface)
		 {
			MessageBox("You must Add,Edit and Delete members from your 3rd party Membership software.", "Warning", MB_ICONWARNING + MB_OK);
		 }
		 else
		 {
			if (MessageBox("Are you sure you wish to remove this Member?", "Warning", MB_ICONWARNING + MB_OKCANCEL) == ID_OK)
			{
			   int ContactKey = (int)sgList->Objects[0][sgList->Row];
			   if (Invoice->GetOutStanding(DBTransaction, ContactKey) || TDBTab::GetOutStanding(DBTransaction, ContactKey))
			   {
				  MessageBox("You must clear all outstanding Invoices and Tabs before this person can be removed.", "Warning",
					 MB_ICONWARNING + MB_OK);
			   }
			   else
			   {
				  TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->DeleteContact(DBTransaction, ContactKey);
			   }
			   LoadUserList(DBTransaction);
			}
		 }
	  }
	  DBTransaction.Commit();
   }
}

// ---------------------------------------------------------------------------
void TfrmUserMaintenance::OnSmartCardInserted(TSystemEvents *Sender)
{
   TMMContactInfo Member;
   eMemberSource MemberSource;
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
   TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
   DBTransaction.StartTransaction();

   TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, Member, MemberSource);
   if (Result == lsAccountBlocked || Result == lsAccepted)
   {
	  int ContactKey = Member.ContactKey;
	  bool ItemFound = false;
	  for (int i = 0; (i < sgList->RowCount) && !ItemFound; i++)
	  {
		 if (sgList->Objects[0][i] == (TObject*)Member.ContactKey)
		 {
			sgList->Row = i;
			ItemFound = true;
		 }
	  }
   }
   DBTransaction.Commit();
}

// ---------------------------------------------------------------------------

void __fastcall TfrmUserMaintenance::btnUserUpMouseClick(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   while (ScrollCount < sgList->VisibleRowCount - 1)
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(sgList->Handle, WM_VSCROLL, SB_LINEUP, 0L);
		 ScrollCount++;
		 if (sgList->TopRow == 0)
		 {
			break;
		 }
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmUserMaintenance::btnUsrDownMouseClick(TObject *Sender)
{
   __int64 TickCount = ::GetTickCount();
   int ScrollCount = 0;
   while (ScrollCount < sgList->VisibleRowCount - 1)
   {
	  if (::GetTickCount() > TickCount + 20)
	  {
		 TickCount = ::GetTickCount();
		 SendMessage(sgList->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
		 ScrollCount++;
		 if (sgList->TopRow == sgList->RowCount - sgList->VisibleRowCount)
		 {
			break;
		 }
	  }
   }
}
// ---------------------------------------------------------------------------


