// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef  PalmMate
#include "Palm.h"
#endif

#include "Transfer.h"
#include "Printing.h"
#include "Comms.h"
#include "SelectTable.h"
#include "Login.h"
#include "MMTouchKeyboard.h"
#include "AddTab.h"
#include "Rooms.h"
#include "MMTouchNumpad.h"
#include "MMMessageBox.h"
#include "TableManager.h"
#include "NetMessageTabChanged.h"
#include "ManagerNet.h"
#include "Invoice.h"
#include "Membership.h"
#include "ReqPrintJob.h"
#include "GUIScale.h"
#include "EnableFloorPlan.h"
#include "TransferComplete.h"
#include "Printing.h"
#include "DBTables.h"
#include "DBTab.h"
#include "DBOrder.h"
#include "VerticalSelect.h"
#include "TransferItemOrGuest.h"
#include "SelectSaveOption.h"
#include "Processing.h"
#include <string>
#include "DBSecurity.h"
#include "DBClippTab.h"
#include "ManagerClippIntegration.h"
#include "SCDPWDChecker.h"
#include "ManagerDiscount.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"

#define SEAT_SELECTED_COLOUR			clGreen
#define SEAT_ALREADYLOCKED_COLOUR  		clRed
#define SEAT_SELECTED_FONT_COLOUR		clWhite
#define SEAT_UNSELECTED_COLOUR          clSilver
#define SEAT_UNSELECTED_FONT_COLOUR		clBlack

#define check_space 0x20

// ---------------------------------------------------------------------------
__fastcall TfrmTransfer::TfrmTransfer(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner),
DBControl(inDBControl)
{

}

// ---------------------------------------------------------------------------

void __fastcall TfrmTransfer::FormCreate(TObject *Sender)
{
   CurrentSourceDisplayMode = eNoDisplayMode;
   CurrentDestDisplayMode = eNoDisplayMode;
   CurrentSourceTabType = TabNone;
    // reset the cmClientManager for Chefmate
   cmClientManager.reset( new TChefmateClientManager() );
   Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTransfer::FormDestroy(TObject *Sender)
{
    TimerLongPress->Enabled = false;
    TimerDestLongPress->Enabled = false;
    IsPartialTransfer = false;
    IsPartialTransferDone = false;

}
// ---------------------------------------------------------------------------

void __fastcall TfrmTransfer::FormShow(TObject *Sender)
{
   FormResize(this);
   DBTransaction->StartTransaction();
   Tabs.clear();
   ChefMateOrderList.clear();
   NewInvoiceKey = 0;
   if(CurrentSourceDisplayMode == eTables && (SelectedTabKey == -1 || SelectedTabKey == 0))
   {
      UpdateSourceSeatDetails(*DBTransaction);
      UpdateSourceTableDetails(*DBTransaction);
   }
   if(SelectedTabKey > 0)
   {
     if(CurrentSourceDisplayMode == eTables)
     {
        UpdateSourceSeatDetails(*DBTransaction);
        UpdateSourceTableDetails(*DBTransaction);
     }
     else
     {
         UnicodeString tabname = GetSourceTabName(*DBTransaction, SelectedTabKey);
         if(tabname != "")
         {
             PopulateSourceDestTabDetails(*DBTransaction, tabname, SelectedTabKey, lbDisplayTransferfrom);
             if(CurrentSourceDisplayMode == eTables)
             {
                UpdateSourceTableDetails(*DBTransaction);
             }
             else
             {
               if(CurrentSourceDisplayMode == eTabs && CurrentSourceTabType == TabNormal)
               {
                 btnTransferFrom->Caption = "Tabs";
               }
               if(CurrentSourceDisplayMode == eTabs && CurrentSourceTabType == TabStaff)
               {
                 btnTransferFrom->Caption = "Staff";
               }
               if(CurrentSourceDisplayMode == eTabs && CurrentSourceTabType == TabMember)
               {
                 btnTransferFrom->Caption = "Members";
               }
               if(CurrentSourceDisplayMode == eRooms)
               {
                 btnTransferFrom->Caption = "Rooms";
               }
               if(CurrentSourceDisplayMode == eTabs && CurrentSourceTabType == TabClipp)
               {
                 btnTransferFrom->Caption = "Clipp Tabs";
               }
             }
             UpdateListBox(lbDisplayTransferfrom);
           }
       }
   }
   InvoiceKey = 0;
   InvoiceTabKey = 0;
   PostMessage(Handle, WM_SIZE, SIZE_MAXIMIZED, (Screen->Width & 0x0000FFFF) + ((Screen->Height << 16) & 0xFFFF0000));
   isTabOrItemSelected = false;
   isTotalTableTransfered = false;
   row_count = 0;
   IsBalAvailable = true;
   SourceAllowStaffTab = false;
   DestAllowStaffTab = false;
   TempUserInfo = TDeviceRealTerminal::Instance().User;
   IsPartialTransfer = false;
   IsPartialTransferDone = false;
   ClipTabSelected =false;
   NewClipTabKey=0;
   CloseClipTab=true;
   ItemTransferredFromClip=false;
   isClipLongPress=false;
}

// ---------------------------------------------------------------------------
void TfrmTransfer::UpdateSourceTableDetails(Database::TDBTransaction &DBTransaction)
{
   if (CurrentSourceDisplayMode == eTables)
   {
	  AnsiString TableName = TDBTables::GetTableName(DBTransaction, CurrentSourceTable);
	  AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, CurrentSourceTable);
	  if (TableName == "")
	  {
		 btnTransferFrom->Caption = "Table #" + IntToStr(CurrentSourceTable);
	  }
	  else
	  {
		 btnTransferFrom->Caption = TableName;
	  }
   }
}

// ---------------------------------------------------------------------------
void TfrmTransfer::UpdateDestTableDetails(Database::TDBTransaction &DBTransaction)
{
   if (CurrentDestDisplayMode == eTables)
   {
	  AnsiString TableName = TDBTables::GetTableName(DBTransaction, CurrentDestTable);
	  AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, CurrentDestTable);
	  if (TableName == "")
	  {
		 btnTransferTo->Caption = "Table #" + IntToStr(CurrentDestTable);
	  }
	  else
	  {
		 btnTransferTo->Caption = TableName;
	  }
   }
}

// ---------------------------------------------------------------------------
void TfrmTransfer::UpdateSourceSeatDetails(Database::TDBTransaction &DBTransaction, bool isItemTransfered)
{
   try
   {
   	  if (CurrentSourceDisplayMode == eTabs)
	  {
         UnicodeString tabname = "";
         int key = 0;
		 if (CurrentSourceTabType == TabNormal || CurrentSourceTabType == TabStaff || CurrentSourceTabType == TabClipp)
		 {
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			std::auto_ptr <TStringList> TabList(new TStringList);
            std::auto_ptr <TStringList> TabListItems(new TStringList);
			switch(CurrentSourceTabType)
			{
			case TabNormal:
                if(lbDisplayTransferfrom->Count > 0)
                {
                    tabname = lbDisplayTransferfrom->Items->Strings[0];
                    key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                }
                btnTransferFrom->Caption = "Tabs";
			   break;
			case TabStaff:
                if(lbDisplayTransferfrom->Count > 0)
                {
                    tabname = lbDisplayTransferfrom->Items->Strings[0];
                    key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                }
                btnTransferFrom->Caption = "Staff";
			   break;
            case TabClipp:
                if(lbDisplayTransferfrom->Count > 0)
                {
                    tabname = lbDisplayTransferfrom->Items->Strings[0];
                    key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                }
                btnTransferFrom->Caption = "Clipp Tabs";
			   break;
			}
            ClearListBox(lbDisplayTransferfrom);
            PopulateSourceDestTabDetails(DBTransaction, tabname, key, lbDisplayTransferfrom, true);
		 }
		 else if (CurrentSourceTabType == TabMember)
		 {
            ClearListBox(lbDisplayTransferfrom);
            btnTransferFrom->Caption = "Members";
			int TabKey = TDBTab::GetTabByOwner(DBTransaction, TempSourceUserInfo.ContactKey);
			if (TabKey != 0)
			{
               PopulateSourceDestTabDetails(DBTransaction, TempSourceUserInfo.Name, TabKey, lbDisplayTransferfrom, true);
			}
			else
			{
			   if (TempSourceUserInfo.TabEnabled && TabKey == 0 && TempSourceUserInfo.ContactKey != 0)
			   {
				  TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
				  TDBTab::SetTabOwner(DBTransaction, TabKey, TempSourceUserInfo.ContactKey, TabMember);
				  TDBTab::SetTabName(DBTransaction, TabKey, TempSourceUserInfo.Name);
				  TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
                  PopulateSourceDestTabDetails(DBTransaction, TempSourceUserInfo.Name, TabKey, lbDisplayTransferfrom, true);
			   }
			   else
			   {
				  btnTransferFrom->Caption = "No Membership Tab for " + TempSourceUserInfo.Name;
			   }
			}
		 }
	  }
	  else if (CurrentSourceDisplayMode == eRooms)
	  {
         ClearListBox(lbDisplayTransferfrom);
		 btnTransferFrom->Caption = "Rooms";
		 if (CurrentSourceRoom != 0 && !TDBRooms::IsEmpty(DBTransaction, CurrentSourceRoom))
		 {
            ClearListBox(lbDisplayTransferfrom);
            int tab_key =  TDBRooms::GetRoomTab(DBTransaction, CurrentSourceRoom);
            UnicodeString roomName = TDBRooms::GetRoomName(DBTransaction, CurrentSourceRoom);
            PopulateSourceDestTabDetails(DBTransaction, roomName, tab_key, lbDisplayTransferfrom, true);
		 }
	  }
	  else if (CurrentSourceDisplayMode == eTables)
	  {
         ClearListBox(lbDisplayTransferfrom);
         std::auto_ptr <TList> TabItemList(new TList);

		 std::set <__int64> SelectedTabs;
		 TDBTables::GetTabKeys(DBTransaction, CurrentSourceTable, SelectedTabs);

		 for (std::set <__int64> ::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end(); advance(itTab, 1))
		 {
			if (TDBTab::GetTabExists(DBTransaction, *itTab))
			{
               UnicodeString tabledata = IntToStr(TDBTables::GetSeatNoFromTabKey(DBTransaction, *itTab)) + ". " + TDBTab::GetTabName(DBTransaction, *itTab);
               int tab_key = *itTab;
               PopulateSourceDestTabDetails(DBTransaction, tabledata, tab_key, lbDisplayTransferfrom);
            }
		 }
	  }
      UpdateListBox(lbDisplayTransferfrom);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

// ---------------------------------------------------------------------------
void TfrmTransfer::UpdateDestSeatDetails(Database::TDBTransaction &DBTransaction)
{

   if (CurrentDestDisplayMode == eTabs)
   {
      UnicodeString tabname = "";
      int key = 0;
	  int Count = 0;
	  if (CurrentDestTabType == TabNormal || CurrentDestTabType == TabStaff  || CurrentDestTabType == TabClipp)
	  {
		 std::auto_ptr <TStringList> TabList(new TStringList);
		 std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		 switch(CurrentDestTabType)
		 {
		 case TabNormal:
            if(lbDisplayTransferto->Count > 0)
            {
                tabname = lbDisplayTransferto->Items->Strings[0];
                key = GetTabKeyFromListBox(lbDisplayTransferto, 0);
                btnTransferTo->Caption = "Tabs";
            }

			break;
		 case TabStaff:
            if(lbDisplayTransferto->Count > 0)
            {
                tabname = lbDisplayTransferto->Items->Strings[0];
                key = GetTabKeyFromListBox(lbDisplayTransferto, 0);
                btnTransferTo->Caption = "Staff";
            }

			break;
        case TabClipp:
            if(lbDisplayTransferto->Count > 0)
            {
                tabname = lbDisplayTransferto->Items->Strings[0];
                key = GetTabKeyFromListBox(lbDisplayTransferto, 0);
                btnTransferTo->Caption = "Clipp Tabs";
            }
			break;
        }
        ClearListBox(lbDisplayTransferto);
        PopulateSourceDestTabDetails(DBTransaction, tabname, key, lbDisplayTransferto, true);
	  }
	  else if (CurrentDestTabType == TabMember)
	  {
         ClearListBox(lbDisplayTransferto);
		 btnTransferTo->Caption = "Members";
		 int TabKey = TDBTab::GetTabByOwner(DBTransaction, TempDestUserInfo.ContactKey);

		 if (TabKey != 0)
		 {
            PopulateSourceDestTabDetails(DBTransaction, TempDestUserInfo.Name, TabKey, lbDisplayTransferto, true);
		 }
		 else
		 {
			if (TempDestUserInfo.TabEnabled && TabKey == 0 && TempDestUserInfo.ContactKey != 0)
			{
               TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
               TDBTab::SetTabOwner(DBTransaction, TabKey, TempDestUserInfo.ContactKey, TabMember);
               TDBTab::SetTabName(DBTransaction, TabKey, TempDestUserInfo.Name);
               TDBTab::SetTabPermanent(DBTransaction, TabKey, true);
               PopulateSourceDestTabDetails(DBTransaction, TempDestUserInfo.Name, TabKey, lbDisplayTransferto, true);
			}
		 }
	  }
   }
   else if (CurrentDestDisplayMode == eRooms)
   {
      btnTransferTo->Caption = "Rooms";
	  if (CurrentDestRoom != 0)
	  {
         ClearListBox(lbDisplayTransferto);
         UnicodeString roomName = "";
         int tab_key =  TDBRooms::GetRoomTab(DBTransaction, CurrentDestRoom);

		 if (TDBRooms::IsEmpty(DBTransaction, CurrentDestRoom))
		 {
			roomName = TDBRooms::GetRoomName(DBTransaction, CurrentDestRoom);
		 }
		 else
		 {
			AnsiString PartyName = TDBRooms::GetPartyName(DBTransaction, CurrentDestRoom);
			if (PartyName != "")
			{
			   roomName = TDBRooms::GetPartyName(DBTransaction, CurrentDestRoom) + " [Active]";
			}
			else
			{
			   roomName = TDBRooms::GetRoomName(DBTransaction, CurrentDestRoom) + " [Active]";
			}
		 }
         PopulateSourceDestTabDetails(DBTransaction, roomName, tab_key, lbDisplayTransferto, true);
	  }
   }
   else if (CurrentDestDisplayMode == eTables)
   {
	  if (TGlobalSettings::Instance().TablesEnabled)
	  {
		 ClearListBox(lbDisplayTransferto);
		 Variant locvalues = CurrentDestTable;
         std::auto_ptr <TList> TabItemList(new TList);
         std::set <__int64> SelectedTabs;
         TDBTables::GetTabKeys(DBTransaction, CurrentDestTable, SelectedTabs);
         for (std::set <__int64> ::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end(); advance(itTab, 1))
         {
            if (TDBTab::GetTabExists(DBTransaction, *itTab))
            {
               UnicodeString tabledata = IntToStr(TDBTables::GetSeatNoFromTabKey(DBTransaction, *itTab)) + ". " + TDBTab::GetTabName(DBTransaction, *itTab);
               int tab_key = *itTab;
               PopulateSourceDestTabDetails(DBTransaction, tabledata, tab_key, lbDisplayTransferto);
            }
         }
	  }
   }
   else if (CurrentDestDisplayMode == eInvoices)
   {
      ClearListBox(lbDisplayTransferto);
      btnTransferTo->Caption = TempDestUserInfo.Name + " Invoices";
      if(NewInvoiceKey > 0)
      {
        PopulateSourceDestTabDetails(DBTransaction, "New Invoice", NewInvoiceKey, lbDisplayTransferto, true);
      }
      else
      {
         PopulateSourceDestTabDetails(DBTransaction, "New Invoice", 0, lbDisplayTransferto, true);
      }
   }
   UpdateListBox(lbDisplayTransferto);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTransfer::FormResize(TObject *Sender)
{
    int Temp = Tag;
    Tag = Screen->Width;
    if((double)Screen->Width/Screen->Height < 1.4)
    {
        ScaleBy(Screen->Width, Temp);
    }
    PnlOkCloseBtn->Left = Temp - PnlOkCloseBtn->Width;
    PnlScreen->Width = Temp - PnlOkCloseBtn->Width -10;
    PnlScreen->Height = Screen->Height - 80;
    PnlOkCloseBtn->Height = Screen->Height - 80;
    lbDisplayTransferfrom->Height = Screen->Height - 250;
    lbDisplayTransferto->Height = Screen->Height - 250;
    lbDisplayTransferfrom->Width = (PnlScreen->Width / 2) - 15;
    lbDisplayTransferto->Width = lbDisplayTransferfrom->Width;
    lbDisplayTransferto->Left = lbDisplayTransferfrom->Width + 23;
    PnlBorder->Left = lbDisplayTransferfrom->Left + lbDisplayTransferfrom->Width + 8;
    lbeTransferTo->Left = lbDisplayTransferto->Left;
    btnTransferTo->Left = lbeTransferTo->Left + lbeTransferTo->Width + 10;
    btnTransferFrom->Left = lbeTransferFrom->Left + lbeTransferFrom->Width + 10;
    btnSourceDown->Left = (lbDisplayTransferfrom->Left + lbDisplayTransferfrom->Width) - btnSourceDown->Width;
    btnSourceUp->Left = (btnSourceDown->Left - btnSourceDown->Width) -10;
    btnDown->Left = (lbDisplayTransferto->Left + lbDisplayTransferto->Width) - btnSourceDown->Width;
    btnUp->Left = (btnDown->Left - btnDown->Width) - 10;
    btnSourceDown->Top = lbDisplayTransferfrom->Height + lbDisplayTransferfrom->Top + 10;
    btnSourceUp->Top = lbDisplayTransferfrom->Height + lbDisplayTransferfrom->Top + 10;
    btnUp->Top = lbDisplayTransferto->Height + lbDisplayTransferto->Top + 10;
    btnDown->Top = lbDisplayTransferto->Height + lbDisplayTransferto->Top + 10;
    btnClose->Top = btnUp->Top + 33;
    PnlBorder->Height = btnUp->Top - btnDown->Width;
    PnlHeader->Width = PnlScreen->Width +5;
    StHeader->Left = (PnlHeader->Width / 2) - 40;
    PnlOkCloseBtn->Height = Screen->Height - 45;
    PnlOkCloseBtn->Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTransfer::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(this);
}

void __fastcall TfrmTransfer::btnCloseClick(TObject *Sender)
{
   DBTransaction->Rollback();
   TimerLongPress->Enabled = false;
   TimerDestLongPress->Enabled = false;
   ModalResult = mrCancel;
   NewInvoiceKey = 0;
   InvoiceKey = 0;
   InvoiceTabKey = 0;

}

void __fastcall TfrmTransfer::FormClose(TObject *Sender, TCloseAction &Action)
{
  TimerLongPress->Enabled = false;
  TimerDestLongPress->Enabled = false;
  TDeviceRealTerminal::Instance().User = TempUserInfo;
  DestTempUserInfo.Clear();
  SourceTempUserInfo.Clear();
  ClearListBox(lbDisplayTransferfrom);
  ClearListBox(lbDisplayTransferto);
}
// ---------------------------------------------------------------------------
bool TfrmTransfer::TabLockedOk(Database::TDBTransaction &DBTransaction, int inTabKey)
{
   bool TabLockedOk = false;

   if (TDBTab::LockTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,inTabKey))
   {
	  TabLockedOk = true;
   }
   else
   {
	  AnsiString LockedBy = TDBTab::GetLocker(DBTransaction,inTabKey);
	  if (MessageBox("This " + TGlobalSettings::Instance().SeatLabel + "/Tab is in use by " + LockedBy + "\rDo you wish to override this lock", "Warning",
			MB_YESNO + MB_ICONWARNING) == ID_YES)
	  {
		 TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,inTabKey);
		 if (TDBTab::LockTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,inTabKey))
		 {
			TabLockedOk = true;
		 }
		 else
		 {
			MessageBox("Unable to Unlock Seat/Tab Try again Later", "Warning", MB_OK + MB_ICONWARNING);
		 }
	  }
   }
   return TabLockedOk;
}
// ---------------------------------------------------------------------------
bool TfrmTransfer::TabPINOk(Database::TDBTransaction &DBTransaction, int inTabKey)
{
   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   AnsiString TabPIN = TDBTab::GetTabPIN(DBTransaction, inTabKey);
   bool TabPINProceed = false;
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
			TabPINProceed = true;
		 }
		 else
		 {
			MessageBox("Incorrect PIN", "Error", MB_OK);
		 }
	  }
   }
   else
   {
	  TabPINProceed = true;
   }
   return TabPINProceed;
}

