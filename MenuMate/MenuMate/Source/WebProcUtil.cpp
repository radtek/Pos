// ---------------------------------------------------------------------------

#pragma hdrstop

#include "WebProcUtil.h"
#include "MM_DBCore.h"
#include "DeviceRealTerminal.h"
#include "ReqPrintJob.h"
#include "DBOrder.h"
#include "DBTab.h"
#include "Comms.h"
#include "DBSecurity.h"
#include "DBSaleTimes.h"
#include "ChitNumberController.h"
#include "ManagerPatron.h"
#include "DocketManager.h"
#include "Invoice.h"
#include "DBRooms.h"
#include "DBWebUtil.h"
#include "DeviceWeb.h"
#include "DBTables.h"
#include "ChefmateClientManager.h"
#include "MMMessageBox.h"
#include "PatronType.h"
#include <vector>
#include <memory>
#include "DBActiveChit.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

void __fastcall TWebProcUtil::ProcessReceipt(Database::TDBTransaction &DBTransaction, TMMTabType TabType, int Primary, int Secondary)
{
	if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
	{
		throw Exception("No receipt printer selected.");
	}
	else
	{
		std::auto_ptr<TReqPrintJob>TempReceipt(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
		TPaymentTransaction ReceiptTransaction(DBTransaction);
		TempReceipt->Transaction = &ReceiptTransaction;

		std::set<__int64>ItemKeys;

		switch(TabType)
		{
		case TabTableSeat: // Tables
			{
				TDBTables::GetOrderKeys(DBTransaction, Primary, ItemKeys);
			}break;
		case TabNormal: // Tabs
		case TabStaff: // Staff
			TDBTab::GetOrderKeys(DBTransaction, Primary, ItemKeys);
			break;
		case TabMember: // Members
        	{
				TabType = TabMember;
                int TabKey = TDBTab::GetTabByOwner(DBTransaction,Primary);
            	TDBTab::GetOrderKeys(DBTransaction, TabKey, ItemKeys);
			}
            break;
		case TabRoom: // Rooms
			TDBTab::GetOrderKeys(DBTransaction, TDBRooms::GetRoomTab(DBTransaction, Primary), ItemKeys);
			break;
		case TabWeb: // Web
			TabType = TabWeb;
		default:
			{

			}break;
		}

		TDBOrder::GetOrdersFromOrderKeys(DBTransaction, ReceiptTransaction.Orders, ItemKeys);

		ReceiptTransaction.Money.CreditAvailable = TDBOrder::LoadCreditFromOrders(DBTransaction, ReceiptTransaction.Orders);
		ReceiptTransaction.Money.Recalc(ReceiptTransaction);

		TempReceipt->JobType = pjReceiptReceipt;
		TempReceipt->SenderType = devPC;
		TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

		std::auto_ptr<TStringList>TabHistory(new TStringList);

		std::set<__int64>SelectedTabs;
		TDBOrder::GetTabKeysFromOrders(ReceiptTransaction.Orders, SelectedTabs);
        bool isTable = false;
        int tabKey = 0;
		for (std::set<__int64>::iterator itTabs = SelectedTabs.begin(); itTabs != SelectedTabs.end(); advance(itTabs, 1))
		{
            tabKey = *itTabs;
            isTable = false;
            if (TabType == TabTableSeat)
                {
                  TTableSeat TableSeat;
                  if (TDBTables::GetTableSeat(DBTransaction, *itTabs, &TableSeat))
                  {
                     tabKey = TableSeat.TableNo;
                     isTable = true;
                  }
                }
            TDBTab::GetPartialPaymentList(DBTransaction, tabKey, TabHistory.get(),isTable);
            for(int x = 0 ; x < TabHistory->Count; x++)
            {
               if(TempReceipt->TabHistory->IndexOf(TabHistory->Strings[x]) == -1)
               {
                 TempReceipt->TabHistory->Add(TabHistory->Strings[x]);
               }
            }
		}

		if (TabType == TabTableSeat)
		{
			AnsiString PartyName = TDBTables::GetPartyName(DBTransaction, Primary);
			TempReceipt->MiscData["PartyName"] = PartyName;
		}
		else if (TabType == TabRoom)
		{
			AnsiString PartyName = TDBRooms::GetPartyName(DBTransaction, Primary);
			TempReceipt->MiscData["PartyName"] = PartyName;
		}

		TempReceipt->SenderType = devPC;
		TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

		if (TabType == TabRoom)
		{
			TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(Primary));
			TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(DBTransaction, Primary));
		}

		TempReceipt->PaymentType = ptPreliminary;

		TempReceipt->PaymentType = ptPreliminary;
		Receipt->GetPrintouts(DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
		TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);

		while (ReceiptTransaction.Orders->Count != 0)
		{
			delete(TItemComplete*)ReceiptTransaction.Orders->Items[0];
			ReceiptTransaction.Orders->Delete(0);
		}
	}
}

