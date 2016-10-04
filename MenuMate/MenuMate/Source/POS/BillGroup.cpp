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
#include "DBClippTab.h"
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
#include <IBQuery.hpp>
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
#include "GUIScale.h"
#include "FreebieManager.h"
#include "SeniorCitizenDiscountChecker.h"
#include "SelectDish.h"
#include "MMInvoicePaymentSystem.h"
#include "ManagerDelayedPayment.h"
#include "SelectSaveOption.h"
#include "ManagerClippIntegration.h"
#include "MallExportOtherDetailsUpdate.h"
#include "MallExportDefines.h"
#include "ManagerLoyaltyVoucher.h"
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
	FormResize(this);
	InCheckFunc = false;

    ClipTabInTable =false;
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
                            case TabClipp:
                                    tbtnSelectZone->Caption = "Clipp Tabs";
                                    ClipOptionSelected=true;
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
            UpdateTableDetails(DBTransaction);
            TabStateChanged(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
            DBTransaction.Commit();
	}

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
	if (SelectedZone() == eNoDisplayMode)
	{
        ClipTabInTable =false;

		if (TGlobalSettings::Instance().TabsEnabled)
		{
			CurrentDisplayMode = eTabs;
			CurrentTabType = TabNormal;
		}
		CurrentSelectedTab = 0;

		UpdateRightButtonDisplay(NULL);
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		UpdateTableDetails(DBTransaction);
		TabStateChanged(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
		DBTransaction.Commit();

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
                                   TManagerDelayedPayment::Instance().MoveOrderToTab(ReceiptTransaction,true);
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
                            std::auto_ptr <TReqPrintJob> TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
                            TPaymentTransaction ReceiptTransaction(DBTransaction);
                            ReceiptTransaction.ApplyMembership(Membership);
                            TempReceipt->Transaction = &ReceiptTransaction;
                            std::set <__int64> ReceiptItemKeys;
                            TDBTables::GetOrderKeys(DBTransaction, CurrentTable, ReceiptItemKeys);
                            TDBOrder::GetOrdersFromOrderKeys(DBTransaction, ReceiptTransaction.Orders, ReceiptItemKeys);
                            if(ReceiptTransaction.Orders->Count > 0)
                            {
                                ReceiptTransaction.Recalc();
                                TManagerDelayedPayment::Instance().MoveOrderToTab(ReceiptTransaction,true);
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

				// Get User Otpions
				bool InformChef = (MessageBox("Do you wish to send a cancellation notice to the Chef?", "Inform Chef?",
						MB_YESNO + MB_ICONQUESTION) == IDYES);

				bool ReturnStock = (MessageBox("Do you wish to Return this Item to Stock. \n(i.e. Put it back on the shelf)",
						"Return To Stock?", MB_YESNO + MB_ICONQUESTION) == IDYES);

				Database::TDBTransaction DBTransaction(DBControl);
				DBTransaction.StartTransaction();

				std::auto_ptr <TPaymentTransaction> TempTransaction(new TPaymentTransaction(DBTransaction));
				std::auto_ptr <TList> OrdersList(new TList);
				TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), ItemsToBeCanceled);

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
                   else if(order1->TabType != TabTableSeat)
                    {
                       tabTableName =  order1->TabName + " : " + order1->TabName;
                    }
 
                    for( ; it != itemGroupsToCancel.end() ; it++ )
                    {
                        _onItemsCanceled( it->second,tabTableName);
                        it->second->Clear();
                    }

                    itemGroupsToCancel.clear();

					std::auto_ptr <TReqPrintJob> Request(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
					Request->Transaction = TempTransaction.get();
					Request->SenderType = devPC;
					Request->Waiter = CancelUserInfo.Name;
					Request->Transaction->Money.Recalc(*Request->Transaction);

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
    Database::TDBTransaction DBTransaction(DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
    DBTransaction.StartTransaction();

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
			         BillItems(DBTransaction, ItemsToBill, eTransOrderSet);
				}


			}
		}
		else
		{
			MessageBox("No Seats to bill!", "Error", MB_OK + MB_ICONERROR);
		}
		DBTransaction.Commit();
		ResetForm();
	}
	catch(Exception & E)
	{
        DBTransaction.Rollback();
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rPlease check this Table is not in use.", "Error", MB_OK + MB_ICONERROR);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnBillSelectedMouseClick(TObject *Sender)
{
    TGlobalSettings::Instance().IsThorBillSelected = true;
    TMMTabType type;
    int noOfTabs = 0;
    int tabKey;
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
					CreditTransaction.Recalc();

					TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(CreditTransaction);
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

                //check whether selected table's selected guest is linked to clipp tab
                TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);

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
                         TDeviceRealTerminal::Instance().PaymentSystem->IsClippSale = false;;

                          //check whether selected table's selected guest is linked to clipp tab
                         TMMTabType allSelectedTabType = TDBTab::GetLinkedTableAndClipTab(DBTransaction, tabKey, true);
                         if(allSelectedTabType == TabClipp)
                         {
                            CurrentSelectedTab = tabKey;
                         }

                        //All itms are being billed on clipp so close Tab
                        if( (CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp) || (sendCloseTabMessage && allSelectedTabType == TabClipp ) ) && (sendCloseTabMessage))
                        {
                            int tableKey = TDBTab::GetTableKeyFromSeat(DBTransaction, CurrentSelectedTab);
                            TDBTables::UpdateTablePartyName(DBTransaction, tableKey);
                            TDeviceRealTerminal::Instance().PaymentSystem->IsClippSale = true;
                            TMMProcessingState State(Screen->ActiveForm, "Processing Transaction With Clipp Please Wait...", "Processing Clipp Transaction");
                            TDeviceRealTerminal::Instance().ProcessingController.Push(State);
                            TManagerClippIntegration::Instance()->CloseTab(CurrentSelectedTab);
                            TManagerClippIntegration::Instance()->OnCloseTabTerminate = &(CloseTerminateCallBack);
                        }
                        else
                        {

                              BillItems(DBTransaction, SelectedItemKeys, transactionType );
                        }

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

                    if((CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp)) && (!sendCloseTabMessage) && (TDeviceRealTerminal::Instance().PaymentSystem->isPaymentProcessed))
                    {
                        //send tab details if some items are selected for bill
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->SendTabDetails(CurrentSelectedTab);
                    }

                    ResetForm();
			    }
                catch(Exception & E)
                {
                    DBTransaction.Rollback();
                    throw;
                }
            }

		}
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
			bool Proceed = TabStaffAccessOk(DBTransaction);
			DBTransaction.Commit();
			if (Proceed)
			{
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

                        PatronCount = DeterminePatronCount();
						BillItems(DBTransaction, SelectedItemKeys, eTransPartialPayment);

					}
				}
				else
				{
					MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
				}
                //check whether selected table's selected guest is linked to clipp tab
                TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);

				DBTransaction.Commit();

                //If tab is clipp tab than send the detail
                if((CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp)) && (TDeviceRealTerminal::Instance().PaymentSystem->isClippTabFullyPaid) && (TDeviceRealTerminal::Instance().PaymentSystem->isPaymentProcessed))
                {
                    //close clipp full payment done by partial button
                    TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                    sendClippTabKey->CloseTab(CurrentSelectedTab);
                }
                else if((CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp)) && (TDeviceRealTerminal::Instance().PaymentSystem->isPaymentProcessed))
                {
                    //Send tab Details if payment is done partially
                    TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                    sendClippTabKey->SendTabDetails(CurrentSelectedTab);
                }

                ResetForm();
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);

		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnSplitPaymentMouseClick(TObject *Sender)
{
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
			bool Proceed = TabStaffAccessOk(DBTransaction);
			DBTransaction.Commit();
			int splittedItemKey = 0;
			if (Proceed)
			{
				DBTransaction.StartTransaction();

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

						PatronCount = DeterminePatronCount();
						splittedItemKey = BillItems(DBTransaction, SelectedItemKeys, eTransSplitPayment);

					}
				}
				else
				{
					MessageBox("Nothing to Bill.", "Info", MB_OK + MB_ICONINFORMATION);
				}

				TDBTab::ReleaseTab(DBTransaction, TDeviceRealTerminal::Instance().ID.Name, 0);

                 //check whether selected table's selected guest is linked to clipp tab
                TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);

				DBTransaction.Commit();
				ResetForm();

                //If tab is clipp tab than send the detail
                if((CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp)) && (TDeviceRealTerminal::Instance().PaymentSystem->isPaymentProcessed) && (TDeviceRealTerminal::Instance().PaymentSystem->splittedDivisionLeft))
                {
                    TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                    sendClippTabKey->SendTabDetails(BackupCurrentSelectedTab);
                }
                 //If tab is clipp tab than send the detail
                else if((CurrentTabType == TabClipp ||  (CurrentTabType == TabTableSeat && type == TabClipp)) && (TDeviceRealTerminal::Instance().PaymentSystem->isPaymentProcessed) && (!TDeviceRealTerminal::Instance().PaymentSystem->splittedDivisionLeft))
                {
                    TManagerClippIntegration* closeClippTab = TManagerClippIntegration::Instance();
                    closeClippTab->CloseTab(BackupCurrentSelectedTab);
                }

				DBTransaction.StartTransaction();
				// Now reslect all the remaining items.
				if(splittedItemKey > 0)
					SelectedItemKeys.insert(splittedItemKey);
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
				UpdateContainerListColourDisplay();
				DBTransaction.Commit();
				ShowReceipt();
			}
		}
	}
	catch(Exception & E)
	{
		MessageBox("Unable to process this bill.\r" "Please report the following message to your service provider :\r\r" + E.Message +
			"\r\rYou may need to reboot the system.", "Error", MB_OK + MB_ICONERROR);

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

        if(CurrentTabType== TabClipp )
        {
          Transfer->ClipPresentInFromPanel =true;
          Transfer->ClipPresentInToPanel =false;

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
		UpdateSeatDetails(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());
		DBTransaction.Commit();
        delete Transfer;
		ShowReceipt();
	}
}
//----------------------------------------------------------------------------
void __fastcall TfrmBillGroup::btnCloseMouseClick(TObject *Sender)
{
    if(TGlobalSettings::Instance().IsThorlinkSelected)
    {
      RemoveThorMembership();
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
	UpdateContainerListColourDisplay();
	DBTransaction.Commit();
	CheckLoyalty();
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
		if (AddToSelectedTabs(DBTransaction, SelectedTab))
		{
			ItemSetAddItems(DBTransaction, SelectedTab);
			CurrentSelectedTab = SelectedTab;
		}
	}
	UpdateRightButtonDisplay(Sender);
	IgnoreItemThreshhold = false;
	UpdateItemListDisplay(DBTransaction);
	UpdateContainerListColourDisplay();
    if(lbeMembership->Visible == false)//todo-Arpit
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
					std::auto_ptr <TList> OrdersList(new TList);
					std::set <__int64> ItemsToBeMoved;
					for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end();
						advance(itItem, 1))
					{
						ItemsToBeMoved.insert(itItem->first);
                        source_key = itItem->first;
					}
					TDBOrder::GetOrdersFromOrderKeys(DBTransaction, OrdersList.get(), ItemsToBeMoved);
					if (TDBOrder::CheckTransferCredit(DBTransaction, OrdersList.get(), TabTransferTo))
					{
						TDBOrder::TransferOrders(DBTransaction, OrdersList.get(), TabTransferTo, TDeviceRealTerminal::Instance().User.ContactKey, source_key);
                       // if items are moved to guest which is  linked to clip tab then update the clip tab
                        if(TDBTab::GetLinkedTableAndClipTab(DBTransaction,TabTransferTo,true)  )
                        {
                           TManagerClippIntegration* updateClippTab = TManagerClippIntegration::Instance();
                           updateClippTab->SendTabDetails(TabTransferTo);
                        }
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
				UpdateTableDetails(DBTransaction);
				UpdateSeatDetails(DBTransaction, TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get());

				DBTransaction.Commit();
                //Send tab details back if selected tab is clipp tab.
                if(CurrentTabType == TabClipp)
                {
                    TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                    sendClippTabKey->SendTabDetails(CurrentSelectedTab);
                }
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

            //Get Table 's guest is linked to clipp tab
            TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);
			DBTransaction.Commit();

            //Send tab details back if selected tab is clipp tab.
            if(CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp))
            {
                TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                sendClippTabKey->SendTabDetails(CurrentSelectedTab);
            }
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

                    //Get Table 's guest is linked to clipp tab
                    TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);
					DBTransaction.Commit();

                    //Send tab details back if selected tab is clipp tab.
                     if(CurrentTabType == TabClipp|| (CurrentTabType == TabTableSeat && type == TabClipp))
                    {
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->SendTabDetails(CurrentSelectedTab);
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
	SelectedZone();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmBillGroup::CardSwipe(Messages::TMessage& Message)
{
    TGlobalSettings::Instance().IsDiscountSelected = false;
	Database::TDBTransaction DBTransaction(DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
	try
	{
        if (CurrentTabType == TabWeb)
        {
            MessageBox("Membership Cannot be applied on Web Order billing" , "Warning", MB_OK + MB_ICONWARNING);
            btnTransfer->Color = clSilver;
            btnTransfer->Enabled = false;
            btnApplyMembership->Enabled = false;
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
                if(TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected)
                {
                    ProcessBillThorVouchers(DBTransaction);
                    TGlobalSettings::Instance().IsProcessThorVoucher = false ;
                }
            }
            else if (Result == lsAccountBlocked)
            {
                MessageBox("Account Blocked " + TempUserInfo.Name + " " + TempUserInfo.AccountInfo, "Account Blocked", MB_OK + MB_ICONINFORMATION);
            }
            DBTransaction.Commit();
            ShowReceipt();
	    }
    }
	catch(Exception & E)
	{
		MessageBox("Unable to Apply Membership :" + E.Message, "Error", MB_OK + MB_ICONERROR);
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

        if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
        {
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
                            ApplyMembership(DBTransaction, TempMembershipInfo);
                            if(TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected)
                            {
                                ProcessBillThorVouchers(DBTransaction);
                                TGlobalSettings::Instance().IsProcessThorVoucher = false ;
                            }
                            TDBTab::SetTabOrdersLoyalty(DBTransaction,CurrentSelectedTab, TempMembershipInfo.ContactKey);
                            //check whether selected table's selected guest is linked to clipp tab
                            TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);
                            //Send tab details back if selected tab is clipp tab.
                            if(CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp))
                            {
                                TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                                sendClippTabKey->SendTabDetails(CurrentSelectedTab);
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
	}
	else
	{
		MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
	}

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
						PaymentTransaction.Recalc();
                  		PaymentTransaction.DeleteOrders();
					}
				}
				else
				{
                    bool applyDiscount = true;

					if (OrderKeySet.size())
					{
						TPaymentTransaction PaymentTransaction(DBTransaction);
						TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, OrderKeySet);
						TDiscount CurrentDiscount;
                        TSeniorCitizenDiscountChecker SCDChecker;
						CurrentDiscount.DiscountKey = SelectDiscount->Key;
						ManagerDiscount->GetDiscount(DBTransaction, CurrentDiscount.DiscountKey, CurrentDiscount);
                        applyDiscount = SCDChecker.SeniorCitizensCheck(CurrentDiscount, PaymentTransaction.Orders);
                        if(SCDChecker.SeniorCitizensCheck(CurrentDiscount, PaymentTransaction.Orders, true) && applyDiscount && CurrentTabType == TabClipp)
                        {
                           isSCDAppliedOnClipp = true;
                        }
                        /*if(applyDiscount)
                        {
                            ManagerDiscount->ClearDiscount(PaymentTransaction.Orders, CurrentDiscount);
                            ManagerDiscount->AddDiscount(PaymentTransaction.Orders, CurrentDiscount);
                            PaymentTransaction.Recalc();
                            ManagerDiscount->SetDiscountAmountDB(DBTransaction, PaymentTransaction.Orders);
                        }*/
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
                else
                {
                    //check whether selected table's selected guest is linked to clipp tab
                    TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);
                    if(CurrentTabType == TabClipp || (CurrentTabType == TabTableSeat && type == TabClipp))
                    {
                        //send tab details if discount is applied
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->SendTabDetails(CurrentSelectedTab);
                    }
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
void __fastcall TfrmBillGroup::ProcessBillThorVouchers(Database::TDBTransaction &DBTransaction)
{
	try
	{
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
			SelectDiscount->MessageType = eThorDiscountReason;
            SelectDiscount->ShowPointsAsDiscount = false;
            TGlobalSettings::Instance().IsProcessThorVoucher = true;
            if(ManagerDiscount->IsVouchersAvailable())
            {
			    if (SelectDiscount->ShowModal() == mrOk)
                {
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
                            ManagerDiscount->ClearThorVouchersDiscounts(PaymentTransaction.Orders);
                            ManagerDiscount->SetDiscountAmountDB(DBTransaction, PaymentTransaction.Orders);
                            PaymentTransaction.Recalc();
                            PaymentTransaction.DeleteOrders();
                        }
                    }
                    else
                    {
                        SelectedDiscount.DiscountKey = ManagerDiscount->GetDiscountKeyForVoucher(SelectDiscount->Key);
                        if (OrderKeySet.size() && SelectedDiscount.DiscountKey > 0)
                        {
                            ManagerDiscount->GetDiscount(DBTransaction, SelectedDiscount.DiscountKey, SelectedDiscount);
                            TPaymentTransaction PaymentTransaction(DBTransaction);
                            TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, OrderKeySet);
                            TSeniorCitizenDiscountChecker SCDChecker;
                            ManagerDiscount->ClearThorVouchersDiscounts(PaymentTransaction.Orders);
                            if(SCDChecker.SeniorCitizensCheck(SelectedDiscount, PaymentTransaction.Orders))
                            {
                              Membership.Member.AutoAppliedDiscounts.clear();
                              Membership.Member.AutoAppliedDiscounts.insert(SelectedDiscount.DiscountKey);
                              PaymentTransaction.ApplyMembership(Membership);
                              //ApplyDiscount(DBTransaction,SelectedDiscount.DiscountKey,dsMMMembership);
                            }
                          PaymentTransaction.Recalc();
                          PaymentTransaction.DeleteOrders();
                        }
                        else if(SelectedDiscount.DiscountKey == 0)
                        {
                           MessageBox("No discount for this voucher has been setup in Menumate.", "Warning", MB_ICONWARNING + MB_OK);
                        }
                    }
                    if(CurrentTabType == TabClipp)
                    {
                        TManagerClippIntegration* sendClippTabKey = TManagerClippIntegration::Instance();
                        sendClippTabKey->SendTabDetails(CurrentSelectedTab);
                    }

                }
            }
		}
        if(SelectedDiscount.IsComplimentaryDiscount() || SelectedDiscount.IsNonChargableDiscount())
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
       CurrentDiscount.IsThorBill = TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected;
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

				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else if (frmDiscount->Mode == DiscModeSetPrice)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);
				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
            	if (frmDiscount->Mode == DiscModeItem)
			{
				CurrentDiscount.Amount = RoundToNearest(frmDiscount->CURResult, MIN_CURRENCY_VALUE,
					TGlobalSettings::Instance().MidPointRoundsDown);

				if (CurrentDiscount.Amount != frmDiscount->CURResult)
				{
					MessageBox("The Discount has been rounded!.", "Warning", MB_ICONWARNING + MB_OK);
				}
			}
			else
			{
				CurrentDiscount.PercentAmount = frmDiscount->PERCResult;
			}
		}
		else
		{
			ProcessDiscount = false;
		}
	}

	if (ProcessDiscount)
	{
      std::set <__int64> SelectedItemKeys;
      for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
      {
         SelectedItemKeys.insert(itItem->first);
      }

      TPaymentTransaction PaymentTransaction(DBTransaction);
      TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, SelectedItemKeys);
      ManagerDiscount->AddDiscount(PaymentTransaction.Orders, CurrentDiscount);
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
            CurrentSelectedTab = SelectedTab;
            SplitItemsInSet(DBTransaction, SelectedTab);
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

                    if (AddToSelectedTabs(DBTransaction, SelectedTab))
                    {
                        SplitItemsInSet(DBTransaction, SelectedTab);
                        ItemSetAddItems(DBTransaction,SelectedTab);
                        CurrentSelectedTab = SelectedTab;
                    }
                }
            }
        }
        UpdateRightButtonDisplay(Sender);
        IgnoreItemThreshhold = false;
        UpdateItemListDisplay(DBTransaction);
        UpdateContainerListColourDisplay();
        UpdateSplitButtonState();
         if(lbeMembership->Visible == false && Membership.Member.AutoAppliedDiscounts.size()>0) //todo-Arpit
        {
           RemoveMembershipDiscounts(DBTransaction);
        }

         if(TGlobalSettings::Instance().IsClippIntegrationEnabled)
        {
            CheckingClipItemsInSelectedList(DBTransaction);
        }
        DBTransaction.Commit();
        if(!TGlobalSettings::Instance().IsThorlinkSelected)
        {
            CheckLoyalty();
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
        TSeniorCitizenDiscountChecker SCDChecker;
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();

        std::set <__int64> SelectedItemKeys;
        for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
        {
            SelectedItemKeys.insert(itItem->first);
        }

        bool canAddItem = SCDChecker.ItemSelectionCheck(DBTransaction, GridButton->Tag, SelectedItemKeys);

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
    else
    {

    }
	ShowReceipt();


    if(TGlobalSettings::Instance().IsClippIntegrationEnabled)
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        CheckingClipItemsInSelectedList(DBTransaction);
        DBTransaction.Commit();
    }
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
   SplitTimer->Enabled = true;
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
            DeselectItem(GridButton);
        }
}
//---------------------------------------------------------------------------
void TfrmBillGroup::SelectItem(TGridButton *GridButton)
{
    TSeniorCitizenDiscountChecker SCDChecker;
    Database::TDBTransaction DBTransaction(DBControl);
    DBTransaction.StartTransaction();

    std::set <__int64> SelectedItemKeys;
    for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
    {
        SelectedItemKeys.insert(itItem->first);
    }

    bool canAddItem = SCDChecker.ItemSelectionCheck(DBTransaction, GridButton->Tag, SelectedItemKeys);

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
	    CheckLoyalty();
    }
    else
    {

    }
	ShowReceipt();


    if(TGlobalSettings::Instance().IsClippIntegrationEnabled)
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        CheckingClipItemsInSelectedList(DBTransaction);
        DBTransaction.Commit();
    }
}
void TfrmBillGroup::SplitItemOnClick(int itemSelected)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (this));
    frmTouchNumpad->Caption = "Enter Quantity";
    frmTouchNumpad->btnSurcharge->Caption = "Ok";
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = true;
    frmTouchNumpad->View = viewQuantity;
    if (frmTouchNumpad->ShowModal() == mrOk && frmTouchNumpad->splitValue > 0 &&
        SelectedItems[itemSelected].Qty >= frmTouchNumpad->splitValue)
    {
        Database::TDBTransaction DBTransaction(DBControl);
        DBTransaction.StartTransaction();
        SplitItem(DBTransaction,itemSelected,frmTouchNumpad->splitValue);
//        UpdateItemListDisplay(DBTransaction);
        if(frmTouchNumpad->splitValue < SelectedItems[itemSelected].Qty)
           RefreshItemStatus(frmTouchNumpad->CURResult,itemSelected,DBTransaction);
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
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
            tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Latched =
                                                         (ptrItem1->Qty  - (int)ptrItem1->Qty) > 0;
            tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
            SelectedItems.erase(itemSelected);
            if(ptrItem1->IsParent)
            {
                TItemComplete* Order = new TItemComplete();
                Order->OrderKey = itemSelected;
                TDBOrder::GetOrderIncludingSidesFromOrderKey(DBTransaction,Order);
                for (int j = 1; j <= Order->SubOrders->Count ; j++)
                {
                    TItemCompleteSub *SubOrder = (TItemCompleteSub *)Order->SubOrders->Items[j-1];
                    qtyStr = FormatFloat("0.00", SubOrder->GetQty()) + " ";
                    if(ptrItem1->Qty != 1)
                        tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Caption = qtyStr + SubOrder->Item;
                    else
                        tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Caption = SubOrder->Item;
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_BUTTONCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_EMPTY][ATTRIB_FONTCOLOR];
                    tgridItemList->Buttons[i+j][ITEM_LIST_COLUMN]->Latched =
                                                                 (SubOrder->GetQty()  - (int)ptrItem1->Qty) > 0;
                    tgridItemList->Buttons[i+j][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_UNSELECTED][ATTRIB_BUTTONCOLOR];
                    SelectedItems.erase(SubOrder->OrderKey);
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
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->Color = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_BUTTONCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_COLUMN]->LatchedFontColor = ButtonColors[BUTTONTYPE_SELECTED][ATTRIB_FONTCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Color = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_BUTTONCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->FontColor = ButtonColors[BUTTONTYPE_SEC_SELECTED][ATTRIB_FONTCOLOR];
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Tag = ptrItem->Key;
        tgridItemList->Buttons[i][ITEM_LIST_ENABLE_COLUMN]->Caption = "Select";
        SelectedItems[ptrItem->Key] = VisibleItems[ptrItem->Key];
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
        bool doSplit = quantity < 10;
        std::map<__int64, TPnMOrder>::iterator i = orders.begin();
        std::map<__int64, TPnMOrder>::iterator j = orders.end();

        for ( ; i != j; i++)
        {
            if ( doSplit && (i->second.IsParent || !i->second.GroupNumber)
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
		for (std::map <__int64, TPnMOrder> ::iterator itItem = VisibleItems.begin(); itItem != VisibleItems.end(); advance(itItem, 1))
		{
			SortingList->Add(&itItem->second);
		}
		//SortingList->Sort(ComparePickNMix);
		tgridItemList->RowCount = 0; // Clears all the Latching.
		tgridItemList->ColCount = 2;
		tgridItemList->RowCount = VisibleItems.size();

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

			if (CurrentDisplayMode == eInvoices || CurrentTabType == TabDelayedPayment)
			{
				tbtnMove->Enabled = false;
			}
			else
			{
				tbtnCancel->Enabled = true;
				tbtnMove->Enabled = true;
				tbtnSplit->Enabled = true;
			}

            //check whether selected table's selected guest is linked to clipp tab
            TMMTabType type = TDBTab::GetLinkedTableAndClipTab(DBTransaction, CurrentSelectedTab, true);
            if(CurrentTabType == TabTableSeat && type == TabClipp)
            {
                tbtnMove->Enabled = false;
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
                    case TabClipp:
                            lbePartyName->Caption = "Clipp Tabs";
                            break;
		}

		if (CurrentTabType == TabNormal || CurrentTabType == TabDelayedPayment || CurrentTabType == TabClipp)
		{
            if(CurrentTabType == TabClipp)
            {
                TDBClippTab::GetOpenClippTabs(DBTransaction, TabList, CurrentTabType);

            }
            else
            {
			    TDBTab::GetTabs(DBTransaction, TabList, CurrentTabType);
            }
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
            if(TDBTab::GetLinkedTableAndClipTab(DBTransaction,(int)TabList->Objects[i],true)  )
            {
                 ClipTabInTable=true;
            }
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

	IgnoreItemThreshhold = false;

	UpdateItemListDisplay(DBTransaction);
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
			for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
			{
				ReceiptItemKeys.insert(itItem->first);
			}

			TDBOrder::GetOrdersFromOrderKeys(DBTransaction, ReceiptTransaction.Orders, ReceiptItemKeys);
            if (CurrentTabType == TabDelayedPayment)
			  {
                TItemComplete *Order = (TItemComplete*)(ReceiptTransaction.Orders->Items[0]);
                ReceiptTransaction.InvoiceNumber = Order->DelayedInvoiceNumber;
              }
			LoadCustNameAndOrderType(ReceiptTransaction);

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

			ReceiptTransaction.Recalc();
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

			ReceiptTransaction.Recalc();
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
int TfrmBillGroup::BillItems(Database::TDBTransaction &DBTransaction, const std::set <__int64> &ItemsToBill,
	TPaymentTransactionType TransType)
{
       int retVal = 0;
	try
	{

 		TPaymentTransaction PaymentTransaction(DBTransaction);
		PaymentTransaction.ApplyMembership(Membership);

        TDBOrder::GetOrdersFromOrderKeys(DBTransaction, PaymentTransaction.Orders, ItemsToBill);

        TMMContactInfo Member;
        if(SelectedDiscount.IsComplimentaryDiscount())
          {
             PaymentTransaction.TypeOfSale = ComplimentarySale;
          }
         else if( SelectedDiscount.IsNonChargableDiscount())
          {
             PaymentTransaction.TypeOfSale = NonChargableSale;
          }
		if (CurrentDisplayMode == eTabs)
		{
			PaymentTransaction.SalesType = eTab;
			if (CurrentTabType == TabWeb)
			{
				PaymentTransaction.SalesType = eWeb;
                PaymentTransaction.ChitNumber.DeliveryTime = delivery_time; //.FormatString("DD/MM/YYYY hh:nn am/pm");
                PaymentTransaction.WebOrderKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, CurrentSelectedTab);

                std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                TDBWebUtil::getWebOrderExtraData(DBTransaction, PaymentTransaction.WebOrderKey, "DELIVERY", WebDeliveryDetials.get());

                std::auto_ptr<TStringList>WebDetials(new TStringList);
                TDBWebUtil::getWebOrderDetials(DBTransaction, PaymentTransaction.WebOrderKey, *WebDetials.get());
                _getWebOrderMemberDetails(DBTransaction, PaymentTransaction, WebDeliveryDetials.get(), WebDetials.get(), PaymentTransaction.WebOrderKey, delivery_time);

			}
		}
		else if (CurrentDisplayMode == eRooms)
		{
			PaymentTransaction.SalesType = eRoomSale;
			PaymentTransaction.BookingID = TDBRooms::GetBookingID(DBTransaction, CurrentRoom);
			PaymentTransaction.RoomNumber = CurrentRoom;
			PaymentTransaction.MiscPrintData["PartyName"] = TDBRooms::GetPartyName(DBTransaction, CurrentRoom);
		}
		else if (CurrentDisplayMode == eInvoices)
		{
			PaymentTransaction.SalesType = eAccount;
			PaymentTransaction.InvoiceKey = CurrentInvoiceKey;    //unused
		}
		else
		{
			PaymentTransaction.SalesType = eTableSeat;
			PaymentTransaction.MiscPrintData["PartyName"] = TDBTables::GetPartyName(DBTransaction, CurrentTable);
		}

		try
		{

            LoadCustNameAndOrderType(PaymentTransaction);

			PaymentTransaction.Recalc();
			PaymentTransaction.Type = TransType;

			TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, PaymentTransaction.Patrons, PatronCount);

            //MM-1649: The patron count which was asked by user when table was selected has to be assigned to the current payment transaction
            if(!PatronTypes.empty() && (TGlobalSettings::Instance().PromptForPatronCount || (PaymentTransaction.SalesType = eTableSeat && TGlobalSettings::Instance().PromptPatronCountOnTableSales)))
            {
                PaymentTransaction.Patrons = PatronTypes;
            }

            if( CurrentDisplayMode == eInvoices )
            {
                // use the InvoicePaymentSystem as it allows paying multiple invoices and acts different on partial n split payments
                std::auto_ptr<TMMInvoicePaymentSystem> invoicePaymentSystem( new TMMInvoicePaymentSystem() );
                invoicePaymentSystem->ProcessTransaction(PaymentTransaction);

                            // display last receipt if any
                _displayLastReceipt( DBTransaction, invoicePaymentSystem->LastReceipt );
            }
            else
            {
                TDeviceRealTerminal::Instance().PaymentSystem->ProcessTransaction(PaymentTransaction, false );
                // display last receipt if any
                _displayLastReceipt( DBTransaction, TDeviceRealTerminal::Instance().PaymentSystem->LastReceipt );
            }

            if(PaymentTransaction.SalesType == eTableSeat)
            {
               if(TDBTables::GetTableExists(DBTransaction,CurrentTable))
                {
                    //check that table is free
                    if(TDBTables::IsEmpty(DBTransaction,CurrentTable))
                    {
                      TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,eNoneStatus);
                    }
                    else if(PaymentTransaction.Type == eTransSplitPayment || PaymentTransaction.Type == eTransPartialPayment)
                    {
                      TDBTables::SetTableBillingStatus(DBTransaction,CurrentTable,ePartialSplit);
                    }
                }
            }
            //changes to get points values..
            if(TGlobalSettings::Instance().IsRunRateBoardEnabled)
            {
                SendPointValueToRunRate(PaymentTransaction);
            }

        }
		__finally
		{
			retVal  = PaymentTransaction.SplittedItemKey;
			PaymentTransaction.DeleteOrders();
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
                if(!TGlobalSettings::Instance().IsThorlinkSelected)
                {
				if (TDeviceRealTerminal::Instance().Modules.Status[eRegMembers]["Enabled"])
				{
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
				}
				else
				{
					MessageBox("Membership is not Enabled.", "Error", MB_OK + MB_ICONERROR);
				}
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
				TFloorPlanReturnParams floorPlanReturnParams;

				// Runs new web app of floorPlan
				if( TEnableFloorPlan::Instance()->Run( ( TForm* )this, false, floorPlanReturnParams ) )
				{
                    CurrentDisplayMode = eTables;
                    CurrentTabType     = TabTableSeat;
                    UpdateRightButtonDisplay(NULL);
                    CurrentTable       = floorPlanReturnParams.TabContainerNumber;
                    int i=     floorPlanReturnParams.TabContainerNumber;
                    CheckLinkedTable(floorPlanReturnParams.TabContainerNumber);
                    ResetForm();
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
            //new cas added for clipp tab
            case TabClipp:
			{
				CurrentDisplayMode = eTabs;
				CurrentTabType = TabClipp;
				UpdateRightButtonDisplay(NULL);
				CurrentTable = 1;
				ResetForm();
          	}break;
		}

        EnableButtons();
		RetVal = CurrentDisplayMode;
	}
	DBTransaction.Commit();
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
//---------------------------------------------------------------------------
 void TfrmBillGroup::UpdateBillGroupBtnVisibilityInClipTabCase(Database::TDBTransaction &DBTransaction,std::map<__int64,TPnMOrder> &SelectedItems)
 {


      if(CheckIfListContainOnlyClipItems(DBTransaction,SelectedItems)==0)
      {   //only clip tab case
          btnBillTable->Enabled =false;
          tbtnMove->Enabled     =   false;
          EnableButtons();

      } else if(CheckIfListContainOnlyClipItems(DBTransaction,SelectedItems)==1)
      {
        //combo case  ,mixture of linked clip tab and normal items
         DisableButtons();
      }else if(CheckIfListContainOnlyClipItems(DBTransaction,SelectedItems)==2)
      { //normal case
        if(CurrentDisplayMode == eTables && !ClipTabInTable)
         {
            btnBillTable->Enabled =true;

         }else
         {
            btnBillTable->Enabled =false;
         }
        EnableButtons();
      }




  /*    if(CheckIfListContainOnlyClipItems(DBTransaction,SelectedItems))
      {  //if only clip tab is present then enable buttons
         EnableButtons();
      }
      else
      {
      // if it contain other items then disable al the required buttons
         DisableButtons();
      }*/
 }
//----------------------------------------------------------------------
void TfrmBillGroup::CheckingClipItemsInSelectedList(Database::TDBTransaction &DBTransaction)
{
   UpdateBillGroupBtnVisibilityInClipTabCase(DBTransaction,SelectedItems);
}
//-----------------------------------------------------------------------------------------------------------
void TfrmBillGroup::EnableButtons()
{

        btnBillSelected->Enabled    =   true;
        btnPartialPayment->Enabled  =   true;
        btnSplitPayment->Enabled    =   true;
        tbtnCancel->Enabled         =   true;
        //tbtnSplit->Enabled          =   true;
        btnApplyMembership->Enabled =   true;
        tbtnDiscount->Enabled       =   true;
}
//------------------------------------------------------------------------------------------
void TfrmBillGroup::DisableButtons()
{

      //      btnBillTable->Enabled       =   false;
            tbtnMove->Enabled           =   false;
            btnBillTable->Enabled       =   false;
            btnBillSelected->Enabled    =   false;
            btnPartialPayment->Enabled  =   false;
            btnSplitPayment->Enabled    =   false;
            tbtnCancel->Enabled         =   false;
        //    tbtnSplit->Enabled          =   false;
            btnApplyMembership->Enabled =   false;
            tbtnDiscount->Enabled       =   false;

}
//------------------------------------------------------------------------------------------
int  TfrmBillGroup::CheckIfListContainOnlyClipItems(Database::TDBTransaction &DBTransaction,std::map<__int64,TPnMOrder> &SelectedItems)
{

   bool linkedClipTabPresent=false;
   int linkedTabKey;


   std::set <__int64> SelectedTabItems;
   int size= SelectedItems.size();
   if(SelectedItems.size()>0)
   {
         for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
         {
            SelectedTabItems.insert(itItem->second.TabKey);

            if(!linkedClipTabPresent)
            {
               if(TDBTab::GetLinkedTableAndClipTab(DBTransaction,itItem->second.TabKey,true)  )
                {
                   linkedClipTabPresent=true;
                   linkedTabKey = itItem->second.TabKey;

                }
            }

         }

      if(linkedClipTabPresent && SelectedTabItems.size() ==1 )
      {   // if only linked clip tab items are selected
          return 0;
      }else if( linkedClipTabPresent &&  SelectedTabItems.size() >1  )
      {
      // if linked clip tab item and normal item is selected
         return 1;
      }else if ( !linkedClipTabPresent)
      {
      // if no linked clip tab item is selected ,Normal case
        return 2;
      }

   }

  return 2;

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
			Membership.Assign(MembershipInfo, MemberSource);
			lbeMembership->Visible = true;
            if(TGlobalSettings::Instance().MembershipType == MembershipTypeThor && TGlobalSettings::Instance().IsThorlinkSelected)
            {
              lbeMembership->Caption = MembershipInfo.Name + " " + " Points:" + FormatFloat("0.00", MembershipInfo.Points.getPointsBalance());
            }
            else
            {
			  lbeMembership->Caption = MembershipInfo.Name + " (" + MembershipInfo.MembershipNumber + ")" + " Points:" + FormatFloat("0.00", MembershipInfo.Points.getPointsBalance());
            }
            // Strip any discounts that are membership based.
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
             TMembership *membershipSystem = TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem.get();
	  }
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::RemoveThorMembership()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
	DBTransaction.StartTransaction();
    RemoveMembership(DBTransaction);
	DBTransaction.Commit();
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::RemoveMembership(Database::TDBTransaction &DBTransaction)
{
	MembershipConfirmed = false;
	Membership.Clear();
	lbeMembership->Visible = false;
	lbeMembership->Caption = "";
    RemoveMembershipFreeItems(DBTransaction);
   	RemoveMembershipDiscounts(DBTransaction);
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
  if(!Membership.Applied())
     return;
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
	if (!MembershipConfirmed)
	{
		if (SelectedItems.size() == 0)
		{
			Membership.Clear();
			lbeMembership->Visible = false;
			lbeMembership->Caption = "";
			ShowReceipt();
		}
		else
		{
			std::set <__int64> PossiableMembers;
			std::set <__int64> ReceiptItemKeys;
			for (std::map <__int64, TPnMOrder> ::iterator itItem = SelectedItems.begin(); itItem != SelectedItems.end(); advance(itItem, 1))
			{
				ReceiptItemKeys.insert(itItem->first);
			}

			Database::TDBTransaction DBTransaction(DBControl);
			TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
			DBTransaction.StartTransaction();
			TDBOrder::GetMemberKeysFromOrderKeys(DBTransaction, PossiableMembers, ReceiptItemKeys);

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
							TLoginSuccess Result = TDeviceRealTerminal::Instance().ManagerMembership->GetMember(DBTransaction, TempUserInfo,
								MemberSource);

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
			DBTransaction.Commit();
		}
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::OnSmartCardInserted(TSystemEvents *Sender)
{
	TMMContactInfo TempUserInfo;
	TDeviceRealTerminal::Instance().ManagerMembership->ManagerSmartCards->GetContactInfo(TempUserInfo);
	if (TempUserInfo.Valid())
	{ // Restore Membership, Reminds the user to remove the smart card.
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
        TManagerLoyaltyVoucher ManagerLoyaltyVoucher;
        ManagerLoyaltyVoucher.DisplayMemberVouchers(DBTransaction,TempUserInfo);
		ApplyMembership(DBTransaction, TempUserInfo);
		DBTransaction.Commit();
		ShowReceipt();
	}
}
// ---------------------------------------------------------------------------
void TfrmBillGroup::OnSmartCardRemoved(TSystemEvents *Sender)
{
	if (TDeviceRealTerminal::Instance().Modules.Status[eSmartCardSystem]["Enabled"])
	{
		Database::TDBTransaction DBTransaction(DBControl);
		TDeviceRealTerminal::Instance().RegisterTransaction(DBTransaction);
		DBTransaction.StartTransaction();
		RemoveMembership(DBTransaction);
		DBTransaction.Commit();
		TDeviceRealTerminal::Instance().ManagerMembership->EndMemberTransaction();
        ShowReceipt();
	}
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