bool TfrmTransfer::TabStaffAccessOk(Database::TDBTransaction &DBTransaction, int inTabKey)
{
   bool Proceed = false;
   if (CurrentSourceDisplayMode == eTabs && CurrentSourceTabType == TabStaff)
   {
      if(!(SourceTempUserInfo.ContactKey > 0))
      {
         SourceTempUserInfo.ContactKey = TDBTab::GetTabOwner(DBTransaction, inTabKey);
      }
      if(SourceTempUserInfo.ContactKey != TempUserInfo.ContactKey)
      {
         Proceed = true;
         if(SourceAllowStaffTab)
         {
            TDeviceRealTerminal::Instance().User = SourceTempUserInfo;
         }
         else
         {
            TDeviceRealTerminal::Instance().User = TempUserInfo;
         }
      }
      else
      {
          std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

          TLoginSuccess Result = Staff->Login(this, DBTransaction, SourceTempUserInfo, CheckPOS);
          if (Result == lsAccepted)
          {
             if (SourceTempUserInfo.ContactKey != TempUserInfo.ContactKey)
             {
                Proceed = true;
                TDeviceRealTerminal::Instance().User = SourceTempUserInfo;
                SourceAllowStaffTab = true;
             }
             else
             {
                MessageBox("You cannot transfer from your own tab.", "Error", MB_OK + MB_ICONERROR);
                SourceTempUserInfo.Clear();
             }

          }
          else if (Result == lsDenied)
          {
             MessageBox("You do not have access to P.O.S.", "Error", MB_OK + MB_ICONERROR);
             SourceTempUserInfo.Clear();
          }
          else if (Result == lsPINIncorrect)
          {
             MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
             SourceTempUserInfo.Clear();
          }
      }
   }
   else
   {
	  Proceed = true;
   }
   return Proceed;
}

bool TfrmTransfer::DestTabStaffAccessOk(Database::TDBTransaction &DBTransaction, int inTabKey)
{
   bool Proceed = false;

   if (CurrentDestDisplayMode == eTabs && CurrentDestTabType == TabStaff)
   {

     if(!(DestTempUserInfo.ContactKey > 0))
     {
       DestTempUserInfo.ContactKey = TDBTab::GetTabOwner(DBTransaction, inTabKey);
     }
     if(DestTempUserInfo.ContactKey != TempUserInfo.ContactKey)
     {
        Proceed = true;
        if(DestAllowStaffTab)
        {
           TDeviceRealTerminal::Instance().User = DestTempUserInfo;
        }
        else
        {
           TDeviceRealTerminal::Instance().User = TempUserInfo;
        }
     }
     else
     {
          std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
          TLoginSuccess Result = Staff->Login(this, DBTransaction, DestTempUserInfo, CheckPOS);
          if (Result == lsAccepted)
          {
             if (DestTempUserInfo.ContactKey != TempUserInfo.ContactKey)
             {
                Proceed = true;
                TDeviceRealTerminal::Instance().User = DestTempUserInfo;
                DestAllowStaffTab = true;
             }
             else
             {
                MessageBox("You cannot transfer from your own tab.", "Error", MB_OK + MB_ICONERROR);
                DestTempUserInfo.Clear();
             }

          }
          else if (Result == lsDenied)
          {
             MessageBox("You do not have access to P.O.S.", "Error", MB_OK + MB_ICONERROR);
             DestTempUserInfo.Clear();
          }
          else if (Result == lsPINIncorrect)
          {
             MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
             DestTempUserInfo.Clear();
          }
     }
   }
   else
   {
	  Proceed = true;
   }
   return Proceed;
}


// ---------------------------------------------------------------------------
unsigned __int32 TfrmTransfer::getMaxSeatCount()
{
    return TEnableFloorPlan::Instance()->GetMaxSeatCount();
}

AnsiString TfrmTransfer::DisplayKeypad( AnsiString inCaption )
{
	AnsiString Name = "";
    int CustNameKey = -1;
	std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (Screen->ActiveForm));
	frmTouchKeyboard->MaxLength = 50;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = "";
	frmTouchKeyboard->Caption = inCaption;
	if (frmTouchKeyboard->ShowModal() == mrOk && frmTouchKeyboard->KeyboardText != "")
	{
		return frmTouchKeyboard->KeyboardText;
	}
}

int TfrmTransfer::DisplayNumpad( AnsiString inCaption )
{
   std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
   frmTouchNumpad->Caption = inCaption;
   frmTouchNumpad->btnSurcharge->Visible = false;
   frmTouchNumpad->btnDiscount->Visible = false;
   frmTouchNumpad->btnOk->Visible = true;
   frmTouchNumpad->Mode = pmNumber;
   if (frmTouchNumpad->ShowModal() == mrOk)
   {
	  return frmTouchNumpad->INTResult;
   }
}

TCustomer TfrmTransfer::GetAndSaveCustomer( Database::TDBTransaction &DBTransaction, const int InvoiceKey )
{
	TCustomer Customer( DisplayNumpad( "Enter Customers Room Number" ),
						InvoiceKey,
						DisplayKeypad( "Enter the Customers Name" ));
	TDBContacts::SaveCustomerAndNumber( DBTransaction, Customer );
	return Customer;
}
//.............................................................................
void TfrmTransfer::UpdateTablePartyName(Database::TDBTransaction &DBTransaction, long DestTabKey, long SourceKey, bool isTabSelected)
{
    TTableSeat TableSeat;
    int TableNo = 0;
    UnicodeString TablePartyName = "";
    if (TDBTables::GetTableSeat(DBTransaction,DestTabKey,&TableSeat))
    {
        TableNo = TableSeat.TableNo;
        TablePartyName = TableSeat.PartyName;
        if(TablePartyName == "")
        {
           UnicodeString PartyName = GetTabNameForTable(DBTransaction, SourceKey, isTabSelected);
           TDBTables::SetPartyName(DBTransaction, TableNo, PartyName);
        }
    }
}
//----------------------------------------------------------------------
void __fastcall TfrmTransfer::btnTransferFromClick(TObject *Sender)
{
   ShowSelectScreen(*DBTransaction, "Select Transfer From");
}

void __fastcall TfrmTransfer::btnTransferToClick(TObject *Sender)
{
   ShowSelectScreen(*DBTransaction, "Select Transfer To");
}

void __fastcall TfrmTransfer::btnOKClick(TObject *Sender)
{
   try
   {
   NewInvoiceKey = 0;
   AnsiString SourceName = "";
   AnsiString DestName = "";
   bool result = false;
   int sourceTabKey = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
   int destTabKey =   GetTabKeyFromListBox(lbDisplayTransferto, 0);
   bool process_closing = true;
   linkCliptabToTable =false;
   CloseClipTab = false;

   if(btnTransferFrom->Caption != "Select" && btnTransferTo->Caption != "Select" && btnTransferTo->Caption != "Access Declined" && btnTransferFrom->Caption != "No Membership Tab for " + TempSourceUserInfo.Name)
   {
        /// check for linked clip tab limit in from or to panel ,if any tab violates credit limit then stop the transfer and show error
         if(!CheckLinkedTabValidations(*DBTransaction,true))
         {
           return;
         }

       if(!isTabOrItemSelected)
       {


           CheckClipAndTableCondition(*DBTransaction);

           //check for if transferrring any linked clip tab or table to other destination
             if(!CheckLinkedTabValidations(*DBTransaction,false))
            {
               return;
            }

           if(CurrentSourceDisplayMode == eTables)
           {
             SourceName = btnTransferFrom->Caption;
           }
           else
           {
              if(lbDisplayTransferfrom->Count > 0)
              {
                SourceName = lbDisplayTransferfrom->Items->Strings[0];
              }
           }
           if(CurrentDestDisplayMode == eTables)
           {
              DestName = btnTransferTo->Caption;
           }
           else
           {
              if(lbDisplayTransferto->Count > 0)
              {
                 DestName = lbDisplayTransferto->Items->Strings[0];
              }
           }
            if(sourceTabKey == destTabKey && !ClipTabSelected)
            {
                MessageBox("Source and Destination of transfers can not be same.", "Error", MB_OK + MB_ICONERROR);
            }
            else
            {
                result = CheckStaffTabAccess(*DBTransaction);
            }
               
          if(result)
          {
               if (MessageBox("Are you sure you want to transfer all items from "+SourceName + "  to " + DestName + "?", "Warning", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
                {
                   if(CheckStaffTabAccess(*DBTransaction))
                   {
                       if(ClipTabSelected)
                         {

                            //callinf this function if we are transferring from or to clipp
                               if(MessageBox("Do you wish these items to be removed From Clipp?", "Save Changes?", MB_YESNO + MB_ICONQUESTION) == ID_YES )
                                  {
                                     CloseClipTab = true;
                                     linkCliptabToTable = false;
                                  }
                                  else
                                  {
                                      CloseClipTab = false;
                                      linkCliptabToTable = true;

                                  }
                         }


                       TotalTransferTableOrTab(*DBTransaction);
                       DeleteEmptyTabs(*DBTransaction);
                       DBTransaction->Commit();

                       //callinf this function if we are treansferring from or to clipp
                       SendTabDetails(btnTransferFrom->Caption, btnTransferTo->Caption, lbDisplayTransferfrom->Count, lbDisplayTransferto->Count );


                       ModalResult = mrOk;
                   }
                }
                else
                {
                   DBTransaction->Commit();
                   ModalResult = mrOk;
                }
            }
       }
       else
       {
          if(sourceTabKey == destTabKey)  //changes here...
          {

             if(IsPartialTransferDone)
             {
                  if(CurrentDestDisplayMode == eTables && CurrentSourceDisplayMode == eTables)
                  {
                      if(IsPartialTransfer)
                      {
                         IsPartialTransfer = false;
                         MessageBox("Source and Destination of transfers can not be same.", "Error", MB_OK + MB_ICONERROR);
                      }
                  }
                  else
                  {
                     MessageBox("Source and Destination of transfers can not be same.", "Error", MB_OK + MB_ICONERROR);
                  }
                  if(process_closing)
                  {
                      DeleteEmptyTabs(*DBTransaction);
                      DBTransaction->Commit();
                      //callinf this function if we are treansferring from or to clipp
                      SendTabDetails(btnTransferFrom->Caption, btnTransferTo->Caption, lbDisplayTransferfrom->Count, lbDisplayTransferto->Count );
                      ModalResult = mrOk;
                  }

             }
             else
             {
                 MessageBox("Source and Destination of transfers can not be same.", "Error", MB_OK + MB_ICONERROR);
             }
          }
          else
          {
              bool  fullTransfer;
              // here ItemTransferredFromClip is used so that if items were transferred from clip tab then only it should enter
              // vidout it u can transfer from table to clip tab and it will ask for linking ,which should not happen and therefore is bug .

              if(ClipTabSelected && ItemTransferredFromClip)
              {
                    fullTransfer= CheckPartialTransferInClipCase(*DBTransaction);
                    if(fullTransfer)
                    {
                           if(MessageBox("Do you wish these items to be removed From Clipp?", "Save Changes?", MB_YESNO + MB_ICONQUESTION) == ID_YES)
                            {
                                CloseClipTab = true;
                                linkCliptabToTable = false;
                            }
                            else
                            {
                                CloseClipTab = false;
                                linkCliptabToTable = true;
                            }

                           if(linkCliptabToTable)
                           {
                              if(ClipPresentInFromPanel)
                              {
                                 DoCliptabLinking(*DBTransaction,GetTabKeyFromListBox(lbDisplayTransferfrom, 0),NewClipTabKey);
                              }
                              else
                              {
                                  DoCliptabLinking(*DBTransaction,GetTabKeyFromListBox(lbDisplayTransferto, 0),NewClipTabKey);
                              }

                           }
                    }
                    else
                    {
                        if(ClipPresentInFromPanel && lbDisplayTransferfrom->Count==1 )
                        {
                            CloseClipTab = true;
                        }
                        else if(ClipPresentInToPanel && lbDisplayTransferto->Count ==1)
                        {
                            CloseClipTab = true;
                        }
                    }
              }

              DeleteEmptyTabs(*DBTransaction);
              DBTransaction->Commit();
              //callinf this function if we are transferring from or to clipp
              SendTabDetails(btnTransferFrom->Caption, btnTransferTo->Caption, lbDisplayTransferfrom->Count, lbDisplayTransferto->Count );
              ModalResult = mrOk;
          }
       }
      ShowProcessingBar();
      TransferOrderListtoChefmate();
      ChefMateOrderList.clear();
   }
   else
   {
      DBTransaction->Commit();
      ModalResult = mrOk;
   }
   SourceAllowStaffTab = false;
   DestAllowStaffTab = false;

 }
   catch(Exception & E)
   {
	  MessageBox(E.Message, "Error", MB_OK + MB_ICONWARNING);
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
}

void __fastcall TfrmTransfer::lbDisplayTransferfromClick(TObject *Sender)
{
  if(CheckStaffTabAccess(*DBTransaction))
  {
      TimerLongPress->Enabled = false;
      isTabOrItemSelected = true;

      if(btnTransferTo->Caption != "Select" && btnTransferTo->Caption != "Access Declined")
      {
         TransferData(*DBTransaction);
      }
  }
}

void __fastcall TfrmTransfer::lbDisplayTransferfromMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TimerLongPress->Enabled = true;
}

void __fastcall TfrmTransfer::lbDisplayTransferfromMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(!TimerLongPress->ComponentCount <= 3000)
  {
     TimerLongPress->Enabled = false;
  }
}

void __fastcall TfrmTransfer::lbDisplayTransfertoMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TimerDestLongPress->Enabled = true;
}

void __fastcall TfrmTransfer::lbDisplayTransfertoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(!TimerDestLongPress->ComponentCount <= 3000)
  {
     TimerDestLongPress->Enabled = false;
  }
}

void __fastcall TfrmTransfer::lbDisplayTransferfromDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
    static int CL_STANDARD = clBlack;
	static int CL_STANDARD_OPTION = clNavy;
	static int CL_STANDARD_SUB = clTeal;
	static int CL_STANDARD_SETMENU_KEY = clMaroon;
	static int CL_STANDARD_SETMENU_ITEM = clBlack;
	static int CL_STANDARD_MEMBER_INFO = clWhite;
	static int CL_STANDARD_DISCOUNT_INFO = clPurple;
	static int CL_PREVIOUS = RGB(90, 90, 90);
	static int CL_PREVIOUS_OPTION = RGB(90, 90, 90);
	static int CL_PREVIOUS_SUB = RGB(90, 90, 90);
	static int CL_PREVIOUS_SETMENU_KEY = clMaroon;
	static int CL_PREVIOUS_SETMENU_ITEM = RGB(90, 90, 90);

	TCanvas *pCanvas = ((TListBox*)Control)->Canvas;
	TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;
	pCanvas->FillRect(Rect); // clear the rectangle

	UnicodeString DollarAmount = CurrToStrF(0, ffNumber, CurrencyDecimals);

    TTabColour *TabColor;
    UnicodeString Caption = ((TListBox*)Control)->Items->Strings[Index];
    TabColor = (TTabColour *)((TListBox*)Control)->Items->Objects[Index];
    double qty = TabColor->Qty;
    if(TabColor->IsTabKey)
    {
        pCanvas->Font->Style = TFontStyles() << fsBold;
        pCanvas->Font->Name = "Tahoma";
        pCanvas->Font->Size = -14;
        pCanvas->Font->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
        pCanvas->Brush->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
        pCanvas->FillRect(Rect);
    }
    else
    {
       pCanvas->Font->Style = TFontStyles();
       pCanvas->Font->Name = "Tahoma";
       pCanvas->Font->Size = -14;
       pCanvas->Font->Color = (TColor)CL_STANDARD_SETMENU_ITEM;
    }
    if(TabColor->IsSideItem)
    {
       Caption = "  " + Caption;
       pCanvas->Font->Style = TFontStyles();
       pCanvas->Font->Name = "Tahoma";
       pCanvas->Font->Size = -14;
       pCanvas->Font->Color = (TColor)CL_STANDARD_SETMENU_ITEM;
    }
	if (State.Contains(odSelected))
	{
		pCanvas->Font->Style = TFontStyles();
		pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
		pCanvas->Brush->Color = CL_BUTTONCOLOUR_SELECTED_SEAT;
        pCanvas->Font->Name = "Tahoma";
        pCanvas->Font->Size = -14;
		pCanvas->FillRect(Rect); // clear the rectangle
	}

    // Adjusting the width to display
	int TextWidth = Rect.right - Rect.left;
	int DollarWidth = TextWidth / 4;
    DollarAmount =  TabColor->ItemPrice;
	if (DollarAmount == "")
		DollarWidth = 0;

	Rect.right = Rect.left + (TextWidth - DollarWidth);

	int TextHeight = DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &TRect(Rect.left, Rect.top, Rect.right, Rect.bottom), DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;
	TRect CentredRect = Rect;
	int RowHeight = Rect.Bottom - Rect.Top;
	if (RowHeight > TextHeight)
	{
		CentredRect.Top += (RowHeight - TextHeight) / 2;
		CentredRect.Bottom = CentredRect.Top + TextHeight;
	}
	DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &CentredRect, DT_WORDBREAK | DT_NOPREFIX, NULL);

	if ((TGlobalSettings::Instance().ShowItemPriceInTransferScreen) && (DollarAmount != ""))
	{
		int DollarTextHeight = DrawTextExW(pCanvas->Handle, DollarAmount.c_str(), DollarAmount.Length(), &TRect(Rect.right, Rect.top, Rect.right + DollarWidth - 5, Rect.bottom),
			DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;

		TRect DollarCentredRect = Rect;
		int RowHeight = Rect.Bottom - Rect.Top;
		if (RowHeight > DollarTextHeight)
		{
			DollarCentredRect.Top += (RowHeight - DollarTextHeight) / 2;
			DollarCentredRect.Bottom = DollarCentredRect.Top + DollarTextHeight;
		}

		TRect DollarRect = DollarCentredRect;
		DollarRect.Left = Rect.right;
		DollarRect.right = DollarRect.Left + DollarWidth - 5;

		DrawTextExW(pCanvas->Handle, DollarAmount.w_str(), DollarAmount.Length(), &DollarRect, DT_WORDBREAK | DT_NOPREFIX | DT_RIGHT, NULL);
     }
	pCanvas->Brush->Color = clBlack;
	pCanvas->Brush->Style = Style;
}

