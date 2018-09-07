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
//        Database::TDBTransaction dBTransaction(TDeviceRealTerminal::Instance().DBControl);
//        TDeviceRealTerminal::Instance().RegisterTransaction(dBTransaction);
//        dBTransaction.StartTransaction();

        UnicodeString orderUniqueId = TDBOnlineOrdering::GetOnlineOrderId(dBTransaction);
        if(orderUniqueId.Trim() == "")
            return;
		// Load the Order.
		TPaymentTransaction PaymentTransaction(dBTransaction);
		PaymentTransaction.SalesType = eWeb;
//		PaymentTransaction.TimeKey = WebOrder.TimeKey;

	 //	std::set<__int64>ItemsTabs;
	  //	ItemsTabs.insert(WebOrder.TabKey);
		TDBOnlineOrdering::GetOrdersByOnlineOrderId(dBTransaction, PaymentTransaction.Orders, orderUniqueId);

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

//	    ProcessChitNumbers(inDisplayOwner, PaymentTransaction);   //UI Distortion
//		ProcessPatrons(PaymentTransaction, PaymentTransaction.SalesType, 1);
		ProcessSecurity(PaymentTransaction);

		// Time Tracking
 //		ProcessTimeTracking(PaymentTransaction);
		// Stock
//		ProcessStock(PaymentTransaction);

  //      PaymentTransaction.ChitNumber.DeliveryTime = WebOrder.DateExpected;

     //   int WebKey = TDBWebUtil::GetWebOrderKeyByTabKey(DBTransaction, WebOrder.TabKey);
       // std::auto_ptr<TStringList>WebDeliveryDetials(new TStringList);
        //TDBWebUtil::getWebOrderExtraData(DBTransaction, WebKey, "DELIVERY", WebDeliveryDetials.get());

//        std::auto_ptr<TStringList>WebDetials(new TStringList);
//        TDBWebUtil::getWebOrderDetials(DBTransaction, WebKey, *WebDetials.get());
//        TMMContactInfo webMember;
//
//        if(WebOrderChitNumber.Valid())
//        {
//            for (int i = 0; i < PaymentTransaction.Orders->Count; i++)
//            {
//                TItemComplete *Order = (TItemComplete*)PaymentTransaction.Orders->Items[i];
//                Order->ChitNumber = WebOrderChitNumber;
//                if (Order->ChitNumber.ChitNumberKey != 0)
//                {
//                    int activeChitKey = TDBActiveChit::GetOrCreateActiveChit(DBTransaction, Order->ChitNumber);
//                    Order->SetActiveChitNumberKey(activeChitKey);
//                }
//                TDBOrder::UpdateOrderTableForWebOrders(DBTransaction, Order);
//            }
//
//        }

//        if(TGlobalSettings::Instance().CaptureCustomerName)
//        {
//            webMember.Clear();
//            if(!WebOrderChitNumber.Valid())
//            {
//               WebDeliveryDetials->Clear();
//            }
//        }
//        if(WebOrderChitNumber.Valid())
//        {
//           if(WebOrderChitNumber.OnlineDeliveryOrder)
//           {
//               webMember = TDBWebUtil::autoAddWebMembers(DBTransaction, WebDetials.get(), WebDeliveryDetials.get(), WebKey);
//               if(Member.ContactKey > 0)
//               {
//                  webMember = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, Member, WebDeliveryDetials.get());
//               }
//               else
//               {
//                   webMember = TDBWebUtil::checkAppliedChit(WebOrderChitNumber, webMember, WebDeliveryDetials.get());
//               }
//           }
//           else
//           {
//              webMember.Clear();
//              WebDeliveryDetials->Clear();
//           }
//           if(WebOrderChitNumber.PromptForPickUpDeliveryTime)
//           {
//                if((double)WebOrderChitNumber.DeliveryTime > 0)
//                {
//                   PaymentTransaction.ChitNumber.DeliveryTime = WebOrderChitNumber.DeliveryTime;
//                }
//                else
//                {
//                   WebOrderChitNumber.DeliveryTime = WebOrder.DateExpected;
//                }
//           }
//           else
//           {
//               WebOrderChitNumber.DeliveryTime = WebOrder.DateExpected;
//           }
//           PaymentTransaction.ChitNumber = WebOrderChitNumber;
//        }
//        PaymentTransaction.Membership.Assign(webMember, emsManual);
//        PaymentTransaction.WebOrderKey =  WebOrder.WebKey;

		// Print the Order to the Kitchen.
		PrintKitchenDockets(PaymentTransaction, 15,orderUniqueId,"OnlineOrder");   //tab key to be changed
		// Print the Receipts.
		AutoPrintReceipts(TabWeb, PaymentTransaction);

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
                        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing Some Web Orders Failed, Please Check Printer.");
                        //throw Exception("Printing Some Orders Failed, Please Check Printer.");
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
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
    }
}
// ---------------------------------------------------------------------------
void TOnlineDocketPrinterThread::AutoPrintReceipts(TMMTabType TabType, TPaymentTransaction &PaymentTransaction)
{
    try
    {
        if (true)  //to be changed..
        {
            UnicodeString str = "Inside  AutoPrintReceipts ";
            str = str + Now()+ "\n";

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
//                    UnicodeString _orderType = checkWebOrderType(PaymentTransaction.DBTransaction, WebKey);
//                    if(_orderType == "Pickup")
//                    {
//                        WebDeliveryDetials->Clear(); //remove delivery info for pickup order in kitchen docket..
//                    }
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
//                TempReceipt->JobType = pjReceiptInvoice;
//                TempReceipt->PaymentType = ptFinal;
//                std::set<__int64>::iterator pTabKey = SelectedTabs.begin();
//                int InvoiceKey = TDBTab::GetTabInvoice(InvoiceTransaction.DBTransaction, *pTabKey);
//                InvoiceTransaction.InvoiceNumber = Invoice->GetInvoiceNumber(InvoiceTransaction.DBTransaction, InvoiceKey);
//                int ContactKey = Invoice->GetInvoiceOwner(InvoiceTransaction.DBTransaction, InvoiceKey);
//
//                TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(InvoiceTransaction.DBTransaction, ContactKey, InvoiceOwnerInfo);
//
//                TempReceipt->ExtraInfo->Add("Name: " + InvoiceOwnerInfo.Name);
//                TempReceipt->ExtraInfo->Add("Member No. " + InvoiceOwnerInfo.MembershipNumber);
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
            //if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
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
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
    }
}
//--------------------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::completeOrderToChefMate(TPaymentTransaction* inTransaction)
{
    if(inTransaction->WebOrderKey > 0)
    {
        sendWebOrderToChefmate(inTransaction);
    }
}
//-----------------------------------------------------------------------------------------
void TOnlineDocketPrinterThread::sendWebOrderToChefmate(TPaymentTransaction* inTransaction)
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
