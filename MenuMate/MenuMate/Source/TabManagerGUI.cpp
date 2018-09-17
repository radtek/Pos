// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TabManagerGUI.h"
#include "AddTab.h"
#include "DBTab.h"
#include "MMTouchNumpad.h"
#include "CardSwipe.h"
#include "MMMessageBox.h"
#include "EditCustomer.h"
#include "Invoice.h"
#include "TableManager.h"
#include "GUIDiscount.h"
#include "DBOrder.h"
#include "ListPaymentSystem.h"
#include "GUIScale.h"

#include "ManagerPatron.h"
#include "DropDown.h"
#include "Membership.h"
#include "VerticalSelect.h"
#include "Printing.h"
#include "DeviceRealTerminal.h"
#include "MMData.h"
#include "ReqPrintJob.h"
#include "ListSecurityRefContainer.h"
#include "ItemSizeCategory.h"
#include "DBSecurity.h"
//#include "ManagerClippIntegration.h"
#include "DBTax.h"
#include "TaxProfileDBAccessManager_MM.h"
#include "TaxProfile.h"
// ---------------------------------------------------------------------------
#define NUMBER_OF_TABS_IN_VIEW 7
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

#define SEAT_SELECTED_COLOUR				clNavy
#define SEAT_SELECTED_FONT_COLOUR		clWhite
#define SEAT_UNSELECTED_COLOUR         clSilver
#define SEAT_UNSELECTED_FONT_COLOUR		clBlack
#define SEAT_ALREADYLOCKED_COLOUR  		clRed