//............................................................
void __fastcall TfrmTransfer::lbDisplayTransfertoClick(TObject *Sender)
{
  if(CheckDesttaffTabAccess(*DBTransaction))
  {
      TimerDestLongPress->Enabled = false;
      isTabOrItemSelected = true;
      if(btnTransferFrom->Caption != "Select")
      {
        ReverseData(*DBTransaction);
      }
  }

}

void __fastcall TfrmTransfer::lbDisplayTransfertoDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
    static int CL_STANDARD = clBlack;
	static int CL_STANDARD_OPTION = clNavy;
	static int CL_STANDARD_SUB = clTeal;
	static int CL_STANDARD_SETMENU_KEY = clMaroon;
	static int CL_STANDARD_SETMENU_ITEM = clBlack;
	static int CL_STANDARD_MEMBER_INFO = clWhite;
	static int CL_STANDARD_DISCOUNT_INFO = clPurple;
	static int CL_PREVIOUS = RGB(90, 90, 90);
	static int CL_PREVIOUS_OPTION = RGB(90, 90, 90);
	static int CL_PREVIOUS_SUB = RGB(90, 90, 90);
	static int CL_PREVIOUS_SETMENU_KEY = clMaroon;
	static int CL_PREVIOUS_SETMENU_ITEM = RGB(90, 90, 90);



	TCanvas *pCanvas = ((TListBox*)Control)->Canvas;
	TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;
	pCanvas->FillRect(Rect); // clear the rectangle

	UnicodeString DollarAmount = CurrToStrF(0, ffNumber, CurrencyDecimals);

    TTabColour *TabColor;
    UnicodeString Caption = ((TListBox*)Control)->Items->Strings[Index];
    TabColor = (TTabColour *)((TListBox*)Control)->Items->Objects[Index];
    if(TabColor->IsTabKey)
    {
        pCanvas->Font->Style = TFontStyles() << fsBold;
        pCanvas->Font->Name = "Tahoma";
        pCanvas->Font->Size = -14;
        pCanvas->Font->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
        pCanvas->Brush->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
        pCanvas->FillRect(Rect);
    }
    else
    {
       pCanvas->Font->Style = TFontStyles();
       pCanvas->Font->Name = "Tahoma";
       pCanvas->Font->Size = -14;
       pCanvas->Font->Color = (TColor)CL_STANDARD_SETMENU_ITEM;
    }
    if(TabColor->IsSideItem)
    {
       Caption = "  " + Caption;
       pCanvas->Font->Style = TFontStyles();
       pCanvas->Font->Name = "Tahoma";
       pCanvas->Font->Size = -14;
       pCanvas->Font->Color = (TColor)CL_STANDARD_SETMENU_ITEM;
    }
	if (State.Contains(odSelected))
	{
		pCanvas->Font->Style = TFontStyles();
		pCanvas->Font->Color = (TColor)CL_STANDARD_MEMBER_INFO;
		pCanvas->Brush->Color = CL_BUTTONCOLOUR_SELECTED_SEAT;
        pCanvas->Font->Name = "Tahoma";
        pCanvas->Font->Size = -14;
		pCanvas->FillRect(Rect); // clear the rectangle
	}
    // Adjusting the width to display
	int TextWidth = Rect.right - Rect.left;
	int DollarWidth = TextWidth / 4;
    DollarAmount =  TabColor->ItemPrice;
	if (DollarAmount == "")
		DollarWidth = 0;

	Rect.right = Rect.left + (TextWidth - DollarWidth);

	int TextHeight = DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &TRect(Rect.left, Rect.top, Rect.right, Rect.bottom), DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;
	TRect CentredRect = Rect;
	int RowHeight = Rect.Bottom - Rect.Top;
	if (RowHeight > TextHeight)
	{
		CentredRect.Top += (RowHeight - TextHeight) / 2;
		CentredRect.Bottom = CentredRect.Top + TextHeight;
	}
	DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &CentredRect, DT_WORDBREAK | DT_NOPREFIX, NULL);

	if ((TGlobalSettings::Instance().ShowItemPriceInTransferScreen) && (DollarAmount != ""))
	{
		int DollarTextHeight = DrawTextExW(pCanvas->Handle, DollarAmount.c_str(), DollarAmount.Length(), &TRect(Rect.right, Rect.top, Rect.right + DollarWidth - 5, Rect.bottom),
			DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX, NULL) + 4;

		TRect DollarCentredRect = Rect;
		int RowHeight = Rect.Bottom - Rect.Top;
		if (RowHeight > DollarTextHeight)
		{
			DollarCentredRect.Top += (RowHeight - DollarTextHeight) / 2;
			DollarCentredRect.Bottom = DollarCentredRect.Top + DollarTextHeight;
		}

		TRect DollarRect = DollarCentredRect;
		DollarRect.Left = Rect.right;
		DollarRect.right = DollarRect.Left + DollarWidth - 5;

		DrawTextExW(pCanvas->Handle, DollarAmount.w_str(), DollarAmount.Length(), &DollarRect, DT_WORDBREAK | DT_NOPREFIX | DT_RIGHT, NULL);
     }
	pCanvas->Brush->Color = clBlack;
	pCanvas->Brush->Style = Style;
}

UnicodeString TfrmTransfer::GetTabNameForTable(Database::TDBTransaction &DBTransaction, long source_key, bool isTabSelected)
{
    UnicodeString tabName ="";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        if(isTabSelected)
        {
           IBInternalQuery->SQL->Text =
              "select B.TAB_NAME from ORDERS B where TAB_KEY = :TAB_KEY group by B.TAB_NAME " ;
           IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = source_key;
        }
        else
        {
            IBInternalQuery->SQL->Text =
                "select B.TAB_NAME from ORDERS B where ORDER_KEY = :ORDER_KEY " ;
            IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = source_key;
        }
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           tabName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
        }
        IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return tabName;
}

bool TfrmTransfer::ShowSourceMembersTab(Database::TDBTransaction &DBTransaction)
{
   bool retVal = false;
   TempSourceUserInfo.Clear();
   eMemberSource MemberSource;
   TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempSourceUserInfo, MemberSource);
   if (Result == lsAccepted || Result == lsAccountBlocked)
   {
     retVal = true;
   }
   else if (Result == lsAccountBlocked)
   {
	  MessageBox("Account Blocked " + TempDestUserInfo.Name + " " + TempDestUserInfo.AccountInfo, "Account Blocked",
		 MB_OK + MB_ICONINFORMATION);
   }
   return retVal;
}

bool TfrmTransfer::ShowDestMembersTab(Database::TDBTransaction &DBTransaction)
{
   bool retVal = false;
   TempDestUserInfo.Clear();
   eMemberSource MemberSource;
   TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempDestUserInfo, MemberSource);
   if (Result == lsAccepted)
   {
     retVal = true;
   }
   else if (Result == lsAccountBlocked)
   {
	  MessageBox("Account Blocked " + TempDestUserInfo.Name + " " + TempDestUserInfo.AccountInfo, "Account Blocked",
		 MB_OK + MB_ICONINFORMATION);
   }
   return retVal;
}

TModalResult TfrmTransfer::NewInvoiceItems(Database::TDBTransaction &DBTransaction)
{
   TModalResult Retval = mrOk;
   TMMContactInfo TempUserInfo;
   TempUserInfo = TDeviceRealTerminal::Instance().User;

   std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
   bool Allow = Staff->TestAccessLevel(TempUserInfo, CheckChargeToInvoice);
   // If not, prompt for a login.
   if (!Allow)
   {
	  TempUserInfo.Clear();
	  TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckChargeToInvoice);
	  if (Result == lsAccepted)
	  {
		 Allow = true;
	  }
	  else if (Result == lsDenied)
	  {
		 MessageBox("You do not have access rights to the Invoice System.", "Error", MB_OK + MB_ICONERROR);
         Retval = mrAbort;
	  }
	  else if (Result == lsPINIncorrect)
	  {
		 MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
         Retval = mrAbort;
	  }
   }

   if (Allow)
   {
	  TempDestUserInfo.Clear();
	  eMemberSource MemberSource;
	  TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempDestUserInfo, MemberSource);
	  if (Result == lsAccepted)
	  {
		 if (TempDestUserInfo.TabEnabled)
		 {
			CurrentDestDisplayMode = eInvoices;
			CurrentDestTabType = TabInvoice;
            TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
			UpdateDestTableDetails(DBTransaction);
			UpdateDestSeatDetails(DBTransaction);
		 }
		 else
		 {
			MessageBox("Credit Disabled " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Sorry No Credit, No Invoices.",
			   MB_OK + MB_ICONINFORMATION);
            Retval = mrAbort;
		 }
	  }
	  else if (Result == lsAccountBlocked)
	  {
		 MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
         Retval = mrAbort;
	  }
	  else
	  {
         Retval = mrAbort;
	  }
   }
   return Retval;
}

void TfrmTransfer::ProcessInvoice(Database::TDBTransaction &DBTransaction, long key, bool isTabSelected,bool ReverseTransfer)
{
   Currency TotalCost;
   Currency TotalCostExcl;
   Currency TotalCredit;

   std::auto_ptr <TList> OrdersList(new TList);
   std::set <__int64> SelectedTabs;
   SelectedTabs.insert(key);
   if(isTabSelected)
   {
     TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs, true);
     TotalCost = TDBTab::GetTabTotal(DBTransaction, key);
     TotalCostExcl = TDBTab::GetTabTotalExcl(DBTransaction, key);
   }
   else
   {
       PrepareItemList(DBTransaction, key,false, OrdersList.get());
       for (int i = 0; i < OrdersList->Count; i++)
       {
          TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
          TotalCost += Order->TotalPriceAdjustmentSides();
          TotalCostExcl += Order->TotalPriceAdjustmentSidesExclGST();
       }
   }
   int OwnerKey = TempDestUserInfo.ContactKey;
   if(!(NewInvoiceKey > 0))
   {
      InvoiceTabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
      InvoiceKey = Invoice->CreateInvoice(DBTransaction, OwnerKey, TotalCostExcl, TotalCost, "");
      NewInvoiceKey = InvoiceTabKey;
      TDBTab::SetTabToInvoice(DBTransaction, InvoiceTabKey, InvoiceKey);
      TDBTab::SetTabOwner(DBTransaction, InvoiceTabKey, OwnerKey, TabInvoice);
   }

   if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), InvoiceTabKey))
   {
       TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), InvoiceTabKey,TDeviceRealTerminal::Instance().User.ContactKey, key, true);
       if (cmClientManager->ChefMateEnabled())
       {
         int invoiceNo=TDBTab::GetInvoiceNo(DBTransaction, InvoiceKey);
         CollectDataForChefmateTransfer(invoiceNo, OrdersList.get(), lbDisplayTransferfrom);
       }
   }
   else
   {
      MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
   }
   Invoice->SetOrdersToInvoiceFormat(DBTransaction, InvoiceKey, InvoiceTabKey, TempDestUserInfo.Name);
   // Credit May have been transfered to this TAB.
   TotalCredit = TDBTab::GetTabCredit(DBTransaction, InvoiceTabKey);
   TPaymentTransaction Transaction(DBTransaction);
   SelectedTabs.clear();
   SelectedTabs.insert(InvoiceTabKey);
   TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, Transaction.Orders, SelectedTabs, true);
   Transaction.InvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, InvoiceKey);
   Transaction.Money.CreditAvailable = TotalCredit;
   Transaction.Money.Recalc(Transaction);
   if( TempDestUserInfo.HasHotelNumber() )
      Transaction.Customer = GetAndSaveCustomer( DBTransaction, InvoiceKey );
   // Print Invoice.
   std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
   TempReceipt->Transaction = &Transaction;
   TempReceipt->JobType = pjReceiptInvoice;
   TempReceipt->SignReceipt = true;
   TempReceipt->SenderType = devPC;
   TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
   TempReceipt->ExtraInfo->Add("Name: " + TempDestUserInfo.Name);
   TempReceipt->ExtraInfo->Add("Member No. " + TempDestUserInfo.MembershipNumber);
   TempReceipt->PaymentType = ptFinal;

   Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
   TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
   if (TGlobalSettings::Instance().PrintSignatureReceiptsTwice)
   {
      TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
   }
   
}