void __fastcall TWebProcUtil::ProcessCallAway(Database::TDBTransaction &DBTransaction, int TableNumber, TStringList *Courses)
{
	try
	{
		if (TableNumber != 0)
		{
			std::auto_ptr<TCallAwayComplete>CallAway(new TCallAwayComplete);
			CallAway->TableNo = TableNumber;
			CallAway->TableName = TDBTables::GetTableName(DBTransaction, CallAway->TableNo);
			CallAway->PartyName = TDBTables::GetPartyName(DBTransaction, CallAway->TableNo);
			CallAway->Courses->AddStrings(Courses);

			std::auto_ptr<TPaymentTransaction>TempTransaction(new TPaymentTransaction(DBTransaction));
			std::auto_ptr<TReqPrintJob>NormalRequest(new TReqPrintJob(&TDeviceRealTerminal::Instance()));
			NormalRequest->Transaction = TempTransaction.get();
			NormalRequest->SenderType = devPalm;
			NormalRequest->TimeStamp = Now();
			NormalRequest->Waiter = TDeviceRealTerminal::Instance().User.Name;
			// NormalRequest->Transaction->Membership.Assign(Membership);
			NormalRequest->MiscData["PartyName"] = CallAway->PartyName;

			NormalRequest->Transaction->Money.Recalc(*NormalRequest->Transaction);

            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
			Kitchen->Initialise(DBTransaction);
			Kitchen->GetPrintouts(DBTransaction, CallAway.get(), NormalRequest.get());

			callAwayToChefMate(DBTransaction,CallAway.get());
			NormalRequest->Printouts->Print(devPC);
		}
	}
	catch(EAbort & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}

}

