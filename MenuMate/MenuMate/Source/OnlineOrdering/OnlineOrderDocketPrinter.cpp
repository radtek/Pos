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
        PrepareDataAndPrintDocket(dbTransaction );
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
    //MessageBox("PrepareDataAndPrintDocket","PrepareDataAndPrintDocket",MB_OK);
    //PrintKitchenDocketsForWaiterApp();

        UnicodeString orderUniqueIdForWAiterApp = TDBOnlineOrdering::GetOnlineOrderGUIDForWaiterApp(dBTransaction);
        MessageBox(orderUniqueIdForWAiterApp,"orderUniqueIdForWAiterApp",MB_OK);
        if(orderUniqueIdForWAiterApp.Trim() != "")
            ManagePrintOperationsForWaiterApp(dBTransaction, orderUniqueIdForWAiterApp);

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
        PaymentTransaction.IgnoreLoyaltyKey = false;
		PaymentTransaction.Recalc();

		ProcessSecurity(PaymentTransaction);

		// Print the Order to the Kitchen.
		PrintKitchenDockets(PaymentTransaction, 15,orderUniqueId,"OnlineOrder");   //tab key to be changed

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
//---------------------------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::PrintKitchenDocketsForWaiterApp()
{
Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);
    dbTransaction.StartTransaction();
    try
    {
        // Print the Orders In the Kitchen if any.
        std::auto_ptr<TDeviceWeb>WebDevice(new TDeviceWeb());

//        if(DeviceName == "")
//        {
//            DeviceName = "WAITER APP";
//        }
//        if(WebDevice->NameToKey(PaymentTransaction.DBTransaction, DeviceName) != 0)
//        {
//            WebDevice->Load(PaymentTransaction.DBTransaction);
            MessageBox("Before","PrepareDataAndPrintDocket",MB_OK);
            std::auto_ptr<TReqPrintJob>Request(new TReqPrintJob(WebDevice.get()));
            std::auto_ptr<TPaymentTransaction>PrintTransaction(new TPaymentTransaction(dbTransaction));
                  MessageBox("After","PrepareDataAndPrintDocket",MB_OK);
            // Copy over the Patron Counts for Printing.
//            PrintTransaction->Patrons =   1;
//            PaymentTransaction.Patrons;

//            if (!PaymentTransaction.CreditTransaction)
//            {
//                Request->BarCodeData = PaymentTransaction.TimeKey;
                Request->Transaction = PrintTransaction.get();
                Request->JobType = pjKitchen;
//                std::auto_ptr<TStringList>WebDetials(new TStringList);
//                if(PaymentTransaction.Orders->Count)
//                {
MessageBox("TItemComplete","PrepareDataAndPrintDocket",MB_OK);
                    TItemComplete *Order;
                    MessageBox("TItemComplete after","PrepareDataAndPrintDocket",MB_OK);
//                     = (TItemComplete*)PaymentTransaction.Orders->Items[0];
//                    Request->Waiter = Order->Security->SecurityGetField(secOrderedBy, secfFrom);

//                    if (Order->TabType == TabTableSeat)
//                    {
//                       if(TDBTables::Valid(Order->TableNo,Order->SeatNo))
//                       {
 MessageBox("TabContainerName before","PrepareDataAndPrintDocket",MB_OK);
                            Order->TabContainerName =  "APP";
                            MessageBox("TabContainerName after","PrepareDataAndPrintDocket",MB_OK);
//                             TDBTables::GetTableName(PaymentTransaction.DBTransaction,Order->TableNo);
                            Order->PartyName =   "WAITER APP";
                            MessageBox("PartyName after","PrepareDataAndPrintDocket",MB_OK);
//                            TDBTables::GetPartyName(PaymentTransaction.DBTransaction,Order->TableNo);
//                       }
//                    }
//                    else if (Order->TabType == TabRoom)
//                    {
//                            Order->PartyName 	= TDBRooms::GetPartyName(PaymentTransaction.DBTransaction,Order->RoomNo);
//                            Order->TabContainerName = TDBRooms::GetRoomName(PaymentTransaction.DBTransaction,Order->RoomNo);
//                            Order->TabName = Order->PartyName;
//                    }
//                    else if (Order->TabType == TabWeb)
//                    {
//                            Order->TabContainerName = TDBWebUtil::GetOrderGUID(PaymentTransaction.DBTransaction,Order->WebKey);
//                    }

//                    Request->Transaction->Membership.Assign(PaymentTransaction.Membership);
//                    PrintTransaction->ChitNumber = 0;
                    MessageBox("party before","PrepareDataAndPrintDocket",MB_OK);
                    Request->MiscData["PartyName"] = "Testing";
                    MessageBox("party after","PrepareDataAndPrintDocket",MB_OK);
//                    for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
//                    {
//                        TSecurityReference *OldSecRef = Order->Security->SecurityGetType(secCredit);
//                        if (OldSecRef == NULL)
//                        {
MessageBox("PrintTransaction before","PrepareDataAndPrintDocket",MB_OK);
                            PrintTransaction->Orders->Add(Order);
                            MessageBox("PrintTransaction after","PrepareDataAndPrintDocket",MB_OK);
//                        }
//                    }
                    UnicodeString OrderType = "Order Type : ";
//                    OrderType = OrderType + (Order->OnlineChitType > 0 ? (Order->OnlineChitType == 1 ? "DineIn" : "TakeAway")  : "PickUp");
//                    WebDetials->Add(OrderType);
//                    OrderType = "Order Id : ";
//                    OrderType =  OrderType + (Order->OnlineOrderId);
//                    WebDetials->Add(OrderType);
//                }

//                if (WebKey != 0)
//                {
//                    TDBWebUtil::getWebOrderDetials(PaymentTransaction.DBTransaction, WebKey, *WebDetials.get());
//                    Request->ExtraInfo->AddStrings(WebDetials.get());
//
//                    std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
//                    std::auto_ptr<TStringList>WebComments(new TStringList);
//                    std::auto_ptr<TStringList>WebPaymentDetials(new TStringList);
//                    TDBWebUtil::getWebOrderData(PaymentTransaction.DBTransaction, WebDeliveryDetials.get(), WebPaymentDetials.get(), WebComments.get(), WebKey);
//
//                    Request->PaymentInfo->AddStrings(WebPaymentDetials.get());
//                    Request->OrderComments->AddStrings(WebComments.get());
////                    UnicodeString _orderType = checkWebOrderType(PaymentTransaction.DBTransaction, WebKey);
//                    if(_orderType == "Pickup")
//                    {
//                        WebDeliveryDetials->Clear(); //remove delivery info for pickup order in kitchen docket..
//                    }
//                    Request->DeliveryInfo->AddStrings(WebDeliveryDetials.get());
//                }
//                PrintTransaction->WebOrderKey =  WebKey;
//
//                Request->Transaction->Money.Recalc(*Request->Transaction);
//                if (PrintTransaction->Orders->Count > 0)
//                {
                    std::auto_ptr<TKitchen> Kitchen(new TKitchen());
                    Kitchen->Initialise(dbTransaction);
                    Kitchen->GetPrintouts(dbTransaction, Request.get());
                    Request->Printouts->Print(devPalm);
//                    if (!Request->Printouts->Print(devPalm,JobName))
//                    {
//                        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing Some Online Orders Failed, Please Check Printer.");
//                        //throw Exception("Printing Some Orders Failed, Please Check Printer.");
//                    }
//                            ManagerDockets->Archive(Request.get());




                            std::auto_ptr<TMemoryStream> receiptStream(new TMemoryStream);
                            receiptStream->LoadFromStream(ManagerDockets->DocketToArchive);
                            receiptStream->Position = 0;
                            AnsiString ReceiptData((char *)receiptStream->Memory,receiptStream->Size);
                            MessageBox(ReceiptData,"ReceiptData",MB_OK);

//                    SendOnlineOrderToChefmate(PrintTransaction.get());
                        dbTransaction.Commit();
//                }
//            }
//        }
//        else
//        {
//            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Docket not Printed Device" +DeviceName+" Not Found!");
//            throw Exception("Docket not Printed Device" +DeviceName+" Not Found!");
//        }
    }
    catch(Exception & E)
	{
        dbTransaction.Rollback();
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::ManagePrintOperationsForWaiterApp(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    try
    {
        std::auto_ptr<TList> Orders(new TList);
        //Getting The OrderList Required For Printing
        Orders = GetOrderItemList(dbTransaction, orderGUID);
        //Printing And Saving The Kitchen Docket
        //Creating And Saving The Receipt
        //Set The IsPrintRequired Flag After Performing Print Operations
        TDBOnlineOrdering::SetOnlineOrderStatusForWaiterApp(dbTransaction, orderGUID);
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
//------------------------------------------------------------------------------
std::auto_ptr<TList> TOnlineDocketPrinterThread::GetOrderItemList(Database::TDBTransaction &dbTransaction, UnicodeString orderGUID)
{
    std::auto_ptr<TList> Orders(new TList);
    try
    {
        std::vector<UnicodeString> sizeNameList;
        std::vector<int> itemKeysList;
        long itemSizeID = 0;
        TItem *item;

        TItemSize *inItemSize;
        MessageBox("Before getting item","ITem Name",MB_OK);
        TDBOnlineOrdering::GetItemKeysFromOrderGUID(dbTransaction, orderGUID,&sizeNameList, &itemKeysList);
        for(int i = 0; i < itemKeysList.size(); i++)
        {
            MessageBox("Before getting item","ITem Name",MB_OK);
            //Getting Order Item Using Item Key
            item = TDeviceRealTerminal::Instance().Menus->VisibleMenu->FetchItemByKey(itemKeysList[i]);
              MessageBox("After getting item","ITem Name",MB_OK);
            //Getting Item Size ID and ItemSize
              MessageBox("Before getting itemsize key","ITem Name",MB_OK);
            itemSizeID = TDBOnlineOrdering::GetItemSizeKeyFromItemKeyAndSizeName(dbTransaction, sizeNameList[i],itemKeysList[i]);
              MessageBox("After getting itemsizekey","ITem Name",MB_OK);
            inItemSize = item->Sizes->SizeGetByItemSizeKey(itemSizeID);
              MessageBox("After getting itemsize","ITem Name",MB_OK);
            //Getting ItemComplete Using Item and ItemSize
            TItemComplete *itemComplete = new TItemComplete;
            LoadItemCompleteForWaiterAppOrder(dbTransaction,item,inItemSize,itemComplete);
              MessageBox("After getting itemcomplete","ITem Name",MB_OK);
            //Adding ItemComplete to Order List
            Orders->Add(itemComplete);
              MessageBox("After adding item in order getting item","ITem Name",MB_OK);
        }
        for(int k = 0; k< Orders->Count; k++)
        {
            TItemComplete *OrderItem = (TItemComplete*)Orders->Items[k];
            MessageBox(OrderItem->Item,"ITem Name",MB_OK);
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
                                                                TItem* Item, TItemSize *inItemSize, TItemComplete *itemComplete)
{
    //TItemSize *inItemSize;
    try
    {
	    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	    itemComplete->ItemOrderedFrom = Item;
	    itemComplete->TableNo = 0;// Get Table no
	    itemComplete->SeatNo = 0;//Get Seat no
	    itemComplete->ItemKey = Item->ItemKey;// Get Time Key
	    itemComplete->Item = Item->Item;
	    itemComplete->ItemKitchenName = Item->ItemKitchenName;
	    itemComplete->Item_ID = Item->Item_ID;
	    itemComplete->MenuName = Item->MenuName;
	    // TODO 4 -o  Michael -c Printing : Remove When Printing Engine Finished
	    itemComplete->MenuKey = Item->MenuKey;
	    itemComplete->ItemType = Item->ItemType;
	    itemComplete->PrintChitNumber = Item->PrintChitNumber;
	    itemComplete->OrderType = NormalOrder;
	    //itemComplete->HappyHour = stHappyHour->Visible;
	    itemComplete->TimeStamp = Now();       // Get Timestamp
	    itemComplete->TransNo = TDBOrder::GetNextTransNumber(DBTransaction); // Need to check

	    itemComplete->Note = "";//Need to check
	    itemComplete->Course = Item->Course;
	    itemComplete->CourseKitchenName = Item->CourseKitchenName;
         //Need to check
        //itemComplete->ServingCourse =  TDeviceRealTerminal::Instance().Menus->GetServingCourse(GetDefaultServingCourse(itemComplete->ItemKey));

	    itemComplete->ItemAppearanceOrder = Item->ItemAppearanceOrder;
	    itemComplete->FontInfo = Item->FontInfo;
	    //itemComplete->Loyalty_Key = SeatOrders[SelectedSeat]->Orders->AppliedMembership.ContactKey;
	    itemComplete->SetQty(Item->GetQty());// Need to check
   	    itemComplete->IsPriceBarcodedItem = Item->IsPriceBarcodedItem;
        itemComplete->PrintCancel = false;
        //itemComplete->LastAddedItem = true; // check item is last added or not..
        itemComplete->IsPayByPoints = false; // initialize pay by points to be false..

		//inItemSize = itemSize;
        //Item->Sizes->SizeGet(0);  // Pass key of Item Size
	    itemComplete->Size = inItemSize->Name;

	    itemComplete->SizeKitchenName = inItemSize->SizeKitchenName;
        itemComplete->GSTPercent = inItemSize->GSTPercent;
	    itemComplete->TaxProfiles = inItemSize->TaxProfiles;
	    itemComplete->PointsPercent = inItemSize->PointsPercent;
	    itemComplete->SetMenuMask = inItemSize->SetMenuMask;
	    itemComplete->SetMenu = inItemSize->SetMenuItem;
        // && SetMenuItem;
	    itemComplete->SetMenuMaster = inItemSize->SetMenuMaster;
	    itemComplete->ThirdPartyKey = inItemSize->ThirdPartyKey;
	    itemComplete->ThirdPartyCode = inItemSize->ThirdPartyCode;
        itemComplete->RevenueCode = inItemSize->RevenueCode;
	    itemComplete->PLU = inItemSize->PLU;

	    itemComplete->MemberFreeSaleCount = inItemSize->MemberFreeSaleCount;
	    itemComplete->MemberFreeSaleDiscount = inItemSize->MemberFreeSaleDiscount;
	    itemComplete->LocationFreeSaleCount = inItemSize->LocationFreeSaleCount;
	    itemComplete->LocationFreeSaleDiscount = inItemSize->LocationFreeSaleDiscount;
        itemComplete->ItemSizeIdentifierKey = inItemSize->ItemSizeIdentifierKey;

	    // Sort Categories
    	itemComplete->Categories->CategoryCopyList(inItemSize->Categories);
	    // Sort Recipes
	    itemComplete->SalesRecipesToApply->RecipeCopyList(inItemSize->Recipes);

	    itemComplete->PriceLevel0 = inItemSize->Price; // Get Price Level0
   	    //itemComplete->PriceLevel1 =;

	    itemComplete->Cost = inItemSize->Cost; // Get default cost if assigned.
        itemComplete->MaxRetailPrice = inItemSize->MaxRetailPrice;
	    itemComplete->CostGSTPercent = inItemSize->CostGSTPercent;
        itemComplete->PatronCount(itemComplete->DefaultPatronCount());

    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
    return itemComplete;
}