bool TfrmTransfer::CheckItemsOnTab(Database::TDBTransaction &DBTransaction,int inTabKey)
{
	bool RetVal = true;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	try
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" SELECT *"
		" FROM "
		"  ORDERS "
		" WHERE "
		"  ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = inTabKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

TModalResult TfrmTransfer::AddNewTab(Database::TDBTransaction &DBTransaction)
{
   TModalResult Retval = mrOk;
   std::auto_ptr <TfrmAddTab> frmAddTab(TfrmAddTab::Create(this));
   frmAddTab->LoadDetails(DBTransaction, 0);
   DBTransaction.Commit();
   DBTransaction.StartTransaction();
   TMMContactInfo currentUserInfo = TDeviceRealTerminal::Instance().User;
   std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
   if (!Staff->TestAccessLevel(TDeviceRealTerminal::Instance().User, CheckAccountCreation))
   {
        MessageBox("You do not have the privileges to create a new tab!", "Error", MB_OK + MB_ICONERROR);
        return mrAbort;
   }
   if (frmAddTab->ShowModal() == mrOk)
   {
	  bool TabExists = false;
	  DBTransaction.StartTransaction();
	  if (TDBTab::GetTab(DBTransaction, frmAddTab->TabName, TabNormal) != 0)
	  {
		 TabExists = true;
	  }
      DBTransaction.Commit();
	  if (TabExists)
	  {
		 MessageBox("A Tab with this name already exists. Please select a different name.", "Warning", MB_OK + MB_ICONWARNING);
         Retval = mrAbort;
	  }
	  else
	  {
         DBTransaction.StartTransaction();
         int TabyKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
		 TDBTab::SetTabDetails(DBTransaction, TabyKey, frmAddTab->TabName, frmAddTab->TabIDType, frmAddTab->TabIDNumber,
			frmAddTab->TabIDExpiry, TabNormal);
		 TDBTab::SetTabCard(DBTransaction, TabyKey, frmAddTab->CardString);
         DBTransaction.Commit();
         CurrentDestDisplayMode = eTabs;
         CurrentDestTabType = TabNormal;
         DBTransaction.StartTransaction();
         ClearListBox(lbDisplayTransferto);
         PopulateSourceDestTabDetails(DBTransaction, frmAddTab->TabName, TabyKey, lbDisplayTransferto, true); //
         btnTransferTo->Caption = "Tabs";
         UpdateListBox(lbDisplayTransferto);
         UpdateDestTableDetails(DBTransaction);
	  }
      DBTransaction.StartTransaction();
   }
   else
   {
      Retval = mrAbort;
   }
   return Retval;
}

void TfrmTransfer::UpdateDestRooms(Database::TDBTransaction &DBTransaction)
{
   CurrentDestDisplayMode = eRooms;
   if (TRooms::Instance().SelectRoom(DBTransaction, eRoomFilterAll) == mrOk)
   {
	  CurrentDestRoom = TRooms::Instance().SelectedRoom->RoomNo;
   }
   else
   {
	  CurrentDestRoom = 0;
   }
   UpdateDestTableDetails(DBTransaction);
   UpdateDestSeatDetails(DBTransaction);
}

void TfrmTransfer::UpdateSourceRooms(Database::TDBTransaction &DBTransaction)
{
   CurrentSourceDisplayMode = eRooms;
   if (TRooms::Instance().SelectRoom(DBTransaction) == mrOk)
   {
	  CurrentSourceRoom = TRooms::Instance().SelectedRoom->RoomNo;
   }
   else
   {
	  CurrentSourceRoom = 0;
   }
   UpdateSourceTableDetails(DBTransaction);
   UpdateSourceSeatDetails(DBTransaction);
}

void __fastcall TfrmTransfer::TimerLongPressTimer(TObject *Sender)
{
  TimerLongPress->Enabled = false;
  isTabOrItemSelected = true;
  std::set <__int64> SelectedTabs;
  int source_key;
  bool isTabSelected = false;
  bool isSideSelected = false;
  int k = 0;
  int transfer_qty = 0;
  UnicodeString itemOrTabName = "";
  double itemQty = 0;
  int seat_key = 0;
  int dest_tabkey = 0;
  bool process_transfer = false;
  int itemPosition=0;

  if(lbDisplayTransferfrom->Count > 0 && CurrentDestDisplayMode == eTables)
  {
      if(CheckStaffTabAccess(*DBTransaction))
      {
          for(int i = 0; i < lbDisplayTransferfrom->Count; i++)
          {
              if(lbDisplayTransferfrom->Selected[i])
              {
                 itemPosition=i;
                 source_key = GetTabKeyFromListBox(lbDisplayTransferfrom, i);
                 isTabSelected = CheckTabOrItemSelected(lbDisplayTransferfrom, i);
                 itemOrTabName = GetItemName(lbDisplayTransferfrom, i, isTabSelected);
                 if(!isTabSelected)
                 {
                    isSideSelected = CheckSideIsSelected(lbDisplayTransferfrom, i);
                    if(!isSideSelected)
                    {
                        k = i;
                        itemQty = GetItemQty(lbDisplayTransferfrom, i, isTabSelected);
                    }
                 }
              }
          }

          if(!isSideSelected)
          {
              isClipLongPress=false;
              TfrmTransferItemOrGuest* frmTransferItemOrGuest = new TfrmTransferItemOrGuest(this);
              frmTransferItemOrGuest->lbeItemName->Caption = itemOrTabName;
              if(!isTabSelected)
              {
                 frmTransferItemOrGuest->SelectedItem = true;
                 frmTransferItemOrGuest->GetItemQty = itemQty;
              }
              if(CurrentDestDisplayMode == eTables)
              {
                __int64 maxSeatCount = getMaxSeatCount();
                for( __int64 i = 1; i <= maxSeatCount; i++)
                {
                   frmTransferItemOrGuest->DestItems->AddObject(TGlobalSettings::Instance().SeatLabel + " " + IntToStr(i), (TObject *)i);
                }
              }
              else
              {
                  for(int i = 0; i < lbDisplayTransferto->Count; i++)
                  {
                     int dest_tabkey = GetTabKeyFromListBox(lbDisplayTransferto, i);
                     frmTransferItemOrGuest->DestItems->AddObject(lbDisplayTransferto->Items->Strings[i], (TObject *)dest_tabkey);
                  }
              }
              frmTransferItemOrGuest->SourceTable = btnTransferFrom->Caption;
              frmTransferItemOrGuest->DestTableName = btnTransferTo->Caption;
              if(frmTransferItemOrGuest->ShowModal() != mrCancel)
              {
                 dest_tabkey = frmTransferItemOrGuest->dest_tab_key;
                 CheckClipAndTableCondition(*DBTransaction);
                int sourceTabKey = GetTabKeyFromListBox(lbDisplayTransferfrom, itemPosition);

                TMMTabType sourceTabType = TDBTab::GetLinkedTableAndClipTab(*DBTransaction, sourceTabKey);
                int seatNo= dest_tabkey;
                if(sourceTabType==TabClipp )
                {
                   MessageBox("Linked tab/table cannot be transferred to other locations.", "Error", MB_OK + MB_ICONERROR);
                   return ;
                }
               if(ClipTabSelected)
                {
                    isClipLongPress=true;
                  if( !ItemTransferredFromClip )
                    {
                       if(TDBTab::GetTabType(*DBTransaction,sourceTabKey)==TabClipp )
                        {
                             ItemTransferredFromClip =true;
                        }
                    }
                 }

                 if(btnTransferFrom->Caption == btnTransferTo->Caption)
                 {
                    if(!isTabSelected)
                    {
                       source_key = GetOrderKeyFromSavedList(*DBTransaction, 1, lbDisplayTransferfrom, k);
                    }
                    seat_key = GetSeatNumber(*DBTransaction, source_key, isTabSelected, CurrentSourceTable);
                    if(seat_key == dest_tabkey)
                    {
                      process_transfer = true;
                      IsPartialTransfer = true;
                    }
                 }
                 if(!process_transfer)
                 {
                     transfer_qty = frmTransferItemOrGuest->ItmQty;
                     IsPartialTransfer = false;
                     if(!isTabSelected)
                     {
                         GetOrderKeyFromSavedList(*DBTransaction, transfer_qty, lbDisplayTransferfrom, k, true);
                         for(std::vector<int>::iterator it = GetSavedOrderKey.begin(); it != GetSavedOrderKey.end(); ++it)
                         {
                           TransferTotal(*it, dest_tabkey, false,  isTabSelected);
                         }
                         GetSavedOrderKey.clear();
                     }
                     else
                     {
                         TransferTotal(source_key, dest_tabkey, false,  isTabSelected);
                     }
                 }

              }
              delete frmTransferItemOrGuest;
          }
      }
  }

}

void __fastcall TfrmTransfer::TimerDestLongPressTimer(TObject *Sender)
{
  TimerDestLongPress->Enabled = false;
  isTabOrItemSelected = true;
  std::set <__int64> SelectedTabs;
  int tab_key;
  bool isTabSelected = false;
  bool isSideSelected = false;
  int k = 0;
  int transfer_qty = 0;
  UnicodeString itemOrTabName = "";

  int seat_key = 0;
  int dest_tabkey = 0;
  bool process_transfer = false;
  double itemQty = 0;
  int itemPosition=0;

  if(lbDisplayTransferto->Count > 0 && CurrentSourceDisplayMode == eTables)
  {
     if(CheckDesttaffTabAccess(*DBTransaction))
     {
          for(int i = 0; i < lbDisplayTransferto->Count; i++)
          {
              if(lbDisplayTransferto->Selected[i])
              {
                 itemPosition=i;
                 tab_key = GetTabKeyFromListBox(lbDisplayTransferto, i);
                 isTabSelected = CheckTabOrItemSelected(lbDisplayTransferto, i);
                 itemOrTabName = GetItemName(lbDisplayTransferto, i, isTabSelected);
                 if(!isTabSelected)
                 {
                    isSideSelected = CheckSideIsSelected(lbDisplayTransferto, i);
                    if(!isSideSelected)
                    {
                        k = i;
                        itemQty = GetItemQty(lbDisplayTransferto, i, isTabSelected);
                    }
                 }
              }
          }
          if(!isSideSelected)
          {
              isClipLongPress=false;
              TfrmTransferItemOrGuest* frmTransferItemOrGuest = new TfrmTransferItemOrGuest(this);
              frmTransferItemOrGuest->lbeItemName->Caption = itemOrTabName;
              if(!isTabSelected)
              {
                 frmTransferItemOrGuest->SelectedItem = true;
                 frmTransferItemOrGuest->GetItemQty = itemQty;
              }
              if(CurrentSourceDisplayMode == eTables)
              {
                __int64 maxSeatCount = getMaxSeatCount();
                for( __int64 i = 1; i <= maxSeatCount; i++)
                {
                   frmTransferItemOrGuest->DestItems->AddObject(TGlobalSettings::Instance().SeatLabel + " " + IntToStr(i), (TObject *)i);
                }
              }
              else
              {
                  for(int i = 0; i < lbDisplayTransferfrom->Count; i++)
                  {
                     int dest_tabkey = GetTabKeyFromListBox(lbDisplayTransferfrom, i);
                     frmTransferItemOrGuest->DestItems->AddObject(lbDisplayTransferfrom->Items->Strings[i], (TObject *)dest_tabkey);
                  }
              }
              frmTransferItemOrGuest->SourceTable = btnTransferTo->Caption;
              frmTransferItemOrGuest->DestTableName = btnTransferFrom->Caption;
              if(frmTransferItemOrGuest->ShowModal() != mrCancel)
              {

                 dest_tabkey = frmTransferItemOrGuest->dest_tab_key;
                  CheckClipAndTableCondition(*DBTransaction);

                   int sourceTabKey = GetTabKeyFromListBox(lbDisplayTransferto, itemPosition);

                    TMMTabType sourceTabType = TDBTab::GetLinkedTableAndClipTab(*DBTransaction,sourceTabKey);
                    int seatNo= dest_tabkey;
                    if(sourceTabType==TabClipp )
                    {
                       MessageBox("Linked tab/table cannot be transferred to other locations..", "Error", MB_OK + MB_ICONERROR);
                       return ;
                    }
                 if(ClipTabSelected)
                  {
                    isClipLongPress=true;
                    if( !ItemTransferredFromClip )
                    {
                       if(TDBTab::GetTabType(*DBTransaction,sourceTabKey)==TabClipp )
                        {
                             ItemTransferredFromClip =true;
                        }
                    }
                  }

                 if(btnTransferFrom->Caption == btnTransferTo->Caption)
                 {
                    if(!isTabSelected)
                    {
                       tab_key = GetOrderKeyFromSavedList(*DBTransaction, 1, lbDisplayTransferto, k);
                    }
                    seat_key = GetSeatNumber(*DBTransaction, tab_key, isTabSelected, CurrentDestTable);
                    if(seat_key == dest_tabkey)
                    {
                      process_transfer = true;
                      IsPartialTransfer = true;
                    }
                 }
                 if(!process_transfer)
                 {
                     IsPartialTransfer = false;
                     transfer_qty = frmTransferItemOrGuest->ItmQty;
                     if(!isTabSelected)
                     {
                         GetOrderKeyFromSavedList(*DBTransaction, transfer_qty, lbDisplayTransferto, k, true);
                         for(std::vector<int>::iterator it = GetSavedOrderKey.begin(); it != GetSavedOrderKey.end(); ++it)
                         {
                           ReverseTransferTotal(*it, dest_tabkey, true,  isTabSelected);
                         }
                         GetSavedOrderKey.clear();
                     }
                     else
                     {
                        ReverseTransferTotal(tab_key, dest_tabkey, true,  isTabSelected);
                     }
                 }
              }
              delete frmTransferItemOrGuest;
          }
      }
  }
}

void __fastcall TfrmTransfer::btnUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(lbDisplayTransferto->ItemIndex > 0)
   {
      lbDisplayTransferto->ItemIndex--;
      if (lbDisplayTransferto->ItemIndex == 0)
      {
         lbDisplayTransferto->ItemIndex++;
      }
      else
      {
         lbDisplayTransferto->ItemIndex--;
      }
   }
}

void __fastcall TfrmTransfer::btnDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (lbDisplayTransferto->ItemIndex >= 0)
    {
        lbDisplayTransferto->ItemIndex++;
    }
}

void __fastcall TfrmTransfer::btnSourceUpMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(lbDisplayTransferfrom->ItemIndex > 0)
   {
      lbDisplayTransferfrom->ItemIndex--;
   }
}

void __fastcall TfrmTransfer::btnSourceDownMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (lbDisplayTransferfrom->ItemIndex >= 0)
	{
		lbDisplayTransferfrom->ItemIndex++;
	}
}

bool TfrmTransfer::CheckStaffTabAccess(Database::TDBTransaction &DBTransaction)
{
  int tabkey = 0;
  bool retVal = true;
  if(CurrentSourceDisplayMode == eTabs && CurrentSourceTabType == TabStaff)
  {
      TimerLongPress->Enabled = false;
      tabkey = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
      if(tabkey > 0)
      {
        if(!(TabLockedOk(DBTransaction, tabkey) && TabStaffAccessOk(DBTransaction, tabkey) && TabPINOk(DBTransaction, tabkey)))
        {
          lbDisplayTransferfrom->Selected[0] = false;
          retVal = false;
        }
      }
  }
  return retVal;
}

bool TfrmTransfer::CheckDesttaffTabAccess(Database::TDBTransaction &DBTransaction)
{
  int tabkey = 0;
  bool retVal = true;
  if(CurrentDestDisplayMode == eTabs && CurrentDestTabType == TabStaff)
  {
      TimerDestLongPress->Enabled = false;
      tabkey = GetTabKeyFromListBox(lbDisplayTransferto, 0);
      if(tabkey > 0)
      {
        if(!(TabLockedOk(DBTransaction, tabkey) && DestTabStaffAccessOk(DBTransaction, tabkey) && TabPINOk(DBTransaction, tabkey)))
        {
           lbDisplayTransferto->Selected[0] = false;
           retVal = false;
        }
      }
  }
  return retVal;
}
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void TfrmTransfer::PopulateSourceDestTabDetails(Database::TDBTransaction &DBTransaction,
                   UnicodeString Name, int key, TListBox *listBox, bool isTabKeyEmpty)
{
   TTabDetails TabInfo;
   Tabs.clear();
   UnicodeString checkItemName =""; //
   int tabkey = 0;
   TabInfo.Tabkey = key;
   TabInfo.TabName = Name;
   std::auto_ptr <TList> TabItemList(new TList);
   std::set <__int64> SelectedTabs;
   SelectedTabs.insert(key);
   TDBOrder::GetOrdersFromTabKeys(DBTransaction, TabItemList.get(), SelectedTabs);

   for(int x = 0; x < TabItemList->Count;x++)
   {
      TItemComplete *item = (TItemComplete*)TabItemList->Items[x];
      item->RunBillCalculator();
      TItemInfo itemInfo;

      itemInfo.Tabkey = key;
      itemInfo.OrderKey = item->OrderKey;
      itemInfo.ItemQty = item->GetQty();
      itemInfo.ItemName = item->Item;
      itemInfo.ItemPrice = FormatFloat("0.00",item->FinalPrice_BillCalc());
      itemInfo.IsSideItem = item->IsSide;
      itemInfo.ItemSize = item->Size;
      itemInfo.TabName = item->TabName;
      itemInfo.CheckHasSide = TDBOrder::CheckItemHasSide(DBTransaction, item->OrderKey);
      itemInfo.IsPriceAdjust = CheckSecurityEvent(DBTransaction, item->Security->GetSecurityRefNumber());
      itemInfo.isWeighted = item->WeightedPrice.WeightedItem;
      TabInfo.TabItems.push_back(itemInfo);
   }

   if(TabItemList->Count > 0)
   {
     Tabs.push_back(TabInfo);
   }
   else
   {
      if(isTabKeyEmpty)
      {
         Tabs.push_back(TabInfo);
         isTabKeyEmpty = false;
      }
   }

   for(std::vector<TTabDetails>::iterator it = Tabs.begin(); it != Tabs.end(); ++it)
   {
      TTabColour *TabColor = new TTabColour;
      UnicodeString tabName = it->TabName;
      UnicodeString itemName ="";
      TabColor->Tabkey = it->Tabkey;
      TabColor->IsTabKey = true;
      DisplayItemsSourceList(listBox, TabColor, tabName);
      for(std::vector<TItemInfo>::iterator ab = it->TabItems.begin(); ab != it->TabItems.end(); ++ab)
      {
           TTabColour lastItem;
           bool check_vale = ab->IsPriceAdjust;

           bool addToLast = false;
           if(CollectItemDetails.size() > 0)
           {
              lastItem = CollectItemDetails.at(CollectItemDetails.size()-1);
              if(lastItem.Tabkey ==  ab->Tabkey &&
              lastItem.ItemName == ab->ItemName &&
              lastItem.ItemSize == ab->ItemSize &&
              lastItem.CheckHasSide == ab->CheckHasSide &&
              lastItem.IsSideItem == ab->IsSideItem &&
              lastItem.IsPriceAdjust == ab->IsPriceAdjust &&
              lastItem.isWeighted == ab->isWeighted)

              {
                addToLast = true;
                CollectItemDetails.pop_back();
              }
           }
           if(!addToLast)
           {
               //double qty =
               TTabColour tabdetails;
               tabdetails.Orderkey = ab->OrderKey;
               tabdetails.Tabkey = ab->Tabkey;
               tabdetails.IsTabKey = false;
               tabdetails.IsSideItem = ab->IsSideItem;
               tabdetails.Qty = ab->ItemQty;
               tabdetails.ItemName = ab->ItemName;
               tabdetails.ItemSize = ab->ItemSize;
               tabdetails.ItemPrice = ab->ItemPrice;
               tabdetails.TabName = ab->TabName;
               tabdetails.CheckHasSide = ab->CheckHasSide;
               tabdetails.IsPriceAdjust = ab->IsPriceAdjust;
               tabdetails.isWeighted = ab->isWeighted;
               itemName = ab->ItemName;
               tabdetails.SaveOrderKey.push_back(ab->OrderKey);
               tabdetails.SaveItemQty.push_back(ab->ItemQty);
               CollectItemDetails.push_back(tabdetails);
           }
           else
           {
               lastItem.Qty += ab->ItemQty;
               double finPrice = lastItem.ItemPrice.ToDouble();
               finPrice += ab->ItemPrice.ToDouble();
               Currency price = finPrice;
               lastItem.ItemPrice = FormatFloat("0.00",price);
               lastItem.SaveItemQty.push_back(ab->ItemQty);
               lastItem.SaveOrderKey.push_back(ab->OrderKey);
               CollectItemDetails.push_back(lastItem);
           }
      }

      for(std::vector<TTabColour>::iterator ab = CollectItemDetails.begin(); ab != CollectItemDetails.end(); ++ab)
      {
           TTabColour *tabdetails = new TTabColour;
           tabdetails->Orderkey = ab->Orderkey;
           tabdetails->Tabkey = ab->Tabkey;
           tabdetails->IsTabKey = false;
           tabdetails->IsSideItem = ab->IsSideItem;
           tabdetails->Qty = ab->Qty;
           tabdetails->ItemName = ab->ItemName;
           tabdetails->ItemSize = ab->ItemSize;
           tabdetails->TabName = ab->TabName;
           tabdetails->ItemPrice = ab->ItemPrice;
           itemName = GetItemQtyWithName(tabdetails->Qty,ab->ItemSize ,ab->ItemName);
           tabdetails->SaveOrderKey = ab->SaveOrderKey;
           tabdetails->SaveItemQty = ab->SaveItemQty;
           DisplayItemsSourceList(listBox, tabdetails, itemName);
      }
       CollectItemDetails.clear();
   }
   Tabs.clear();
}

void TfrmTransfer::DisplayItemsSourceList(TListBox *listBox, TTabColour *TabColor, UnicodeString Name)
{
  listBox->AddItem(Name, (TObject *)TabColor);
  listBox->ItemIndex++;
}

void TfrmTransfer::ClearListBox(TListBox *listBox)
{
    int i = 0;
    while(listBox->Count > i)
    {
       TTabColour* item = (TTabColour *)listBox->Items->Objects[i];
       if(item)
         delete item;
       i++;
    }
    listBox->Clear();
}


void TfrmTransfer::UpdateListBox(TListBox *listBox)
{
   listBox->TopIndex = 0;
   listBox->Refresh();
}

int TfrmTransfer::GetTabKeyFromListBox(TListBox *listBox, int i)
{
    int key = 0;
    if(listBox->Count > 0)
    {
       TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[i];
       key = getDestId->Tabkey;

    }
    return key;
}

bool TfrmTransfer::CheckTabOrItemSelected(TListBox *listBox, int i)
{
    bool isTabSelected = false;
    if(listBox->Count > 0)
    {
      TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[i];
      isTabSelected = getDestId->IsTabKey;
    }
    return isTabSelected;
}

TModalResult TfrmTransfer::ShowTabDetails(Database::TDBTransaction &DBTransaction, int TabType, TListBox *listbox, TTouchBtn *btntransfer, AnsiString title, AnsiString Section)
{
    TModalResult Retval = mrOk;
    std::auto_ptr<TfrmVerticalSelect>SelectionForm(TfrmVerticalSelect::Create<TfrmVerticalSelect>(this));
    TVerticalSelection Item;
    Item.Title = "Cancel";
    Item.Properties["Color"] = IntToStr(clMaroon);
    Item.CloseSelection = true;
    SelectionForm->Items.push_back(Item);
    std::auto_ptr<TStringList>TabList(new TStringList);
    switch(TabType)
    {
        case TabNormal:
        {
            if(Section == "Select Transfer To")
            {
                Item.Title = "New Tab";
				Item.Properties["TabKey"] = -1;
				Item.Properties["Color"] = clNavy;
				Item.CloseSelection = true;
				SelectionForm->Items.push_back(Item);
            }
            TDBTab::GetTabs(DBTransaction, TabList.get(), TabType);
            for (int i = 0; i < TabList->Count; i++)
            {
                Item.Title = TabList->Strings[i];
                Item.Properties["TabKey"] = (int)TabList->Objects[i];
                Item.Properties["Color"] = clInfoBk;
                Item.CloseSelection = true;
                SelectionForm->Items.push_back(Item);
            }
        }break;
        case TabStaff:
        {
            std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
            Staff->GetTabs(DBTransaction, TabList.get());
            for (int i = 0; i < TabList->Count; i++)
            {
                Item.Title = TabList->Strings[i];
                Item.Properties["TabKey"] = (int)TabList->Objects[i];
                Item.Properties["Color"] = clInfoBk;
                Item.CloseSelection = true;
                SelectionForm->Items.push_back(Item);
            }
        }break;
         case TabClipp:
        {
            TDBClippTab::GetOpenClippTabs(DBTransaction, TabList.get(), TabType);
            for (int i = 0; i < TabList->Count; i++)
            {
                Item.Title = TabList->Strings[i];
                Item.Properties["TabKey"] = (int)TabList->Objects[i];
                Item.Properties["Color"] = clInfoBk;
                Item.CloseSelection = true;
                SelectionForm->Items.push_back(Item);
            }
        }break;
    }
    SelectionForm->ShowModal();
    TVerticalSelection SelectedItem;
    if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
    {
        int tabKey = SelectedItem.Properties["TabKey"];
        ClearListBox(listbox);
        //btntransfer->Caption = title;
        if(tabKey == -1)
        {
           Retval = AddNewTab(DBTransaction);
        }
        else
        {
            UnicodeString tabname = SelectedItem.Title;
            if(Section == "Select Transfer From")
            {
               CurrentSourceDisplayMode = eTabs;
               if(TabType == TabNormal)
               {
                  CurrentSourceTabType = TabNormal;
               }
               if(TabType == TabStaff)
               {
                  CurrentSourceTabType = TabStaff;
               }
                if(TabType == TabClipp)
               {
                  CurrentSourceTabType = TabClipp;
               }
            }
            else
            {
               if(Section == "Select Transfer To")
               {
                  CurrentDestDisplayMode = eTabs;
                if(TabType == TabNormal)
                {
                  CurrentDestTabType = TabNormal;
               }
               if(TabType == TabStaff)
               {
                  CurrentDestTabType = TabStaff;
               }

                if(TabType == TabClipp)
                {
                    CurrentDestTabType = TabClipp;
                }
               }
            }
            PopulateSourceDestTabDetails(DBTransaction, tabname, tabKey, listbox, true);
            UpdateListBox(listbox);
        }
    }
    else
    {
      Retval = mrAbort;
    }
    return Retval;
}