void __fastcall TWebProcUtil::ProcessWebOrder(TForm *inDisplayOwner, Database::TDBTransaction &DBTransaction, TWebOrder &WebOrder, TChitNumber &WebOrderChitNumber)
{
	try
	{
		// Load the Order.
		TPaymentTransaction PaymentTransaction(DBTransaction);
		PaymentTransaction.SalesType = eWeb;
		PaymentTransaction.TimeKey = WebOrder.TimeKey;

		std::set<__int64>ItemsTabs;
		ItemsTabs.insert(WebOrder.TabKey);
		TDBOrder::GetOrdersIncludingSidesFromTabKeys(DBTransaction, PaymentTransaction.Orders, ItemsTabs);

		TMMContactInfo Member;
		TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, WebOrder.ContactKey, Member);
		if (Member.Valid())
		{
			PaymentTransaction.Membership.Assign(Member, emsManual);
		}

		int SecRefNumber = TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction);

		for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];

			// Complete Order Security.
			Order->Security->SetSecurityRefNumber(SecRefNumber);
			TSecurityReference SecRef;
			SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
			SecRef.Event = SecurityTypes[secOrderedBy];
			SecRef.From = TDeviceRealTerminal::Instance().User.Name;
			SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
			SecRef.TimeStamp = Now();
			Order->Security->SecurityUpdate(secOrderedBy, SecRef);
			for (int j = 0; j < Order->SubOrders->Count; j++)
			{
				TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
				if (SubOrder)
				{
					SubOrder->Security->SetSecurityRefNumber(SecRefNumber);
					TSecurityReference SecRef;
					SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
					SecRef.Event = SecurityTypes[secOrderedBy];
					SecRef.From = TDeviceRealTerminal::Instance().User.Name;
					SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
					SecRef.TimeStamp = Now();
					SubOrder->Security->SecurityUpdate(secOrderedBy, SecRef);
				}
			}
		}

		PaymentTransaction.Recalc();

		ProcessChitNumbers(inDisplayOwner, PaymentTransaction);
		ProcessPatrons(PaymentTransaction, PaymentTransaction.SalesType, 1);
		ProcessSecurity(PaymentTransaction);

		// PaymentTransaction.Membership.Assign(Membership);

		// Time Tracking
		ProcessTimeTracking(PaymentTransaction);
		// Stock
		ProcessStock(PaymentTransaction);

        PaymentTransaction.ChitNumber.DeliveryTime = WebOrder.DateExpected;

        int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, WebOrder.TabKey);
        std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
        TDBWebUtil::getWebOrderExtraData(DBTransaction, WebKey, "DELIVERY", WebDeliveryDetials.get());

        std::auto_ptr<TStringList>WebDetials(new TStringList);
        TDBWebUtil::getWebOrderDetials(DBTransaction, WebKey, *WebDetials.get());

        TMMContactInfo webMember;

        if(WebOrderChitNumber.Valid())
        {
            for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
            {
                TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
                Order->ChitNumber = WebOrderChitNumber;
                if (Order->ChitNumber.ChitNumberKey != 0)
                {
                    int activeChitKey = TDBActiveChit::GetOrCreateActiveChit(DBTransaction, Order->ChitNumber);
                    Order->SetActiveChitNumberKey(activeChitKey);
                }
                TDBOrder::UpdateOrderTableForWebOrders(DBTransaction, Order);
            }

        }

        if(TGlobalSettings::Instance().CaptureCustomerName)
        {
            webMember.Clear();
            if(!WebOrderChitNumber.Valid())
            {
               WebDeliveryDetials->Clear();
            }
        }
        if(WebOrderChitNumber.Valid())
        {
           if(WebOrderChitNumber.OnlineDeliveryOrder)
           {
               webMember = TDBWebUtil::autoAddWebMembers(DBTransaction, WebDetials.get(), WebDeliveryDetials.get(), WebKey);
               if(Member.ContactKey > 0)
               {
                  webMember = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, Member, WebDeliveryDetials.get());
               }
               else
               {
                   webMember = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, webMember, WebDeliveryDetials.get());
               }
           }
           else
           {
              webMember.Clear();
              WebDeliveryDetials->Clear();
           }
           if(WebOrderChitNumber.PromptForPickUpDeliveryTime)
           {
                if((double)WebOrderChitNumber.DeliveryTime > 0)
                {
                   PaymentTransaction.ChitNumber.DeliveryTime = WebOrderChitNumber.DeliveryTime;
                }
                else
                {
                   WebOrderChitNumber.DeliveryTime = WebOrder.DateExpected;
                }
           }
           else
           {
               WebOrderChitNumber.DeliveryTime = WebOrder.DateExpected;
           }
           PaymentTransaction.ChitNumber = WebOrderChitNumber;
        }

        PaymentTransaction.Membership.Assign(webMember, emsManual);
        PaymentTransaction.WebOrderKey =  WebOrder.WebKey;

		// Print the Order to the Kitchen.
		PrintKitchenDockets(PaymentTransaction, WebOrder.WebKey,WebOrder.GUID,"");
		// Print the Receipts.
		AutoPrintReceipts(TabWeb, PaymentTransaction);

		// Change the Order Status.
		TDBWebUtil::SetWebOrderStatus(PaymentTransaction.DBTransaction, WebOrder.WebKey, ewosProcessed);

     	ProcessKitchenMod(true, PaymentTransaction);

        //MessageBox( "Kitchen Docket Prepared ", "Pos droid", MB_OK + MB_ICONWARNING);
        if(PaymentTransaction.Orders->Count > 0 && TDeviceRealTerminal::Instance().KitchenMod->Enabled)
        {
            for (int i = 0; i <PaymentTransaction.Orders->Count; i++)
            {
                TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
                if (Order->OriginalItemKey != NULL)
                {
                    Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);
                }
            }
            //completeOrderToChefMate(&PaymentTransaction);
        }


	}
	catch(EAbort &E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void __fastcall TWebProcUtil::ProcessWebOrder(Database::TDBTransaction &DBTransaction, eTransactionType TransType, int defaultCovers, TMMContactInfo &Staff, TList * OrdersList,
	TMMContactInfo &Member, int inTimeKey, int WebKey)
{
	try
	{
		// Load the Order.
		TPaymentTransaction PaymentTransaction(DBTransaction);
		PaymentTransaction.SalesType = TransType;
		PaymentTransaction.StaffMember = Staff;
		PaymentTransaction.TimeKey = inTimeKey;

		PaymentTransaction.Orders->Assign(OrdersList);
		TManagerPatron::Instance().SetDefaultPatrons(DBTransaction, PaymentTransaction.Patrons, 1);

		std::set<__int64>ItemsTabs;
		TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, ItemsTabs);

		if (Member.Valid())
		{
			PaymentTransaction.Membership.Assign(Member, emsManual);
		}

		for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];

			TSecurityReference SecRef;
			SecRef.UserKey = PaymentTransaction.StaffMember.ContactKey;
			SecRef.Event = SecurityTypes[secOrderedBy];
			SecRef.From = PaymentTransaction.StaffMember.Name;
			SecRef.To = PaymentTransaction.StaffMember.Initials;
			SecRef.TimeStamp = Now();
			Order->Security->SecurityUpdate(secOrderedBy, SecRef);
			for (int j = 0; j < Order->SubOrders->Count; j++)
			{
				TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
				if (SubOrder)
				{
					TSecurityReference SecRef;
					SecRef.UserKey = PaymentTransaction.StaffMember.ContactKey;
					SecRef.Event = SecurityTypes[secOrderedBy];
					SecRef.From = PaymentTransaction.StaffMember.Name;
					SecRef.To = PaymentTransaction.StaffMember.Initials;
					SecRef.TimeStamp = Now();
					SubOrder->Security->SecurityUpdate(secOrderedBy, SecRef);
				}
			}
		}

		PaymentTransaction.Recalc();

		// ProcessChitNumbers(inDisplayOwner,PaymentTransaction);
		ProcessPatrons(PaymentTransaction, PaymentTransaction.SalesType, defaultCovers);
		ProcessSecurity(PaymentTransaction);

		// PaymentTransaction.Membership.Assign(Membership);

		// Time Tracking
		ProcessTimeTracking(PaymentTransaction);
		// Stock
		ProcessStock(PaymentTransaction);
		// Print the Order to the Kitchen.

		PrintKitchenDockets(PaymentTransaction, WebKey,IntToStr(WebKey),"");
        MessageBox( "PrintKitchenDockets(PaymentTransaction, WebKey,IntToStr(WebKey),"");", "ProcessWebOrder", MB_OK + MB_ICONWARNING);

		// Print the Receipts.
		AutoPrintReceipts(TabWeb, PaymentTransaction);

		// Chefmate.
      	ProcessKitchenMod(true, PaymentTransaction);

		for (int o = 0; o < PaymentTransaction.Orders->Count; o++)
		{
			TItemComplete *Order = (TItemComplete*)(PaymentTransaction.Orders->Items[o]);
			delete Order;
		}

	}
	catch(EAbort & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessPrintJob(Database::TDBTransaction &DBTransaction, UnicodeString TransNo, UnicodeString DeviceName, std::vector<UnicodeString>GUIDS)
{
	try
	{
		TPaymentTransaction PaymentTransaction(DBTransaction);
		PaymentTransaction.TimeKey = 0;

		std::set<__int64>ItemsKeys;
		TDBOrder::GetOrdersFromOrderGUID(DBTransaction, ItemsKeys, GUIDS);
		if (!ItemsKeys.empty())
		{
			TDBOrder::GetOrdersIncludingSidesFromOrderKeys(DBTransaction, PaymentTransaction.Orders, ItemsKeys);

			TMMContactInfo Member;

			int SecRefNumber = TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction);

			if (PaymentTransaction.Orders->Count > 0)
			{
				TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
                PaymentTransaction.ChitNumber = Order->ChitNumber;

				switch(Order->ContainerTabType)
				{
				case TabNormal:
				case TabStaff:
				case TabMember:
					PaymentTransaction.SalesType = eTab;
					break;
				case TabTableSeat:
					PaymentTransaction.SalesType = eTableSeat;
					break;
				case TabRoom:
					PaymentTransaction.SalesType = eRoomSale;
					break;
				case TabInvoice:
					PaymentTransaction.SalesType = eAccount;
					break;
				case TabWeb:
					PaymentTransaction.SalesType = eWeb;
					break;
				case TabParkedSale:
                                case TabHandheldCashAccount:
				case TabCashAccount:
				case TabNone:
				default:
					PaymentTransaction.SalesType = eCash;

				}
			}
			PaymentTransaction.Recalc();

			// ProcessChitNumbers(inDisplayOwner, PaymentTransaction);
			ProcessPatrons(PaymentTransaction, PaymentTransaction.SalesType, 1);

			// Print the Order to the Kitchen.
          	PrintKitchenDockets(PaymentTransaction, 0,TransNo,DeviceName);
			// Print the Receipts.
			AutoPrintReceipts(PaymentTransaction.SalesType, PaymentTransaction);
            //MessageBox( "PrintKitchenDockets(PaymentTransaction, 0,TransNo,DeviceName);", "ProcessPrintJob", MB_OK + MB_ICONWARNING);

		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Recived a Print Job I had no orders for? Is PosDroid Looking at the same DB as me?");
		}
	}
	catch(EAbort & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessTimeTracking(TPaymentTransaction &PaymentTransaction)
{
	TDBSaleTimes::CloseSaleStartTime(PaymentTransaction.DBTransaction, PaymentTransaction.TimeKey); // Close the Sale Key for Chefmate.
}

// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessStock(TPaymentTransaction &PaymentTransaction)
{
  try
  {
 	TDeviceRealTerminal::Instance().ManagerStock->UpdateStock(PaymentTransaction.DBTransaction, PaymentTransaction.Orders);
  }
  catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessSecurity(TPaymentTransaction &PaymentTransaction)
{
	for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
	{
		TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
		TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, Order->Security);
		for (int j = 0; j < Order->SubOrders->Count; j++)
		{
			TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
			if (SubOrder)
			{
				TDBSecurity::ProcessSecurity(PaymentTransaction.DBTransaction, SubOrder->Security);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessChitNumbers(TForm *inDisplayOwner, TPaymentTransaction &PaymentTransaction)
{
	TChitNumber ChitNumber;
	TChitNumberController ChitNumberController(inDisplayOwner, PaymentTransaction.DBTransaction);
	ChitResult Result = ChitNumberController.GetChitNumber(false, ChitNumber);
	switch(Result)
	{
	case ChitDisabled:
		PaymentTransaction.ChitNumber.Clear();
		break;
	case ChitOk:
		PaymentTransaction.ChitNumber = ChitNumber;
		break;
	case ChitCancelled:
		throw EAbort("Cancelled by User.");
		break;
	case ChitNone:
		PaymentTransaction.ChitNumber.Clear();
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessPatrons(TPaymentTransaction &PaymentTransaction, eTransactionType TransType, int DefaultCovers)
{
	switch(TransType)
	{
	case eTableSeat:
		{
			if (PaymentTransaction.Orders->Count > 0)
			{
				TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];

                //MM-1649: Now table patrons will be stored on a separate table.
                std::vector<TPatronType> tablePatronsVector = TDBTables::GetPatronCount(PaymentTransaction.DBTransaction, Order->TableNo);

                int PatronCount = 0;
                //set default covers from DB
                DefaultCovers = TDBTables::GetPatronNumbersForWebOrders(PaymentTransaction.DBTransaction, Order->TableNo);

                for(std::vector<TPatronType>::iterator it = tablePatronsVector.begin(); it != tablePatronsVector.end(); ++it)
                {
                    PatronCount += it->Count;
                }

                if (PatronCount > 0)
				{
					TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, PatronCount);
				}
				else
				{
					TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, DefaultCovers);
				}
			}
			else
			{
				TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, DefaultCovers);
			}
		}break;
	default:
		TManagerPatron::Instance().SetDefaultPatrons(PaymentTransaction.DBTransaction, PaymentTransaction.Patrons, DefaultCovers);
		break;
	}
}

void __fastcall TWebProcUtil::PrintKitchenDockets(TPaymentTransaction &PaymentTransaction, int WebKey, UnicodeString JobName,UnicodeString DeviceName)
{
	// Print the Orders In the Kitchen if any.
	std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());

    if(DeviceName == "")
    {
    	DeviceName = "WebMate";
    }

	if(WebDevice->NameToKey(PaymentTransaction.DBTransaction, DeviceName) != 0)
    {
        WebDevice->Load(PaymentTransaction.DBTransaction);

        std::auto_ptr<TReqPrintJob>Request(new TReqPrintJob(WebDevice.get()));
        std::auto_ptr<TPaymentTransaction>PrintTransaction(new TPaymentTransaction(PaymentTransaction.DBTransaction));

        // Copy over the Patron Counts for Printing.
        PrintTransaction->Patrons = PaymentTransaction.Patrons;

        if (!PaymentTransaction.CreditTransaction)
        {
            Request->BarCodeData = PaymentTransaction.TimeKey;
            Request->Transaction = PrintTransaction.get();
            Request->JobType = pjKitchen;
            TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
            Request->Waiter = Order->Security->SecurityGetField(secOrderedBy, secfFrom);

            if (Order->TabType == TabTableSeat)
            {
               if(TDBTables::Valid(Order->TableNo,Order->SeatNo))
               {
					Order->TabContainerName = TDBTables::GetTableName(PaymentTransaction.DBTransaction,Order->TableNo);
					Order->PartyName = TDBTables::GetPartyName(PaymentTransaction.DBTransaction,Order->TableNo);
               }
            }
            else if (Order->TabType == TabRoom)
            {
					Order->PartyName 	= TDBRooms::GetPartyName(PaymentTransaction.DBTransaction,Order->RoomNo);
					Order->TabContainerName = TDBRooms::GetRoomName(PaymentTransaction.DBTransaction,Order->RoomNo);
	           		Order->TabName = Order->PartyName;
            }
            else if (Order->TabType == TabWeb)
            {
					Order->TabContainerName = TDBWebUtil::GetOrderGUID(PaymentTransaction.DBTransaction,Order->WebKey);
            }

            Request->Transaction->Membership.Assign(PaymentTransaction.Membership);
            PrintTransaction->ChitNumber = PaymentTransaction.ChitNumber;
            Request->MiscData["PartyName"] = Order->PartyName;

            //MessageBox( IntToStr(PaymentTransaction.Orders->Count), "Pos droid", MB_OK + MB_ICONWARNING);
            for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
            {
                TSecurityReference *OldSecRef = Order->Security->SecurityGetType(secCredit);
                if (OldSecRef == NULL)
                {
                    PrintTransaction->Orders->Add(PaymentTransaction.Orders->Items[i]);
                }
            }

            if (WebKey != 0)
            {
                std::auto_ptr<TStringList>WebDetials(new TStringList);
                TDBWebUtil::getWebOrderDetials(PaymentTransaction.DBTransaction, WebKey, *WebDetials.get());
                Request->ExtraInfo->AddStrings(WebDetials.get());

                std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                std::auto_ptr<TStringList>WebComments(new TStringList);
                std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
                TDBWebUtil::getWebOrderData(PaymentTransaction.DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

                Request->PaymentInfo->AddStrings(WebPaymentDetials.get());
                Request->OrderComments->AddStrings(WebComments.get());
                UnicodeString _orderType = checkWebOrderType(PaymentTransaction.DBTransaction, WebKey);
                if(_orderType == "Pickup")
                {
                    WebDeliveryDetials->Clear(); //remove delivery info for pickup order in kitchen docket..
                }
                Request->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
            }
            PrintTransaction->WebOrderKey =  WebKey;

            Request->Transaction->Money.Recalc(*Request->Transaction);
            //MessageBox( IntToStr(PrintTransaction->Orders->Count), "Pos droid", MB_OK + MB_ICONWARNING);
            if (PrintTransaction->Orders->Count > 0)
            {
	            std::auto_ptr<TKitchen> Kitchen(new TKitchen());
				Kitchen->Initialise(PaymentTransaction.DBTransaction);
                Kitchen->GetPrintouts(PrintTransaction->DBTransaction, Request.get());
                if (!Request->Printouts->Print(devPalm,JobName))
                {
                    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing Some Web Orders Failed, Please Check Printer.");
                    throw Exception("Printing Some Orders Failed, Please Check Printer.");
                }
                ManagerDockets->Archive(Request.get());
                completeOrderToChefMate(PrintTransaction.get());
            }
        }
    }
    else
    {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Docket not Printed Device" +DeviceName+" Not Found!");
        throw Exception("Docket not Printed Device" +DeviceName+" Not Found!");
    }
}
// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::AutoPrintReceipts(TMMTabType TabType, TPaymentTransaction &PaymentTransaction)
{
	if (checkAutoPrintReceipts(TabType))
	{
		bool OrdersLoadedFromTabs = false;
		TMMContactInfo InvoiceOwnerInfo;
		TPaymentTransaction InvoiceTransaction(PaymentTransaction.DBTransaction);

		std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());
		WebDevice->NameToKey(PaymentTransaction.DBTransaction, "WebMate");
		WebDevice->Load(PaymentTransaction.DBTransaction);

		std::auto_ptr<TReqPrintJob>TempReceipt(new TReqPrintJob(WebDevice.get()));

		std::set<__int64>SelectedTabs;
		if (TGlobalSettings::Instance().SaveAndPrintPrintsPartialOrder && TabType != TabInvoice || TabType == TabHandheldCashAccount || TabType == TabCashAccount)
		{
			InvoiceTransaction.Orders->Assign(PaymentTransaction.Orders);
			TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, SelectedTabs);
			InvoiceTransaction.Money.CreditAvailable = TDBTab::GetTabsCredit(InvoiceTransaction.DBTransaction, SelectedTabs);
			InvoiceTransaction.Money.Recalc(InvoiceTransaction);
		}
		else
		{
			TDBOrder::GetTabKeysFromOrders(PaymentTransaction.Orders, SelectedTabs);
			TDBOrder::GetOrdersFromTabKeys(InvoiceTransaction.DBTransaction, InvoiceTransaction.Orders, SelectedTabs);
			InvoiceTransaction.Money.CreditAvailable = TDBTab::GetTabsCredit(InvoiceTransaction.DBTransaction, SelectedTabs);
			InvoiceTransaction.Money.Recalc(InvoiceTransaction);
			OrdersLoadedFromTabs = true;
		}

		if (InvoiceTransaction.Money.TotalAdjustment != 0)
		{
			InvoiceTransaction.TotalAdjustment = InvoiceTransaction.Money.TotalAdjustment;
			InvoiceTransaction.DiscountReason = InvoiceTransaction.TotalAdjustment < 0 ? "Discount " : "Surcharge";
		}

		InvoiceTransaction.TabCredit.clear();
		for (std::set<__int64>::iterator pTabKey = SelectedTabs.begin(); pTabKey != SelectedTabs.end(); advance(pTabKey, 1))
		{
			int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(InvoiceTransaction.DBTransaction, *pTabKey);
			if (WebKey != 0)
			{
				std::auto_ptr<TStringList>WebDetials(new TStringList);
				TDBWebUtil::getWebOrderDetials(InvoiceTransaction.DBTransaction, WebKey, *WebDetials.get());
				TempReceipt->ExtraInfo->AddStrings(WebDetials.get());

                std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                std::auto_ptr<TStringList>WebComments(new TStringList);
                std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
                TDBWebUtil::getWebOrderData(PaymentTransaction.DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

                TempReceipt->PaymentInfo->AddStrings(WebPaymentDetials.get());
                TempReceipt->OrderComments->AddStrings(WebComments.get());
                UnicodeString _orderType = checkWebOrderType(PaymentTransaction.DBTransaction, WebKey);
                if(_orderType == "Pickup")
                {
                    WebDeliveryDetials->Clear(); //remove delivery info for pickup order in kitchen docket..
                }
                TempReceipt->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
			}

			Currency TabCurrentCredit = TDBTab::GetTabCredit(InvoiceTransaction.DBTransaction, *pTabKey);
			if (TabCurrentCredit != 0)
			{
				InvoiceTransaction.Money.CreditAvailable += TabCurrentCredit;
				TTabCredit Credit = InvoiceTransaction.TabCredit[*pTabKey];
				Credit.CurrentCredit = TabCurrentCredit;
				InvoiceTransaction.TabCredit[*pTabKey] = Credit;
			}
		}

		InvoiceTransaction.Membership.Assign(PaymentTransaction.Membership);

		TempReceipt->JobType = pjReceiptReceipt;
		TempReceipt->PaymentType = ptPreliminary;
		TempReceipt->MiscData["PartyName"] = "";

		if (TabType == TabInvoice)
		{
			TempReceipt->JobType = pjReceiptInvoice;
			TempReceipt->PaymentType = ptFinal;
			std::set<__int64>::iterator pTabKey = SelectedTabs.begin();
			int InvoiceKey = TDBTab::GetTabInvoice(InvoiceTransaction.DBTransaction, *pTabKey);
			InvoiceTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(InvoiceTransaction.DBTransaction, InvoiceKey);
			int ContactKey = Invoice->GetInvoiceOwner(InvoiceTransaction.DBTransaction, InvoiceKey);

			TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(InvoiceTransaction.DBTransaction, ContactKey, InvoiceOwnerInfo);

			TempReceipt->ExtraInfo->Add("Name: " + InvoiceOwnerInfo.Name);
			TempReceipt->ExtraInfo->Add("Member No. " + InvoiceOwnerInfo.MembershipNumber);
		}
		else if (TabType == TabRoom)
		{
			std::set<__int64>::iterator pTabKey = SelectedTabs.begin();
			int RoomNo = TDBRooms::GetRoomNumber(InvoiceTransaction.DBTransaction, *pTabKey);
			TempReceipt->ExtraInfo->Add("Room Number # " + IntToStr(RoomNo));
			TempReceipt->ExtraInfo->Add("Guest " + TDBRooms::GetPartyName(InvoiceTransaction.DBTransaction, RoomNo));
		}
		else if (TabType == TabTableSeat)
		{
			std::set<__int64>::iterator pTabKey = SelectedTabs.begin();
			int SeatKey = TDBTables::GetSeatKey(InvoiceTransaction.DBTransaction, *pTabKey);
			int TableKey = TDBTables::GetTableKey(InvoiceTransaction.DBTransaction, SeatKey);
			int TableNo = TDBTables::GetTableNo(InvoiceTransaction.DBTransaction, TableKey);
			TempReceipt->MiscData["PartyName"] = TDBTables::GetPartyName(InvoiceTransaction.DBTransaction, TableNo);
		}

		// Print Invoice.
		TempReceipt->Transaction = &InvoiceTransaction;
		TempReceipt->SignReceipt = true;
		TempReceipt->SenderType = devPC;
		TempReceipt->Waiter = TDeviceRealTerminal::Instance().User.Name;

		Receipt->GetPrintouts(InvoiceTransaction.DBTransaction, TempReceipt.get(), TComms::Instance().ReceiptPrinter);
		TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
		if (TGlobalSettings::Instance().PrintSignatureReceiptsTwice)
		{
			TempReceipt->Printouts->Print(TDeviceRealTerminal::Instance().ID.Type);
		}

		if (OrdersLoadedFromTabs)
		{
			while (InvoiceTransaction.Orders->Count != 0)
			{
				delete(TItemComplete*)InvoiceTransaction.Orders->Items[0];
				InvoiceTransaction.Orders->Delete(0);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TWebProcUtil::ProcessKitchenMod(bool Finial, TPaymentTransaction &PaymentTransaction)
{
	try
	{
		if (TDeviceRealTerminal::Instance().KitchenMod->Enabled)
		{
			Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
			DBTransaction.StartTransaction();
			std::auto_ptr<TPaymentTransaction>Transaction(new TPaymentTransaction(DBTransaction));
			Transaction->Orders->Assign(PaymentTransaction.Orders);

			if (Transaction->Orders->Count > 0)
			{
				std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());
				WebDevice->NameToKey(PaymentTransaction.DBTransaction, "WebMate");
				WebDevice->Load(PaymentTransaction.DBTransaction);

				std::auto_ptr<TReqPrintJob>Request(new TReqPrintJob(WebDevice.get()));
				Request->Transaction = Transaction.get();
				TItemComplete *Order = (TItemComplete*)Transaction->Orders->Items[0];
				Request->Waiter = PaymentTransaction.StaffMember.Name;
				// Request->Transaction->Membership.Assign(Membership);
				Request->Transaction->ChitNumber = PaymentTransaction.ChitNumber;
				Request->MiscData["PartyName"] = Order->PartyName;
				Request->Transaction->Money.Recalc(*Request->Transaction);

				if (Order->TabKey != 0)
				{
					std::auto_ptr<TStringList>WebDetials(new TStringList);
                    int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, Order->TabKey);
                    PaymentTransaction.WebOrderKey =  WebKey;
					TDBWebUtil::getWebOrderDetials(DBTransaction,WebKey , *WebDetials.get());
					Request->ExtraInfo->AddStrings(WebDetials.get());

                    std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                    std::auto_ptr<TStringList>WebComments(new TStringList);
                    std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
                    TDBWebUtil::getWebOrderData(DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

                    Request->PaymentInfo->AddStrings(WebPaymentDetials.get());
                    Request->OrderComments->AddStrings(WebComments.get());
                    UnicodeString _orderType = checkWebOrderType(PaymentTransaction.DBTransaction, WebKey);
                    if(_orderType == "Pickup")
                    {
                        WebDeliveryDetials->Clear(); //remove delivery info for pickup order in kitchen docket..
                    }
                    Request->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
				}

				TSectionInstructStorage Template;
				TPSectionInstruction CMInstruction = TPSectionInstruction(epofiFormatChefMate);
				CMInstruction.LineAbove = true;
				CMInstruction.LineBelow = false;
				CMInstruction.Visible = true;
				Template.push_back(CMInstruction);
                std::auto_ptr<TKitchen> Kitchen(new TKitchen());
				Kitchen->Initialise(DBTransaction);
				Kitchen->GetPrintouts(DBTransaction, Request.get() );
				std::auto_ptr<TNetMessageChefMate>ChefRequest(new TNetMessageChefMate());
				ChefRequest->OrderNumber = TGlobalSettings::Instance().KitchenOrderNumber;
				if (Finial)
				{
					TGlobalSettings::Instance().KitchenOrderNumber++;
					ChefRequest->FinalOrder = true;
				}
				ChefRequest->RemoveOrder = false;
				ChefRequest->TillColour = TGlobalSettings::Instance().KitchenColour;
				ChefRequest->ChitNumber = PaymentTransaction.ChitNumber.ChitNumber;
				ChefRequest->Device = TDeviceRealTerminal::Instance().ID.Name;
				ChefRequest->User = PaymentTransaction.StaffMember.Name;
				ChefRequest->TimeKey = PaymentTransaction.TimeKey;
				Request->Printouts->FilterForChefMate();
			}
			else
			{
				std::auto_ptr<TStringList>ChefMateIPList(new TStringList);
				TManagerPhysicalPrinter ManagerPhysicalPrinter;
				ManagerPhysicalPrinter.GetPrinterServerList(DBTransaction, ChefMateIPList.get(), ptChefMate_Printer);
				std::auto_ptr<TNetMessageChefMate>ChefRequest(new TNetMessageChefMate);
				ChefRequest->OrderNumber = TGlobalSettings::Instance().KitchenOrderNumber;
				ChefRequest->RemoveOrder = false;
				if (Finial)
				{
					TGlobalSettings::Instance().KitchenOrderNumber++;
					ChefRequest->FinalOrder = true;
				}
				ChefRequest->TillColour = TGlobalSettings::Instance().KitchenColour;
				ChefRequest->ChitNumber = PaymentTransaction.ChitNumber.ChitNumber;
				ChefRequest->Device = TDeviceRealTerminal::Instance().ID.Name;
				ChefRequest->User = PaymentTransaction.StaffMember.Name;
				ChefRequest->TimeKey = PaymentTransaction.TimeKey;

				for (int i = 0; i < ChefMateIPList->Count; i++)
				{
					AnsiString ChefMateIP = ChefMateIPList->Strings[i];
					TDeviceRealTerminal::Instance().KitchenMod->SendRequest(ChefRequest.get(), ChefMateIP);
				}
			}
			DBTransaction.Commit();
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

//---------------------------------------------------------------------------
void __fastcall TWebProcUtil::completeOrderToChefMate(TPaymentTransaction* inTransaction)
{
    if(inTransaction->WebOrderKey > 0)
    {
        sendWebOrderToChefmate(inTransaction);
    }
    else
    {
        sendPosDroidOrderToChefmate(inTransaction);
    }   ///todo
}

//---------------------------------------------------------------------------
void __fastcall TWebProcUtil::callAwayToChefMate(Database::TDBTransaction &DBTransaction, TCallAwayComplete* inCallAway)
{
    std::auto_ptr<TChefmateClientManager> ChefMateClientManager(new TChefmateClientManager());

    if( ChefMateClientManager->ChefMateEnabled() )
	{
        CMC_ERROR error = ChefMateClientManager->SendCallAwayOrder(inCallAway);

        if(error == CMC_ERROR_FAILED)
        {
            MessageBox( "Menumate failed to send an call away order to Chefmate",
                        "Chefmate",
                        MB_OK + MB_ICONWARNING);
        }
    }
    else
    {
    	TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Menumate WebMate failed to Open Chefmate Interface for Call Aways");
    }


}
//---------------------------------------------------------------------------
bool __fastcall TWebProcUtil::checkAutoPrintReceipts(TMMTabType TabType)
{
    bool autoPrintReceipt = false;
    switch(TabType)
    {
        case TabNormal:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintTabReceipts;
            break;
        case TabStaff:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintStaffReceipts;
            break;
        case TabMember:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintMemberReceipts;
            break;
        case TabTableSeat:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintTableReceipts;
            break;
        case TabCashAccount:
        case TabHandheldCashAccount:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintCashReceipts;
            break;
        case TabRoom:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintRoomReceipts;
            break;
        case TabInvoice:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintInvoiceReceipts;
            break;
        case TabWeb:
            autoPrintReceipt = TGlobalSettings::Instance().AutoPrintWebReceipts;
            break;
        default:
            autoPrintReceipt = false;
            break;
    }
        return autoPrintReceipt;
}

UnicodeString TWebProcUtil::checkWebOrderType(Database::TDBTransaction &DBTransaction, int key)
{
    UnicodeString orderType ="";
    std::auto_ptr<TStringList>webOrderType(new TStringList);
    TDBWebUtil::getWebOrderExtraData(DBTransaction, key, "ORDERTYPE", webOrderType.get());
    if(webOrderType->Count > 0)
    {
        orderType = webOrderType->Strings[0];
    }
    return orderType;
}

void TWebProcUtil::sendWebOrderToChefmate(TPaymentTransaction* inTransaction)
{
    try
    {
        std::auto_ptr<TChefmateClientManager> ChefMateClientManager( new TChefmateClientManager() );
        if( ChefMateClientManager->ChefMateEnabled() )
        {
            Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
            DBTransaction.StartTransaction();
            TMMContactInfo memberInfo = TDBWebUtil::LoadMemberDetails(DBTransaction, inTransaction->WebOrderKey);
            UnicodeString paymentStatus = TDBWebUtil::LoadPaymentStatus(DBTransaction, inTransaction->WebOrderKey);
            //MessageBox( "Send Posdroid Orders ", "Pos droid", MB_OK + MB_ICONWARNING);
            //MessageBox( IntToStr(inTransaction->WebOrderKey), "paymentStatus", MB_OK + MB_ICONWARNING);
            CMC_ERROR error =  ChefMateClientManager->SendWebOrder(inTransaction, paymentStatus, memberInfo );
            if( error == CMC_ERROR_FAILED )
            {
                TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Menumate WebMate failed to send an complete order to Chefmate");
            }
            DBTransaction.Commit();
        }
    }
    catch(Exception & E)
    {
       TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Menumate WebMate failed to Open Chefmate Interface");
    }
}

void TWebProcUtil::sendPosDroidOrderToChefmate(TPaymentTransaction* inTransaction)
{
   try
    {
        std::auto_ptr<TChefmateClientManager> ChefMateClientManager( new TChefmateClientManager() );
        if( ChefMateClientManager->ChefMateEnabled() )
        {
            CMC_ERROR error =  ChefMateClientManager->SendCompleteOrder(inTransaction);
            if( error == CMC_ERROR_FAILED )
            {
                TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Pos Droid failed to send an complete order to Chefmate");
            }
        }
    }
    catch(Exception & E)
    {
       TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Pos Droid failed to Open Chefmate Interface");
    }
}