// ---------------------------------------------------------------------------
__fastcall TfrmTabManager::TfrmTabManager(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl)
{
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::FormCreate(TObject *Sender)
{
	CurrentDisplayMode = eNoDisplayMode;
}

void __fastcall TfrmTabManager::FormShow(TObject *Sender)
{
	FormResize(this);
	SelectedTable = 0;
	SelectedSeat = 0;
	SelectedTab = 0;
	SelectedRoomNo = 0;
	CurrentTabBtn = NULL;
	CurrentTabContainerName = "";
	SelectedPartyName = "";
    ScrollPosition = 0;
	if (CurrentDisplayMode == eNoDisplayMode)
	{
		if (TGlobalSettings::Instance().TabsEnabled)
		{
			CurrentDisplayMode = eTabs;
		}
	}
	// tsTabManager
	pgControl->ActivePage = tsTabManager;
	btnShowNormalTabs->Caption = "Tabs";
	btnStaffTabs->Caption = "Staff Tabs";

	btnMemberTabs->Enabled = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"];
	btnMemberTabs->Visible = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"];
    if(TGlobalSettings::Instance().IsThorlinkSelected)
    {
     btnMemberTabs->Visible = false;
    }

	btnShowNormalTabs->Enabled = TGlobalSettings::Instance().TabsEnabled;
	btnShowNormalTabs->Visible = TGlobalSettings::Instance().TabsEnabled;

    btnClippTab->Visible = TGlobalSettings::Instance().IsClippIntegrationEnabled;

	btnManInvoice->Enabled = false;

	btnMemberTabs->Caption = "Member Tabs";
	lbePartyName->Caption = "";
	btnChangeDetails->Caption = "Change Details";

	if (TGlobalSettings::Instance().TabsEnabled || (static_cast <bool> (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"]) && !static_cast <bool>
				(TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["ReadOnly"])))
	{
		btnAddNewTab->Enabled = true;
		btnChangeDetails->Enabled = true;
	}
	else
	{
		btnAddNewTab->Enabled = false;
		btnChangeDetails->Enabled = false;
	}

	SelectedTable = 0;
	SelectedSeat = 0;
	SelectedRoomNo = 0;
	CurrentTabContainerName = "";
	CurrentTabType = TabNormal;

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
	DBTransaction.Commit();
	ShowTabsDetails();
	RefreshTabDetails();
	SetGridColors(tgDiscounts);
	btnSubsidisedProfile->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTabManager::btnShowNormalTabsClick(TObject *Sender)
{
	SelectedTable = 0;
	SelectedSeat = 0;
	SelectedRoomNo = 0;
	btnAddNewTab->Enabled = true;
	btnChangeDetails->Enabled = true;
	btnManInvoice->Enabled = false;
	btnSubsidisedProfile->Enabled = false;
	btnAddNewTab->Caption = "Create Tab";

	CurrentTabContainerName = "";
	CurrentTabType = TabNormal;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name);
	DBTransaction.Commit();
	ShowTabsDetails();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTabManager::btnMemberTabsClick(TObject *Sender)
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
	{
		TempUserInfo.Clear();
		eMemberSource MemberSource;
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo, MemberSource);
		DBTransaction.Commit();
		if (Result == lsAccepted || Result == lsAccountBlocked)
		{  btnSubsidisedProfile->Enabled = false;
			CurrentDisplayMode = eTabs;
			CurrentTabType = TabMember;
			SelectedTable = 0;
			SelectedSeat = 0;
			SelectedTab = 0;
			SelectedRoomNo = 0;
			if (TGlobalSettings::Instance().TabsEnabled || (static_cast <bool> (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"])
						&& !static_cast <bool> (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["ReadOnly"])))
			{
				btnAddNewTab->Enabled = true;
				btnChangeDetails->Enabled = true;
			}
			else
			{
				btnAddNewTab->Enabled = false;
				btnChangeDetails->Enabled = false;
			}

			btnManInvoice->Enabled = true;
			btnAddNewTab->Caption = "Create Member";

			CurrentTabContainerName = "";

			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
			DBTransaction.Commit();

			ShowTabsDetails();
		}
		else
		{
			lbePartyName->Caption = "Access Declined";
		}
	}
	else
	{
		MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTabManager::btnStaffTabsClick(TObject *Sender)
{
	SelectedTable = 0;
	SelectedSeat = 0;
	SelectedTab = 0;
	SelectedRoomNo = 0;
	btnAddNewTab->Enabled = false;
	btnChangeDetails->Enabled = false;
	btnManInvoice->Enabled = false;
	btnSubsidisedProfile->Enabled = false;
	btnAddNewTab->Caption = "Create Staff Member";

	CurrentTabContainerName = "";
	CurrentDisplayMode = eTabs;
	CurrentTabType = TabStaff;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name);
	DBTransaction.Commit();

	ShowTabsDetails();
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TfrmTabManager::ShowTabsDetails()
{
    MaxScrollPosition = 0;
    sbTabs->ScrollBy( 0, -ScrollPosition );
    sbTabs->Update();
    ScrollPosition = 0;
	while (sbTabs->ControlCount != 0)
	{
		delete sbTabs->Controls[0];
	}

	CurrentTabBtn = NULL;

	if (CurrentDisplayMode == eTabs)
	{
		if (CurrentTabType == TabNormal)
		{
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->SQL->Text = "SELECT " "TAB.TAB_KEY, " "TAB.TAB_NAME NAME " "FROM " "TAB " "WHERE "
			"TAB.TAB_KEY NOT IN (SELECT SEAT.TAB_KEY FROM SEAT WHERE SEAT.TAB_KEY IS NOT NULL)" " AND ( TAB.TAB_TYPE = " + IntToStr
			(CurrentTabType) + " OR TAB.TAB_TYPE = " + IntToStr(TabCashAccount) + " ) " + " ORDER BY " "TAB.TAB_KEY DESC";
			IBInternalQuery->ExecQuery();
			int Y = 0;
			while (!IBInternalQuery->Eof)
			{
				int LableHeight = sbTabs->ClientHeight / NUMBER_OF_TABS_IN_VIEW;
				// Slow
				TTouchBtn *cbTabName = new TTouchBtn(this);
				cbTabName->Top = Y + Label1->Top;
				cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
				cbTabName->Left = Label1->Left;
				cbTabName->Height = LableHeight - Label1->Top;
				cbTabName->Caption = IBInternalQuery->FieldByName("NAME")->AsString;
				cbTabName->Tag = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
				cbTabName->OnMouseClick = TouchButtonTabClick;
				cbTabName->Parent = sbTabs;
				cbTabName->ParentFont = true;
				cbTabName->ButtonColor = SEAT_UNSELECTED_COLOUR;
				Y += LableHeight;
                MaxScrollPosition += LableHeight;
				IBInternalQuery->Next();
			}
			DBTransaction.Commit();

			lbePartyName->Caption = "Tabs";
		}
        else if (CurrentTabType == TabClipp)
		{
//			Database::TDBTransaction DBTransaction(DBControl);
//			DBTransaction.StartTransaction();
//
//			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
//
//            IBInternalQuery->SQL->Text = "SELECT " "TAB.TAB_KEY, " "TAB.TAB_NAME NAME " "FROM " "TAB "
//            "INNER JOIN CLIPP_TAB on tab.TAB_KEY = CLIPP_TAB.CLIPP_TAB_TAB_KEY " "WHERE "
//            "CLIPP_TAB.IS_TAB_CLOSED = 'F' " "AND " "TAB.TAB_KEY NOT IN (SELECT SEAT.TAB_KEY FROM SEAT WHERE SEAT.TAB_KEY IS NOT NULL)" " AND ( TAB.TAB_TYPE = " + IntToStr
//            (CurrentTabType) + " OR TAB.TAB_TYPE = " + IntToStr(TabCashAccount) + " ) " + " ORDER BY " "TAB.TAB_KEY DESC";
//            IBInternalQuery->ExecQuery();
//			int Y = 0;
//			while (!IBInternalQuery->Eof)
//			{
//				int LableHeight = sbTabs->ClientHeight / NUMBER_OF_TABS_IN_VIEW;
//				// Slow
//				TTouchBtn *cbTabName = new TTouchBtn(this);
//				cbTabName->Top = Y + Label1->Top;
//				cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
//				cbTabName->Left = Label1->Left;
//				cbTabName->Height = LableHeight - Label1->Top;
//				cbTabName->Caption = IBInternalQuery->FieldByName("NAME")->AsString;
//				cbTabName->Tag = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
//				cbTabName->OnMouseClick = TouchButtonTabClick;
//				cbTabName->Parent = sbTabs;
//				cbTabName->ParentFont = true;
//				cbTabName->ButtonColor = SEAT_UNSELECTED_COLOUR;
//				Y += LableHeight;
//                MaxScrollPosition += LableHeight;
//				IBInternalQuery->Next();
//			}
//			DBTransaction.Commit();
//
//			lbePartyName->Caption = "Clipp Tabs";
		}
		else
		{
			if (CurrentTabType == TabMember)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				int LableHeight = sbTabs->ClientHeight / NUMBER_OF_TABS_IN_VIEW;
				int Y = 0;
				int TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
				DBTransaction.Commit();
				if (TabKey != 0)
				{
					DBTransaction.StartTransaction();
					TTouchBtn *cbTabName = new TTouchBtn(this);
					cbTabName->Top = Y + Label1->Top;
					cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
					cbTabName->Left = Label1->Left;
					cbTabName->Height = LableHeight - Label1->Top;
					cbTabName->Caption = TempUserInfo.Name;
					cbTabName->Tag = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
					cbTabName->OnMouseClick = TouchButtonTabClick;
					cbTabName->Parent = sbTabs;
					cbTabName->ParentFont = true;
					cbTabName->ButtonColor = SEAT_UNSELECTED_COLOUR;
					Y += LableHeight;
                    MaxScrollPosition += LableHeight;
					lbePartyName->Caption = "Membership Tabs";
					DBTransaction.Commit();
					TouchButtonTabClick(cbTabName);
				}
				else
				{
					if (TempUserInfo.TabEnabled && TabKey == 0 && TempUserInfo.ContactKey != 0)
					{
						DBTransaction.StartTransaction();
						TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
						TDBTab::SetTabOwner(DBTransaction, TabKey, TempUserInfo.ContactKey, TabMember);
						TDBTab::SetTabName(DBTransaction, TabKey, TempUserInfo.Name);
						TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
						TTouchBtn *cbTabName = new TTouchBtn(this);
						cbTabName->Top = Y + Label1->Top;
						cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
						cbTabName->Left = Label1->Left;
						cbTabName->Height = LableHeight - Label1->Top;
						cbTabName->Caption = TempUserInfo.Name;
						cbTabName->Tag = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
						cbTabName->OnMouseClick = TouchButtonTabClick;
						cbTabName->Parent = sbTabs;
						cbTabName->ParentFont = true;
						cbTabName->ButtonColor = SEAT_UNSELECTED_COLOUR;
						Y += LableHeight;
                        MaxScrollPosition += LableHeight;
						lbePartyName->Caption = "Membership Tabs";
						DBTransaction.Commit();
						TouchButtonTabClick(cbTabName);
					}
					else
					{
						lbePartyName->Caption = "No Membership Tab for " + TempUserInfo.Name;
					}
				}
			}
			else if (CurrentTabType == TabStaff)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr <TStringList> List(new TStringList);
				std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
				Staff->GetTabs(DBTransaction, List.get());
				int Y = 0;
				for (int i = 0; i < List->Count; i++)
				{
					int TabKey = int(List->Objects[i]);
					AnsiString Name = List->Strings[i];

					int LableHeight = sbTabs->ClientHeight / NUMBER_OF_TABS_IN_VIEW;
					// Slow
					TTouchBtn *cbTabName = new TTouchBtn(this);
					cbTabName->Top = Y + Label1->Top;
					cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
					cbTabName->Left = Label1->Left;
					cbTabName->Height = LableHeight - Label1->Top;
					cbTabName->Caption = Name;
					cbTabName->Tag = TabKey;
					cbTabName->OnMouseClick = TouchButtonTabClick;
					cbTabName->Parent = sbTabs;
					cbTabName->ParentFont = true;
					cbTabName->ButtonColor = SEAT_UNSELECTED_COLOUR;
					Y += LableHeight;
                    MaxScrollPosition += LableHeight;
				}

				lbePartyName->Caption = "Staff Tabs";
				DBTransaction.Commit();
			}
		}
	}
	else if (CurrentDisplayMode == eTables)
	{
		int LableHeight = sbTabs->ClientHeight / NUMBER_OF_TABS_IN_VIEW;
		int Y = 0;

		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Text = "SELECT DISTINCT " "SEAT.SEATNO SEAT_NUMBER, TAB.TAB_NAME TAB_NAME, TAB.TAB_KEY TAB_KEY " "FROM "
		"TABLES INNER JOIN SEAT ON TABLES.TABLE_KEY = SEAT.TABLE_KEY " "INNER JOIN TAB ON SEAT.TAB_KEY = TAB.TAB_KEY "
		"INNER JOIN ORDERS ON TAB.TAB_KEY = ORDERS.TAB_KEY " "WHERE " "TABLES.TABLE_NUMBER = :TABLE_NUMBER " "ORDER BY " "SEAT.SEATNO";

		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = SelectedTable;
		IBInternalQuery->ExecQuery();
		while (!IBInternalQuery->Eof)
		{
			TTouchBtn *cbTabName = new TTouchBtn(this);
			cbTabName->Top = Y + Label1->Top;
			cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
			cbTabName->Left = Label1->Top;
			cbTabName->Height = LableHeight - Label1->Top;
			if (IBInternalQuery->FieldByName("TAB_NAME")->AsString == "")
			{
				cbTabName->Caption = TGlobalSettings::Instance().SeatLabel + " " + IBInternalQuery->FieldByName("SEAT_NUMBER")->AsString;
			}
			else
			{
				cbTabName->Caption = IBInternalQuery->FieldByName("SEAT_NUMBER")->AsString + "." + IBInternalQuery->FieldByName("TAB_NAME")
				->AsString;
			}
			cbTabName->Tag = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
			// cbTabName->OnClick = TouchButtonTabClick;
			cbTabName->Parent = sbTabs;
			cbTabName->ParentFont = true;
			cbTabName->Color = SEAT_UNSELECTED_COLOUR;
			Y += LableHeight;
            MaxScrollPosition += LableHeight;
			IBInternalQuery->Next();
		}
		DBTransaction.Commit();
	}
	else if (CurrentDisplayMode == eRooms)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

		int LableHeight = sbTabs->ClientHeight / NUMBER_OF_TABS_IN_VIEW;
		int Y = 0;
		if (TDBRooms::GetRoomTab(DBTransaction, SelectedRoomNo) != 0)
		{
			AnsiString RoomCaption = TDBRooms::GetRoomName(DBTransaction, SelectedRoomNo);
			AnsiString TabCaption = TDBRooms::GetPartyName(DBTransaction, SelectedRoomNo);

			TTouchBtn *cbTabName = new TTouchBtn(this);
			cbTabName->Top = Y + Label1->Top;
			cbTabName->Width = pnlTabs->ClientWidth - (Label1->Top * 2);
			cbTabName->Left = Label1->Top;
			cbTabName->Height = LableHeight - Label1->Top;
			cbTabName->Caption = RoomCaption + " " + TabCaption;
			cbTabName->Tag = TDBRooms::GetRoomTab(DBTransaction, SelectedRoomNo);
			// cbTabName->OnClick = TouchButtonTabClick;
			cbTabName->Parent = sbTabs;
			cbTabName->ParentFont = true;
			cbTabName->ButtonColor = SEAT_UNSELECTED_COLOUR;
			Y += LableHeight;
            MaxScrollPosition += LableHeight;
		}
		DBTransaction.Commit();
	}
}

void __fastcall TfrmTabManager::btnCloseClick(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTabManager::pnlTabsResize(TObject *Sender)
{
	sbTabs->Align = alClient;
	sbTabs->Align = alNone;
	sbTabs->Width += 20;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTabManager::btnSeatsUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int scrolledBy  = pnlTabs->Height/2;
    if( ScrollPosition - scrolledBy < 0)
     {
         scrolledBy =  ScrollPosition;
     }
    if (ScrollPosition > 0)
	{
        sbTabs->ScrollBy( 0, scrolledBy );
        sbTabs->Update();
        ScrollPosition -= scrolledBy;
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnSeatsDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
     int scrolledBy  = pnlTabs->Height/2;
     if( ScrollPosition + scrolledBy + pnlTabs->Height > MaxScrollPosition )
     {
        scrolledBy =   MaxScrollPosition - pnlTabs->Height - ScrollPosition + 10;
     }

    if (ScrollPosition < (MaxScrollPosition - pnlTabs->Height))
	{
        sbTabs->ScrollBy( 0, - scrolledBy );
        sbTabs->Update();
        ScrollPosition += scrolledBy;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TfrmTabManager::TouchButtonTabClick(TObject *Sender)
{
	// Can we get a lock on the New Tab.
	if (CurrentTabBtn == NULL)
	{
		CurrentTabBtn = (TTouchBtn*)Sender;
	}

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	TTouchBtn *NewBtn = (TTouchBtn*)Sender;
	if (TDBTab::GetTabExists(DBTransaction, NewBtn->Tag))
	{
		if (NewBtn->Tag != -1 && NewBtn->Tag != 0)
		{
			AnsiString TabPIN = TDBTab::GetTabPIN(DBTransaction, NewBtn->Tag);
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			bool Proceed = false;
			if (TabPIN != "" && !Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckTabPINOverride))
			{
				std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
				frmTouchNumpad->Caption = "Enter the PIN for this Tab";
				frmTouchNumpad->btnSurcharge->Caption = "Ok";
				frmTouchNumpad->btnDiscount->Visible = false;
				frmTouchNumpad->btnSurcharge->Visible = true;
				frmTouchNumpad->Mode = pmPIN;
				if (frmTouchNumpad->ShowModal() == mrOk)
				{
					if (frmTouchNumpad->STRResult == TabPIN)
					{
						Proceed = true;
					}
					else
					{
						MessageBox("Incorrect PIN", "Error", MB_OK);
					}
				}
			}
			else
			{
				Proceed = true;
			}

			if (Proceed)
			{

				if (TDBTab::LockTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,NewBtn->Tag))
				{ // Yes
					if (CurrentTabBtn)
					{
						if (CurrentTabBtn->Tag != 0 && (NewBtn->Tag != CurrentTabBtn->Tag))
						{
							TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,CurrentTabBtn->Tag);
						}
						CurrentTabBtn->Color = SEAT_UNSELECTED_COLOUR;
						CurrentTabBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
					}

					CurrentTabBtn = (TTouchBtn*)Sender;
					CurrentTabBtn->Color = SEAT_SELECTED_COLOUR;
					CurrentTabBtn->Font->Color = SEAT_SELECTED_FONT_COLOUR;
				}
				else
				{ // No
					if (NewBtn->Color == SEAT_ALREADYLOCKED_COLOUR)
					{
						AnsiString LockedBy = TDBTab::GetLocker(DBTransaction,((TComponent*)Sender)->Tag);
						if (MessageBox("This Seat/Tab is in use by " + LockedBy + "\rDo you wish to override this lock", "Warning",
									MB_YESNO + MB_ICONWARNING) == ID_YES)
						{
							if (CurrentTabBtn->Tag != 0)
							{
								TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,CurrentTabBtn->Tag);
							}
							if (TDBTab::LockTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,NewBtn->Tag))
							{
								if (CurrentTabBtn)
								{
									CurrentTabBtn->Color = SEAT_UNSELECTED_COLOUR;
									CurrentTabBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
								}

								CurrentTabBtn = (TTouchBtn*)Sender;
								CurrentTabBtn->Color = SEAT_SELECTED_COLOUR;
								CurrentTabBtn->Font->Color = SEAT_SELECTED_FONT_COLOUR;
							}
							else
							{
								CurrentTabBtn->Color = SEAT_UNSELECTED_COLOUR;
								CurrentTabBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
								CurrentTabBtn = NULL;
								MessageBox("Unable to Unlock " + TGlobalSettings::Instance().SeatLabel + "/Tab Try again Later", "Warning", MB_OK + MB_ICONWARNING);
							}
						}
					}
					else
					{
						CurrentTabBtn->Color = SEAT_UNSELECTED_COLOUR;
						CurrentTabBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
						CurrentTabBtn = NULL;
						NewBtn->Color = SEAT_ALREADYLOCKED_COLOUR;
						NewBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
					}
				}

				if (CurrentTabBtn)
				{
					SelectedTab = CurrentTabBtn->Tag;
				}
			}
		}
		else
		{
			if (CurrentTabBtn)
			{
				if (CurrentTabBtn->Tag != -1 && CurrentTabBtn->Tag != 0)
				{
					TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,CurrentTabBtn->Tag);
					CurrentTabBtn->Color = SEAT_UNSELECTED_COLOUR;
					CurrentTabBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
				}
				else
				{
					CurrentTabBtn->Color = SEAT_UNSELECTED_COLOUR;
					CurrentTabBtn->Font->Color = SEAT_UNSELECTED_FONT_COLOUR;
				}
				CurrentTabBtn = (TTouchBtn*)Sender;
				CurrentTabBtn->Color = SEAT_SELECTED_COLOUR;
				CurrentTabBtn->Font->Color = SEAT_SELECTED_FONT_COLOUR;
				TDBTab::LockTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,CurrentTabBtn->Tag);
			}
			SelectedTab = CurrentTabBtn->Tag;
		}
	}
	else
	{
		MessageBox("Tab no longer Exists", "Error", MB_OK);
	}
	DBTransaction.Commit();

	RefreshTabDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnAddNewTabClick(TObject *Sender)
{
	std::auto_ptr <TfrmAddTab> frmAddTab(TfrmAddTab::Create(this));
    bool IsUserAccess = true;

	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();

    TMMContactInfo currentUserInfo = TDeviceRealTerminal::Instance().User;
    std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
    if (!Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckAccountCreation))
    {
        MessageBox("You do not have the privileges to create a new tab!", "Error", MB_OK + MB_ICONERROR);
        IsUserAccess = false;
    }
    frmAddTab->LoadDetails(DBTransaction, 0);
    DBTransaction.Commit();
    if(IsUserAccess)
    {
        if (CurrentTabType == TabMember && TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ReadOnlyInterface)
        {
            MessageBox("You must Add,Edit and Delete members from your 3rd party Membership software.", "Warning", MB_ICONWARNING + MB_OK);
        }
        if (CurrentTabType == TabMember)
        {
            TempUserInfo.Clear();
            TDeviceRealTerminal::Instance().ManagerMembership->AddMember(TempUserInfo);
            ShowTabsDetails();
        }
        else if (frmAddTab->ShowModal() == mrOk)
        {
            DBTransaction.StartTransaction();
            if (TDBTab::GetTab(DBTransaction, frmAddTab->TabName, TabNormal) != 0)
            {
                MessageBox("A Tab with this name already exists. Please select a different name.", "Warning", MB_OK + MB_ICONWARNING);
            }
            else
            {
                int TabyKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
                TDBTab::SetTabDetails(DBTransaction, TabyKey, frmAddTab->TabName, frmAddTab->TabIDType, frmAddTab->TabIDNumber,
                frmAddTab->TabIDExpiry, TabNormal);
                TDBTab::SetTabCard(DBTransaction, TabyKey, frmAddTab->CardString);
                SelectedTable = 0;
                SelectedSeat = 0;
                SelectedTabName = frmAddTab->TabName;
            }
            DBTransaction.Commit();
        }
        ShowTabsDetails();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnPermanentClick(TObject *Sender)
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	if (TDBTab::GetTabPermanent(DBTransaction, SelectedTab))
	{
		TDBTab::SetTabPermanent(DBTransaction, SelectedTab, false);
	}
	else
	{
		TDBTab::SetTabPermanent(DBTransaction, SelectedTab, true);
	}
	DBTransaction.Commit();
	RefreshTabDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnLockTabClick(TObject *Sender)
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	if (TDBTab::GetTabLocked(DBTransaction, SelectedTab))
	{
		TDBTab::SetTabLocked(DBTransaction, SelectedTab, false, "");
		btnLockTab->Caption = "Lock Tab";
	}
	else
	{
		std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
		frmTouchKeyboard->MaxLength = 80;
		frmTouchKeyboard->AllowCarriageReturn = false;
		frmTouchKeyboard->StartWithShiftDown = true;
		frmTouchKeyboard->KeyboardText = "";
		frmTouchKeyboard->Caption = "Please enter a Tab lock reason.";
		if (frmTouchKeyboard->ShowModal() == mrOk)
		{
			if (frmTouchKeyboard->KeyboardText == "")
			{
				do
				{
					MessageBox("You must enter a Tab lock reason.", "Please Comply", MB_OK + MB_ICONINFORMATION);
					frmTouchKeyboard->ShowModal();
				}
				while (frmTouchKeyboard->KeyboardText == "");
			}
			TDBTab::SetTabLocked(DBTransaction, SelectedTab, true, frmTouchKeyboard->KeyboardText);
			btnLockTab->Caption = "Unlock Tab";
		}
	}
	DBTransaction.Commit();
	RefreshTabDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnAddCreditToTabClick()
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();

		bool Proceed = true;
		if (CurrentDisplayMode == eTabs && CurrentTabType == TabStaff)
		{
			Proceed = false;
			TMMContactInfo StaffUserInfo;
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			TLoginSuccess Result = Staff->Login(this, DBTransaction, StaffUserInfo, CheckPOS);
			if (Result == lsAccepted)
			{
				TDeviceRealTerminal::Instance().User = StaffUserInfo;
				if (StaffUserInfo.ContactKey != TDBTab::GetTabOwner(DBTransaction, SelectedTab))
				{
					Proceed = true;
				}
				else
				{
					MessageBox("You cannot add credit to your own Tab.", "Error", MB_OK + MB_ICONERROR);
				}
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access to P.O.S.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
		}

		if (Proceed)
		{
			std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
			frmTouchNumpad->Caption = "Enter the amount of credit to Add/Deduct";
			frmTouchNumpad->CURInitial = 0;
			frmTouchNumpad->btnSurcharge->Caption = "Add Credit";
			frmTouchNumpad->btnDiscount->Caption = "Deduct Credit";
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->Mode = pmCurrency;
			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				if (frmTouchNumpad->CURResult != 0)
				{
					TPaymentTransaction CreditTransaction(DBTransaction);
					CreditTransaction.Type = eTransCreditPurchase;
					CreditTransaction.SalesType = eCreditPurchase;
					CreditTransaction.Money.Change = 0;
					CreditTransaction.Money.CreditAvailable = 0;
					CreditTransaction.Money.PaymentAmount = 0;
					// CreditTransaction.Money.Total = 0;
					CreditTransaction.Money.GrandTotal = frmTouchNumpad->CURResult;
					CreditTransaction.Money.PaymentDue = 0;
					TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, CreditTransaction.Patrons, 1);

					TTabCredit Credit = CreditTransaction.TabCredit[SelectedTab];
					Credit.CurrentCredit = TDBTab::GetTabCredit(DBTransaction, SelectedTab);
					Credit.CreditRedeemed = -frmTouchNumpad->CURResult; // Adding Credit.
					Credit.PaymentGroupNumber = TGlobalSettings::Instance().CreditPaymentGroupNumber;
					CreditTransaction.TabCredit[SelectedTab] = Credit;

					if (TempUserInfo.ContactKey == TDBTab::GetTabOwner(DBTransaction, SelectedTab))
					{
						CreditTransaction.Membership.Assign(TempUserInfo, emsManual);
					}

					TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(CreditTransaction);
					if (TDBTab::GetTabCreditLimit(DBTransaction, SelectedTab) == -1)
					{
                        // Zero the Credit limit ( no credit) if they havn't already got one assigned.
						TDBTab::SetTabCreditLimit(DBTransaction, SelectedTab, 0);
					}

                    if(CurrentTabType == TabClipp)
                    {
                        //send clipp tab details back
//                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
//                        sendClippTabKey->SendTabDetails(SelectedTab);
                    }
				}
			}
		}
		DBTransaction.Commit();
		RefreshTabDetails();

	}
	catch(Exception & E)
	{
		MessageBox("Unable to Process this Bill.\r" + E.Message + "\rPlease try again later.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnRefundCreditToTabClick()
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();

		bool Proceed = true;
		if (CurrentDisplayMode == eTabs && CurrentTabType == TabStaff)
		{
			Proceed = false;
			TMMContactInfo TempUserInfo;
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckPOS);
			if (Result == lsAccepted)
			{
				TDeviceRealTerminal::Instance().User = TempUserInfo;
				if (TempUserInfo.ContactKey != TDBTab::GetTabOwner(DBTransaction, SelectedTab))
				{
					Proceed = true;
				}
				else
				{
					MessageBox("You cannot refund credit from your own Tab.", "Error", MB_OK + MB_ICONERROR);
				}
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access to P.O.S.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}
		}

		if (Proceed)
		{
            std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
			frmTouchNumpad->Caption = "Enter the amount of credit to Add/Deduct";
			frmTouchNumpad->CURInitial = 0;
			frmTouchNumpad->btnSurcharge->Caption = "Deduct Credit";
			frmTouchNumpad->btnDiscount->Visible = false;
			frmTouchNumpad->btnSurcharge->Visible = true;
			frmTouchNumpad->Mode = pmCurrency;
			if (frmTouchNumpad->ShowModal() == mrOk)
			{
				if (frmTouchNumpad->CURResult != 0)
				{
                    TPaymentTransaction CreditTransaction(DBTransaction);
                    CreditTransaction.Type = eTransCreditPurchase;
                    CreditTransaction.SalesType = eCreditPurchase;
                    CreditTransaction.Money.Change = 0;
                    CreditTransaction.Money.CreditAvailable = 0;
                    CreditTransaction.Money.PaymentAmount = 0;
                    CreditTransaction.Money.GrandTotal = frmTouchNumpad->CURResult;
                    CreditTransaction.Money.PaymentDue = frmTouchNumpad->CURResult;

                    TTabCredit Credit = CreditTransaction.TabCredit[SelectedTab];
                    Credit.CurrentCredit = TDBTab::GetTabCredit(DBTransaction, SelectedTab);
                    Credit.CreditRedeemed = frmTouchNumpad->CURResult;
                    CreditTransaction.TabCredit[SelectedTab] = Credit;

                    if(Credit.CurrentCredit >= Credit.CreditRedeemed)
                    {
                        TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, CreditTransaction.Patrons, 1);
                        TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(CreditTransaction);
                        TDBTab::SetTabCreditLimit(DBTransaction, SelectedTab, (Credit.CurrentCredit + Credit.CreditRedeemed));
                    }
                    else
                    {
                        MessageBox("Credit entered is greater than available credit.", "Error", MB_OK + MB_ICONERROR);
                    }
                }
            }
		}
		DBTransaction.Commit();
		RefreshTabDetails();
	}
	catch(Exception & E)
	{
		MessageBox("Unable to Process this Bill.\r" + E.Message + "\rPlease try again later.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnChangeDetailsClick(TObject *Sender)
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}
	std::auto_ptr <TfrmAddTab> frmAddTab(TfrmAddTab::Create(this));
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	frmAddTab->LoadDetails(DBTransaction, SelectedTab);
	DBTransaction.Commit();

	if (CurrentTabType == TabMember && TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->ReadOnlyInterface)
	{
		MessageBox("You must Add,Edit and Delete members from your 3rd party Membership software.", "Warning", MB_ICONWARNING + MB_OK);
	}
	else if (CurrentTabType == TabMember)
	{
		DBTransaction.StartTransaction();
		if (TDBTab::GetTabOwner(DBTransaction, SelectedTab) == 0)
		{
			MessageBox("This Member Tab has not been assigned to a member. Please select one now.", "Error", MB_OK + MB_ICONERROR);
			TMMContactInfo TempUserInfo;
			eMemberSource MemberSource;
			TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo, MemberSource);
			if (Result == lsAccepted)
			{
				if (TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey) == 0)
				{
					TDBTab::SetTabDetails(DBTransaction, SelectedTab, TempUserInfo.Name, frmAddTab->TabIDType, frmAddTab->TabIDNumber,
					frmAddTab->TabIDExpiry, TabMember);
					TDBTab::SetTabOwner(DBTransaction, SelectedTab, TempUserInfo.ContactKey, TabMember);
					TDBTab::SetTabName(DBTransaction, SelectedTab, TempUserInfo.Name);
				}
				else
				{
					MessageBox("This Member already has a Tab.\rTo merge these tabs use the Transfer faculty.", "Error", MB_OK + MB_ICONERROR);
				}
			}
		}
		else
		{
			TempUserInfo.Clear();
			TempUserInfo.ContactKey = TDBTab::GetTabOwner(DBTransaction, SelectedTab);
			TDeviceRealTerminal::Instance().ManagerMembership->EditMember(DBTransaction, TempUserInfo);
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, TempUserInfo.ContactKey, TempUserInfo);
			ShowTabsDetails();
		}
		DBTransaction.Commit();
	}
	else if (CurrentTabType == TabStaff)
	{
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		if (TDBTab::GetTabOwner(DBTransaction, SelectedTab) == 0) // Staff tab not set to a staff member.
		{
			MessageBox("This Staff Tab has not been assigned to a staff member. Please select one now.", "Error", MB_OK + MB_ICONERROR);
			TMMContactInfo TempUserInfo;
			TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckNone);
			if (Result == lsAccepted)
			{
				if (TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey) == 0)
				{
					TDBTab::SetTabDetails(DBTransaction, SelectedTab, TempUserInfo.Name, frmAddTab->TabIDType, frmAddTab->TabIDNumber,
					frmAddTab->TabIDExpiry, TabStaff);
					TDBTab::SetTabOwner(DBTransaction, SelectedTab, TempUserInfo.ContactKey, TabStaff);
					TDBTab::SetTabName(DBTransaction, SelectedTab, TempUserInfo.Name);
				}
				else
				{
					MessageBox("This Staff Member already has a Tab.\rTo merge these tabs use the Transfer faculty.", "Error",
					MB_OK + MB_ICONERROR);
				}
			}
		}
		else
		{
			TDBTab::SetTabDetails(DBTransaction, SelectedTab, Staff->GetContactName(DBTransaction, TDBTab::GetTabOwner(DBTransaction,
			SelectedTab)), frmAddTab->TabIDType, frmAddTab->TabIDNumber, frmAddTab->TabIDExpiry, TabStaff);
		}
		DBTransaction.Commit();
	}
	else if (frmAddTab->ShowModal() == mrOk)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TDBTab::SetTabDetails(DBTransaction, SelectedTab, frmAddTab->TabName, frmAddTab->TabIDType, frmAddTab->TabIDNumber,
		frmAddTab->TabIDExpiry, TabNormal);
		SelectedTabName = frmAddTab->TabName;
		TDBTab::SetTabCard(DBTransaction, SelectedTab, frmAddTab->CardString);
		DBTransaction.Commit();
	}
	ShowTabsDetails();
	RefreshTabDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnSetCreditLimitClick()
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}
	std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
	frmTouchNumpad->Caption = "Enter the credit Limit.";

	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	if (TDBTab::GetTabCreditLimit(DBTransaction, SelectedTab) == -1)
	{
		frmTouchNumpad->CURInitial = 0;
	}
	else
	{
		frmTouchNumpad->CURInitial = TDBTab::GetTabCreditLimit(DBTransaction, SelectedTab);
	}
	frmTouchNumpad->btnSurcharge->Caption = "Set\r Credit Limit";
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->btnDiscount->Caption = "No\r Credit Limit";
	frmTouchNumpad->btnDiscount->Visible = true;
	frmTouchNumpad->Mode = pmCurrency;
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		if (frmTouchNumpad->BtnExit == 1)
		{
			TDBTab::SetTabCreditLimit(DBTransaction, SelectedTab, frmTouchNumpad->CURResult);
		}
		else if (frmTouchNumpad->BtnExit == 2)
		{
			TDBTab::SetTabCreditLimit(DBTransaction, SelectedTab, -1);
		}
	}
	DBTransaction.Commit();
	RefreshTabDetails();
}

