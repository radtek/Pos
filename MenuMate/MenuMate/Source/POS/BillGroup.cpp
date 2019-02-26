// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "BillGroup.h"
#include "Printing.h"
#include "Comms.h"
#include "AddTab.h"
#include "MMTouchNumpad.h"
#include "PayType.h"
#include "GetMoney.h"
#include "SplitPayment.h"
#include "Rooms.h"
#include "DBTab.h"
#include "ContactStaff.h"
#include "Invoice.h"
#include "MMMessageBox.h"
#include "Transfer.h"
#include "DBOrder.h"
#include "TableManager.h"
#include "GUIScale.h"
#include "EnableFloorPlan.h"
#include "DropDownVar.h"
#include "DeviceRealTerminal.h"
#include "GlobalSettings.h"
#include "ManagerStock.h"
#include "ReceiptManager.h"
#include "ListSecurityRefContainer.h"
#include "DBWebUtil.h"
#include "GUIDiscount.h"
#include "Message.h"
#include "MMTouchKeyboard.h"
#include "VerticalSelect.h"
#include "SmartCardAPI.h"
#include "ThemeManager.h"
#include "ReqPrintJob.h"
#include <DateUtils.hpp>
#include "DBTables.h"
#include "ManagerDiscount.h"
#include "ManagerPatron.h"
#include "FreebieManager.h"
#include "SCDPWDChecker.h"
#include "SelectDish.h"
#include "MMInvoicePaymentSystem.h"
#include "ManagerDelayedPayment.h"
#include "SelectSaveOption.h"
#include "MallExportOtherDetailsUpdate.h"
#include "ManagerLoyaltyVoucher.h"
#include "PMSHelper.h"
#include "SCDPatronUtility.h"
#include "SaveLogs.h"
#include "DBOnlineOrdeing.h"
#include "ManagerSyndCode.h"
#include "TransactionHelper.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma resource "*.dfm"


#define NUMBER_OF_SEATS_IN_VIEW 8

#define CONTAINER_LIST_FUNC_COLUMN 0
#define CONTAINER_LIST_TAB_COLUMN 1
#define ITEM_LIST_ENABLE_COLUMN 0
#define ITEM_LIST_COLUMN 1
// ---------------------------------------------------------------------------
int __fastcall ComparePickNMix(void *Item1, void *Item2)