UnicodeString TfrmTransfer::GetSourceTabName(Database::TDBTransaction &DBTransaction, long source_key)
{
    UnicodeString tabName ="";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        if(CurrentSourceDisplayMode == eTabs)
        {
           IBInternalQuery->SQL->Text =
            "SELECT B.TAB_NAME FROM TAB B WHERE TAB_KEY = :TAB_KEY group by B.TAB_NAME " ;
        }
        else
        {
           IBInternalQuery->SQL->Text =
            "select B.TAB_NAME from ORDERS B where TAB_KEY = :TAB_KEY group by B.TAB_NAME " ;
        }
        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = source_key;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           tabName = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
        }
        IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return tabName;
}

void TfrmTransfer::DeleteEmptyTabs(Database::TDBTransaction &DBTransaction)
{
   if(CurrentSourceDisplayMode == eTabs && (CurrentSourceTabType == TabNormal || CurrentSourceTabType == TabClipp))
   {
      if(lbDisplayTransferfrom->Count > 0)
      {
        int key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
        if (TDBTab::GetIsEmpty(DBTransaction, key))
        {
           Currency credit = TDBTab::GetTabCredit(DBTransaction, key);
           if(!(credit > 0))
           {
              TDBTab::ClearTab(DBTransaction, key);
           }
        }
      }

   }
}

void TfrmTransfer::ShowSelectScreen(Database::TDBTransaction &DBTransaction, AnsiString title)
{
   try
   {
        TModalResult Retval = mrOk;
        TSaveOrdersTo OrderContainer;
        std::auto_ptr<TfrmSelectSaveOption>SelectionForm(TfrmSelectSaveOption::Create<TfrmSelectSaveOption>(this));
        SelectionForm->Caption = title;
        do
        {
            Retval = mrOk;

            if(SelectionForm->ShowModal() != mrCancel)
            {
              if(ClipTabSelected)
                {
                    MessageBox("You cannot Changed option now once clip tab and table is selected", "Error",MB_OK + MB_ICONINFORMATION);
                    Retval = mrAbort;
                    return;
                }
                isTabOrItemSelected = false;
                ItemTransferredFromClip=false;
                if(title == "Select Transfer From")
                {
                   SourceAllowStaffTab = false;
                   SourceTempUserInfo.Clear();
                   TDeviceRealTerminal::Instance().User = TempUserInfo;
                }
                else if(title == "Select Transfer To")
                {
                   DestAllowStaffTab = false;
                   DestTempUserInfo.Clear();
                   TDeviceRealTerminal::Instance().User = TempUserInfo;
                }
                OrderContainer.Location["TMMDisplayMode"] = SelectionForm->DisplayMode;
                OrderContainer.Location["TMMTabType"] = SelectionForm->SelectedTabType;
                switch(int(OrderContainer.Location["TMMTabType"]))
                {

                    case TabNormal:
                    {
                       TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
                       if(title == "Select Transfer From")
                       {
                           Retval = ShowTabDetails(DBTransaction, TabNormal, lbDisplayTransferfrom, btnTransferFrom, "Tabs", "Select Transfer From");
                       }
                       if(title == "Select Transfer To")
                       {
                          Retval = ShowTabDetails(DBTransaction, TabNormal, lbDisplayTransferto, btnTransferTo, "Tabs", "Select Transfer To");
                       }
                    }break;
                    case TabStaff:
                      {
                        TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
                        if(title == "Select Transfer From")
                        {
                           Retval = ShowTabDetails(DBTransaction, TabStaff, lbDisplayTransferfrom, btnTransferFrom, "Staff", "Select Transfer From");
                        }
                        else
                        {
                          TempDestUserInfo.Clear();
                          Retval = ShowTabDetails(DBTransaction, TabStaff, lbDisplayTransferto, btnTransferTo, "Staff", "Select Transfer To");
                        }
                    }break;
                case TabClipp:
                  {
                    TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
                    if(title == "Select Transfer From")
                    {
                       Retval = ShowTabDetails(DBTransaction, TabClipp, lbDisplayTransferfrom, btnTransferFrom, "Clipp Tabs", "Select Transfer From");
                    }
                    else
                    {
                      TempDestUserInfo.Clear();
                      Retval = ShowTabDetails(DBTransaction, TabClipp, lbDisplayTransferto, btnTransferTo, "Clipp Tabs", "Select Transfer To");
                    }

                  if(title == "Select Transfer From")
                  {
                      ClipPresentInFromPanel =true;
                      ClipPresentInToPanel=false;
                  }
                  else
                  {
                     ClipPresentInToPanel=true;
                     ClipPresentInFromPanel=false;
                  }
                  isClipLongPress=false;

                }break;
                case TabMember:
                    {
                        if(title == "Select Transfer From")
                        {
                           if(ShowSourceMembersTab(DBTransaction))
                           {
                              CurrentSourceDisplayMode = eTabs;
                              CurrentSourceTabType = TabMember;
                              TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
                              UpdateSourceTableDetails(DBTransaction);
                              UpdateSourceSeatDetails(DBTransaction);
                           }
                           else
                           {
                              Retval = mrAbort;
                           }

                        }
                        else
                        {
                           if(ShowDestMembersTab(DBTransaction))
                           {
                              if (TempDestUserInfo.TabEnabled)
                              {
                                 CurrentDestDisplayMode = eTabs;
                                 CurrentDestTabType = TabMember;
                                 TDBTab::ReleaseTab(DBTransaction,TDeviceRealTerminal::Instance().ID.Name,0);
                                 UpdateDestTableDetails(DBTransaction);
                                 UpdateDestSeatDetails(DBTransaction);
                              }
                              else
                              {
                                 MessageBox("Credit Disabled " + TempDestUserInfo.Name + " " + TempDestUserInfo.AccountInfo, "Sorry No Credit, No Invoices.",
                                    MB_OK + MB_ICONINFORMATION);
                                    Retval = mrAbort;
                              }
                           }
                           else
                           {
                             Retval = mrAbort;
                           }
                        }
                    }break;
                case TabTableSeat:
                    {
                        if(title == "Select Transfer To")
                        {
                             TFloorPlanReturnParams floorPlanReturnParams;
                             CurrentDestDisplayMode = eTables;
                            // Runs new web app of floorPlan
                           if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, true, floorPlanReturnParams ) )
                           {
                              lbDisplayTransferto->Clear();
                              if( CurrentDestTable != floorPlanReturnParams.TabContainerNumber )
                              {
                                 CurrentDestTable = floorPlanReturnParams.TabContainerNumber;
                              }
                              UpdateDestTableDetails(DBTransaction);
                              UpdateDestSeatDetails(DBTransaction);
                              CurrentDestDisplayMode =eTables;
                           }
                           else
                           {
                              floorPlanReturnParams.Ver = 0;
                              Retval = mrAbort;
                           }

                        }
                        else
                        {
                             TFloorPlanReturnParams floorPlanReturnParams;
                             CurrentSourceDisplayMode = eTables;
                            // Runs new web app of floorPlan
                           if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, false, floorPlanReturnParams ) )
                           {
                             if( CurrentSourceTable != floorPlanReturnParams.TabContainerNumber )
                             {
                                CurrentSourceTable = floorPlanReturnParams.TabContainerNumber;
                             }
                             UpdateSourceTableDetails(DBTransaction);
                             UpdateSourceSeatDetails(DBTransaction);
                             CurrentSourceDisplayMode = eTables;
                           }
                           else
                           {
                               floorPlanReturnParams.Ver = 0;
                               Retval = mrAbort;
                           }
                        }

                    }break;
                case TabRoom:
                    {
                       if(title == "Select Transfer To")
                       {
                         UpdateDestRooms(DBTransaction);
                       }
                       else
                       {
                         UpdateSourceRooms(DBTransaction);
                       }
                    }break;
                case TabInvoice:
                    {
                       NewInvoiceKey = 0;
                       InvoiceKey = 0;
                       InvoiceTabKey = 0;
                       Retval = NewInvoiceItems(DBTransaction);
                    }
                    break;
                    default:
                        Retval = mrCancel;
                    }
               }
               else
               {
                  Retval = mrCancel;
               }
           }
           while (Retval == mrAbort);                        }
    catch(Exception & E)
    {
        MessageBox(E.Message, "Error", MB_OK + MB_ICONERROR);
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

void TfrmTransfer::PrepareItemList(Database::TDBTransaction &DBTransaction, int sourceKey, bool isTabSelected, TList *Orders)
{
   std::set <__int64> SelectedKeys;
   SelectedKeys.insert(sourceKey);
   if(isTabSelected)
    {
      TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, Orders, SelectedKeys, true);
    }
    else
    {
       TDBOrder::GetOrdersIncludingSidesFromOrderKeys(DBTransaction, Orders, SelectedKeys, true);
    }
}


void TfrmTransfer::TransferData(Database::TDBTransaction &DBTransaction)
{
       CheckClipAndTableCondition(DBTransaction);
       if(IsSourceDestinationSame() && !ClipTabSelected)
       {
          IsPartialTransfer = true;
          return;
       }

       int sourcekey, dest_key = 0;
       bool isTabSelected;
       UnicodeString itemName = "";
       bool isSideSelected = false;
       double itemQty = 0;
       int k = 0;

       for(int i = 0; i < lbDisplayTransferfrom->Count; i++)
       {
          if(lbDisplayTransferfrom->Selected[i])
          {
            isTabSelected = CheckTabOrItemSelected(lbDisplayTransferfrom, i);
            sourcekey = GetTabKeyFromListBox(lbDisplayTransferfrom, i);

            if(ClipTabSelected && !ItemTransferredFromClip )
            {
               if(TDBTab::GetTabType(DBTransaction,sourcekey)==TabClipp )
                {
                     ItemTransferredFromClip =true;
                }
            }
           if(!isTabSelected)
            {
              itemName = lbDisplayTransferfrom->Items->Strings[i];
              isSideSelected = CheckSideIsSelected(lbDisplayTransferfrom, i);
              if(!isSideSelected)
              {
                k = i;
              }
            }
            break;
          }
       }
       if(!isSideSelected)
       {

           if (CurrentSourceDisplayMode == eTables && CurrentDestDisplayMode == eTables)
           {
              if(!isTabSelected)
              {
                 sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferfrom, k);
                 dest_key = GetSeatNumber(DBTransaction, sourcekey, isTabSelected, CurrentSourceTable);
              }
              else
              {
                 dest_key = GetSeatNumber(DBTransaction, sourcekey, isTabSelected, CurrentSourceTable);
              }
           }
           else if(CurrentDestDisplayMode == eTables)
           {
              dest_key = 1;
              if(!isTabSelected)
              {
                 sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferfrom, k);
              }
           }
           else if(CurrentDestDisplayMode == eTabs  || CurrentDestDisplayMode == eInvoices || CurrentDestDisplayMode == eRooms)
           {
              dest_key = GetTabKeyFromListBox(lbDisplayTransferto, 0);
              if(!isTabSelected)
              {
                 sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferfrom, k);
              }
           }

        if(CheckIfTransferringLinkedTabItems(DBTransaction,sourcekey,isTabSelected) )
         {
           MessageBox("Linked tab/table cannot be transferred to other locations.", "Error", MB_OK + MB_ICONERROR);
           isTabOrItemSelected = false;
           ItemTransferredFromClip=false;
           return ;
         }
           TransferTotal(sourcekey, dest_key, false, isTabSelected);
       }


}

void TfrmTransfer::ReverseData(Database::TDBTransaction &DBTransaction)
{

       CheckClipAndTableCondition(DBTransaction);

       if(IsSourceDestinationSame() && !ClipTabSelected)
       {
          IsPartialTransfer = true;
          return;
       }

       int sourcekey, dest_key = 0;
       bool isTabSelected;
       bool isSideSelected = false;
       int k = 0;

      for(int i = 0; i < lbDisplayTransferto->Count; i++)
       {
          if(lbDisplayTransferto->Selected[i])
          {
            isTabSelected = CheckTabOrItemSelected(lbDisplayTransferto, i);
            sourcekey = GetTabKeyFromListBox(lbDisplayTransferto, i);
            if(ClipTabSelected && !ItemTransferredFromClip )
            {
               if(TDBTab::GetTabType(DBTransaction,sourcekey)==TabClipp )
                {
                     ItemTransferredFromClip =true;
                }
            }
            if(!isTabSelected)
            {
               isSideSelected = CheckSideIsSelected(lbDisplayTransferto, i);
            }
            k = i;
            break;
          }
       }

       if(!isSideSelected)
       {
           if (CurrentSourceDisplayMode == eTables )
           {
              if(CurrentDestDisplayMode == eTables)
              {
                 if(isTabSelected)
                 {
                     dest_key = GetSeatNumber(DBTransaction, sourcekey, isTabSelected, CurrentDestTable);
                 }
                 else
                 {
                     sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferto, k);
                     dest_key = GetSeatNumber(DBTransaction, sourcekey, isTabSelected, CurrentDestTable);
                 }
              }
              else
              {
                  dest_key = 1;
                  if(!isTabSelected)
                  {
                     sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferto, k);
                  }
              }

           }
           else if(CurrentSourceDisplayMode == eTabs)
           {
              dest_key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
              if(!isTabSelected)
              {
                 sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferto, k);
              }

           }
           else if(CurrentSourceDisplayMode == eRooms)
           {
              dest_key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
              if(!isTabSelected)
              {
                 sourcekey = GetOrderKeyFromSavedList(DBTransaction, 1, lbDisplayTransferto, k);
              }

           }

            if(CheckIfTransferringLinkedTabItems(DBTransaction,sourcekey,isTabSelected) )
             {
               MessageBox("Linked tab/table cannot be transferred to other locations.", "Error", MB_OK + MB_ICONERROR);
               isTabOrItemSelected = false;
               ItemTransferredFromClip=false;
               return ;
             }

            ReverseTransferTotal(sourcekey, dest_key, true, isTabSelected);
       }
}