// ---------------------------------------------------------------------------

void TfrmTabManager::RefreshTabDetails()
{
	if (!SelectedTab)
	{
		lbeTabName->Caption = "Please Select a Tab.";
		lbeIDType->Caption = "";
		lbeIDNumber->Caption = "";
		lbeIDExpire->Caption = "";
		lbeCredit->Caption = "";
		lbeBalance->Caption = "Balance Owing";
		lbeCreditLimit->Caption = "";
		lbeLockedReason->Caption = "";
		lbeTotalPoints->Caption = "";
		btnSubsidisedProfile->Enabled = false;
	}
	else
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		if (CurrentTabType == TabNormal)
		{
			btnSubsidisedProfile->Enabled = true;
		}
		UnicodeString Name, IDType, IDNumber, IDExpire;
		TMMTabType TabType;
		TDBTab::GetTabDetails(DBTransaction, SelectedTab, Name, IDType, IDNumber, IDExpire, TabType);
		lbeTabName->Caption = Name;
		lbeIDType->Caption = IDType;
		lbeIDNumber->Caption = IDNumber;
		lbeIDExpire->Caption = IDExpire;

		int TabOwner = TDBTab::GetTabOwner(DBTransaction, SelectedTab);
		if (TabOwner != 0 && (TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactType(DBTransaction,
						TabOwner) == eMember || TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactType(DBTransaction,
						TabOwner) == eERSMember))
		{
			TMMContactInfo Member;
			Member.ContactKey = TabOwner;
			TLoginSuccess LoginSuccess = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(DBTransaction,Member);
			if(LoginSuccess == LoginSuccess)
			{
				lbeTotalPoints->Caption = FloatToStrF(Member.Points.getPointsBalance(), ffFixed, 15, 2);
				btnManInvoice->Enabled = true;
			}
		}
		else
		{
			lbeTotalPoints->Caption = "NA";
			btnManInvoice->Enabled = false;
		}

		if (TDBTab::GetTabBalance(DBTransaction, SelectedTab) < 0)
		{
			lbeBalance->Caption = "In Credit";
			lbeCredit->Caption = FloatToStrF(-TDBTab::GetTabBalance(DBTransaction, SelectedTab), ffCurrency, 15, 2);
		}
		else
		{
			lbeBalance->Caption = "Balance Owing";
			lbeCredit->Caption = FloatToStrF(TDBTab::GetTabBalance(DBTransaction, SelectedTab), ffCurrency, 15, 2);
		}

		if (TDBTab::GetTabCreditLimit(DBTransaction, SelectedTab) == -1)
		{
			lbeCreditLimit->Caption = "None";
		}
		else
		{
			lbeCreditLimit->Caption = FloatToStrF(TDBTab::GetTabCreditLimit(DBTransaction, SelectedTab), ffCurrency, 15, 2);
		}
		lbeLockedReason->Caption = TDBTab::GetTabLockedReason(DBTransaction, SelectedTab);
		// Update Perminance.
		if (TDBTab::GetTabPermanent(DBTransaction, SelectedTab))
		{
			btnPermanent->Color = clRed;
			btnPermanent->Caption = "Make Tab Temporary";
		}
		else
		{
			btnPermanent->Color = clGreen;
			btnPermanent->Caption = "Make Tab Permanent";
		}
		// Update Locked Unlocked Status.
		if (TDBTab::GetTabLocked(DBTransaction, SelectedTab))
		{
			btnLockTab->Color = clRed;
			btnLockTab->Caption = "Unlock Tab";
		}
		else
		{
			btnLockTab->Color = clGreen;
			btnLockTab->Caption = "Lock Tab";
		}
		if (TDBTab::GetTabPIN(DBTransaction, SelectedTab) != "")
		{
			btnPINTab->Color = clRed;
			btnPINTab->Caption = "Change PIN";
		}
		else
		{
			btnPINTab->Color = clGreen;
			btnPINTab->Caption = "Set Tab PIN";
		}
		DBTransaction.Commit();
	}
}

