//---------------------------------------------------------------------------

#pragma hdrstop

#include "OnlineOrderDocketPrinter.h"
#include "DBOnlineOrdeing.h"
#include "DBSecurity.h"
#include "WebProcUtil.h"
#include "DeviceWeb.h"
#include "DBTables.h"
#include "DBRooms.h"
#include "DBWebUtil.h"
#include "DBTab.h"
#include "DocketManager.h"
#include "ChefmateClientManager.h"
#include "ReceiptManager.h"
#include "ManagerHappyHour.h"
#include "Comms.h"
#include "ItemSize.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TOnlineOrderDocketPrinter* TOnlineOrderDocketPrinter::instance = NULL;

TOnlineOrderDocketPrinter::TOnlineOrderDocketPrinter()
    :docketPrinterThreadTerminated(true),
     docketPrinterThread(NULL)
{
    docketPrinterThreadTimer = new TTimer(NULL);
    docketPrinterThreadTimer->Enabled = false;
    docketPrinterThreadTimer->Interval = 1000;
    docketPrinterThreadTimer->OnTimer = OnDocketPrinterThreadTimerTick;

    StartDocketPrinterThreadTimer();
}
//-------------------------------------------------------------------------------------------
TOnlineOrderDocketPrinter::~TOnlineOrderDocketPrinter()
{
    delete docketPrinterThread;
    delete docketPrinterThreadTimer;
}
//---------------------------------------------------------------------------
TOnlineDocketPrinterThread::TOnlineDocketPrinterThread() : TThread(true)
{
    FreeOnTerminate = true;
}
//-------------------------------------------------------------------------------
void __fastcall TOnlineOrderDocketPrinter::DocketPrinterTheadTerminate( TObject* sender )
{
    docketPrinterThreadTerminated = true;
    StartDocketPrinterThreadTimer();
}
//---------------------------------------------------------------------------
void TOnlineOrderDocketPrinter::InitiateDocketPrinterThread()
{
    if( docketPrinterThreadTerminated )
    {
        docketPrinterThread = new TOnlineDocketPrinterThread();
        docketPrinterThread->OnTerminate = DocketPrinterTheadTerminate;
    }
}
//-----------------------------------------------------------
void __fastcall TOnlineOrderDocketPrinter::OnDocketPrinterThreadTimerTick(TObject *Sender)
{
    if(docketPrinterThreadTerminated && IsDocketPrintingPending())
    {
        InitiateDocketPrinterThread();
        StartDocketPrinterThread();
    }
    else
        StartDocketPrinterThreadTimer();
}
//------------------------------------------------------------------------------------------------------
void TOnlineOrderDocketPrinter::StartDocketPrinterThread()
{
    if( docketPrinterThreadTerminated )
    {
        docketPrinterThreadTerminated = false;
        StartDocketPrinterThreadTimer();
        docketPrinterThread->Start();
    }
}
//---------------------------------------------------------------------------
void TOnlineOrderDocketPrinter::StartDocketPrinterThreadTimer()
{
    docketPrinterThreadTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void TOnlineOrderDocketPrinter::StopDocketPrinterThreadTimer()
{
    docketPrinterThreadTimer->Enabled = false;
}
//-------------------------------------------------------------------------------------
void  __fastcall TOnlineDocketPrinterThread::Execute()
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        PrepareDataAndPrintDocket(dbTransaction);
        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
bool TOnlineOrderDocketPrinter::IsDocketPrintingPending()
{
    bool isRecordExist = false;

    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();

    try
    {
        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->SQL->Text = "SELECT a.ORDER_KEY "
                                      "FROM ORDERS a "
                                      "WHERE a.IS_DOCKET_PRINTED = :IS_DOCKET_PRINTED ";
        IBInternalQuery->ParamByName("IS_DOCKET_PRINTED")->AsString = "F";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            isRecordExist = true;
        else
            isRecordExist = IsPrePaidOrderPrintingPending(dbTransaction);

        dbTransaction.Commit();
    }
    catch(Exception &E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return isRecordExist;
}
//---------------------------------------------------------------------------------------------------------------------------
void TOnlineOrderDocketPrinter::TriggerTransactionSync()
{
    InitiateDocketPrinterThread();
    StartDocketPrinterThread();
}
//---------------------------------------------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::PrepareDataAndPrintDocket(Database::TDBTransaction &dBTransaction)
{
	try
	{
        //First Checking In DayArcBill Table
        UnicodeString orderUniqueIdForWAiterApp = TDBOnlineOrdering::GetOnlineOrderGUIDForWaiterApp(dBTransaction, true);

        if(orderUniqueIdForWAiterApp.Trim() != "")
        {
            ManagePrintOperationsForWaiterApp(dBTransaction, orderUniqueIdForWAiterApp, true);
        }
        else
        {
            orderUniqueIdForWAiterApp = TDBOnlineOrdering::GetOnlineOrderGUIDForWaiterApp(dBTransaction, false);
            if(orderUniqueIdForWAiterApp.Trim() != "")
            {
                ManagePrintOperationsForWaiterApp(dBTransaction, orderUniqueIdForWAiterApp, false);
            }
        }


        UnicodeString orderUniqueId = TDBOnlineOrdering::GetOnlineOrderGUID(dBTransaction);
        if(orderUniqueId.Trim() == "")
            return;

		// Load the Order.
		TPaymentTransaction PaymentTransaction(dBTransaction);
		PaymentTransaction.SalesType = eWeb;

		TDBOnlineOrdering::GetOrdersByOnlineOrderGUID(PaymentTransaction.DBTransaction, PaymentTransaction.Orders, orderUniqueId);

         //Process happy hour for every order if it fall in HH time zone.
        if(TGlobalSettings::Instance().ForceHappyHour || !TGlobalSettings::Instance().TerminalExemptFromHappyHour)
        {
            ProcessHappyHour(PaymentTransaction, orderUniqueId);
        }

		TMMContactInfo Member;
	  //	TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, WebOrder.ContactKey, Member);
		if (Member.Valid())
		{
			PaymentTransaction.Membership.Assign(Member, emsManual);
		}

		int SecRefNumber = TDBSecurity::GetNextSecurityRef(PaymentTransaction.DBTransaction);

        int contactKey = 0;
        UnicodeString contactName = "";
        UnicodeString contactInitials = "";
        bool IsPayLaterOrder = false;

		for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];

			// Complete Order Security.
			Order->Security->SetSecurityRefNumber(SecRefNumber);
			TSecurityReference SecRef;
            if(Order->Email.Trim() == "")
            {
                TDBOnlineOrdering::LoadWaiterContactDetails(PaymentTransaction.DBTransaction, contactKey, contactName, contactInitials);
                SecRef.UserKey = contactKey;
                SecRef.From = contactName;
			    SecRef.To = contactInitials;
                IsPayLaterOrder = true;
            }
            else
            {
			    SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
                SecRef.From = TDeviceRealTerminal::Instance().User.Name;
			    SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
            }

			SecRef.Event = SecurityTypes[secOrderedBy];
			SecRef.TimeStamp = Now();
			Order->Security->SecurityUpdate(secOrderedBy, SecRef);
			for (int j = 0; j < Order->SubOrders->Count; j++)
			{
				TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
				if (SubOrder)
				{
					SubOrder->Security->SetSecurityRefNumber(SecRefNumber);
					TSecurityReference SecRef;
                    if(SubOrder->Email.Trim() == "")
                    {
                        SecRef.UserKey = contactKey;
                        SecRef.From = contactName;
			            SecRef.To = contactInitials;
                    }
                    else
                    {
                        SecRef.UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
                        SecRef.From = TDeviceRealTerminal::Instance().User.Name;
					    SecRef.To = TDeviceRealTerminal::Instance().User.Initials;
                    }

					SecRef.Event = SecurityTypes[secOrderedBy];
					SecRef.TimeStamp = Now();
					SubOrder->Security->SecurityUpdate(secOrderedBy, SecRef);
				}
			}
		}
        PaymentTransaction.IgnoreLoyaltyKey = false;
		PaymentTransaction.Recalc();

		ProcessSecurity(PaymentTransaction);

		// Print the Order to the Kitchen.
        if(IsPayLaterOrder)
        {
            UnicodeString deviceName = "";
            deviceName = TDBOnlineOrdering::GetTerminalNameForOrderGUID(PaymentTransaction.DBTransaction, orderUniqueId, IsPayLaterOrder);
		    PrintKitchenDockets(PaymentTransaction, 15,orderUniqueId, deviceName);
        }
        else
        {
            PrintKitchenDockets(PaymentTransaction, 15,orderUniqueId,"OnlineOrder");
        }

        // Change the Order Status.
        TDBOnlineOrdering::SetOnlineOrderStatus(PaymentTransaction.DBTransaction, orderUniqueId);
     	TWebProcUtil::ProcessKitchenMod(true, PaymentTransaction);

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
        }
//        dBTransaction.Commit();
	}
	catch(EAbort &E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
//----------------------------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::ProcessSecurity(TPaymentTransaction &PaymentTransaction)
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
//---------------------------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::PrintKitchenDockets(TPaymentTransaction &PaymentTransaction, int WebKey, UnicodeString JobName,UnicodeString DeviceName)
{
    try
    {
        // Print the Orders In the Kitchen if any.
        std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());

        if(DeviceName == "")
        {
            DeviceName = "OnlineOrder";
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
                std::auto_ptr<TStringList>WebDetials(new TStringList);
                if(PaymentTransaction.Orders->Count)
                {
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
                    for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
                    {
                        TSecurityReference *OldSecRef = Order->Security->SecurityGetType(secCredit);
                        if (OldSecRef == NULL)
                        {
                            PrintTransaction->Orders->Add(PaymentTransaction.Orders->Items[i]);
                        }
                    }
                    UnicodeString OrderType = "Order Type : ";
                    OrderType = OrderType + (Order->OnlineChitType > 0 ? (Order->OnlineChitType == 1 ? "DineIn" : "TakeAway")  : "PickUp");
                    WebDetials->Add(OrderType);
                    OrderType = "Order Id : ";
                    OrderType =  OrderType + (Order->OnlineOrderId);
                    WebDetials->Add(OrderType);
                }

                if (WebKey != 0)
                {
                    TDBWebUtil::getWebOrderDetials(PaymentTransaction.DBTransaction, WebKey, *WebDetials.get());
                    Request->ExtraInfo->AddStrings(WebDetials.get());

                    std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
                    std::auto_ptr<TStringList>WebComments(new TStringList);
                    std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
                    TDBWebUtil::getWebOrderData(PaymentTransaction.DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);

                    Request->PaymentInfo->AddStrings(WebPaymentDetials.get());
                    Request->OrderComments->AddStrings(WebComments.get());
//                    UnicodeString _orderType = checkWebOrderType(PaymentTransaction.DBTransaction, WebKey);
//                    if(_orderType == "Pickup")
//                    {
//                        WebDeliveryDetials->Clear(); //remove delivery info for pickup order in kitchen docket..
//                    }
                    Request->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
                }
                PrintTransaction->WebOrderKey =  WebKey;
                Request->Transaction->Money.Recalc(*Request->Transaction);
                if (PrintTransaction->Orders->Count > 0)
                {
                    std::auto_ptr<TKitchen> Kitchen(new TKitchen());
                    Kitchen->Initialise(PaymentTransaction.DBTransaction);
                    Kitchen->GetPrintouts(PrintTransaction->DBTransaction, Request.get());
                    if (!Request->Printouts->Print(devPalm,JobName))
                    {
                        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing Some Online Orders Failed, Please Check Printer.");
                        //throw Exception("Printing Some Orders Failed, Please Check Printer.");
                    }
                    ManagerDockets->Archive(Request.get());
                    SendOnlineOrderToChefmate(PrintTransaction.get());
                }
            }
        }
        else
        {
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Docket not Printed Device" +DeviceName+" Not Found!");
            throw Exception("Docket not Printed Device" +DeviceName+" Not Found!");
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
// ---------------------------------------------------------------------------
void TOnlineDocketPrinterThread::SendOnlineOrderToChefmate(TPaymentTransaction* inTransaction)
{
    try
    {
        if(inTransaction->WebOrderKey > 0)
        {
            std::auto_ptr<TChefmateClientManager> ChefMateClientManager( new TChefmateClientManager() );
            if( ChefMateClientManager->ChefMateEnabled() )
            {
                TMMContactInfo memberInfo;// = TDBWebUtil::LoadMemberDetails(inTransaction->DBTransaction, inTransaction->WebOrderKey);
                UnicodeString paymentStatus;
                if(inTransaction->Orders->Count)
                {
                        TItemComplete *Order = (TItemComplete*)inTransaction->Orders->Items[0];

                        memberInfo.Name = Order->Email;
                        memberInfo.EMail = Order->Email;

                        if(Order->OnlineChitType != 1)
                        {
                            //  TDBWebUtil::LoadMemberDetails(inTransaction->DBTransaction, inTransaction->WebOrderKey);
                         //   paymentStatus = TDBWebUtil::LoadPaymentStatus(inTransaction->DBTransaction, inTransaction->WebOrderKey);
                        }
                }
//                UnicodeString paymentStatus  = TDBWebUtil::LoadPaymentStatus(inTransaction->DBTransaction, inTransaction->WebOrderKey);
                CMC_ERROR error =  ChefMateClientManager->SendWebOrder(inTransaction, paymentStatus, memberInfo );
                if( error == CMC_ERROR_FAILED )
                {
                    TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Menumate Online Ordering failed to send an complete order to Chefmate");
                }
            }
        }
    }
    catch(Exception & E)
    {
       TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Menumate Online Ordering failed to Open Chefmate Interface");
    }
}
//-------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::ProcessHappyHour(TPaymentTransaction &paymentTransaction, UnicodeString orderUniqueId)
{
    try
    {
        TManagerHappyHour* isHappyHour = new TManagerHappyHour();
        int priceLevel;

        //Check whether current time fall under any hh profile..
        bool happyHour = isHappyHour->IsCurrentTimeHappyHour(TDeviceRealTerminal::Instance().ID.DeviceKey,priceLevel);

        if(TGlobalSettings::Instance().ForceHappyHour)
        {
            happyHour = true;
            priceLevel = TGlobalSettings::Instance().HHPriceLevelKey;
        }

        if(happyHour)
        {
            for (int i = 0; i < paymentTransaction.Orders->Count; i++)
		    {
                TItemComplete *Order = (TItemComplete*)paymentTransaction.Orders->Items[i];

                int itemSizeKey = TDBOnlineOrdering::GetItemSizeKey(paymentTransaction.DBTransaction, Order->ItemKey, Order->Size);

                //Get HH Price as per item size and pricelevels
                Currency happyHourPrice = TDBOnlineOrdering::GetHappyHourPrice(paymentTransaction.DBTransaction, itemSizeKey, priceLevel);

                Order->SetPriceLevelCustom(happyHourPrice);
                Order->RunBillCalculator();

                //Update Happy Hour Price Corresponding to item key and item size.
                TDBOnlineOrdering::UpdateHappyHourPriceForItem(paymentTransaction.DBTransaction,Order->ItemKey, Order->Size, orderUniqueId, happyHourPrice,
                                Order->BillCalcResult.BasePrice);

                for (int j = 0; j < Order->SubOrders->Count; j++)
                {
                    TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);

                    if (SubOrder)
                    {
                        itemSizeKey = TDBOnlineOrdering::GetItemSizeKey(paymentTransaction.DBTransaction, SubOrder->ItemKey, SubOrder->Size);

                        //Get HH Price as per item size and pricelevels
                        happyHourPrice = TDBOnlineOrdering::GetHappyHourPrice(paymentTransaction.DBTransaction, itemSizeKey, priceLevel);

                        SubOrder->SetPriceLevelCustom(happyHourPrice);
                        SubOrder->RunBillCalculator();

                        //Update Happy Hour Price Corresponding to item key and item size.
                        TDBOnlineOrdering::UpdateHappyHourPriceForItem(paymentTransaction.DBTransaction,SubOrder->ItemKey, SubOrder->Size, orderUniqueId,
                                            happyHourPrice, SubOrder->BillCalcResult.BasePrice);
                    }
                }
            }
        }
    }
    catch(EAbort &E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::ManagePrintOperationsForWaiterApp(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID, bool IsDayArcBill)
{
    TList *OrdersWaiterApp = new TList;
    try
    {
        UnicodeString terminalName = "";

        //Get Terminal Name For OrderGUID Used For Printing KOT
        terminalName = TDBOnlineOrdering::GetTerminalNameForOrderGUID(dbTransaction, orderGUID);

        //Getting The OrderList Required For Printing
        OrdersWaiterApp = GetOrderItemList(dbTransaction, orderGUID);

        //Printing And Saving The Kitchen Docket
        TPaymentTransaction paymentTransactionNew(dbTransaction);

		paymentTransactionNew.SalesType = eWeb;
		paymentTransactionNew.Orders = OrdersWaiterApp;//Orders List

        PrintKitchenDocketsForWaiterApp(paymentTransactionNew, 15,orderGUID, terminalName);

        //Adding Payment Information to PaymentTrasaction Object
        AddPaymentInfo(paymentTransactionNew, orderGUID);

        //Creating And Saving The Receipt
        paymentTransactionNew.InvoiceNumber = TDBOnlineOrdering::GetInvoiceNumberForOrderGUID(paymentTransactionNew.DBTransaction, orderGUID);
        ArchiveReceiptForWaiterAppOrders(paymentTransactionNew, orderGUID, terminalName);

        //Set The IsPrintRequired Flag And Save The Generated Receipt Using OrderGuid After Performing Print Operations
        TDBOnlineOrdering::SetStatusAndSaveReceiptForWApp(paymentTransactionNew.DBTransaction, orderGUID, ManagerReceipt->ReceiptToArchive, IsDayArcBill);

        
        //Posting Invoice To Web For Waiter App Orders(Paid)
        GetAndUploadOnlineOrderingInvoice(paymentTransactionNew, terminalName, orderGUID, ManagerReceipt->ReceiptToArchive);
        
        //Posting To MEWS PMS
        if (TDeviceRealTerminal::Instance().BasePMS->Enabled)
        {
            if(TGlobalSettings::Instance().PMSType == Mews)
                ManagePostingToMews(paymentTransactionNew, orderGUID);
        }

    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//---------------------------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::PrintKitchenDocketsForWaiterApp(TPaymentTransaction &PaymentTransaction, int WebKey, UnicodeString JobName, UnicodeString DeviceName)
{

    try
    {
        // Print the Orders In the Kitchen if any.
        std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());
//
//        if(DeviceName == "")
//        {
//            DeviceName = "OnlineOrder";
//        }

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
                std::auto_ptr<TStringList>WebDetials(new TStringList);
                if(PaymentTransaction.Orders->Count)
                {
                    TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[0];
                    Request->Waiter = "WAITER";

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
                    for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
                    {
                        TSecurityReference *OldSecRef = Order->Security->SecurityGetType(secCredit);
                        if (OldSecRef == NULL)
                        {
                            PrintTransaction->Orders->Add(PaymentTransaction.Orders->Items[i]);
                        }
                    }
                    UnicodeString OrderType = "Order Type : ";
                    OrderType = OrderType + (Order->OnlineChitType > 0 ? (Order->OnlineChitType == 1 ? "DineIn" : "TakeAway")  : "PickUp");
                    WebDetials->Add(OrderType);
                    OrderType = "Order Id : ";
                    OrderType =  OrderType + (Order->OnlineOrderId);
                    WebDetials->Add(OrderType);
                }
                Request->Transaction->Money.Recalc(*Request->Transaction);
                if (PrintTransaction->Orders->Count > 0)
                {
                    std::auto_ptr<TKitchen> Kitchen(new TKitchen());
                    Kitchen->Initialise(PaymentTransaction.DBTransaction);
                    Kitchen->GetPrintouts(PrintTransaction->DBTransaction, Request.get());
                    if (!Request->Printouts->Print(devPalm,JobName))
                    {
                        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing Some Online Orders Failed, Please Check Printer.");
                        //throw Exception("Printing Some Orders Failed, Please Check Printer.");
                    }
                    ManagerDockets->Archive(Request.get());
                    SendOnlineOrderToChefmate(PrintTransaction.get());
                }
            }
        }
        else
        {
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Docket not Printed Device" +DeviceName+" Not Found!");
            throw Exception("Docket not Printed Device" +DeviceName+" Not Found!");
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
TList* TOnlineDocketPrinterThread::GetOrderItemList(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    TList *Orders =new TList;
    try
    {
        std::list<TItemInfoKey> itemInfoKeyList;

        TItem *item;
        TItemSize *inItemSize;
        itemInfoKeyList = TDBOnlineOrdering::GetItemInfoKeyListFromOrderGUID(dbTransaction, orderGUID);


        for(std::list<TItemInfoKey>::iterator it = itemInfoKeyList.begin(); it != itemInfoKeyList.end(); ++it)
        {

            //Getting Order Item Using Item Key
            item = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(it->itemId);

            //Getting Item Size ID and ItemSize
            inItemSize = item->Sizes->SizeGetByItemSizeKey(it->itemSizeKey);

            //Getting ItemComplete Using Item and ItemSize
            TItemComplete *itemComplete = new TItemComplete;
            LoadItemCompleteForWaiterAppOrder(dbTransaction,item,inItemSize,itemComplete, orderGUID, it->itemSizeIdentifierKey );

            //Adding ItemComplete to Order List
            Orders->Add(itemComplete);
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
    return Orders;
}
//------------------------------------------------------------------------------
TItemComplete* TOnlineDocketPrinterThread::LoadItemCompleteForWaiterAppOrder(Database::TDBTransaction &DBTransaction,
TItem* Item, TItemSize *inItemSize, TItemComplete *itemComplete, UnicodeString orderGUID, int itemSizeIdentifier)
{
    try
    {
	    //Loading Available ItemComplete Details From DayArchive And Archive Table
        TDBOnlineOrdering::LoadItemCompleteInfoForOrderGUID(DBTransaction, orderGUID, itemComplete, itemSizeIdentifier);

        //Loading Remaining ItemComplete Details
	    itemComplete->ItemOrderedFrom = Item;
	    itemComplete->Item = Item->Item;
	    itemComplete->ItemKitchenName = Item->ItemKitchenName;
	    itemComplete->Item_ID = Item->Item_ID;
	    itemComplete->MenuName = Item->MenuName;
	    itemComplete->MenuKey = Item->MenuKey;
	    itemComplete->ItemType = Item->ItemType;
	    itemComplete->PrintChitNumber = Item->PrintChitNumber;
	    itemComplete->OrderType = NormalOrder;
//	    itemComplete->TransNo = TDBOrder::GetNextTransNumber(DBTransaction); // Need to check
	    itemComplete->Course = Item->Course;
	    itemComplete->CourseKitchenName = Item->CourseKitchenName;
	    itemComplete->ItemAppearanceOrder = Item->ItemAppearanceOrder;
	    itemComplete->FontInfo = Item->FontInfo;
   	    itemComplete->IsPriceBarcodedItem = Item->IsPriceBarcodedItem;
        itemComplete->PrintCancel = false;
        itemComplete->IsPayByPoints = false;
	    itemComplete->Size = inItemSize->Name;
	    itemComplete->SizeKitchenName = inItemSize->SizeKitchenName;
	    itemComplete->TaxProfiles = inItemSize->TaxProfiles;
	    itemComplete->PointsPercent = inItemSize->PointsPercent;
	    itemComplete->SetMenuMask = inItemSize->SetMenuMask;
	    itemComplete->SetMenu = inItemSize->SetMenuItem;
	    itemComplete->SetMenuMaster = inItemSize->SetMenuMaster;
	    itemComplete->ThirdPartyKey = inItemSize->ThirdPartyKey;
	    itemComplete->ThirdPartyCode = inItemSize->ThirdPartyCode;
        itemComplete->RevenueCode = inItemSize->RevenueCode;
	    itemComplete->MemberFreeSaleCount = inItemSize->MemberFreeSaleCount;
	    itemComplete->MemberFreeSaleDiscount = inItemSize->MemberFreeSaleDiscount;
	    itemComplete->LocationFreeSaleCount = inItemSize->LocationFreeSaleCount;
	    itemComplete->LocationFreeSaleDiscount = inItemSize->LocationFreeSaleDiscount;
        itemComplete->ItemSizeIdentifierKey = inItemSize->ItemSizeIdentifierKey;
    	itemComplete->Categories->CategoryCopyList(inItemSize->Categories);
	    itemComplete->SalesRecipesToApply->RecipeCopyList(inItemSize->Recipes);
        itemComplete->MaxRetailPrice = inItemSize->MaxRetailPrice;
        itemComplete->PatronCount(itemComplete->DefaultPatronCount());
        itemComplete->ContainerTabType = TabTableSeat;
        itemComplete->TabType = TabTableSeat;
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
    return itemComplete;
}
//------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::ArchiveReceiptForWaiterAppOrders(TPaymentTransaction &PaymentTransaction, UnicodeString orderGUID, UnicodeString DeviceName)
{
    try
    {
        std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());
        if(WebDevice->NameToKey(PaymentTransaction.DBTransaction, DeviceName) != 0)
        {
            WebDevice->Load(PaymentTransaction.DBTransaction);

            std::auto_ptr<TReqPrintJob>LastReceipt(new TReqPrintJob(WebDevice.get()));

	        LastReceipt->Transaction = &PaymentTransaction;
            LastReceipt->JobType = pjReceiptReceipt;
            LastReceipt->SenderType = devPalm;
            LastReceipt->Waiter = "WAITER";
            LastReceipt->PaymentType = ptFinal;
    //	    LastReceipt->WaitTime = TDBSaleTimes::GetAverageWaitTimeMins(PaymentTransaction.DBTransaction);
            LastReceipt->MiscData = PaymentTransaction.MiscPrintData;

            std::auto_ptr<TReceipt> Receipt(new TReceipt());
            Receipt->Initialise(PaymentTransaction.DBTransaction);
            Receipt->GetPrintouts(PaymentTransaction.DBTransaction, LastReceipt.get(), TComms::Instance().ReceiptPrinter);

            //Receipt->GetPrintouts(PaymentTransaction.DBTransaction, LastReceipt, TComms::Instance().ReceiptPrinter);

            std::auto_ptr <TStringList> StringReceipt(new TStringList);
            //Get EFTPOS Receipt
            TMemoryStream *EFTPOSReceipt  = new  TMemoryStream;
            EFTPOSReceipt = TDBOnlineOrdering::GetEFTPOSReceiptForOrderGUID(PaymentTransaction.DBTransaction, orderGUID);
            StringReceipt->LoadFromStream(EFTPOSReceipt);

            //Merge EFTPOS Receipt
            MergePOSAndEFTPOSReceipt(StringReceipt, LastReceipt);

            StringReceipt->Clear();
            LastReceipt->Printouts->PrintToStrings(StringReceipt.get());

            //Saving Final Merged Receipt
            ManagerReceipt->ReceiptToArchive->Clear();
            ManagerReceipt->ReceiptToArchive->Position = 0;
            StringReceipt->SaveToStream(ManagerReceipt->ReceiptToArchive);


    //        if(TGlobalSettings::Instance().ExportReprintReceipt)
    //            ExportReceipt(StringReceipt.get(),PaymentTransaction);

            ManagerReceipt->ReceiptToArchive->Position = 0;

            for (int i = 0; i < StringReceipt->Count; i++)
            {
                TDeviceRealTerminal::Instance().SecurityPort->SetData(StringReceipt->Strings[i]);
            }
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::MergePOSAndEFTPOSReceipt(std::auto_ptr<TStringList> &eftPosReceipt, std::auto_ptr<TReqPrintJob> &LastReceipt)
{
    try
    {
        TPrintout *Printout = new TPrintout();
		if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey == 0)
		{
			TPrinterPhysical DefaultScreenPrinter;
			DefaultScreenPrinter.NormalCharPerLine = 40;
			DefaultScreenPrinter.BoldCharPerLine = 40;
			Printout->Printer = DefaultScreenPrinter;
		}
		else
		{
			Printout->Printer = TComms::Instance().ReceiptPrinter;
		}

		Printout->PrintFormat->Line->FontInfo.Reset();
		Printout->PrintFormat->Line->ColCount = 1;
		Printout->PrintFormat->Line->Columns[0]->Width = Printout->PrintFormat->Width;

        if(!TGlobalSettings::Instance().EnableEftPosAdyen)
		    Printout->PrintFormat->Line->Columns[0]->Alignment = taCenter;
        else
           Printout->PrintFormat->Line->Columns[0]->Alignment = taLeftJustify;

		Printout->PrintFormat->Line->Columns[0]->Text = Printout->PrintFormat->DocumentName;
		Printout->PrintFormat->AddLine();
		Printout->PrintFormat->NewLine();

		for (int i = 0; i < eftPosReceipt->Count; i++)
		{
			Printout->PrintFormat->Line->Columns[0]->Text = eftPosReceipt->Strings[i];
			Printout->PrintFormat->AddLine();
		}

		Printout->PrintFormat->PartialCut();
		// TODO : The last efpos receipt isnt cleared is the next transaction has no eftpos.
		// so all following transactions ha eftpos receipt attached
		LastReceipt->Printouts->Add(Printout);
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
void TOnlineDocketPrinterThread::AddPaymentInfo(TPaymentTransaction &PaymentTransaction, UnicodeString orderGUID)
{
    try
    {
        std::list<TPaymentInfo> paymentInfoList;

        //Loading Payment Details From DayArcBillPay
        paymentInfoList = TDBOnlineOrdering::GetPaymentInfoForOrderGUID(PaymentTransaction.DBTransaction, orderGUID);

        //Running Bill Calculator For Loading Cost And Assigning Payment Details
        PaymentTransaction.Recalc();
        TDeviceRealTerminal::Instance().PaymentSystem->PaymentsReload(PaymentTransaction);
        for(std::list<TPaymentInfo>::iterator it = paymentInfoList.begin(); it != paymentInfoList.end(); ++it)
        {
            PaymentTransaction.Type = eTransQuickSale;

            TPayment *Payment = PaymentTransaction.PaymentFind(it->payType);
            Payment->SetPay(it->amount);
        }

    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::ManagePostingToMews(TPaymentTransaction &PaymentTransaction, UnicodeString orderGUID)
{
    try
    {
        PaymentTransaction.IsBackGroundPosting = true;

        TDeviceRealTerminal::Instance().BasePMS->ExportData(PaymentTransaction, TDeviceRealTerminal::Instance().User.ContactKey);
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::GetAndUploadOnlineOrderingInvoice(TPaymentTransaction paymentTransaction, UnicodeString terminalName, UnicodeString orderGUID, TMemoryStream *ReceiptToArchive)
{
    try
    {
        TSiteOrderModel siteOrderModel = GetInvoiceInfoForOnlineOrdering(paymentTransaction, terminalName, orderGUID, ReceiptToArchive);

        TDeviceRealTerminal::Instance().PaymentSystem->GetAndUploadOnlineOrderingInvoice(siteOrderModel);
    }
     catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        //throw;
    }
}
//----------------------------------------------------------------------------
TSiteOrderModel TOnlineDocketPrinterThread::GetInvoiceInfoForOnlineOrdering(TPaymentTransaction paymentTransaction, UnicodeString terminalName, UnicodeString orderGUID, TMemoryStream *ReceiptToArchive)
{
    TSiteOrderModel siteOrderModel;
    try
    {
        TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[0]);
        siteOrderModel.CompanyId = 0;
        int siteID = 0;
        int onlineOrderID = 0;
        TDBOnlineOrdering::LoadSiteIDAndOOIdForOrderGuid(paymentTransaction.DBTransaction, orderGUID, siteID, onlineOrderID);
        siteOrderModel.SiteId = siteID;
        siteOrderModel.OrderId = onlineOrderID;
        siteOrderModel.Location = TDeviceRealTerminal::Instance().ID.Location;;
        siteOrderModel.TotalAmount = paymentTransaction.Money.RoundedGrandTotal;
        siteOrderModel.ContainerNumber = Order->TableNo;
        siteOrderModel.ContainerType = OnlineTable;
        siteOrderModel.ContainerName = Order->TabContainerName;
        siteOrderModel.OrderGuid = orderGUID;
        siteOrderModel.UserReferenceId = 0;
        siteOrderModel.UserType = 1;//         to do check whetrher user is a member or staff..
        siteOrderModel.TerminalName = terminalName;
        siteOrderModel.TransactionDate = Order->TimeStamp; // Add function
        siteOrderModel.OrderType = Order->OrderType;
        siteOrderModel.IsConfirmed = true;
        siteOrderModel.UserEmailId = Order->Email;
        siteOrderModel.OrderItems = GetOrderItemModel(paymentTransaction);
        siteOrderModel.TransactionType = eTableSeat;
        siteOrderModel.OrderInvoiceTransaction = GetOrderInvoiceTransaction(paymentTransaction, terminalName , orderGUID, ReceiptToArchive, siteID);
    }
     catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        throw;
    }
    return siteOrderModel;
}
//--------------------------------------------------------------------------------------
std::list<TOrderItemModel> TOnlineDocketPrinterThread::GetOrderItemModel(TPaymentTransaction paymentTransaction)
{
    std::list<TOrderItemModel> orderItemModelList;
    try
    {
        std::map<int, TOrderItemModel >OrderItemModelMap;
        for (int CurrentIndex = 0; CurrentIndex < paymentTransaction.Orders->Count; CurrentIndex++)
        {
            TOrderItemModel orderItemModel;
            TItemComplete* Order = (TItemComplete*)(paymentTransaction.Orders->Items[CurrentIndex]);


             std::map<int,TOrderItemModel>::iterator it;
             it = OrderItemModelMap.find(Order->OrderItemId);

             if(it == OrderItemModelMap.end())
             {
                orderItemModel.OrderItemId = Order->OrderItemId;
                orderItemModel.Name = Order->Item;
                orderItemModel.Description = Order->Item;
                orderItemModel.SiteItemId = 0;//Order->SiteId;
                orderItemModel.Price = Order->PriceEach_BillCalc();

                std::list<TOrderItemSizeModel> orderItemSizeModelList;

                TOrderItemSizeModel orderItemSizeModel;

                orderItemSizeModel.OrderItemSizeId = Order->OrderItemSizeId;
                orderItemSizeModel.OrderItemId = Order->OrderItemId;
                orderItemSizeModel.Name = Order->Size;
                orderItemSizeModel.ItemSizeId = Order->Item_ID;
                orderItemSizeModel.Quantity = Order->GetQty();
                orderItemSizeModel.MenuPrice = Order->PriceLevel0;
                orderItemSizeModel.Price = Order->PriceEach_BillCalc();
                orderItemSizeModel.PriceInclusive = Order->BillCalcResult.PriceIncl;
                orderItemSizeModel.BasePrice = Order->BillCalcResult.BasePrice;
                orderItemSizeModel.ReferenceOrderItemSizeId  = Order->ReferenceOrderItemSizeId;
                orderItemSizeModel.OrderItemSizeDiscounts = GetOrderItemSizeDiscountModel(Order);

                orderItemSizeModelList.push_back(orderItemSizeModel);

                orderItemModel.OrderItemSizes = orderItemSizeModelList;//.push_back(orderItemSizeModel);

                OrderItemModelMap[Order->OrderItemId] = orderItemModel;
            }
            else
            {
                it->second.Price = it->second.Price + Order->PriceEach_BillCalc();

                TOrderItemSizeModel orderItemSizeModel;

                orderItemSizeModel.OrderItemSizeId = Order->OrderItemSizeId;
                orderItemSizeModel.OrderItemId = Order->OrderItemId;
                orderItemSizeModel.Name = Order->Size;
                orderItemSizeModel.ItemSizeId = Order->Item_ID;
                orderItemSizeModel.Quantity = Order->GetQty();
                orderItemSizeModel.MenuPrice = Order->PriceLevel0;
                orderItemSizeModel.Price = Order->PriceEach_BillCalc();
                orderItemSizeModel.PriceInclusive = Order->BillCalcResult.PriceIncl;
                orderItemSizeModel.BasePrice = Order->BillCalcResult.BasePrice;
                orderItemSizeModel.ReferenceOrderItemSizeId  = Order->ReferenceOrderItemSizeId;
                orderItemSizeModel.OrderItemSizeDiscounts = GetOrderItemSizeDiscountModel(Order);

                it->second.OrderItemSizes.push_back(orderItemSizeModel);
            }
        }

        //Now iterate values from map and insert it into list..
        for(std::map<int,TOrderItemModel>::iterator it = OrderItemModelMap.begin();  it != OrderItemModelMap.end(); it++)
        {
            orderItemModelList.push_back(it->second);
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        throw;
    }
    return orderItemModelList;
}
//--------------------------------------------------------------------------------------
std::list<TOrderItemSizeDiscountModel> TOnlineDocketPrinterThread::GetOrderItemSizeDiscountModel(TItemMinorComplete *Order)
{
    std::list<TOrderItemSizeDiscountModel> orderItemSizeDiscountModelList;
    try
    {
        for (std::vector <TDiscount> ::const_iterator ptrDiscounts = Order->Discounts.begin(); ptrDiscounts != Order->Discounts.end();
        std::advance(ptrDiscounts, 1))
        {
            if(Order->DiscountValue_BillCalc(ptrDiscounts) == 0)
                continue;

            TOrderItemSizeDiscountModel orderItemSizeDisountModel;
            orderItemSizeDisountModel.OrderItemSizeId = Order->OrderItemSizeId;
            orderItemSizeDisountModel.Name = ptrDiscounts->Name.SubString(1, 15);
            orderItemSizeDisountModel.Value = RoundToNearest(Order->DiscountValue_BillCalc(ptrDiscounts),0.01,TGlobalSettings::Instance().MidPointRoundsDown);
            orderItemSizeDisountModel.Code = ptrDiscounts->DiscountCode;
            orderItemSizeDiscountModelList.push_back(orderItemSizeDisountModel);
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        throw;
    }
    return orderItemSizeDiscountModelList;
}
//-------------------------------------------------------------------------------------------------
TOrderInvoiceTransactionModel TOnlineDocketPrinterThread::GetOrderInvoiceTransaction(TPaymentTransaction paymentTransaction, UnicodeString terminalName ,UnicodeString orderGUID, TMemoryStream *ReceiptToArchive, int siteID)
{
    TOrderInvoiceTransactionModel orderInvoiceTransactionModel;
    try
    {
        orderInvoiceTransactionModel.OrderInvoiceTransactionId = 0;
        orderInvoiceTransactionModel.OrderId = 0;//;
        orderInvoiceTransactionModel.InvoiceTransactionId = 0;           //todo
        orderInvoiceTransactionModel.InvoiceTransaction = GetInvoiceTransaction(paymentTransaction, terminalName, orderGUID, ReceiptToArchive, siteID);
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        throw;
    }
    return orderInvoiceTransactionModel;
}
//---------------------------------------------------------------------------------------
TInvoiceTransactionModel TOnlineDocketPrinterThread::GetInvoiceTransaction(TPaymentTransaction paymentTransaction, UnicodeString terminalName, UnicodeString orderGUID, TMemoryStream *ReceiptToArchive, int siteID)
{
    TInvoiceTransactionModel invoiceTransactionModel;
    try
    {
         TItemComplete *Order = (TItemComplete*)(paymentTransaction.Orders->Items[0]);
         invoiceTransactionModel.InvoiceTransactionId = 0;;
	     invoiceTransactionModel.InvoiceNumber = paymentTransaction.InvoiceNumber;
	     invoiceTransactionModel.TotalSaleAmount = paymentTransaction.Money.RoundedGrandTotal;
	     invoiceTransactionModel.TransactionDate = Order->TimeStamp;
	     invoiceTransactionModel.SiteId = siteID;
	     invoiceTransactionModel.TerminalName = terminalName;

         if( ReceiptToArchive->Size > 0 )
		 {
            ReceiptToArchive-> Position =  0 ;
            invoiceTransactionModel.Receipt.set_length ( ReceiptToArchive->Size) ;
            ReceiptToArchive->ReadBuffer ( & invoiceTransactionModel.Receipt[ 0 ] , ReceiptToArchive-> Size ) ;
		 }

	     invoiceTransactionModel.ReceiptPath = "";
	     invoiceTransactionModel.Rounding = RoundToNearest(paymentTransaction.Money.RoundingAdjustment, 0.01,
                                                    TGlobalSettings::Instance().MidPointRoundsDown);
	     invoiceTransactionModel.UserReferenceId = 0;
	     invoiceTransactionModel.UserType = 1;//         to do check whetrher user is a member or staff..
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        throw;
    }
    return invoiceTransactionModel;
}
//------------------------------------------------------------------------------
bool TOnlineOrderDocketPrinter::IsPrePaidOrderPrintingPending(Database::TDBTransaction &dbTransaction)
{
    bool retVal = false;
    try
    {
        TIBSQL *IBInternalQuery = dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text = "SELECT a.ARCBILL_KEY "
                                      "FROM DAYARCBILL a "
                                      "WHERE a.IS_PRINT_REQUIRED = :IS_PRINT_REQUIRED "
                                      "UNION ALL "
                                      "SELECT a.ARCBILL_KEY "
                                      "FROM ARCBILL a "
                                      "WHERE a.IS_PRINT_REQUIRED = :IS_PRINT_REQUIRED ";

        IBInternalQuery->ParamByName("IS_PRINT_REQUIRED")->AsString = "T";
        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount)
            retVal = true;

    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Ex.Message);
        throw;
    }
    return retVal;
}
