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
//-------------------------------------------------------------------------------------