void TfrmTransfer::ReverseTransferTotal(int source_key, int dest_tabkey, bool isReverse, bool isTabSelected)
{
   try
   {
	  switch(CurrentSourceDisplayMode)
	  {
	  case eRooms:
		 {
			int RoomTabKey = TDBRooms::FindOrCreateRoomTab(*DBTransaction, CurrentSourceRoom);
			std::auto_ptr <TList> OrdersList(new TList);
            PrepareItemList(*DBTransaction, source_key, isTabSelected , OrdersList.get()) ;

			if (TDBOrder::CheckTransferCredit(*DBTransaction, OrdersList.get(), RoomTabKey))
			{
			   TDBOrder::TransferOrders(*DBTransaction, OrdersList.get(), RoomTabKey,TDeviceRealTerminal::Instance().User.ContactKey, source_key,true);
			}
			else
			{
			   MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
			}
			TDBRooms::SetRoomStatus(*DBTransaction, CurrentSourceRoom, eOccupied);
		 }break;
	  case eTabs:
		 {
           std::auto_ptr <TList> OrdersList(new TList);
           PrepareItemList(*DBTransaction, source_key, isTabSelected , OrdersList.get()) ;
           int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTab(*DBTransaction,dest_tabkey);

            TMMTabType TabType = TDBTab::GetTabType(*DBTransaction, dest_tabkey);
            TList *Orders = OrdersList.get();
            bool isSCDApplied = false;
            bool isPWDAppplied = false;
            if(TabType == 13)
            {
                isSCDApplied = IsSCDAppliedOnDest(Orders);
                isPWDAppplied = IsPWDAppliedOnDest(Orders);
            }
            if(isSCDApplied )
            {
                MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
            }
            else if(isPWDAppplied)
            {
                MessageBox("Order with PWD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
            }
            else
            {
                   if (TDBOrder::CheckTransferCredit(*DBTransaction, OrdersList.get(), dest_tabkey))
                   {
                      TDBOrder::TransferOrders(*DBTransaction, OrdersList.get(), dest_tabkey,TDeviceRealTerminal::Instance().User.ContactKey,source_key, true);
                      TDBOrder::SetOrderIdentificationNumberForTab(*DBTransaction,dest_tabkey,identificationNumber);
                      if(cmClientManager->ChefMateEnabled())
                       {
                          CollectDataForChefmateTransfer(dest_tabkey, OrdersList.get(), lbDisplayTransferto, true);
                       }
                   }
                   else
                   {
                      MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                   }
            }

		 }break;
	  case eTables:
		 {
            int DestTabKey = 0;
            if(ClipTabSelected && isTabSelected && !isClipLongPress)
            {
               TMMTabType tabtype = TDBTab::GetTabType(*DBTransaction, source_key,isTabSelected);
                if(tabtype == TabClipp)
                {
                    dest_tabkey = TDBTab::GetMaxAvailableSeatKey(*DBTransaction, CurrentSourceTable);
                }

            }

            int SeatKey = TDBTables::GetOrCreateSeat(*DBTransaction, CurrentSourceTable, dest_tabkey);
            bool tabName = CheckNameIsNullOrNot(*DBTransaction, CurrentSourceTable, dest_tabkey, SeatKey); //reverse
            if(tabName)
            {
               DestTabKey = TDBTab::GetOrCreateTab(*DBTransaction, TDBTables::GetTabKey(*DBTransaction, CurrentSourceTable, dest_tabkey));
            }
            else
            {
               DestTabKey = TDBTab::GetOrCreateTab(*DBTransaction, TDBTables::GetTabKey(*DBTransaction, SeatKey));
            }
			TDBTab::SetTabType(*DBTransaction, DestTabKey, TabTableSeat);
			TDBTables::SetSeatTab(*DBTransaction, SeatKey, DestTabKey);
            std::auto_ptr <TList> OrdersList(new TList);
            PrepareItemList(*DBTransaction, source_key, isTabSelected , OrdersList.get()) ;
            int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTable(*DBTransaction,CurrentSourceTable);

            if(ClipTabSelected)
            {
                if(CheckIfClipTransferringToAnotherLinkedGuest(*DBTransaction,source_key, DestTabKey, isTabSelected ))
                {
                  return;
                }
            }

            if (TDBOrder::CheckTransferCredit(*DBTransaction, OrdersList.get(), DestTabKey))
            {
               TDBOrder::TransferOrders(*DBTransaction, OrdersList.get(), DestTabKey,TDeviceRealTerminal::Instance().User.ContactKey,source_key, true);
               TDBOrder::SetOrderIdentificationNumberForTable(*DBTransaction,CurrentSourceTable,identificationNumber);
               if(cmClientManager->ChefMateEnabled())
                {
                   //CollectDataForChefmateTransfer(dest_tabkey, OrdersList.get(), lbDisplayTransferto);
//                   ChefMateOrderList.pop_back();
                   CollectDataForChefmateTransfer(DestTabKey, OrdersList.get(), lbDisplayTransferto, true);
                }
            }
            else
            {
               MessageBox("Insufficient credit on Source Tab.", "Warning", MB_OK + MB_ICONWARNING);
            }
		 }break;
	  case eInvoices:
		 {
            bool ReverseTransfer=true;
            if(TDBTab::GetIsEmpty(*DBTransaction, source_key))
            {
               if(CheckItemsOnTab(*DBTransaction, source_key))
               {
                  MessageBox("This Source 'Tab' is Empty.", "Error", MB_OK + MB_ICONERROR);
               }
               else
               {
                  ProcessInvoice(*DBTransaction, source_key, isTabSelected,ReverseTransfer);
               }
            }
            else
            {
               ProcessInvoice(*DBTransaction, source_key, isTabSelected,ReverseTransfer);
            }
		 }
	  }
   }
   catch(Exception & E)
   {
	  MessageBox(E.Message, "Error", MB_OK + MB_ICONWARNING);
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   // Most likly not Required in 3.3+
   TNetMessageTabChanged *Request = new TNetMessageTabChanged;
   TDeviceRealTerminal::Instance().ManagerNet->SendToAll(Request);
   delete Request;
   UpdateSourceTableDetails(*DBTransaction);
   UpdateSourceSeatDetails(*DBTransaction);
   UpdateDestTableDetails(*DBTransaction);
   UpdateDestSeatDetails(*DBTransaction);
   IsPartialTransferDone = true;
}

void TfrmTransfer::TransferTotal(int source_key, int dest_tabkey, bool isReverse, bool isTabSelected)
{
   try
   {
	  switch(CurrentDestDisplayMode)
	  {
	  case eRooms:
		 {
			int RoomTabKey = TDBRooms::FindOrCreateRoomTab(*DBTransaction, CurrentDestRoom);
			std::auto_ptr <TList> OrdersList(new TList);
			PrepareItemList(*DBTransaction, source_key, isTabSelected , OrdersList.get()) ;
			if (TDBOrder::CheckTransferCredit(*DBTransaction, OrdersList.get(), RoomTabKey))
			{
			   TDBOrder::TransferOrders(*DBTransaction, OrdersList.get(), RoomTabKey,TDeviceRealTerminal::Instance().User.ContactKey, source_key,true);
			}
			else
			{
			   MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
			}
			TDBRooms::SetRoomStatus(*DBTransaction, CurrentDestRoom, eOccupied);

		 }break;
	  case eTabs:
		 {
           std::auto_ptr <TList> OrdersList(new TList);
           PrepareItemList(*DBTransaction, source_key, isTabSelected , OrdersList.get()) ;
           int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTab(*DBTransaction,dest_tabkey);

            TMMTabType TabType = TDBTab::GetTabType(*DBTransaction, dest_tabkey);
            TList *Orders = OrdersList.get();
            bool isSCDApplied = false;
            bool isPWDAppplied = false;
            if(TabType == 13)
            {
                isSCDApplied = IsSCDAppliedOnDest(Orders);
                isPWDAppplied = IsPWDAppliedOnDest(Orders);
            }
            if(isSCDApplied )
            {
                MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
            }
            else if(isPWDAppplied)
            {
                MessageBox("Order with PWD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
            }
            else
            {
               if (TDBOrder::CheckTransferCredit(*DBTransaction, OrdersList.get(), dest_tabkey))
               {
                  TDBOrder::TransferOrders(*DBTransaction, OrdersList.get(), dest_tabkey,TDeviceRealTerminal::Instance().User.ContactKey,source_key, true);
                  TDBOrder::SetOrderIdentificationNumberForTab(*DBTransaction,dest_tabkey,identificationNumber);
                  if (cmClientManager->ChefMateEnabled())
                   {
                      CollectDataForChefmateTransfer(dest_tabkey, OrdersList.get(), lbDisplayTransferfrom);
                   }
               }
               else
               {
                  MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
               }
            }
		 }break;
	  case eTables:
		 {
            int DestTabKey = 0;
           if(ClipTabSelected && isTabSelected && !isClipLongPress)
           {

               TMMTabType tabtype = TDBTab::GetTabType(*DBTransaction, source_key,isTabSelected);
                if(tabtype == TabClipp)
                {
                    dest_tabkey = TDBTab::GetMaxAvailableSeatKey(*DBTransaction, CurrentDestTable);
                }
            }

            int SeatKey = TDBTables::GetOrCreateSeat(*DBTransaction, CurrentDestTable, dest_tabkey);
            bool tabName = CheckNameIsNullOrNot(*DBTransaction, CurrentDestTable, dest_tabkey, SeatKey);
            if(tabName)
            {
               DestTabKey = TDBTab::GetOrCreateTab(*DBTransaction, TDBTables::GetTabKey(*DBTransaction, CurrentDestTable, dest_tabkey));
            }
            else
            {
               DestTabKey = TDBTab::GetOrCreateTab(*DBTransaction, TDBTables::GetTabKey(*DBTransaction, SeatKey));
            }
			TDBTab::SetTabType(*DBTransaction, DestTabKey, TabTableSeat);
			TDBTables::SetSeatTab(*DBTransaction, SeatKey, DestTabKey);
			std::auto_ptr <TList> OrdersList(new TList);
            PrepareItemList(*DBTransaction, source_key, isTabSelected , OrdersList.get()) ;
            int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTable(*DBTransaction,CurrentDestTable);
            UpdateTablePartyName(*DBTransaction, DestTabKey, source_key, isTabSelected);

            if(ClipTabSelected)
            {
                if(CheckIfClipTransferringToAnotherLinkedGuest(*DBTransaction,source_key, DestTabKey, isTabSelected ))
                {
                  return;
                }
            }

            if (TDBOrder::CheckTransferCredit(*DBTransaction, OrdersList.get(), DestTabKey))
            {
                TDBOrder::TransferOrders(*DBTransaction, OrdersList.get(), DestTabKey,TDeviceRealTerminal::Instance().User.ContactKey,source_key, true);
                TDBOrder::SetOrderIdentificationNumberForTable(*DBTransaction,CurrentDestTable,identificationNumber);
                if(cmClientManager->ChefMateEnabled())
                {
                    CollectDataForChefmateTransfer(dest_tabkey, OrdersList.get(), lbDisplayTransferfrom);
                }
            }
            else
            {
               MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
            }

		 }break;
	  case eInvoices:
		 {
            bool ReverseTransfer=false;
            if(TDBTab::GetIsEmpty(*DBTransaction, source_key))
            {
               if(CheckItemsOnTab(*DBTransaction, source_key))
               {
                 MessageBox("This Source 'Tab' is Empty.", "Error", MB_OK + MB_ICONERROR);
               }
               else
               {
                  ProcessInvoice(*DBTransaction, source_key, isTabSelected,ReverseTransfer);
               }
            }
            else
            {
               ProcessInvoice(*DBTransaction, source_key, isTabSelected,ReverseTransfer);
            }
		 }
	  }
   }
   catch(Exception & E)
   {
	  MessageBox(E.Message, "Error", MB_OK + MB_ICONWARNING);
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }
   // Most likly not Required in 3.3+
   TNetMessageTabChanged *Request = new TNetMessageTabChanged;
   TDeviceRealTerminal::Instance().ManagerNet->SendToAll(Request);
   delete Request;
   UpdateSourceTableDetails(*DBTransaction);
   UpdateSourceSeatDetails(*DBTransaction);
   UpdateDestTableDetails(*DBTransaction);
   UpdateDestSeatDetails(*DBTransaction);
   IsPartialTransferDone = true;
}

void TfrmTransfer::TotalTransferTableOrTab(Database::TDBTransaction &DBTransaction)
{
    TTransferComplete *TransferComplete = new TTransferComplete();
	  try
	  {
    	 switch(CurrentDestDisplayMode)
		 {
		 case eRooms:
			{
			   int RoomTabKey = TDBRooms::FindOrCreateRoomTab(DBTransaction, CurrentDestRoom);
			   std::auto_ptr <TList> OrdersList(new TList);
			   std::set <__int64> SelectedTabs;
               int source_key = 0;
               if(CurrentSourceDisplayMode == eTables)
               {
                  TDBTables::GetTabKeys(DBTransaction, CurrentSourceTable, SelectedTabs);
                  TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);
                  for (std::set <__int64> ::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end(); advance(itTab, 1))
                  {
                     source_key = *itTab;
                  }
               }
               else
               {
                   source_key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                   SelectedTabs.insert(source_key);
                   TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);
               }

			   if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), RoomTabKey))
			   {
				  TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), RoomTabKey,TDeviceRealTerminal::Instance().User.ContactKey, source_key); //changes for transfer report..
			   }
			   else
			   {
				  MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
			   }
			   TDBRooms::SetRoomStatus(DBTransaction, CurrentDestRoom, eOccupied);
			}break;
		 case eTabs:
			{
                   std::auto_ptr <TList> OrdersList(new TList);
                   std::set <__int64> SelectedTabs;
                   int source_key;
                   int dest_key = GetTabKeyFromListBox(lbDisplayTransferto, 0);
                   bool tabExists = false;
                   int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTab(DBTransaction, dest_key);

                   TMMTabType TabType = TDBTab::GetTabType(DBTransaction, dest_key);
                   bool isSCDApplied = false;
                   bool isPWDApplied = false;

                   if(CurrentSourceDisplayMode == eTables)
                   {
                      TDBTables::GetTabKeys(DBTransaction, CurrentSourceTable, SelectedTabs);
                      for (std::set <__int64> ::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end(); advance(itTab, 1))
                      {
                         if (TDBTab::GetTabExists(DBTransaction, *itTab))
                         {
                            tabExists = true;
                            std::set <__int64> SelectedTabs;
                            SelectedTabs.insert(*itTab);
                            OrdersList->Clear();
                            TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);

                            //if destination is clipp tab and source have scd discount applied then it can't be transfer
                            if(TabType == 13)
                            {
                                isSCDApplied = IsSCDAppliedOnDest(OrdersList.get());
                                isPWDApplied = IsPWDAppliedOnDest(OrdersList.get());
                            }
                            if(isSCDApplied )
                            {
                                MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                            }
                            else if(isPWDApplied)
                            {
                                MessageBox("Order with PWD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                            }
                            else
                            {
                                source_key = *itTab;
                                if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), dest_key))
                                {
                                  TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), dest_key ,TDeviceRealTerminal::Instance().User.ContactKey,source_key);
                                  TDBOrder::SetOrderIdentificationNumberForTab(DBTransaction, dest_key ,identificationNumber);
                                  if (cmClientManager->ChefMateEnabled())
                                  {
                                      CollectDataForChefmateTransfer(dest_key, OrdersList.get(), lbDisplayTransferfrom);
                                  }
                                }
                                else
                                {
                                  IsBalAvailable = false;
                                  MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                                }
                            }
                         }
                      }

                   }
                   else
                   {
                       source_key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                       SelectedTabs.insert(source_key);
                       TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);

                        if(TabType == 13)
                        {
                            isSCDApplied = IsSCDAppliedOnDest(OrdersList.get());
                            isPWDApplied = IsPWDAppliedOnDest(OrdersList.get());
                        }
                        if(isSCDApplied )
                        {
                            MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                        }
                        else if(isPWDApplied)
                        {
                            MessageBox("Order with PWD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                        }
                        else
                        {
                           if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), dest_key))
                           {
                              TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), dest_key ,TDeviceRealTerminal::Instance().User.ContactKey,source_key);
                              TDBOrder::SetOrderIdentificationNumberForTab(DBTransaction, dest_key ,identificationNumber);
                              if(cmClientManager->ChefMateEnabled())
                                {
                                    CollectDataForChefmateTransfer(dest_key, OrdersList.get(), lbDisplayTransferfrom);
                                }
                           }
                           else
                           {
                              MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                           }
                        }
                   }
			}break;
		 case eTables:
			{
                  if(CurrentSourceDisplayMode == eTables)
                  {
                       int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTable(DBTransaction,CurrentDestTable);
                       std::set <__int64> SelectedTabs;
                       TDBTables::GetTabKeys(DBTransaction, CurrentSourceTable, SelectedTabs);
                       for (std::set <__int64> ::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end(); advance(itTab, 1))
                       {
                          if (TDBTab::GetTabExists(DBTransaction, *itTab))
                          {
                             TTableSeat Info;
                             TDBTables::GetTableSeat(DBTransaction, *itTab, &Info);
                             int SeatKey = TDBTables::GetOrCreateSeat(DBTransaction, CurrentDestTable, Info.SeatNo);
                             int DestTabKey = TDBTab::GetOrCreateTab(DBTransaction, TDBTables::GetTabKey(DBTransaction, SeatKey));
                             TDBTab::SetTabType(DBTransaction, DestTabKey, TabTableSeat);
                             TDBTables::SetSeatTab(DBTransaction, SeatKey, DestTabKey);
                             bool Proceed = false;
                             if (!TDBTab::GetIsEmpty(DBTransaction, DestTabKey))
                             {
                                if
                                    (MessageBox
                                   ("Destination Is not empty!" "\rAre you sure you wish to merge orders with this " + TGlobalSettings::Instance().SeatLabel + "?",
                                      "Confirmation", MB_YESNO + MB_DEFBUTTON1 + MB_ICONWARNING) == IDYES)
                                {
                                   Proceed = true;
                                   isTotalTableTransfered = false;
                                }
                                else
                                {
                                  isTotalTableTransfered = true;
                                }
                             }
                             else
                             {
                                Proceed = true;
                             }

                             if (Proceed)
                             {

                                std::auto_ptr <TList> OrdersList(new TList);
                                std::set <__int64> SelectedTabs;
                                SelectedTabs.insert(*itTab);
                                TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);

                                if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), DestTabKey))
                                {
                                   TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), DestTabKey,TDeviceRealTerminal::Instance().User.ContactKey,*itTab);
                                   TDBOrder::SetOrderIdentificationNumberForTable(DBTransaction,CurrentDestTable,identificationNumber);
                                   if (cmClientManager->ChefMateEnabled())
                                   {
                                        CollectDataForChefmateTransfer(0, OrdersList.get(), lbDisplayTransferfrom);
                                   }
                                }
                                else
                                {
                                   MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                                }

                                //Shift table patron count to new table...
                                TDBTables::ShiftPatronCountToNewTable(DBTransaction, CurrentSourceTable, CurrentDestTable);
                             }
                          }
                       }

                     if(TGlobalSettings::Instance().PrintNoticeOnTransfer)
                       {
                            if(MessageBox("Do you want to inform the chef?","Confirmation", MB_YESNO  + MB_ICONWARNING) == IDYES)
                            {
                                std::auto_ptr <TReqPrintJob> TransferRequest(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
                                std::auto_ptr<TKitchen> Kitchen(new TKitchen());
                                Kitchen->Initialise(DBTransaction);
                                TransferComplete->UserName =  TDeviceRealTerminal::Instance().User.Name;
                                TransferComplete->TableTransferedFrom =  TDBTables::GetTableName(DBTransaction,CurrentSourceTable);
                                TransferComplete->TableTransferedTo =  TDBTables::GetTableName(DBTransaction,CurrentDestTable);
                                Kitchen->GetPrintouts(DBTransaction, TransferComplete, TransferRequest.get());
                                TransferRequest->Printouts->Print(devPC);
                            }
                       }
                   }
                 else
                   {
                      std::auto_ptr <TList> OrdersList(new TList);
                      std::set <__int64> SelectedTabs;
                      int source_key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                      int dest_tabkey = 1;
                      if(ClipTabSelected  )
                      {
                           //create new seat in case of clipp transfer
                           TMMTabType tabtype = TDBTab::GetTabType(DBTransaction, source_key);
                            if(tabtype == TabClipp)
                            {
                                dest_tabkey = TDBTab::GetMaxAvailableSeatKey(DBTransaction, CurrentDestTable);
                            }
                      }

                      int SeatKey = TDBTables::GetOrCreateSeat(DBTransaction, CurrentDestTable, dest_tabkey);
                      int DestTabKey = TDBTab::GetOrCreateTab(DBTransaction, TDBTables::GetTabKey(DBTransaction, SeatKey));
                      TDBTab::SetTabType(DBTransaction, DestTabKey, TabTableSeat);
                      TDBTables::SetSeatTab(DBTransaction, SeatKey, DestTabKey);
                      SelectedTabs.insert(source_key);
                      TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);

                      int identificationNumber = TDBOrder::GetOrderIdentificationNumberForTable(DBTransaction,CurrentDestTable);
                      UpdateTablePartyName(DBTransaction, DestTabKey, source_key, true);
                      if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), DestTabKey))
                      {
                         if(ClipTabSelected && linkCliptabToTable )
                         {
                           DoCliptabLinking(DBTransaction,source_key,DestTabKey);
                           NewClipTabKey = DestTabKey;
                         }

                         TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), DestTabKey,TDeviceRealTerminal::Instance().User.ContactKey,source_key);
                         TDBOrder::SetOrderIdentificationNumberForTable(DBTransaction,CurrentDestTable,identificationNumber);
                         if (cmClientManager->ChefMateEnabled())
                           {
                              CollectDataForChefmateTransfer(DestTabKey, OrdersList.get(), lbDisplayTransferfrom);
                           }
                      }
                      else
                      {
                           MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                      }

                   }

			}break;
		 case eInvoices:
			{
                  Currency TotalCost = 0;
                  Currency TotalCostExcl = 0;
                  Currency TotalCredit = 0;
				  std::auto_ptr <TList> OrdersList(new TList);
				  std::set <__int64> SelectedTabs;
                  int source_key = 0;

                  if( CurrentSourceDisplayMode== eTables)
                  {
                      TDBTables::GetTabKeys(DBTransaction, CurrentSourceTable, SelectedTabs);
                      TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);
                      //CollectTransferOrderKeyFromTab(OrdersList.get());
                      for (std::set <__int64> ::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end(); advance(itTab, 1))
                      {
                         TotalCost += TDBTab::GetTabTotal(DBTransaction, *itTab);
                         TotalCostExcl += TDBTab::GetTabTotalExcl(DBTransaction, *itTab);
                         source_key = *itTab;
                      }
                  }
                  else
                  {

                     if(lbDisplayTransferfrom->Count > 0)
                     {
                        source_key = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
                     }
                     SelectedTabs.insert(source_key);

                     TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, OrdersList.get(), SelectedTabs);

                     TotalCost = TDBTab::GetTabTotal(DBTransaction, source_key);
                     TotalCostExcl = TDBTab::GetTabTotalExcl(DBTransaction, source_key);
                  }
				  int OwnerKey = TempDestUserInfo.ContactKey;
				  int InvoiceTabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
				  int InvoiceKey = Invoice->CreateInvoice(DBTransaction, OwnerKey, TotalCostExcl, TotalCost, "");

				  TDBTab::SetTabToInvoice(DBTransaction, InvoiceTabKey, InvoiceKey);
				  TDBTab::SetTabOwner(DBTransaction, InvoiceTabKey, OwnerKey, TabInvoice);

				  if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), InvoiceTabKey))
				  {
					 TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), InvoiceTabKey,TDeviceRealTerminal::Instance().User.ContactKey,source_key);
                      if (cmClientManager->ChefMateEnabled())
                       {
                         int invoiceNo=TDBTab::GetInvoiceNo(DBTransaction, InvoiceKey);
                         CollectDataForChefmateTransfer(invoiceNo, OrdersList.get(), lbDisplayTransferfrom);
                       }
        		  }
				  else
				  {
					 MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
				  }

				  Invoice->SetOrdersToInvoiceFormat(DBTransaction, InvoiceKey, InvoiceTabKey, TempDestUserInfo.Name);

				  // Credit May have been transfered to this TAB.
				  TotalCredit = TDBTab::GetTabCredit(DBTransaction, InvoiceTabKey);

				  TPaymentTransaction Transaction(DBTransaction);

				  SelectedTabs.clear();
				  SelectedTabs.insert(InvoiceTabKey);

                  TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, Transaction.Orders, SelectedTabs);

				  Transaction.InvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, InvoiceKey);

				  Transaction.Money.CreditAvailable = TotalCredit;
				  Transaction.Money.Recalc(Transaction);

			   if( TempDestUserInfo.HasHotelNumber() )
				  Transaction.Customer = GetAndSaveCustomer( DBTransaction, InvoiceKey );

				  // Print Invoice.
				  std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
				  TempReceipt->Transaction = &Transaction;
				  TempReceipt->JobType = pjReceiptInvoice;
				  TempReceipt->SignReceipt = true;
				  TempReceipt->SenderType = devPC;
				  TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
				  TempReceipt->ExtraInfo->Add("Name: " + TempDestUserInfo.Name);
				  TempReceipt->ExtraInfo->Add("Member No. " + TempDestUserInfo.MembershipNumber);
				  TempReceipt->PaymentType = ptFinal;

				  Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
				  TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
				  if (TGlobalSettings::Instance().PrintSignatureReceiptsTwice)
				  {
					 TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
				  }
			}
		 }
	  }
	  catch(Exception & E)
	  {
		 MessageBox(E.Message, "Error", MB_OK + MB_ICONWARNING);
		 TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  }
      delete TransferComplete;
}