{
	TPnMOrder *ptrItem1 = (TPnMOrder*)Item1;
	TPnMOrder *ptrItem2 = (TPnMOrder*)Item2;

	if (ptrItem1->Price == 0 && ptrItem2->Price != 0 && TGlobalSettings::Instance().HideFreeItemsAtBilling)
	{
		return 1;
	}
	else if (ptrItem1->Price != 0 && ptrItem2->Price == 0 && TGlobalSettings::Instance().HideFreeItemsAtBilling)
	{
        return -1;
	}
	else
	{
		if (ptrItem1->GroupNumber > ptrItem2->GroupNumber)
		{
			return 1;
		}
		else if (ptrItem1->GroupNumber == ptrItem2->GroupNumber)
		{
			if (ptrItem1->IsParent < ptrItem2->IsParent)
			{
				return 1;
			}
			else if (ptrItem1->IsParent == ptrItem2->IsParent)
			{
				if (ptrItem1->TimeStamp > ptrItem2->TimeStamp)
				{
					return 1;
				}
				else if (ptrItem1->TimeStamp == ptrItem2->TimeStamp)
				{
					if (ptrItem1->Name > ptrItem2->Name)
					{
						return 1;
					}
					else if (ptrItem1->Name == ptrItem2->Name)
					{
						if (ptrItem1->Qty > ptrItem2->Qty)
						{
							return 1;
						}
						else if (ptrItem1->Qty == ptrItem2->Qty)
						{
							return 0;
						}
						else
						{
							return -1;
						}
					}
					else
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
}
// ---------------------------------------------------------------------------
bool IsWeightedSize(TIBSQL &query, TPnMOrder &order)
{
	query.Close();
	query.SQL->Text = "select weighted_size "
	                  "       from orders o "
	                  "            inner join sizes s on "
	                  "                  s.size_name = o.size_name "
	                  "       where weighted_size = 'T' "
	                  "             and o.order_key = :order_key; ";

	query.ParamByName("order_key")->AsInteger = order.Key;
	query.ExecQuery();

	return !query.Eof;
}
// ---------------------------------------------------------------------------
__fastcall TfrmBillGroup::TfrmBillGroup(TComponent* Owner, Database::TDBControl &inDBControl) : TZForm(Owner), DBControl(inDBControl),
TabList(new TStringList)
{
	MembershipConfirmed = false;
    HasOnlineOrders = false;
	Scaled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::FormCreate(TObject *Sender)
{
	CurrentDisplayMode = eNoDisplayMode;
	TDeviceRealTerminal::Instance().AfterTabStateChanged.RegisterForEvent(OnTabChange);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.RegisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardRemoved.RegisterForEvent(OnSmartCardRemoved);
    cmClientManager.reset( new TChefmateClientManager() );
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::FormDestroy(TObject *Sender)
{
	TDeviceRealTerminal::Instance().AfterTabStateChanged.DeregisterForEvent(OnTabChange);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardInserted.DeregisterForEvent(OnSmartCardInserted);
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->OnCardRemoved.DeregisterForEvent(OnSmartCardRemoved);
    delete TabList;
    SelectedItems.clear();
    VisibleItems.clear();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::FormShow(TObject *Sender)
{
//    TGlobalSettings::Instance().EnableOnlineOrdering = true;
	FormResize(this);
	InCheckFunc = false;
    ClipTabInTable =false;
    VoucherCode = "";
	// SelectedTabs.clear();
	// Remove these to values in order to have page remember the last
	// selected Tab Container. But beware the Member/Staff Picker will pop up before page shows.
	btnApplyMembership->Enabled = TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Registered"];

    // apply waiter station settings if enabled in Maintainance area
    applyWaiterStationSettingsIfEnabled();
	if (CurrentDisplayMode == eNoDisplayMode)
	{
		PostMessage(Handle, UWM_SHOWZONESELECT, 0, 0);
	}
	else
	{
            switch(CurrentDisplayMode)
            {
                case eNoDisplayMode:
                    CurrentSelectedTab = 0;
                    break;
                case eTables:
                    CurrentDisplayMode = eTables;
                    tbtnSelectZone->Caption = "Tables";
                    break;
                case eTabs:
                    switch(CurrentTabType)
                    {
                            case TabDelayedPayment:
                            tbtnSelectZone->Caption = "Delayed Payment Tabs";
                                    break;
                            case TabNormal:
                                    tbtnSelectZone->Caption = "Tabs";
                                    break;
                            case TabStaff:
                                    tbtnSelectZone->Caption = "Staff Tabs";
                                    break;
                            case TabMember:
                                    tbtnSelectZone->Caption = "Members Tab";
                                    break;
                    }
                    break;
                case eRooms:
                    tbtnSelectZone->Caption = "Rooms";
                    break;
            }
            UpdateRightButtonDisplay(Sender);
            Database::TDBTransaction DBTransaction(DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
            DBTransaction.StartTransaction();
            SelectedTabs.clear();
            SelectedItems.clear();
            UpdateTableDetails(DBTransaction);
            TabStateChanged(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
            DBTransaction.Commit();
            UpdateTableForOnlineOrdering();
            DisableTransferButtonWhenLMIsEnabled();
	}
    TGlobalSettings::Instance().IsPOSOffline = true;
	if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
	{ // Restore Membership, Reminds the user to remove the smart card.
		OnSmartCardInserted(NULL);
	}
	else
	{
		OnSmartCardRemoved(NULL);

	}
	SetGridColors(tgridContainerList);
	SetGridColors(tgridItemList);
    CheckDiscountPointsBillGroup = false; // MM-3908

   //mm-5145
     if(TGlobalSettings::Instance().MandatoryMembershipCard )
     {
            btnApplyMembership->ButtonColor = clRed;
      }else
      {
         btnApplyMembership->ButtonColor = clPurple;
      }
      //mm-5145
      delivery_time = 0;
      PatronCountForMallExport = 0;
      tbtnToggleGST->Visible = false;
    logList.reset(new TStringList());
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::FormResize(TObject *Sender)
{

  if(Scaled)
  {
      Scaled = false;
      GUIScale::ParentWidth(pnlgridContainerListBoarder,0.5);
      GUIScale::ParentWidth(Panel7,0.30);
      GUIScale::ChildrenHeight(Panel6, 0.1);
      GUIScale::ParentWidth(pnlgridContainerListBoarder,0.50);
      GUIScale::ChildrenWidth(Panel8, 0.17);
      GUIScale::ParentWidth(tbtnSelectZone,0.50);
      GUIScale::ParentHeight(Panel5,0.09);
      GUIScale::ParentWidth(tbtnSelectAll,0.50);
      Tag = Screen->Width;
      if((double)Screen->Width/Screen->Height < 1.4 )
        {
                tgridContainerList->Height = tgridContainerList->Height * 0.7;
        }
   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::FormClose(TObject *Sender, TCloseAction &Action)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
	DBTransaction.Commit();
	TDeviceRealTerminal::Instance().PoleDisplay->UpdatePoleDisplayDefault();
	ResetSelection();
    delivery_time = 0;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::SelectZone(Messages::TMessage& Message)
{
	if(SelectedZone() == eNoDisplayMode)
	{
       UpdateContainerList();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::UpdateRightButtonDisplay(TObject *Sender)
{
	if (CurrentDisplayMode == eTabs)
	{
        btnBillTable->Enabled = false;
        btnBillSelected->Enabled = true;
        btnPartialPayment->Enabled = true;
        btnSplitPayment->Enabled = true;
        btnTransfer->Enabled = true;
        btnTransfer->Caption = "Transfer To";
        btnTransfer->ButtonColor = clOlive;
        btnApplyMembership->Enabled = true;
        btnApplyMembership->ButtonColor = clPurple;
        btnApplyMembership->Caption = "Apply Membership";
        if (CurrentTabType == TabWeb)
        {
          btnTransfer->Color = clSilver;
          btnTransfer->Enabled = false;
          //
          btnApplyMembership->Enabled = false;
        }
	}
	else if (CurrentDisplayMode == eTables)
	{
		btnBillTable->Enabled = true;
		btnBillTable->Caption = "Bill Entire Table";
		btnBillTable->ButtonColor = clGreen;
		btnTransfer->Enabled = true;
		btnTransfer->Caption = "Transfer To";
		btnTransfer->ButtonColor = clOlive;
		btnApplyMembership->Enabled = true;
		btnApplyMembership->ButtonColor = clPurple;
		btnApplyMembership->Caption = "Apply Membership";
	}
	else if (CurrentDisplayMode == eRooms)
	{
		btnBillTable->Enabled = false;
		btnTransfer->Enabled = true;
		btnTransfer->Caption = "Transfer To";
		btnTransfer->ButtonColor = clOlive;
		btnApplyMembership->Enabled = true;
		btnApplyMembership->ButtonColor = clPurple;
		btnApplyMembership->Caption = "Apply Membership";
	}
	else if (CurrentDisplayMode == eInvoices)
	{
		btnBillTable->Enabled = false;
		btnTransfer->Color = clSilver;
		btnTransfer->Enabled = false;
		btnBillSelected->Enabled = true;
		btnApplyMembership->Enabled = false;
		btnApplyMembership->ButtonColor = clSilver;
		btnApplyMembership->Caption = "Apply Membership";
	}

    applyWaiterStationSettingsIfEnabled();

    if(CurrentTabType == TabDelayedPayment)
    {
       tbtnSelectZone->OnMouseClick = NULL;
       tbtnMove->Enabled = false;
       tbtnSelectAll->Enabled = false;
       btnTransfer->Enabled = false;
    }
    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney)
    {
      btnTransfer->Color = clSilver;
      btnTransfer->Enabled = false;
    }
    IsRegistrationVerified();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnReprintReceiptsMouseClick(TObject *Sender)
{
        bool OrderMoved = false;
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		MessageBox("Please select a receipt printer from Setup first.", "Print error", MB_OK + MB_ICONERROR);
	}
	else
	{
		if (!SelectedItems.empty())
		{
			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
			TPaymentTransaction ReceiptTransaction(DBTransaction);
			ReceiptTransaction.ApplyMembership(Membership);
			TempReceipt->Transaction = &ReceiptTransaction;
            if((TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType != SiHot) ||
               (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled))
                TempReceipt->Transaction->Customer = TCustomer(0,0,"");

            if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
              TempReceipt->Transaction->Customer = TCustomer("",0,"");

			std::set <__int64> ReceiptItemKeys;
			for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
			{
				ReceiptItemKeys.insert(itItem->first);
			}

			TDBOrder::GetOrdersFromOrderKeys(DBTransaction, ReceiptTransaction.Orders, ReceiptItemKeys);

            LoadCustNameAndOrderType(ReceiptTransaction);

			if (TGlobalSettings::Instance().EnableMenuPatronCount)
			{
				ReceiptTransaction.CalculatePatronCountFromMenu();
			}
            else
                TempReceipt->Transaction->Patrons = PatronTypes;

			ReceiptTransaction.Money.CreditAvailable = TDBOrder::LoadCreditFromOrders(DBTransaction, ReceiptTransaction.Orders);
			ReceiptTransaction.Money.Recalc(ReceiptTransaction);

			TempReceipt->JobType = pjReceiptReceipt;
			TempReceipt->SenderType = devPC;
			TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
             bool isTable = false;
            int tabKey = 0;
			TStringList *TabHistory = new TStringList;
			for (std::set <__int64> ::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end(); advance(itTabs, 1))
			{
				tabKey = *itTabs;
                isTable = false;
                if (CurrentDisplayMode == eTables)
                    {
                       tabKey = CurrentTable;
                       isTable = true;
                    }
				TDBTab::GetPartialPaymentList(DBTransaction, tabKey, TabHistory,isTable);
				for(int x = 0 ; x < TabHistory->Count; x++)
                {
                   if(TempReceipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
                   {
                     TempReceipt->TabHistory->Add(TabHistory->Strings[x]);
                   }
                }
			}
			delete TabHistory;

			if (CurrentDisplayMode == eTables)
			{
				AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, CurrentTable);
				TempReceipt->MiscData["PartyName"] = PartyName;
                    //If TransferTableOnPrintPrelim is on then move selected items to a tab
                    TMMContactInfo TempUserInfo;
                    TempUserInfo = TDeviceRealTerminal::Instance().User;
                    std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
                    bool allowed = Staff->TestAccessLevel(TempUserInfo,CheckPaymentAccess);
                    if(allowed && TGlobalSettings::Instance().TransferTableOnPrintPrelim)
                    {
                        int delayedTabKey = 0;
                        std::auto_ptr<TList>FoodOrders(new TList);
                        std::auto_ptr<TList>BevOrders(new TList);
                        if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
                        {
                            TManagerDelayedPayment::Instance().SplitDelayedPaymentOrderByMenuType(ReceiptTransaction.Orders, FoodOrders.get(), BevOrders.get());
                        }

                       if(TGlobalSettings::Instance().IsBillSplittedByMenuType && BevOrders->Count )
                       {
                            delayedTabKey = TManagerDelayedPayment::Instance().MoveOrderToTab(ReceiptTransaction,true, false);
                       }
                       else
                       {
                            delayedTabKey = TManagerDelayedPayment::Instance().MoveOrderToTab(ReceiptTransaction,true);
                       }
                        if(delayedTabKey != 0)
                        {
                            TDBTab::SetDelayedPatronCount(DBTransaction,delayedTabKey,ReceiptTransaction.Patrons);
                        }
                       if(TDBTables::IsEmpty(DBTransaction,CurrentTable))
                        {
                          TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,eNoneStatus);
                        }
                       OrderMoved = true;
                    }
                    else
                    {
                      TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,ePrelim);
                    }

			}
			else if (CurrentDisplayMode == eRooms)
			{
				AnsiString PartyName = TDBRooms::GetPartyName(DBTransaction, CurrentRoom);
				TempReceipt->MiscData["PartyName"] = PartyName;
			}

			if (CurrentDisplayMode == eRooms)
			{
				TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(CurrentRoom));
				TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, CurrentRoom));
			}

			if (CurrentInvoiceKey != 0)
			{
				TempReceipt->AccountPayment = true;
				TempReceipt->AccountInvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, CurrentInvoiceKey);
			}

			TempReceipt->PaymentType = ptPreliminary;
			Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
			TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
            ReceiptTransaction.DeleteOrders();

			DBTransaction.Commit();
            if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount != ""
                    || TGlobalSettings::Instance().LoyaltyMateEnabled)
            {
                VoucherCode = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount;
                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
                DBTransaction1.StartTransaction();
                RemoveMembership(DBTransaction1);
                TDBTables::GetOrderKeys(DBTransaction1, CurrentTable, ReceiptItemKeys);
                RemoveLoyaltymateMembership(ReceiptItemKeys);
                DBTransaction1.Commit();
                ClearLoyaltyVoucher();
                VoucherCode = "";
            }
		}
		else
		{
                    // If TransferTableOnPrintPrelim and no item is selected then move all items to tab
                    if(TGlobalSettings::Instance().TransferTableOnPrintPrelim && CurrentDisplayMode == eTables)
                     {
                        Database::TDBTransaction DBTransaction(DBControl);
                        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
                        DBTransaction.StartTransaction();
                        TMMContactInfo TempUserInfo;
                        TempUserInfo = TDeviceRealTerminal::Instance().User;
                        std::auto_ptr<TContactStaff>Staff(new TContactStaff(DBTransaction));
                        bool allowed = Staff->TestAccessLevel(TempUserInfo,CheckPaymentAccess);
                        if(allowed)
                        {
                            std::set <__int64> ReceiptItemKeys;
                            std::auto_ptr<TList>OrdersList(new TList);
                            std::auto_ptr<TList>FoodOrdersList(new TList);
                            std::auto_ptr<TList>BevOrdersList(new TList);
                            bool isMixedMenuOrder = true;
                            int Size = 1;

                            TDBTables::GetOrderKeys(DBTransaction, CurrentTable, ReceiptItemKeys);
                            TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), ReceiptItemKeys);

                            if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
                            {
                                TManagerDelayedPayment::Instance().SplitDelayedPaymentOrderByMenuType(OrdersList.get(), FoodOrdersList.get(), BevOrdersList.get());
                            }

                            if(BevOrdersList->Count && FoodOrdersList->Count)
                                Size = 2;

                            for(int index = 0; index < Size; index++)
                            {
                                std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
                                TPaymentTransaction ReceiptTransaction(DBTransaction);
                                ReceiptTransaction.ApplyMembership(Membership);
                                ReceiptTransaction.Patrons = PatronTypes;
                                if(TGlobalSettings::Instance().IsBillSplittedByMenuType && Size == 2)
                                {
                                    if(index )
                                        ReceiptTransaction.Orders->Assign(BevOrdersList.get());
                                    else
                                        ReceiptTransaction.Orders->Assign(FoodOrdersList.get());
                                }
                                else
                                {
                                    ReceiptTransaction.Orders->Assign(OrdersList.get());
                                }

                                TempReceipt->Transaction = &ReceiptTransaction;
                                if((TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType != SiHot) ||
                                   (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled))
                                    TempReceipt->Transaction->Customer = TCustomer(0,0,"");

                                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
                                    TempReceipt->Transaction->Customer = TCustomer("",0,"");

                                if(ReceiptTransaction.Orders->Count > 0)
                                {
                                    ReceiptTransaction.IgnoreLoyaltyKey = false;
                                    ReceiptTransaction.Recalc();

                                    if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
                                    {
                                        TItemComplete *Order = (TItemComplete*)ReceiptTransaction.Orders->Items[0];
                                        isMixedMenuOrder = Order->ItemType == 1 ? false : true;
                                    }

                                    int delayedTabKey = TManagerDelayedPayment::Instance().MoveOrderToTab(ReceiptTransaction,true, isMixedMenuOrder);
                                    if(delayedTabKey != 0)
                                    {
                                        TDBTab::SetDelayedPatronCount(DBTransaction,delayedTabKey,ReceiptTransaction.Patrons);
                                        TempReceipt->Transaction->Patrons = PatronTypes;
                                    }
                                    OrderMoved = true;
                                    TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,eNoneStatus);
                                    TempReceipt->JobType = pjReceiptReceipt;
                                    TempReceipt->SenderType = devPC;
                                    TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;
                                    TempReceipt->PaymentType = ptPreliminary;
                                    Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
                                    TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
                                    ReceiptTransaction.DeleteOrders();
                                }
                            }
                            if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount != ""
                                    || TGlobalSettings::Instance().LoyaltyMateEnabled)
                            {
                                VoucherCode = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount;
                                Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                                TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
                                DBTransaction1.StartTransaction();
                                RemoveMembership(DBTransaction1);
                                TDBTables::GetOrderKeys(DBTransaction1, CurrentTable, ReceiptItemKeys);
                                RemoveLoyaltymateMembership(ReceiptItemKeys);
                                DBTransaction1.Commit();
                                ClearLoyaltyVoucher();
                                VoucherCode = "";
                            }
                        }
			            DBTransaction.Commit();
                     }
                     else
                     {
		                MessageBox("Nothing selected to print.", "Print error", MB_OK + MB_ICONERROR);
                     }
		}
        if(OrderMoved)
        {
           ResetForm();
        }
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::CancelItems(Database::TDBTransaction &DBTransaction, std::set <__int64> ItemsToBeCanceled,
	TMMContactInfo &CancelUserInfo)
{
    // Mall Export Decalartion
    bool CancelDiscSCDStatus = false;
    int tabKey = 0;
    TMallExportOtherDetailsUpdate exportOtherDetailsUpdate;
	try
	{
		if (!ItemsToBeCanceled.empty())
		{
			if (MessageBox("These items have already been sent and a credit will appear on the customer's receipt.\rDo you wish to continue?",
					"Credit Item?", MB_OKCANCEL + MB_ICONQUESTION) == IDOK)
			{
				// Get the Note.
				AnsiString Note = "";
				std::auto_ptr <TfrmMessage> frmMessage(TfrmMessage::Create <TfrmMessage> (this, DBControl));
				frmMessage->MessageType = eCancelReason;
				frmMessage->ShowModal();
				if (TGlobalSettings::Instance().ForcedReasons && frmMessage->TextResult == "")
				{
					do
					{
						MessageBox("You must enter a cancel reason", "Please Comply", MB_OK + MB_ICONINFORMATION);
						frmMessage->ShowModal();
					}
					while (frmMessage->TextResult == "");
				}
				Note = frmMessage->TextResult;


				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr <TPaymentTransaction> TempTransaction(new TPaymentTransaction(DBTransaction));
				std::auto_ptr <TList> OrdersList(new TList);
				TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), ItemsToBeCanceled);
                 bool InformChef = false;
                bool ReturnStock = false;
                std::auto_ptr<TfrmPaymentType> frmPaymentType;
                 if(frmPaymentType->IsReceipeAdded(OrdersList.get()))
                {
                   ReturnStock = (MessageBox("Do you wish to Return this Item to Stock. \n(i.e. Put it back on the shelf)", "Return To Stock?", MB_YESNO + MB_ICONQUESTION) == IDYES);
                }
                if(frmPaymentType->IsItemAssignToPrinter(OrdersList.get()))
                {
                    InformChef = (MessageBox("Do you wish to send a cancellation notice to the Chef?", "Inform Chef?", MB_YESNO + MB_ICONQUESTION) == IDYES);
                }
				for (int i = 0; i < OrdersList->Count; i++)
				{
					TItemComplete *Order = (TItemComplete*)OrdersList->Items[i];
                    if(TGlobalSettings::Instance().DeleteItemSizeAfterSale && ReturnStock)
                    {
                        Database::TDBTransaction dbTransaction(DBControl);
                        dbTransaction.StartTransaction();
                        TDBTab::SetVisible(dbTransaction, Order->ItemKey, Order->Size, Order->MenuKey);
                        dbTransaction.Commit();
                    }
					Order->Cancel(ReturnStock);

					if (Order->OriginalItemKey != NULL)
					{
						Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);
					}

					TSecurityReference *SecRef = new TSecurityReference;
					SecRef->UserKey = CancelUserInfo.ContactKey;
                    if(ReturnStock)
                    {
                        SecRef->Event = "CancelY";
                    }
                    else
                    {
					    SecRef->Event = SecurityTypes[secCancel];
                    }

					SecRef->From = CurrToStrF(Order->GrandTotal(), ffCurrency, 2);
					SecRef->Note = Note;
					SecRef->To = "0.00";
					SecRef->TimeStamp = Now();
					Order->Security->SecurityAddUniqueRef(DBTransaction, Order->OrderKey, SecRef);

					TempTransaction->Orders->Add(Order);

					for (int i = 0; i < Order->SubOrders->Count; i++)
					{
						TItemCompleteSub *SubOrder = (TItemCompleteSub*)Order->SubOrders->Items[i];
						SubOrder->Cancel(ReturnStock);
					}

                    // MallExport
                    tabKey = Order->TabKey;
				}

                // MallExport
                if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
                {
                    CancelDiscSCDStatus = exportOtherDetailsUpdate.getCancelledDiscountStatus(tabKey) ? true : false;
                }

				TDBOrder::CancelOrder(DBTransaction, TempTransaction->Orders);
				TDeviceRealTerminal::Instance().ManagerStock->UpdateStock(DBTransaction, TempTransaction->Orders);

				if( InformChef )
				{
                    std::map<__int64, TList*> itemGroupsToCancel;

					std::map<__int64, TPnMOrder>::const_iterator ordIT;

					for( ordIT = SelectedItems.begin(); ordIT != SelectedItems.end(); ordIT++ )
					{
						TPnMOrder* item = new TPnMOrder( ( TPnMOrder )ordIT->second );

                        if(itemGroupsToCancel.find(item->TimeKey) == itemGroupsToCancel.end())
                        {
                            TList *itemGroup = new TList;
                            itemGroupsToCancel.insert( std::pair<__int64, TList*>((__int64)item->TimeKey, itemGroup));
                        }

                        itemGroupsToCancel.find(item->TimeKey)->second->Add(item);
					}

                    std::map< __int64, TList*>::iterator it  = itemGroupsToCancel.begin();

                    TItemComplete *order1 = ( TItemComplete* )TempTransaction->Orders->Items[0];



                    AnsiString tabTableName =  order1->TabContainerName + " : " + order1->TabName;
                   if(order1->TabType == TabWeb)
                    {
                       tabTableName =  TDBWebUtil::GetOrderGUID(DBTransaction,order1->WebKey) + " : " + order1->TabName;
                    }
                   else if(order1->TabType == TabInvoice)
                    {
                       tabTableName =  order1->InvoiceNumber + " : " + order1->TabName;
                    }
                   else if(order1->TabType != TabTableSeat)
                    {
                       tabTableName =  order1->TabName + " : " + order1->TabName;
                    }
                    if(TGlobalSettings::Instance().IsRegistrationVerified)
                    {
                        for( ; it != itemGroupsToCancel.end() ; it++ )
                        {

                            _onItemsCanceled( it->second,tabTableName);
                            it->second->Clear();
                        }
                    }
                    itemGroupsToCancel.clear();

					std::auto_ptr <TReqPrintJob> Request(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
					Request->Transaction = TempTransaction.get();
					Request->SenderType = devPC;
					Request->Waiter = CancelUserInfo.Name;
					Request->Transaction->Money.Recalc(*Request->Transaction);

                    if((TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType != SiHot) ||
                       (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled))
                        Request->Transaction->Customer = TCustomer(0,0,"");

                    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
                        Request->Transaction->Customer = TCustomer("",0,"");

                    std::auto_ptr<TKitchen> Kitchen(new TKitchen());
					Kitchen->Initialise(DBTransaction);
					Kitchen->GetPrintouts(DBTransaction, Request.get());
					Request->Printouts->Print(devPC);
					if (Request->Header.Error != proA_Ok)
					{
						ShowMessage(Request->Header.ErrorMsg);
					}

				}
				DBTransaction.Commit();

				while (OrdersList->Count != 0)
				{
					TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
					OrdersList->Remove(Order);
					delete Order;
				}

                // MallExport
                if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
                {
                    TDateTime MEDateTime = Now();
                    exportOtherDetailsUpdate.extractCancelStatus( MEDateTime, CancelDiscSCDStatus, tabKey, PatronCountForMallExport);
                }
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to Cancel this Set of Items.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}

}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnBillTableMouseClick(TObject *Sender)
{
    RecordFiscalLogsPaymentSystem(logList.get(),"-----------------------btnBillTableMouseClick() called-----------------------------");
    bool ispaymentComplete = false;
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    TPaymentTransaction PaymentTransaction(DBTransaction);
    if(TGlobalSettings::Instance().LoyaltyMateEnabled )     //&& HasOnlineOrders
        DownloadOnlineMember();
	try
	{
        TGlobalSettings::Instance().IsThorPay = true;

		ResetSelection();

		std::set <__int64> TabsToLock;
		TDBTables::GetTabKeys(DBTransaction, CurrentTable, TabsToLock);
        std::set <__int64> ::iterator itTabs = TabsToLock.begin();
        int totalDivision = 0;
        int divisionLeft = 0;
        int tabkey = 0;
        if (itTabs != TabsToLock.end())
        {
            divisionLeft = TDBTab::GetSplitDivisions(DBTransaction, *itTabs);
            totalDivision = TDBTab::GetTotalDivisions(DBTransaction, *itTabs);
            tabkey = *itTabs;
        }
        TSCDPWDChecker scdpwdChecker;
		if (!TabsToLock.empty())
		{
			if (TabsInUseOk(DBTransaction, TabsToLock))
			{
				std::set <__int64> ItemsToBill;
				TDBTables::GetOrderKeys(DBTransaction, CurrentTable, ItemsToBill);
				if (ItemsToBill.empty())
				{
					MessageBox("No Orders to bill!", "Error", MB_OK + MB_ICONERROR);
				}
                else if(!scdpwdChecker.CheckForBillingWithPWD(DBTransaction,ItemsToBill))
                {
                    MessageBox("Items with PWD Discounts and items with Non PWD Discounts can not be billed at the same time.", "Error", MB_ICONWARNING + MB_OK);
                }
				else
				{
                       if(totalDivision > 0 && CurrentDisplayMode == eTables)
                       {
                           int ptrCount = 0;
                           if((!TGlobalSettings::Instance().PromptForPatronCount)&&(!TGlobalSettings::Instance().PromptPatronCountOnTableSales))
                           {
                              PatronCount = TDBTables::GetPatronNumbers(DBTransaction,CurrentTable);
                           }
                           else
                           {
                              UpdateBilledPatronCount(DBTransaction,CurrentTable);
                           }
                       }
                       else
                       {

                         if((!TGlobalSettings::Instance().PromptForPatronCount))
                           {
                              PatronCount = TabsToLock.size();
                           }
                           else
                           {
                             UpdateBilledPatronCount(DBTransaction,CurrentTable);
                           }
                       }
                       if (PatronCount <= 0)
                             PatronCount = 1;
                       BillItems(PaymentTransaction, DBTransaction, ItemsToBill, eTransOrderSet,ispaymentComplete);
				}
			}
		}
		else
		{
			MessageBox("No Seats to bill!", "Error", MB_OK + MB_ICONERROR);
		}
        RecordFiscalLogsPaymentSystem(logList.get(),"Transaction Commit of  btnBillTableMouseClick()");
		DBTransaction.Commit();
        if(ispaymentComplete && CurrentDisplayMode != eInvoices)
            SendFiscalPrint(PaymentTransaction);
        ResetTransactionAfterCommit(PaymentTransaction);
		ResetForm();
        UpdateRightButtonDisplay(Sender);
        if(CurrentDisplayMode == eTables)
        {
            if(NeedtoUpdateTableForOnlineOrdering())
               UpdateTableForOnlineOrdering();
            else
                DisableTransferButtonWhenLMIsEnabled();

            UpdateTabeleStateForOO();
        }
        else if(CurrentDisplayMode == eTabs)
        {
            if(TDBTab::HasOnlineOrders(CurrentSelectedTab))
                UpdateTabForOnlineOrdering();
            else
                DisableTransferButtonWhenLMIsEnabled();
        }
        else
            HasOnlineOrders = false;
	}

	catch(Exception & E)
	{
        DBTransaction.Rollback();
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rPlease check this Table is not in use.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        RecordFiscalLogsPaymentSystem(logList.get(),"Transaction Rollback of  btnBillTableMouseClick()");
	}
}
//----------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnBillSelectedMouseClick(TObject *Sender)
{
    RecordFiscalLogsPaymentSystem(logList.get(),"-----------------------btnBillSelectedMouseClick() called-----------------------------");
    TGlobalSettings::Instance().IsThorBillSelected = true;
    TMMTabType type;
    int noOfTabs = 0;
    int tabKey;

    if(CurrentDisplayMode == eTabs || CurrentDisplayMode == eTables)
    {
        if(HasOnlineOrders)
        {
            int sideCount = 0;
		    for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
		    {
                if(itItem->second.Price == 0 && itItem->second.IsSide == true && itItem->second.IsItemFree == false)  //Added condition to exclude Side which has Cost equal to 0
                sideCount++;
		    }
            if(VisibleItems.size()-sideCount > SelectedItems.size())
            {
                MessageBox("To bill off Online ordering tab , please select all the Items.","Info",MB_OK+MB_ICONINFORMATION);
                return;
            }
        }
        if(TGlobalSettings::Instance().LoyaltyMateEnabled )    //&& HasOnlineOrders
            DownloadOnlineMember();
    }

	try
	{
		if (SelectedItems.empty())
		{
			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			bool Proceed = false;
			if (SelectedTabs.empty() && CurrentSelectedTab != 0)
			{
				if (TDBTab::GetTabCredit(DBTransaction, CurrentSelectedTab) != 0)
				{
					Proceed = true;
				}
			}
			DBTransaction.Commit();

			if (Proceed)
			{
				DBTransaction.StartTransaction();
                bool ispaymentComplete = false;
				Proceed = TabStaffAccessOk(DBTransaction);
				if (Proceed)
				{
                    TPaymentTransaction CreditTransaction(DBTransaction);
					CreditTransaction.Type = eTransCreditPurchase;
					CreditTransaction.SalesType = eCreditPurchase;
					CreditTransaction.Money.Change = 0;
					CreditTransaction.Money.CreditAvailable = 0;
					CreditTransaction.Money.PaymentAmount = 0;
					CreditTransaction.Money.GrandTotal = 0;
					CreditTransaction.Money.PaymentDue = 0;
                    if(SelectedDiscount.IsComplimentaryDiscount())
                      {
                         CreditTransaction.TypeOfSale = ComplimentarySale;
                      }
                    else if( SelectedDiscount.IsNonChargableDiscount())
                      {
                         CreditTransaction.TypeOfSale = NonChargableSale;
                      }
					TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, CreditTransaction.Patrons, 1);

					TTabCredit Credit = CreditTransaction.TabCredit[CurrentSelectedTab];
					Credit.CurrentCredit = TDBTab::GetTabCredit(DBTransaction, CurrentSelectedTab);
					Credit.CreditRedeemed = Credit.CurrentCredit;
					CreditTransaction.TabCredit[CurrentSelectedTab] = Credit;
                    CreditTransaction.IgnoreLoyaltyKey = false;
					CreditTransaction.Recalc();
                    if(TGlobalSettings::Instance().PMSType == SiHot && ((TItemComplete*)CreditTransaction.Orders->Items[0])->RoomNoStr != "" && TGlobalSettings::Instance().EnableCustomerJourney)
                        CustomizeForSiHot(CreditTransaction);
					ispaymentComplete = TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(CreditTransaction);
                    if(ispaymentComplete && CurrentDisplayMode != eInvoices)
                        SendFiscalPrint(CreditTransaction);
					TDBTab::SetTabCreditLimit(DBTransaction, CurrentSelectedTab, -1);
				}
				DBTransaction.Commit();
			  	ResetForm();
			}
			else
			{
				MessageBox("Nothing selected to bill!", "Error", MB_OK + MB_ICONERROR);
			}
		}
		else
		{
			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
            TPaymentTransaction PaymentTransaction(DBTransaction);
            bool ispaymentComplete = false;
			bool Proceed = TabStaffAccessOk(DBTransaction);
			DBTransaction.Commit();
            bool  AllowTimeOut = false;

			if (Proceed)
			{
				DBTransaction.StartTransaction();
                try
                {
				bool TabRemoved = false;
				bool StuffToBill = false;
                bool sendCloseTabMessage = true;
				for (std::set <__int64> ::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end(); advance(itTabs, 1))
				{
					if (!TDBTab::GetTabPermanent(DBTransaction, *itTabs) && TDBTab::GetIsEmpty(DBTransaction, *itTabs) && TDBTab::GetTabCredit
						(DBTransaction, *itTabs) == 0)
					{
						if (MessageBox(("This tab (" + TDBTab::GetTabName(DBTransaction, *itTabs) + ") is empty. Do you wish to remove it?").c_str
								(), "Information", MB_ICONINFORMATION + MB_OKCANCEL) == IDOK)
						{
							TDBTab::ClearTab(DBTransaction, *itTabs);
							TabRemoved = true;
						}
						SelectedTabs.erase(*itTabs);
					}
					else
					{
						StuffToBill = true;
					}
				}
				if (TabRemoved == false)
				{
					if (StuffToBill)
					{
						std::set <__int64> SelectedItemKeys;
						for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
							advance(itItem, 1))
						{
							SelectedItemKeys.insert(itItem->first);
						}

                        if(TGlobalSettings::Instance().HideFreeSides)
                            MergeZeroPriceSideKeysWithSelectedItemKeys(SelectedItemKeys); //Merging the Item keys of Zero Price Sides with Selected Item Keys

						int mypatroncount = 0;
						std::map <__int64, TPnMOrder> TabItems;
						for (std::set <__int64> ::iterator CrntTabKey = SelectedTabs.begin(); CrntTabKey != SelectedTabs.end();
							advance(CrntTabKey, 1))
						{
							bool AllSelected = true;
                            noOfTabs = SelectedTabs.size();
                            if(noOfTabs == 1)
                            {
                                tabKey = *CrntTabKey;
                            }
							TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction, *CrntTabKey, TabItems);
							for (std::map <__int64, TPnMOrder> ::iterator itItem = TabItems.begin(); itItem != TabItems.end(); advance(itItem, 1))
							{
								if (SelectedItems.find(itItem->first) == SelectedItems.end())
								{
									AllSelected = false;
                                    sendCloseTabMessage = false;
									break;
								}
							}
							if (AllSelected)
								mypatroncount++;
						}

						if (mypatroncount == 0)
							mypatroncount = 1;
						PatronCount = mypatroncount;
                        if(TGlobalSettings::Instance().PromptForPatronCount &&  CurrentDisplayMode == eTables)
                        {
                              for (std::vector<TPatronType>::iterator ptrPatron = PatronTypes.begin();
                                    ptrPatron != PatronTypes.end(); ptrPatron++)
                               {
                                  if(!ptrPatron->Default)
                                  {
                                    ptrPatron->Count = 0;
                                  }
                               }
                              TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, PatronTypes, PatronCount);
                        }

                        // set the transaction type as we use it to differenciate between which payment system to use
                        TPaymentTransactionType transactionType = CurrentDisplayMode != eInvoices ? eTransOrderSet : eTransInvoicePayment;
                        TDeviceRealTerminal::Instance().PaymentSystem->IsClippSale = false;
                        BillItems(PaymentTransaction, DBTransaction, SelectedItemKeys, transactionType,ispaymentComplete);
					}
					else
					{
						MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
					}
				}
				else
				{
					MessageBox("Empty tabs have been removed or unselected. Please confirm your new Tab selection and bill them off.", "Info",
						MB_OK + MB_ICONINFORMATION);
				}

                    DBTransaction.Commit();
                    if(ispaymentComplete && CurrentDisplayMode != eInvoices)
                        SendFiscalPrint(PaymentTransaction);
                    ResetTransactionAfterCommit(PaymentTransaction);
                    RecordFiscalLogsPaymentSystem(logList.get(),"Transaction commit in btnBillSelectedMouseClick()");
                    ResetForm();
                    if(TGlobalSettings::Instance().IsBillSplittedByMenuType )
                    {
                        if(CurrentDisplayMode == eTables)
                            ChangeBillEntireTableState();

                        if(!tgridItemList->RowCount)
                            tbtnToggleGST->Visible = false;
                     }
			    }
                catch(Exception & E)
                {
                    DBTransaction.Rollback();
                    RecordFiscalLogsPaymentSystem(logList.get(),"Transaction rollback in btnBillSelectedMouseClick()");
                    throw;
                }
            }
		}
        UpdateRightButtonDisplay(Sender);
        if(CurrentDisplayMode == eTables)
        {
            if(NeedtoUpdateTableForOnlineOrdering())
               UpdateTableForOnlineOrdering();
            else
                DisableTransferButtonWhenLMIsEnabled();

            UpdateTabeleStateForOO();
        }
        else if(CurrentDisplayMode == eTabs)
        {
            if(TDBTab::HasOnlineOrders(CurrentSelectedTab))
                UpdateTabForOnlineOrdering();
            else
                DisableTransferButtonWhenLMIsEnabled();
        }
        else
            HasOnlineOrders = false;
	}
	catch(Exception & E)
	{
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rPlease check that none of the selected tabs are in use.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
    TGlobalSettings::Instance().IsThorBillSelected = false;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::CloseTerminateCallBack(TObject* sender)
{
      ResetForm();
      TDeviceRealTerminal::Instance().ProcessingController.Pop();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnPartialPaymentMouseClick(TObject *Sender)
{
    RecordFiscalLogsPaymentSystem(logList.get(),"-----------------------btnPartialPaymentMouseClick() called-----------------------------");
    std::set <__int64> SplitItemKeySet;
    int SplitItemKey = 0;
	try
	{
		if (SelectedItems.empty())
		{
			MessageBox("Nothing selected to pay!", "Error", MB_OK + MB_ICONERROR);
		}
		else
		{
			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
            TPaymentTransaction PaymentTransaction(DBTransaction);
            bool ispaymentComplete = false;
			bool Proceed = TabStaffAccessOk(DBTransaction);
			DBTransaction.Commit();
			if (Proceed)
			{
                if(TGlobalSettings::Instance().LoyaltyMateEnabled && (CurrentDisplayMode == eTabs || CurrentDisplayMode == eTables))
                    DownloadOnlineMember();

				DBTransaction.StartTransaction();
				Proceed = false;
				bool StuffToBill = false;
				std::set <__int64> TabsToLock;
				for (std::set <__int64> ::iterator CrntTabKey = SelectedTabs.begin(); CrntTabKey != SelectedTabs.end(); advance(CrntTabKey, 1))
				{
					TabsToLock.insert(*CrntTabKey);
					if (!TDBTab::GetIsEmpty(DBTransaction, *CrntTabKey))
					{
						StuffToBill = true;
					}
				}

				if (StuffToBill)
				{
					if (!TDBTab::LockTabs(DBTransaction, TabsToLock, TDeviceRealTerminal::Instance().ID.Name))
					{
						if (MessageBox("There are seats/tabs that are in use by other terminals."
								"\rDo you wish to override these locks on these seats/tabs?", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
						{
							TDBTab::LockTabs(DBTransaction, TabsToLock, TDeviceRealTerminal::Instance().ID.Name, true);
							Proceed = true;
						}
						else
						{
							MessageBox("You can Bill off the unlocked seats individually.", "Infomation", MB_OK + MB_ICONINFORMATION);
						}
					}
					else
					{
						Proceed = true;
					}

					if (Proceed)
					{
						std::set <__int64> SelectedItemKeys;
						for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
							advance(itItem, 1))
						{
							SelectedItemKeys.insert(itItem->first);
						}
                        if(TGlobalSettings::Instance().HideFreeSides)
                        MergeZeroPriceSideKeysWithSelectedItemKeys(SelectedItemKeys);    //Merging the Item keys of Zero Price Sides with Selected Item Keys
                        PatronCount = DeterminePatronCount();
						SplitItemKey = BillItems(PaymentTransaction, DBTransaction, SelectedItemKeys, eTransPartialPayment,ispaymentComplete);
					}
				}
				else
				{
					MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
				}
				DBTransaction.Commit();
                if(ispaymentComplete && CurrentDisplayMode != eInvoices)
                    SendFiscalPrint(PaymentTransaction);
                ResetTransactionAfterCommit(PaymentTransaction);
                RecordFiscalLogsPaymentSystem(logList.get(),"Transaction commit of btnPartialPaymentMouseClick()");

                SplitItemKeySet.insert(SplitItemKey);
                if(VoucherCode != "" && TGlobalSettings::Instance().LoyaltyMateEnabled)
                {
                    RemoveLoyaltymateMembership(SplitItemKeySet);
                }

                if(CurrentDisplayMode == eTables)
                    UpdateTabeleStateForOO();
                ResetForm();
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);

		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        RecordFiscalLogsPaymentSystem(logList.get(),"Exception caught in btnPartialPaymentMouseClick()");
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnSplitPaymentMouseClick(TObject *Sender)
{
    RecordFiscalLogsPaymentSystem(logList.get(),"-----------------------btnSplitPaymentMouseClick() called-----------------------------");
	try
	{
		if (SelectedItems.empty())
		{
			MessageBox("Nothing selected to pay!", "Error", MB_OK + MB_ICONERROR);
		}
		else
		{
			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
            TPaymentTransaction PaymentTransaction(DBTransaction);
            bool ispaymentComplete = false;
			bool Proceed = TabStaffAccessOk(DBTransaction);
			DBTransaction.Commit();
			int splittedItemKey = 0;
			if (Proceed)
			{
				DBTransaction.StartTransaction();

                if(TGlobalSettings::Instance().LoyaltyMateEnabled && (CurrentDisplayMode == eTabs || CurrentDisplayMode == eTables))
                    DownloadOnlineMember();

				std::set <__int64> SelectedItemKeys;
				std::set <__int64> BackupOfSelectedTabKeys;
				BackupOfSelectedTabKeys = SelectedTabs;
				int BackupCurrentSelectedTab = CurrentSelectedTab;
				Proceed = false;
				bool StuffToBill = false;
				std::set <__int64> TabsToLock;
				for (std::set <__int64> ::iterator CrntTabKey = SelectedTabs.begin(); CrntTabKey != SelectedTabs.end(); advance(CrntTabKey, 1))
				{
					TabsToLock.insert(*CrntTabKey);
					if (!TDBTab::GetIsEmpty(DBTransaction, *CrntTabKey))
					{
						StuffToBill = true;
					}
				}

				if (StuffToBill)
				{
					if (!TDBTab::LockTabs(DBTransaction, TabsToLock, TDeviceRealTerminal::Instance().ID.Name))
					{
						if (MessageBox("There are seats/tabs that are in use by other terminals."
								"\rDo you wish to override these locks on these seats/tabs?", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
						{
							TDBTab::LockTabs(DBTransaction, TabsToLock, TDeviceRealTerminal::Instance().ID.Name, true);
							Proceed = true;
						}
						else
						{
							MessageBox("You can Bill off the unlocked seats individually.", "Infomation", MB_OK + MB_ICONINFORMATION);
						}
					}
					else
					{
						Proceed = true;
					}

					if (Proceed)
					{
						for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
							advance(itItem, 1))
						{
							SelectedItemKeys.insert(itItem->first);
						}
                        if(TGlobalSettings::Instance().HideFreeSides)
                        MergeZeroPriceSideKeysWithSelectedItemKeys(SelectedItemKeys); //Merging the Item keys of Zero Price Sides with Selected Item Keys
						PatronCount = DeterminePatronCount();
						splittedItemKey = BillItems(PaymentTransaction, DBTransaction, SelectedItemKeys, eTransSplitPayment,ispaymentComplete);

					}
				}
				else
				{
					MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
				}

				TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
				DBTransaction.Commit();
                if(ispaymentComplete && CurrentDisplayMode != eInvoices)
                    SendFiscalPrint(PaymentTransaction);
                ResetTransactionAfterCommit(PaymentTransaction);
				ResetForm();
                RecordFiscalLogsPaymentSystem(logList.get(),"-Transaction commit in btnSplitPaymentMouseClick()");
				DBTransaction.StartTransaction();
				// Now reslect all the remaining items.
				if(splittedItemKey > 0)
					SelectedItemKeys.insert(splittedItemKey);
                if(VoucherCode != "" && TGlobalSettings::Instance().LoyaltyMateEnabled)
                {
                    RemoveLoyaltymateMembership(SelectedItemKeys);
                }
				TDBOrder::LoadPickNMixOrders(DBTransaction, SelectedItemKeys, SelectedItems);

				int LastTabInSelectionGroup = 0;
				for (int i = 0; i < TabList->Count; i++)
				{
					if (BackupOfSelectedTabKeys.find((int)TabList->Objects[i]) != BackupOfSelectedTabKeys.end())
					{
						SelectedTabs.insert((int)TabList->Objects[i]);
						LastTabInSelectionGroup = (int)TabList->Objects[i];
					}
					if ((int)TabList->Objects[i] == BackupCurrentSelectedTab)
					{
						CurrentSelectedTab = BackupCurrentSelectedTab;
					}
				}

				if (CurrentSelectedTab == 0)
				{
					CurrentSelectedTab = LastTabInSelectionGroup;
				}
				UpdateItemListDisplay(DBTransaction);
                if(TGlobalSettings::Instance().IsBillSplittedByMenuType && CurrentDisplayMode == eTables)
                {
                    DisableBillEntireTable(DBTransaction);
                }
				UpdateContainerListColourDisplay();
				DBTransaction.Commit();
				ShowReceipt();

                if(CurrentDisplayMode == eTables)
                    UpdateTabeleStateForOO();
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);

		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        RecordFiscalLogsPaymentSystem(logList.get(),"-Exception in btnSplitPaymentMouseClick()");
	}
}
//----------------------------------------------------------------------------
void TfrmBillGroup::RemoveLoyaltymateMembership(std::set <__int64> SelectedItemKeys)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    try
    {
        TPaymentTransaction ReceiptTransaction(DBTransaction);
        TDBOrder::GetOrdersFromOrderKeys(DBTransaction, ReceiptTransaction.Orders, SelectedItemKeys);
        for(int i = 0; i < ReceiptTransaction.Orders->Count; i++)
        {
            TItemComplete *Order = (TItemComplete*)(ReceiptTransaction.Orders->Items[i]);
            for(int k = 0; k < Order->Discounts.size(); k++)
            {
                if(Order->Discounts[k].DiscountCode == VoucherCode)
                {
                    Order->DiscountRemove(Order->Discounts[k]);
                }
            }
        }
        TManagerDiscount managerDiscount;
        managerDiscount.SetDiscountAmountDB(DBTransaction, ReceiptTransaction.Orders);
        ClearLoyaltyVoucher();
        VoucherCode = "";

        DBTransaction.Commit();
    }
	catch(Exception & E)
	{
        DBTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnTransferMouseClick(TObject *Sender)
{
	bool Proceed = true;
	if (SelectedTabs.empty() && CurrentDisplayMode != eTables)
	{
		if (CurrentSelectedTab > 0)
		{
			SelectedTabs.insert(CurrentSelectedTab);
		}
		else
		{
			Proceed = false;
			if (CurrentTabType == TabMember)
			{
				MessageBox("Please Select a Member!", "Error", MB_OK + MB_ICONERROR);
			}
			else
			{
				MessageBox("Please Select a Tab!", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
	else if (SelectedTabs.size() > 1 && CurrentDisplayMode == eTabs && CurrentTabType == TabMember)
	{
		MessageBox("You can only transfer a single member tab.", "Error", MB_OK + MB_ICONERROR);
		Proceed = false;
	}

	if (Proceed)
	{
        TfrmTransfer* Transfer = new TfrmTransfer(this, DBControl);
		Transfer->CurrentSourceDisplayMode = CurrentDisplayMode;
		Transfer->CurrentSourceTabType = CurrentTabType;
		Transfer->CurrentSourceTable = CurrentTable;
		Transfer->CurrentSourceRoom = CurrentRoom;
        Transfer->SelectedTabKey = CurrentSelectedTab;
        if (!SelectedTabs.empty())
        {
        	Transfer->PreSelectedTab = *SelectedTabs.begin();
        }

		if (CurrentTabType == TabMember || CurrentTabType == TabStaff)
		{
			std::set <__int64> ::iterator CrntTabKey = SelectedTabs.begin();
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			Staff->GetContactDetails(DBTransaction, TDBTab::GetTabOwner(DBTransaction, *CrntTabKey), TempUserInfo);
			DBTransaction.Commit();
		}
		Transfer->TempSourceUserInfo = TempUserInfo;
        Database::TDBTransaction DBTransaction(DBControl);
        Transfer->DBTransaction = &DBTransaction;
        if(Transfer->ShowModal() != mrOk)
        {
           DBTransaction.Rollback();
        }
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		ResetSelection();
        if(TGlobalSettings::Instance().LoyaltyMateEnabled && SelectedItems.size() == 0)
        {
            CheckLoyalty();
        }
		UpdateSeatDetails(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
		DBTransaction.Commit();
        delete Transfer;
        TGlobalSettings::Instance().IsPOSOffline = true;
		ShowReceipt();
	}
}
//----------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnCloseMouseClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
       ClearLoyaltyVoucher();
    }
    Close();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnClearAllMouseClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	for (int i = 0; i < tgridContainerList->RowCount; i++)
	{
		TGridButton *GridButton = tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN];
		int SelectedTab = GridButton->Tag;
		ItemSetRemoveItems(DBTransaction, SelectedTab);
		CurrentSelectedTab = SelectedTab;
		// Unlock this Tab.
		TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, SelectedTab);
	}
	CurrentSelectedTab = 0;

	UpdateRightButtonDisplay(Sender);
	IgnoreItemThreshhold = false;
	UpdateItemListDisplay(DBTransaction);
    if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
    {
        if(CurrentDisplayMode == eTables)
        {
            DisableBillEntireTable(DBTransaction);
        }
        DisableToggleGSTButton(DBTransaction);
    }
	UpdateContainerListColourDisplay();
    UpdateTableForOnlineOrdering();
    DisableTransferButtonWhenLMIsEnabled();
	DBTransaction.Commit();
	CheckLoyalty();
    ClearLoyaltyVoucher();
	ShowReceipt();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnSelectAllMouseClick(TObject *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();

	for (int i = 0; i < tgridContainerList->RowCount; i++)
	{
		TGridButton *GridButton = tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN];
		int SelectedTab = GridButton->Tag;
        if(CheckTabCompatablityForOnlineOrdering(SelectedTab) )
        {
            if (AddToSelectedTabs(DBTransaction, SelectedTab))
            {
                ItemSetAddItems(DBTransaction, SelectedTab);
                CurrentSelectedTab = SelectedTab;
            }
            else
            {
                if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney )
                {
                    if(i > 1)
                        break;
                }
            }
        }
	}
	UpdateRightButtonDisplay(Sender);
	IgnoreItemThreshhold = false;
	UpdateItemListDisplay(DBTransaction);
    if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
    {
        if(CurrentDisplayMode == eTables)
        {
            DisableBillEntireTable(DBTransaction);
        }
        DisableToggleGSTButton(DBTransaction);
    }
	UpdateContainerListColourDisplay();

    if(CurrentDisplayMode == eTables)
    {
        UpdateTableForOnlineOrdering();
    }
    else if(CurrentDisplayMode == eTabs)
        UpdateTabForOnlineOrdering();

    DisableTransferButtonWhenLMIsEnabled();
    if(lbeMembership->Visible == false)//todo-Arpit
    {
      RemoveMembershipDiscounts(DBTransaction);
    }
	DBTransaction.Commit();
    if(!TGlobalSettings::Instance().IsThorlinkSelected)
    {
        if((TGlobalSettings::Instance().LoyaltyMateEnabled && SelectedItems.size() == 0)
            || !TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
	        CheckLoyalty();
        }
    }
	ShowReceipt();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnScrollBillUpAutoRepeat(TObject *Sender)
{
	Memo1->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnScrollBillDownAutoRepeat(TObject *Sender)
{
	Memo1->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnMoveMouseClick(TObject *Sender)
{
	try
	{
		TMMContactInfo TempUserInfo;
		TempUserInfo = TDeviceRealTerminal::Instance().User;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

		bool Procced = TabStaffAccessOk(DBTransaction);
		DBTransaction.Commit();

		if (Procced)
		{
			IgnoreItemThreshhold = true;
			if (!SelectedItems.empty())
			{
                int source_key = 0;
				Database::TDBTransaction DBTransaction(DBControl);
				TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
				DBTransaction.StartTransaction();

				std::auto_ptr <TfrmDropDownVar> DropDown(TfrmDropDownVar::Create <TfrmDropDownVar> (this));
				for (int i = 0; i < TabList->Count; i++)
				{
					DropDown->AddButton(TddvBtn(TabList->Strings[i], (int)TabList->Objects[i]));
				}

				if (DropDown->ShowModal() == mrOk)
				{
					int TabTransferTo = DropDown->Result;
                    int sourceAccNumber = 0;
					std::auto_ptr <TList> OrdersList(new TList);
					std::set <__int64> ItemsToBeMoved;
					for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
						advance(itItem, 1))
					{
						ItemsToBeMoved.insert(itItem->first);
                        source_key = itItem->first;
                        sourceAccNumber = atoi(itItem->second.AccNumber.t_str());
					}

                    if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot &&
                       TGlobalSettings::Instance().EnableCustomerJourney )
                    {
                        if(!CanMoveItemToOtherTab(sourceAccNumber, TabTransferTo))
                        {
                            MessageBox("Destination has items for different SiHot account number","Error", MB_OK + MB_ICONERROR);
                            DBTransaction.Commit();
                            return;
                        }
                    }

					TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), ItemsToBeMoved);
                    if(!CheckIfMembershipUpdateRequired(DBTransaction, source_key, TabTransferTo))
                        return;
					if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), TabTransferTo))
					{
						TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), TabTransferTo, TDeviceRealTerminal::Instance().User.ContactKey, source_key);
            		}
					else
					{
						MessageBox("Insufficient credit on destination Tab.", "Warning", MB_OK + MB_ICONWARNING);
					}

					while (OrdersList->Count != 0)
					{
						TItemComplete *Order = (TItemComplete*)OrdersList->Items[0];
						OrdersList->Remove(Order);
						delete Order;
					}
				}
				SelectedItems.clear();
                if(TGlobalSettings::Instance().LoyaltyMateEnabled && SelectedItems.size() == 0)
                {
                    CheckLoyalty();
                }
				UpdateTableDetails(DBTransaction);
				UpdateSeatDetails(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());

				DBTransaction.Commit();
				ShowReceipt();
			}
			else
			{
				MessageBox("Please Select an Item(s) to Move.", "Warning", MB_OK + MB_ICONWARNING);
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to Move these Items.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
//----------------------------------------------------------------------------
bool TfrmBillGroup::CanMoveItemToOtherTab(int sourceAccNumber, int TabTransferTo)
{
    bool retValue = true;
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try{
        TIBSQL *IBInternalQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT ACC_NO FROM ORDERS WHERE TAB_KEY = :TAB_KEY AND ACC_NO <> :ACC_NO";
        IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabTransferTo;
        IBInternalQuery->ParamByName("ACC_NO")->AsInteger = sourceAccNumber;
        IBInternalQuery->ExecQuery();
        if(IBInternalQuery->RecordCount > 0)
           retValue = false;
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
    return retValue;
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnSplitMouseClick(TObject *Sender)
{
	IgnoreItemThreshhold = true;
	try
	{
		std::set <__int64> ItemsToBeSplit;
		for (std::map<__int64, TPnMOrder>::const_iterator itItem = \
		       SelectedItems.begin(); itItem != SelectedItems.end(); ++itItem)
		{
			if (!itItem->second.IsSide)
				ItemsToBeSplit.insert(itItem->first);
		}

		int WaysToSplit = 1;
		std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
		frmTouchNumpad->Mode = pmNumber;
		frmTouchNumpad->Caption = "How many ways do you wish to split the Item(s)?";
		frmTouchNumpad->btnSurcharge->Caption = "Ok";
		frmTouchNumpad->btnDiscount->Visible = false;
		frmTouchNumpad->btnSurcharge->Visible = true;
		frmTouchNumpad->INTInitial = 1;
		if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->INTResult > 0)
		{
			WaysToSplit = frmTouchNumpad->INTResult;
			int SplitCount = 0;
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			for (std::set <__int64> ::iterator pItemKey = ItemsToBeSplit.begin(); pItemKey != ItemsToBeSplit.end(); advance(pItemKey, 1))
			{
				SplitCount = TDBOrder::SplitOrder(DBTransaction, *pItemKey, WaysToSplit);
				if (SplitCount == 0)
				{
					MessageBox("This Item was unable to be split any further", "Information", MB_OK + MB_ICONINFORMATION);
				}
				else if (SplitCount != WaysToSplit)
				{
					MessageBox("This Item was split the maxium of " + IntToStr(SplitCount) + " ways", "Information", MB_OK + MB_ICONINFORMATION);
				}
			}
			UpdateItemListDisplay(DBTransaction);
            if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
            {
                if(CurrentDisplayMode == eTables)
                {
                    DisableBillEntireTable(DBTransaction);
                }

                DisableToggleGSTButton(DBTransaction);
            }
			DBTransaction.Commit();
			ShowReceipt();
            EnableButtons();
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to split this Item.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
    applyWaiterStationSettingsIfEnabled();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnCancelMouseClick(TObject *Sender)
{
	try
	{
		TMMContactInfo TempUserInfo;
		TempUserInfo = TDeviceRealTerminal::Instance().User;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));

		bool Procced = TabStaffAccessOk(DBTransaction);
		DBTransaction.Commit();

		if (Procced)
		{
			bool Allow = Staff->TestAccessLevel(TempUserInfo, CheckCredit);
			if (!Allow)
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();

				TempUserInfo.Clear();
				TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckCredit);
				if (Result == lsAccepted)
				{
					Allow = true;
				}
				else if (Result == lsDenied)
				{
					MessageBox("You do not have access rights to Cancels.", "Error", MB_OK + MB_ICONERROR);
				}
				else if (Result == lsPINIncorrect)
				{
					MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
				}
				DBTransaction.Commit();
			}

			if (Allow)
			{
				IgnoreItemThreshhold = true;
				if (!SelectedItems.empty())
				{
					Database::TDBTransaction DBTransaction(DBControl);
					DBTransaction.StartTransaction();

					std::set <__int64> ItemsToBeCanceled;
					for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
						advance(itItem, 1))
					{
						ItemsToBeCanceled.insert(itItem->first);
					}

					CancelItems(DBTransaction, ItemsToBeCanceled, TempUserInfo);
					UpdateItemListDisplay(DBTransaction);
                    if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
                    {
                        if(CurrentDisplayMode == eTables)
                        {
                            DisableBillEntireTable(DBTransaction);
                        }
                        DisableToggleGSTButton(DBTransaction);
                    }
					ShowReceipt();
				}
				else
				{
					MessageBox("Please Select an Item to Cancel.", "Warning", MB_OK + MB_ICONWARNING);
				}
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to Cancel this Single Item.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnSelectZoneMouseClick(TObject *Sender)
{
	if (SelectedZone() != eNoDisplayMode)//SelectedZone();
    {
        TGlobalSettings::Instance().IsPOSOffline = true;
        applyWaiterStationSettingsIfEnabled();
        if(TGlobalSettings::Instance().IsBillSplittedByMenuType && CurrentDisplayMode == eTables)
        {
           ChangeBillEntireTableState();
        }
        UpdateTableForOnlineOrdering();
        DisableTransferButtonWhenLMIsEnabled();
    }
    else
    {
       UpdateContainerList();
    }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::CardSwipe(Messages::TMessage& Message)
{    Membership.Member.Clear();
    TGlobalSettings::Instance().IsDiscountSelected = false;
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	try
	{
        if (CurrentTabType == TabWeb)
        {
            MessageBox("Membership Can not be applied on Web Order billing" , "Warning", MB_OK + MB_ICONWARNING);
            btnTransfer->Color = clSilver;
            btnTransfer->Enabled = false;
            btnApplyMembership->Enabled = false;
        }
        else if(TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            if((CurrentDisplayMode == eTabs && TDBTab::HasOnlineOrders(CurrentSelectedTab)) ||
                (CurrentDisplayMode == eTables && TDBTables::HasOnlineOrders(CurrentTable)))
            {
                MessageBox("Membership already applied on this online order.","Info",MB_OK+MB_ICONINFORMATION);
                return;
            }
            AnsiString Data = *((AnsiString*)Message.WParam);
            GetMemberByBarcode(DBTransaction,Data);
        }
        else
        {
            TMMContactInfo TempUserInfo;
            AnsiString Data = *((AnsiString*)Message.WParam);
            TempUserInfo.CardStr = Data.SubString(1, 80);
            TempUserInfo.ProxStr = Data.SubString(1, 80);
            TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->FindMember(DBTransaction, TempUserInfo);
            if (Result == lsAccepted)
            {
                ApplyMembership(DBTransaction, TempUserInfo);
            }
            else if (Result == lsAccountBlocked)
            {
                MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
            }
	    }
        DBTransaction.Commit();
        ShowReceipt();
    }
	catch(Exception & E)
	{
		MessageBox("Unable to Apply Membership :" + E.Message, "Error", MB_OK + MB_ICONERROR);
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::GetMemberByBarcode(Database::TDBTransaction &DBTransaction,AnsiString Barcode)
{
 	TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
	TMMContactInfo info;
    bool memberExist = drt.ManagerMembership->LoyaltyMemberSelected(DBTransaction,info,Barcode,true);

	if (info.Valid())
     {
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,info);

        if(TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            SetLoyaltyMemberInfo(DBTransaction,info);
            DownloadOnlineMember();

        }
        ApplyMembership(DBTransaction, info);
        ShowReceipt();
	}

}
// ---------------------------------------------------------------------------
void TfrmBillGroup::GetLoyaltyMember(Database::TDBTransaction &DBTransaction, TMMContactInfo &Info)
{
     eMemberSource MemberSource = emsManual;
     TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, Info, MemberSource);

     if (Result == lsAccountBlocked)
     {
            MessageBox("Account Blocked " + Info.Name + " " + Info.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
     }
     else if (Result == lsAccepted)
     {
          if(Info.ContactKey != 0)
          {
            TDeviceRealTerminal &drt = TDeviceRealTerminal::Instance();
            bool memberExist = drt.ManagerMembership->LoyaltyMemberSelected(DBTransaction,Info,Info.MemberCode,false);

            if (Info.Valid())
             {
                TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
                ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,Info);
                ApplyMembership(DBTransaction, Info);
             }
          }
     }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnApplyMembershipMouseClick(TObject *Sender)
{
     //mm-5145
    if(TGlobalSettings::Instance().MandatoryMembershipCard  )
    {
      btnApplyMembership->ButtonColor = clRed;
    }
    else
    {
        btnApplyMembership->ButtonColor = clPurple;    //mm-5145

//        if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
//        {
            if (SelectedTabs.empty() && CurrentDisplayMode == eTabs && CurrentTabType == TabMember)
            {
                MessageBox("No Tabs selected to Apply!", "Error", MB_OK + MB_ICONERROR);
            }
            else if (SelectedTabs.size() > 1 && CurrentDisplayMode == eTabs && CurrentTabType == TabMember)
            {
                MessageBox("You can apply Loyalty a single Membership tab.", "Error", MB_OK + MB_ICONERROR);
            }
            else // Adding loyalty to a serices of Tabs.
            {
                if (SelectedTabs.empty())
                {
                    MessageBox("Nothing selected to add Loyalty to!", "Error", MB_OK + MB_ICONERROR);
                }
                else
                { // Add Loyalty to Tabs Orders.
                    try
                    {
                        Database::TDBTransaction DBTransaction(DBControl);
                        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
                        DBTransaction.StartTransaction();
                        TMMContactInfo TempMembershipInfo;
                        TempMembershipInfo.Clear();
                        eMemberSource MemberSource;
                        TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempMembershipInfo,MemberSource);
                        if (Result == lsAccepted)
                        {
                            TGlobalSettings::Instance().IsDiscountSelected = false;
                            if(TGlobalSettings::Instance().LoyaltyMateEnabled)
                            {
                                GetLoyaltyMember(DBTransaction,TempMembershipInfo);
                                SetLoyaltyMemberInfo(DBTransaction,TempMembershipInfo);
                            }
                            else
                            {
                                ApplyMembership(DBTransaction, TempMembershipInfo);
                            }
                            if(!TGlobalSettings::Instance().LoyaltyMateEnabled)
                            {
                                for(std::set <__int64> ::iterator CrntTabKey = SelectedTabs.begin();
                                    CrntTabKey != SelectedTabs.end(); advance(CrntTabKey, 1))
                                {
                                    TDBTab::SetTabOrdersLoyalty(DBTransaction,*CrntTabKey, TempMembershipInfo.ContactKey);
                                }
                            }
                            else
                            {
                                TDBTab::SetTabOrdersLoyalty(DBTransaction,CurrentSelectedTab, TempMembershipInfo.ContactKey);
                            }
                         }
                         else if (Result == lsAccountBlocked)
                         {
                            MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked",
                                MB_OK + MB_ICONINFORMATION);
                         }
                         else if (Result == lsCancel)
                         {
                            RemoveMembership(DBTransaction);
                         }
                         DBTransaction.Commit();
                         ShowReceipt();
				}
				catch(Exception & E)
				{
					MessageBox("Unable to Apply Membership :" + E.Message, "Error", MB_OK + MB_ICONERROR);
				}
			}
		}
//	}
//	else
//	{
//		MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
//	}

   }
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnDiscountMouseClick(TObject *Sender)
{
	try
	{
        TGlobalSettings::Instance().IsDiscountSelected = true;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
        if((Membership.Member.ContactKey != 0) && TPaySubsUtility::IsLocalLoyalty() && !Membership.Member.Points.PointsRulesSubs.Contains(eprAllowDiscounts))
        {
            MessageBox("Discounts are disabled for this Member.", "INFORMATION", MB_OK + MB_ICONINFORMATION);
            return;
        }
		TMMContactInfo TempUserInfo;
		TempUserInfo = TDeviceRealTerminal::Instance().User;
		bool AllowDiscount = false;
		AnsiString DiscountMenu = "";
		if (!AllowDiscount)
		{
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckDiscountBill);
			if (Result == lsAccepted)
			{
				AllowDiscount = true;
			}
			else if (Result == lsDenied)
			{
				MessageBox("You do not have access rights to Discounts / Surcharges.", "Error", MB_OK + MB_ICONERROR);
			}
			else if (Result == lsPINIncorrect)
			{
				MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			}

		}

		if (AllowDiscount)
		{
			std::auto_ptr <TfrmMessage> SelectDiscount(TfrmMessage::Create <TfrmMessage> (this, DBControl));
			SelectDiscount->MessageType = eDiscountReason;
            SelectDiscount->ShowPointsAsDiscount = false;
            bool isSCDAppliedOnClipp = false;
            bool isPWDAppliedOnClipp = false;
			if (SelectDiscount->ShowModal() == mrOk)
			{
                ManagerDiscount->GetDiscount(DBTransaction, SelectDiscount->Key, SelectedDiscount);
				std::set <__int64> OrderKeySet;
				for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
				{
					OrderKeySet.insert(itItem->first);
				}
				if (SelectDiscount->Key == -1)
				{
					if (OrderKeySet.size())
					{
						TPaymentTransaction PaymentTransaction(DBTransaction);
						TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, OrderKeySet);
                        ManagerDiscount->ClearDiscounts(PaymentTransaction.Orders);
						ManagerDiscount->SetDiscountAmountDB(DBTransaction, PaymentTransaction.Orders);
                        PaymentTransaction.IgnoreLoyaltyKey = false;
						PaymentTransaction.Recalc();
                  		PaymentTransaction.DeleteOrders();
					}
				}
				else
				{


                if(SelectedDiscount.IsComplimentaryDiscount() && !TGlobalSettings::Instance().IsRegistrationVerified)
                  {
                          MessageBox("Complimentary Discount cannot be applied on unregistered POS","Error",MB_OK + MB_ICONERROR);
                          return;
                  }
                 else if(SelectedDiscount.IsNonChargableDiscount() && !TGlobalSettings::Instance().IsRegistrationVerified)
                 {

                         MessageBox("Non-Chargeable Discount cannot be applied on unregistered POS","Error",MB_OK + MB_ICONERROR);
                         return;
                 }
                    bool applyDiscount = true;
					if (OrderKeySet.size())
					{
						TPaymentTransaction PaymentTransaction(DBTransaction);
						TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, OrderKeySet);
						TDiscount CurrentDiscount;
                        TSCDPWDChecker SCDChecker;
						CurrentDiscount.DiscountKey = SelectDiscount->Key;
						ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

//                        applyDiscount = SCDChecker.SeniorCitizensCheck(CurrentDiscount, PaymentTransaction.Orders)  &&
//                                        SCDChecker.PWDCheck(CurrentDiscount, PaymentTransaction.Orders);
                        std::auto_ptr<TSCDPatronUtility> patronUtility(new TSCDPatronUtility());
                        if(patronUtility->CanByPassSCDValidity(PaymentTransaction.Orders,PatronTypes,CurrentDiscount))
                        {
                            applyDiscount = SCDChecker.PWDCheck(CurrentDiscount, PaymentTransaction.Orders);
                        }
                        else
                        {
                            applyDiscount = SCDChecker.SeniorCitizensCheck(CurrentDiscount, PaymentTransaction.Orders) &&
                                                   SCDChecker.PWDCheck(CurrentDiscount, PaymentTransaction.Orders);
                        }
						PaymentTransaction.DeleteOrders();
                        if(applyDiscount)
                        {
                            ApplyDiscount(DBTransaction, SelectDiscount->Key, dsMMUser);
                        }
					}


				}
                if(isSCDAppliedOnClipp )
                {
                     MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                     return;
                }
                else if(isPWDAppliedOnClipp)
                {
                     MessageBox("Order with SCD Discount can't be saved to clipp Tab.", "Error", MB_OK + MB_ICONERROR);
                     return;
                }
			}
		}
		DBTransaction.Commit();
        //Bill the table if user selects complimentary or NonChargable Discount
        if((SelectedDiscount.IsComplimentaryDiscount() || SelectedDiscount.IsNonChargableDiscount()) && AllowDiscount)
          {
            if(CheckDiscountPointsBillGroup)
            {
              CheckDiscountPointsBillGroup = false;
            }
            else
            {
              btnBillSelectedMouseClick(NULL);
            }
          }
		if (AllowDiscount)
		  {
			ShowReceipt();
		  }
	}
	catch(Exception & E)
	{
		MessageBox("Unable to discount this Item.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::ApplyDiscount(Database::TDBTransaction &DBTransaction, int DiscountKey, TDiscountSource DiscountSource)
{
	bool ProcessDiscount = true;
	TDiscount CurrentDiscount;
	bool bailout = false;
	CurrentDiscount.DiscountKey = DiscountKey;
	ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);

    if(DiscountSource == dsMMMembership)
    {
       CurrentDiscount.IsThorBill = false;
    }

    if(CurrentDiscount.Source == dsMMUser)
    {
        CurrentDiscount.Source = DiscountSource;
    }
    else if(CurrentDiscount.Source == dsMMMebersPoints)
    {
        if(Membership.Applied())
        {
            CurrentDiscount.Amount = Membership.Member.Points.getPointsBalance(ptstLoyalty);
            CurrentDiscount.MaximumValue = Membership.Member.Points.getPointsBalance(pasDatabase,ptstLoyalty);
        }
        else
        {
            ProcessDiscount = false;
        }
    }

	if (CurrentDiscount.Type == dtPromptDescription || CurrentDiscount.Type == dtPromptDescriptionAmount)
	{
		std::auto_ptr <TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create <TfrmTouchKeyboard> (this));
		do
		{
			frmTouchKeyboard->MaxLength = 200;
			frmTouchKeyboard->AllowCarriageReturn = false;
			frmTouchKeyboard->StartWithShiftDown = true;
			frmTouchKeyboard->MustHaveValue = true;
			frmTouchKeyboard->KeyboardText = CurrentDiscount.Description;
			frmTouchKeyboard->Caption = "Please enter a discount / surcharge Description";
			if (frmTouchKeyboard->ShowModal() == mrOk)
			{
				CurrentDiscount.Description = frmTouchKeyboard->KeyboardText;
			}
			else
			{
				bailout = true;
                CheckDiscountPointsBillGroup = true;      //MM-3908
			}
		}
		while (frmTouchKeyboard->KeyboardText == "" && !bailout);

		if (bailout)
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount && (CurrentDiscount.Type == dtPromptAmount || CurrentDiscount.Type == dtPromptDescriptionAmount))
	{
		std::auto_ptr <TfrmDiscount> frmDiscount(TfrmDiscount::Create <TfrmDiscount> (this));
		frmDiscount->Mode = CurrentDiscount.Mode;
		frmDiscount->CURInitial = CurrentDiscount.Amount;
		frmDiscount->PERCInitial = CurrentDiscount.PercentAmount;
		// Partial Payments.

		if (frmDiscount->ShowModal() == mrOk)
		{
			CurrentDiscount.Mode = frmDiscount->Mode;
			if (frmDiscount->Mode == DiscModeCurrency || frmDiscount->Mode == DiscModePoints)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else if (frmDiscount->Mode == DiscModeSetPrice)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else if (frmDiscount->Mode == DiscModeCombo)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
            if (frmDiscount->Mode == DiscModeItem)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
                CurrentDiscount.OriginalAmount = CurrentDiscount.Amount;
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else
			{
				CurrentDiscount.PercentAmount = frmDiscount->PERCResult;
                if(frmDiscount->Mode == DiscModePercent)
                {
                   CurrentDiscount.OriginalAmount = CurrentDiscount.PercentAmount;
                }
			}
		}
		else
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount)
	{
      CurrentDiscount.DiscountAppliedTime = Now();
      std::set <__int64> SelectedItemKeys;
      for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
      {
         SelectedItemKeys.insert(itItem->first);
      }

      TPaymentTransaction PaymentTransaction(DBTransaction);
      TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, SelectedItemKeys);
      ManagerDiscount->ClearDiscount(PaymentTransaction.Orders, CurrentDiscount); // clear already applied discount from saved sales...
      //ManagerDiscount->AddDiscount(PaymentTransaction.Orders, CurrentDiscount);
      ApplyDiscountWithRestructure(PaymentTransaction, CurrentDiscount);
   	  PaymentTransaction.ApplyMembership(Membership);
      ManagerDiscount->SetDiscountAmountDB(DBTransaction, PaymentTransaction.Orders);
      PaymentTransaction.DeleteOrders();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnShowItemsMouseClick(TObject *Sender)
{
	IgnoreItemThreshhold = !IgnoreItemThreshhold;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	UpdateItemListDisplay(DBTransaction);
    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && CurrentDisplayMode == eTables)
    {
        DisableBillEntireTable(DBTransaction);
    }
	DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tgridContainerListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift,
	TGridButton *GridButton)
{
    TMMProcessingState State(Screen->ActiveForm, "Loading Items Please Wait...", "Loading Items");
    TDeviceRealTerminal::Instance().ProcessingController.Push(State);
    try
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        CurrentSelectedTab = 0;
        int SelectedTab = GridButton->Tag;

        if (tgridContainerList->Col(GridButton) == CONTAINER_LIST_TAB_COLUMN)
        {
            // Find the old Focused button and set it to just Selected
            if(CheckTabCompatablityForOnlineOrdering(SelectedTab))
            {
                CurrentSelectedTab = SelectedTab;
                SplitItemsInSet(DBTransaction, SelectedTab);
            }
        }
        else if (tgridContainerList->Col(GridButton) == CONTAINER_LIST_FUNC_COLUMN)
        {
            if (GridButton->Color == ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR])
            {
                if (SelectedTab == -1)         /* Clicked on the All guests button, when table payments */
                {
                    for (int i = 0; i < TabList->Count; i++)
                    {
                        ItemSetRemoveItems(DBTransaction, (int)TabList->Objects[i]);
                    }
                    CurrentSelectedTab = SelectedTab;
                }
                else
                {
                    ItemSetRemoveItems(DBTransaction, SelectedTab);
                    CurrentSelectedTab = SelectedTab;
                }
                // Unlock this Tab.
                TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, SelectedTab);
            }
            else
            {
                if (SelectedTab == -1)      /* TODO : remove unused if condition */
                {
                    for (int i = 0; i < TabList->Count; i++)
                    {
                        const int tab_key =reinterpret_cast<int>(TabList->Objects[i]);

                        if (AddToSelectedTabs(DBTransaction,tab_key)== true )
                        {
                            SplitItemsInSet(DBTransaction,tab_key);
                            ItemSetAddItems(DBTransaction,tab_key);
                        }
                        else
                        {
                            if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney )
                                break;
                        }
                    }
                    CurrentSelectedTab = SelectedTab;
                }
                else
                {
                    //If current tab type is delayed payment
                    //then remove previous selected items
                    if(CurrentTabType == TabDelayedPayment)
                    {
                        ClearTabOnDelayedPaymentSelection(DBTransaction,SelectedTab);
                    }
                    if(CheckTabCompatablityForOnlineOrdering(SelectedTab))
                    {
                        if (AddToSelectedTabs(DBTransaction, SelectedTab))
                        {
                            SplitItemsInSet(DBTransaction, SelectedTab);
                            ItemSetAddItems(DBTransaction,SelectedTab);
                            CurrentSelectedTab = SelectedTab;
                            HasOnlineOrders = TDBTab::HasOnlineOrders(SelectedTab);
                            // Get online member.
                        }
                    }
                    else
                    {
                        TDeviceRealTerminal::Instance().ProcessingController.Pop();
                        return;
                    }
                }
            }
        }
        UpdateRightButtonDisplay(Sender);
        IgnoreItemThreshhold = false;
        UpdateItemListDisplay(DBTransaction);
        if(TGlobalSettings::Instance().IsBillSplittedByMenuType )
        {
            if(CurrentDisplayMode == eTables)
            {
                DisableBillEntireTable(DBTransaction);
            }
            DisableToggleGSTButton(DBTransaction);
        }
        UpdateContainerListColourDisplay();
        HasOnlineOrders = TDBTab::HasOnlineOrders(CurrentSelectedTab);
        UpdateTableForOnlineOrdering();
//        MessageBox(CurrentSelectedTab,"CurrentSelectedTab in tgridClick",MB_OK);
        UpdateTabForOnlineOrdering();
        DisableTransferButtonWhenLMIsEnabled();

        UpdateSplitButtonState();
         if(lbeMembership->Visible == false && Membership.Member.AutoAppliedDiscounts.size()>0) //todo-Arpit
        {
           RemoveMembershipDiscounts(DBTransaction);
        }
        DBTransaction.Commit();
        if(!TGlobalSettings::Instance().IsThorlinkSelected)
        {
            if((TGlobalSettings::Instance().LoyaltyMateEnabled && SelectedItems.size() == 0)
              || !TGlobalSettings::Instance().LoyaltyMateEnabled)
            {
                CheckLoyalty();
            }
        }
        ShowReceipt();
    }
    catch(Exception &E)
    {
      //
    }
    TDeviceRealTerminal::Instance().ProcessingController.Pop();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tgridItemListMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
    SplitTimer->Enabled = true;
    // Relatch button to resdisplay Split status.
	if (VisibleItems[GridButton->Tag].Qty != 1)
	{
		GridButton->Latched = true;
	}
	else
	{
		GridButton->Latched = false;
	}

	if (SelectedItems.find(GridButton->Tag) == SelectedItems.end())
	{ // Not Found add it.
        TSCDPWDChecker SCDChecker;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

        std::set <__int64> SelectedItemKeys;
        for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
        {
            SelectedItemKeys.insert(itItem->first);
        }

        bool canAddItem = SCDChecker.ItemSelectionCheck(DBTransaction, GridButton->Tag, SelectedItemKeys) &&
                          SCDChecker.ItemSelectionCheckPWD(DBTransaction, GridButton->Tag, SelectedItemKeys) ;

		if (canAddItem && AddToSelectedTabs(DBTransaction, VisibleItems[GridButton->Tag].TabKey))
		{
			SelectedItems[GridButton->Tag] = VisibleItems[GridButton->Tag];
			if (CurrentDisplayMode == eInvoices)
			{ // Must selected the Entire Invoice.
				SelectedItems.clear();
				SelectedItems = VisibleItems;
			}
			UpdateItemListColourDisplay();
		}
		DBTransaction.Commit();
	}
	else
	{
		bool TabContainsItems = false;
		SelectedItems.erase(GridButton->Tag);

		if (CurrentDisplayMode == eInvoices)
		{ // Must selected the Entire Invoice.
			SelectedItems.clear();
		}

		for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
		{
			if (itItem->second.TabKey == VisibleItems[GridButton->Tag].TabKey)
			{
				TabContainsItems = true;
			}
		}

		if (!TabContainsItems)
		{
			SelectedTabs.erase(VisibleItems[GridButton->Tag].TabKey);
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, VisibleItems[GridButton->Tag].TabKey);
			DBTransaction.Commit();
		}
		UpdateItemListColourDisplay();
	}
	// Reset the Split Payment Form.

	if (SelectedItems.size() == 0 /*&& Discounts.size() > 0*/)
	{
		if (MessageBox("Remove all Membership & Discounts from the bill?", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
		{
			// Discounts.clear();
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			RemoveMembership(DBTransaction);
  			DBTransaction.Commit();
		}
	}
 	frmSplitPayment->DivisionsLeft = 0;
	UpdateContainerListColourDisplay();
	UpdateSplitButtonState();
    if(!TGlobalSettings::Instance().IsThorlinkSelected)
    {
	    CheckLoyalty();
    }
	ShowReceipt();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::SplitTimerTick(TObject *Sender)
{
    SplitTimer->Enabled = false;
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Caption = "Enter Quantity";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->CURResult > 0)
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        SplitItem(DBTransaction,SelectedItemKey,frmTouchNumpad->CURResult);
        UpdateItemListDisplay(DBTransaction);
        DBTransaction.Commit();
    }
    SelectedItemKey = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tgridItemListMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
   if(TGlobalSettings::Instance().MergeSimilarItem)
   {
        SplitTimer->Enabled = true;
   }
   SelectedItemKey = GridButton->Tag;
}
//---------------------------------------------------------------------------
void TfrmBillGroup::ToggleItemState(TGridButton *GridButton)
{
    if (SelectedItems.find(GridButton->Tag) == SelectedItems.end())
    { // Not Found add it.
        SelectItem(GridButton);
    }
    else
    {
        if(HasOnlineOrders)
            MessageBox("To bill off Online ordering tab , all items must be selected.","Info",MB_OK+MB_ICONINFORMATION);
        else
            DeselectItem(GridButton);
    }

    if(TGlobalSettings::Instance().IsBillSplittedByMenuType )
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        DisableToggleGSTButton(DBTransaction);
        DBTransaction.Commit();
    }
}
//---------------------------------------------------------------------------
void TfrmBillGroup::SelectItem(TGridButton *GridButton)
{
    TSCDPWDChecker SCDChecker;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    std::set <__int64> SelectedItemKeys;
    TItemType itemType;
    for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
    {
        itemType = itItem->second.ItemType;
        SelectedItemKeys.insert(itItem->first);
    }

    bool canAddItem = SCDChecker.ItemSelectionCheckPWD(DBTransaction, GridButton->Tag, SelectedItemKeys);

    if (canAddItem && AddToSelectedTabs(DBTransaction, VisibleItems[GridButton->Tag].TabKey))
    {
        if((!TGlobalSettings::Instance().IsBillSplittedByMenuType) ||
                ((itemType == VisibleItems[GridButton->Tag].ItemType) && (SelectedItemKeys.size())) ||(!SelectedItemKeys.size())  )
        {
            SelectedItems[GridButton->Tag] = VisibleItems[GridButton->Tag];
        }
        else
        {
             MessageBox("Items with different menu types can't be selected at the same time.", "Error", MB_ICONWARNING + MB_OK);
        }

        if (CurrentDisplayMode == eInvoices)
        { // Must selected the Entire Invoice.
            SelectedItems.clear();
            SelectedItems = VisibleItems;
        }
        UpdateItemListColourDisplay();
    }
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmBillGroup::DeselectItem(TGridButton *GridButton)
{
    bool TabContainsItems = false;
    SelectedItems.erase(GridButton->Tag);
    if (CurrentDisplayMode == eInvoices)
    { // Must selected the Entire Invoice.
        SelectedItems.clear();
    }
    for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
    {
        if (itItem->second.TabKey == VisibleItems[GridButton->Tag].TabKey)
        {
            TabContainsItems = true;
        }
    }

    if (!TabContainsItems)
    {
        SelectedTabs.erase(VisibleItems[GridButton->Tag].TabKey);
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, VisibleItems[GridButton->Tag].TabKey);
        DBTransaction.Commit();
    }
    UpdateItemListColourDisplay();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tgridItemListMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
    SelectedItemKey = 0;
    SplitTimer->Enabled = false;
//    GridButton->Latched = (VisibleItems[GridButton->Tag].Qty  - (int)VisibleItems[GridButton->Tag].Qty) > 0;
    for(int i = 0; i < tgridItemList->RowCount; i++)
    {
        if(tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Tag == GridButton->Tag)
        {
           tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched =
                (VisibleItems[GridButton->Tag].Qty  - (int)VisibleItems[GridButton->Tag].Qty) > 0;
        }
    }
    if(GridButton->Caption == "Select")
    {
        ToggleItemState(GridButton);
    }
    else
    {
            if(SelectedItems.find(GridButton->Tag) != SelectedItems.end())
            {
                if(SelectedItems[GridButton->Tag].Qty > 1.0f && VisibleItems[GridButton->Tag].Type != CanceledOrder)
                {
                    if(!SelectedItems[GridButton->Tag].IsSide)
                        SplitItemOnClick(GridButton->Tag);
                    else
                        MessageBox("It is an ItemSide. Selected operation is not allowed", "Warning", MB_OK + MB_ICONWARNING);
                }
                else if((SelectedItems[GridButton->Tag].Qty <= 1.0f))
                    ToggleItemState(GridButton);
            }
            else
            {
                ToggleItemState(GridButton);
            }
    }
	// Reset the Split Payment Form.

	if (SelectedItems.size() == 0 /*&& Discounts.size() > 0*/)
	{
		if (MessageBox("Remove all Membership & Discounts from the bill?", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
		{
			// Discounts.clear();
			Database::TDBTransaction DBTransaction(DBControl);
			DBTransaction.StartTransaction();
			RemoveMembership(DBTransaction);
  			DBTransaction.Commit();
		}
	}
 	frmSplitPayment->DivisionsLeft = 0;
	UpdateContainerListColourDisplay();
	UpdateSplitButtonState();
    if(!TGlobalSettings::Instance().IsThorlinkSelected)
    {
        if((TGlobalSettings::Instance().LoyaltyMateEnabled && SelectedItems.size() == 0)
          || !TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
	        CheckLoyalty();
        }
    }
	ShowReceipt();
}
//---------------------------------------------------------------------------
void TfrmBillGroup::SplitItemOnClick(int itemSelected)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Caption = "Enter Quantity";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->View = viewQuantity;
    if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->splitValue > 0 &&
        SelectedItems[itemSelected].Qty > frmTouchNumpad->splitValue)
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        double qtyLeft = SelectedItems[itemSelected].Qty - frmTouchNumpad->splitValue;
        SplitItem(DBTransaction,itemSelected,qtyLeft);
        if(frmTouchNumpad->splitValue < SelectedItems[itemSelected].Qty)
           RefreshItemStatus(frmTouchNumpad->CURResult,itemSelected,DBTransaction);
        UpdateItemListDisplay(DBTransaction);
        DBTransaction.Commit();
    }
}
//----------------------------------------------------------------------------
void TfrmBillGroup::RefreshItemStatus(Currency splitValue,int itemSelected,Database::TDBTransaction &DBTransaction)
{
    int oldSize = VisibleItems.size();
    VisibleItems.clear();
    std::auto_ptr <TList> SortingList(new TList);
    if (CurrentSelectedTab == -1)
    {
        for (int i = 0; i < TabList->Count; i++)
        {
            TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction,(int)TabList->Objects[i],VisibleItems);
        }
    }
    else
    {
        TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction, CurrentSelectedTab, VisibleItems);
    }
    for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
    {
        SortingList->Add(&itItem->second);
    }
    tgridItemList->RowCount = VisibleItems.size();

    if(TGlobalSettings::Instance().IsBillSplittedByMenuType)
        oldSize = VisibleItems.size()-1;
    // Disable the button which was split
    for(int i = 0; i < SortingList->Count ; i++)
    {
        TPnMOrder *ptrItem1 = (TPnMOrder*)SortingList->Items[i];
        if(ptrItem1->Key == itemSelected)
        {
            AnsiString qtyStr = FormatFloat("0.00", ptrItem1->Qty) + " ";
            if(ptrItem1->Qty != 1)
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Caption = qtyStr + ptrItem1->Name;
            else
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Caption = ptrItem1->Name;
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched =
                                                         (ptrItem1->Qty  - (int)ptrItem1->Qty) > 0;
            tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
           SelectedItems[ptrItem1->Key] = VisibleItems[ptrItem1->Key];
            if(ptrItem1->IsParent)
            {
                TItemComplete* Order = new TItemComplete();
                Order->OrderKey = itemSelected;
                TDBOrder::GetOrderIncludingSidesFromOrderKey(DBTransaction,Order);
                for (int j = 1; j <= Order->SubOrders->Count ; j++)
                {
                    TItemCompleteSub *SubOrder = (TItemCompleteSub *)Order->SubOrders->Items[j-1];
                    SubOrder->SetQty(splitValue);
                    qtyStr = FormatFloat("0.00", SubOrder->GetQty()) + " ";
                    if(ptrItem1->Qty != 1)
                        tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Caption = qtyStr + SubOrder->Item;
                    else
                        tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Caption = SubOrder->Item;
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Latched =
                                                                 (SubOrder->GetQty()  - (int)ptrItem1->Qty) > 0;
                    tgridItemList->Buttons[i+j][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
                    SelectedItems[SubOrder->OrderKey] = VisibleItems[SubOrder->OrderKey];
                }
            }
            break;
        }
    }
    for(int i = oldSize ; i < VisibleItems.size();i++)
    {
        TPnMOrder *ptrItem = (TPnMOrder*)SortingList->Items[i];
        AnsiString QtyStr = "";
        if (ptrItem->Qty != 1)
        {
            QtyStr = FormatFloat("0.00", ptrItem->Qty) + " ";
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched = (ptrItem->Qty  - (int)ptrItem->Qty) > 0;
        }
        else
        {
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched = false;
        }
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Caption = QtyStr + ptrItem->Name;
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Tag = ptrItem->Key;
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Tag = ptrItem->Key;
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Caption = "Select";
        SelectedItems.erase(ptrItem->Key);
    }
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::SplitItemsInSet(Database::TDBTransaction &transaction, int tab_key)
{
	//TIBSQL *query = transaction.Query(transaction.AddQuery());
    if(!TGlobalSettings::Instance().MergeSimilarItem)
    {
        std::map<__int64, TPnMOrder> orders;
        double quantity = TDBOrder::LoadPickNMixOrdersAndGetQuantity(transaction, tab_key, orders);
        bool orderSplit = false;
        std::map<__int64, TPnMOrder>::iterator i = orders.begin();
        std::map<__int64, TPnMOrder>::iterator j = orders.end();

        for ( ; i != j; i++)
        {
            if ( (i->second.IsParent || !i->second.GroupNumber)
                    && !i->second.IsWeighted && i->second.Qty > 1)
            {
                TDBOrder::SplitOrder(transaction, i->second.Key, i->second.Qty);
                orderSplit = true;
            }
        }
    }
   //  delete query;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::SplitItem(Database::TDBTransaction &transaction, int orderKey,double quantityToSplit)
{
	TDBOrder::SplitOrderToQuantity(transaction,orderKey,quantityToSplit);
}
// ---------------------------------------------------------------------------
bool TfrmBillGroup::DoSplit(Database::TDBTransaction &transaction,std::map<__int64, TPnMOrder> orders)
{
    double accumulatedQuantity = 0;
    //TIBSQL *query = transaction.Query(transaction.AddQuery());
    std::map<__int64, TPnMOrder>::iterator i = orders.begin();
	std::map<__int64, TPnMOrder>::iterator j = orders.end();

	for ( ; i != j; i++)
	{
	  	if (!i->second.IsWeighted	&& i->second.Qty > 1)
		{
			accumulatedQuantity += i->second.Qty;
		}
	}
   // delete query;
    return accumulatedQuantity < 10;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateItemListDisplay(Database::TDBTransaction &DBTransaction)
{

	if (CurrentSelectedTab != 0)
	{
		VisibleItems.clear();
		if (CurrentSelectedTab == -1)
		{
			for (int i = 0; i < TabList->Count; i++)
			{
				TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction,(int)TabList->Objects[i],VisibleItems);
			}
		}
		else
		{
			TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction, CurrentSelectedTab, VisibleItems);
		}
		std::auto_ptr <TList> SortingList(new TList);
        int sideCount = 0;
        if(TGlobalSettings::Instance().HideFreeSides)
        {
            for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
            {
                if(!(itItem->second.Price == 0 && itItem->second.IsSide == true && itItem->second.IsItemFree == false))  //Added condition to exclude Side which has Cost equal to 0
                  SortingList->Add(&itItem->second);
                else
                  sideCount++;
            }
        }
        else
        {
            for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
            {
                  SortingList->Add(&itItem->second);
            }
        }
		//SortingList->Sort(ComparePickNMix);
		tgridItemList->RowCount = 0; // Clears all the Latching.
		tgridItemList->ColCount = 2;
        if(TGlobalSettings::Instance().HideFreeSides)
		    tgridItemList->RowCount = VisibleItems.size() - sideCount;  // To reduce the Count of Sides with Zero Price from RowCount
        else
            tgridItemList->RowCount = VisibleItems.size();
        if(TGlobalSettings::Instance().IsBillSplittedByMenuType &&  VisibleItems.size() != SelectedItems.size() && VisibleItems.size() &&
                SelectedItems.size() && VisibleItems.size() > 1 )
        {
            tbtnToggleGST->Visible = true;
        }
        else
        {
            tbtnToggleGST->Visible = false;
        }

		for (int i = 0; i < SortingList->Count; i++)
		{
			TPnMOrder *ptrItem = (TPnMOrder*)SortingList->Items[i];
			if (SelectedItems.find(ptrItem->Key) == SelectedItems.end())
			{ // Not Found add it.
				if(ptrItem->Type == CanceledOrder)
				{
					tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR_CANCELLED];
				}
                else
                {
                    tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
                }
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Tag = ptrItem->Key;
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Caption = "Select";
			}
			else
			{
                if(SelectedItems[ptrItem->Key].Qty != ptrItem->Qty)
                    SelectedItems[ptrItem->Key].Qty = ptrItem->Qty;
                if(ptrItem->Type == CanceledOrder)
                {
                    tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR_CANCELLED];
                }
                else
                {
                    tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                }
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Tag = ptrItem->Key;
                tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Caption = "Select";
            }

			AnsiString QtyStr = "";
			if (ptrItem->Qty != 1)
			{
				QtyStr = FormatFloat("0.00", ptrItem->Qty) + " ";
				tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched = (ptrItem->Qty  - (int)ptrItem->Qty) > 0;
			}
			else
			{
				tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched = false;
			}
			tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Caption = QtyStr + ptrItem->Name;
			tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Tag = ptrItem->Key;

			if (CurrentDisplayMode == eInvoices || CurrentTabType == TabDelayedPayment || (CurrentDisplayMode == eTabs && HasOnlineOrders))
			{
				tbtnMove->Enabled = false;
			}
			else
			{
				tbtnCancel->Enabled = true;
				tbtnMove->Enabled = true;
				tbtnSplit->Enabled = true;
			}
		}
	}
	else // Clear Display.
	{
		tgridItemList->RowCount = 0;
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::ItemSetRemoveItems(Database::TDBTransaction &DBTransaction, int TabKey)
{
	try
	{
		SelectedTabs.erase(TabKey);
		std::set <__int64> ItemsToRemove;
		TDBOrder::GetOrderKeys(DBTransaction, TabKey, ItemsToRemove);

		for (std::set <__int64> ::iterator ptrItemKey = ItemsToRemove.begin(); ptrItemKey != ItemsToRemove.end(); ptrItemKey++)
		{
			SelectedItems.erase(*ptrItemKey);
		}
		// Reset the Split Payment Form.
		frmSplitPayment->DivisionsLeft = 0;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::ItemSetAddItems(Database::TDBTransaction &DBTransaction, int TabKey)
{
	try
	{
		if (AddToSelectedTabs(DBTransaction, TabKey))
		{
			TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction, TabKey, SelectedItems, true);
			// Reset the Split Payment Form.
			frmSplitPayment->DivisionsLeft = 0;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateContainerListColourDisplay()
{
	/*
	This function on updates the Container List colours.
	For acutally populating the container list see UpdateSeatDetails */
	bool AllItemsAreSelected = true;
	for (int i = 0; i < tgridContainerList->RowCount; i++)
	{
		bool HasItemSelected = false;
		TGridButton *GridButton = tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN];
		int ButtonTab = GridButton->Tag;

		if (ButtonTab == CurrentSelectedTab)
		{
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
		}
		else
		{
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		}

		if (ButtonTab != -1)
		{
			for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
			{
				if (itItem->second.TabKey == ButtonTab)
				{
					HasItemSelected = true;
				}
			}

			if (HasItemSelected)
			{
				GridButton->Color = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
				GridButton->FontColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_FONTCOLOR];
				GridButton->LatchedColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
				GridButton->LatchedFontColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_FONTCOLOR];
			}
			else
			{
				GridButton->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
				GridButton->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
				GridButton->LatchedColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
				GridButton->LatchedFontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
				AllItemsAreSelected = false;
			}
		}
	}

	// Go back and shade the Selected button for all guests if every thhing is selected.
	if (AllItemsAreSelected)
	{
		for (int i = 0; i < tgridContainerList->RowCount; i++)
		{
			TGridButton *GridButton = tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN];
			if (GridButton->Tag == -1)
			{
				GridButton->Color = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
				GridButton->FontColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_FONTCOLOR];
				GridButton->LatchedColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
				GridButton->LatchedFontColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_FONTCOLOR];
			}
		}
	}
	else
	{
		for (int i = 0; i < tgridContainerList->RowCount; i++)
		{
			TGridButton *GridButton = tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN];
			if (GridButton->Tag == -1)
			{
				GridButton->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR]; ;
				GridButton->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
				GridButton->LatchedColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR]; ;
				GridButton->LatchedFontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
			}
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateItemListColourDisplay()
{
	for (int i = 0; i < tgridItemList->RowCount; i++)
	{
		TGridButton *GridButton = tgridItemList->Buttons[i][ITEM_LIST_COLUMN];
		int ButtonItemKey = GridButton->Tag;
                TPnMOrder ptrItem = VisibleItems[ButtonItemKey];
		if (SelectedItems.find(ButtonItemKey) == SelectedItems.end())
		{
            if(ptrItem.Type == CanceledOrder)
            {
                GridButton->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR_CANCELLED];
            }
            else
            {
                GridButton->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
            }
            GridButton->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
            GridButton->LatchedFontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
           tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color= ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
  		}
		else
		{
            if(ptrItem.Type == CanceledOrder)
            {
                GridButton->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR_CANCELLED];
            }
            else
            {
                GridButton->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
            }

            GridButton->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
            GridButton->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color= ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateTableDetails(Database::TDBTransaction &DBTransaction)
{
	if (CurrentDisplayMode == eTables)
	{
		AnsiString TableName = TDBTables::GetTableName(DBTransaction, CurrentTable);
		AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, CurrentTable);
		tbtnSelectZone->Caption = TableName;
		lbePartyName->Caption = PartyName;

		if (PartyName == "")
		{
			lbePartyName->Caption = "Tables";
		}
        if((TGlobalSettings::Instance().PromptForPatronCount || TGlobalSettings::Instance().PromptPatronCountOnTableSales) && CurrentTable > 0)
        {
            PatronTypes = TDBTables::GetPatronCount(DBTransaction, CurrentTable);
        }
	}
	else if (CurrentDisplayMode == eRooms)
	{
		AnsiString Name = TDBRooms::GetRoomName(DBTransaction, CurrentRoom);
		AnsiString Party = TDBRooms::GetPartyName(DBTransaction, CurrentRoom);
		tbtnSelectZone->Caption = Name;
		lbePartyName->Caption = Party;
		if (lbePartyName->Caption == "")
		{
			lbePartyName->Caption = "Rooms";
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateSeatDetails(Database::TDBTransaction &DBTransaction, TMembership *Membership)
{
	TabList->Clear();
	if (CurrentDisplayMode == eNoDisplayMode)
	{
		tgridContainerList->RowCount = 0;
		tgridContainerList->ColCount = 0;
	}
	else
	{
        tgridContainerList->RowCount = 0;
		tgridContainerList->ColCount = 2;

	}

	if (CurrentDisplayMode == eTabs)
	{
		switch(CurrentTabType)
		{
            case TabDelayedPayment:
                    lbePartyName->Caption = "Delayed Payment Tabs";
                    break;
            case TabNormal:
                    lbePartyName->Caption = "Tabs";
                    break;
            case TabStaff:
                    lbePartyName->Caption = "Staff Tabs";
                    break;
            case TabMember:
                    lbePartyName->Caption = "Members Tabs";
                    break;
            case TabWeb:
                    lbePartyName->Caption = "Web Orders";
                    break;
		}

		if (CurrentTabType == TabNormal || CurrentTabType == TabDelayedPayment)
		{
		    TDBTab::GetTabs(DBTransaction, TabList, CurrentTabType);
			tgridContainerList->RowCount = TabList->Count;

			for (int i = 0; i < TabList->Count; i++)
			{
                AnsiString receiptNo = TabList->Strings[i];
                AnsiString tableNo = "";

                if(CurrentTabType == TabDelayedPayment)
                {

                    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
                    DBTransaction.StartTransaction();
                    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
                    IBInternalQuery->Close();

                  IBInternalQuery->SQL->Text =
                        " SELECT "
                        " FROM_VAL "
                        " FROM "
                        "  SECURITY "
                        " WHERE "
                        "  TO_VAL = :TO_VAL ";
                    IBInternalQuery->ParamByName("TO_VAL")->AsString = receiptNo;
                    IBInternalQuery->ExecQuery();
                    tableNo = IBInternalQuery->FieldByName("FROM_VAL")->AsString;
                    IBInternalQuery->Close();

                   if (tableNo.SubString(1,11) == "Receipt No.")
                    {
                        IBInternalQuery->SQL->Text =
                            " SELECT "
                            " FROM_VAL "
                            " FROM "
                            "  SECURITY "
                            " WHERE "
                            "  TO_VAL = :TO_VAL ";
                        IBInternalQuery->ParamByName("TO_VAL")->AsString = receiptNo;
                        IBInternalQuery->ExecQuery();
                        tableNo = IBInternalQuery->FieldByName("FROM_VAL")->AsString;
                        IBInternalQuery->Close();
                    }

                    DBTransaction.Commit();
                }
                 AnsiString combinedName = receiptNo;
                 if(tableNo != "")
                 {
                    combinedName += "_";
                    combinedName += tableNo;
                 }



				tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Caption = combinedName;
				tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Tag = (int)TabList->Objects[i];
			}
		}
		else if (CurrentTabType == TabMember)
		{
			if (TempUserInfo.ContactKey != 0)
			{
				int TabKey = TDBTab::GetTabByOwner(DBTransaction, TempUserInfo.ContactKey);
				if (TabKey != 0)
				{
					tgridContainerList->RowCount = 1;
                    tgridContainerList->ColCount = 2;

					tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Caption = TempUserInfo.Name;
					tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Tag = TabKey;
					TabList->AddObject(TempUserInfo.Name, (TObject*)TabKey);
				}
				else
				{   tgridContainerList->RowCount = 0;
                    tgridContainerList->ColCount = 0;
					if (TempUserInfo.TabEnabled && TabKey == 0 && TempUserInfo.ContactKey != 0)
					{
						TabKey = TDBTab::GetOrCreateTab(DBTransaction, 0);
						TDBTab::SetTabOwner(DBTransaction, TabKey, TempUserInfo.ContactKey, TabMember);
						TDBTab::SetTabName(DBTransaction, TabKey, TempUserInfo.Name);
						TDBTab::SetTabPermanent(DBTransaction, TabKey, true);

						tgridContainerList->RowCount = 1;
                        tgridContainerList->ColCount = 2;

						tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Caption = TempUserInfo.Name;
						tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Tag = TabKey;
						TabList->AddObject(TempUserInfo.Name, (TObject*)TabKey);
					}
					else
					{
						lbePartyName->Caption = "No Membership Tab for " + TempUserInfo.Name;
					}
				}
			}
			else
			{
				tgridContainerList->RowCount = 0;
			}
		}
		else if (CurrentTabType == TabStaff)
		{
			std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
			Staff->GetTabs(DBTransaction, TabList);
			tgridContainerList->RowCount = TabList->Count;

			for (int i = 0; i < TabList->Count; i++)
			{
				tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Caption = TabList->Strings[i];
				tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Tag = (int)TabList->Objects[i];
			}
		}

		if (CurrentTabType == TabWeb)
		{
			TDBWebUtil::GetUnpaidTabs(DBTransaction, TabList);
			tgridContainerList->RowCount = TabList->Count;

			for (int i = 0; i < TabList->Count; i++)
			{
				tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Caption = TabList->Strings[i];
				tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Tag = (int)TabList->Objects[i];
			}
		}
	}
	else if (CurrentDisplayMode == eInvoices)
	{
		Membership->GetInvoices(DBTransaction, TabList, CurrentMember);
		tgridContainerList->RowCount = TabList->Count;

		for (int i = 0; i < TabList->Count; i++)
		{
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Caption = TabList->Strings[i];
			tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Tag = (int)TabList->Objects[i];
		}
	}
	else if (CurrentDisplayMode == eTables)
	{
		TDBTables::GetSeats(DBTransaction, TabList, CurrentTable);
		tgridContainerList->RowCount = TabList->Count + 1;

		tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Caption = "All " + TGlobalSettings::Instance().SeatLabel;
		tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Tag = -1;
		tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];

		for (int i = 0; i < TabList->Count; i++)
		{
			tgridContainerList->Buttons[i + 1][CONTAINER_LIST_TAB_COLUMN]->Caption = TabList->Strings[i];
			tgridContainerList->Buttons[i + 1][CONTAINER_LIST_TAB_COLUMN]->Tag = (int)TabList->Objects[i];
			tgridContainerList->Buttons[i + 1][CONTAINER_LIST_TAB_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
			tgridContainerList->Buttons[i + 1][CONTAINER_LIST_TAB_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		}

	}
	else if (CurrentDisplayMode == eRooms)
	{
		if (TDBRooms::GetRoomTab(DBTransaction, CurrentRoom) != 0)
		{
			AnsiString RoomCaption = TDBRooms::GetRoomName(DBTransaction, CurrentRoom);

			tgridContainerList->RowCount = 1;

			tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Caption = RoomCaption;
			tgridContainerList->Buttons[0][CONTAINER_LIST_TAB_COLUMN]->Tag = TDBRooms::GetRoomTab(DBTransaction, CurrentRoom);
			TabList->AddObject(RoomCaption, (TObject*)tgridContainerList->Buttons[0][0]->Tag);
		}
		else
		{
			tgridContainerList->RowCount = 0;
		}
	}

     //tbtnToggleGST->Visible = false;

	IgnoreItemThreshhold = false;

	UpdateItemListDisplay(DBTransaction);
    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && CurrentDisplayMode == eTables)
    {
        DisableBillEntireTable(DBTransaction);
    }
	UpdateContainerListColourDisplay();

	// Colour Buttons.
	for (int i = 0; i < tgridContainerList->RowCount; i++)
	{
		tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
		tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
		tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
		tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
		tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN]->Tag = tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Tag;
		tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN]->Caption = "Select";
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateSplitButtonState()
{
	tbtnSplit->Enabled = !IsSelectionComposedOnlyOfSides();
}
// ---------------------------------------------------------------------------
bool TfrmBillGroup::IsSelectionComposedOnlyOfSides()
const
{
	bool only_sides = true;

	for (std::map<__int64, TPnMOrder>::const_iterator i = \
	     SelectedItems.begin(); only_sides && i != SelectedItems.end(); ++i)
		only_sides &= i->second.IsSide;

	return only_sides;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::ShowReceipt()
{
	try
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
        PatronCountForMallExport = 0;

		if (!SelectedItems.empty())
		{
			std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
			TPaymentTransaction ReceiptTransaction(DBTransaction);

			ReceiptTransaction.ApplyMembership(Membership);
			TempReceipt->Transaction = &ReceiptTransaction;

			std::set <__int64> ReceiptItemKeys;
            if(TGlobalSettings::Instance().HideFreeSides)
            {
                for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
                {
                    if(!(itItem->second.Price == 0 && itItem->second.IsSide == true && itItem->second.IsItemFree == false))     //Added condition to exclude Side which has Cost equal to 0
                        ReceiptItemKeys.insert(itItem->first);
                }
            }
            else
            {
                for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
                {
                        ReceiptItemKeys.insert(itItem->first);
                }
            }
			TDBOrder::GetOrdersFromOrderKeys(DBTransaction, ReceiptTransaction.Orders, ReceiptItemKeys);
            if (CurrentTabType == TabDelayedPayment)
			  {
                TItemComplete *Order = (TItemComplete*)(ReceiptTransaction.Orders->Items[0]);
                ReceiptTransaction.InvoiceNumber = Order->DelayedInvoiceNumber;
              }
			LoadCustNameAndOrderType(ReceiptTransaction);

            if(CurrentTabType == TabDelayedPayment)
                  PatronTypes = TDBTab::GetDelayedPatronCount(DBTransaction, CurrentSelectedTab);

			if (TGlobalSettings::Instance().EnableMenuPatronCount)
			{
				ReceiptTransaction.CalculatePatronCountFromMenu();
			}

            if((TGlobalSettings::Instance().PromptForPatronCount || (CurrentDisplayMode == eTables && TGlobalSettings::Instance().PromptPatronCountOnTableSales) && !PatronTypes.empty()))
            {
                ReceiptTransaction.Patrons = PatronTypes;
                PatronCountForMallExport = extractPatronCountForMallExport(ReceiptTransaction);
            }

			ReceiptTransaction.Money.CreditAvailable = TDBOrder::LoadCreditFromOrders(DBTransaction, ReceiptTransaction.Orders);
			ReceiptTransaction.Money.Recalc(ReceiptTransaction);

			std::set <__int64> ItemsTabs;
			TDBOrder::GetTabKeysFromOrders(ReceiptTransaction.Orders, ItemsTabs);

			ReceiptTransaction.TabCredit.clear();
			for (std::set <__int64> ::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
			{
				Currency TabCurrentCredit = TDBTab::GetTabCredit(ReceiptTransaction.DBTransaction, *itTabs);
				if (TabCurrentCredit != 0)
				{
					ReceiptTransaction.Money.CreditAvailable += TabCurrentCredit;
					TTabCredit Credit = ReceiptTransaction.TabCredit[*itTabs];
					Credit.CurrentCredit = TabCurrentCredit;
					ReceiptTransaction.TabCredit[*itTabs] = Credit;
				}
			}
            ReceiptTransaction.IgnoreLoyaltyKey = false;
			ReceiptTransaction.Recalc();
            ReceiptTransaction.ProcessPoints();

            bool isTable = false;
            int tabKey = 0;
			TStringList *TabHistory = new TStringList;
			for (std::set <__int64> ::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
			{
                tabKey = *itTabs;
                isTable = false;
                if (CurrentDisplayMode == eTables)
                    {
                       tabKey = CurrentTable;
                       isTable = true;
                    }
				TDBTab::GetPartialPaymentList(DBTransaction, tabKey, TabHistory,isTable);
                for(int x = 0 ; x < TabHistory->Count; x++)
                {
                   if(TempReceipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
                   {
                     TempReceipt->TabHistory->Add(TabHistory->Strings[x]);
                   }
                }
			}
			delete TabHistory;

			TempReceipt->SenderType = devPC;
			TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

			if (CurrentDisplayMode == eRooms)
			{
				TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(CurrentRoom));
				TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, CurrentRoom));
			}
			else if (CurrentDisplayMode == eRooms)
			{
				TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(CurrentRoom));
				TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, CurrentRoom));
			}

			if (CurrentTabType == TabWeb)
			{

				for (std::set <__int64> ::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
				{
                        std::auto_ptr <TStringList> WebDetials(new TStringList);
                        int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, *itTabs);
                        TDBWebUtil::getWebOrderDetials(DBTransaction, WebKey, *WebDetials.get());
                        TempReceipt->ExtraInfo->AddStrings(WebDetials.get());

                        std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                        std::auto_ptr<TStringList>WebComments(new TStringList);
                        std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
                        TDBWebUtil::getWebOrderData(DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

                        TempReceipt->PaymentInfo->AddStrings(WebPaymentDetials.get());
                        TempReceipt->OrderComments->AddStrings(WebComments.get());

                        delivery_time = TDBWebUtil::getWebOrderDeliveryTime(DBTransaction, WebKey, delivery_time);
                        ReceiptTransaction.ChitNumber.DeliveryTime = delivery_time;

                        _getWebOrderMemberDetails(DBTransaction, ReceiptTransaction, WebDeliveryDetials.get(), WebDetials.get(), WebKey, delivery_time);
                        TempReceipt->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
				}

			}

			TempReceipt->PaymentType = ptPreliminary;

			if (CurrentDisplayMode == eTables)
			{
				AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, CurrentTable);
				TempReceipt->MiscData["PartyName"] = PartyName;
			}
			else if (CurrentDisplayMode == eRooms)
			{
				AnsiString PartyName = TDBRooms::GetPartyName(DBTransaction, CurrentRoom);
				TempReceipt->MiscData["PartyName"] = PartyName;
			}

			if (CurrentInvoiceKey != 0)
			{
				TempReceipt->AccountPayment = true;
				TempReceipt->AccountInvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, CurrentInvoiceKey);
				TempReceipt->Transaction->Customer = TDBContacts::GetCustomerAndRoomNumber( DBTransaction, CurrentInvoiceKey );
			}

            if((TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType != SiHot)||
               (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled))
                TempReceipt->Transaction->Customer = TCustomer(0,0,"");

            if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
                        TempReceipt->Transaction->Customer = TCustomer("",0,"");

			TPrinterPhysical DefaultScreenPrinter;
			DefaultScreenPrinter.NormalCharPerLine = 30;
			DefaultScreenPrinter.BoldCharPerLine = 30;
			DefaultScreenPrinter.PhysicalPrinterKey = TComms::Instance().ReceiptPrinter.PhysicalPrinterKey;
			Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), DefaultScreenPrinter, eDispBCOff);
			Memo1->Lines->Clear();
			TempReceipt->Printouts->PrintToStrings(Memo1->Lines);
            ReceiptTransaction.DeleteOrders();
		}
		else if (CurrentSelectedTab != 0)
		{
			std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
			TPaymentTransaction ReceiptTransaction(DBTransaction);
			ReceiptTransaction.ApplyMembership(Membership);

			TempReceipt->Transaction = &ReceiptTransaction;
            if((TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType != SiHot)||
               (!TRooms::Instance().Enabled && !TDeviceRealTerminal::Instance().BasePMS->Enabled))
                TempReceipt->Transaction->Customer = TCustomer(0,0,"");

            if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
                        TempReceipt->Transaction->Customer = TCustomer("",0,"");

			ReceiptTransaction.Money.CreditAvailable = TDBOrder::LoadCreditFromOrders(DBTransaction, ReceiptTransaction.Orders);
			ReceiptTransaction.Money.Recalc(ReceiptTransaction);

			std::set <__int64> ItemsTabs;
			ItemsTabs.insert(CurrentSelectedTab);

			ReceiptTransaction.TabCredit.clear();
			for (std::set <__int64> ::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
			{
				Currency TabCurrentCredit = TDBTab::GetTabCredit(ReceiptTransaction.DBTransaction, *itTabs);
				if (TabCurrentCredit != 0)
				{
					ReceiptTransaction.Money.CreditAvailable += TabCurrentCredit;
					TTabCredit Credit = ReceiptTransaction.TabCredit[*itTabs];
					Credit.CurrentCredit = TabCurrentCredit;
					ReceiptTransaction.TabCredit[*itTabs] = Credit;
				}
			}
            ReceiptTransaction.IgnoreLoyaltyKey = false;
			ReceiptTransaction.Recalc();
            ReceiptTransaction.ProcessPoints();
            bool isTable = false;
            int tabKey = 0;
			TStringList *TabHistory = new TStringList;
			for (std::set <__int64> ::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
			{
				tabKey = *itTabs;
                isTable = false;
                if (CurrentDisplayMode == eTables)
                    {
                       tabKey = CurrentTable;
                       isTable = true;
                    }
				TDBTab::GetPartialPaymentList(DBTransaction, tabKey, TabHistory,isTable);
                for(int x = 0 ; x < TabHistory->Count; x++)
                {
                   if(TempReceipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
                   {
                     TempReceipt->TabHistory->Add(TabHistory->Strings[x]);
                   }
                }
			}
			delete TabHistory;

			TempReceipt->SenderType = devPC;
			TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

			if (CurrentDisplayMode == eRooms)
			{
				TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(CurrentRoom));
				TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, CurrentRoom));
			}

			if (CurrentTabType == TabWeb)
			{
				for (std::set <__int64> ::iterator itTabs = ItemsTabs.begin(); itTabs != ItemsTabs.end(); advance(itTabs, 1))
				{
                    std::auto_ptr <TStringList> WebDetials(new TStringList);
                    int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, *itTabs);
                    TDBWebUtil::getWebOrderDetials(DBTransaction, WebKey , *WebDetials.get());
                    TempReceipt->ExtraInfo->AddStrings(WebDetials.get());
                    std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                    std::auto_ptr<TStringList>WebComments(new TStringList);
                    std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
                    TDBWebUtil::getWebOrderData(DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

                    TempReceipt->PaymentInfo->AddStrings(WebPaymentDetials.get());
                    TempReceipt->OrderComments->AddStrings(WebComments.get());

                    delivery_time = TDBWebUtil::getWebOrderDeliveryTime(DBTransaction, WebKey, delivery_time);
                    ReceiptTransaction.ChitNumber.DeliveryTime = delivery_time;

                    _getWebOrderMemberDetails(DBTransaction, ReceiptTransaction, WebDeliveryDetials.get(), WebDetials.get(), WebKey, delivery_time);
                    TempReceipt->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
				}
			}

			TempReceipt->PaymentType = ptPreliminary;

			if (CurrentDisplayMode == eTables)
			{
				AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, CurrentTable);
				TempReceipt->MiscData["PartyName"] = PartyName;
			}
			else if (CurrentDisplayMode == eRooms)
			{
				AnsiString PartyName = TDBRooms::GetPartyName(DBTransaction, CurrentRoom);
				TempReceipt->MiscData["PartyName"] = PartyName;
			}

			if (CurrentInvoiceKey != 0)
			{
				TempReceipt->AccountPayment = true;
				TempReceipt->AccountInvoiceNumber = Invoice->GetInvoiceNumber(DBTransaction, CurrentInvoiceKey);
			}

			TPrinterPhysical DefaultScreenPrinter;
			DefaultScreenPrinter.NormalCharPerLine = 30;
			DefaultScreenPrinter.BoldCharPerLine = 30;
			DefaultScreenPrinter.PhysicalPrinterKey = TComms::Instance().ReceiptPrinter.PhysicalPrinterKey;
			Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), DefaultScreenPrinter, eDispBCOff);
			Memo1->Lines->Clear();
			TempReceipt->Printouts->PrintToStrings(Memo1->Lines);
         	        ReceiptTransaction.DeleteOrders();
		}
		else
		{
			if (TDeviceRealTerminal::Instance().PaymentSystem->LastReceipt == NULL)
			{
				Memo1->Lines->Clear();
			}
 		}
		DBTransaction.Commit();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		Memo1->Lines->Clear();
		Memo1->Lines->Add(E.Message);
		MessageBox("Unable to display receipt.\r" "Please report the following message to your service provider :\r\r" + E.Message, "Error",
			MB_OK + MB_ICONERROR);
	}
}
// ---------------------------------------------------------------------------
bool TfrmBillGroup::AddToSelectedTabs(Database::TDBTransaction &DBTransaction, long TabKey)
{
	bool retval = false;

	if (TabKey != -1)
	{
        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney )
        {
            UnicodeString accountNumber = "";
            bool canProceed = true;
            for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
                advance(itItem, 1))
            {
                accountNumber = itItem->second.AccNumber;
                break;
            }
            if(accountNumber != "" && CurrentDisplayMode != eTables)
            {
                if(!CanMoveItemToOtherTab(atoi(accountNumber.t_str()), TabKey))
                {
                    MessageBox("Tabs with different SiHot Accounts can not be selected at same time.","Error",MB_OK);
                    return false;
                }
            }
        }

		if (SelectedTabs.find(TabKey) == SelectedTabs.end())
		{
			if (TabInUseOk(TabKey))
			{
				if (TabPINOk(DBTransaction, TabKey))
				{
					SelectedTabs.insert(TabKey);
					retval = true;
				}
			}
		}
		else if (TabInUseOk(TabKey))
		{
				retval = true;
        }
	}
	return retval;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::OnTabChange(TSystemEvents *Sender)
{
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	TabStateChanged(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
	DBTransaction.Commit();
	ShowReceipt();
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::TabStateChanged(Database::TDBTransaction &DBTransaction, TMembership *inMembership)
{
	UpdateSeatDetails(DBTransaction, inMembership);
	for (std::set <__int64> ::iterator CrntTabKey = SelectedTabs.begin(); CrntTabKey != SelectedTabs.end(); advance(CrntTabKey, 1))
	{
		for (int i = 0; i < tgridContainerList->RowCount; i++)
		{
			if (tgridContainerList->Buttons[i][0]->Tag == *CrntTabKey)
			{
				if (TDBTab::GetLocker(DBTransaction, tgridContainerList->Buttons[i][0]->Tag) == TDeviceRealTerminal::Instance().ID.Name)
				{
					SelectedTabs.erase(tgridContainerList->Buttons[i][0]->Tag);
					CrntTabKey = SelectedTabs.begin();
					TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, tgridContainerList->Buttons[i][0]->Tag);
					tgridContainerList->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
					tgridContainerList->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
				}
				else
				{
					SelectedTabs.erase(tgridContainerList->Buttons[i][0]->Tag);
					CrntTabKey = SelectedTabs.begin();
					tgridContainerList->Buttons[i][0]->Color = ButtonColors[BUTTONTYPE_FULL][ATTRIB_BUTTONCOLOR];
					tgridContainerList->Buttons[i][0]->FontColor = ButtonColors[BUTTONTYPE_FULL][ATTRIB_FONTCOLOR];
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------
bool TfrmBillGroup::TabInUseOk(int inTabKey)
{
	bool TabLockOk = false;
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	if (TDBTab::LockTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, inTabKey))
	{
		TabLockOk = true;
	}
	else
	{
		AnsiString LockedBy = TDBTab::GetLocker(DBTransaction, inTabKey);
		if (MessageBox("This " + TGlobalSettings::Instance().SeatLabel + "/Tab is in use by " + LockedBy +
				"\rDo you wish to override this lock", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
		{
			TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, inTabKey);
			if (TDBTab::LockTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, inTabKey))
			{
				TabLockOk = true;
			}
			else
			{
				MessageBox("Unable to Unlock Seat/Tab Try again Later", "Warning", MB_OK + MB_ICONWARNING);
			}
		}
	}
	DBTransaction.Commit();
	return TabLockOk;
}
// ---------------------------------------------------------------------------
bool TfrmBillGroup::TabsInUseOk(Database::TDBTransaction &DBTransaction, std::set <__int64> TabsToLock)
{
	bool TabsLockOk = false;

	if (!TDBTab::LockTabs(DBTransaction, TabsToLock, TDeviceRealTerminal::Instance().ID.Name))
	{
		if (MessageBox("There are " + TGlobalSettings::Instance().SeatLabel +
				" on this table that are in use by other terminals." "\rDo you wish to override these locks on this table.", "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
		{
			TDBTab::LockTabs(DBTransaction, TabsToLock, TDeviceRealTerminal::Instance().ID.Name, true);
			TabsLockOk = true;
		}
		else
		{
			MessageBox("You can Bill off the unlocked seats individually.", "Infomation", MB_OK + MB_ICONINFORMATION);
		}
	}
	else
	{
		TabsLockOk = true;
	}

	return TabsLockOk;
}
// ---------------------------------------------------------------------------
bool TfrmBillGroup::TabPINOk(Database::TDBTransaction &DBTransaction, int inTabKey)
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
// ---------------------------------------------------------------------------
bool TfrmBillGroup::TabStaffAccessOk(Database::TDBTransaction &DBTransaction)
{
	bool Proceed = true;
	if (CurrentDisplayMode == eTabs && CurrentTabType == TabStaff)
	{
		TMMContactInfo TempUserInfo;
		std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
		TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckPOS);
		if (Result == lsAccepted)
		{
			TDeviceRealTerminal::Instance().User = TempUserInfo;
			for (std::set <__int64> ::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end() && Proceed; advance(itTabs, 1))
			{
				if (TempUserInfo.ContactKey == TDBTab::GetTabOwner(DBTransaction, *itTabs))
				{
					Proceed = false;
					MessageBox("You cannot Bill,Cancel or Move Items off your own Tab.", "Error", MB_OK + MB_ICONERROR);
				}
			}
		}
		else if (Result == lsDenied)
		{
			MessageBox("You do not have access to P.O.S.", "Error", MB_OK + MB_ICONERROR);
			Proceed = false;
		}
		else if (Result == lsPINIncorrect)
		{
			MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
			Proceed = false;
		}
		else
		{
			Proceed = false;
		}
	}
	return Proceed;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::ClearTabOnDelayedPaymentSelection(Database::TDBTransaction &DBTransaction,int inSelectedTab)
{
      MembershipConfirmed = false;
      Membership.Clear();
      for (int i = 0; i < tgridContainerList->RowCount; i++)
	{
		TGridButton *GridButton = tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN];
		int SelectedTab = GridButton->Tag;
		ItemSetRemoveItems(DBTransaction, SelectedTab);
		CurrentSelectedTab = SelectedTab;
		// Unlock this Tab.
		//TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, SelectedTab);
        if (tgridContainerList->Buttons[i][0]->Tag != inSelectedTab)
        {
            tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
            tgridContainerList->Buttons[i][CONTAINER_LIST_TAB_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
            tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
            tgridContainerList->Buttons[i][CONTAINER_LIST_FUNC_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_FONTCOLOR];
        }
	}
	CurrentSelectedTab = 0;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::ResetForm()
{
    applyWaiterStationSettingsIfEnabled();
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	ResetSelection();
	UpdateTableDetails(DBTransaction);
	UpdateSeatDetails(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
	DBTransaction.Commit();
	PatronCount = 1;
	ShowReceipt();
	if (TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->CardOk)
	{ // Restore Membership, Reminds the user to remove the smart card.
		OnSmartCardInserted(NULL);
	}
	else
	{
	   OnSmartCardRemoved(NULL);
	}
    UpdateTableForOnlineOrdering();
    DisableTransferButtonWhenLMIsEnabled();
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::ResetSelection()
{
	CurrentSelectedTab = 0;
	PatronCount = 1;
	SelectedTabs.clear();
	SelectedItems.clear();
	Database::TDBTransaction DBTransaction(DBControl);
	DBTransaction.StartTransaction();
	TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);
	DBTransaction.Commit();

}
// ---------------------------------------------------------------------------
void TfrmBillGroup::UpdateBilledPatronCount(Database::TDBTransaction &DBTransaction,int tableNo)
{
      int counter = 0;
      std::vector<TPatronType> billedPatrons =  TDBTables::GetBilledPatronCount(DBTransaction,tableNo);
      for (std::vector<TPatronType>::iterator ptrBilledPatron = billedPatrons.begin();
           ptrBilledPatron != billedPatrons.end(); ptrBilledPatron++)
       {
           for (std::vector<TPatronType>::iterator ptrPatron = PatronTypes.begin();
                ptrPatron != PatronTypes.end(); ptrPatron++)
           {
              if(ptrPatron->Name == ptrBilledPatron->Name)
              {
                ptrPatron->Count = ptrPatron->Count- ptrBilledPatron->Count;
                if(ptrPatron->Count <= 0)
                  ptrPatron->Count = 0;
                counter += ptrPatron->Count;

              }

           }
       }
       if(counter == 0)
       {
         TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, PatronTypes, 1);
       }
}
// ---------------------------------------------------------------------------
int TfrmBillGroup::BillItems(TPaymentTransaction &paymentTransaction, Database::TDBTransaction &DBTransaction, const std::set <__int64> &ItemsToBill,
                        	 TPaymentTransactionType TransType, bool &ispaymentComplete)
{
    int retVal = 0;
    VoucherCode = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount;
	try
	{
		paymentTransaction.ApplyMembership(Membership);

        TDBOrder::GetOrdersFromOrderKeys(DBTransaction, paymentTransaction.Orders, ItemsToBill);

        if(IsPMSConfigured())
        {
            std::auto_ptr<TPMSHelper> pmsHelper(new TPMSHelper());
            pmsHelper->GetRevenueCode(paymentTransaction.Orders);
            pmsHelper->GetItemSizeIdentifierKeys(paymentTransaction.Orders);
        }
        TMMContactInfo Member;
        if(SelectedDiscount.IsComplimentaryDiscount())
          {
             paymentTransaction.TypeOfSale = ComplimentarySale;
          }
         else if( SelectedDiscount.IsNonChargableDiscount())
          {
             paymentTransaction.TypeOfSale = NonChargableSale;
          }
		if (CurrentDisplayMode == eTabs)
		{
			paymentTransaction.SalesType = eTab;
			if (CurrentTabType == TabWeb)
			{
				paymentTransaction.SalesType = eWeb;
                paymentTransaction.ChitNumber.DeliveryTime = delivery_time; //.FormatString("DD/MM/YYYY hh:nn am/pm");
                paymentTransaction.WebOrderKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, CurrentSelectedTab);

                std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                TDBWebUtil::getWebOrderExtraData(DBTransaction, paymentTransaction.WebOrderKey, "DELIVERY", WebDeliveryDetials.get());

                std::auto_ptr<TStringList>WebDetials(new TStringList);
                TDBWebUtil::getWebOrderDetials(DBTransaction, paymentTransaction.WebOrderKey, *WebDetials.get());
                _getWebOrderMemberDetails(DBTransaction, paymentTransaction, WebDeliveryDetials.get(), WebDetials.get(), paymentTransaction.WebOrderKey, delivery_time);

			}
		}
		else if (CurrentDisplayMode == eRooms)
		{
			paymentTransaction.SalesType = eRoomSale;
			paymentTransaction.BookingID = TDBRooms::GetBookingID(DBTransaction, CurrentRoom);
			paymentTransaction.RoomNumber = CurrentRoom;
			paymentTransaction.MiscPrintData["PartyName"] = TDBRooms::GetPartyName(DBTransaction, CurrentRoom);
		}
		else if (CurrentDisplayMode == eInvoices)
		{
			paymentTransaction.SalesType = eAccount;
			paymentTransaction.InvoiceKey = CurrentInvoiceKey;    //unused
		}
		else
		{
			paymentTransaction.SalesType = eTableSeat;
			paymentTransaction.MiscPrintData["PartyName"] = TDBTables::GetPartyName(DBTransaction, CurrentTable);
		}

		try
		{

            LoadCustNameAndOrderType(paymentTransaction);
            paymentTransaction.IgnoreLoyaltyKey = false;
			paymentTransaction.Recalc();
			paymentTransaction.Type = TransType;

			TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, paymentTransaction.Patrons, PatronCount);

            //MM-1649: The patron count which was asked by user when table was selected has to be assigned to the current payment transaction
            if(!PatronTypes.empty() && (TGlobalSettings::Instance().PromptForPatronCount ||
                    (paymentTransaction.SalesType = eTableSeat && TGlobalSettings::Instance().PromptPatronCountOnTableSales)))
            {
                paymentTransaction.Patrons = PatronTypes;
            }

            if( CurrentDisplayMode == eInvoices )
            {
                // use the InvoicePaymentSystem as it allows paying multiple invoices and acts different on partial n split payments
                std::auto_ptr<TMMInvoicePaymentSystem> invoicePaymentSystem( new TMMInvoicePaymentSystem() );

                if(TGlobalSettings::Instance().PMSType == SiHot && ((TItemComplete*)paymentTransaction.Orders->Items[0])->RoomNoStr != "" && TGlobalSettings::Instance().EnableCustomerJourney)
                    CustomizeForSiHot(paymentTransaction);

                invoicePaymentSystem->ProcessTransaction(paymentTransaction);
                // display last receipt if any
                _displayLastReceipt( DBTransaction, invoicePaymentSystem->LastReceipt );
            }
            else
            {
                if(TGlobalSettings::Instance().PMSType == SiHot && ((TItemComplete*)paymentTransaction.Orders->Items[0])->RoomNoStr != "" && TGlobalSettings::Instance().EnableCustomerJourney)
                    CustomizeForSiHot(paymentTransaction);
                ispaymentComplete = TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(paymentTransaction, false );
                // display last receipt if any
                _displayLastReceipt( DBTransaction, TDeviceRealTerminal::Instance().PaymentSystem->LastReceipt );
            }

            if(paymentTransaction.SalesType == eTableSeat)
            {
               if(TDBTables::GetTableExists(DBTransaction,CurrentTable))
                {
                    //check that table is free
                    if(TDBTables::IsEmpty(DBTransaction,CurrentTable))
                    {
                      TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,eNoneStatus);
                    }
                    else if(paymentTransaction.Type == eTransSplitPayment || paymentTransaction.Type == eTransPartialPayment)
                    {
                      TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,ePartialSplit);
                    }
                }
            }

            //changes to get points values..
            if(TGlobalSettings::Instance().IsRunRateBoardEnabled)
            {
                SendPointValueToRunRate(paymentTransaction);
            }

        }
		__finally
		{
			retVal  = paymentTransaction.SplittedItemKey;
            if(CurrentDisplayMode == eInvoices)
            {
                paymentTransaction.DeleteOrders();
                if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount != ""
                        || TGlobalSettings::Instance().LoyaltyMateEnabled)
                {
                    ClearLoyaltyVoucher();
                    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
                    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
                    DBTransaction1.StartTransaction();
                    RemoveMembership(DBTransaction1);
                    DBTransaction1.Commit();
                }
                TGlobalSettings::Instance().IsPOSOffline = true;
            }
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);

		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return retVal;
}
//----------------------------------------------------------------------------
void TfrmBillGroup::CustomizeForSiHot(TPaymentTransaction &PaymentTransaction)
{
    PaymentTransaction.WasSavedSales = true;
    PaymentTransaction.SalesType = eRoomSale;

    if(CurrentDisplayMode == eTabs || CurrentDisplayMode == eTables)
    {
        CustomizeDefaultCustomerInfo(PaymentTransaction);
    }
    else
    {
        for(int orderIndex = 0; orderIndex <  PaymentTransaction.Orders->Count; orderIndex++)
        {
            if(((TItemComplete*)PaymentTransaction.Orders->Items[orderIndex])->RoomNoStr != "")
            {
                PaymentTransaction.Customer.RoomNumberStr = ((TItemComplete*)PaymentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                PaymentTransaction.Phoenix.FirstName =  ((TItemComplete*)PaymentTransaction.Orders->Items[orderIndex])->FirstName;
                PaymentTransaction.Phoenix.LastName =  ((TItemComplete*)PaymentTransaction.Orders->Items[orderIndex])->LastName;
                PaymentTransaction.Phoenix.AccountNumber = ((TItemComplete*)PaymentTransaction.Orders->Items[orderIndex])->AccNo;
                PaymentTransaction.Phoenix.AccountName = PaymentTransaction.Phoenix.FirstName + " " +
                                                         PaymentTransaction.Phoenix.LastName;
                PaymentTransaction.Phoenix.RoomNumber = ((TItemComplete*)PaymentTransaction.Orders->Items[orderIndex])->RoomNoStr;
                break;
            }
        }
    }
}
// ---------------------------------------------------------------------------
std::pair<int, int> TfrmBillGroup::CalculatePatronCount()
{
   Database::TDBTransaction tr(
     TDeviceRealControl::ActiveInstance().DBControl);
   int n_completely_selected_tabs = 0;
   int total_item_patron_count = 0;

   tr.StartTransaction();
   for (std::set<__int64>::const_iterator i = SelectedTabs.begin();
          i != SelectedTabs.end(); ++i) {
      std::map<__int64, TPnMOrder> items_in_tab;
      bool all_items_in_tab_selected = true;

      TDBOrder::LoadPickNMixOrdersAndGetQuantity(tr, *i, items_in_tab);
      for (std::map<__int64, TPnMOrder>::const_iterator j =
             items_in_tab.begin(); j != items_in_tab.end(); ++j) {
         all_items_in_tab_selected &=
           SelectedItems.find(j->first) != SelectedItems.end();
         total_item_patron_count += j->second.PatronCount;
      }

      n_completely_selected_tabs += all_items_in_tab_selected == true;
   }
   tr.Commit();

   return std::pair<int, int>((n_completely_selected_tabs
                               + (n_completely_selected_tabs == 0)),
                              total_item_patron_count);
}
// ---------------------------------------------------------------------------
int TfrmBillGroup::DeterminePatronCount()
{
   const std::pair<int, int> counts = CalculatePatronCount();
   /* Storing the value to ease debugging / value inspection. */
   const int selected_count =
     (TGlobalSettings::Instance().EnableMenuPatronCount == true
      ? counts.second
      : counts.first);

   return selected_count;
}
// ---------------------------------------------------------------------------
eDisplayMode TfrmBillGroup::SelectedZone()
{
    ClipTabInTable =false;
	eDisplayMode RetVal = eNoDisplayMode;
	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    std::auto_ptr<TfrmSelectSaveOption>SelectionForm(TfrmSelectSaveOption::Create<TfrmSelectSaveOption>(this));
    SelectionForm->IsBillingMode = true;
   /* Disable the currently active membership. We don't want member discounts
	 * being applied to Tables just because we were looking at Member accounts
	 * previously.
	 */
    RemoveMembership(DBTransaction);
	DBTransaction.Commit();
	DBTransaction.StartTransaction();

	if (SelectionForm->ShowModal() != mrCancel)
	{
        if(SelectionForm->SelectedTabType != TabTableSeat)
            tbtnSelectZone->Caption = SelectionForm->Title;

		switch(int(SelectionForm->SelectedTabType))
		{
		case TabNormal:
			{
                CurrentDisplayMode = eTabs;
				CurrentTabType = TabNormal;
				UpdateRightButtonDisplay(NULL);
				CurrentTable = 1;
				ResetForm();
			}break;
		case TabStaff:
			{
                CurrentDisplayMode = eTabs;
				CurrentTabType = TabStaff;
				UpdateRightButtonDisplay(NULL);
				CurrentTable = 1;
				ResetForm();
			}break;
		case TabMember:
			{
                if(!TGlobalSettings::Instance().IsThorlinkSelected )
                {
                
//                    if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
//                    {
                        CurrentDisplayMode = eTabs;
                        CurrentTabType = TabMember;
                        CurrentInvoiceKey = 0;
                        TempUserInfo.Clear();
                        Database::TDBTransaction DBTransaction(DBControl);
                        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
                        DBTransaction.StartTransaction();
                        eMemberSource MemberSource;
                        TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo,
                            MemberSource, true);
                        DBTransaction.Commit();
                        if (Result != lsAccepted)
                        {
                            TempUserInfo.Clear();
                        }

                        /* We're "committing" to the member here. This is needed as
                         * the form pulls all member details from it's "Membership"
                         * field. ApplyMembership(...) handles adding discounts and
                         * the copying of TempUserInfo to "Membership."
                         */
                        DBTransaction.StartTransaction();
                        ApplyMembership(DBTransaction, TempUserInfo);
                        DBTransaction.Commit();

                        UpdateRightButtonDisplay(NULL);
                        CurrentTable = 1;

                        /* ResetForm() used to remove the membership which was a little
                         * paradoxical. It no longer removes the membership. It's
                         * persistent while viewing the "Member" zone.
                    */
                        ResetForm();
//				}
//				else
//				{
//					MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
//				}
                }
                else
                {
                MessageBox("No Membership Tab for Thor.", "Error", MB_OK + MB_ICONERROR);
                }
			}break;
		case TabWeb:
			{
                CurrentDisplayMode = eTabs;
                CurrentTabType = TabWeb;
                UpdateRightButtonDisplay(NULL);
                CurrentTable = 1;
                ResetForm();
                btnApplyMembership->Enabled = false;

			}break;
		case TabTableSeat:
			{
                std::auto_ptr<TEnableFloorPlan> floorPlan(new TEnableFloorPlan());
				TFloorPlanReturnParams floorPlanReturnParams;
				// Runs new web app of floorPlan
                if( floorPlan->Run( ( TForm* )this, false, floorPlanReturnParams ) )
//				if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, false, floorPlanReturnParams ) )
				{
                    if(TGlobalSettings::Instance().IsTableLockEnabled)
                    {
                        UnicodeString StaffName = TDBTables::GetStaffNameForSelectedTable(DBTransaction, floorPlanReturnParams.TabContainerNumber);
                        if(StaffName.Trim() != "" && TDeviceRealTerminal::Instance().User.Name.Pos(StaffName) == 0)
                        {
                            MessageBox("This Table can only be billed by staff " + StaffName,"Error",MB_OK);
                            ResetForm();
                            floorPlan.reset();
                            break;
                        }
                     }
                     tbtnSelectZone->Caption = SelectionForm->Title;
                    CurrentDisplayMode = eTables;
                    CurrentTabType     = TabTableSeat;
                    UpdateRightButtonDisplay(NULL);

                    CurrentTable       = floorPlanReturnParams.TabContainerNumber;
                    HasOnlineOrders    = floorPlanReturnParams.HasOnlineOrders;
                    CheckLinkedTable(floorPlanReturnParams.TabContainerNumber);
                    ResetForm();
                    floorPlan.reset();
				}
                else
                {
                    CurrentDisplayMode = eNoDisplayMode;
                    UpdateRightButtonDisplay(NULL);
                    ResetForm();
                    floorPlan.reset();
                }
			}break;
		case TabRoom:
			{
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();
				if (TRooms::Instance().SelectRoom(DBTransaction) == mrOk)
				{
					CurrentDisplayMode = eRooms;
					UpdateRightButtonDisplay(NULL);
					CurrentRoom = TRooms::Instance().SelectedRoom->RoomNo;
					CurrentGroup = 0;
					ResetForm();
				}
				DBTransaction.Commit();
			}break;
		case TabInvoice:
			{
                if(!TGlobalSettings::Instance().IsThorlinkSelected)
                {
				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr <TContactStaff> Staff(new TContactStaff(DBTransaction));
				TMMContactInfo TempUserInfo;
				TempUserInfo = TDeviceRealTerminal::Instance().User;
				bool Allow = Staff->TestAccessLevel(TempUserInfo, CheckPayInvoice);
				// If not, prompt for a login.
				if (!Allow)
				{
					TempUserInfo.Clear();
					TLoginSuccess Result = Staff->Login(this, DBTransaction, TempUserInfo, CheckPayInvoice);
					if (Result == lsAccepted)
					{
						Allow = true;
					}
					else if (Result == lsDenied)
					{
						MessageBox("You do not have access rights to the Invoice System.", "Error", MB_OK + MB_ICONERROR);
					}
					else if (Result == lsPINIncorrect)
					{
						MessageBox("The login was unsuccessful.", "Error", MB_OK + MB_ICONERROR);
					}
				}
				DBTransaction.Commit();

				if (Allow)
				{
					// Display Member Picker.
					TempUserInfo.Clear();
					Database::TDBTransaction DBTransaction(DBControl);
					TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
					DBTransaction.StartTransaction();
					eMemberSource MemberSource;
					TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo,
						MemberSource, true);
					DBTransaction.Commit();
					if (Result != lsAccepted)
					{
						TempUserInfo.Clear();
					}
					CurrentDisplayMode = eInvoices;
					CurrentTabType = TabMember;
					CurrentMember = TempUserInfo.ContactKey;
					CurrentInvoiceKey = 0;
					UpdateRightButtonDisplay(NULL);
					CurrentTable = 1;
					ResetForm();
				}
                }
                else
                {
                   MessageBox("No Membership Tab for Thor.", "Error", MB_OK + MB_ICONERROR);
                }
			}break;
		}

        EnableButtons();
		RetVal = CurrentDisplayMode;
	}
	DBTransaction.Commit();
    UpdateTableForOnlineOrdering();
    DisableTransferButtonWhenLMIsEnabled();
    IsRegistrationVerified();
	return RetVal;
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::LoadCustNameAndOrderType(TPaymentTransaction &PaymentTransaction)
{
    if(TGlobalSettings::Instance().CaptureCustomerName)
    {
        TItemComplete* Order = (TItemComplete *)PaymentTransaction.Orders->Items[0];
        TCustNameAndOrderType* CustNameAndOrderType = TCustNameAndOrderType::Instance();
        CustNameAndOrderType->LoadFromOrdersDatabase(Order->TableNo);
        PaymentTransaction.CustomerOrder = CustNameAndOrderType->GetStringPair();
        CustNameAndOrderType->ClearNameCaught();
    }
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::_onItemsCanceled( TList* inList,AnsiString inTabTableName)
{
	if( inList != NULL )
	{
		CMC_ERROR error = cmClientManager->SendCancelOrder( inList,inTabTableName );

		if( error == CMC_ERROR_FAILED )
		{
			MessageBox( "Menumate failed to send an cancel order to Chefmate",
						"Chefmate",
						MB_OK + MB_ICONWARNING);
		}
	}
}
//-----------------------------------------------------------------------------
void TfrmBillGroup::applyWaiterStationSettingsIfEnabled()
{    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
     DBTransaction.StartTransaction();
     std::auto_ptr<TContactStaff> Staff(new TContactStaff(DBTransaction));
	 TMMContactInfo TempUserInfo;
     TempUserInfo = TDeviceRealTerminal::Instance().User;
     IsWaiterLogged = !Staff->TestAccessLevel(TempUserInfo, CheckPaymentAccess);
     DBTransaction.Commit();
    btnBillTable->Enabled = !TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged;

	if (CurrentDisplayMode == eTabs)
	{

		btnBillTable->Enabled = false;
	}
	else if (CurrentDisplayMode == eTables)
	{
		btnBillTable->Enabled = !TGlobalSettings::Instance().EnableWaiterStation&& !IsWaiterLogged;
	}
	else if (CurrentDisplayMode == eRooms)
	{
		btnBillTable->Enabled = false;
	}
	else if (CurrentDisplayMode == eInvoices)
	{
		btnBillTable->Enabled = false;
	}

    //checking whether waiter station setting is checked in maintainence area

    btnBillSelected->Enabled = !TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged;
    btnPartialPayment->Enabled = !TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged;

    btnSplitPayment->Enabled = !TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged;


    tbtnDiscount->Enabled = !TGlobalSettings::Instance().EnableWaiterStation && !IsWaiterLogged;
    IsRegistrationVerified();
}
//-----------------------------------------------------------------------------
void TfrmBillGroup::_displayLastReceipt( Database::TDBTransaction &DBTransaction, TReqPrintJob *LastReceipt )
{
    if ( LastReceipt != NULL)
    {
        TPrinterPhysical DefaultScreenPrinter;
        DefaultScreenPrinter.NormalCharPerLine = 30;
        DefaultScreenPrinter.BoldCharPerLine = 30;
        DefaultScreenPrinter.PhysicalPrinterKey = TComms::Instance().ReceiptPrinter.PhysicalPrinterKey;
        Receipt->GetPrintouts(DBTransaction, LastReceipt, DefaultScreenPrinter, eDispBCOff);
        Memo1->Lines->Clear();
        LastReceipt->Printouts->PrintToStrings(Memo1->Lines);
    }
    else
    {
        Memo1->Lines->Clear();
    }
}
//-----------------------------------------------------------------------------
void TfrmBillGroup::_getWebOrderMemberDetails(Database::TDBTransaction &DBTransaction, TPaymentTransaction &PaymentTransaction, TStringList *webDeliverydetails, TStringList *webdetails, int web_key, TDateTime delivery_time)
{
    TMMContactInfo webMemberInfo;
    eMemberSource MemberSource;

    for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
    {
        TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
        WebOrderChitNumber = Order->ChitNumber;
    }

    if(TGlobalSettings::Instance().CaptureCustomerName)
    {
       webMemberInfo.Clear();
       if(!WebOrderChitNumber.Valid())
       {
          webDeliverydetails->Clear();
       }
    }

    if(WebOrderChitNumber.Valid())
    {
       if(WebOrderChitNumber.OnlineDeliveryOrder)
       {
           webMemberInfo = TDBWebUtil::autoAddWebMembers(DBTransaction, webdetails, webDeliverydetails, web_key);
           if(MemberInfo.ContactKey > 0)
           {
              webMemberInfo = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, MemberInfo, webDeliverydetails);
           }
           else
           {
              webMemberInfo = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, webMemberInfo, webDeliverydetails);
           }
       }
       else
       {
           webMemberInfo.Clear();
           webDeliverydetails->Clear();
       }
       if(WebOrderChitNumber.PromptForPickUpDeliveryTime)
       {
          if((double)WebOrderChitNumber.DeliveryTime > 0)
          {
              PaymentTransaction.ChitNumber.DeliveryTime = WebOrderChitNumber.DeliveryTime;
          }
          else
          {
             WebOrderChitNumber.DeliveryTime = delivery_time;
          }
       }
       PaymentTransaction.ChitNumber = WebOrderChitNumber;
    }
    PaymentTransaction.Membership.Assign(webMemberInfo, MemberSource);
}
//-----------------------------------------------------------------------------------------------------------
void TfrmBillGroup::EnableButtons()
{
    btnBillSelected->Enabled    =   true;
    btnPartialPayment->Enabled  =   true;
    btnSplitPayment->Enabled    =   true;
    tbtnCancel->Enabled         =   true;
    btnApplyMembership->Enabled =   true;
    tbtnDiscount->Enabled       =   true;
}
//------------------------------------------------------------------------------------------
void TfrmBillGroup::DisableButtons()
{
    tbtnMove->Enabled           =   false;
    btnBillTable->Enabled       =   false;
    btnBillSelected->Enabled    =   false;
    btnPartialPayment->Enabled  =   false;
    btnSplitPayment->Enabled    =   false;
    tbtnCancel->Enabled         =   false;
    btnApplyMembership->Enabled =   false;
    tbtnDiscount->Enabled       =   false;
}
//---------------------------------------------------------------------
void TfrmBillGroup::CheckLinkedTable(int TableNumber)
 {
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

    IBInternalQuery->SQL->Text =
           "SELECT  tb.TAB_KEY FROM TABLES a   "
           " inner join seat  st on st.TABLE_KEY = a.TABLE_KEY  "
           " inner  join tab tb on tb.TAB_KEY =st.TAB_KEY  "
           " inner join CLIPP_TAB ctb on ctb.CLIPP_TAB_TAB_KEY = tb.TAB_KEY "
           "  where a.TABLE_NUMBER = :TABLE_NUMBER ";

    IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
    IBInternalQuery->ExecQuery();

    if(IBInternalQuery->RecordCount)
    {
        btnBillTable->Enabled       =   false;
        tbtnMove->Enabled           =   false;
    }else
    {
        btnBillTable->Enabled       =   true;
        tbtnMove->Enabled           =   true;
    }

    DBTransaction.Commit();
 }
// ---------------------------------------------------------------------------
void TfrmBillGroup::ApplyMembership(Database::TDBTransaction &DBTransaction, TMMContactInfo &MembershipInfo)
{
	try
	{
        VoucherCode = "";
		eMemberSource MemberSource;
		TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, MembershipInfo, MemberSource);

		if (Result == lsAccountBlocked)
		{
			MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
		}
		else if (Result == lsAccepted)
		{
            btnApplyMembership->ButtonColor = clPurple;
			RemoveMembershipDiscounts(DBTransaction);
			MembershipConfirmed = true;
            if(TGlobalSettings::Instance().LoyaltyMateEnabled)
            {
               TManagerDiscount managerDiscount;
               managerDiscount.GetMembershipDiscounts(DBTransaction,MembershipInfo.AutoAppliedDiscounts);
            }
			Membership.Assign(MembershipInfo, MemberSource);
			lbeMembership->Visible = true;
            lbeMembership->Caption = MembershipInfo.Name + " (" + MembershipInfo.MembershipNumber + ")" + " Points:" + FormatFloat("0.00", GetAvailableRedeemPoints(MembershipInfo));
            std::set <__int64> SelectedItemKeys;
            for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
             {
                SelectedItemKeys.insert(itItem->first);
             }
             TPaymentTransaction PaymentTransaction(DBTransaction);
             TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, SelectedItemKeys);
             ManagerDiscount->ClearMemberExemtDiscounts(PaymentTransaction.Orders);
             PaymentTransaction.ApplyMembership(Membership);
             PaymentTransaction.DeleteOrders();
	  }
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::RemoveMembership(Database::TDBTransaction &DBTransaction)
{
	MembershipConfirmed = false;
	Membership.Clear();
	lbeMembership->Visible = false;
	lbeMembership->Caption = "";

     std::set <__int64> Items;
    std::auto_ptr <TList> OrdersList(new TList);
    for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
      advance(itItem, 1))
    {
		Items.insert(itItem->first);
    }
    TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), Items);
    // Remove all Free Items.
    ManagerFreebie->UndoFreeCount(DBTransaction, OrdersList.get());

    ManagerDiscount->ClearMemberDiscounts(OrdersList.get());
    ManagerDiscount->SetDiscountAmountDB(DBTransaction, OrdersList.get());

    while (OrdersList->Count != 0)
	{
		delete(TItemComplete*)OrdersList->Items[0];
		OrdersList->Delete(0);
    }

    for (std::set <__int64> ::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end() ; advance(itTabs, 1))
    {
        TDBTab::SetTabOrdersLoyalty(DBTransaction,*itTabs,0);
    }
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::RemoveMembershipFreeItems(Database::TDBTransaction &DBTransaction)
{
    std::set <__int64> Items;
    std::auto_ptr <TList> OrdersList(new TList);
    for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
      advance(itItem, 1))
    {
		Items.insert(itItem->first);
    }
    TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), Items);
    // Remove all Free Items.
    ManagerFreebie->UndoFreeCount(DBTransaction, OrdersList.get());

    while (OrdersList->Count != 0)
    {
        delete(TItemComplete*)OrdersList->Items[0];
        OrdersList->Delete(0);
    }
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::RemoveMembershipDiscounts(Database::TDBTransaction &DBTransaction)
{
  //if(!Membership.Applied())
  //   return;
   std::set <__int64> Items;
   std::auto_ptr <TList> OrdersList(new TList);
   for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
      advance(itItem, 1))
   {
      Items.insert(itItem->first);
   }
   TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), Items);
   ManagerDiscount->ClearMemberDiscounts(OrdersList.get());
   ManagerDiscount->SetDiscountAmountDB(DBTransaction, OrdersList.get());
   while (OrdersList->Count != 0)
	{
		delete(TItemComplete*)OrdersList->Items[0];
		OrdersList->Delete(0);
    }
}
//-----------------------------------------------------------------------------------------------------
void TfrmBillGroup::SendPointValueToRunRate( TPaymentTransaction &inTransaction )
{
        TRunRateClient *cmClient = new TRunRateClient();
        cmClient->SendPointDetailsToRunRate( inTransaction );
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::CheckLoyalty()
{
   bool allow = false;
   if(!TGlobalSettings::Instance().LoyaltyMateEnabled ||
                (TGlobalSettings::Instance().LoyaltyMateEnabled &&  !TGlobalSettings::Instance().IsPOSOffline))
   {
      allow = true;
   }
   else
   {
        Database::TDBTransaction DBTransaction(DBControl);
        TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
        DBTransaction.StartTransaction();
        RemoveMembership(DBTransaction);
        ClearLoyaltyVoucher();
        DBTransaction.Commit();

   }

    if (allow)
	{
        std::set <__int64> ReceiptItemKeys;
        for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
        {
            ReceiptItemKeys.insert(itItem->first);
        }
        CheckLoyalty(ReceiptItemKeys);
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::CheckLoyalty(std::set <__int64> ReceiptItemKeys)
{
    std::set <__int64> PossiableMembers;
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    TDBOrder::GetMemberKeysFromOrderKeys(DBTransaction, PossiableMembers, ReceiptItemKeys);
    if(PossiableMembers.size() == 0 && CurrentTabType != TabMember)
    {
        Membership.Clear();
        ClearLoyaltyVoucher();
        MembershipConfirmed = false;
        lbeMembership->Visible = false;
        lbeMembership->Caption = "";
    }
    else if(!MembershipConfirmed)
    {
        if (PossiableMembers.size() > 1)
        {
            // Display Reports List
            std::auto_ptr <TfrmVerticalSelect> SelectionForm(TfrmVerticalSelect::Create <TfrmVerticalSelect> (this));
            for (std::set <__int64> ::iterator pMembersKey = PossiableMembers.begin(); pMembersKey != PossiableMembers.end();
                advance(pMembersKey, 1))
            {
                TMMContactInfo TempUserInfo;
                eMemberSource MemberSource;
                TempUserInfo.ContactKey = *pMembersKey;

                TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo,
                    MemberSource);
                if (Result != lsUserNotFound)
                {
                    TVerticalSelection Item;
                    Item.Title = TempUserInfo.Name + " " + TempUserInfo.MembershipNumber;
                    Item.Properties["Color"] = IntToStr(clNavy);
                    Item.Properties["Member"] = IntToStr(TempUserInfo.ContactKey);
                    Item.CloseSelection = true;
                    SelectionForm->Items.push_back(Item);
                }
            }

            if (SelectionForm->Items.size() > 0)
            {
                SelectionForm->ShowModal();
                TVerticalSelection SelectedItem;
                if (SelectionForm->GetFirstSelectedItem(SelectedItem) && SelectedItem.Title != "Cancel")
                {
                    int MemberKey = StrToIntDef(SelectedItem.Properties["Member"], 0);
                    if (MemberKey != 0)
                    {
                        TMMContactInfo TempUserInfo;
                        eMemberSource MemberSource;
                        TempUserInfo.ContactKey = MemberKey;
                        TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo, MemberSource);
                        if (Result == lsAccepted)
                        {
                            ApplyMembership(DBTransaction, TempUserInfo);
                        }
                        else if (Result == lsAccountBlocked)
                        {
                            MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked",
                                MB_OK + MB_ICONINFORMATION);
                        }
                    }
                }
            }
        }
        else
        {
            std::set <__int64> ::iterator pMemberKey = PossiableMembers.begin();
            if (pMemberKey != PossiableMembers.end())
            {
                TMMContactInfo MembershipInfo;
                eMemberSource MemberSource;
                MembershipInfo.ContactKey = *pMemberKey;
                TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, MembershipInfo,MemberSource);
                if (Result == lsAccepted)
                {
                  ApplyMembership(DBTransaction, MembershipInfo);
                }
            }
        }
    }
    DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::OnSmartCardInserted(TSystemEvents *Sender)
{
	TMMContactInfo TempUserInfo;
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetContactInfo(TempUserInfo);
	if (TempUserInfo.Valid() && !MembershipConfirmed && Active)
	{ // Restore Membership, Reminds the user to remove the smart card.
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,TempUserInfo);

        if(TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            SetLoyaltyMemberInfo(DBTransaction,TempUserInfo);
            DownloadOnlineMember();

        }
		ApplyMembership(DBTransaction, TempUserInfo);
		DBTransaction.Commit();
		ShowReceipt();
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::OnSmartCardRemoved(TSystemEvents *Sender)
{
//	if (TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
//	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		RemoveMembership(DBTransaction);
		DBTransaction.Commit();
		TDeviceRealTerminal::Instance().ManagerMembership->EndMemberTransaction();
        ShowReceipt();
//	}
}
// ---------------------------------------------------------------------------
int TfrmBillGroup::extractPatronCountForMallExport(TPaymentTransaction &paymentTransaction)
{
    int result = 0;
    if(TGlobalSettings::Instance().MallIndex == FEDERALLANDMALL)
    {
        std::vector <TPatronType> ::iterator ptrPatronTypes = paymentTransaction.Patrons.begin();
        for ( ; ptrPatronTypes != paymentTransaction.Patrons.end(); ptrPatronTypes++ )
        {
            result += ptrPatronTypes->Count != 0 ? ptrPatronTypes->Count : 0;
        }
    }
    return result;
}
// ---------------------------------------------------------------------------
Currency TfrmBillGroup::GetAvailableRedeemPoints(TMMContactInfo &Member)
{
    Currency points = 0;
    if (TGlobalSettings::Instance().EnableSeperateEarntPts)
    {
       points = Member.Points.getPointsBalance(pasDatabase,ptstAccount);
    }
    else
    {
       points = Member.Points.getPointsBalance(pasDatabase);
    }

  return points;
}

void TfrmBillGroup::UpdateContainerList()
{
    ClipTabInTable =false;

    CurrentSelectedTab = 0;

    UpdateRightButtonDisplay(NULL);
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    UpdateTableDetails(DBTransaction);
    TabStateChanged(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
    TMembership* memberShip = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();
    DBTransaction.Commit();
}
//------------------------------------------------------------------------------------------------------
void __fastcall TfrmBillGroup::tbtnToggleGSTMouseClick(TObject *Sender)
{
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();

    if(TabList->Count > 1 && CurrentSelectedTab > 0)
    {
        VisibleItems.clear();
        TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction, CurrentSelectedTab, VisibleItems);
    }
    TItemType itemType;

    if(SelectedItems.size())
    {
        std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin();
        itemType = itItem->second.ItemType;
    }
    else
    {
        itemType = eDrinksItem;
    }

     SelectedItems.clear();

    for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
    {
       TPnMOrder ptrSelectItem = VisibleItems[itItem->first];
       if(itItem->second.ItemType != itemType)
            SelectedItems[itItem->first] = ptrSelectItem;
    }

    UpdateItemListDisplay(DBTransaction);
    if(TGlobalSettings::Instance().IsBillSplittedByMenuType && CurrentDisplayMode == eTables)
    {
        DisableBillEntireTable(DBTransaction);
    }
    UpdateContainerListColourDisplay();
    UpdateSplitButtonState();
     if(lbeMembership->Visible == false && Membership.Member.AutoAppliedDiscounts.size()>0) //todo-Arpit
    {
       RemoveMembershipDiscounts(DBTransaction);
    }
    DBTransaction.Commit();
    if(!TGlobalSettings::Instance().IsThorlinkSelected)
    {
        CheckLoyalty();
    }
    ShowReceipt();
}
//-------------------------------------------------------------------------------------------------
void TfrmBillGroup::DisableBillEntireTable(Database::TDBTransaction &DBTransaction)
{
    TItemType itemType;
    btnBillTable->Enabled = true;
    for (int i = 0; i < TabList->Count; i++)
    {
        TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction,(int)TabList->Objects[i],VisibleItems);
    }

    if(SelectedItems.size())
    {
        std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin();
        itemType = itItem->second.ItemType;
    }
    else
    {
        std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin();
        itemType = itItem->second.ItemType;
    }

    for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
    {
        if(itemType !=  itItem->second.ItemType)
        {
            btnBillTable->Enabled = false;
            break;
        }
    }
}
//---------------------------------------------------------------------------------------------------------
void TfrmBillGroup::ChangeBillEntireTableState()
{
    VisibleItems.clear();
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    DisableBillEntireTable(DBTransaction);
    if(btnBillTable->Enabled)
        tbtnToggleGST->Visible = false;
    DBTransaction.Commit();
}
//--------------------------------------------------------------------------------------------------------
void TfrmBillGroup::DisableToggleGSTButton(Database::TDBTransaction &DBTransaction)
{
    //Other cases already tested so seperating code.
    tbtnToggleGST->Visible = false;
    TItemType itemType;
    std::map<__int64,TPnMOrder> VisibleItemsForGST;

    if(CurrentSelectedTab == -1)
    {
        for (int i = 0; i < TabList->Count; i++)
        {
            TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction,(int)TabList->Objects[i],VisibleItemsForGST);
        }
    }
    else
    {
        TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction, CurrentSelectedTab, VisibleItemsForGST);
    }

        std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItemsForGST.begin();
        itemType = itItem->second.ItemType;

    for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItemsForGST.begin(); itItem != VisibleItemsForGST.end(); advance(itItem, 1))
    {
        if(itemType !=  itItem->second.ItemType)
        {
            tbtnToggleGST->Visible = true;
            break;
        }
    }
}
//-----------------------------------------------------------------------------
void TfrmBillGroup::ClearLoyaltyVoucher()
{
    if(TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
        TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount = "";
        TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherName = "";
    }
}
//-----------------------------------------------------------------------------
void TfrmBillGroup::ApplyDiscountWithRestructure(TPaymentTransaction &paymentTransaction, TDiscount discount)
{
    Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction1.StartTransaction();
    try
    {
        paymentTransaction.Patrons = PatronTypes;
        std::auto_ptr<TSCDPatronUtility> patronUtility(new TSCDPatronUtility());
        if(patronUtility->CheckPatronsAvailable(paymentTransaction) && !discount.IsPersonWithDisabilityDiscount())
        {
            std::auto_ptr <TList> SCDOrders(new TList);
            SCDOrders->Clear();
            std::auto_ptr <TList> NormalOrders(new TList);
            NormalOrders->Clear();

            patronUtility->DivideBill(paymentTransaction,SCDOrders,NormalOrders);
            for(int index = 0; index < SCDOrders->Count; index++)
            {
                TItemComplete *ic = (TItemComplete*)SCDOrders->Items[index];
            }
            for(int index = 0; index < NormalOrders->Count; index++)
            {
                TItemComplete *ic = (TItemComplete*)NormalOrders->Items[index];
            }
            if(discount.IsSeniorCitizensDiscount())
            {
               ManagerDiscount->AddDiscount(SCDOrders.get(), discount);
            }
            else
            {
               ManagerDiscount->AddDiscount(NormalOrders.get(), discount);
            }
            paymentTransaction.Orders->Clear();
            for(int indexSCD = 0; indexSCD < SCDOrders->Count; indexSCD++)
            {
                TItemComplete *Order = (TItemComplete *)SCDOrders->Items[indexSCD];
                paymentTransaction.Orders->Add(Order);
            }
            for(int indexNormal = 0; indexNormal < NormalOrders->Count; indexNormal++)
            {
                TItemComplete *Order = (TItemComplete *)NormalOrders->Items[indexNormal];
                paymentTransaction.Orders->Add(Order);
            }
            paymentTransaction.Money.Recalc(paymentTransaction);
        }
        else
        {
            ManagerDiscount->AddDiscount(paymentTransaction.Orders, discount);
        }
        TDBOrder::DeleteOrdersForreatructure(paymentTransaction.Orders);
        std::vector<int> newOrderKeys;
        newOrderKeys.clear();
        for(int index = 0; index < paymentTransaction.Orders->Count; index++)
        {
            TItemComplete *ic = (TItemComplete*)paymentTransaction.Orders->Items[index];
            TDBOrder::ProcessOrder(DBTransaction1,ic);
            newOrderKeys.push_back(ic->OrderKey);
        }
        DBTransaction1.Commit();
        DBTransaction1.StartTransaction();
        SelectedItems.clear();
        VisibleItems.clear();
        for (std::set <__int64> ::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end() ; advance(itTabs, 1))
        {
            TDBOrder::LoadPickNMixOrdersAndGetQuantity(DBTransaction1, *itTabs, VisibleItems);
        }

        DBTransaction1.Commit();
        DBTransaction1.StartTransaction();
        std::map<__int64,TPnMOrder>::iterator itVisible = VisibleItems.begin();
        for(;itVisible !=  VisibleItems.end(); advance(itVisible,1))
        {
            if(std::find(newOrderKeys.begin(), newOrderKeys.end(), itVisible->first) != newOrderKeys.end())
            {
               SelectedItems[itVisible->first] = itVisible->second;
            }
        }
        std::map<__int64,TPnMOrder>::iterator itSelected = SelectedItems.begin();
        UpdateItemListDisplay(DBTransaction1);
        DBTransaction1.Commit();
    }
    catch(Exception &Ex)
    {
        DBTransaction1.Rollback();
        MessageBox(Ex.Message,"Exception",MB_OK);
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    paymentTransaction.Patrons.clear();
}
//----------------------------------------------------------------------------
void TfrmBillGroup::CustomizeDefaultCustomerInfo(TPaymentTransaction &PaymentTransaction)
{
    PaymentTransaction.Phoenix.AccountNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultAccountNumber;;
    PaymentTransaction.Phoenix.AccountName = TManagerVariable::Instance().GetStr(PaymentTransaction.DBTransaction,vmSiHotDefaultTransactionName);
    PaymentTransaction.Phoenix.RoomNumber = TDeviceRealTerminal::Instance().BasePMS->DefaultTransactionAccount;
    PaymentTransaction.Phoenix.FirstName = TManagerVariable::Instance().GetStr(PaymentTransaction.DBTransaction,vmSiHotDefaultTransactionName);
    PaymentTransaction.Customer.RoomNumberStr = PaymentTransaction.Phoenix.RoomNumber;
    if(PaymentTransaction.Orders->Count > 0 && (PaymentTransaction.Type != eTransSplitPayment && PaymentTransaction.Type != eTransPartialPayment))
    {
        ((TItemComplete*)PaymentTransaction.Orders->Items[0])->RoomNoStr = PaymentTransaction.Phoenix.RoomNumber;
    }
}
//-----------------------------------------------------------------------------------------------

/* In reference to case #90727(Salesforce)
In case of Loyaltymate membership can not get saved to table.
If memebrship is applied on bill group screen, with number of selected
items as x. Now if we start deselecting items and reach a state where
number of selected items becomes 0, membership should get removed.
If Voucher was applied, Voucher/Discount should get removed.
Case of number of selected items becoming zero can be achieved by multiple ways
like by transferring partial/complete to another table/tab,
by moving item/items of one guest to another guest.
Method ClearLoyaltyVoucher() is created for this purpose. This method is called from
multiple places to handle the required situations.
Also changes to CheckLoyalty() calling is changed in case of loyaltymate by
appending if clause to the calling.
*/
//---------------------------------------------------------------------------
bool TfrmBillGroup::IsPMSConfigured()
{
    bool retValue = false;
    retValue = ((TTransactionHelper::IsOracleConfigured() || TTransactionHelper::IsSiHotConfigured() ) &&
                (TGlobalSettings::Instance().PMSType == Oracle || TGlobalSettings::Instance().PMSType == SiHot));
    return retValue;
}
//----------------------------------------------------------------------------
//----------Delete this code--------------of no use now-----------------------
bool TfrmBillGroup::NeedtoUpdateTableForOnlineOrdering()
{
    if(TGlobalSettings::Instance().EnableOnlineOrdering)
    {
        if(CurrentDisplayMode == eTables && (TDBTables::HasOnlineOrders(CurrentTable)))
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void TfrmBillGroup::UpdateTableForOnlineOrdering()
{
    if(CurrentDisplayMode == eTables && HasOnlineOrders)
    {
        btnBillSelected->Color      = clSilver;
        btnBillSelected->Enabled    = false;
        btnTransfer->Color          = clSilver;
        btnTransfer->Enabled        = false;
        tbtnMove->Color             = clSilver;
        tbtnMove->Enabled           = false;
        btnPartialPayment->Color    = clSilver;
        btnPartialPayment->Enabled  = false;
        btnSplitPayment->Color      = clSilver;
        btnSplitPayment->Enabled    = false;
        btnApplyMembership->Color   = clSilver;
        btnApplyMembership->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void TfrmBillGroup::UpdateTabForOnlineOrdering()
{
    bool isTableGuest = CurrentTable && TDBTables::HasOnlineOrders(CurrentTable);
    if((CurrentDisplayMode == eTabs && HasOnlineOrders) || (isTableGuest))
    {
        btnTransfer->Color          = clSilver;
        btnTransfer->Enabled        = false;
        tbtnMove->Color             = clSilver;
        tbtnMove->Enabled           = false;
        btnPartialPayment->Color    = clSilver;
        btnPartialPayment->Enabled  = false;
        btnSplitPayment->Color      = clSilver;
        btnSplitPayment->Enabled    = false;
        btnApplyMembership->Color   = clSilver;
        btnApplyMembership->Enabled = false;
        if(isTableGuest)
            btnBillSelected->Enabled = false;
    }
}
//---------------------------------------------------------------------------
bool TfrmBillGroup::CheckTabCompatablityForOnlineOrdering(int tabKey)
{
    bool retValue = true;
    try
    {
        bool currentTabHoldsOnlineOrders = TDBTab::HasOnlineOrders(tabKey);

        if(CurrentDisplayMode == eTabs)
        {
            std::set <__int64> ::iterator itTabsSelected = SelectedTabs.begin();
            for(; itTabsSelected != SelectedTabs.end(); advance(itTabsSelected,1))
            {
                if(currentTabHoldsOnlineOrders)
                {
                    if(!TDBTab::HasOnlineOrders(*itTabsSelected))
                    {
                        retValue = false;
                        MessageBox("Tabs with online orders and Normal Tabs can not be selected simultaneously.","Info",MB_OK+MB_ICONINFORMATION);
                        break;
                    }
                }
                else
                {
                    if(TDBTab::HasOnlineOrders(*itTabsSelected))
                    {
                        retValue = false;
                        MessageBox("Normal Tabs and Tabs with online orders can not be selected simultaneously.","Info",MB_OK+MB_ICONINFORMATION);
                        break;
                    }
                }
            }
        }
    }
    catch(Exception &Ex)
    {
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TfrmBillGroup::DownloadOnlineMember()
{
    try
    {
        UnicodeString emailId = GetMemberEmailIdForOrder();
        if(emailId.Trim() != "")
            GetLoyaltyMemberByEmail(emailId);
    }
    catch(Exception &ex)
    {
    }
}
//---------------------------------------------------------------------------
UnicodeString TfrmBillGroup::GetMemberEmailIdForOrder()
{
    UnicodeString emailId = "";
    if(CurrentDisplayMode == eTabs)
    {
        emailId = TDBTab::GetMemberEmail(CurrentSelectedTab);
    }
    else if(CurrentDisplayMode == eTables)
    {
        emailId = TDBTables::GetMemberEmail(CurrentTable);
    }
    return emailId;
}
//---------------------------------------------------------------------------
void TfrmBillGroup::GetLoyaltyMemberByEmail(UnicodeString email)
{
    bool memberDownloadStatus = false;
    bool MemberNotExist = false;
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        TMMContactInfo contactInfo;
        contactInfo.EMail = email;
        TManagerSyndCode ManagerSyndicateCode;
        ManagerSyndicateCode.Initialise(dbTransaction);
        if(TDeviceRealTerminal::Instance().ManagerMembership->GetMemberFromCloudForOO(contactInfo))
        {
            int contactKey = TDBContacts::GetContactByEmail(dbTransaction,email);
            TDBContacts::GetContactDetails(dbTransaction, contactKey, contactInfo);
            TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
            ManagerLoyaltyVoucher.DisplayMemberVouchers(dbTransaction,contactInfo);
            TManagerDiscount managerDiscount;
            managerDiscount.GetMembershipDiscounts(dbTransaction,contactInfo.AutoAppliedDiscounts);
            Membership.Assign(contactInfo, emsManual);
        }

        dbTransaction.Commit();
    }
    catch(Exception &ex)
    {
        dbTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TfrmBillGroup::DisableTransferButtonWhenLMIsEnabled()
{
    if(TGlobalSettings::Instance().LoyaltyMateEnabled)
    {
        UnicodeString email = "";

        if(CurrentDisplayMode == eTabs)
            email = TDBTab::GetMemberEmail(CurrentSelectedTab);
        else if(CurrentDisplayMode == eTables)
            email = TDBTables::GetMemberEmail(CurrentTable);

        if(email.Trim() != "")
        {
            btnTransfer->Enabled = false;
            tbtnMove->Enabled = false;
        }
    }
}
//--------------------------------------------------
void TfrmBillGroup::SetLoyaltyMemberInfo(Database::TDBTransaction &DBTransaction, TMMContactInfo info)
{
    if(CurrentDisplayMode == eTables)
    {
        TDBOrder::SetMemberEmailLoyaltyKeyForTable(DBTransaction, CurrentTable, info.ContactKey, info.EMail);
    }
    else if(CurrentDisplayMode == eTabs)
    {
        TDBOrder::SetMemberEmailLoyaltyKeyForTab(DBTransaction, CurrentSelectedTab, info.ContactKey, info.EMail);
    }
}
//--------------------------------------------------
void TfrmBillGroup:: MergeZeroPriceSideKeysWithSelectedItemKeys(std::set<__int64> &SelectedItemKeys)
{
    std::set <__int64> ZeroPriceSideKeys;
    for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
    {
        if(itItem->second.Price == 0 && itItem->second.IsSide == true && itItem->second.IsItemFree == false)
          ZeroPriceSideKeys.insert(itItem->first);
    }
    SelectedItemKeys.insert(ZeroPriceSideKeys.begin(),ZeroPriceSideKeys.end());    //Merging the Item keys of Zero Price Sides with Selected Item Keys
}
//---------------------------------------------
void TfrmBillGroup:: IsRegistrationVerified()
{
    bool isEnabled = TGlobalSettings::Instance().IsRegistrationVerified &&
                    !IsWaiterLogged && !TGlobalSettings::Instance().EnableWaiterStation;


    btnBillSelected->Enabled   = isEnabled;//TGlobalSettings::Instance().IsRegistrationVerified && !IsWaiterLogged;
    btnPartialPayment->Enabled = isEnabled;//TGlobalSettings::Instance().IsRegistrationVerified && !IsWaiterLogged;
    btnSplitPayment->Enabled  = isEnabled;//TGlobalSettings::Instance().IsRegistrationVerified && !IsWaiterLogged;

    if(CurrentDisplayMode == eTables && isEnabled) //&& TGlobalSettings::Instance().IsRegistrationVerified && !IsWaiterLogged)
        btnBillTable->Enabled = true;
    else
        btnBillTable->Enabled = false;

//    tbtnReprintReceipts->Enabled = TGlobalSettings::Instance().IsRegistrationVerified;

    if((TGlobalSettings::Instance().IsRegistrationVerified && CurrentDisplayMode != eInvoices &&  CurrentTabType != TabDelayedPayment)
        && !(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot && TGlobalSettings::Instance().EnableCustomerJourney))
        btnTransfer->Enabled = true;
    else
        btnTransfer->Enabled = false;

    tbtnReprintReceipts->Enabled = TGlobalSettings::Instance().IsRegistrationVerified;

}
//---------------------------------------------------
void TfrmBillGroup::UpdateTabeleStateForOO()
{
    //If whole table is billed then unseat is if already seated.
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();
    bool isTableBilled = TGlobalSettings::Instance().EnableOnlineOrdering && TDBTables::IsTableMarked(DBTransaction, CurrentTable) &&
                                TDBTables::IsTableBilled(DBTransaction, CurrentTable);
     if(isTableBilled)
        TDBTables::UpdateTableStateForOO(DBTransaction, CurrentTable, false);
    DBTransaction.Commit();
}
//--------------------------------------------------------
void TfrmBillGroup::SendFiscalPrint(TPaymentTransaction &paymentTransactionNew)
{
    try
    {
        if(TGlobalSettings::Instance().UseItalyFiscalPrinter &&  TTransactionHelper::CheckRoomPaytypeWhenFiscalSettingEnable(paymentTransactionNew))
            TDeviceRealTerminal::Instance().PaymentSystem->PrintFiscalReceipt(paymentTransactionNew);
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//--------------------------------------------------------
void TfrmBillGroup::ResetTransactionAfterCommit(TPaymentTransaction &paymentTransaction)
{
    try
    {
        paymentTransaction.DeleteOrders();
        if(TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->RedeemedVoucherDiscount != ""
                || TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            ClearLoyaltyVoucher();
            Database::TDBTransaction DBTransaction1(TDeviceRealTerminal::Instance().DBControl);
            TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction1);
            DBTransaction1.StartTransaction();
            RemoveMembership(DBTransaction1);
            DBTransaction1.Commit();
        }
        TGlobalSettings::Instance().IsPOSOffline = true;
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
    }
}
//--------------------------------------------------------
void TfrmBillGroup::RecordFiscalLogsPaymentSystem(TStringList* logList, AnsiString logValue)
{
    try
    {
        logList->Add(logValue);
        TSaveLogs::RecordFiscalLogs(logList);
        logList->Clear();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        MessageBox(ex.Message,"Error in logging for Fiscal Printer",MB_OK+MB_ICONERROR);
    }
}
//--------------------------------------------------------
bool TfrmBillGroup::CheckIfMembershipUpdateRequired(Database::TDBTransaction &DBTransaction,int source_key, int DestTabKey)
{
    bool retValue = true;
    UnicodeString SourceEmail = "";
    UnicodeString DestinationEmail = "";
    try
    {
        SourceEmail        = TDBOrder::GetOrderEmail(DBTransaction, source_key);
        DestinationEmail   = TDBOrder::GetOrderEmail(DBTransaction, DestTabKey);
        if(SourceEmail.Trim() != "" && DestinationEmail.Trim() != "" && !SameStr(SourceEmail.Trim(),DestinationEmail.Trim()))
        {
            UnicodeString message = DestinationEmail +" on Table " + IntToStr(CurrentTable) + " will be replaced by "+ SourceEmail +" of Table " + IntToStr(CurrentTable);
            if(MessageBox(message,"Warning",MB_YESNO  + MB_ICONWARNING) == IDYES)
            {
                TDBTables::UpdateMemberEmail(DBTransaction, SourceEmail, DestinationEmail, DestTabKey);
            }
            else
            {
                retValue = false;
            }
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
    return retValue;
}