void __fastcall TfrmTabManager::FormResize(TObject *Sender)
{
	double ratio = Tag/Screen->Width;
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
	tsTabManager->Height = ClientHeight;
	Panel2->Height = tsTabManager->Height - 30;
	Panel3->Height = Panel2->Height;
	pnlDisplay->Height = Panel2->Height;
	pnlDisplay->Left = Panel2->Left + Panel2->Width;
	Panel3->Left = pnlDisplay->Left + pnlDisplay->Width;
	pnlTabs->Height = Panel2->ClientHeight - pnlTabs->Top - Label1->Top;
	btnSeatsDown->Top = pnlTabs->Top + pnlTabs->Height - btnSeatsDown->Height;
	btnSeatsUp->Top = pnlTabs->Top;
	btnClose->Height = (ClientHeight - Panel3->Height) + 30;
	btnClose->Left = 10;
	TTouchBtn* buttons[10] = {btnAddNewTab,btnChangeDetails,TouchButton1,btnTabCredit,
		btnPINTab,btnLockTab,btnPermanent,btnRemoveTab,btnManInvoice,btnSubsidisedProfile};
	double top = btnClose->Top + btnClose->Height + 5 ;
	double left = 10;
	for(int i =0; i<10; i++)
	{
		buttons[i]->Height = btnClose->Height * 1.5;
		if(top + buttons[i]->Height > Panel3->Height)
		{
			left += (buttons[i]->Width + 6);
			top = btnClose->Top + btnClose->Height + 5 ;
		}
		buttons[i]->Left = left;
		buttons[i]->Top = top;
		top += (buttons[i]->Height + 5);
	}
	lbeLockedReason->Height =  pnlDisplay->Height * 0.30;
	tgDiscounts->Top = Panel1->Top;
	tgDiscounts->Width = ClientWidth - (Panel1->Width + 20);
	tgDiscounts->Height = Panel1->Height;
	tgDiscounts->Left = 10;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnPINTabClick(TObject *Sender)
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	AnsiString TempPIN1, TempPIN2;

	std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
	frmTouchNumpad->Caption = "Enter PIN";
	frmTouchNumpad->btnSurcharge->Caption = "Ok";
	frmTouchNumpad->btnDiscount->Visible = false;
	frmTouchNumpad->btnSurcharge->Visible = true;
	frmTouchNumpad->Mode = pmPIN;
	frmTouchNumpad->StrInitial = "";
	if (frmTouchNumpad->ShowModal() == mrOk)
	{
		TempPIN1 = frmTouchNumpad->STRResult;
		frmTouchNumpad->Caption = "Confirm PIN";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmPIN;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			TempPIN2 = frmTouchNumpad->STRResult;

			if (TempPIN1 == TempPIN2)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				TDBTab::SetTabPIN(DBTransaction, SelectedTab, TempPIN1);
				DBTransaction.Commit();
			}
			else
			{
				MessageBox("Mismatch PINs, please try again.", "Error", MB_OK);
			}
		}
	}
	RefreshTabDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::TouchButton1Click(TObject *Sender)
{
	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	AnsiString CardString;
	std::auto_ptr <TfrmCardSwipe> frmCardSwipe(TfrmCardSwipe::Create <TfrmCardSwipe> (this));
	frmCardSwipe->ShowModal();
	if (frmCardSwipe->ModalResult == mrOk || frmCardSwipe->ModalResult == mrAbort)
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();

		CardString = AnsiString(frmCardSwipe->SwipeString).SubString(1, 80);

		if (SelectedTab != 0) // Assign Card to valid tab.
		{
			TDBTab::SetTabCard(DBTransaction, SelectedTab, CardString);
		}

		if (TDBTab::GetTabOwner(DBTransaction, SelectedTab) != 0) // Retrive the Tabs Owner if any.
		{
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, TDBTab::GetTabOwner(DBTransaction, SelectedTab),
			TempUserInfo);
		}

		if (TempUserInfo.ContactKey != 0) // Set the Tabs owner or current members card.
		{
			TempUserInfo.CardStr = CardString;
			TempUserInfo.CardsToAdd.insert(CardString);
			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SetContactDetails(DBTransaction, TempUserInfo.ContactKey, TempUserInfo);
		}
		DBTransaction.Commit();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnManInvoiceClick(TObject *Sender)
{
	const AnsiString Category = "Manual Invoices";
	if (!SelectedTab)
	{
		MessageBox("Please select a Member to work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
	frmTouchKeyboard->MaxLength = 50;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->KeyboardText = "Manual Adjustment";
	frmTouchKeyboard->Caption = "Enter an Invoice Description";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		AnsiString ItemDescription = frmTouchKeyboard->KeyboardText;
		std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
		frmTouchNumpad->Caption = "Enter the total value of the invoice";
		frmTouchNumpad->CURInitial = 0;
		frmTouchNumpad->btnSurcharge->Caption = "Charge";
		frmTouchNumpad->btnDiscount->Caption = "Credit";
		frmTouchNumpad->btnDiscount->Visible = true;
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->Mode = pmCurrency;
		if (frmTouchNumpad->ShowModal() == mrOk)
		{
			Currency TotalPrice = frmTouchNumpad->CURResult;
			Currency TotalCostExcl = 0;
            Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
            std::vector<TTaxProfile> taxProfiles = TDBTax::GetAllProfilesOfType(DBTransaction,0);
            DBTransaction.Commit();
            std::auto_ptr <TfrmVerticalSelect> TaxSelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));
            TVerticalSelection CancelItem;
			CancelItem.Title = "Cancel";
			CancelItem.Properties["Color"] = IntToStr(clMaroon);
			CancelItem.CloseSelection = true;
			TaxSelectionForm->Items.push_back(CancelItem);


            for(std::vector<TTaxProfile>::iterator taxIt = taxProfiles.begin(); taxIt!= taxProfiles.end();std::advance(taxIt,1))
            {
               TVerticalSelection TaxItem;
               TaxItem.Title = taxIt->Name + " (" + CurrToStr(taxIt->Rate) + " %)";
			   TaxItem.Properties["TAX_KEY"] = IntToStr(taxIt->ProfileKey);
			   TaxItem.Properties["Color"] = IntToStr(clNavy);
			   TaxItem.CloseSelection = true;
			   TaxSelectionForm->Items.push_back(TaxItem);
            }


            TaxSelectionForm->ShowModal();
 			TVerticalSelection SelectedTaxItem;
            if (TaxSelectionForm->GetFirstSelectedItem(SelectedTaxItem) && SelectedTaxItem.Title != "Cancel")
			 {
				DBTransaction.StartTransaction();

                 int selectedTaxKey = StrToIntDef(SelectedTaxItem.Properties["TAX_KEY"], 0);
                TTaxProfileDBAccessManager_MM taxProfileManager;
                TaxProfile selectedtaxProfile = taxProfileManager.LoadTaxProfileFromKey(DBTransaction, selectedTaxKey);

                				// Get the Financial Category for this invoice.
				std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));
				std::auto_ptr <TStringList> CategoryList(new TStringList);

				TVerticalSelection Item;
				Item.Title = "Cancel";
				Item.Properties["Color"] = IntToStr(clMaroon);
				Item.CloseSelection = true;
				SelectionForm->Items.push_back(Item);

				TDBOrder::GetCategoryList(DBTransaction, CategoryList.get());
				int OrderServingCourseKey = TDeviceRealTerminal::Instance().Menus->GetServingCourseByName(DBTransaction, "No Serving Course");

				if (OrderServingCourseKey == 0)
				{
					MessageBox("Unable to locate System Serving course 'No Serving Course'", "Error", MB_OK);
				}
				else
				{
					for (int i = 0; i < CategoryList->Count; i++)
					{
						TVerticalSelection Item;
						Item.Title = CategoryList->Strings[i];
						Item.Properties["CATEGORY_KEY"] = IntToStr((int)CategoryList->Objects[i]);
						Item.Properties["Color"] = IntToStr(clNavy);
						Item.CloseSelection = true;
						SelectionForm->Items.push_back(Item);
					}

					SelectionForm->ShowModal();

					TVerticalSelection SelectedItem;
					if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
					{
						AnsiString FinancialCategory = SelectedItem.Title;
						int FinancialCategoryKey = StrToIntDef(SelectedItem.Properties["CATEGORY_KEY"], 0);
						TotalCostExcl = (TotalPrice) * 100 / (selectedtaxProfile.taxPercentage + 100);
						// Assign Tab To Invoice.
						int Owner = TDBTab::GetTabOwner(DBTransaction, SelectedTab);
						TMMContactInfo Info;
						TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, Owner, Info);

						int InvoiceKey = Invoice->CreateInvoice(DBTransaction, Owner, TotalCostExcl, TotalPrice, "");

						UnicodeString Name, IDType, Number, Date, Card;
						TMMTabType TabType;
						TDBTab::GetTabDetails(DBTransaction, SelectedTab, Name, IDType, Number, Date, TabType);

						int InvoiceTab = TDBTab::GetOrCreateTab(DBTransaction, 0);
						TDBTab::SetTabDetails(DBTransaction, InvoiceTab, Name, IDType, Number, Date, TabMember);
						TDBTab::SetTabOwner(DBTransaction, InvoiceTab, Owner, TabMember);
						TDBTab::SetTabToInvoice(DBTransaction, InvoiceTab, InvoiceKey);

						TPaymentTransaction PaymentTransaction(DBTransaction);
						// Print Invoice.
						std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
						try
						{
							PaymentTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, InvoiceKey);

							TItemComplete *Order = new TItemComplete;
							Order->TabKey = InvoiceTab;
							Order->TableNo = 0;
							Order->SeatNo = 0;
							Order->Item = ItemDescription;
							Order->Item_ID = 9999;
							Order->MenuName = "Manual Invoice";

							Order->Categories->FinancialCategory = FinancialCategory;
							Order->Categories->FinancialCategoryKey = FinancialCategoryKey;
							Order->Categories->FinancialCategoryGroup = TDBOrder::GetCategoryGroup(DBTransaction,
							TDBOrder::GetCategoriesCategoryGroup(DBTransaction, Order->Categories->FinancialCategoryKey));
							// TODO 4 -o  Michael -c Printing : Remove When Printing Engine Finished
							Order->MenuKey = 0;
							Order->ItemType = eFoodItem;
							Order->PrintChitNumber = false;
							Order->OrderType = NormalOrder;
							Order->HappyHour = false;
							Order->TimeStamp = Now();
							Order->TransNo = TDBOrder::GetNextTransNumber(DBTransaction);

							Order->SetPriceLevelCustom(TotalPrice);
							Order->Note = ItemDescription;
							Order->Course = "";
							Order->CourseKitchenName = "";
							Order->SizeKitchenName = "";
							Order->Size = "Default";
							Order->ItemKitchenName = "";
							//Order->GSTPercent = frmDiscount->PERCResult;
							Order->TaxProfiles.push_back(selectedtaxProfile);
							Order->PointsPercent = 0;
							Order->SetMenuMask = 0;
							Order->SetMenu = false;
							Order->SetMenuMaster = false;
							Order->Cost = 0;
							Order->CostGSTPercent = selectedtaxProfile.taxPercentage;
							Order->PriceLevel0 = TotalPrice;
							Order->PriceLevel1 = TotalPrice;
							// Retrives the
							Order->ServingCourse = TDeviceRealTerminal::Instance().Menus->GetServingCourseFromDB(DBTransaction, OrderServingCourseKey);

							// Order Categories.
							TItemSizeCategory *InvoiceCat = new TItemSizeCategory;
							InvoiceCat->Category = Category;
							InvoiceCat->CategoryKey = TDBOrder::SetArchiveCategory(DBTransaction, InvoiceCat->Category);
							Order->Categories->CategoryAdd(InvoiceCat);

							// Complete Order Security.
							int SecRefNumber = TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction);
							Order->Security->SetSecurityRefNumber(SecRefNumber);
							TSecurityReference *SecRef = new TSecurityReference;
							SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
							SecRef->Event = SecurityTypes[secOrderedBy];
							SecRef->From = TDeviceRealTerminal::Instance().User.Name;
							SecRef->To = TDeviceRealTerminal::Instance().User.Initials;
							SecRef->TimeStamp = Now();
							Order->Security->SecurityAdd(SecRef);

							PaymentTransaction.Orders->Add(Order);
                            PaymentTransaction.IgnoreLoyaltyKey = false;
							PaymentTransaction.Recalc();
							// Put Orders in DB where required.
							TDBOrder::ProcessOrders(DBTransaction, PaymentTransaction.Orders);
							// Put Security Records in the DB as well (incase of power failure).
							for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
							{
								TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
								TDBSecurity::ProcessSecurity(DBTransaction, Order->Security);
								for (int j = 0; j < Order->SubOrders->Count; j++)
								{
									TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
									if (SubOrder)
									{
										TDBSecurity::ProcessSecurity(DBTransaction, SubOrder->Security);
									}
								}
							}

							TempReceipt->Transaction = &PaymentTransaction;
							TempReceipt->JobType = pjReceiptInvoice;
							TempReceipt->SignReceipt = true;
							TempReceipt->SenderType = devPC;
							TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
							TempReceipt->ExtraInfo->Add("Name: " + Info.Name);
							TempReceipt->ExtraInfo->Add("Member No. " + Info.MembershipNumber);
							TempReceipt->PaymentType = ptFinal;

							Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
							TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
							if (TGlobalSettings::Instance().PrintSignatureReceiptsTwice)
							{
								TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
							}
                            //if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
						}
						__finally
						{
							while (PaymentTransaction.Orders->Count)
							{
								delete(TItemComplete*)PaymentTransaction.Orders->Items[0];
								PaymentTransaction.Orders->Delete(0);
							}
						}
					}
				}
				DBTransaction.Commit();
			}
		}
	}
	RefreshTabDetails();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnTabCreditClick(TObject *Sender)
{
	std::auto_ptr <TfrmDropDownFunc> (frmDropDown)(TfrmDropDownFunc::Create <TfrmDropDownFunc> (this));

	frmDropDown->AddButton("Add Credit To Tab", &btnAddCreditToTabClick);
    if(CurrentTabType != TabClipp)
    {
//        frmDropDown->AddButton("Refund Credit From Tab", &btnRefundCreditToTabClick);
//        frmDropDown->AddButton("Set Credit Limit", &btnSetCreditLimitClick);
    }

	if (frmDropDown->ShowModal() == mrOk)
	{
		frmDropDown->FunctionToCall();
        frmDropDown.reset();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::btnRemoveTabClick(TObject *Sender)
{
    bool IsUserAccess = true;
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);


	if (!SelectedTab)
	{
		MessageBox("Please select a Tab to Work with.", "Error", MB_OK + MB_ICONERROR);
		return;
	}

	try
	{
        DBTransaction.StartTransaction();
        TMMContactInfo currentUserInfo = TDeviceRealTerminal::Instance().User;
        std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
        if (!Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckAccountCreation))
        {
            MessageBox("You do not have the privileges to remove a tab!", "Error", MB_OK + MB_ICONERROR);
            IsUserAccess = false;
        }
        DBTransaction.Commit();
        if(IsUserAccess)
        {
            if (MessageBox("Are you sure you wish to remove this Tab?", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
            {
                DBTransaction.StartTransaction();
                TDBTab::CloseTab(DBTransaction, SelectedTab);
                DBTransaction.Commit();
                SelectedTab = 0;
                ShowTabsDetails();
                RefreshTabDetails();
            }
        }
	}
	catch(Exception & E)
	{
		MessageBox("Unable to close this tab.\r" + E.Message + "\rPlease try again later.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTabManager::tgDiscountsMouseClick(TObject *Sender, TMouseButton Button,
TShiftState Shift, TGridButton *GridButton)
{
	std::set <int> ::iterator ptrDiscount;
	ptrDiscount = TempUserInfo.AutoAppliedDiscounts.find(GridButton->Tag);
    AnsiString message = "";
    message = CheckDiscountApplicability(GridButton->Tag);
	try
	{
		if (ptrDiscount == TempUserInfo.AutoAppliedDiscounts.end())
		{ // Not Found.
            // Check if Discount is applicable(should be other than 0% and 100%)
            if(message.Length() != 0)
            {
                MessageBox(message,"Caution",MB_ICONWARNING + MB_OK);
                return;
            }
			TempUserInfo.AutoAppliedDiscounts.insert(GridButton->Tag);
			GridButton->Latched = true;
		}
		else
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction.StartTransaction();
			AnsiString discountCode = ManagerDiscount->GetDiscountCodeByKey(DBTransaction,*ptrDiscount);
			DBTransaction.Commit();

			if(discountCode != "")
			TempUserInfo.AutoAppliedDiscountsID.erase(discountCode);

			TempUserInfo.AutoAppliedDiscounts.erase(ptrDiscount);
			GridButton->Latched = false;
		}
	}
	catch(Exception &err)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,err.Message);
	}

}
//---------------------------------------------------------------------------
void __fastcall TfrmTabManager::btnSubsidisedProfileMouseClick(TObject *Sender)
{
    TempUserInfo.Clear();
	pgControl->ActivePage = tsDiscounts;
	RedrawDiscounts();
}


// ---------------------------------------------------------------------------
void TfrmTabManager::RedrawDiscounts()
{
	tgDiscounts->ColCount = 0;
	tgDiscounts->RowCount = 0;

	tgDiscounts->ColCount = 3;
	tgDiscounts->RowCount = 8;

	std::auto_ptr<TStringList>List(new TStringList);
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	ManagerDiscount->GetDiscountListByPriority(DBTransaction,List.get());

	for (int i = 0; i < tgDiscounts->RowCount; i++)
	{
		for (int j = 0; j < tgDiscounts->ColCount; j++)
		{
			int Index = ((i * 3) + j);
			if (List->Count > Index)
			{
				tgDiscounts->Buttons[i][j]->Visible = true;
				tgDiscounts->Buttons[i][j]->Caption = List->Strings[Index];
				tgDiscounts->Buttons[i][j]->Color = clNavy;
				tgDiscounts->Buttons[i][j]->DisabledColor = clBtnFace;
				tgDiscounts->Buttons[i][j]->FontColor = clWhite;
				tgDiscounts->Buttons[i][j]->Tag = (int)List->Objects[Index];
			}
			else
			{
				tgDiscounts->Buttons[i][j]->Caption = "";
				tgDiscounts->Buttons[i][j]->Visible = false;
			}
		}
	}
	TempUserInfo.Clear();
	TempUserInfo=  TDBTab::GetDiscountDetails(DBTransaction, SelectedTab, TempUserInfo);
	DBTransaction.Commit();

	for (int i = 0; i < tgDiscounts->RowCount; i++)
	{
		for (int j = 0; j < tgDiscounts->ColCount; j++)
		{
			if (std::count(TempUserInfo.AutoAppliedDiscounts.begin(), TempUserInfo.AutoAppliedDiscounts.end(), tgDiscounts->Buttons[i][j]->Tag))
			{
				tgDiscounts->Buttons[i][j]->Latched = true;

			}
			else
			{
				tgDiscounts->Buttons[i][j]->Latched = false;
			}
		}
	}


}

//---------------------------------------------------------------------------

void __fastcall TfrmTabManager::TouchBtn1MouseClick(TObject *Sender)
{

	pgControl->ActivePage = tsTabManager;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBTab::DeleteTabDiscounts(DBTransaction, SelectedTab);
	DBTransaction.Commit();


	for (std::set <int> ::iterator ptrDiscountKey = TempUserInfo.AutoAppliedDiscounts.begin();
	ptrDiscountKey != TempUserInfo.AutoAppliedDiscounts.end(); ptrDiscountKey++)
	{

		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		TDBTab::SetTabDiscounts(DBTransaction, SelectedTab, *ptrDiscountKey)  ;
		DBTransaction.Commit();
	}


}

void __fastcall TfrmTabManager::btnClippTabMouseClick(TObject *Sender)
{
	//Set Clipp Tab details to be form here..

//    SelectedTable = 0;
//	SelectedSeat = 0;
//	SelectedRoomNo = 0;
//
//    //Enable/disable the buttons for Clipp Tab..
//	btnAddNewTab->Enabled = false;
//	btnChangeDetails->Enabled = false;
//	btnManInvoice->Enabled = false;
//	btnSubsidisedProfile->Enabled = false;
//    btnTabCredit->Enabled = true;
//    btnPermanent->Enabled = false;
//	btnAddNewTab->Caption = "Create Tab";
//
//	CurrentTabContainerName = "";
//	CurrentTabType = TabClipp;
//
//	Database::TDBTransaction DBTransaction(DBControl);
//	DBTransaction.StartTransaction();
//
//	TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name);
//
//	DBTransaction.Commit();
//
//
//	ShowTabsDetails();
}
//---------------------------------------------------------------------------
AnsiString TfrmTabManager::CheckDiscountApplicability(int discountKey)
{
    bool retValue = true;
    AnsiString message = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
	try
	{
        TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

        IBInternalQuery->SQL->Text = "SELECT " "a.PERCENTAGE, " "a.AMOUNT," " a.DISCOUNT_MODE," " a.DISCOUNT_TYPE " "FROM " "DISCOUNTS a " "WHERE "
        "a.DISCOUNT_KEY = :DISCOUNT_KEY";
        IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = discountKey;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount > 0)
        {
             if(IBInternalQuery->FieldByName("DISCOUNT_TYPE")->AsInteger != dtPromptAmount &&
                IBInternalQuery->FieldByName("DISCOUNT_TYPE")->AsInteger != dtPromptDescriptionAmount )
             {
                switch(IBInternalQuery->FieldByName("DISCOUNT_MODE")->AsInteger)
                {
                    case DiscModeCurrency:
                    {
                        if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                        {
                            message = "Currency mode Discount/Surcharge of value 0 can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        break;
                    }
                    case DiscModePercent:
                    {
                        if((double)IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency == 0 ||
                          (double)IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency == 100)
                        {
                            message = "Percentage mode Discount/Surcharge of percentage " +
                                       IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency +
                                       " can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        if((double)IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency == -100)
                        {
                            message = "Percentage mode Discount/Surcharge of percentage " +
                                       IBInternalQuery->FieldByName("PERCENTAGE")->AsCurrency +
                                       " can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        break;
                    }
                    case DiscModeSetPrice:
                    {
                        if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                        {
                            message = "Set Price mode Discount/Surcharge of value 0 can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        break;
                    }
                    case DiscModeCombo:
                    {
                        if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                        {
                            message = "Combo mode Discount/Surcharge of value 0 can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        break;
                    }
                    case DiscModeDeal:
                    {
                        if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                        {
                            message = "Deal mode Discount/Surcharge of value 0 can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        break;
                    }
                    case DiscModeItem:
                    {
                        if(IBInternalQuery->FieldByName("AMOUNT")->AsCurrency == 0)
                        {
                            message = "Item mode Discount/Surcharge of value 0 can not be assigned to subsidized tab.";
                            retValue = false;
                        }
                        break;
                    }
                    default:
                       break;
                }
             }
        }
        DBTransaction.Commit();
	}
	catch(Exception &err)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,err.Message);
	}
    return message;
}
//---------------------------------------------------------------------------