void TfrmTransfer::CollectDataForChefmateTransfer(int dest_key, TList *OrdersList, TListBox *listBox, bool isReverseTransfer)
{
    AnsiString CurrentSource = "";
    AnsiString CurrentDestination = "";

    if(!isReverseTransfer)
    {
        if(CurrentSourceDisplayMode == eTables)
        {
           CurrentSource = TDBTables::GetTableName(*DBTransaction,CurrentSourceTable);
        }
        else
        {
           if(listBox->Count > 0)
           {
              CurrentSource = listBox->Items->Strings[0];
           }
        }
        if(CurrentDestDisplayMode == eTables)
        {
           CurrentDestination = TDBTables::GetTableName(*DBTransaction,CurrentDestTable);
        }
        else if (CurrentDestDisplayMode == eInvoices)
        {
          CurrentDestination =  dest_key;

        }
        else
        {
           CurrentDestination = TDBTab::GetTabName(*DBTransaction, dest_key);
        }
    }
   else
    {
       if(CurrentDestDisplayMode == eTables)
       {
          CurrentSource = TDBTables::GetTableName(*DBTransaction,CurrentDestTable);
       }
       else if(CurrentDestDisplayMode == eInvoices)
       {
          CurrentSource = TDBTab::GetInvoiceNoFromTabKey(*DBTransaction,GetTabKeyFromListBox(lbDisplayTransferto, 0));
       }
       else
       {
           if(listBox->Count > 0)
           {
              CurrentSource = listBox->Items->Strings[0];
           }
       }
       if(CurrentSourceDisplayMode == eTables)
       {
          CurrentDestination = TDBTables::GetTableName(*DBTransaction,CurrentSourceTable);
       }
       else
       {
          CurrentDestination = TDBTab::GetTabName(*DBTransaction, dest_key);
       }

    }


    for(int i = 0; i < OrdersList->Count; i++)
     {
           bool itemAdded = false;
           TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
           for (std::vector<TTransferChefMateItem>::iterator item_check = ChefMateOrderList.begin(); item_check != ChefMateOrderList.end(); ++item_check)
           {
               //remove item if previously transferred
              

               if(item_check->CurrentDest == CurrentSource &&  item_check->CurrentSource == CurrentDestination && item_check->TimeKey == Order->TimeKey)
               {
                  bool found = false;
                  std::vector<TItemComplete *>::iterator it;
                  for ( it = item_check->Items.begin(); it != item_check->Items.end(); ++it)
                   {
                      if(Order->Item_ID == (*it)->Item_ID && Order->Size == (*it)->Size)
                      {
                         found = true;
                         break;
                      }
                   }

                   if(found)
                    {
                      if(Order->GetQty() == (*it)->GetQty())
                      {
                        item_check->Items.erase(it);
                      }
                      else
                      {
                        double remainingQuantity = (*it)->GetQty() - Order->GetQty();
                        (*it)->SetQty(remainingQuantity);
                      }
                      itemAdded = true;
                    }
               }
               else if(item_check->CurrentDest == CurrentDestination &&  item_check->CurrentSource == CurrentSource && item_check->TimeKey == Order->TimeKey)
               {
                     TItemComplete *NewOrder = new TItemComplete();
                     Order->Assign(NewOrder);
                     item_check->Items.push_back(NewOrder);
                     itemAdded = true;
               }
           }

           if(!itemAdded)
           {
             TTransferChefMateItem chefmateItem;
             chefmateItem.CurrentSource =  CurrentSource;
             chefmateItem.CurrentDest =  CurrentDestination;
             chefmateItem.TimeKey = Order->TimeKey;
             chefmateItem.Username = TDeviceRealTerminal::Instance().User.Name;
             TItemComplete *NewOrder = new TItemComplete();
             Order->Assign(NewOrder);
             chefmateItem.Items.push_back(NewOrder);
             ChefMateOrderList.push_back(chefmateItem);
           }
     }
}

void  TfrmTransfer::transferOrderToChefMate(TList* orderList, TTransferComplete *Transfer)
{

    try
    {
        for (int i = 0; i < orderList->Count; i++)
        {
            TItemComplete *Order = (TItemComplete*)orderList->Items[i];
            Order->TabContainerName = Transfer->TableTransferedTo;
            if (Order->OriginalItemKey != NULL)
            {
                Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);
            }
        }
        // This method is called at the time of order tranfer from one table to another
        CMC_ERROR error = cmClientManager->SendTransferOrder( orderList,Transfer->TableTransferedFrom );
        if( error == CMC_ERROR_FAILED )
        {
            MessageBox( "Menumate failed to send an complete order to Chefmate",
                        "Chefmate",
                        MB_OK + MB_ICONWARNING);
        }

    }
   catch(Exception & E)
   {
	  MessageBox(E.Message, "Error", MB_OK + MB_ICONWARNING);
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
   }

}

void TfrmTransfer::TransferOrderListtoChefmate()
{
 try
  {
     if (cmClientManager->ChefMateEnabled())
     {
       for(std::vector<TTransferChefMateItem>::iterator it = ChefMateOrderList.begin(); it != ChefMateOrderList.end(); ++it)
       {
          for(std::vector<TItemComplete*>::iterator item = it->Items.begin(); item != it->Items.end(); ++item)
          {

               TItemComplete *Order = new TItemComplete();
               (*item)->Assign(Order);
               if(CHEFMATE_ITEM_GROUP_MAP[Order->TimeKey].Items.size() == 0)
               {
                  CHEFMATE_ITEM_GROUP_MAP[Order->TimeKey].Username = it->Username;
                  CHEFMATE_ITEM_GROUP_MAP[Order->TimeKey].CurrentSource = it->CurrentSource;
                  CHEFMATE_ITEM_GROUP_MAP[Order->TimeKey].CurrentDest = it->CurrentDest;
                  CHEFMATE_ITEM_GROUP_MAP[Order->TimeKey].Items.push_back(Order);
               }
               else
               {
                  CHEFMATE_ITEM_GROUP_MAP[Order->TimeKey].Items.push_back(Order);
               }
               delete (*item);
           }

           for(std::map<int,TTransferChefMateItem>::iterator kn = CHEFMATE_ITEM_GROUP_MAP.begin(); kn != CHEFMATE_ITEM_GROUP_MAP.end(); ++kn)
           {
              TTransferComplete *TransferComplete = new TTransferComplete();
                TList *FinalOrder = new TList();
                TransferComplete->UserName =  kn->second.Username;
                TransferComplete->TableTransferedFrom =  kn->second.CurrentSource;
                TransferComplete->TableTransferedTo =  kn->second.CurrentDest;
                for(std::vector<TItemComplete *>::iterator item1 = kn->second.Items.begin(); item1 != kn->second.Items.end(); ++item1)
                {
                    TItemComplete *Order = new TItemComplete();
                    (*item1)->Assign(Order);
                    FinalOrder->Add(Order);
                    delete (*item1);
                }
                transferOrderToChefMate(FinalOrder, TransferComplete);
                while (FinalOrder->Count != 0)
                {
                    delete(TItemComplete*)FinalOrder->Items[0];
                    FinalOrder->Delete(0);
                }
                delete TransferComplete;
                Sleep(1000);
           }
           CHEFMATE_ITEM_GROUP_MAP.clear();
        }
        ChefMateOrderList.clear();
      }
    }
    catch(Exception & E)
    {
     MessageBox(E.Message, "Error", MB_OK + MB_ICONWARNING);
     TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}

void TfrmTransfer::ShowProcessingBar()
{
   std::auto_ptr <TfrmProcessing> (Processing)(TfrmProcessing::Create <TfrmProcessing> (this));
   Processing->Message = "Transfering Items...";
   Processing->Show();
}
//-----------------------------------------------------------------------

AnsiString TfrmTransfer::GetItemQtyWithName(double qty, AnsiString itemSize, UnicodeString itemName)
{
   AnsiString returnValue = "";
   if (SameText(itemSize,"DEFAULT"))
   {
      returnValue = FloatToStr(qty) + "x " + itemName ;
   }
   else
   {
      returnValue = FloatToStr(qty) + "x "+ itemSize+ " " + itemName ;
   }
   return returnValue;
}

int TfrmTransfer::GetSeatNumber(Database::TDBTransaction &DBTransaction, long source_key, bool isTabSelected, int table_no)
{
    int sourceSeatNumber = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "SELECT s.SEATNO FROM TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
            "INNER JOIN TAB a ON s.TAB_KEY = a.TAB_KEY "
            "inner join ORDERS on ORDERS.TAB_KEY = a.TAB_KEY "
            "where "
            "t.TABLE_NUMBER = :TABLE_NUMBER and " ;
            if(isTabSelected)
            {
               IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
               "ORDERS.TAB_KEY = :TAB_KEY group by s.SEATNO" ;
               IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = source_key;
            }
            else
            {
               IBInternalQuery->SQL->Text = IBInternalQuery->SQL->Text +
               "ORDERS.ORDER_KEY = :ORDER_KEY group by s.SEATNO " ;
               IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = source_key;
            }
        IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = table_no;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           sourceSeatNumber = IBInternalQuery->FieldByName("SEATNO")->AsInteger;
        }
        IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return sourceSeatNumber;
}

bool TfrmTransfer::CheckSideIsSelected(TListBox *listBox, int i)
{
    bool isSideSelected = false;
    if(listBox->Count > 0)
    {
       TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[i];
       isSideSelected = getDestId->IsSideItem;
    }
    return isSideSelected;
}

int TfrmTransfer::GetOrderKey(Database::TDBTransaction &DBTransaction, int tab_key, UnicodeString itemName)
{
    int Orderkey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "select First 1 a.Order_key from ORDERS a where a.TAB_KEY = :TAB_KEY and a.ITEM_NAME = :ITEM_NAME " ;
        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tab_key;
        IBInternalQuery->ParamByName("ITEM_NAME")->AsString = itemName;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           Orderkey = IBInternalQuery->FieldByName("Order_key")->AsInteger;
        }
        IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return Orderkey;
}

int TfrmTransfer::GetOrderKeyFromSavedList(Database::TDBTransaction &DBTransaction, int transfer_qty, TListBox *listBox, int i, bool isLongPressed)
{
    std::vector<int> OrderKeyList;
    std::vector<double> OrderQty;
    int source_key = 0;
    double itemQty = 0;

    int prev_source_key = 0;
    int source_tab_key = 0;

    std::vector<int>::iterator ab;
    std::vector<double>::iterator it;

    if(listBox->Count > 0)
    {
       TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[i];
       OrderKeyList = getDestId->SaveOrderKey;
       OrderQty = getDestId->SaveItemQty;
       source_tab_key = getDestId->Tabkey;

       ab = OrderKeyList.begin();
       source_key = *ab;
       it = OrderQty.begin();
       itemQty = *it;

       if(!isLongPressed)
       {
           if(itemQty <= transfer_qty)
           {
              prev_source_key = source_key;
              return source_key;
           }
           else
           {
              prev_source_key = source_key;
              source_key = SplitOrder(DBTransaction, source_key, transfer_qty);
           }
       }
       else
       {
          while(transfer_qty > 0)
          {
              if(itemQty <= transfer_qty)
              {
                  GetSavedOrderKey.push_back(source_key);
                  transfer_qty -= itemQty;
                  if(transfer_qty > 0)
                  {
                        ++ab;
                        ++it;
                        source_key = *ab;
                        itemQty = *it;
                  }
              }
              else
              {
                 source_key = SplitOrder(DBTransaction, source_key, transfer_qty);
                 GetSavedOrderKey.push_back(source_key);
                 transfer_qty -= itemQty;
                 if(transfer_qty > 0)
                 {
                     ++ab;
                     ++it;
                     source_key = *ab;
                     itemQty = *it;
                 }
              }
              if(transfer_qty <= 0)
              {
                 break;
              }
          }
       }
    }
    return source_key;
}


__int64 TfrmTransfer::SplitOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey, int WaysToSplit)
{

	std::auto_ptr<TItemComplete> Order(new TItemComplete());
	Order->OrderKey 	   = OrderKey;
	TDBOrder::GetOrderIncludingSidesFromOrderKey(DBTransaction,Order.get(), true);
	Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);

	std::auto_ptr<TItemComplete> OrderImage(new TItemComplete());
	OrderImage->OrderKey 		= OrderKey;
	TDBOrder::GetOrderIncludingSidesFromOrderKey(DBTransaction,OrderImage.get(), true);
	OrderImage->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(OrderImage->OriginalItemKey);

	Currency tempPatronCount = Currency(Order->PatronCount()) / WaysToSplit;
	int splitItemPatronCount = tempPatronCount + (tempPatronCount % 1 != 0);
	int itemPatronCount 	 = Order->PatronCount();

	bool SplitAsMuchAsPossible = false;

    SplitAsMuchAsPossible = !DeductOrder(Order.get(), OrderImage.get(), WaysToSplit);
    if (!SplitAsMuchAsPossible)
    {
        OrderImage->TransNo	= TDBOrder::GetNextTransNumber(DBTransaction);

        OrderImage->PatronCount(0, false);
        if (itemPatronCount > splitItemPatronCount) {
            OrderImage->PatronCount(splitItemPatronCount, false);
            itemPatronCount -= splitItemPatronCount;
        }
        Order->PatronCount(itemPatronCount, false);

        Order->RunBillCalculator();
        OrderImage->RunBillCalculator();
        for (int i = 0; i < OrderImage->SubOrders->Count; i++)
        {
            TItemCompleteSub *SubOrder = OrderImage->SubOrders->SubOrderGet(i);
            if(SubOrder)
            {
               SubOrder->RunBillCalculator(); //changes here...
            }
        }

        TDBOrder::SetOrder(DBTransaction, OrderImage.get());
        TDBOrder::UpdateOrder(DBTransaction, Order.get());
        TDBSecurity::ProcessSecurity(DBTransaction,OrderImage->Security, true);
        for (int i = 0; i < OrderImage->SubOrders->Count; i++)
        {
            TItemCompleteSub *SubOrder = OrderImage->SubOrders->SubOrderGet(i);
            if(SubOrder)
            {
                TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security, true);
            }
        }
    }
	return OrderImage->OrderKey;
}

bool TfrmTransfer::DeductOrder(
TItemMinorComplete * const Order1,
TItemMinorComplete * const Order2,
int cnt)
{
	int original_patron_count = Order1->PatronCount();

	/* Handy for debugging. */
	const Currency order_1_qty = Order1->GetQty();
	const Currency order_2_qty = Order2->GetQty();
	const Currency new_quantity = order_1_qty - cnt;

	Order1->SetQtyCustom(new_quantity);
    Order2->SetQtyCustom(cnt);
	Order1->Cost 			-= Order2->Cost;
	Order1->Redeemed 		-= Order2->Redeemed;

    return true;
}

UnicodeString TfrmTransfer::GetItemName(TListBox *listBox, int i, bool isTabSelected)
{
    UnicodeString itemOrTabName = "";
    if(listBox->Count > 0)
    {
       TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[i];
       if(!isTabSelected)
       {
         itemOrTabName = getDestId->ItemName;
       }
       else
       {
          itemOrTabName = getDestId->TabName;
       }
    }
    return itemOrTabName;
}

double TfrmTransfer::GetItemQty(TListBox *listBox, int i, bool isTabSelected)
{
    double itemQty = 0;
    if(listBox->Count > 0)
    {
       TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[i];
       if(!isTabSelected)
       {
         itemQty = getDestId->Qty;
       }
    }
    return itemQty;
}

bool TfrmTransfer::CheckNameIsNullOrNot(Database::TDBTransaction &DBTransaction, int tableNumber, int key, int seatKey)
{
     AnsiString SeatName = "";
     bool retVal = false;
     int TabKey = TDBTables::GetTabKey(DBTransaction, tableNumber, key);
     if (TabKey != 0)
     {
        SeatName = TDBTab::GetTabName(DBTransaction, TabKey);
     }
     else
     {
        TabKey = Gettabkey(tableNumber, seatKey);
        if(TabKey != 0)
        {
           SeatName = TabName(TabKey);
        }
     }
     if(SeatName.Length() > 0)
     {
         char* temp_char = new char[SeatName.Length()+1];
         strcpy(temp_char, SeatName.c_str());
         for(int i = 0; i < SeatName.Length(); i++)
         {
             if(temp_char[i] == check_space)
             {
                retVal = true;
             }
             else
             {
                retVal = false;
                break;
             }
         }
     }
     return retVal;
}

int TfrmTransfer::Gettabkey(int table_no, int seat_key)
{
    int tabkey = 0;
	try
	{
        Database::TDBTransaction SelectDBTransaction(TDeviceRealTerminal::Instance().DBControl);
        SelectDBTransaction.StartTransaction();
		TIBSQL *IBInternalQuery = SelectDBTransaction.Query(SelectDBTransaction.AddQuery());
		IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "SELECT a.TAB_KEY FROM TABLES t INNER JOIN SEAT s ON t.TABLE_KEY = s.TABLE_KEY "
            "INNER JOIN TAB a ON s.TAB_KEY = a.TAB_KEY "
            "WHERE "
            "t.TABLE_NUMBER = :TABLE_NUMBER and "
            "s.SEAT_KEY = :SEAT_KEY" ;
        IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = table_no;
        IBInternalQuery->ParamByName("SEAT_KEY")->AsInteger = seat_key;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           tabkey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
        }
        IBInternalQuery->Close();
        SelectDBTransaction.Commit();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return tabkey;
}

UnicodeString TfrmTransfer::TabName(int TabKey)
{
	UnicodeString RetVal = "";
	try
	{
        Database::TDBTransaction SelectDBTransaction(TDeviceRealTerminal::Instance().DBControl);
        SelectDBTransaction.StartTransaction();
		TIBSQL *IBInternalQuery = SelectDBTransaction.Query(SelectDBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
			"TAB.TAB_NAME "
		"FROM "
			"TAB "
		"WHERE "
			"TAB.TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TAB_NAME")->AsString;
		}
        SelectDBTransaction.Commit();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TfrmTransfer:: SendTabDetails(AnsiString source, AnsiString dest, int sourceItemCount, int destItemCount )
{

    if (ClipTabSelected)
    {
        std::set <int> ClipTabToBeUpdatedKeys;
        std::set <int> ClipTabToBeClosedKeys;
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();



         for(int key = 0; key < lbDisplayTransferfrom->Count; key++)
            {
                 int sourcekey = GetTabKeyFromListBox(lbDisplayTransferfrom, key);

                   TMMTabType tabtype = TDBTab::GetTabType(DBTransaction, sourcekey);
                    if(tabtype == TabClipp)
                    {
                         if(CloseClipTab )//!linkCliptabToTable && CloseClipTab)
                         {
                            ClipTabToBeClosedKeys.insert(sourcekey);
                         }
                         else
                         {
                            ClipTabToBeUpdatedKeys.insert(sourcekey);
                         }
                   }

              }

        for(int key = 0; key < lbDisplayTransferto->Count; key++)
            {
                 int sourcekey = GetTabKeyFromListBox(lbDisplayTransferto, key);

                   TMMTabType tabtype = TDBTab::GetTabType(DBTransaction, sourcekey);
                    if(tabtype == TabClipp)
                    {
                         if(CloseClipTab)//!linkCliptabToTable &&CloseClipTab)
                         {
                            ClipTabToBeClosedKeys.insert(sourcekey);
                         }
                         else
                         {
                            ClipTabToBeUpdatedKeys.insert(sourcekey);
                         }
                   }

              }

         DBTransaction.Commit();

        int size1= ClipTabToBeClosedKeys.size();
        std::set<int>::iterator key;
        for (key = ClipTabToBeClosedKeys.begin(); key != ClipTabToBeClosedKeys.end(); ++key)
        {
           int i= *key;
           CloseClippTab(*key);
        }

        int size= ClipTabToBeUpdatedKeys.size();
        std::set<int>::iterator updateKey;
        for (updateKey = ClipTabToBeUpdatedKeys.begin(); updateKey != ClipTabToBeUpdatedKeys.end(); ++updateKey)
        {
           int i= *updateKey;
           SendClippTabDetails(*updateKey);
        }


    }
    else
    {
            int sourceTabKey = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
            int destTabKey =   GetTabKeyFromListBox(lbDisplayTransferto, 0);

            if((source == "Clipp Tabs") && (sourceItemCount > 1) && (IsBalAvailable))
            {
                if(!isTabOrItemSelected)
                {
                     CloseClippTab(sourceTabKey);
                }
                else
                {
                    SendClippTabDetails(sourceTabKey);
                }
            }

            else if((source == "Clipp Tabs") &&  (sourceItemCount == 1)&& (IsBalAvailable))
            {
                 CloseClippTab(sourceTabKey);
            }

            if((dest == "Clipp Tabs") && (destItemCount > 1)&& (IsBalAvailable))
            {
                SendClippTabDetails(destTabKey);
            }
            else if((dest == "Clipp Tabs") &&  (destItemCount == 1) && (IsBalAvailable))
            {
                if(!isTabOrItemSelected && sourceItemCount > 1)
                {
                    SendClippTabDetails(destTabKey);
                }
                else
                {
                    CloseClippTab(destTabKey);
                }
            }


    }



}

//-----------------------------------------------------------------------

void TfrmTransfer::SendClippTabDetails(int clippTabKey)
{
   TManagerClippIntegration* updateClippTab = TManagerClippIntegration::Instance();
   updateClippTab->SendTabDetails(clippTabKey);
}

//-----------------------------------------------------------------------

void TfrmTransfer::CloseClippTab(int clippTabKey)
{
   TManagerClippIntegration* closeClippTab = TManagerClippIntegration::Instance();
   closeClippTab->CloseTab(clippTabKey);
}

bool TfrmTransfer::IsSourceDestinationSame()
{
   bool isSameTab = false;
   int sourceTabKey = GetTabKeyFromListBox(lbDisplayTransferfrom, 0);
   int destTabKey =   GetTabKeyFromListBox(lbDisplayTransferto, 0);
   if(sourceTabKey == destTabKey)
   {
      isSameTab  = true;
   }
   return isSameTab;
}

bool TfrmTransfer::CheckSecurityEvent(Database::TDBTransaction &DBTransaction, int sec_ref)
{
    bool retVal = true;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
            "select a.SECURITY_EVENT, a.SECURITY_REF FROM SECURITY a where a.SECURITY_REF = :SECURITY_REF and a.SECURITY_EVENT = 'Price Adjust' " ;
        IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = sec_ref;
		IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount)
        {
           retVal = false;
        }
        IBInternalQuery->Close();
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
   return retVal;
}

//----------------------------------------------------------------------------------------------------
bool TfrmTransfer::IsSCDAppliedOnDest(TList* Orders)
{
    bool isSCDApplied = false;
    TSCDPWDChecker SCDChecker;

     for(int i = 0 ; i < Orders->Count; i++)
    {
        TItemMinorComplete *order = reinterpret_cast<TItemMinorComplete *>(Orders->Items[i]);
        for(std::vector<TDiscount>::iterator it = order->Discounts.begin(); it != order->Discounts.end(); ++it)
        {
            TDiscount CurrentDiscount;
            CurrentDiscount.DiscountKey =  it->DiscountKey;
             ManagerDiscount->GetDiscount(*DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

              if(SCDChecker.SeniorCitizensCheck(CurrentDiscount, Orders, true))
              {
                  isSCDApplied = true;
              }
        }
    }
    return isSCDApplied;
}
//------------------------------------------------------------------------------------------------------
bool TfrmTransfer::IsPWDAppliedOnDest(TList* Orders)
{
    bool IsPWDApplied = false;
    TSCDPWDChecker SCDChecker;

     for(int i = 0 ; i < Orders->Count; i++)
    {
        TItemMinorComplete *order = reinterpret_cast<TItemMinorComplete *>(Orders->Items[i]);
        for(std::vector<TDiscount>::iterator it = order->Discounts.begin(); it != order->Discounts.end(); ++it)
        {
            TDiscount CurrentDiscount;
            CurrentDiscount.DiscountKey =  it->DiscountKey;
             ManagerDiscount->GetDiscount(*DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

              if(SCDChecker.PWDCheck(CurrentDiscount, Orders, true))
              {
                  IsPWDApplied = true;
              }
        }
    }
    return IsPWDApplied;
}
//------------------------------------------------------------------------------------------------------
void TfrmTransfer::SaveClipItemsInStructure(Database::TDBTransaction &DBTransaction)
{

    TListBox *destinationListBox;
    int orderKey;
    double  qty;


   if(ClipPresentInFromPanel )
   {
     destinationListBox =  lbDisplayTransferfrom;
   }
   else
   {
      destinationListBox =  lbDisplayTransferto;
   }

  ClipItemsContainer.clear();

 for(int destkey=0;destkey< destinationListBox->Count; destkey++)
   {
        TTabColour *getDestId = (TTabColour *)destinationListBox->Items->Objects[destkey];

        if( !getDestId->IsTabKey )
        {
           orderKey =getDestId->Orderkey;
           qty=  getDestId->Qty;
          ClipItemsContainer.insert( std::pair<int,double>(orderKey, qty) );
        }

    }
 }

 ///-----------------------------------------------------------------------------------------------

 void  TfrmTransfer::DoCliptabLinking(Database::TDBTransaction &DBTransaction,int source_key,int DestTabKey)
 {
     UpdateTabNameAndKey(DBTransaction, source_key, DestTabKey);
     UpdatePanelSourceKey(source_key , DestTabKey);
 }
 ///--------------------------------------------------------------------------------------------

void TfrmTransfer::UpdateTabNameAndKey(Database::TDBTransaction &DBTransaction, int source_key, int dest_key)
{
    //Get clipp customer Name
    UnicodeString customerName = TDBClippTab::GetCustomerName(DBTransaction, source_key);

    //update clipp Tab by table key to which it was transferred
    TDBClippTab::UpdateClippTabTabKey(DBTransaction, source_key, dest_key);

    UnicodeString tabName =  "Clipp-" + customerName + "-" + dest_key;

    //update Tab Name according to new tab key
    TDBTab::UpdateTabName(DBTransaction, dest_key, tabName, true);

     //update Tab type according to new tab key
  //  TDBTab::UpdateTabType(DBTransaction, dest_key, TabClipp);


    //Update Table's guest limit
    TDBTab::UpdateGuestLimit(DBTransaction, source_key, dest_key);
}

///------------------------------------------------------------------------------------
bool TfrmTransfer::CheckLinkedTabValidations(Database::TDBTransaction &DBTransaction,bool checkClipTabLimit)
{
   if( checkClipTabLimit)
   {
         if(lbDisplayTransferfrom->Count>1)
          {
           // for lbDisplayTransferfrom panel
             for(int key = 0; key < lbDisplayTransferfrom->Count; key++)
               {
                  int sourcekey = GetTabKeyFromListBox(lbDisplayTransferfrom, key);
                  if(CheckIfTransferringLinkedTabItems(DBTransaction,sourcekey,true) )
                    {
                      	Currency CreditLimit = TDBTab::GetTabCreditLimit(DBTransaction,sourcekey);
                        if(TDBTab::GetTabBalance(DBTransaction,sourcekey) > CreditLimit)
                        {
                          MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                          return false;
                        }
                     }
               }
          }

        if( lbDisplayTransferto->Count >1)
        {
         // for lbDisplayTransferto panel
              for(int key = 0; key < lbDisplayTransferto->Count; key++)
               {
                  int sourcekey = GetTabKeyFromListBox(lbDisplayTransferto, key);
                  if(CheckIfTransferringLinkedTabItems(DBTransaction,sourcekey,true) )
                    {
                        Currency CreditLimit = TDBTab::GetTabCreditLimit(DBTransaction,sourcekey);
                        if(TDBTab::GetTabBalance(DBTransaction,sourcekey) > CreditLimit)
                        {
                          MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
                          return false;
                        }
                    }
               }
         }




   }
   else
   {
               int destTabKey;
               bool dilinkingCase;
               bool isOkPressed = true;

                   for(int key = 0; key < lbDisplayTransferfrom->Count; key++)
                   {
                         int sourcekey = GetTabKeyFromListBox(lbDisplayTransferfrom, key);
                         if(CheckIfTransferringLinkedTabItems(DBTransaction,sourcekey,true) )
                         {
                            isOkPressed = false;
                           if(lbDisplayTransferto->Count ==0)
                            {
                                MessageBox("Linked tab/table cannot be transferred to other locations.", "Error", MB_OK + MB_ICONERROR);
                                return false;
                            }
                           else
                           {
                               dilinkingCase=false;
                               for(int destkey=0;destkey< lbDisplayTransferto->Count; destkey++)
                                 {
                                      destTabKey =GetTabKeyFromListBox(lbDisplayTransferto, destkey);
                                      if(destTabKey== sourcekey)
                                         {
                                           dilinkingCase=true;
                                           break;
                                        }
                                 }

                                if(dilinkingCase)
                                {
                                  /// if linked clip tab or table is found in "from" then check the destination for dilinking case
                                  // if destination contain same tab key then it means it is a dilinking case
                                   DilinkingClipTab(DBTransaction,sourcekey, destTabKey);
                                   DBTransaction.Commit();
                                   ModalResult = mrOk;
                                }
                                else
                                {
                                   MessageBox("Linked tab/table cannot be transferred to other locations.", "Error", MB_OK + MB_ICONERROR);
                                 }
                               return false;
                             }
                         }
                   }
    }


  return true;
}
/////------------------------------------------------------------------------------------------------------
bool TfrmTransfer::CheckIfTransferringLinkedTabItems(Database::TDBTransaction &DBTransaction,int sourcekey,bool isTabSelected)
{

    TMMTabType sourceTabType = TDBTab::GetLinkedTableAndClipTab(DBTransaction, sourcekey,isTabSelected);
    if(sourceTabType == TabClipp )
    {
         return true;
    }

    return false;

}


/////----------------------------------------------------------------------------------
void TfrmTransfer::DilinkingClipTab(Database::TDBTransaction &DBTransaction,long sourceTabKey,long destTabKey)
 {
    TDBTab::UpdateTabType(DBTransaction, sourceTabKey,TabClipp);
    int tableKey = TDBTab::GetTableKeyFromSeat(DBTransaction, sourceTabKey);
    TDBTables::SetSeatTabToNull(DBTransaction, sourceTabKey);
    TDBOrder::UpdateOrderTableDlinkingWithClipp(DBTransaction, sourceTabKey);
    TDBTables::UpdateTablePartyName(DBTransaction, tableKey);
 }

///-----------------------------------------------------------------------------------------------

bool TfrmTransfer::CheckItemsInSeat(Database::TDBTransaction &DBTransaction, long tableNo,long seatNo)
{

    bool isItemPresent =false;

   	try
	{
           TIBSQL *SelectQuery = DBTransaction.Query(DBTransaction.AddQuery());
           SelectQuery->Close();
            SelectQuery->SQL->Text =
                         "SELECT a.SEATNO from ORDERS a where a.TABLE_NUMBER = :TABLE_NUMBER and a.SEATNO = :SEATNO " ;

            SelectQuery->ParamByName("TABLE_NUMBER")->AsInteger = tableNo;
            SelectQuery->ParamByName("SEATNO")->AsInteger = seatNo;
            SelectQuery->ExecQuery();

            if(!SelectQuery->Eof)
            {
                isItemPresent =true;
            }


	}
	catch(Exception &Err)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
    return isItemPresent;
}

///-------------------------------------------------------------------------

void TfrmTransfer::UpdateGuestNameByClippName(Database::TDBTransaction &DBTransaction, long DestTabKey, long SourceKey, bool isTabSelected)
{
    UnicodeString clipTabName = GetTabNameForTable(DBTransaction, SourceKey, isTabSelected);

    TDBOrder::UpdateTabNameInOrder(DBTransaction, SourceKey,clipTabName,isTabSelected);
    TDBTab::UpdateTabName(DBTransaction, DestTabKey,clipTabName,isTabSelected);
}
//---------------------------------------------------------------------------------

bool TfrmTransfer::CheckPartialTransferInClipCase(Database::TDBTransaction &DBTransaction)
{
    TListBox *destinationListBox;
    std::map<int ,double > DestinationItemsContainer;
    int orderKey;
    double qty;
    bool partialTransfer=false;

   if(ClipPresentInFromPanel)
   {
     destinationListBox =lbDisplayTransferto  ;
   }
   else
   {
      destinationListBox =  lbDisplayTransferfrom;
   }
  DestinationItemsContainer.clear();

   std::map<int, double>::iterator ptr = ClipItemsContainer.begin();
    for(; ptr != ClipItemsContainer.end(); advance(ptr,1))
    {
        bool itemFound=false;
        for(int destkey=0;destkey< destinationListBox->Count; destkey++)
          {
             TTabColour *getDestId = (TTabColour *)destinationListBox->Items->Objects[destkey];
             if( !getDestId->IsTabKey )
              {
                    if(getDestId->Orderkey == ptr->first && ptr->second == getDestId->Qty)
                     {
                        itemFound=true;
                        NewClipTabKey = getDestId->Tabkey;
                        break;
                     }
              }

           }

          if(!itemFound )
          {
           partialTransfer=true;
           break;
          }
    }



 /*for(int destkey=0;destkey< destinationListBox->Count; destkey++)
   {
        TTabColour *getDestId = (TTabColour *)destinationListBox->Items->Objects[destkey];

        if( !getDestId->IsTabKey )
        {
            orderKey =getDestId->Orderkey;
            qty=  getDestId->Qty;
            std::map<int, int>::iterator ptr = ClipItemsContainer.begin();
            for(; ptr != ClipItemsContainer.end(); advance(ptr,1))
            {
                if (ptr->first == orderKey && ptr->second == qty)
                {
                     NewClipTabKey = getDestId->Tabkey;
                     break;
                }
            }

             if(NewClipTabKey!=0)
             {
                break;
             }
        }
   }
          */

   ///  if NewClipTabKey contain value other than zero then transfer all the items having same tab key to DestinationItemsContainer vector
     // it will include clip tab items and also previous items that were present in before tranfer (Partial tranfer case)
   if(NewClipTabKey!=0 && !partialTransfer)
     {
        for(int destkey=0;destkey< destinationListBox->Count; destkey++)
           {
                TTabColour *getDestId = (TTabColour *)destinationListBox->Items->Objects[destkey];

                if( !getDestId->IsTabKey )
                {
                   orderKey =getDestId->Orderkey;
                   qty=  getDestId->Qty;
                   if(getDestId->Tabkey == NewClipTabKey )
                   {
                     DestinationItemsContainer.insert( std::pair<int,int>(orderKey, qty) );

                   }
                }
            }

     if( DestinationItemsContainer.size() == ClipItemsContainer.size() )
       {
         return true;
       }
   }

  return false;
}

///-------------------------------------------------------------------
void TfrmTransfer::UpdatePanelSourceKey(int PreviousValue , int NewValue)
{
    for(int key = 0; key < lbDisplayTransferfrom->Count; key++)
        {
            int sourcekey = GetTabKeyFromListBox(lbDisplayTransferfrom, key);
            if(sourcekey == PreviousValue)
            {
               ChangeItemTabKey(lbDisplayTransferfrom,key,NewValue);
            }
        }

    for(int key = 0; key < lbDisplayTransferto->Count; key++)
        {
            int sourcekey = GetTabKeyFromListBox(lbDisplayTransferto, key);
            if(sourcekey == PreviousValue)
            {
               ChangeItemTabKey(lbDisplayTransferto,key,NewValue);
            }

       }
 }

////---------------------------------------------------------------------------------

void TfrmTransfer::ChangeItemTabKey(TListBox *listBox, int pos ,int value)
{
    int key = 0;
    if(listBox->Count > 0)
    {
       TTabColour *getDestId = (TTabColour *)listBox->Items->Objects[pos];
        getDestId->Tabkey =value;
    }

}

////--------------------------------------------------------------------------------
void TfrmTransfer::CheckClipAndTableCondition(Database::TDBTransaction &DBTransaction)
{
    // if clip and table is selected in from and to panel ,vice versa
       // if clip and table is selected in from and to panel ,vice versa
          if( !ClipTabSelected )
          {
              if((CurrentSourceTabType == TabClipp  && CurrentDestDisplayMode == eTables) || (CurrentSourceDisplayMode == eTables  && CurrentDestTabType == TabClipp))
             {
                  ClipTabSelected=true;
                  SaveClipItemsInStructure(DBTransaction);
             }
          }
}


///------------------------------------------------------------------------------------------
bool TfrmTransfer::CheckIfClipTransferringToAnotherLinkedGuest(Database::TDBTransaction &DBTransaction,int source_key, int DestTabKey, bool isTabSelected )
{

  if(source_key != DestTabKey)
  {
    TMMTabType tabtype = TDBTab::GetTabType(DBTransaction, source_key,isTabSelected);
    if(tabtype == TabClipp)
     {

        //now check the destination if it is linked guest
        if(CheckIfTransferringLinkedTabItems(DBTransaction,DestTabKey,true) )
         {
           MessageBox("Clip Tab cannot be transferred to guest which is linked to another Clip Tab.", "Error", MB_OK + MB_ICONERROR);
           return true;
         }
     }
  }

 return false;
}





