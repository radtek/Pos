//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBOrder.h"
#include "ItemCompleteSub.h"
#include "MMLogging.h"
#include "OrderUtils.h"
#include "DBActiveChit.h"
#include "DBTab.h"
#include "DBTables.h"
#include "DBSecurity.h"
#include "DBRooms.h"
#include "DBThirdPartyCodes.h"
#include "DBSecurity.h"
#include "MMUtilFunc.h"
#include "DBWebUtil.h"
#include "TaxProfileDBAccessManager_MM.h"

#include "ItemSizeCategory.h"
#include "ManagerDiscount.h"
#include "ItemRecipe.h"
#include "enumMenu.h"
#include "EnumOrderStatus.h"
#include "SCDPWDChecker.h"
#include "Invoice.h"
//---------------------------------------------------------------------------
// Only used to reload items for paarked sales.
#include "DeviceRealTerminal.h"
//---------------------------------------------------------------------------
#include "ManagerDiscount.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TDBOrder::TDBOrder()
{
}

TDBOrder::~TDBOrder()
{
}

__int64 TDBOrder::SplitOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey, int WaysToSplit)
{
	// Price = Price / WaysToSplit
	// Qty = Qty / WaysToSpit
	// Duplicate edited Item Ways To Split-1 times.
	// Recursivly split Sides this item has.
   	if( WaysToSplit == 1 || WaysToSplit == 0)
       return 1;

	TItemComplete* Order = new TItemComplete();
	Order->OrderKey  = OrderKey;
	GetOrderIncludingSidesFromOrderKey(DBTransaction,Order);
	Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);

	TItemComplete* OrderImage = new TItemComplete();
	OrderImage->OrderKey 		= OrderKey;
	GetOrderIncludingSidesFromOrderKey(DBTransaction,OrderImage);
	OrderImage->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(OrderImage->OriginalItemKey);

	Currency tempPatronCount = Currency(Order->PatronCount()) / WaysToSplit;
	int splitItemPatronCount = tempPatronCount + (tempPatronCount % 1 != 0);
	int itemPatronCount 	 = Order->PatronCount();

	OrderImage->SplitOrder(WaysToSplit);
    OrderImage->RunBillCalculator();

    for (int i = 0; i < OrderImage->SubOrders->Count ; i++)
    {
        TItemCompleteSub *SubOrder = (TItemCompleteSub *)OrderImage->SubOrders->Items[i];
        SubOrder->RunBillCalculator();
    }

	int SplitCount = 1;
	bool SplitAsMuchAsPossible = false;
	do
	{
		SplitAsMuchAsPossible = !DeductOrder(Order, OrderImage);
        OrderImage->RunBillCalculator();
		if (!SplitAsMuchAsPossible)
		{
			OrderImage->TransNo	= GetNextTransNumber(DBTransaction);
  			OrderImage->PatronCount(0, false);
			if (itemPatronCount > splitItemPatronCount)
            {
				OrderImage->PatronCount(splitItemPatronCount, false);
				itemPatronCount -= splitItemPatronCount;
			}
			Order->PatronCount(itemPatronCount, false);
			SetOrder(DBTransaction, OrderImage);
			TDBSecurity::ProcessSecurity(DBTransaction,OrderImage->Security);
			for (int i = 0; i < OrderImage->SubOrders->Count; i++)
			{
				TItemCompleteSub *SubOrder = OrderImage->SubOrders->SubOrderGet(i);
				if(SubOrder)
				{
					TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security);
				}
			}
		}
		SplitCount++;
	}
	while (SplitCount < WaysToSplit && SplitAsMuchAsPossible == false);
    Order->RunBillCalculator();
    UpdateOrder(DBTransaction, Order);
    delete Order;
    delete OrderImage;
	return SplitCount;
}

__int64 TDBOrder::SplitOrderToQuantity(Database::TDBTransaction &DBTransaction,__int64 OrderKey, double quantityToSplit)
{
	TItemComplete* Order = new TItemComplete();
	Order->OrderKey  = OrderKey;
	GetOrderIncludingSidesFromOrderKey(DBTransaction,Order);
	Order->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(Order->OriginalItemKey);
    double quantity = Order->GetQty();
    if(quantityToSplit >= quantity)
       return 1;

	TItemComplete* OrderImage = new TItemComplete();
	OrderImage->OrderKey 		= OrderKey;
	GetOrderIncludingSidesFromOrderKey(DBTransaction,OrderImage);
	OrderImage->ItemOrderedFrom = TDeviceRealTerminal::Instance().Menus->FetchItemByKey(OrderImage->OriginalItemKey);

	double tempPatronCount = (Order->PatronCount() * quantityToSplit)  / quantity;
	int splitItemPatronCount = (int)tempPatronCount ;//+ (tempPatronCount % 1 != 0);
    splitItemPatronCount += (tempPatronCount - splitItemPatronCount) > 0 ? 1 : 0;
	int itemPatronCount 	 = Order->PatronCount() - splitItemPatronCount;

	OrderImage->SplitOrder(quantityToSplit);
    OrderImage->RunBillCalculator();

    for (int i = 0; i < OrderImage->SubOrders->Count ; i++)
    {
        TItemCompleteSub *SubOrder = (TItemCompleteSub *)OrderImage->SubOrders->Items[i];
        SubOrder->RunBillCalculator();
    }
    Order->SetQtyCustom(quantity - quantityToSplit);
	//Order->Cost -= OrderImage->Cost;
	Order->Redeemed -= OrderImage->Redeemed;
    Order->PatronCount(itemPatronCount, false);

    OrderImage->TransNo	= GetNextTransNumber(DBTransaction);
    OrderImage->PatronCount(0, false);
    if (itemPatronCount > splitItemPatronCount)
    {
        OrderImage->PatronCount(splitItemPatronCount, false);
    }
    SetOrder(DBTransaction, OrderImage);
    TDBSecurity::ProcessSecurity(DBTransaction,OrderImage->Security);
    for (int i = 0; i < OrderImage->SubOrders->Count; i++)
    {
        TItemCompleteSub *SubOrder = OrderImage->SubOrders->SubOrderGet(i);
        if(SubOrder)
        {
            TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security);
        }
    }
    Order->RunBillCalculator();
    UpdateOrder(DBTransaction, Order);
    delete Order;
    delete OrderImage;
	return 2;
}

void TDBOrder::CloneOrders(Database::TDBTransaction &DBTransaction,TList *SourceOrders, TList *DestOrders)
{
	try
	{
		for(int i = 0; i < SourceOrders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete *)SourceOrders->Items[i];
			TItemComplete *NewOrder = new TItemComplete();
			Order->Assign(NewOrder);
			NewOrder->OrderKey = 0;
			AssignNewSecurityRef(DBTransaction,NewOrder);

			SetOrder(DBTransaction,NewOrder);

			TDBSecurity::ProcessSecurity(DBTransaction,NewOrder->Security);
			for (int i = 0; i < NewOrder->SubOrders->Count; i++)
			{
				TItemCompleteSub *SubOrder = NewOrder->SubOrders->SubOrderGet(i);
				if(SubOrder)
				{
					TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security);
				}
			}
			DestOrders->Add(NewOrder);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::AssignNewSecurityRef(Database::TDBTransaction &DBTransaction,TItemComplete *Order)
{
	try
	{
		Order->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
		for (int i = 0; i < Order->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
			if(SubOrder)
			{
				SubOrder->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int __fastcall SortByFinalPrice(void *Item1,void *Item2)
{
	TItemMinorComplete *Order1 = (TItemMinorComplete *)Item1;
	TItemMinorComplete *Order2 = (TItemMinorComplete *)Item2;
	if (Order1->BillCalcResult.FinalPrice > Order2->BillCalcResult.FinalPrice)
	{
		return 1;
	}
	else if (Order1->BillCalcResult.FinalPrice == Order2->BillCalcResult.FinalPrice)
	{
		return -0;
	}
	else
	{
		return -1;
	}
}

void TDBOrder::UpdateOrdersForPartialPayment(
Database::TDBTransaction &DBTransaction,
TList * SourceOrders,
TList * PayableOrders,
TItemComplete *SplittedItem,
Currency RequestedTotal)
{
	//Sort orders in descending order
	// check its price with amount paid
	//if it is less than amount paid then add for processing
	//otherwise split item
	try
	{
		SourceOrders->Sort(&SortByFinalPrice);
		for(int i = 0; i < SourceOrders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete *)SourceOrders->Items[i];
			Currency OrderPrice =  RoundToNearest(Order->BillCalcResult.FinalPrice, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
			if(RequestedTotal > 0)
			{
				if(RequestedTotal < OrderPrice)
				{
                    double RequestedTotal_double = double(RequestedTotal.Val);  //MM-6306
                    double OrderPrice_double = double(OrderPrice.Val);  //MM-6306
                    double SplitPercentage = RequestedTotal_double/OrderPrice_double;   //MM-6306 : double data type in place of currency
					Order->Assign(SplittedItem);
					AssignNewSecurityRef(DBTransaction,SplittedItem);
					// Update the Qty.
					// New Order is the reminder left behind.
					double newQuantity = SplittedItem->GetQty() * (1 - SplitPercentage);    //MM-6306 : double data type in place of currency
					SplittedItem->SetQty(newQuantity,false);
					SplittedItem->PatronCount(0, false);
					DeductOrder(Order, SplittedItem, false);
					PayableOrders->Add(Order);
				}
				else
				{
					PayableOrders->Add(Order);
				}
			}
			//Update amound paid by substracting item price
			RequestedTotal-=OrderPrice;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::CloneOrdersSplitPaymentDifference(
Database::TDBTransaction &DBTransaction,
TList * const SourceOrders,
TList * const DestOrders,
const double &SplitPercentage,
const Currency &RequestedTotal)
{
	try
	{
		for(int i = 0; i < SourceOrders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete *)SourceOrders->Items[i];
			TItemComplete *NewOrder = new TItemComplete();
			Order->Assign(NewOrder);
			AssignNewSecurityRef(DBTransaction,NewOrder);
			// Update the Qty.
			// New Order is the reminder left behind.
			NewOrder->SetQty(NewOrder->GetQty() * (1 - SplitPercentage),false);
			NewOrder->PatronCount(0, false);
			DeductOrder(Order, NewOrder, false);
			DestOrders->Add(NewOrder);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::CloneOrdersPartialPaymentDifference(
Database::TDBTransaction &DBTransaction,
TList * const SourceOrders,
TList * const DestOrders,
const double &SplitPercentage,
const Currency &RequestedTotal)
{
	try
	{
		for(int i = 0; i < SourceOrders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete *)SourceOrders->Items[i];
			TItemComplete *NewOrder = new TItemComplete();
			Order->Assign(NewOrder);
			AssignNewSecurityRef(DBTransaction,NewOrder);

			// Update the Qty.
			// New Order is the reminder left behind.
			NewOrder->SetQty(NewOrder->GetQty() * (1 - SplitPercentage),false);
			NewOrder->PatronCount(0, false);
			DeductOrder(Order, NewOrder, false);
			DestOrders->Add(NewOrder);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

bool TDBOrder::DeductOrder(TItemMinorComplete * const Order1,TItemMinorComplete * const Order2, const bool alter_patron_count)
{
	int original_patron_count = Order1->PatronCount();

	if((Order1->TotalPriceAdjustmentSides() - Order2->TotalPriceAdjustmentSides() == 0) && (Order1->GetQty() - Order2->GetQty() == 0))
	{
		return false;
	}

	/* Handy for debugging. */
	const double order_1_qty = Order1->GetQty();    //MM-6306 : double data type in place of currency
	const double order_2_qty = Order2->GetQty();    //MM-6306 : double data type in place of currency
	const double new_quantity = order_1_qty - order_2_qty;  //MM-6306 : double data type in place of currency

	Order1->SetQtyCustom(new_quantity);
	//Order1->Cost -= Order2->Cost;
	Order1->Redeemed -= Order2->Redeemed;

	if (alter_patron_count == false)
	    Order1->PatronCount(original_patron_count, false);
	return true;
}

void TDBOrder::UpdateOrder(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order)
{
	try
	{
		if(Order->OrderKey != 0)
		{
            if(Order->Cost < -1000000 || Order->Cost > 900000000)
                Order->Cost = 0;

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"ORDERS "
			"SET "
			"QTY = :QTY, "
			"PRICE = :PRICE, "
			"DISCOUNT = :DISCOUNT, "
			"REDEEMED = :REDEEMED, "
			"IS_FREE  = :IS_FREE, "
            "DELAYED_PAYMENT_PRICE = :DELAYED_PAYMENT_PRICE, "
			"patron_count = :patron_count, "
			"COST = :COST, "
            "DISCOUNT_WITHOUT_TAX = :DISCOUNT_WITHOUT_TAX "
			"WHERE "
			"ORDER_KEY = :ORDER_KEY";
			IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
			IBInternalQuery->ParamByName("QTY")->AsCurrency = Order->GetQty();
			IBInternalQuery->ParamByName("PRICE")->AsCurrency = Order->PriceEach();
			IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = Order->TotalAdjustment();
			IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = Order->Redeemed;
			IBInternalQuery->ParamByName("COST")->AsCurrency = Order->Cost;
			IBInternalQuery->ParamByName("IS_FREE")->AsString = Order->IsFreebe ? "T" : "F";
			IBInternalQuery->ParamByName("patron_count")->AsCurrency = Order->PatronCount();
            IBInternalQuery->ParamByName("DELAYED_PAYMENT_PRICE")->AsCurrency = Order->BillCalcResult.FinalPrice;
            IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = Order->BillCalcResult.DiscountWithoutTax;
			IBInternalQuery->ExecQuery();

			// Update Discounts.
			//Clear all the Current Orders Discounts.
			DeleteOrderDiscounts(DBTransaction,Order);
			// Reapply them to the DB.
			SetOrderDiscounts(DBTransaction,Order);

			for (int i = 0; i < Order->SubOrders->Count ; i++)
			{
				TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
                SubOrder->RunBillCalculator();

                if(SubOrder->Cost < -1000000 || SubOrder->Cost > 900000000)
                    SubOrder->Cost = 0;

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
				"UPDATE "
				"ORDERS "
				"SET "
				"QTY = :QTY, "
				"PRICE = :PRICE, "
				"DISCOUNT = :DISCOUNT, "
				"REDEEMED = :REDEEMED, "
				"IS_FREE  = :IS_FREE, "
                                "DELAYED_PAYMENT_PRICE = :DELAYED_PAYMENT_PRICE, "
				"PATRON_COUNT = :PATRON_COUNT, "
				"COST = :COST, "
				"ZED_PRICE = :ZED_PRICE, "
                "DISCOUNT_WITHOUT_TAX = :DISCOUNT_WITHOUT_TAX "
				"WHERE "
				"ORDER_KEY = :ORDER_KEY";
				IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = SubOrder->OrderKey;
				IBInternalQuery->ParamByName("QTY")->AsCurrency = SubOrder->GetQty();
				IBInternalQuery->ParamByName("PRICE")->AsCurrency = SubOrder->PriceEach();
				IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = SubOrder->TotalAdjustment();
				IBInternalQuery->ParamByName("REDEEMED")->AsCurrency = SubOrder->Redeemed;
				IBInternalQuery->ParamByName("COST")->AsCurrency = SubOrder->Cost;
				IBInternalQuery->ParamByName("IS_FREE")->AsString = SubOrder->IsFreebe ? "T" : "F";
				IBInternalQuery->ParamByName("PATRON_COUNT")->AsCurrency = SubOrder->PatronCount();
				IBInternalQuery->ParamByName("ZED_PRICE")->AsCurrency = SubOrder->PriceEach_BillCalc();
                IBInternalQuery->ParamByName("DELAYED_PAYMENT_PRICE")->AsCurrency = SubOrder->BillCalcResult.FinalPrice;
                IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = Order->BillCalcResult.DiscountWithoutTax;
				IBInternalQuery->ExecQuery();
				// Update Discounts.
				//Clear all the Current Orders Discounts.
				DeleteOrderDiscounts(DBTransaction,SubOrder);
				// Reapply them to the DB.
				SetOrderDiscounts(DBTransaction,SubOrder);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetOrdersToValuesOfFromTabs(Database::TDBTransaction &DBTransaction,int TabKey, Currency ValueToClear, TList *Orders)
{

	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

	std::set<__int64> OrderLocalKeysToBeRomoved;
	OrderLocalKeysToBeRomoved.clear();
	Currency ItemTotal = 0.0;

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT ORDER_KEY,PRICE,DISCOUNT,REDEEMED "
	"FROM ORDERS WHERE TAB_KEY = :TAB_KEY ORDER BY PRICE";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
	IBInternalQuery->ExecQuery();
	if (IBInternalQuery->RecordCount)
	{
		while(!IBInternalQuery->Eof && ValueToClear != 0)
		{
			ItemTotal = IBInternalQuery->FieldByName("PRICE")->AsCurrency +
			IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency +
			IBInternalQuery->FieldByName("REDEEMED")->AsCurrency;

			if (ValueToClear >= ItemTotal)
			{
				OrderLocalKeysToBeRomoved.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
				ValueToClear = ValueToClear - ItemTotal;
				ItemTotal = 0.0;
			}
			else
			{
				OrderLocalKeysToBeRomoved.insert(SplitOrder(DBTransaction,IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger, ValueToClear));
				ValueToClear = 0.0;
			}
			IBInternalQuery->Next();
		}
	}
	else
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG, "Error Finding Tab in GetOrdersToValuesOfFromTabs. Tab #" + IntToStr(TabKey) +" $ " + CurrToStrF(ValueToClear, ffCurrency, CurrencyDecimals));
	}
	GetOrdersFromOrderKeys(DBTransaction,Orders, OrderLocalKeysToBeRomoved);
}
//---------------------------------------------------------------------------
__int64 TDBOrder::GetNextTransNumber(Database::TDBTransaction &DBTransaction)
{
	int key = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PCINTERNALTRANSNUMBER, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		key = IBInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return key;
}
//------------------------------------------------------------------------------
__int64 TDBOrder::GetNextSetMenuGroupNumber(Database::TDBTransaction &DBTransaction)
{
	int SetMenuGroupNumber = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SETMENUGROUP, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		SetMenuGroupNumber = IBInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return SetMenuGroupNumber;
}
//---------------------------------------------------------------------------
bool TDBOrder::CheckTransferCredit(Database::TDBTransaction &DBTransaction,TList *Orders, int TabDestKey)
{
	bool ReturnVal = true;
	Currency CreditLimit = TDBTab::GetTabCreditLimit(DBTransaction,TabDestKey);
	if(CreditLimit != -1)
	{
		Currency OrdersTotal = 0;
		for (int b=0; b < Orders->Count; b++)
		{
			TItemMinorComplete *Order	= (TItemMinorComplete *)Orders->Items[b];
			// This list contains all the orders on the Tab not jsut the transerred ones.
			if(Order->OrderKey != 0)
			{
				OrdersTotal += Order->TotalPriceAdjustmentSides();
			}
		}

		//TTabs Tabs;
		if((OrdersTotal + TDBTab::GetTabBalance(DBTransaction,TabDestKey)) > CreditLimit)
		{
			ReturnVal = false;
		}
	}
	return ReturnVal;
}
//TDeviceRealTerminal::Instance().User.ContactKey;
void TDBOrder::TransferOrders(Database::TDBTransaction &DBTransaction,TList *Orders, int TabDestKey, int UserKey, int TabSourceKey, bool isTransactionCommit)
{
	try
	{
        bool tabInvoice = false;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        //changes
        TMMContactInfo InvoiceOwnerInfo;
		typedef std::set<__int64> tTabKey;

		typedef std::map<__int64,tTabKey> tSecRefTabKey;
		tSecRefTabKey SecRefTabKey;

		typedef tSecRefTabKey::value_type value_type;
		SecRefTabKey.clear();
		int destTimeKey = GetTimeKey(DBTransaction,TabDestKey);
		if(Orders->Count > 0 && destTimeKey == 0)
		{
			TItemComplete *Item1 = (TItemComplete *)Orders->Items[0];
			destTimeKey = Item1->TimeKey;
		}
		for(int i = 0 ; i < Orders->Count; i++)
		{
			TItemComplete *Item = (TItemComplete *)Orders->Items[i];
			if(Item->OrderKey != 0)
			{
				/* If the main item being transfered IS a side then this should
				also make it a normal item.*/
				int TableNo = 0;
				int SeatNo = 0;
				int RoomNumber = 0;
				UnicodeString TableName = "";
				UnicodeString PartyName = "";
				UnicodeString TabName = TDBTab::GetTabName(DBTransaction,TabDestKey);
				TMMTabType TabType = TDBTab::GetTabType(DBTransaction,TabDestKey);
                TMMTabType SourceTabType = TDBTab::GetTabType(DBTransaction,TabSourceKey);
                if(SourceTabType == 0)
                {
                   SourceTabType = TDBTab::GetTabType(DBTransaction, Item->TabKey);
                   TabSourceKey = Item->TabKey;
                }
                UnicodeString InvoiceName ="";

				switch(TabType)
				{
				case TabTableSeat:
					{
						TTableSeat TableSeat;
						if (TDBTables::GetTableSeat(DBTransaction,TabDestKey,&TableSeat))
						{
							TableNo = TableSeat.TableNo;
							SeatNo = TableSeat.SeatNo;
							TableName = TDBTables::GetTableName(DBTransaction,TableNo);
							PartyName  = TableSeat.PartyName;
						}
					}
					break;
				case TabRoom:
					{
						PartyName = TabName;
					}
					break;
                 case TabInvoice:
                    {
                      int invoiceKey = TDBTab::GetTabInvoice(DBTransaction, TabDestKey);
                      int contactKey = Invoice->GetInvoiceOwner(DBTransaction, invoiceKey);
                      UnicodeString invoice_Number = Invoice->GetInvoiceNumber(DBTransaction, invoiceKey);
                      TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, contactKey, InvoiceOwnerInfo);
                      TabName = InvoiceOwnerInfo.Name + "-" + invoice_Number;
                    }
                    break;
				 case TabStaff:
					{
                       TabName = GetContactName(DBTransaction, TabDestKey);
					}
					break;
				}
                switch(SourceTabType)
				{
				case TabInvoice:
					{
                      int invoiceKey = TDBTab::GetTabInvoice(DBTransaction, TabSourceKey);
                      int contactKey = Invoice->GetInvoiceOwner(DBTransaction, invoiceKey);
                      UnicodeString invoice_Number = Invoice->GetInvoiceNumber(DBTransaction, invoiceKey);
                      TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, contactKey, InvoiceOwnerInfo);
                      tabInvoice = true;
                      InvoiceName = InvoiceOwnerInfo.Name + "-" + invoice_Number;
					}
					break; //
                }
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
				" UPDATE ORDERS "
				" SET "
				" ORDERS.TAB_KEY = :TabDestKey, "
				" ORDERS.TABLE_NUMBER = :TABLE_NUMBER, "
				" ORDERS.SEATNO = :SEATNO, "
				" ORDERS.TABLE_NAME = :TABLE_NAME, "
				" ORDERS.PARTY_NAME = :PARTY_NAME, "
				" ORDERS.TAB_NAME = :TAB_NAME, "
				" ORDERS.TAB_TYPE = :TAB_TYPE, "
				" ORDERS.TIME_KEY = :TIME_KEY, "
				" ORDERS.SIDE_ORDER_KEY = NULL, "
				" ORDERS.SETMENU_MASK = 0, "
				" ORDERS.SETMENU_GROUP = 0 "

//				" ORDERS.ROOM_NO = :ROOM_NO, "
//				" ORDERS.ACC_NO = :ACC_NO, "
//				" ORDERS.FIRST_NAME = :FIRST_NAME, "
//				" ORDERS.LAST_NAME = :LAST_NAME "

				" WHERE "
				" ORDERS.ORDER_KEY = :ORDER_KEY;";
				IBInternalQuery->ParamByName("TabDestKey")->AsInteger = TabDestKey;
				IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Item->OrderKey;
				IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabType;
				IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
				IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = destTimeKey;
				IBInternalQuery->ParamByName("SEATNO")->AsInteger = SeatNo;
				IBInternalQuery->ParamByName("TABLE_NAME")->AsString = TableName == "" ? TabName : TableName;
				IBInternalQuery->ParamByName("PARTY_NAME")->AsString = PartyName;
				IBInternalQuery->ParamByName("TAB_NAME")->AsString = TabName;
//
//                IBInternalQuery->ParamByName("ROOM_NO")->AsInteger = Item->RoomNo;
//				IBInternalQuery->ParamByName("ACC_NO")->AsString = Item->AccNo;
//				IBInternalQuery->ParamByName("FIRST_NAME")->AsString = Item->FirstName;
//				IBInternalQuery->ParamByName("LAST_NAME")->AsString = Item->LastName;

				IBInternalQuery->ExecQuery();

				TSecurityReference *SecRef = new TSecurityReference;
				SecRef->UserKey = UserKey;
				SecRef->Event =   SecurityTypes[secTransferred];
                if(tabInvoice)
                {
                  tabInvoice = false;
                  SecRef->From = InvoiceName;
                }
                else
                {
                  if(SourceTabType == 0 || SourceTabType == 13 || SourceTabType == 1 || SourceTabType == 2)
                  {
                     SecRef->From = Item->TabName;
                  }
                  else
                  {
                     SecRef->From = Item->TabContainerName;
                  }
                }
				SecRef->Note =    "";
                if(TabType == 3)
                {
                  SecRef->To = TableName;
                }
                else
                {
                  SecRef->To = TabName;
                }
				SecRef->TimeStamp = Now();

				Item->Security->SecurityAddUniqueRef(DBTransaction,Item->OrderKey,SecRef, true);
				for(int j = 0 ; j < Item->SubOrders->Count; j++)
				{
					TItemCompleteSub *SubItem = Item->SubOrders->SubOrderGet(j);
					if(SubItem->OrderKey != 0)
					{
						IBInternalQuery->Close();
						IBInternalQuery->SQL->Clear();
						IBInternalQuery->SQL->Text =
						" UPDATE ORDERS "
						" SET ORDERS.TAB_KEY = :TabDestKey, "
						" ORDERS.TAB_TYPE = :TAB_TYPE, "
						" ORDERS.TABLE_NUMBER = :TABLE_NUMBER, "
						" ORDERS.TIME_KEY = :TIME_KEY, "
						" ORDERS.SEATNO = :SEATNO, "
						" ORDERS.TABLE_NAME = :TABLE_NAME, "
						" ORDERS.PARTY_NAME = :PARTY_NAME, "
						" ORDERS.TAB_NAME = :TAB_NAME"
						" WHERE "
						" ORDERS.ORDER_KEY = :ORDER_KEY;";
						IBInternalQuery->ParamByName("TabDestKey")->AsInteger = TabDestKey;
						IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabType;
						IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = SubItem->OrderKey;
						IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNo;
						IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = destTimeKey;
						IBInternalQuery->ParamByName("SEATNO")->AsInteger = SeatNo;
						IBInternalQuery->ParamByName("TABLE_NAME")->AsString = TableName == "" ? TabName : TableName;
						IBInternalQuery->ParamByName("PARTY_NAME")->AsString = PartyName;
						IBInternalQuery->ParamByName("TAB_NAME")->AsString = TabName;
						IBInternalQuery->ExecQuery();

						TSecurityReference *SecRef = new TSecurityReference;
						SecRef->UserKey = UserKey;
						SecRef->Event = SecurityTypes[secTransferred];
                        if(tabInvoice)
                        {
                          tabInvoice = false;
                          SecRef->From = InvoiceName;
                        }
                        else
                        {
                          if(SourceTabType == 0 || SourceTabType == 13 || SourceTabType == 1 || SourceTabType == 2)
                          {
                             SecRef->From = Item->TabName;
                          }
                          else
                          {
                             SecRef->From = Item->TabContainerName;
                          }
                        }
						SecRef->Note =    "";
                        if(TabType == 3)
                        {
                          SecRef->To = TableName;
                        }
                        else
                        {
                          SecRef->To = TabName;
                        }
						SecRef->TimeStamp = Now();
                        SubItem->Security->SecurityAddUniqueRef(DBTransaction,SubItem->OrderKey,SecRef, true);
					}
				}
				TDBSecurity::ProcessSecurity(DBTransaction,Item->Security, isTransactionCommit);
				for (int j = 0; j < Item->SubOrders->Count; j++)
				{
					TItemCompleteSub *SubOrder = Item->SubOrders->SubOrderGet(j);
					if(SubOrder)
					{
						TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security, isTransactionCommit);
					}
				}

				if(TDBTab::GetIsEmpty(DBTransaction,Item->TabKey))
				{
					TMMTabType TabType = TDBTab::GetTabType(DBTransaction,Item->TabKey);
					if (TabType == TabTableSeat)
					{
						TTableSeat TableSeat;
						if (TDBTables::GetTableSeat(DBTransaction,Item->TabKey,&TableSeat))
						{
							if(TDBTables::IsEmpty(DBTransaction,TableSeat.TableNo))
							{
								TDBTables::SetPartyName(DBTransaction,TableSeat.TableNo,"");
							}
						}
						TDBTab::CloseTab(DBTransaction,Item->TabKey);
					}
					else if(TabType == TabRoom)
					{
						int RoomNumber = TDBRooms::GetRoomNumber(DBTransaction,Item->TabKey);
						TDBRooms::SetRoomStatus(DBTransaction,RoomNumber,eVacant);
					}
					else if(TabType == TabNormal)
					{
						TDBTab::CloseTab(DBTransaction,Item->TabKey, isTransactionCommit);
					}
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TDBOrder::CancelOrder(Database::TDBTransaction &DBTransaction,TList *OrderObjects)
{
	try
	{
		for(int i = 0 ; i < OrderObjects->Count ; i++)
		{
			TItemComplete *Order = (TItemComplete *)OrderObjects->Items[i];
			CancelOrder(DBTransaction,Order);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::CancelOrder(Database::TDBTransaction &DBTransaction,TItemComplete * Order)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"UPDATE ORDERS "
		"SET "
		"PRICE = 0, "
        "ZED_PRICE = 0, "
		"DISCOUNT = 0, "
		"DISCOUNT_REASON = '', "
		"ORDER_TYPE = :ORDER_TYPE, "
		"CANCEL_BASE_PRICE = :CANCEL_BASE_PRICE, "
		"CANCEL_GROSS_PRICE = :CANCEL_GROSS_PRICE, "
		"CANCEL_FINAL_PRICE = :CANCEL_FINAL_PRICE, "
		"CANCEL_TOTAL_TAX = :CANCEL_TOTAL_TAX, "
		"CANCEL_TOTAL_DISCOUNT = :CANCEL_TOTAL_DISCOUNT "
		"WHERE "
		"ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsString = Order->OrderKey;
		IBInternalQuery->ParamByName("CANCEL_BASE_PRICE")->AsCurrency = Order->CancelledBillCalcResult.BasePrice;
		IBInternalQuery->ParamByName("CANCEL_GROSS_PRICE")->AsCurrency = Order->CancelledBillCalcResult.GrossPrice;
		IBInternalQuery->ParamByName("CANCEL_FINAL_PRICE")->AsCurrency = Order->CancelledBillCalcResult.FinalPrice;
		IBInternalQuery->ParamByName("CANCEL_TOTAL_TAX")->AsCurrency = Order->CancelledBillCalcResult.TotalTax;
		IBInternalQuery->ParamByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency = Order->CancelledBillCalcResult.TotalDiscount;
		IBInternalQuery->ParamByName("ORDER_TYPE")->AsInteger = CanceledOrder;
		IBInternalQuery->ExecQuery();

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"DELETE FROM ORDERDISCOUNTS "
		"WHERE "
		"ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsString = Order->OrderKey;
		IBInternalQuery->ExecQuery();

		TDBSecurity::ProcessSecurity(DBTransaction,Order->Security);
		for (int i = 0; i < Order->SubOrders->Count; i++)
		{
			TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);
			if(SubOrder)
			{
				TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void __fastcall TDBOrder::ProcessOrders(Database::TDBTransaction &DBTransaction,TList *OrderObjects)
{
	try
	{
		for(int i = 0 ; i < OrderObjects->Count ; i++)
		{
			TItemComplete *Order = (TItemComplete *)OrderObjects->Items[i];
			if (Order->ChitNumber.ChitNumberKey != 0)
			{
				int activeChitKey = TDBActiveChit::GetOrCreateActiveChit(DBTransaction, Order->ChitNumber);
				Order->SetActiveChitNumberKey(activeChitKey);
			}
			ProcessOrder(DBTransaction,Order);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void __fastcall  TDBOrder::ProcessOrder(Database::TDBTransaction &DBTransaction,TItemComplete *Order)
{
	try
	{
		if (Order->OrderType == NormalOrder)
		{
			if (Order->TabType != TabNone)
			{
				if (Order->TabType == TabTableSeat)
				{
					if(TDBTables::Valid(Order->TableNo,Order->SeatNo))
					{
						int SeatKey  = TDBTables::GetOrCreateSeat(DBTransaction,Order->TableNo,Order->SeatNo);
						Order->TabKey = TDBTab::GetOrCreateTab(DBTransaction,TDBTables::GetTabKey(DBTransaction,SeatKey));
						TDBTables::SetSeatTab(DBTransaction,SeatKey,Order->TabKey);
						Order->TabName = TDBTab::GetTabName(DBTransaction,Order->TabKey);
						Order->TabContainerName = TDBTables::GetTableName(DBTransaction,Order->TableNo);
						if(Order->TabName == "")
						{
							Order->TabName = TDBTables::GetNameMinor(DBTransaction,Order->TableNo,Order->SeatNo);
							TDBTab::SetTabName(DBTransaction,Order->TabKey,Order->TabName);
						}
						TDBTab::SetTabType(DBTransaction,Order->TabKey, TabTableSeat);
						Order->PartyName = TDBTables::GetPartyName(DBTransaction,Order->TableNo);
					}
					else
					{
						throw Exception("ProcessOrder Invalid Zone Options");
					}
				}
				else if (Order->TabType == TabRoom)
				{
					Order->TabKey 		= TDBRooms::FindOrCreateRoomTab(DBTransaction,Order->RoomNo);
					Order->PartyName 	= TDBRooms::GetPartyName(DBTransaction,Order->RoomNo);
					Order->TabContainerName = TDBRooms::GetRoomName(DBTransaction,Order->RoomNo);
					Order->TabName = Order->PartyName;
					TDBRooms::SetRoomStatus(DBTransaction,Order->RoomNo,eOccupied);
				}
				else if (Order->TabType == TabWeb)
				{
					Order->WebKey 		= TDBWebUtil::FindOrCreateWebOrder(DBTransaction,Order->WebKey);
					TDBWebUtil::SetOrderGUID(DBTransaction,Order->WebKey,Order->WebID);
					Order->TabKey		= TDBWebUtil::GetTabKeyByWebOrderKey(DBTransaction,Order->WebKey);
					if(Order->TabKey == 0)
					{
						Order->TabKey 	= TDBTab::GetOrCreateTab(DBTransaction,Order->TabKey);
						TDBTab::SetTabType(DBTransaction,Order->TabKey, TabWeb);
						TDBTab::SetTabName(DBTransaction,Order->TabKey, Order->TabName);
						TDBWebUtil::SetTabKey(DBTransaction,Order->WebKey,Order->TabKey);
						TDBTab::SetTabCredit(DBTransaction,Order->TabKey,TDBWebUtil::GetWebOrderPrePaid(DBTransaction,Order->WebKey));
					}
					Order->TabContainerName = TDBWebUtil::GetOrderGUID(DBTransaction,Order->WebKey);
				}
				// Add the order to the DB.
				SetOrder(DBTransaction,Order);
			}
		}
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Err.Message);
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,"Tab Data Type :" + IntToStr(Order->TabType) +
		" Tab Key " + IntToStr(Order->TabKey));
		throw;
	}
}

void TDBOrder::SetOrder(Database::TDBTransaction &DBTransaction,TItemComplete * Order)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT ORDER_KEY FROM ORDERS "
		" WHERE "
		" ITEM_ID = :ITEM_ID AND "
		" SIZE_NAME = :SIZE_NAME AND "
		" TERMINAL_NAME = :TERMINAL_NAME AND "
		" TRANSNO = :TRANSNO";
		IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = Order->Item_ID;
		IBInternalQuery->ParamByName("SIZE_NAME")->AsString = Order->Size;
		IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = Order->Terminal;
		IBInternalQuery->ParamByName("TRANSNO")->AsString = Order->TransNo;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount == 0)
		{
			// Add the required Finical Category.
            int MasterOrderKey = 0;
            if(Order->GetQty() != 0)
            {
			int PrimaryArcCatkey = GetArchiveCategory(DBTransaction,Order->Categories->FinancialCategory);

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			Order->OrderKey = IBInternalQuery->Fields[0]->AsInteger;
            MasterOrderKey = Order->OrderKey;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
			"INSERT INTO ORDERS "
			"( "
			"ORDER_KEY,"
			"TAB_KEY,"
			"ITEM_NAME,"
			"ITEM_ID,"
			"SIZE_NAME,"
			"NOTE,"
			"PARTY_NAME,"
			"TABLE_NUMBER,"
			"TABLE_NAME,"
			"SEATNO,"
			"PRICE,"
			"PRINTED,"
			"TRANSNO,"
			"ORDER_TYPE,"
			"TERMINAL_NAME,"
			"MENU_NAME,"
			"TAB_NAME,"
			"COURSE_NAME,"
			"HAPPYHOUR,"
			"ORDER_LOCATION,"
			"TAB_TYPE,"
			"TIME_STAMP,"
			"COST,"
			"LOYALTY_KEY,"
			"MASTER_CONTAINER,"
			"SETMENU_MASK,"
			"SETMENU_GROUP,"
			"ITEM_CATEGORY,"
			"SECURITY_REF,"
			"TIME_KEY,"
			"GST_PERCENT,"
			"COST_GST_PERCENT,"
			"QTY,"
			"DISCOUNT,"
			"DISCOUNT_REASON,"
			"REDEEMED,"
			"ITEM_KITCHEN_NAME,"
			"SIZE_KITCHEN_NAME,"
			"COURSE_KITCHEN_NAME,"
			"POINTS_PERCENT,"
			"CATEGORY_KEY,"
			"THIRDPARTYCODES_KEY,"
			"MEMBER_FREE_SALE_COUNT,"
			"MEMBER_FREE_SALE_DISCOUNT,"
			"LOCATION_FREE_SALE_COUNT,"
			"LOCATION_FREE_SALE_DISCOUNT,"
			"IS_FREE,"
			"SERVINGCOURSES_KEY,"
			"PRICE_LEVEL0,"
			"PRICE_LEVEL1,"
			"ITEM_TYPE,"
			"MENU_ITEM_KEY,"
			"PLU,"
			"ORDER_TYPE_MESSAGE,"
			"CONTACTS_KEY,"
			"ACTIVECHITNUMBER_KEY, "
			"PATRON_COUNT,"
			"ZED_PRICE,"
			"CANCEL_BASE_PRICE,"
			"CANCEL_GROSS_PRICE,"
			"CANCEL_FINAL_PRICE,"
			"CANCEL_TOTAL_TAX,"
			"CANCEL_TOTAL_DISCOUNT,"
			"BASE_PRICE,"
			"DISCOUNT_WITHOUT_TAX,"
			"TAX_ON_DISCOUNT,"
			"DELAYED_INVOICE_NUMBER,"
			"DELAYED_PAYMENT_PRICE,"
			"MAXRETAILPRICE,"
            "ORDER_IDENTIFICATION_NUMBER, "
            "IS_MANUALLY_ENTERED_WEIGHT, "
            "PRICE_INCL, "
            "PRICE_ADJUST, "
            "ROOM_NO, "
            "ACC_NO, "
            "FIRST_NAME, "
            "LAST_NAME, "
            "ONLINE_CHIT_TYPE, "
            "ORDER_GUID, "
            "SITE_ID, "
            "ORDER_ITEM_ID, "
            "ORDER_ITEM_SIZE_ID, "
            "REFERENCE_ORDER_ITEM_SIZE_ID, "
            "EMAIL, "
            "ONLINE_ORDER_ID "
            ") "
			"VALUES "
			"( "
			":ORDER_KEY,"
			":TAB_KEY,"
			":ITEM_NAME,"
			":ITEM_ID,"
			":SIZE_NAME,"
			":NOTE,"
			":PARTY_NAME,"
			":TABLE_NUMBER,"
			":TABLE_NAME,"
			":SEATNO,"
			":PRICE,"
			":PRINTED,"
			":TRANSNO,"
			":ORDER_TYPE,"
			":TERMINAL_NAME,"
			":MENU_NAME,"
			":TAB_NAME,"
			":COURSE_NAME,"
			":HAPPYHOUR,"
			":ORDER_LOCATION,"
			":TAB_TYPE,"
			":TIME_STAMP,"
			":COST,"
			":LOYALTY_KEY,"
			":MASTER_CONTAINER,"
			":SETMENU_MASK,"
			":SETMENU_GROUP,"
			":ITEM_CATEGORY,"
			":SECURITY_REF,"
			":TIME_KEY,"
			":GST_PERCENT,"
			":COST_GST_PERCENT,"
			":QTY,"
			":DISCOUNT,"
			":DISCOUNT_REASON,"
			":REDEEMED,"
			":ITEM_KITCHEN_NAME,"
			":SIZE_KITCHEN_NAME,"
			":COURSE_KITCHEN_NAME,"
			":POINTS_PERCENT,"
			":CATEGORY_KEY,"
			":THIRDPARTYCODES_KEY,"
			":MEMBER_FREE_SALE_COUNT,"
			":MEMBER_FREE_SALE_DISCOUNT,"
			":LOCATION_FREE_SALE_COUNT,"
			":LOCATION_FREE_SALE_DISCOUNT,"
			":IS_FREE,"
			":SERVINGCOURSES_KEY,"
			":PRICE_LEVEL0,"
			":PRICE_LEVEL1,"
			":ITEM_TYPE,"
			":MENU_ITEM_KEY,"
			":PLU,"
			":ORDER_TYPE_MESSAGE,"
			":CONTACTS_KEY,"
			":ACTIVECHITNUMBER_KEY, "
			":PATRON_COUNT,"
			":ZED_PRICE,"
			":CANCEL_BASE_PRICE,"
			":CANCEL_GROSS_PRICE,"
			":CANCEL_FINAL_PRICE,"
			":CANCEL_TOTAL_TAX,"
			":CANCEL_TOTAL_DISCOUNT,"
			":BASE_PRICE,"
			":DISCOUNT_WITHOUT_TAX,"
			":TAX_ON_DISCOUNT,"
			":DELAYED_INVOICE_NUMBER,"
			":DELAYED_PAYMENT_PRICE,"
			":MAXRETAILPRICE,"
            ":ORDER_IDENTIFICATION_NUMBER, "
            ":IS_MANUALLY_ENTERED_WEIGHT, "
            ":PRICE_INCL, "
            ":PRICE_ADJUST, "
            ":ROOM_NO, "
            ":ACC_NO, "
            ":FIRST_NAME, "
            ":LAST_NAME, "
            ":ONLINE_CHIT_TYPE, "
            ":ORDER_GUID, "
            ":SITE_ID, "
            ":ORDER_ITEM_ID, "
            ":ORDER_ITEM_SIZE_ID, "
            ":REFERENCE_ORDER_ITEM_SIZE_ID, "
            ":EMAIL, "
            ":ONLINE_ORDER_ID "
            ");";

			IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
            IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = Order->OrderIdentificationNo;
			IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = Order->TabKey;
			IBInternalQuery->ParamByName("ITEM_NAME")->AsString = Order->Item;
			IBInternalQuery->ParamByName("ITEM_ID")->AsInteger = Order->Item_ID;
			IBInternalQuery->ParamByName("SIZE_NAME")->AsString = Order->Size;
			IBInternalQuery->ParamByName("NOTE")->AsString = Order->Note.SubString(1,78);
			IBInternalQuery->ParamByName("PARTY_NAME")->AsString = Order->PartyName;
			switch(Order->TabType)
			{
			case TabNormal :
			case TabStaff :
			case TabMember :
			case TabHandheldCashAccount :
			case TabCashAccount :
			case TabNone :
			case TabDelayedPayment :
			case TabTableSeat :
			IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
			IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
				break;
			case TabRoom :
				IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->RoomNo;
				IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
			case TabParkedSale :
				IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
				IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
				break;
            default :
                 IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
                 IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
                 break;
			}
			IBInternalQuery->ParamByName("SEATNO")->AsInteger = Order->SeatNo;
			IBInternalQuery->ParamByName("PRICE")->AsCurrency = Order->PriceEach();
            IBInternalQuery->ParamByName("MAXRETAILPRICE")->AsCurrency = Order->MaxRetailPrice;
            IBInternalQuery->ParamByName("PRINTED")->AsString = "F";
            IBInternalQuery->ParamByName("TRANSNO")->AsString = Order->TransNo;
			IBInternalQuery->ParamByName("ORDER_TYPE")->AsInteger = Order->OrderType;
			IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = Order->Terminal;
			IBInternalQuery->ParamByName("MENU_NAME")->AsString = Order->MenuName;
			IBInternalQuery->ParamByName("TAB_NAME")->AsString =  Order->TabName.SubString(1,80);
			IBInternalQuery->ParamByName("COURSE_NAME")->AsString = Order->Course;
			IBInternalQuery->ParamByName("HAPPYHOUR")->AsString 	= Order->HappyHour?"T":"F";
			IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = Order->OrderedLocation;
			IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = Order->TabType;
			IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Order->TimeStamp;

            if(Order->Cost < -1000000 || Order->Cost > 900000000)
                Order->Cost = 0;

			IBInternalQuery->ParamByName("COST")->AsCurrency = Order->Cost;
			IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = Order->Loyalty_Key;
			IBInternalQuery->ParamByName("MASTER_CONTAINER")->AsString = Order->MasterContainer;
			IBInternalQuery->ParamByName("SETMENU_MASK")->AsInteger = Order->SetMenuMask;
			IBInternalQuery->ParamByName("SETMENU_GROUP")->AsInteger = Order->SetMenuGroup;
			IBInternalQuery->ParamByName("ITEM_CATEGORY")->AsString = "";
			IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Order->Security->GetSecurityRefNumber();
			IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = Order->TimeKey;
			//IBInternalQuery->ParamByName("GST_PERCENT")->AsFloat = Order->GSTPercent;
			IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsFloat = Order->CostGSTPercent;
			//IBInternalQuery->ParamByName("SIDE_ORDER_KEY")->AsInteger = 0;
			IBInternalQuery->ParamByName("QTY")->AsCurrency = Order->GetQty();
			IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = Order->TotalAdjustment();
			IBInternalQuery->ParamByName("DISCOUNT_REASON")->AsString = Order->DiscountReason.SubString(1,40);
			IBInternalQuery->ParamByName("REDEEMED")->AsFloat = 0;

			IBInternalQuery->ParamByName("ITEM_KITCHEN_NAME")->AsString = Order->ItemKitchenName;
			IBInternalQuery->ParamByName("SIZE_KITCHEN_NAME")->AsString = Order->SizeKitchenName;
			IBInternalQuery->ParamByName("COURSE_KITCHEN_NAME")->AsString = Order->CourseKitchenName;

			IBInternalQuery->ParamByName("POINTS_PERCENT")->AsFloat = Order->PointsPercent;
			IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = PrimaryArcCatkey;
			IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = Order->ThirdPartyKey;
			IBInternalQuery->ParamByName("MEMBER_FREE_SALE_COUNT")->AsInteger = Order->MemberFreeSaleCount;
			IBInternalQuery->ParamByName("MEMBER_FREE_SALE_DISCOUNT")->AsFloat = Order->MemberFreeSaleDiscount;
			IBInternalQuery->ParamByName("LOCATION_FREE_SALE_COUNT")->AsInteger = Order->LocationFreeSaleCount;
			IBInternalQuery->ParamByName("LOCATION_FREE_SALE_DISCOUNT")->AsFloat = Order->LocationFreeSaleDiscount;
			IBInternalQuery->ParamByName("IS_FREE")->AsString = Order->IsFreebe ? "T" : "F";
			IBInternalQuery->ParamByName("PRICE_LEVEL0")->AsCurrency = Order->PriceLevel0;
			IBInternalQuery->ParamByName("PRICE_LEVEL1")->AsCurrency = Order->PriceLevel1;
			IBInternalQuery->ParamByName("ITEM_TYPE")->AsInteger = Order->ItemType;
			IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = Order->ServingCourse.ServingCourseKey;
			IBInternalQuery->ParamByName("PATRON_COUNT")->AsCurrency = Order->PatronCount();
			IBInternalQuery->ParamByName("ZED_PRICE")->AsCurrency = Order->PriceEach_BillCalc();  // store the value for a single item for zed. it will be multiplied by qty to produce the final amount
			IBInternalQuery->ParamByName("CANCEL_BASE_PRICE")->AsCurrency = Order->CancelledBillCalcResult.BasePrice;
			IBInternalQuery->ParamByName("CANCEL_GROSS_PRICE")->AsCurrency = Order->CancelledBillCalcResult.GrossPrice;
			IBInternalQuery->ParamByName("CANCEL_FINAL_PRICE")->AsCurrency = Order->CancelledBillCalcResult.FinalPrice;
			IBInternalQuery->ParamByName("CANCEL_TOTAL_TAX")->AsCurrency = Order->CancelledBillCalcResult.TotalTax;
			IBInternalQuery->ParamByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency = Order->CancelledBillCalcResult.TotalDiscount;
            IBInternalQuery->ParamByName("BASE_PRICE")->AsCurrency = Order->BillCalcResult.BasePrice ;
			IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = Order->BillCalcResult.DiscountWithoutTax;
			IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency = Order->BillCalcResult.TaxOnDiscount;
            IBInternalQuery->ParamByName("DELAYED_INVOICE_NUMBER")->AsString = Order->DelayedInvoiceNumber;
            IBInternalQuery->ParamByName("DELAYED_PAYMENT_PRICE")->AsCurrency = Order->BillCalcResult.FinalPrice;
            IBInternalQuery->ParamByName("IS_MANUALLY_ENTERED_WEIGHT")->AsString = Order->isManuallyEnteredWeight? "T" : "F";
            IBInternalQuery->ParamByName("PRICE_INCL")->AsCurrency = Order->BillCalcResult.PriceIncl;
            IBInternalQuery->ParamByName("PRICE_ADJUST")->AsCurrency = Order->PriceLevelCustom;
            IBInternalQuery->ParamByName("ROOM_NO")->AsString =  Order->RoomNoStr;
            if(Order->AccNo !="0")
                IBInternalQuery->ParamByName("ACC_NO")->AsString =  Order->AccNo;
            else
                IBInternalQuery->ParamByName("ACC_NO")->AsString =  "";
            IBInternalQuery->ParamByName("FIRST_NAME")->AsString =  Order->FirstName;
            IBInternalQuery->ParamByName("LAST_NAME")->AsString =  Order->LastName;

			if(Order->ItemOrderedFrom != NULL)
			{
				IBInternalQuery->ParamByName("MENU_ITEM_KEY")->AsInteger = Order->ItemOrderedFrom->ItemKey;
			}
			else if(Order->OriginalItemKey != NULL)
			{
				IBInternalQuery->ParamByName("MENU_ITEM_KEY")->AsInteger = Order->OriginalItemKey;
			}
			else
			{
				IBInternalQuery->ParamByName("MENU_ITEM_KEY")->Clear();
			}
			IBInternalQuery->ParamByName("PLU")->AsInteger = Order->PLU;
			IBInternalQuery->ParamByName("ORDER_TYPE_MESSAGE")->AsString = Order->OrderTypeMessage;
			IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = Order->ContactsKey;
			if(Order->GetActiveChitNumberKey() == 0)
			{
				IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY")->Clear();
			}
			else
			{
				IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY")->AsInteger = Order->GetActiveChitNumberKey();
			}


            IBInternalQuery->ParamByName("ONLINE_CHIT_TYPE")->AsInteger = Order->OnlineChitType;
            IBInternalQuery->ParamByName("ORDER_GUID")->AsString = Order->OrderGuid;
            IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Order->SiteId;
            IBInternalQuery->ParamByName("ORDER_ITEM_ID")->AsInteger = Order->OrderItemId;
            IBInternalQuery->ParamByName("ORDER_ITEM_SIZE_ID")->AsInteger = Order->OrderItemSizeId;
            IBInternalQuery->ParamByName("REFERENCE_ORDER_ITEM_SIZE_ID")->AsInteger = Order->ReferenceOrderItemSizeId;
            IBInternalQuery->ParamByName("EMAIL")->AsString = Order->Email;
            IBInternalQuery->ParamByName("ONLINE_ORDER_ID")->AsInteger = Order->OnlineOrderId;

			IBInternalQuery->ExecQuery();

			// Options
			for (int s = 0 ; s < Order->OptionsSelected->Count ; s ++)
			{
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_OPTION, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int OptionKey = IBInternalQuery->Fields[0]->AsInteger;

				TItemOption *CurrentOption = Order->OptionsSelected->OptionGet(s);
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Clear();
				IBInternalQuery->SQL->Text =
				"INSERT INTO ORDEROPTION ("
				"ORDEROPTION_KEY,"
				"ORDER_KEY,"
				"OPTION_NAME,"
				"OPTION_ID,"
				"PLUS,"
				"OPTION_KITCHEN_NAME) "
				"VALUES ("
				":ORDEROPTION_KEY,"
				":ORDER_KEY,"
				":OPTION_NAME,"
				":OPTION_ID,"
				":PLUS,"
				":OPTION_KITCHEN_NAME);";
				IBInternalQuery->ParamByName("ORDEROPTION_KEY")->AsInteger  =  OptionKey;
				IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger  =  Order->OrderKey;
				IBInternalQuery->ParamByName("OPTION_NAME")->AsString =  CurrentOption->Name;
				IBInternalQuery->ParamByName("OPTION_ID")->AsInteger  =  CurrentOption->OptionID;
				IBInternalQuery->ParamByName("PLUS")->AsString       =  CurrentOption->IsPlus ? "T" : "F";
				IBInternalQuery->ParamByName("OPTION_KITCHEN_NAME")->AsString =  CurrentOption->KitchenName;
				IBInternalQuery->ExecQuery();
			}

			// Catagories
			for (int q = 0 ; q < Order->Categories->Count ; q ++)
			{
				TItemSizeCategory *CurrentCat = Order->Categories->CategoryGet(q);
				CurrentCat->CategoryKey = GetArchiveCategory(DBTransaction,CurrentCat->Category);

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
				"INSERT INTO ORDERCATEGORY ("
				"ORDER_KEY,"
				"CATEGORY_KEY) "
				"VALUES ("
				":ORDER_KEY,"
				":CATEGORY_KEY);";
				IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
				IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CurrentCat->CategoryKey;
				IBInternalQuery->ExecQuery();
			}

			// Receipies
			for (int s = 0 ; s < Order->SalesRecipesToApply->Count ; s ++)
			{
				TItemRecipe *CurrentRecipe = Order->SalesRecipesToApply->RecipeGet(s);
				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERRECIPE, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();
				int ReceipeKey = IBInternalQuery->Fields[0]->AsInteger;
                if(CurrentRecipe->Cost < -1000000)
                    CurrentRecipe->Cost = 0;

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
				"INSERT INTO ORDERRECIPE ("
				"ORDERRECIPE_KEY,"
				"ORDER_KEY,"
				"STOCK_CODE,"
				"QTY,"
				"COST,"
				"STOCK_LOCATION) "
				"VALUES ("
				":ORDERRECIPE_KEY,"
				":ORDER_KEY,"
				":STOCK_CODE,"
				":QTY,"
				":COST,"
				":STOCK_LOCATION);";
				IBInternalQuery->ParamByName("ORDERRECIPE_KEY")->AsInteger = ReceipeKey;
				IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
				IBInternalQuery->ParamByName("STOCK_CODE")->AsString =  CurrentRecipe->StockCode;
				IBInternalQuery->ParamByName("QTY")->AsFloat         =  CurrentRecipe->Qty;
				IBInternalQuery->ParamByName("COST")->AsCurrency     =  CurrentRecipe->Cost;
				IBInternalQuery->ParamByName("STOCK_LOCATION")->AsString	=  CurrentRecipe->StockLocation;
				IBInternalQuery->ExecQuery();

			}

			// Discounts
			SetOrderDiscounts(DBTransaction,Order);

			// Taxes
			SetOrderTaxProfiles(DBTransaction,Order);
            }
			// Sides
			for (int i = 0 ; i < Order->SubOrders->Count ; i ++)
			{
				TItemCompleteSub *CurrentSubOrder = (TItemCompleteSub *)Order->SubOrders->Items[i];
                if(CurrentSubOrder->GetQty() != 0)
                {
				int SubPrimaryArcCatkey = GetArchiveCategory(DBTransaction,CurrentSubOrder->Categories->FinancialCategory);

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERS, 1) FROM RDB$DATABASE";
				IBInternalQuery->ExecQuery();

				CurrentSubOrder->OrderKey = IBInternalQuery->Fields[0]->AsInteger;
				CurrentSubOrder->MasterOrderKey = MasterOrderKey;

				IBInternalQuery->Close();
				IBInternalQuery->SQL->Text =
				"INSERT INTO ORDERS "
				"( "
				"ORDER_KEY,"
				"TAB_KEY,"
				"ITEM_NAME,"
				"ITEM_ID,"
				"SIZE_NAME,"
				"NOTE,"
				"PARTY_NAME,"
				"TABLE_NUMBER,"
				"TABLE_NAME,"
				"SEATNO,"
				"PRICE,"
				"PRINTED,"
				"TRANSNO,"
				"ORDER_TYPE,"
				"TERMINAL_NAME,"
				"MENU_NAME,"
				"TAB_NAME,"
				"COURSE_NAME,"
				"HAPPYHOUR,"
				"ORDER_LOCATION,"
				"TAB_TYPE,"
				"TIME_STAMP,"
				"COST,"
				"LOYALTY_KEY,"
				"MASTER_CONTAINER,"
				"SETMENU_MASK,"
				"SETMENU_GROUP,"
				"ITEM_CATEGORY,"
				"SECURITY_REF,"
				"TIME_KEY,"
				"GST_PERCENT,"
				"COST_GST_PERCENT,"
				"SIDE_ORDER_KEY, "
				"QTY,"
				"DISCOUNT,"
				"DISCOUNT_REASON,"
				"REDEEMED,"
				"ITEM_KITCHEN_NAME,"
				"SIZE_KITCHEN_NAME,"
				"COURSE_KITCHEN_NAME,"
				"POINTS_PERCENT,"
				"CATEGORY_KEY,"
				"THIRDPARTYCODES_KEY,"
				"MEMBER_FREE_SALE_COUNT,"
				"MEMBER_FREE_SALE_DISCOUNT,"
				"LOCATION_FREE_SALE_COUNT,"
				"LOCATION_FREE_SALE_DISCOUNT,"
				"IS_FREE,"
				"SERVINGCOURSES_KEY,"
				"PRICE_LEVEL0,"
				"PRICE_LEVEL1,"
				"ITEM_TYPE,"
				"MENU_ITEM_KEY,"
				"PLU,"
				"ORDER_TYPE_MESSAGE,"
				"CONTACTS_KEY,"
				"ACTIVECHITNUMBER_KEY, "
				"PATRON_COUNT,"
				"ZED_PRICE,"
				"CANCEL_BASE_PRICE,"
				"CANCEL_GROSS_PRICE,"
				"CANCEL_FINAL_PRICE,"
				"CANCEL_TOTAL_TAX,"
				"CANCEL_TOTAL_DISCOUNT,"
				"BASE_PRICE,"
				"DISCOUNT_WITHOUT_TAX,"
				"TAX_ON_DISCOUNT,"
                "DELAYED_INVOICE_NUMBER,"
                "DELAYED_PAYMENT_PRICE,"
                "ORDER_IDENTIFICATION_NUMBER, "
                "IS_MANUALLY_ENTERED_WEIGHT, "
                "PRICE_INCL, "
                "PRICE_ADJUST, "
                "ROOM_NO, "
                "ACC_NO, "
                "FIRST_NAME, "
                "LAST_NAME, "
               "ONLINE_CHIT_TYPE, "
                "ORDER_GUID, "
                "SITE_ID, "
                "ORDER_ITEM_ID, "
                "ORDER_ITEM_SIZE_ID, "
                "REFERENCE_ORDER_ITEM_SIZE_ID, "
                "EMAIL, "
                "ONLINE_ORDER_ID "
		   	") "
				"VALUES "
				"( "
				":ORDER_KEY,"
				":TAB_KEY,"
				":ITEM_NAME,"
				":ITEM_ID,"
				":SIZE_NAME,"
				":NOTE,"
				":PARTY_NAME,"
				":TABLE_NUMBER,"
				":TABLE_NAME,"
				":SEATNO,"
				":PRICE,"
				":PRINTED,"
				":TRANSNO,"
				":ORDER_TYPE,"
				":TERMINAL_NAME,"
				":MENU_NAME,"
				":TAB_NAME,"
				":COURSE_NAME,"
				":HAPPYHOUR,"
				":ORDER_LOCATION,"
				":TAB_TYPE,"
				":TIME_STAMP,"
				":COST,"
				":LOYALTY_KEY,"
				":MASTER_CONTAINER,"
				":SETMENU_MASK,"
				":SETMENU_GROUP,"
				":ITEM_CATEGORY,"
				":SECURITY_REF,"
				":TIME_KEY,"
				":GST_PERCENT,"
				":COST_GST_PERCENT,"
				":SIDE_ORDER_KEY, "
				":QTY,"
				":DISCOUNT,"
				":DISCOUNT_REASON,"
				":REDEEMED,"
				":ITEM_KITCHEN_NAME,"
				":SIZE_KITCHEN_NAME,"
				":COURSE_KITCHEN_NAME,"
				":POINTS_PERCENT,"
				":CATEGORY_KEY,"
				":THIRDPARTYCODES_KEY,"
				":MEMBER_FREE_SALE_COUNT,"
				":MEMBER_FREE_SALE_DISCOUNT,"
				":LOCATION_FREE_SALE_COUNT,"
				":LOCATION_FREE_SALE_DISCOUNT,"
				":IS_FREE,"
				":SERVINGCOURSES_KEY,"
				":PRICE_LEVEL0,"
				":PRICE_LEVEL1,"
				":ITEM_TYPE,"
				":MENU_ITEM_KEY,"
				":PLU,"
				":ORDER_TYPE_MESSAGE,"
				":CONTACTS_KEY,"
				":ACTIVECHITNUMBER_KEY, "
				":PATRON_COUNT,"
				":ZED_PRICE,"
				":CANCEL_BASE_PRICE,"
				":CANCEL_GROSS_PRICE,"
				":CANCEL_FINAL_PRICE,"
				":CANCEL_TOTAL_TAX,"
				":CANCEL_TOTAL_DISCOUNT,"
				":BASE_PRICE,"
				":DISCOUNT_WITHOUT_TAX,"
				":TAX_ON_DISCOUNT,"
                ":DELAYED_INVOICE_NUMBER,"
                ":DELAYED_PAYMENT_PRICE,"
                ":ORDER_IDENTIFICATION_NUMBER, "
                ":IS_MANUALLY_ENTERED_WEIGHT, "
                ":PRICE_INCL,"
                ":PRICE_ADJUST, "
                ":ROOM_NO, "
                ":ACC_NO, "
                ":FIRST_NAME, "
                ":LAST_NAME, "
                ":ONLINE_CHIT_TYPE, "
                ":ORDER_GUID, "
                ":SITE_ID, "
                ":ORDER_ITEM_ID, "
                ":ORDER_ITEM_SIZE_ID, "
                ":REFERENCE_ORDER_ITEM_SIZE_ID, "
                ":EMAIL, "
                ":ONLINE_ORDER_ID "
            	");";

				IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = CurrentSubOrder->OrderKey;
                IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = CurrentSubOrder->OrderIdentificationNo;
				IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = Order->TabKey;
				IBInternalQuery->ParamByName("ITEM_NAME")->AsString = CurrentSubOrder->Item;
				IBInternalQuery->ParamByName("ITEM_ID")->AsString = CurrentSubOrder->Item_ID;
				IBInternalQuery->ParamByName("SIZE_NAME")->AsString = CurrentSubOrder->Size;
				IBInternalQuery->ParamByName("NOTE")->AsString = "";
				IBInternalQuery->ParamByName("PARTY_NAME")->AsString = Order->PartyName;
				switch(Order->TabType)
				{
				case TabNormal :
				case TabStaff :
				case TabMember :
				case TabHandheldCashAccount :
				case TabCashAccount :
				case TabNone :
                                case TabDelayedPayment :
				case TabTableSeat :
					IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
					IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
					break;
				case TabRoom :
					IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->RoomNo;
					IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
					break;
				case TabParkedSale :
					IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = Order->TableNo;
					IBInternalQuery->ParamByName("TABLE_NAME")->AsString = Order->TabContainerName.SubString(1,25);
					break;
				}
				IBInternalQuery->ParamByName("SEATNO")->AsInteger = Order->SeatNo;
				IBInternalQuery->ParamByName("PRICE")->AsCurrency = CurrentSubOrder->PriceEach();
				IBInternalQuery->ParamByName("PRINTED")->AsString = "F";
				IBInternalQuery->ParamByName("TRANSNO")->AsString = Order->TransNo;
				IBInternalQuery->ParamByName("ORDER_TYPE")->AsInteger = Order->OrderType;
				IBInternalQuery->ParamByName("TERMINAL_NAME")->AsString = Order->Terminal;
				IBInternalQuery->ParamByName("MENU_NAME")->AsString = Order->MenuName;
				IBInternalQuery->ParamByName("TAB_NAME")->AsString =  Order->TabName;
				IBInternalQuery->ParamByName("COURSE_NAME")->AsString = CurrentSubOrder->Course;
				IBInternalQuery->ParamByName("HAPPYHOUR")->AsString 	= Order->HappyHour?"T":"F";
				IBInternalQuery->ParamByName("ORDER_LOCATION")->AsString = Order->OrderedLocation;
				IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = Order->TabType;
				IBInternalQuery->ParamByName("TIME_STAMP")->AsDateTime = Order->TimeStamp;
                if(CurrentSubOrder->Cost < -1000000 || CurrentSubOrder->Cost > 900000000)
                    CurrentSubOrder->Cost = 0;
				IBInternalQuery->ParamByName("COST")->AsCurrency = CurrentSubOrder->Cost;
				IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = CurrentSubOrder->Loyalty_Key;
				IBInternalQuery->ParamByName("MASTER_CONTAINER")->AsString = Order->MasterContainer;
				IBInternalQuery->ParamByName("SETMENU_MASK")->AsInteger = Order->SetMenuMask;
				IBInternalQuery->ParamByName("SETMENU_GROUP")->AsInteger = Order->SetMenuGroup;
				IBInternalQuery->ParamByName("ITEM_CATEGORY")->AsString = "";
				IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = CurrentSubOrder->Security->GetSecurityRefNumber();
				IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = Order->TimeKey;
				//IBInternalQuery->ParamByName("GST_PERCENT")->AsFloat = CurrentSubOrder->GSTPercent;
				IBInternalQuery->ParamByName("COST_GST_PERCENT")->AsFloat = CurrentSubOrder->CostGSTPercent;
				IBInternalQuery->ParamByName("SIDE_ORDER_KEY")->AsInteger = CurrentSubOrder->MasterOrderKey;
				IBInternalQuery->ParamByName("QTY")->AsCurrency = CurrentSubOrder->GetQty();
				IBInternalQuery->ParamByName("DISCOUNT")->AsCurrency = CurrentSubOrder->TotalAdjustment();
				IBInternalQuery->ParamByName("DISCOUNT_REASON")->AsString = CurrentSubOrder->DiscountReason.SubString(1,40);
				IBInternalQuery->ParamByName("REDEEMED")->AsFloat = 0;
				IBInternalQuery->ParamByName("POINTS_PERCENT")->AsFloat = CurrentSubOrder->PointsPercent;

		 		IBInternalQuery->ParamByName("ITEM_KITCHEN_NAME")->AsString = CurrentSubOrder->ItemKitchenName;
				IBInternalQuery->ParamByName("SIZE_KITCHEN_NAME")->AsString = CurrentSubOrder->SizeKitchenName;
				IBInternalQuery->ParamByName("COURSE_KITCHEN_NAME")->AsString = CurrentSubOrder->CourseKitchenName;

				IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = SubPrimaryArcCatkey;
				IBInternalQuery->ParamByName("THIRDPARTYCODES_KEY")->AsInteger = CurrentSubOrder->ThirdPartyKey;
				IBInternalQuery->ParamByName("MEMBER_FREE_SALE_COUNT")->AsInteger = CurrentSubOrder->MemberFreeSaleCount;
				IBInternalQuery->ParamByName("MEMBER_FREE_SALE_DISCOUNT")->AsFloat = CurrentSubOrder->MemberFreeSaleDiscount;
				IBInternalQuery->ParamByName("LOCATION_FREE_SALE_COUNT")->AsInteger = CurrentSubOrder->LocationFreeSaleCount;
				IBInternalQuery->ParamByName("LOCATION_FREE_SALE_DISCOUNT")->AsFloat = CurrentSubOrder->LocationFreeSaleDiscount;
				IBInternalQuery->ParamByName("IS_FREE")->AsString = CurrentSubOrder->IsFreebe ? "T" : "F";
				IBInternalQuery->ParamByName("PRICE_LEVEL0")->AsCurrency = CurrentSubOrder->PriceLevel0;
				IBInternalQuery->ParamByName("PRICE_LEVEL1")->AsCurrency = CurrentSubOrder->PriceLevel1;
                IBInternalQuery->ParamByName("BASE_PRICE")->AsCurrency = CurrentSubOrder->BillCalcResult.BasePrice;
				IBInternalQuery->ParamByName("DISCOUNT_WITHOUT_TAX")->AsCurrency = CurrentSubOrder->BillCalcResult.DiscountWithoutTax;
				IBInternalQuery->ParamByName("TAX_ON_DISCOUNT")->AsCurrency = CurrentSubOrder->BillCalcResult.TaxOnDiscount;
				IBInternalQuery->ParamByName("ITEM_TYPE")->AsInteger = CurrentSubOrder->SubItemType;
				IBInternalQuery->ParamByName("SERVINGCOURSES_KEY")->AsInteger = CurrentSubOrder->ServingCourse.ServingCourseKey;
				IBInternalQuery->ParamByName("MENU_ITEM_KEY")->Clear();
				IBInternalQuery->ParamByName("PLU")->AsInteger = CurrentSubOrder->PLU;
				if(Order->GetActiveChitNumberKey() == 0)
				{
					IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY")->Clear();
				}
				else
				{
					IBInternalQuery->ParamByName("ACTIVECHITNUMBER_KEY")->AsInteger = Order->GetActiveChitNumberKey();
				}
				IBInternalQuery->ParamByName("PATRON_COUNT")->AsCurrency = CurrentSubOrder->PatronCount();
				IBInternalQuery->ParamByName("ZED_PRICE")->AsCurrency = CurrentSubOrder->PriceEach_BillCalc();
				IBInternalQuery->ParamByName("CANCEL_BASE_PRICE")->AsCurrency = Order->CancelledBillCalcResult.BasePrice;
				IBInternalQuery->ParamByName("CANCEL_GROSS_PRICE")->AsCurrency = Order->CancelledBillCalcResult.GrossPrice;
				IBInternalQuery->ParamByName("CANCEL_FINAL_PRICE")->AsCurrency = Order->CancelledBillCalcResult.FinalPrice;
				IBInternalQuery->ParamByName("CANCEL_TOTAL_TAX")->AsCurrency = Order->CancelledBillCalcResult.TotalTax;
				IBInternalQuery->ParamByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency = Order->CancelledBillCalcResult.TotalDiscount;
                IBInternalQuery->ParamByName("DELAYED_INVOICE_NUMBER")->AsString = Order->DelayedInvoiceNumber;
                IBInternalQuery->ParamByName("DELAYED_PAYMENT_PRICE")->AsCurrency = Order->BillCalcResult.FinalPrice;
                IBInternalQuery->ParamByName("IS_MANUALLY_ENTERED_WEIGHT")->AsString = Order->isManuallyEnteredWeight? "T" : "F";
                IBInternalQuery->ParamByName("PRICE_INCL")->AsCurrency = CurrentSubOrder->BillCalcResult.PriceIncl;
                IBInternalQuery->ParamByName("PRICE_ADJUST")->AsCurrency = CurrentSubOrder->PriceLevelCustom;
                IBInternalQuery->ParamByName("ROOM_NO")->AsString =  Order->RoomNoStr;
                if(Order->AccNo !="0")
                    IBInternalQuery->ParamByName("ACC_NO")->AsString =  Order->AccNo;
                else
                    IBInternalQuery->ParamByName("ACC_NO")->AsString =  "";
                IBInternalQuery->ParamByName("FIRST_NAME")->AsString =  Order->FirstName;
                IBInternalQuery->ParamByName("LAST_NAME")->AsString =  Order->LastName;
                IBInternalQuery->ParamByName("ONLINE_CHIT_TYPE")->AsInteger = Order->OnlineChitType;
                IBInternalQuery->ParamByName("ORDER_GUID")->AsString = Order->OrderGuid;
                IBInternalQuery->ParamByName("SITE_ID")->AsInteger = Order->SiteId;
                IBInternalQuery->ParamByName("ORDER_ITEM_ID")->AsInteger = Order->OrderItemId;
                IBInternalQuery->ParamByName("ORDER_ITEM_SIZE_ID")->AsInteger = Order->OrderItemSizeId;
                IBInternalQuery->ParamByName("REFERENCE_ORDER_ITEM_SIZE_ID")->AsInteger = Order->ReferenceOrderItemSizeId;
                IBInternalQuery->ParamByName("EMAIL")->AsString = Order->Email;
                IBInternalQuery->ParamByName("ONLINE_ORDER_ID")->AsInteger = Order->OnlineOrderId;

				IBInternalQuery->ExecQuery();

				for (int w = 0 ; w < CurrentSubOrder->Categories->Count ; w ++)
				{
					TItemSizeCategory *CurrentCat = CurrentSubOrder->Categories->CategoryGet(w);
					CurrentCat->CategoryKey = GetArchiveCategory(DBTransaction,CurrentCat->Category);

					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text =
					"INSERT INTO ORDERCATEGORY "
					"("
					"ORDER_KEY,"
					"CATEGORY_KEY"
					")"
					" VALUES "
					"("
					":ORDER_KEY,"
					":CATEGORY_KEY"
					");";
					IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = CurrentSubOrder->OrderKey;
					IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CurrentCat->CategoryKey;
					IBInternalQuery->ExecQuery();
				}

				// Discounts
				SetOrderDiscounts(DBTransaction,CurrentSubOrder);

				// Taxes
				SetOrderTaxProfiles(DBTransaction, CurrentSubOrder);

				for (int s = 0 ; s < CurrentSubOrder->SalesRecipesToApply->Count ; s ++)
				{
					TItemRecipe *CurrentRecipe = CurrentSubOrder->SalesRecipesToApply->RecipeGet(s);
					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERRECIPE, 1) FROM RDB$DATABASE";
					IBInternalQuery->ExecQuery();
					int ReceipeKey = IBInternalQuery->Fields[0]->AsInteger;
                    if(CurrentRecipe->Cost < -1000000)
                        CurrentRecipe->Cost = 0;

					IBInternalQuery->Close();
					IBInternalQuery->SQL->Text =
					"INSERT INTO ORDERRECIPE ("
					"ORDERRECIPE_KEY,"
					"ORDER_KEY,"
					"STOCK_CODE,"
					"QTY,"
					"COST,"
					"STOCK_LOCATION) "
					"VALUES ("
					":ORDERRECIPE_KEY,"
					":ORDER_KEY,"
					":STOCK_CODE,"
					":QTY,"
					":COST,"
					":STOCK_LOCATION);";
					IBInternalQuery->ParamByName("ORDERRECIPE_KEY")->AsInteger = ReceipeKey;
					IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = CurrentSubOrder->OrderKey;
					IBInternalQuery->ParamByName("STOCK_CODE")->AsString =  CurrentRecipe->StockCode;
					IBInternalQuery->ParamByName("QTY")->AsFloat         =  CurrentRecipe->Qty;
					IBInternalQuery->ParamByName("COST")->AsCurrency     =  CurrentRecipe->Cost;
					IBInternalQuery->ParamByName("STOCK_LOCATION")->AsString	=  CurrentRecipe->StockLocation;
					IBInternalQuery->ExecQuery();
				}
                }
			}
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "WARNING");
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Order Already Exists");
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Order already added with the Unique Key.");
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Device :" + Order->Terminal);
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Item :" + Order->Item);
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Size :" + Order->Size);
			TManagerLogs::Instance().Add(__FUNC__,DEBUGLOG, "Trans No :" + Order->TransNo);
			Order->DuplicateDetected = true;
		}

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"SetOrder Excep Serving Course Key " + IntToStr(Order->ServingCourse.ServingCourseKey));
		TManagerLogs::Instance().Add(__FUNC__,ERRORLOG,"SetOrder Excep Serving Course Name " + Order->ServingCourse.Name);

		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TDBOrder::GetOrderIdentificationNumberForTable(Database::TDBTransaction &DBTransaction,int tableNumber)
{
   int identificationNumber = 0;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT first(1) ORDER_IDENTIFICATION_NUMBER FROM ORDERS "
                                  " WHERE TABLE_NUMBER=:TABLE_NUMBER";
   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = tableNumber;
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof && IBInternalQuery->Fields[0]->AsInteger > 0)
   {
     identificationNumber = IBInternalQuery->Fields[0]->AsInteger;
   }
   else
   {
     IBInternalQuery->Close();
	 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDER_ID_NO, 1) FROM RDB$DATABASE";
	 IBInternalQuery->ExecQuery();
	 identificationNumber = IBInternalQuery->Fields[0]->AsInteger;
   }
   return identificationNumber;
}

int TDBOrder::GetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,AnsiString tabName)
{
   int identificationNumber = 0;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT first(1) ORDER_IDENTIFICATION_NUMBER FROM ORDERS "
                                  " WHERE TAB_NAME=:TAB_NAME";
   IBInternalQuery->ParamByName("TAB_NAME")->AsString = tabName;
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof)
   {
     identificationNumber = IBInternalQuery->Fields[0]->AsInteger;
   }
   else
   {
     IBInternalQuery->Close();
	 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDER_ID_NO, 1) FROM RDB$DATABASE";
	 IBInternalQuery->ExecQuery();
	 identificationNumber = IBInternalQuery->Fields[0]->AsInteger;
   }
   return identificationNumber;
}

int TDBOrder::GetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,int tabKey)
{
   int identificationNumber = 0;
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "SELECT first(1) ORDER_IDENTIFICATION_NUMBER FROM ORDERS "
                                  " WHERE TAB_KEY=:TAB_KEY";
   IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
   IBInternalQuery->ExecQuery();
   if(!IBInternalQuery->Eof)
   {
     identificationNumber = IBInternalQuery->Fields[0]->AsInteger;
   }
   else
   {
     IBInternalQuery->Close();
	 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDER_ID_NO, 1) FROM RDB$DATABASE";
	 IBInternalQuery->ExecQuery();
	 identificationNumber = IBInternalQuery->Fields[0]->AsInteger;
   }
   return identificationNumber;
}

void TDBOrder::SetOrderIdentificationNumberForTable(Database::TDBTransaction &DBTransaction,int tableNumber,int identificationNumber)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "UPDATE ORDERS SET ORDER_IDENTIFICATION_NUMBER =:ORDER_IDENTIFICATION_NUMBER "
                                  " WHERE TABLE_NUMBER=:TABLE_NUMBER";
   IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificationNumber;
   IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = tableNumber;
   IBInternalQuery->ExecQuery();
}

void TDBOrder::SetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,AnsiString tabName,int identificationNumber)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "UPDATE ORDERS SET ORDER_IDENTIFICATION_NUMBER =:ORDER_IDENTIFICATION_NUMBER "
                                " WHERE TAB_NAME=:TAB_NAME";
   IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificationNumber;
   IBInternalQuery->ParamByName("TAB_NAME")->AsString = tabName;
   IBInternalQuery->ExecQuery();
}

void TDBOrder::SetOrderIdentificationNumberForTab(Database::TDBTransaction &DBTransaction,int tabKey,int identificationNumber)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "UPDATE ORDERS SET ORDER_IDENTIFICATION_NUMBER =:ORDER_IDENTIFICATION_NUMBER "
                                " WHERE TAB_KEY=:TAB_KEY";
   IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificationNumber;
   IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
   IBInternalQuery->ExecQuery();
}

void TDBOrder::SetOrderIdentificationNumber(Database::TDBTransaction &DBTransaction,TList *Orders, int identificationNumber)
{
   TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
   IBInternalQuery->Close();
   IBInternalQuery->SQL->Text = "UPDATE ORDERS SET ORDER_IDENTIFICATION_NUMBER =:ORDER_IDENTIFICATION_NUMBER "
                                " WHERE ORDER_KEY=:ORDER_KEY";
   IBInternalQuery->ParamByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger = identificationNumber;
  for(int i = 0 ; i < Orders->Count; i++)
    {
        TItemComplete *Item = (TItemComplete *)Orders->Items[i];
        if(Item->OrderKey != 0)
        {
            IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Item->OrderKey;
            IBInternalQuery->ExecQuery();
        }
    }
}

void TDBOrder::DeleteOrderDiscounts(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"DELETE FROM ORDERDISCOUNTS WHERE ORDER_KEY = :ORDER_KEY";
	IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
	IBInternalQuery->ExecQuery();
}

void TDBOrder::SetOrderDiscounts(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order)
{
	if(GetOrderExists(DBTransaction,Order->OrderKey))
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();

		for (std::vector<TDiscount>::const_iterator ptrDiscounts = Order->Discounts.begin();ptrDiscounts != Order->Discounts.end(); std::advance(ptrDiscounts,1))
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ORDERDISCOUNTS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			int OrderDiscountKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO ORDERDISCOUNTS ("
			"ORDERDISCOUNTS_KEY,"
			"ORDER_KEY,"
			"DISCOUNT_KEY,"
			"NAME,"
			"DESCRIPTION,"
			"PERCENTAGE,"
			"AMOUNT,"
			"DISCOUNTED_VALUE,"
			"DISCOUNT_TYPE,"
			"DISCOUNT_MODE,"
			"SOURCE,"
			"ROUNDING,"
			"APPEARANCE_ORDER,"
			"PRIORITY,"
			"DISCOUNT_ID,"
			"MEMBERS_ONLY,"
			"MEMBERS_EXEMPT,"
            "ISTHOR_DISCOUNT) "
			"VALUES ("
			":ORDERDISCOUNTS_KEY,"
			":ORDER_KEY,"
			":DISCOUNT_KEY,"
			":NAME,"
			":DESCRIPTION,"
			":PERCENTAGE,"
			":AMOUNT,"
			":DISCOUNTED_VALUE,"
			":DISCOUNT_TYPE,"
			":DISCOUNT_MODE,"
			":SOURCE,"
			":ROUNDING,"
			":APPEARANCE_ORDER,"
			":PRIORITY,"
			":DISCOUNT_ID,"
			":MEMBERS_ONLY,"
			":MEMBERS_EXEMPT,"
            ":ISTHOR_DISCOUNT)";
			IBInternalQuery->ParamByName("ORDERDISCOUNTS_KEY")->AsInteger = OrderDiscountKey;
			IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
			IBInternalQuery->ParamByName("DISCOUNT_KEY")->AsInteger = ptrDiscounts->DiscountKey;
			IBInternalQuery->ParamByName("NAME")->AsString = ptrDiscounts->Name.SubString(1,15);
			IBInternalQuery->ParamByName("DESCRIPTION")->AsString = ptrDiscounts->Description;
			IBInternalQuery->ParamByName("PERCENTAGE")->AsFloat = ptrDiscounts->PercentAmount;
			IBInternalQuery->ParamByName("AMOUNT")->AsCurrency = ptrDiscounts->Amount;
			IBInternalQuery->ParamByName("DISCOUNTED_VALUE")->AsCurrency = Order->DiscountValue_BillCalc(ptrDiscounts);
			IBInternalQuery->ParamByName("ROUNDING")->AsCurrency = ptrDiscounts->Rounding;
			IBInternalQuery->ParamByName("DISCOUNT_TYPE")->AsInteger = ptrDiscounts->Type;
			IBInternalQuery->ParamByName("DISCOUNT_MODE")->AsInteger = ptrDiscounts->AppliedMode;
			IBInternalQuery->ParamByName("SOURCE")->AsInteger = ptrDiscounts->Source;
			IBInternalQuery->ParamByName("APPEARANCE_ORDER")->AsInteger = ptrDiscounts->AppearanceOrder;
			IBInternalQuery->ParamByName("PRIORITY")->AsInteger = ptrDiscounts->Priority;
			IBInternalQuery->ParamByName("DISCOUNT_ID")->AsString = ptrDiscounts->DiscountCode;
			IBInternalQuery->ParamByName("MEMBERS_ONLY")->AsString = (ptrDiscounts->MembersOnly == true) ? "T" : "F";
			IBInternalQuery->ParamByName("MEMBERS_EXEMPT")->AsString = (ptrDiscounts->MembersExempt == true) ? "T" : "F";
            IBInternalQuery->ParamByName("ISTHOR_DISCOUNT")->AsString = (ptrDiscounts->IsThorBill == true) ? "T" : "F";
			IBInternalQuery->ExecQuery();

		}
	}
}

void TDBOrder::DeleteOrderTaxProfiles(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text =
	"DELETE FROM TAXPROFILES_ORDERS WHERE ORDER_KEY = :ORDER_KEY";
	IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
	IBInternalQuery->ExecQuery();
}

void TDBOrder::SetOrderTaxProfiles(Database::TDBTransaction &DBTransaction,TItemMinorComplete * Order)
{
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();

	for (std::vector<TaxProfile>::const_iterator itTaxProfiles = Order->TaxProfiles.begin(); itTaxProfiles != Order->TaxProfiles.end(); std::advance(itTaxProfiles,1))
	{
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_TAXPROFILES_ORDERS, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int TaxProfileOrderKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"INSERT INTO TAXPROFILES_ORDERS "
		"( "
		"TPO_KEY,"
		"PROFILE_KEY,"
		"ORDER_KEY "
		") "
		"VALUES "
		"( "
		":TPO_KEY,"
		":PROFILE_KEY,"
		":ORDER_KEY "
		");";
		IBInternalQuery->ParamByName("TPO_KEY")->AsInteger = TaxProfileOrderKey;
		IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = itTaxProfiles->GetTaxProfileDBKey();
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
		IBInternalQuery->ExecQuery();
	}
}

void TDBOrder::ApplyPricingLevel0(Database::TDBTransaction &DBTransaction,const std::set<__int64> &OrderKeys,bool HappyHourFilter)
{
	try
	{
		if(!OrderKeys.empty())
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			std::auto_ptr<TStringList> KeysList(new TStringList);
			KeysList->QuoteChar = ' ';
			KeysList->Delimiter = ',';
			int i = 1;
			for (std::set<__int64>::const_iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1),i++)
			{
				UnicodeString Key = IntToStr((int)*pOrderKey);
				if(i%1000 == 0)
				{
					i = 1;
					Key = Key +  ") OR ORDER_KEY IN (" + Key;
				}
				KeysList->Add(Key);
			}

			UnicodeString SQL =
			" UPDATE ORDERS "
			" SET PRICE = PRICE_LEVEL0 "
			" WHERE "
			" ORDER_KEY IN (" + KeysList->DelimitedText + ") ";

			if(HappyHourFilter)
			{
				SQL =
				" UPDATE ORDERS "
				" SET PRICE = PRICE_LEVEL0 "
				" WHERE "
				" HAPPYHOUR = 'T' AND "
				" ORDER_KEY IN (" + KeysList->DelimitedText + ") ";
			}

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = SQL;
			IBInternalQuery->ExecQuery();

		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::ApplyPricingLevel1(Database::TDBTransaction &DBTransaction,const std::set<__int64> &OrderKeys,bool HappyHourFilter)
{

	try
	{
		if(!OrderKeys.empty())
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			std::auto_ptr<TStringList> KeysList(new TStringList);
			KeysList->QuoteChar = ' ';
			KeysList->Delimiter = ',';
			int i = 1;
			for (std::set<__int64>::const_iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1),i++)
			{
				UnicodeString Key = IntToStr((int)*pOrderKey);
				if(i%1000 == 0)
				{
					i = 1;
					Key = Key +  ") OR ORDER_KEY IN (" + Key;
				}
				KeysList->Add(Key);
			}

			UnicodeString SQL =
			" UPDATE ORDERS "
			" SET PRICE = PRICE_LEVEL1 "
			" WHERE "
			" ORDER_KEY IN (" + KeysList->DelimitedText + ") ";

			if(HappyHourFilter)
			{
				SQL =
				" UPDATE ORDERS "
				" SET PRICE = PRICE_LEVEL1 "
				" WHERE "
				" HAPPYHOUR = 'T' AND "
				" ORDER_KEY IN (" + KeysList->DelimitedText + ") ";
			}

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = SQL;
			IBInternalQuery->ExecQuery();

		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::SetOrdersLoyalty(Database::TDBTransaction &DBTransaction,const std::set<__int64> &OrderKeys, int inContactKey)
{

	try
	{
		if(!OrderKeys.empty())
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			std::auto_ptr<TStringList> KeysList(new TStringList);
			KeysList->QuoteChar = ' ';
			KeysList->Delimiter = ',';
			int i = 1;
			for (std::set<__int64>::const_iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1))
			{
				UnicodeString Key = IntToStr((int)*pOrderKey);
				if(i%1000 == 0)
				{
					i = 1;
					Key = Key +  ") OR ORDER_KEY IN (" + Key;
				}
				KeysList->Add(Key);
			}

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			" UPDATE ORDERS "
			" SET LOYALTY_KEY = :LOYALTY_KEY "
			" WHERE "
			" ORDER_KEY IN (" + KeysList->DelimitedText + ") ";
			IBInternalQuery->ParamByName("LOYALTY_KEY")->AsString = inContactKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetPrevOrders(Database::TDBTransaction &DBTransaction,UnicodeString TerminalName,int TableNo,int SeatNo,std::vector<TSeatOrders *> &SeatOrders)
{
	// Locate the Table.
	try
	{
		std::set<__int64> SelectedTabs;
		TDBTables::GetTabKeys(DBTransaction,TableNo,SelectedTabs);
		for(std::set<__int64>::iterator itTab = SelectedTabs.begin(); itTab != SelectedTabs.end();	advance(itTab,1) )
		{
			int SeatNo = TDBTables::GetSeatNoFromTabKey(DBTransaction,*itTab);
			SeatOrders[SeatNo]->SeatName = TDBTab::GetTabName(DBTransaction,*itTab);

			std::set<__int64> Orders;
			TDBTab::GetOrderKeysExcludeSides(DBTransaction,*itTab,&Orders);
			TDBTab::LockTab(DBTransaction,TerminalName,*itTab);
			for(std::set<__int64>::iterator itOrder = Orders.begin(); itOrder != Orders.end();	advance(itOrder,1) )
			{
				TItemMinorComplete *PrevItem = new TItemMinorComplete;
				GetPrevOrder(DBTransaction,*itOrder,PrevItem);
				SeatOrders[SeatNo]->Orders->AddPrev(PrevItem);
                SeatOrders[SeatNo]->RoomNumber = PrevItem->RoomNoStr;   //todo add condition
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetPrevOrder(Database::TDBTransaction &DBTransaction,int OrderKey,TItemMinorComplete *PrevItem)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT MENU_ITEM_KEY, ORDER_KEY, QTY, ITEM_NAME,SIZE_NAME,SETMENU_MASK,SETMENU_GROUP,MENU_NAME,"
		" PRICE,DISCOUNT,SECURITY_REF,ORDERS.SERVINGCOURSES_KEY,PRICE_LEVEL0,PRICE_LEVEL1,"
		" ORDER_TYPE_MESSAGE, CONTACTS_KEY, SERVINGCOURSES.*, patron_count, CANCEL_BASE_PRICE, "
		" CANCEL_GROSS_PRICE, CANCEL_FINAL_PRICE, CANCEL_TOTAL_TAX, CANCEL_TOTAL_DISCOUNT, "
        " FIRST_NAME, LAST_NAME, ROOM_NO, ACC_NO, TAB_KEY "
		" FROM ORDERS "
		" LEFT JOIN SERVINGCOURSES ON ORDERS.SERVINGCOURSES_KEY = SERVINGCOURSES.SERVINGCOURSES_KEY "
		" WHERE "
		" ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			PrevItem->ItemKey =
			IBInternalQuery->FieldByName("menu_item_key")->AsInteger;
			PrevItem->OrderKey = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
			PrevItem->Item = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
			PrevItem->Size = IBInternalQuery->FieldByName("SIZE_NAME")->AsString;
			PrevItem->SetQty(IBInternalQuery->FieldByName("QTY")->AsCurrency);
			PrevItem->SetMenuMask = IBInternalQuery->FieldByName("SETMENU_MASK")->AsInteger;
			PrevItem->SetMenuGroup = IBInternalQuery->FieldByName("SETMENU_GROUP")->AsInteger;
			PrevItem->MenuName = IBInternalQuery->FieldByName("MENU_NAME")->AsString;
			PrevItem->PatronCount(IBInternalQuery->FieldByName("patron_count")->AsCurrency, false);
			PrevItem->PriceLevel0 = IBInternalQuery->FieldByName("PRICE_LEVEL0")->AsCurrency;
			PrevItem->PriceLevel1 = IBInternalQuery->FieldByName("PRICE_LEVEL1")->AsCurrency;
			PrevItem->OrderTypeMessage = IBInternalQuery->FieldByName("ORDER_TYPE_MESSAGE")->AsString;
			PrevItem->ContactsKey = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
            PrevItem->AccNo = IBInternalQuery->FieldByName("ACC_NO")->AsString;
            PrevItem->RoomNoStr = IBInternalQuery->FieldByName("ROOM_NO")->AsString;
            PrevItem->FirstName = IBInternalQuery->FieldByName("FIRST_NAME")->AsString;
            PrevItem->LastName = IBInternalQuery->FieldByName("LAST_NAME")->AsString;
            PrevItem->TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
			if (PrevItem->SetMenuMask)
			{
				if (TST_PROMO_MASTER(IBInternalQuery->FieldByName("SETMENU_MASK")->AsInteger))
				{
					PrevItem->SetMenu = false;
					PrevItem->SetMenuMaster = true;
				}
				else
				{
					PrevItem->SetMenu = true;
					PrevItem->SetMenuMaster = false;
				}
			}
			else
			{
				PrevItem->SetMenu = false;
				PrevItem->SetMenuMaster = false;
			}
			PrevItem->SetPriceLevelCustom(IBInternalQuery->FieldByName("PRICE")->AsCurrency);
			PrevItem->Security->SetSecurityRefNumber(IBInternalQuery->FieldByName("SECURITY_REF")->AsInteger);
			PrevItem->CancelledBillCalcResult.BasePrice = IBInternalQuery->FieldByName("CANCEL_BASE_PRICE")->AsCurrency;
			PrevItem->CancelledBillCalcResult.GrossPrice = IBInternalQuery->FieldByName("CANCEL_GROSS_PRICE")->AsCurrency;
			PrevItem->CancelledBillCalcResult.FinalPrice = IBInternalQuery->FieldByName("CANCEL_FINAL_PRICE")->AsCurrency;
			PrevItem->CancelledBillCalcResult.TotalTax = IBInternalQuery->FieldByName("CANCEL_TOTAL_TAX")->AsCurrency;
			PrevItem->CancelledBillCalcResult.TotalDiscount = IBInternalQuery->FieldByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency;
			LoadOrderSecurity(DBTransaction,PrevItem->Security);
			LoadOrderServingCourse(IBInternalQuery, PrevItem->ServingCourse);
			LoadOrderCategories(DBTransaction,PrevItem->OrderKey,PrevItem->Categories);
			LoadOrderDiscounts(DBTransaction,PrevItem);

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
			" SELECT ORDER_KEY,QTY,ITEM_NAME,SIZE_NAME,SETMENU_MASK,SETMENU_GROUP,MENU_NAME,"
			" PRICE, CANCEL_BASE_PRICE, CANCEL_GROSS_PRICE, CANCEL_FINAL_PRICE,"
			" CANCEL_TOTAL_TAX, CANCEL_TOTAL_DISCOUNT, "
            " FIRST_NAME, LAST_NAME, ROOM_NO, ACC_NO, TAB_KEY "
            " FROM ORDERS "
			" WHERE "
			" SIDE_ORDER_KEY = :SIDE_ORDER_KEY";
			IBInternalQuery->ParamByName("SIDE_ORDER_KEY")->AsInteger = OrderKey;
			IBInternalQuery->ExecQuery();
			for (; !IBInternalQuery->Eof ; IBInternalQuery->Next())
			{
				TItemCompleteSub *SubItem = new TItemCompleteSub;
				SubItem->OrderKey = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
				SubItem->Item = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
				SubItem->Size = IBInternalQuery->FieldByName("SIZE_NAME")->AsString;
				SubItem->SetMenuMask = 0;
				SubItem->SetMenu = false;
				SubItem->SetMenuMaster = false;
				SubItem->SetQty(IBInternalQuery->FieldByName("QTY")->AsCurrency);
				SubItem->SetPriceLevelCustom(IBInternalQuery->FieldByName("PRICE")->AsCurrency);
				SubItem->CancelledBillCalcResult.BasePrice = IBInternalQuery->FieldByName("CANCEL_BASE_PRICE")->AsCurrency;
				SubItem->CancelledBillCalcResult.GrossPrice = IBInternalQuery->FieldByName("CANCEL_GROSS_PRICE")->AsCurrency;
				SubItem->CancelledBillCalcResult.FinalPrice = IBInternalQuery->FieldByName("CANCEL_FINAL_PRICE")->AsCurrency;
				SubItem->CancelledBillCalcResult.TotalTax = IBInternalQuery->FieldByName("CANCEL_TOTAL_TAX")->AsCurrency;
				SubItem->CancelledBillCalcResult.TotalDiscount = IBInternalQuery->FieldByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency;
                SubItem->AccNo = IBInternalQuery->FieldByName("ACC_NO")->AsString;
                SubItem->RoomNoStr = IBInternalQuery->FieldByName("ROOM_NO")->AsString;
                SubItem->FirstName = IBInternalQuery->FieldByName("FIRST_NAME")->AsString;
                SubItem->LastName = IBInternalQuery->FieldByName("LAST_NAME")->AsString;
                PrevItem->TabKey = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
				LoadOrderCategories(DBTransaction,SubItem->OrderKey,SubItem->Categories);
				LoadOrderDiscounts(DBTransaction,SubItem);
				PrevItem->SubOrders->SubOrderAdd(SubItem);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TDBOrder::GetSetMenuGroupNumber(Database::TDBTransaction &DBTransaction)
{
	int RetVal = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_SETMENUGROUP, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		RetVal = IBInternalQuery->Fields[0]->AsInteger;
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBOrder::LoadPickNMixOrders(Database::TDBTransaction &DBTransaction,int TabKey, std::vector<TPnMOrder> &Orders)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT ORDER_KEY,ORDER_TYPE,ITEM_NAME,SIZE_NAME,MENU_NAME,SIDE_ORDER_KEY "
		" PRICE,DISCOUNT,QTY,TIME_KEY, PATRON_COUNT FROM ORDERS "
		" WHERE "
		" TAB_KEY = :TAB_KEY "
		" ORDER BY ITEM_NAME,SIZE_NAME";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
		{
			TPnMOrder Order;
			Order.TabKey	=  TabKey;
			Order.Selected	=  false;
			Order.Key 		= 	IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
			Order.Type 		= 	(OrderStatus)IBInternalQuery->FieldByName("ORDER_TYPE")->AsInteger;
			Order.Price 	=  IBInternalQuery->FieldByName("PRICE")->AsCurrency +
			IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;

			Order.TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
			Order.PatronCount =
			IBInternalQuery->FieldByName("patron_count")->AsInteger;

			if(IBInternalQuery->FieldByName("SIZE_NAME")->AsString.UpperCase() != "DEFAULT")
			{
				Order.Name = 	IBInternalQuery->FieldByName("SIZE_NAME")->AsString + " " +
				IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
			}
			else
			{
				Order.Name = 	IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
			}

            if(IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger >0)
            {
                Order.IsSide = true;
            }
            else
            {
                Order.IsSide = false;
            }
			Orders.push_back(Order);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------
double TDBOrder::LoadPickNMixOrdersAndGetQuantity(Database::TDBTransaction &DBTransaction,int TabKey, std::map<__int64,TPnMOrder> &Orders, bool SelectingItems){
    double accumulatedQuantity = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
        "SELECT a.ORDER_KEY,a.ORDER_TYPE,a.ITEM_NAME,a.SIZE_NAME,a.MENU_NAME,a.PRICE,a.DISCOUNT,a.QTY, "
        " a.SIDE_ORDER_KEY,a.TIME_STAMP, a.ITEM_ID,	a.TIME_KEY,a.PATRON_COUNT,a.ITEM_TYPE,b.WEIGHTED_SIZE, "
        " a.ROOM_NO, a.ACC_NO, a.FIRST_NAME, a.LAST_NAME "
        " FROM ORDERS a inner join SIZES b  "
        " on a.SIZE_NAME = b.SIZE_NAME  "
        " WHERE a.TAB_KEY = :TAB_KEY "
        " group by 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19  "
        " ORDER BY a.ITEM_NAME,a.SIZE_NAME,a.Price,a.QTY ";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
		std::set<__int64> ValidOrderKeys;
		std::set<__int64> OrderKeysThatHaveSides;
		TSCDPWDChecker SCDChecker;

        ///Check whether any discount exist having  SCD Or PWD Discount group.
        bool isSCDOrPWDDiscountExist = false;
        bool isPWDDiscountExist = IsSCDOrPWDDiscountConfigured(DBTransaction, "Person with Disability");
        isSCDOrPWDDiscountExist = isPWDDiscountExist;
        bool checkSCDOrPWDExist = false;
        TItemType itemType = eDrinksItem;

        //Create Set For inserting OrderKeys having PWD Discount.
        std::set<__int64> orderKeysWithPWDDiscount;

        //Create Set For inserting OrderKeys having no Discount.
        std::set<__int64> orderKeysWithNoDiscount;

        if(isPWDDiscountExist)
        {
            //Load OrderKeys Having PWD Discount Applied.
            LoadOrderKeysWIthSCDOrPWDDiscount(DBTransaction, TabKey, orderKeysWithPWDDiscount, "Person with Disability");
        }

        if(isPWDDiscountExist)
        {
            //load OrderKeys Which have No Discount..
            LoadOrderKeysWIthoutDiscount(DBTransaction, TabKey, orderKeysWithNoDiscount);

            //If no order key have scd or pwd discount  then make isSCDOrPWDDiscountExist false.
            isSCDOrPWDDiscountExist = /*orderKeysWithSCDDiscount.size() == 0 &&*/ orderKeysWithPWDDiscount.size() == 0 ? false : true;
        }

		for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
		{
			TPnMOrder Order;
			Order.TabKey	  = TabKey;
			Order.ID	      = IBInternalQuery->FieldByName("ITEM_ID")->AsInteger;;
			Order.Selected	  = false;
			Order.Key 		  = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
			Order.Type 		  = (OrderStatus)IBInternalQuery->FieldByName("ORDER_TYPE")->AsInteger;
			Order.GroupNumber = IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger;
			Order.TimeStamp	  = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			Order.TimeKey     = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
            Order.IsWeighted  = IBInternalQuery->FieldByName("WEIGHTED_SIZE")->AsString == "T";
            Order.ItemType 	  = (TItemType)IBInternalQuery->FieldByName("ITEM_TYPE")->AsInteger;
            Order.RoomNumber      = IBInternalQuery->FieldByName("ROOM_NO")->AsString;
            Order.AccNumber      = IBInternalQuery->FieldByName("ACC_NO")->AsString;
            Order.FirstName      = IBInternalQuery->FieldByName("FIRST_NAME")->AsString;
            Order.LastName      = IBInternalQuery->FieldByName("LAST_NAME")->AsString;

            if(isSCDOrPWDDiscountExist)
            {
                bool isSCDApplied = false;
                bool isPWDApplied = false;
                bool noDiscountApplied = false;

                if(orderKeysWithPWDDiscount.find(Order.Key) != orderKeysWithPWDDiscount.end())
                {
                    isPWDApplied = (SCDChecker.ItemSelectionCheckPWD(DBTransaction, Order.Key, ValidOrderKeys, false));
                }
                else if(orderKeysWithNoDiscount.find(Order.Key) != orderKeysWithNoDiscount.end())
                {
                    noDiscountApplied = true;
                }

                checkSCDOrPWDExist = (isPWDApplied || noDiscountApplied);
            }

			if(!SelectingItems || (!isSCDOrPWDDiscountExist || checkSCDOrPWDExist))
			{
				if(Order.GroupNumber != 0)
				{
					OrderKeysThatHaveSides.insert(Order.GroupNumber);
				}

				Order.IsSide = Order.GroupNumber != 0;

				Order.Price = IBInternalQuery->FieldByName("PRICE")->AsCurrency +
				IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;

                if(IBInternalQuery->FieldByName("SIZE_NAME")->AsString.UpperCase() != "DEFAULT")
                {
                    Order.Name = IBInternalQuery->FieldByName("SIZE_NAME")->AsString + " " +
                                        IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
                }
                else
                {
                    Order.Name = IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
                }
                Order.Qty = IBInternalQuery->FieldByName("QTY")->AsFloat;
                Order.PatronCount = IBInternalQuery->FieldByName("PATRON_COUNT")->AsInteger;

                if(IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger >0)
                {
                    Order.IsSide = true;
                }
                else
                {
                    Order.IsSide = false;
                }

                if(Orders.size())
                {
                    std::map <__int64, TPnMOrder> ::iterator itItem = Orders.begin();
                    itemType = itItem->second.ItemType;
                }
                else
                {
                    itemType = Order.ItemType;
                }

                if(SelectingItems && TGlobalSettings::Instance().IsBillSplittedByMenuType)
                {
                    if(itemType == Order.ItemType)
                        Orders[Order.Key] = Order;
                }
                else if(( !SelectingItems && TGlobalSettings::Instance().IsBillSplittedByMenuType) ||
                                (!TGlobalSettings::Instance().IsBillSplittedByMenuType))
                {
                    bool canBeAdded = true;
                    if(orderKeysWithPWDDiscount.find(Order.Key) != orderKeysWithPWDDiscount.end())
                    {
                        canBeAdded = SCDChecker.CanAddItemToSelectedList(DBTransaction,Order,Orders);
                    }
                    if(canBeAdded)
                        Orders[Order.Key] = Order;
                }

                if(ValidOrderKeys.size() == 0)
                  ValidOrderKeys.insert(Order.Key);

                if (!Order.IsWeighted	&& Order.Qty > 1)
                {
                    accumulatedQuantity += Order.Qty;
                }
            }
		}

        ///count orderkeys which have normaldiscount applied
        int itemsWithNormalDiscount = IBInternalQuery->RecordCount - orderKeysWithNoDiscount.size() -  orderKeysWithPWDDiscount.size();

        if(SelectingItems && orderKeysWithPWDDiscount.size() > 0 && itemsWithNormalDiscount > 0)
        {
            MessageBox("Items with PWD Discounts and items with Non PWD Discounts can not be billed at the same time.", "Error", MB_ICONWARNING + MB_OK);
        }

		for(std::set<__int64>::iterator ptrOrderKeys = OrderKeysThatHaveSides.begin(); ptrOrderKeys != OrderKeysThatHaveSides.end(); ptrOrderKeys++ )
		{
			if(Orders.find(*ptrOrderKeys) != Orders.end())
			{  // Associates the Parent with its side ( for sorting) and sets it's parent property.
				Orders[*ptrOrderKeys].GroupNumber = *ptrOrderKeys;
				Orders[*ptrOrderKeys].IsParent = true;
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return accumulatedQuantity;
}//------------------------------------------------------------------------------
void TDBOrder::LoadPickNMixOrders(Database::TDBTransaction &DBTransaction,std::set<__int64> &OrderKeys, std::map<__int64,TPnMOrder> &Orders)
{
	try
	{
		if(!OrderKeys.empty())
		{
			std::auto_ptr<TStringList> KeysList(new TStringList);
			KeysList->QuoteChar = ' ';
			KeysList->Delimiter = ',';
			int i = 1;
			for (std::set<__int64>::const_iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1),i++)
			{
				UnicodeString Key = IntToStr((int)*pOrderKey);
				if(i%1000 == 0)
				{
					i = 1;
					Key = Key +  ") OR ORDER_KEY IN (" + Key;
				}
				KeysList->Add(Key);
			}

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Clear();
			IBInternalQuery->SQL->Text =
			" SELECT ORDER_KEY,ORDER_TYPE,TAB_KEY,ITEM_NAME,SIZE_NAME,MENU_NAME,SIDE_ORDER_KEY, "
			" PRICE,DISCOUNT,QTY,SIDE_ORDER_KEY,TIME_STAMP, TIME_KEY, patron_count "
			" FROM ORDERS "
			" WHERE "
			" ORDER_KEY IN (" + KeysList->DelimitedText + ") "
			" ORDER BY ITEM_NAME,SIZE_NAME";
			IBInternalQuery->ExecQuery();
			std::set<__int64> OrderKeysThatHaveSides;
			for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
			{
				TPnMOrder Order;
				Order.TabKey		=  IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;;
				Order.Selected		=  false;
				Order.Key 			= 	IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
				Order.Type 			= 	(OrderStatus)IBInternalQuery->FieldByName("ORDER_TYPE")->AsInteger;
				Order.GroupNumber = 	IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger;
				Order.TimeStamp	= 	IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
				Order.TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;

				if(Order.GroupNumber != 0)
				{
					OrderKeysThatHaveSides.insert(Order.GroupNumber);
				}
				Order.Price 	=  IBInternalQuery->FieldByName("PRICE")->AsCurrency +
				IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;

				Order.IsSide      = Order.GroupNumber != 0;

				if(IBInternalQuery->FieldByName("SIZE_NAME")->AsString.UpperCase() != "DEFAULT")
				{
					Order.Name 	= 	IBInternalQuery->FieldByName("SIZE_NAME")->AsString + " " +
					IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
				}
				else
				{
					Order.Name	= 	IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
				}
				Order.Qty		= 	IBInternalQuery->FieldByName("QTY")->AsFloat;
                Order.PatronCount = IBInternalQuery->FieldByName("patron_count")->AsInteger;

                if(IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger >0)
                {
                    Order.IsSide = true;
                }
                else
                {
                    Order.IsSide = false;
                }
				Orders[Order.Key] = Order;
			}

			for(std::set<__int64>::iterator ptrOrderKeys = OrderKeysThatHaveSides.begin(); ptrOrderKeys != OrderKeysThatHaveSides.end(); ptrOrderKeys++ )
			{
				if(Orders.find(*ptrOrderKeys) != Orders.end())
				{  // Associates the Parent with its side ( for sorting) and sets it's parent property.
					Orders[*ptrOrderKeys].GroupNumber = *ptrOrderKeys;
					Orders[*ptrOrderKeys].IsParent = true;
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::LoadPickNMixOrder(Database::TDBTransaction &DBTransaction,int OrderKey,TPnMOrder *Order)
{
	try
	{

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT ITEM_NAME,SIZE_NAME,MENU_NAME,SIDE_ORDER_KEY, "
		" PRICE, DISCOUNT, TIME_KEY, PATRON_COUNT FROM ORDERS "
		" WHERE "
		" ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			Order->Key = OrderKey;
			Order->TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
			Order->Price = IBInternalQuery->FieldByName("PRICE")->AsCurrency +
			IBInternalQuery->FieldByName("DISCOUNT")->AsCurrency;

			if(IBInternalQuery->FieldByName("SIZE_NAME")->AsString.UpperCase() != "DEFAULT")
			{
				Order->Name = 	IBInternalQuery->FieldByName("SIZE_NAME")->AsString + " " +
				IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
			}
			else
			{
				Order->Name = 	IBInternalQuery->FieldByName("ITEM_NAME")->AsString;
			}
			Order->PatronCount =
			IBInternalQuery->FieldByName(
			"patron_count")->AsInteger;

            if(IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger >0)
            {
                Order->IsSide = true;
            }
            else
            {
                Order->IsSide = false;
            }
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void TDBOrder::GetOrdersIncludingSidesFromOrderKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> OrderKeys, bool isItemTransfer)
{
	// Leave the Trans State the way we found it.
	try
	{
		for (std::set<__int64>::iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1))
		{
			if(GetOrderExists(DBTransaction,*pOrderKey))
			{
				TItemComplete *Order = new TItemComplete;
				Order->OrderKey = *pOrderKey;
				GetOrderIncludingSidesFromOrderKey(DBTransaction,Order, isItemTransfer);
				Orders->Add(Order);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
}
//------------------------------------------------------------------------------
void TDBOrder::GetMemberKeysFromOrderKeys(Database::TDBTransaction &DBTransaction,std::set<__int64> &MemberKeys, std::set<__int64> OrderKeys)
{
	if(OrderKeys.empty())
	{
		return;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		std::auto_ptr<TStringList> KeysList(new TStringList);
		KeysList->QuoteChar = ' ';
		KeysList->Delimiter = ',';

		int i = 1;
		for (std::set<__int64>::const_iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1), i++)
		{
			UnicodeString OrderKey = IntToStr((int)*pOrderKey);
			if(i%1000 == 0)
			{
				i = 1;
				OrderKey = OrderKey + ") AND LOYALTY_KEY IS NOT NULL AND LOYALTY_KEY != 0 OR ORDER_KEY IN ( " + OrderKey;
			}
			KeysList->Add(OrderKey);
		}

		IBInternalQuery->SQL->Text =
		"SELECT "
		"LOYALTY_KEY "
		"FROM "
		"ORDERS "
		"WHERE "
		"ORDER_KEY IN (" + KeysList->DelimitedText + ") AND LOYALTY_KEY IS NOT NULL "
		"AND LOYALTY_KEY != 0 "
		"ORDER BY LOYALTY_KEY,ORDER_KEY";
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			for(; !IBInternalQuery->Eof;	IBInternalQuery->Next())
			{
				MemberKeys.insert(IBInternalQuery->FieldByName("LOYALTY_KEY")->AsInteger);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetOrdersFromOrderGUID(Database::TDBTransaction &DBTransaction,std::set<__int64> &OrderKeys, std::vector<UnicodeString> OrderGUID)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		TIBSQL *IBInternalQuerySetMenu = DBTransaction.Query(DBTransaction.AddQuery());

		std::auto_ptr<TStringList> KeysList(new TStringList);
		KeysList->QuoteChar = ' ';
		KeysList->Delimiter = ',';
		int i = 1;
		for (std::vector<UnicodeString>::iterator pOrderGUID = OrderGUID.begin(); pOrderGUID != OrderGUID.end(); advance(pOrderGUID,1),i++)
		{
			UnicodeString ItemKey =  "'" +*pOrderGUID + "'";
			if(i%1000 == 0)
			{
				i = 1;
				ItemKey = ItemKey +  ") OR ORDERS.TRANSNO IN (" + ItemKey;
			}
			KeysList->Add(ItemKey);
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
		" SELECT ORDER_KEY,SETMENU_MASK, SETMENU_GROUP FROM ORDERS "
		" WHERE "
		" ORDERS.TRANSNO IN (" + KeysList->DelimitedText + ")";
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
			{
				int Key = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
				OrderKeys.insert(Key);
				// If this Item is a Set Menu Master Item then load the Key for it's
				// Sub Items (but not there sides if any)
				int SetMenuGroup = IBInternalQuery->FieldByName("SETMENU_GROUP")->AsInteger;
				if (IBInternalQuery->FieldByName("SETMENU_MASK")->AsInteger & 0x80000000 && SetMenuGroup != 0)
				{
					/* This SETMENU_GROUP is populated by a Generator so will be unique for this group of orders.*/
					/* Exclude the Setmenu Master Item Above.
					Exclude Sides they are loaded by the parent item later. */

					IBInternalQuerySetMenu->Close();
					IBInternalQuerySetMenu->SQL->Clear();
					IBInternalQuerySetMenu->SQL->Text =
					" SELECT ORDER_KEY FROM ORDERS "
					" WHERE "
					" ORDERS.SETMENU_GROUP = :SETMENU_GROUP AND "
					" ORDERS.ORDER_KEY != :ORDER_KEY AND "
					" ORDERS.SIDE_ORDER_KEY IS NULL ";
					IBInternalQuerySetMenu->ParamByName("SETMENU_GROUP")->AsInteger = SetMenuGroup;
					IBInternalQuerySetMenu->ParamByName("ORDER_KEY")->AsInteger = Key;
					IBInternalQuerySetMenu->ExecQuery();
					for (; !IBInternalQuerySetMenu->Eof ; IBInternalQuerySetMenu->Next())
					{
						int SetMenuItemKey = IBInternalQuerySetMenu->FieldByName("ORDER_KEY")->AsInteger;
						OrderKeys.insert(SetMenuItemKey);
					}
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetOrdersFromOrderKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> OrderKeys)
{
	try
	{
		if(!OrderKeys.empty())
		{
			std::auto_ptr<TStringList> KeysList(new TStringList);
			KeysList->QuoteChar = ' ';
			KeysList->Delimiter = ',';
			int i = 1;
			for (std::set<__int64>::iterator pOrderKey = OrderKeys.begin(); pOrderKey != OrderKeys.end(); advance(pOrderKey,1),i++)
			{
				UnicodeString ItemKey = IntToStr((int)*pOrderKey);
				if(i%1000 == 0)
				{
					i = 1;
					ItemKey = ItemKey +  ") OR MAINORDERS.ORDER_KEY IN (" + ItemKey;
				}
				KeysList->Add(ItemKey);
			}

			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
			IBInternalQuery->SQL->Text =
			"SELECT "
			"MAINORDERS.*, "
			"TAB.TAB_KEY, "
			//MM2038
			"TAB.ID_NUMBER ID_NUMBER, "
			"TAB.TAB_NAME TAB_NAME, "
			"TAB.TAB_TYPE CURRENT_TAB_TYPE, "
			"OPTIONS.PRINT_UNDERLINED, "
			"OPTIONS.PRINT_BOLD, "
			"OPTIONS.PRINT_DOUBLE_WIDTH, "
			"OPTIONS.PRINT_DOUBLE_HEIGHT, "
			"OPTIONS.PRINT_COLOUR, "
			"OPTIONS.PRINT_FONT, "
			"ITEM.ITEM_KEY, "
			"ITEM.PRINT_UNDERLINED ITEM_PRINT_UNDERLINED, "
			"ITEM.PRINT_BOLD ITEM_PRINT_BOLD, "
			"ITEM.PRINT_DOUBLE_WIDTH ITEM_PRINT_DOUBLE_WIDTH, "
			"ITEM.PRINT_DOUBLE_HEIGHT ITEM_PRINT_DOUBLE_HEIGHT, "
			"ITEM.PRINT_COLOUR ITEM_PRINT_COLOUR, "
			"ITEM.PRINT_FONT ITEM_PRINT_FONT, "
			"ORDEROPTION.*, "
			"ORDERRECIPE.ORDERRECIPE_KEY, "
			"ORDERRECIPE.STOCK_CODE RCP_STOCK_CODE, "
			"ORDERRECIPE.QTY RCP_QTY, "
			"ORDERRECIPE.COST RCP_COST, "
			"ORDERRECIPE.STOCK_LOCATION RCP_STOCK_LOCATION, "
			"INVOICES.INVOICE_NUMBER INVOICE_NUMBER, "
			"FINANCIAL_CAT_TABLE.CATEGORY FINANCIAL_CAT, "
            "FINANCIAL_CAT_TABLE.GL_CODE FINANCIAL_CAT_GL_CODE, "
			"FINANCIAL_CAT_TABLE.CATEGORY_KEY FINANCIAL_CAT_KEY, "
			"BREAK_DOWN_CAT_TABLE.CATEGORY BREAK_DOWN_CAT, "
			"BREAK_DOWN_CAT_TABLE.CATEGORY_KEY BREAK_DOWN_CAT_KEY, "
			"SECURITY.SECURITY_KEY SECURITY_SECURITY_KEY, "
			"SECURITY.SECURITY_REF SECURITY_SECURITY_REF, "
			"SECURITY.SECURITY_EVENT SECURITY_SECURITY_EVENT, "
			"SECURITY.FROM_VAL SECURITY_FROM_VAL, "
			"SECURITY.TO_VAL SECURITY_TO_VAL, "
			"SECURITY.NOTE SECURITY_NOTE, "
			"SECURITY.TERMINAL_NAME SECURITY_TERMINAL_NAME, "
			"SECURITY.USER_KEY SECURITY_USER_KEY, "
			"SECURITY.TIME_STAMP SECURITY_TIME_STAMP, "
			"MENU.MENU_KEY, "
			"SERVINGCOURSES.*, "
			"WEBORDERS.* "
			"FROM "
			"ORDERS MAINORDERS "
			"LEFT JOIN TAB ON MAINORDERS.TAB_KEY = TAB.TAB_KEY "
			"LEFT JOIN INVOICES ON TAB.INVOICE_KEY = INVOICES.INVOICE_KEY "
			"LEFT JOIN ORDEROPTION ON MAINORDERS.ORDER_KEY = ORDEROPTION.ORDER_KEY "
			"LEFT JOIN ORDERRECIPE ON MAINORDERS.ORDER_KEY = ORDERRECIPE.ORDER_KEY "
			"LEFT JOIN MENU ON MAINORDERS.MENU_NAME = MENU.MENU_NAME "
			"LEFT JOIN SERVINGCOURSES ON MAINORDERS.SERVINGCOURSES_KEY = SERVINGCOURSES.SERVINGCOURSES_KEY "
			"LEFT JOIN ARCCATEGORIES FINANCIAL_CAT_TABLE ON MAINORDERS.CATEGORY_KEY = FINANCIAL_CAT_TABLE.CATEGORY_KEY "
			"LEFT JOIN ORDERCATEGORY ON MAINORDERS.ORDER_KEY = ORDERCATEGORY.ORDER_KEY "
			"LEFT JOIN ARCCATEGORIES BREAK_DOWN_CAT_TABLE ON ORDERCATEGORY.CATEGORY_KEY = BREAK_DOWN_CAT_TABLE.CATEGORY_KEY "
			"LEFT JOIN SECURITY ON MAINORDERS.SECURITY_REF = SECURITY.SECURITY_REF "
			"LEFT JOIN WEBORDERS ON MAINORDERS.TAB_KEY = WEBORDERS.TAB_KEY "
			"LEFT JOIN OPTIONS ON OPTIONS.OPTIONS_KEY = ORDEROPTION.OPTION_ID "
			"LEFT JOIN ITEM ON MAINORDERS.ITEM_ID = ITEM.ITEM_ID "
			"WHERE "
			"MAINORDERS.ORDER_KEY IN (" + KeysList->DelimitedText + ") "
			"ORDER BY TAB.TAB_KEY,MAINORDERS.ORDER_KEY,ORDEROPTION.ORDEROPTION_KEY, "
			" ORDERRECIPE.ORDERRECIPE_KEY,SECURITY.SECURITY_KEY,BREAK_DOWN_CAT_TABLE.CATEGORY_KEY; ";
		 //	IBInternalQuery->SQL->SaveToFile("temp.txt");
			IBInternalQuery->ExecQuery();

			int LastOrderKey = 0;
			int LastOptionKey = 0;
			int LastReceipeKey = 0;
			std::set<__int64> SecurityKeys;
			std::set<__int64> BreakDownCatKeys;
			TItemComplete *Order = NULL;
			for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
			{
				if(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger != LastOrderKey)
				{
					Order = new TItemComplete;
					Orders->Add(Order);
					Order->OrderKey = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
                    Order->OrderIdentificationNo = IBInternalQuery->FieldByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger;
					Order->Categories->FinancialCategory = IBInternalQuery->FieldByName("FINANCIAL_CAT")->AsString;
                    Order->Categories->FinancialCategoryGLCode = IBInternalQuery->FieldByName("FINANCIAL_CAT_GL_CODE")->AsString;
					Order->Categories->FinancialCategoryKey = IBInternalQuery->FieldByName("FINANCIAL_CAT_KEY")->AsInteger;
					Order->Categories->FinancialCategoryGroup = GetCategoryGroup(DBTransaction, GetCategoriesCategoryGroup(DBTransaction,Order->Categories->FinancialCategoryKey));

					Order->PatronCount(IBInternalQuery->FieldByName("PATRON_COUNT")->AsCurrency, false);

					Order->ThirdPartyKey = IBInternalQuery->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
					Order->ThirdPartyCode = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,Order->ThirdPartyKey);
                    if(IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger>0)
                    {
                        Order->IsSide= true;
                    }
                    else
                    {
                        Order->IsSide= false;
                    }

					LastOrderKey = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;

					LoadOrder(DBTransaction,IBInternalQuery,Order);
					// There are no sides at this point only orders.
					// LoadOrderSide(Transaction,Order->SubOrders,IBInternalQuery.get());
					LoadOrderServingCourse(IBInternalQuery,Order->ServingCourse);
					LoadOrderCategories(DBTransaction,Order->OrderKey,Order->Categories); 	//Been replaced on lines 2431 to 2441
					LoadOrderDiscounts(DBTransaction,Order);
					LoadOrderTaxProfiles(DBTransaction,Order);
					LoadOrderPrinting(IBInternalQuery, Order);
					LastOptionKey = 0;
					LastReceipeKey = 0;
					SecurityKeys.clear();
					BreakDownCatKeys.clear();
				}

				if(IBInternalQuery->FieldByName("ORDEROPTION_KEY")->AsInteger != LastOptionKey)
				{
					LastOptionKey = IBInternalQuery->FieldByName("ORDEROPTION_KEY")->AsInteger;
					if(!IBInternalQuery->FieldByName("ORDEROPTION_KEY")->IsNull)
					{
						LoadOrderOptions(IBInternalQuery,Order->OptionsSelected);
					}
				}

				if(IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->AsInteger != LastReceipeKey)
				{
					LastReceipeKey = IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->AsInteger;
					if(!IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->IsNull)
					{
						LoadOrderReceipe(IBInternalQuery,Order->SalesRecipesToApply);
					}
				}


				if(SecurityKeys.find(IBInternalQuery->FieldByName("SECURITY_SECURITY_KEY")->AsInteger) == SecurityKeys.end())
				{
					SecurityKeys.insert(IBInternalQuery->FieldByName("SECURITY_SECURITY_KEY")->AsInteger);
					if(!IBInternalQuery->FieldByName("SECURITY_SECURITY_REF")->IsNull)
					{
						TSecurityReference   *SecRef = new TSecurityReference;
						SecRef->UserKey      = IBInternalQuery->FieldByName("SECURITY_USER_KEY")->AsInteger;
						SecRef->Event        = IBInternalQuery->FieldByName("SECURITY_SECURITY_EVENT")->AsString;
						SecRef->From         = IBInternalQuery->FieldByName("SECURITY_FROM_VAL")->AsString;
						SecRef->To           = IBInternalQuery->FieldByName("SECURITY_TO_VAL")->AsString;
						SecRef->Note         = IBInternalQuery->FieldByName("SECURITY_NOTE")->AsString;
						SecRef->TimeStamp    = IBInternalQuery->FieldByName("SECURITY_TIME_STAMP")->AsDateTime;
						Order->Security->SecurityAdd(SecRef);
					}
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void  TDBOrder::GetTabKeysFromOrders(TList *Orders, std::set<__int64> &TabKeys)
{
	try
	{
		TabKeys.clear();
		for(int i = 0; i < Orders->Count; i++)
		{
			TItemComplete *Order = (TItemComplete *)Orders->Items[i];
			if(Order->TabKey != 0)
			{
				TabKeys.insert(Order->TabKey);
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void TDBOrder::GetOrderFromOrderKey(Database::TDBTransaction &DBTransaction,TItemComplete *Order)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.TAB_KEY, "
		"TAB.TAB_TYPE CURRENT_TAB_TYPE, "
		"TAB.ID_NUMBER ID_NUMBER, "
		"PARTY_NAME, "
		"MAINORDERS.*, "
		"OPTIONS.PRINT_UNDERLINED, "
		"OPTIONS.PRINT_BOLD, "
		"OPTIONS.PRINT_DOUBLE_WIDTH, "
		"OPTIONS.PRINT_DOUBLE_HEIGHT, "
		"OPTIONS.PRINT_COLOUR, "
		"OPTIONS.PRINT_FONT, "
		"ORDEROPTION.*, "
		"ORDERRECIPE.ORDERRECIPE_KEY, "
		"ORDERRECIPE.STOCK_CODE RCP_STOCK_CODE, "
		"ORDERRECIPE.QTY RCP_QTY, "
		"ORDERRECIPE.COST RCP_COST, "
		"ORDERRECIPE.STOCK_LOCATION RCP_STOCK_LOCATION, "
		"INVOICES.INVOICE_NUMBER INVOICE_NUMBER, "
		"MENU.MENU_KEY, "
		"SERVINGCOURSES.*, "
		"WEBORDERS.* "
		"FROM "
		"TAB LEFT JOIN ORDERS MAINORDERS ON TAB.TAB_KEY = MAINORDERS.TAB_KEY "
		"LEFT JOIN INVOICES ON TAB.INVOICE_KEY = INVOICES.INVOICE_KEY "
		"LEFT JOIN ORDEROPTION ON MAINORDERS.ORDER_KEY = ORDEROPTION.ORDER_KEY "
		"LEFT JOIN ORDERRECIPE ON MAINORDERS.ORDER_KEY = ORDERRECIPE.ORDER_KEY "
		"LEFT JOIN MENU ON MAINORDERS.MENU_NAME = MENU.MENU_NAME "
		"LEFT JOIN SERVINGCOURSES ON MAINORDERS.SERVINGCOURSES_KEY = SERVINGCOURSES.SERVINGCOURSES_KEY "
		"LEFT JOIN WEBORDERS ON MAINORDERS.TAB_KEY = WEBORDERS.TAB_KEY "
		"LEFT JOIN OPTIONS ON OPTIONS.OPTIONS_KEY = ORDEROPTION.OPTION_ID "
		"WHERE "
		"MAINORDERS.ORDER_KEY = :ORDER_KEY "
		"ORDER BY TAB.TAB_KEY,MAINORDERS.ORDER_KEY,ORDEROPTION.ORDEROPTION_KEY ";

		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			int LastOrderKey = 0;
			int LastOptionKey = 0;
			int LastReceipeKey = 0;
			for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
			{
				if(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger != LastOrderKey)
				{
					LastOrderKey = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
					LoadOrder(DBTransaction,IBInternalQuery,Order);
					LoadOrderCategories(DBTransaction,Order->OrderKey,Order->Categories);
					LoadOrderSecurity(DBTransaction,Order->Security);
					LoadOrderServingCourse(IBInternalQuery,Order->ServingCourse);
					LoadOrderDiscounts(DBTransaction,Order);
					LoadOrderTaxProfiles(DBTransaction,Order);

				}

				if(IBInternalQuery->FieldByName("ORDEROPTION_KEY")->AsInteger != LastOptionKey)
				{
					LastOptionKey = IBInternalQuery->FieldByName("ORDEROPTION_KEY")->AsInteger;
					if(!IBInternalQuery->FieldByName("ORDEROPTION_KEY")->IsNull)
					{
						LoadOrderOptions(IBInternalQuery,Order->OptionsSelected);
					}
				}



				if(IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->AsInteger != LastReceipeKey)
				{
					LastReceipeKey = IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->AsInteger;
					if(!IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->IsNull)
					{
						LoadOrderReceipe(IBInternalQuery,Order->SalesRecipesToApply);
					}
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void TDBOrder::GetOrderIncludingSidesFromOrderKey(Database::TDBTransaction &DBTransaction,TItemComplete *Order, bool isItemTransfer)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->SQL->Text =
		"SELECT "
		"TAB.TAB_KEY, "
		"TAB.TAB_TYPE CURRENT_TAB_TYPE, "
		"TAB.ID_NUMBER ID_NUMBER, "
		"PARTY_NAME, "
		"MAINORDERS.*, "
		"OPTIONS.PRINT_UNDERLINED, "
		"OPTIONS.PRINT_BOLD, "
		"OPTIONS.PRINT_DOUBLE_WIDTH, "
		"OPTIONS.PRINT_DOUBLE_HEIGHT, "
		"OPTIONS.PRINT_COLOUR, "
		"OPTIONS.PRINT_FONT, "
		"ITEM.ITEM_KEY, "
		"ITEM.PRINT_UNDERLINED ITEM_PRINT_UNDERLINED, "
		"ITEM.PRINT_BOLD ITEM_PRINT_BOLD, "
		"ITEM.PRINT_DOUBLE_WIDTH ITEM_PRINT_DOUBLE_WIDTH, "
		"ITEM.PRINT_DOUBLE_HEIGHT ITEM_PRINT_DOUBLE_HEIGHT, "
		"ITEM.PRINT_COLOUR ITEM_PRINT_COLOUR, "
		"ITEM.PRINT_FONT ITEM_PRINT_FONT, "
		"ORDEROPTION.*, "
		"ORDERRECIPE.ORDERRECIPE_KEY, "
		"ORDERRECIPE.STOCK_CODE RCP_STOCK_CODE, "
		"ORDERRECIPE.QTY RCP_QTY, "
		"ORDERRECIPE.COST RCP_COST, "
		"ORDERRECIPE.STOCK_LOCATION RCP_STOCK_LOCATION, "
		"INVOICES.INVOICE_NUMBER INVOICE_NUMBER, "
		"MENU.MENU_KEY, "
		"SERVINGCOURSES.*, "
		"WEBORDERS.* "
		"FROM "
		"ORDERS MAINORDERS LEFT JOIN TAB ON TAB.TAB_KEY = MAINORDERS.TAB_KEY "
		"LEFT JOIN INVOICES ON TAB.INVOICE_KEY = INVOICES.INVOICE_KEY "
		"LEFT JOIN ORDEROPTION ON MAINORDERS.ORDER_KEY = ORDEROPTION.ORDER_KEY "
		"LEFT JOIN ORDERRECIPE ON MAINORDERS.ORDER_KEY = ORDERRECIPE.ORDER_KEY "
		"LEFT JOIN MENU ON MAINORDERS.MENU_NAME = MENU.MENU_NAME "
		"LEFT JOIN SERVINGCOURSES ON MAINORDERS.SERVINGCOURSES_KEY = SERVINGCOURSES.SERVINGCOURSES_KEY "
		"LEFT JOIN WEBORDERS ON MAINORDERS.TAB_KEY = WEBORDERS.TAB_KEY "
		"LEFT JOIN OPTIONS ON OPTIONS.OPTION_ID = ORDEROPTION.OPTION_ID "
		"LEFT JOIN ITEM ON MAINORDERS.ITEM_ID = ITEM.ITEM_ID "
		"WHERE "
		"MAINORDERS.ORDER_KEY = :ORDER_KEY "
		"OR MAINORDERS.SIDE_ORDER_KEY = :ORDER_KEY "
		"ORDER BY TAB.TAB_KEY,MAINORDERS.ORDER_KEY,ORDEROPTION.ORDEROPTION_KEY ";

		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
		IBInternalQuery->ExecQuery();
        Order->OptionsSelected->Clear();
		if (IBInternalQuery->RecordCount)
		{
			int LastOrderKey = 0;
			int LastOptionKey = 0;
			int LastReceipeKey = 0;
			for (; !IBInternalQuery->Eof  ;IBInternalQuery->Next())
			{
				if(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger != LastOrderKey)
				{
					LastOrderKey = IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger;
					if(IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->IsNull || IBInternalQuery->FieldByName("SIDE_ORDER_KEY")->AsInteger == 0)
					{
						LoadOrder(DBTransaction,IBInternalQuery,Order);
						LoadOrderCategories(DBTransaction,Order->OrderKey,Order->Categories);
						LoadOrderSecurity(DBTransaction,Order->Security);
						LoadOrderServingCourse(IBInternalQuery,Order->ServingCourse);
						LoadOrderDiscounts(DBTransaction,Order);
						LoadOrderTaxProfiles(DBTransaction,Order);
						LoadOrderPrinting(IBInternalQuery, Order);
					}
					else
				   	{
						LoadOrderSide(DBTransaction,Order->SubOrders,IBInternalQuery);
					}
				}

				if(IBInternalQuery->FieldByName("ORDEROPTION_KEY")->AsInteger != LastOptionKey)
				{
					LastOptionKey = IBInternalQuery->FieldByName("ORDEROPTION_KEY")->AsInteger;
					if(!IBInternalQuery->FieldByName("ORDEROPTION_KEY")->IsNull)
					{
						LoadOrderOptions(IBInternalQuery,Order->OptionsSelected);
					}
				}

				if(IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->AsInteger != LastReceipeKey)
				{
					LastReceipeKey = IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->AsInteger;
					if(!IBInternalQuery->FieldByName("ORDERRECIPE_KEY")->IsNull)
					{
						LoadOrderReceipe(IBInternalQuery,Order->SalesRecipesToApply);
					}
				}
			}
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------
void TDBOrder::LoadOrderSide(Database::TDBTransaction &DBTransaction,TListSubOrderContainer *SubOrders,TIBSQL *SubOrdersTable)
{
	try
	{
		TItemCompleteSub *CurrentSubOrder = new TItemCompleteSub;

		CurrentSubOrder->ItemKey = SubOrdersTable->FieldByName("ITEM_KEY")->AsInteger;
		CurrentSubOrder->Item_ID = SubOrdersTable->FieldByName("ITEM_ID")->AsInteger;
		CurrentSubOrder->OrderKey = SubOrdersTable->FieldByName("ORDER_KEY")->AsInteger;
		CurrentSubOrder->TransNo  = SubOrdersTable->FieldByName("TRANSNO")->AsString;

		CurrentSubOrder->MasterOrderKey = SubOrdersTable->FieldByName("SIDE_ORDER_KEY")->AsInteger;
		CurrentSubOrder->Course = SubOrdersTable->FieldByName("COURSE_NAME")->AsString;
		CurrentSubOrder->Size = SubOrdersTable->FieldByName("SIZE_NAME")->AsString;
		CurrentSubOrder->Item = SubOrdersTable->FieldByName("ITEM_NAME")->AsString;

		CurrentSubOrder->CourseKitchenName	= SubOrdersTable->FieldByName("COURSE_KITCHEN_NAME")->AsString;
		CurrentSubOrder->ItemKitchenName  	= SubOrdersTable->FieldByName("ITEM_KITCHEN_NAME")->AsString;
		CurrentSubOrder->SizeKitchenName	= SubOrdersTable->FieldByName("SIZE_KITCHEN_NAME")->AsString;

		CurrentSubOrder->Item_ID = SubOrdersTable->FieldByName("ITEM_ID")->AsInteger;
		CurrentSubOrder->ItemKey = SubOrdersTable->FieldByName("ITEM_KEY")->AsInteger;

		CurrentSubOrder->PriceLevel0			= SubOrdersTable->FieldByName("PRICE_LEVEL0")->AsCurrency;
		CurrentSubOrder->PriceLevel1			= SubOrdersTable->FieldByName("PRICE_LEVEL1")->AsCurrency;
		if(SubOrdersTable->FieldByName("QTY")->IsNull)
		{
			CurrentSubOrder->SetQty(1);
		}
		else
		{
			CurrentSubOrder->SetQty(SubOrdersTable->FieldByName("QTY")->AsFloat);
		}
		CurrentSubOrder->SetPriceLevelCustom(SubOrdersTable->FieldByName("PRICE")->AsCurrency);

		CurrentSubOrder->Cost = SubOrdersTable->FieldByName("COST")->AsCurrency;
		CurrentSubOrder->CostGSTPercent = SubOrdersTable->FieldByName("COST_GST_PERCENT")->AsFloat;
		//CurrentSubOrder->GSTPercent  = SubOrdersTable->FieldByName("GST_PERCENT")->AsFloat;
		CurrentSubOrder->PointsPercent = SubOrdersTable->FieldByName("POINTS_PERCENT")->AsFloat;

		CurrentSubOrder->DiscountReason = SubOrdersTable->FieldByName("DISCOUNT_REASON")->AsString;
		CurrentSubOrder->Redeemed = SubOrdersTable->FieldByName("REDEEMED")->AsCurrency;
		CurrentSubOrder->ThirdPartyKey = SubOrdersTable->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
		CurrentSubOrder->ThirdPartyCode = TDBThirdPartyCodes::GetThirdPartyCodeByKey(DBTransaction,CurrentSubOrder->ThirdPartyKey);
		CurrentSubOrder->PLU = SubOrdersTable->FieldByName("PLU")->AsInteger;

		CurrentSubOrder->MemberFreeSaleCount = SubOrdersTable->FieldByName("MEMBER_FREE_SALE_COUNT")->AsInteger;
		CurrentSubOrder->MemberFreeSaleDiscount = SubOrdersTable->FieldByName("MEMBER_FREE_SALE_DISCOUNT")->AsFloat;
		CurrentSubOrder->LocationFreeSaleCount = SubOrdersTable->FieldByName("LOCATION_FREE_SALE_COUNT")->AsInteger;
		CurrentSubOrder->LocationFreeSaleDiscount = SubOrdersTable->FieldByName("LOCATION_FREE_SALE_DISCOUNT")->AsFloat;
		CurrentSubOrder->IsFreebe 				= SubOrdersTable->FieldByName("IS_FREE")->AsString.UpperCase() == "F" ? false : true;
		CurrentSubOrder->SetQty(SubOrdersTable->FieldByName("QTY")->AsFloat);
		if(SubOrdersTable->FieldByName("QTY")->IsNull){ CurrentSubOrder->SetQty(1);}


		CurrentSubOrder->Security->SetSecurityRefNumber(SubOrdersTable->FieldByName("SECURITY_REF")->AsInteger);
		CurrentSubOrder->Categories->FinancialCategoryKey = SubOrdersTable->FieldByName("CATEGORY_KEY")->AsInteger;
		CurrentSubOrder->SetMenuMask	= SubOrdersTable->FieldByName("SETMENU_MASK")->AsInteger;
		CurrentSubOrder->SetMenuGroup	= SubOrdersTable->FieldByName("SETMENU_GROUP")->AsInteger;
		if(CurrentSubOrder->SetMenuMask != 0 && CurrentSubOrder->SetMenuGroup != 0)
		{
			CurrentSubOrder->SetMenu = true;
		}
		CurrentSubOrder->CancelledBillCalcResult.BasePrice = SubOrdersTable->FieldByName("CANCEL_BASE_PRICE")->AsCurrency;
		CurrentSubOrder->CancelledBillCalcResult.GrossPrice = SubOrdersTable->FieldByName("CANCEL_GROSS_PRICE")->AsCurrency;
		CurrentSubOrder->CancelledBillCalcResult.FinalPrice = SubOrdersTable->FieldByName("CANCEL_FINAL_PRICE")->AsCurrency;
		CurrentSubOrder->CancelledBillCalcResult.TotalTax = SubOrdersTable->FieldByName("CANCEL_TOTAL_TAX")->AsCurrency;
		CurrentSubOrder->CancelledBillCalcResult.TotalDiscount = SubOrdersTable->FieldByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency;
        CurrentSubOrder->DelayedInvoiceNumber = SubOrdersTable->FieldByName("DELAYED_INVOICE_NUMBER")->AsString;
        CurrentSubOrder->isManuallyEnteredWeight = (SubOrdersTable->FieldByName("IS_MANUALLY_ENTERED_WEIGHT")->AsString == "T")?1:0;

		LoadOrderCategories(DBTransaction,CurrentSubOrder->OrderKey,CurrentSubOrder->Categories);
		LoadOrderDiscounts(DBTransaction,CurrentSubOrder);
		LoadOrderServingCourse(SubOrdersTable,CurrentSubOrder->ServingCourse);
		LoadOrderTaxProfiles(DBTransaction, CurrentSubOrder);
		SubOrders->SubOrderAdd(CurrentSubOrder);
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetOrdersFromTabKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> SelectedTabs, bool isSide)
{
	if(SelectedTabs.empty())
	{
		return;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		std::auto_ptr<TStringList> KeysList(new TStringList);
		KeysList->QuoteChar = ' ';
		KeysList->Delimiter = ',';

		int i = 1;
		for (std::set<__int64>::iterator pTabKey = SelectedTabs.begin(); pTabKey != SelectedTabs.end(); advance(pTabKey,1), i++)
		{
			UnicodeString TabKey = IntToStr((int)*pTabKey);
			if(i%1000 == 0)
			{
				i = 1;
				TabKey = ") OR TAB_KEY IN (" + TabKey;
			}
			KeysList->Add(TabKey);
		}

		IBInternalQuery->SQL->Text =
		"SELECT "
		"ORDER_KEY "
		"FROM "
		"ORDERS "
		"WHERE "
		"TAB_KEY IN (" + KeysList->DelimitedText + ") "
		"ORDER BY TAB_KEY,ORDER_KEY";
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			std::set<__int64> OrderKeys;
			for(; !IBInternalQuery->Eof;	IBInternalQuery->Next())
			{
				OrderKeys.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
			}

			GetOrdersFromOrderKeys(DBTransaction,Orders,OrderKeys);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::GetOrdersIncludingSidesFromTabKeys(Database::TDBTransaction &DBTransaction,TList *Orders, std::set<__int64> SelectedTabs, bool isItemTransfer)
{
	if(SelectedTabs.empty())
	{
		return;
	}

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		std::auto_ptr<TStringList> KeysList(new TStringList);
		KeysList->QuoteChar = ' ';
		KeysList->Delimiter = ',';

		int i = 1;
		for (std::set<__int64>::iterator pTabKey = SelectedTabs.begin(); pTabKey != SelectedTabs.end(); advance(pTabKey,1), i++)
		{
			UnicodeString TabKey = IntToStr((int)*pTabKey);
			if(i%1000 == 0)
			{
				i = 1;
				TabKey = TabKey + ") OR TAB_KEY IN (" + TabKey;
			}
			KeysList->Add(TabKey);
		}
		/* Exclud the Sides here it will be loaded in as part of the order */
		IBInternalQuery->SQL->Text =
		"SELECT "
		"ORDER_KEY "
		"FROM "
		"ORDERS "
		"WHERE "
		"TAB_KEY IN (" + KeysList->DelimitedText + ") "
		"AND (SIDE_ORDER_KEY IS NULL OR SIDE_ORDER_KEY = 0)"
		"ORDER BY TAB_KEY,ORDER_KEY";
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			std::set<__int64> OrderKeys;
			for(; !IBInternalQuery->Eof;	IBInternalQuery->Next())
			{
				OrderKeys.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
			}
			GetOrdersIncludingSidesFromOrderKeys(DBTransaction,Orders,OrderKeys, true);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

bool TDBOrder::GetOrderExists(Database::TDBTransaction &DBTransaction,int OrderKey)
{
	bool RetVal = false;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"ORDER_KEY "
		"FROM "
		"ORDERS "
		"WHERE "
		"ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBOrder::GetOrderKeys(Database::TDBTransaction &DBTransaction,int TabKey,std::set<__int64> &SelectedItems)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT "
		"ORDER_KEY "
		"FROM "
		"ORDERS "
		"WHERE "
		"TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			SelectedItems.insert(IBInternalQuery->FieldByName("ORDER_KEY")->AsInteger);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::SetSecurityRef(Database::TDBTransaction &DBTransaction,int OrderKey,int OldRef,int SecurityRef)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" UPDATE ORDERS "
		" SET "
		" SECURITY_REF = :SECURITY_REF "
		" WHERE "
		"  ORDER_KEY = :ORDER_KEY AND SECURITY_REF = :OLDSECURITY_REF";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ParamByName("OLDSECURITY_REF")->AsInteger = OldRef;
		IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = SecurityRef;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
int TDBOrder::GetCategoryGroup(Database::TDBTransaction &DBTransaction,UnicodeString CategoryGroup)
{
	int CatGroupkey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CATEGORYGROUPS_KEY FROM CATEGORYGROUPS "
		"WHERE NAME = :NAME";
		IBInternalQuery->ParamByName("NAME")->AsString = CategoryGroup;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			CatGroupkey = IBInternalQuery->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CatGroupkey;
}
//---------------------------------------------------------------------------
UnicodeString TDBOrder::GetCategoryGroup(Database::TDBTransaction &DBTransaction,int CategoryGroupKey)
{
	UnicodeString CatGroup = "";
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT NAME FROM CATEGORYGROUPS "
		"WHERE CATEGORYGROUPS_KEY = :CATEGORYGROUPS_KEY";
		IBInternalQuery->ParamByName("CATEGORYGROUPS_KEY")->AsString = CategoryGroupKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			CatGroup = IBInternalQuery->FieldByName("NAME")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CatGroup;
}

int TDBOrder::GetCategoriesCategoryGroup(Database::TDBTransaction &DBTransaction,int CategoryKey)
{
	int CatGroupkey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CATEGORYGROUPS_KEY FROM ARCCATEGORIES "
		"WHERE CATEGORY_KEY = :CATEGORY_KEY";
		IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount)
		{
			CatGroupkey = IBInternalQuery->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CatGroupkey;
}

int TDBOrder::GetOrCreateCategoryGroup(Database::TDBTransaction &DBTransaction,UnicodeString CategoryGroup)
{
	int CatGroupkey = 0;
	try
	{
		if(CategoryGroup == "")
		{
			throw Exception("A Blank Category is not allowed.");
		}

		CatGroupkey = GetCategoryGroup(DBTransaction,CategoryGroup);
		if (CatGroupkey == 0)
		{
			CatGroupkey = SetCategoryGroup(DBTransaction,CategoryGroup);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CatGroupkey;
}

int TDBOrder::SetCategoryGroup(Database::TDBTransaction &DBTransaction,UnicodeString CategoryGroup)
{
	int CategoryGroupKey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(CategoryGroup == "")
		{
			throw Exception("A Blank Category is not allowed.");
		}

		CategoryGroupKey = GetCategoryGroup(DBTransaction,CategoryGroup);
		if(CategoryGroupKey == 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_CATEGORYGROUPS, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			CategoryGroupKey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO CATEGORYGROUPS ("
			"CATEGORYGROUPS_KEY,"
			"NAME,"
			"VISIBLE) "
			"VALUES ("
			":CATEGORYGROUPS_KEY,"
			":NAME,"
			":VISIBLE);";
			IBInternalQuery->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CategoryGroupKey;
			IBInternalQuery->ParamByName("NAME")->AsString = CategoryGroup;
			IBInternalQuery->ParamByName("VISIBLE")->AsString = "T";
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return CategoryGroupKey;
}

int TDBOrder::SetCategoryToGroup(Database::TDBTransaction &DBTransaction,int CategoryKey, int CategoryGroupKey)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(GetCategoriesCategoryGroup(DBTransaction,CategoryKey) != CategoryGroupKey)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"ARCCATEGORIES "
			"SET "
			"CATEGORYGROUPS_KEY = :CATEGORYGROUPS_KEY "
			"WHERE "
			"CATEGORY_KEY = :CATEGORY_KEY";
			IBInternalQuery->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CategoryGroupKey;
			IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
	return CategoryGroupKey;
}
//---------------------------------------------------------------------------
int TDBOrder::SetArchiveCategory(Database::TDBTransaction &DBTransaction,UnicodeString Category)
{
	int ArcCatkey = 0;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(Category == "")
		{
			throw Exception("A Blank Category is not allowed.");
		}

		ArcCatkey = GetArchiveCategory(DBTransaction,Category);

		if (ArcCatkey == 0)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) FROM RDB$DATABASE";
			IBInternalQuery->ExecQuery();
			ArcCatkey = IBInternalQuery->Fields[0]->AsInteger;

			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"INSERT INTO ARCCATEGORIES ("
			"CATEGORY_KEY,"
			"CATEGORY,"
			"VISIBLE) "
			"VALUES ("
			":CATEGORY_KEY,"
			":CATEGORY,"
			":VISIBLE);";
			IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = ArcCatkey;
			IBInternalQuery->ParamByName("CATEGORY")->AsString = Category;
			IBInternalQuery->ParamByName("VISIBLE")->AsString = "T";
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return ArcCatkey;
}
//---------------------------------------------------------------------------
void TDBOrder::SetArchiveCategoryGroupEnabled(Database::TDBTransaction &DBTransaction,int CategoryGroupKey, bool Enabled)
{
	if (CategoryGroupKey == 0) return;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(GetCategoryGroupEnabled(DBTransaction,CategoryGroupKey) != Enabled)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"CATEGORYGROUPS "
			"SET "
			"VISIBLE = :VISIBLE "
			"WHERE "
			"CATEGORYGROUPS_KEY = :CATEGORYGROUPS_KEY";
			IBInternalQuery->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CategoryGroupKey;
			IBInternalQuery->ParamByName("VISIBLE")->AsString = Enabled ? "T" : "F";
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
void TDBOrder::SetArchiveCategoryEnabled(Database::TDBTransaction &DBTransaction,int CategoryKey, bool Enabled)
{
	if (CategoryKey == 0) return;
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		if(GetCategoryEnabled(DBTransaction,CategoryKey) != Enabled)
		{
			IBInternalQuery->Close();
			IBInternalQuery->SQL->Text =
			"UPDATE "
			"ARCCATEGORIES "
			"SET "
			"VISIBLE = :VISIBLE "
			"WHERE "
			"CATEGORY_KEY = :CATEGORY_KEY";
			IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
			IBInternalQuery->ParamByName("VISIBLE")->AsString = Enabled ? "T" : "F";
			IBInternalQuery->ExecQuery();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
int TDBOrder::GetArchiveCategory(Database::TDBTransaction &DBTransaction,UnicodeString Category)
{
	int RetVal = 0;
	if(Category == "") return RetVal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES "
		"WHERE CATEGORY = :CATEGORY";
		IBInternalQuery->ParamByName("CATEGORY")->AsString = Category;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}
//---------------------------------------------------------------------------
UnicodeString TDBOrder::GetArchiveCategory(Database::TDBTransaction &DBTransaction,int CategoryKey)
{
	UnicodeString RetVal = "";
	if(CategoryKey == 0) return RetVal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CATEGORY FROM ARCCATEGORIES "
		"WHERE CATEGORY_KEY = :CATEGORY_KEY";
		IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("CATEGORY")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBOrder::GetArchiveCategories(Database::TDBTransaction &DBTransaction,std::map<int,UnicodeString > &CatList)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CATEGORY_KEY,CATEGORY FROM ARCCATEGORIES ";
		IBInternalQuery->ExecQuery();

		for (; !IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			CatList[IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger] = IBInternalQuery->FieldByName("CATEGORY")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

bool TDBOrder::GetCategoryEnabled(Database::TDBTransaction &DBTransaction,int CategoryKey)
{
	bool RetVal = false;
	if(CategoryKey == 0) return RetVal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT VISIBLE FROM ARCCATEGORIES "
		"WHERE CATEGORY_KEY = :CATEGORY_KEY";
		IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = CategoryKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = (IBInternalQuery->FieldByName("VISIBLE")->AsString == "T") ? true : false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

bool TDBOrder::GetCategoryGroupEnabled(Database::TDBTransaction &DBTransaction,int CategoryGroupKey)
{
	bool RetVal = false;
	if(CategoryGroupKey == 0) return RetVal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT VISIBLE FROM CATEGORYGROUPS "
		"WHERE CATEGORYGROUPS_KEY = :CATEGORYGROUPS_KEY";
		IBInternalQuery->ParamByName("CATEGORYGROUPS_KEY")->AsInteger = CategoryGroupKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			RetVal = (IBInternalQuery->FieldByName("VISIBLE")->AsString == "T") ? true : false;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

bool TDBOrder::DeleteOrder(Database::TDBTransaction &DBTransaction,__int64 OrderKey)
{
	bool RetVal = false;
	if(OrderKey == 0) return RetVal;

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Unhook any sides to this order.
		IBInternalQuery->Close();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
		"UPDATE "
		"ORDERS "
		"SET "
		"SIDE_ORDER_KEY = NULL "
		"WHERE "
		"SIDE_ORDER_KEY = :SIDE_ORDER_KEY";
		IBInternalQuery->ParamByName("SIDE_ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();

		// Retrieve active chit key for latter deletion
		IBInternalQuery->Close();
		IBInternalQuery->ParamCheck = true;
		IBInternalQuery->SQL->Text =
		"SELECT "
		"ACTIVECHITNUMBER_KEY "
		"FROM "
		"ORDERS "
		"WHERE "
		"ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();
		int activeChitKey = 0;
		if(!IBInternalQuery->Eof)
		{
			if(!IBInternalQuery->FieldByName("ACTIVECHITNUMBER_KEY")->IsNull)
			{
				activeChitKey = IBInternalQuery->FieldByName("ACTIVECHITNUMBER_KEY")->AsInteger;
			}
		}

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		" DELETE "
		" FROM "
		" ORDERS "
		" WHERE "
		" ORDER_KEY = :LOC_ORDER_KEY";
		IBInternalQuery->ParamByName("LOC_ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RowsAffected != 0)
		{
			RetVal = true;
		}

		if (activeChitKey != 0)
		{
			TDBActiveChit::DeleteActiveChit(DBTransaction, activeChitKey);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBOrder::LoadOrder(Database::TDBTransaction &DBTransaction,TIBSQL *OrderTable,TItemComplete *Order)
{
	if (OrderTable != NULL)
	{
		Order->OrderKey				= OrderTable->FieldByName("ORDER_KEY")->AsInteger;
        Order->OrderIdentificationNo = OrderTable->FieldByName("ORDER_IDENTIFICATION_NUMBER")->AsInteger;
		Order->TransNo				= OrderTable->FieldByName("TRANSNO")->AsString;
		Order->TabType				= (TMMTabType)OrderTable->FieldByName("TAB_TYPE")->AsInteger;
		Order->PartyName			= OrderTable->FieldByName("PARTY_NAME")->AsString;
		Order->TabKey				= OrderTable->FieldByName("TAB_KEY")->AsInteger;
		//MM2038
		Order->IdName               = OrderTable->FieldByName("TAB_NAME")->AsString;
		Order->IdNumber             = OrderTable->FieldByName("ID_NUMBER")->AsString;
		Order->WebKey				= OrderTable->FieldByName("WEBORDER_KEY")->AsInteger;
		Order->TabName				= OrderTable->FieldByName("TAB_NAME")->AsString;
		Order->ContainerTabType		= Order->TabType;
		Order->TableNo				= OrderTable->FieldByName("TABLE_NUMBER")->AsInteger;
		Order->TabContainerName		= OrderTable->FieldByName("TABLE_NAME")->AsString;
		Order->SeatNo				= OrderTable->FieldByName("SEATNO")->AsInteger;
		Order->Terminal				= OrderTable->FieldByName("TERMINAL_NAME")->AsString;
		Order->MenuName				= OrderTable->FieldByName("MENU_NAME")->AsString;
		Order->ItemType				= (TItemType)OrderTable->FieldByName("ITEM_TYPE")->AsInteger;
		// TODO 4 -o Michael -c Printing : Remove When Printing Engine Finished
		Order->MenuKey 				= OrderTable->FieldByName("MENU_KEY")->AsInteger;
		Order->ItemKey 				= OrderTable->FieldByName("ITEM_KEY")->AsInteger;

		//---------------------------
		Order->PriceLevel0 = OrderTable->FieldByName("PRICE_LEVEL0")->AsCurrency;
		Order->PriceLevel1 = OrderTable->FieldByName("PRICE_LEVEL1")->AsCurrency;
        Order->MaxRetailPrice = OrderTable->FieldByName("MAXRETAILPRICE")->AsCurrency;

		Order->HappyHour =
		OrderTable->FieldByName("HAPPYHOUR")->AsString.UpperCase() != "F";

		Order->OrderType = OrderTable->FieldByName("ORDER_TYPE")->AsInteger;

		Currency curr_price = OrderTable->FieldByName("price")->AsCurrency;
		if ((curr_price != Order->PriceLevel0 && curr_price != Order->PriceLevel1) || Order->OrderType == CanceledOrder)
		Order->SetPriceLevelCustom(curr_price);


		Order->Item_ID = OrderTable->FieldByName("ITEM_ID")->AsInteger;
		Order->Size =
		OrderTable->FieldByName("SIZE_NAME")->AsString;

		Order->SetQty(
		OrderTable->FieldByName("QTY")->AsCurrency);
		if(OrderTable->FieldByName("QTY")->IsNull)
		Order->SetQty(1);
		Order->PatronCount(
		OrderTable->FieldByName("patron_count")->AsCurrency,
		false);

		Order->Course							= OrderTable->FieldByName("COURSE_NAME")->AsString;
		Order->Item								= OrderTable->FieldByName("ITEM_NAME")->AsString;

		Order->CourseKitchenName				= OrderTable->FieldByName("COURSE_KITCHEN_NAME")->AsString;
		Order->ItemKitchenName  				= OrderTable->FieldByName("ITEM_KITCHEN_NAME")->AsString;
		Order->SizeKitchenName					= OrderTable->FieldByName("SIZE_KITCHEN_NAME")->AsString;

		Order->MasterContainer					= OrderTable->FieldByName("MASTER_CONTAINER")->AsString;
		Order->SetMenuMask						= OrderTable->FieldByName("SETMENU_MASK")->AsInteger;
		Order->SetMenuMaster					= TST_PROMO_MASTER(Order->SetMenuMask);
		Order->Note								= OrderTable->FieldByName("NOTE")->AsString;
		Order->OrderedLocation					= OrderTable->FieldByName("ORDER_LOCATION")->AsString;
		Order->TimeStamp						= OrderTable->FieldByName("TIME_STAMP")->AsDateTime;
		Order->Loyalty_Key						= OrderTable->FieldByName("LOYALTY_KEY")->AsInteger;
		//Order->GSTPercent        				= OrderTable->FieldByName("GST_PERCENT")->AsFloat;
		Order->PointsPercent					= OrderTable->FieldByName("POINTS_PERCENT")->AsFloat;
		Order->Cost								= OrderTable->FieldByName("COST")->AsCurrency;
		Order->Redeemed							= OrderTable->FieldByName("REDEEMED")->AsCurrency;
		Order->CostGSTPercent					= OrderTable->FieldByName("COST_GST_PERCENT")->AsFloat;
		Order->TimeKey 							= OrderTable->FieldByName("TIME_KEY")->AsInteger;
		Order->DiscountReason					= OrderTable->FieldByName("DISCOUNT_REASON")->AsString;
		// SQL in by left joining in the Invoive table via the tab key.
		//		Order->InvoiceNumber			= Invoice->GetInvoiceNumber(Tabs.GetTabInvoice(Order->TabKey));
		Order->InvoiceNumber					= OrderTable->FieldByName("INVOICE_NUMBER")->AsString;

		Order->ThirdPartyKey 					= OrderTable->FieldByName("THIRDPARTYCODES_KEY")->AsInteger;
		Order->PLU 								= OrderTable->FieldByName("PLU")->AsInteger;

		Order->MemberFreeSaleCount 				= OrderTable->FieldByName("MEMBER_FREE_SALE_COUNT")->AsInteger;
		Order->MemberFreeSaleDiscount 			= OrderTable->FieldByName("MEMBER_FREE_SALE_DISCOUNT")->AsFloat;
		Order->LocationFreeSaleCount 			= OrderTable->FieldByName("LOCATION_FREE_SALE_COUNT")->AsInteger;
		Order->LocationFreeSaleDiscount 		= OrderTable->FieldByName("LOCATION_FREE_SALE_DISCOUNT")->AsFloat;
		Order->IsFreebe 						= OrderTable->FieldByName("IS_FREE")->AsString.UpperCase() == "F" ? false : true;

		Order->Security->SetSecurityRefNumber(OrderTable->FieldByName("SECURITY_REF")->AsInteger);
		Order->Categories->FinancialCategoryKey = OrderTable->FieldByName("CATEGORY_KEY")->AsInteger;
		Order->SetMenuMask						= OrderTable->FieldByName("SETMENU_MASK")->AsInteger;
		Order->SetMenuGroup						= OrderTable->FieldByName("SETMENU_GROUP")->AsInteger;
		if(Order->SetMenuMask != 0 && Order->SetMenuGroup != 0)
		{
			Order->SetMenu = true;
		}
		Order->OriginalItemKey = OrderTable->FieldByName("MENU_ITEM_KEY")->AsInteger;

		if(!OrderTable->FieldByName("ACTIVECHITNUMBER_KEY")->IsNull)
		{
			Order->SetActiveChitNumberKey(OrderTable->FieldByName("ACTIVECHITNUMBER_KEY")->AsInteger);
			Order->ChitNumber = TDBActiveChit::LoadChitNumberFromKey(DBTransaction, Order->GetActiveChitNumberKey());
		}

		// This will effect speed but it wont crash now.
        Order->IsCanBePaidForUsingPoints = GetCanBePaidWithPoints(DBTransaction,Order);

		Order->CancelledBillCalcResult.BasePrice = OrderTable->FieldByName("CANCEL_BASE_PRICE")->AsCurrency;
		Order->CancelledBillCalcResult.GrossPrice = OrderTable->FieldByName("CANCEL_GROSS_PRICE")->AsCurrency;
		Order->CancelledBillCalcResult.FinalPrice = OrderTable->FieldByName("CANCEL_FINAL_PRICE")->AsCurrency;
		Order->CancelledBillCalcResult.TotalTax = OrderTable->FieldByName("CANCEL_TOTAL_TAX")->AsCurrency;
		Order->CancelledBillCalcResult.TotalDiscount = OrderTable->FieldByName("CANCEL_TOTAL_DISCOUNT")->AsCurrency;
        Order->DelayedInvoiceNumber = OrderTable->FieldByName("DELAYED_INVOICE_NUMBER")->AsString;
        Order->isManuallyEnteredWeight = (OrderTable->FieldByName("IS_MANUALLY_ENTERED_WEIGHT")->AsString == "T")?1:0;
        Order->ItemPriceForPointsOriginal = GetPriceForPoints(DBTransaction,Order);
        Order->ItemPriceForPoints = Order->ItemPriceForPointsOriginal;
        Order->RoomNoStr = OrderTable->FieldByName("ROOM_NO")->AsString;
        Order->AccNo = atoi(AnsiString(OrderTable->FieldByName("ACC_NO")->AsString).c_str());
        Order->FirstName = (AnsiString(OrderTable->FieldByName("FIRST_NAME")->AsString));
        Order->LastName = (AnsiString(OrderTable->FieldByName("LAST_NAME")->AsString));
        Order->OrderGuid = (AnsiString(OrderTable->FieldByName("ORDER_GUID")->AsString));
        Order->OrderItemId = OrderTable->FieldByName("ORDER_ITEM_ID")->AsInteger;
        Order->OrderItemSizeId = OrderTable->FieldByName("ORDER_ITEM_SIZE_ID")->AsInteger;
        Order->ReferenceOrderItemSizeId = OrderTable->FieldByName("REFERENCE_ORDER_ITEM_SIZE_ID")->AsInteger;
        Order->SiteId = OrderTable->FieldByName("SITE_ID")->AsInteger;
		Order->Email = (AnsiString(OrderTable->FieldByName("EMAIL")->AsString));
        Order->ContactsKey = OrderTable->FieldByName("CONTACTS_KEY")->AsInteger;
        Order->OnlineChitType = OrderTable->FieldByName("ONLINE_CHIT_TYPE")->AsInteger;
        Order->OnlineOrderId = OrderTable->FieldByName("ONLINE_ORDER_ID")->AsInteger;
 	}
}

bool TDBOrder::GetCanBePaidWithPoints(Database::TDBTransaction &DBTransaction,TItemComplete *Order)
{
    bool RetVal = false;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->ParamCheck = true;
    IBInternalQuery->SQL->Text =
    "SELECT "
    "CAN_BE_PAID_FOR_USING_POINTS "
    "FROM "
    "ITEMSIZE "
    "WHERE ITEM_KEY = :ITEM_KEY AND "
    "SIZE_NAME = :SIZE_NAME";
    IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = Order->ItemKey ;
    IBInternalQuery->ParamByName("SIZE_NAME")->AsString = Order->Size ;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
        RetVal = IBInternalQuery->FieldByName("CAN_BE_PAID_FOR_USING_POINTS")->AsInteger != 0;
    }

    return RetVal;
}

void TDBOrder::LoadOrderPrinting(TIBSQL *OrderTable,TItemComplete *Order)
{
	if (OrderTable != NULL)
	{
		Order->FontInfo.Underlined				= (OrderTable->FieldByName("ITEM_PRINT_UNDERLINED")->AsString == "T");
		Order->FontInfo.Bold						= (OrderTable->FieldByName("ITEM_PRINT_BOLD")->AsString == "T");
		Order->FontInfo.Colour					= (OrderTable->FieldByName("ITEM_PRINT_COLOUR")->AsInteger == 0) ? fcBlack : fcRed;
		Order->FontInfo.Font						= (OrderTable->FieldByName("ITEM_PRINT_FONT")->AsInteger == 0) ? ftFontA : ftFontB;
		Order->FontInfo.Width					= (OrderTable->FieldByName("ITEM_PRINT_DOUBLE_WIDTH")->AsString == "T") ? fsDoubleSize : fsNormalSize;
		Order->FontInfo.Height					= (OrderTable->FieldByName("ITEM_PRINT_DOUBLE_HEIGHT")->AsString == "T") ? fsDoubleSize : fsNormalSize;
	}
}

void TDBOrder::LoadOrderSecurity(Database::TDBTransaction &DBTransaction,TListSecurityRefContainer *Security)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		// Check For Dupliacte Ref and Events
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT * FROM SECURITY WHERE SECURITY_REF = :SECURITY_REF";
		IBInternalQuery->ParamByName("SECURITY_REF")->AsInteger = Security->GetSecurityRefNumber();
		IBInternalQuery->ExecQuery();
		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			TSecurityReference   *SecRef = new TSecurityReference;
			SecRef->UserKey      = IBInternalQuery->FieldByName("USER_KEY")->AsInteger;
			SecRef->Event        = IBInternalQuery->FieldByName("SECURITY_EVENT")->AsString;
			SecRef->From         = IBInternalQuery->FieldByName("FROM_VAL")->AsString;
			SecRef->To           = IBInternalQuery->FieldByName("TO_VAL")->AsString;
			SecRef->Note         = IBInternalQuery->FieldByName("NOTE")->AsString;
			SecRef->TimeStamp    = IBInternalQuery->FieldByName("TIME_STAMP")->AsDateTime;
			Security->SecurityAdd(SecRef);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::LoadOrderDiscounts(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();

		// Check For Dupliacte Ref and Events
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT * FROM ORDERDISCOUNTS WHERE ORDER_KEY = :ORDER_KEY Order by ORDERDISCOUNTS_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
		IBInternalQuery->ExecQuery();
		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			TDiscount Discount;
			Discount.DBKey = IBInternalQuery->FieldByName("ORDERDISCOUNTS_KEY")->AsInteger;
			Discount.Name = IBInternalQuery->FieldByName("NAME")->AsString;
			Discount.Description = IBInternalQuery->FieldByName("DESCRIPTION")->AsString;
			Discount.DiscountKey = IBInternalQuery->FieldByName("DISCOUNT_KEY")->AsInteger;
			Discount.DiscountCode = IBInternalQuery->FieldByName("DISCOUNT_ID")->AsString;
			Discount.Priority = IBInternalQuery->FieldByName("PRIORITY")->AsInteger;
			Discount.AppearanceOrder = IBInternalQuery->FieldByName("APPEARANCE_ORDER")->AsInteger;
			Discount.PercentAmount = IBInternalQuery->FieldByName("PERCENTAGE")->AsFloat;
			Discount.Amount = IBInternalQuery->FieldByName("AMOUNT")->AsCurrency;
			Discount.Rounding = IBInternalQuery->FieldByName("ROUNDING")->AsCurrency;
			Discount.Type = static_cast<TDiscountType>(IBInternalQuery->FieldByName("DISCOUNT_TYPE")->AsInteger);
			Discount.Mode = static_cast<TDiscountMode>(IBInternalQuery->FieldByName("DISCOUNT_MODE")->AsInteger);
            Discount.AppliedMode = static_cast<TDiscountMode>(IBInternalQuery->FieldByName("DISCOUNT_MODE")->AsInteger);
			Discount.Source = static_cast<TDiscountSource>(IBInternalQuery->FieldByName("SOURCE")->AsInteger);
			Discount.MembersOnly = IBInternalQuery->FieldByName("MEMBERS_ONLY")->AsString == "T";
			Discount.MembersExempt = IBInternalQuery->FieldByName("MEMBERS_EXEMPT")->AsString == "T";
            Discount.IsThorBill = IBInternalQuery->FieldByName("ISTHOR_DISCOUNT")->AsString == "T";
            if(Discount.IsThorBill)
                {
                  TManagerDiscount::GetThorVoucherCategories(DBTransaction,Discount.DiscountKey,Discount);
                }
              else
                {
                    TManagerDiscount::GetDiscountCategories(DBTransaction,Discount.DiscountKey,Discount);
                }

            TManagerDiscount::PopulateDiscountGroupPerType( Discount.DiscountKey, Discount );
			Order->DiscountAdd(Discount);

        }
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::LoadOrderTaxProfiles(Database::TDBTransaction &DBTransaction,TItemMinorComplete *Order)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQuery->Close();

		TTaxProfileDBAccessManager_MM taxProfileManager;

		// Check For Dupliacte Ref and Events
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT PROFILE_KEY FROM TAXPROFILES_ORDERS WHERE ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
		IBInternalQuery->ExecQuery();
		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
			int taxProfileKey = IBInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
			TaxProfile taxProfile = taxProfileManager.LoadTaxProfileFromKey(DBTransaction, taxProfileKey);
			Order->TaxProfiles.push_back(taxProfile);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::LoadOrderOptions(TIBSQL *OptionsTable,TListOptionContainer *OptionsSelected)
{
	try
	{
		TItemOption *Option = new TItemOption;
		Option->Owner = OptionsSelected;
		OptionsSelected->OptionAdd(Option);

		Option->OptionKey		= OptionsTable->FieldByName("ORDEROPTION_KEY")->AsInteger;
		Option->OptionID 		= (unsigned short)OptionsTable->FieldByName("OPTION_ID")->AsInteger;
		Option->Name 			= OptionsTable->FieldByName("OPTION_NAME")->AsString;
		Option->KitchenName 	=  OptionsTable->FieldByName("OPTION_KITCHEN_NAME")->AsString;
		if(Option->KitchenName == UnicodeString(""))
		{
			Option->KitchenName = Option->Name;
		}
		Option->Enabled 		= true;
		Option->Flags 			= 0;
		Option->GroupNumber 	= 0;  // least sig nibble of lsb
		Option->IsPlus 		= OptionsTable->FieldByName("PLUS")->AsString.UpperCase() == "F" ? false : true;
		Option->PlusOption 	= 0;
		Option->MinusOption 	= 0;
		Option->ForcedMask 	= 0;
		Option->Palm_ID 		= 0;

		Option->FontInfo.Underlined = (OptionsTable->FieldByName("PRINT_UNDERLINED")->AsString == "T");;
		Option->FontInfo.Bold = (OptionsTable->FieldByName("PRINT_BOLD")->AsString == "T");;
		Option->FontInfo.Width = (OptionsTable->FieldByName("PRINT_DOUBLE_WIDTH")->AsString == "T") ? fsDoubleSize : fsNormalSize;;
		Option->FontInfo.Height = (OptionsTable->FieldByName("PRINT_DOUBLE_HEIGHT")->AsString == "T") ? fsDoubleSize : fsNormalSize;
		Option->FontInfo.Colour = (OptionsTable->FieldByName("PRINT_COLOUR")->AsInteger == 0) ? fcBlack : fcRed;
		Option->FontInfo.Font = (OptionsTable->FieldByName("PRINT_FONT")->AsInteger == 0) ? ftFontA : ftFontB;

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::LoadOrderCategories(Database::TDBTransaction &DBTransaction, int OrderKey,TListCategoryContainer *Categories)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT CATEGORY_KEY,CATEGORY,GL_CODE FROM ARCCATEGORIES "
		"WHERE "
		"CATEGORY_KEY = :CATEGORY_KEY";
		IBInternalQuery->ParamByName("CATEGORY_KEY")->AsInteger = Categories->FinancialCategoryKey;
		IBInternalQuery->ExecQuery();
		if(IBInternalQuery->RecordCount)
		{
			Categories->FinancialCategory = IBInternalQuery->FieldByName("CATEGORY")->AsString;
            Categories->FinancialCategoryGLCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
		}

		Categories->FinancialCategoryGroup = GetCategoryGroup(DBTransaction,GetCategoriesCategoryGroup(DBTransaction,Categories->FinancialCategoryKey));

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT ORDERCATEGORY.CATEGORY_KEY,ARCCATEGORIES.CATEGORY, ARCCATEGORIES.GL_CODE  FROM ORDERCATEGORY INNER JOIN "
		" ARCCATEGORIES ON ARCCATEGORIES.CATEGORY_KEY = ORDERCATEGORY.CATEGORY_KEY"
		" WHERE ORDERCATEGORY.ORDER_KEY = :ORDER_KEY";
		IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = OrderKey;
		IBInternalQuery->ExecQuery();

		for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
		{
			TItemSizeCategory *SizeCat = new TItemSizeCategory;
			SizeCat->CategoryKey = IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger;
			SizeCat->Category = IBInternalQuery->FieldByName("CATEGORY")->AsString;
            SizeCat->GLCode = IBInternalQuery->FieldByName("GL_CODE")->AsString;
			Categories->CategoryAdd(SizeCat);
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

void TDBOrder::LoadOrderReceipe(TIBSQL *RecipeTable,TListRecipeContainer *Recipes)
{
	TItemRecipe *CurrentRecipe = new TItemRecipe;
	CurrentRecipe->StockCode = RecipeTable->FieldByName("RCP_STOCK_CODE")->AsString;
	CurrentRecipe->Qty = RecipeTable->FieldByName("RCP_QTY")->AsFloat;
	CurrentRecipe->Cost = RecipeTable->FieldByName("RCP_COST")->AsCurrency;
	CurrentRecipe->StockLocation = RecipeTable->FieldByName("RCP_STOCK_LOCATION")->AsString;
	Recipes->RecipeAdd(CurrentRecipe);
}

void TDBOrder::LoadOrderServingCourse(TIBSQL *ServeringCourseTable, TServingCourse &ServingCourse)
{
	ServingCourse.ServingCourseKey = ServeringCourseTable->FieldByName("SERVINGCOURSES_KEY")->AsInteger;
	ServingCourse.Name = ServeringCourseTable->FieldByName("SERVINGCOURSE_NAME")->AsString;
	ServingCourse.KitchenName = ServeringCourseTable->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString;
	if(ServingCourse.KitchenName == UnicodeString(""))
	{
		ServingCourse.KitchenName = ServingCourse.Name;
	}
	ServingCourse.Colour = (TColor)ServeringCourseTable->FieldByName("COLOUR")->AsInteger;
	ServingCourse.SCOO = ServeringCourseTable->FieldByName("DISPLAY_ORDER")->AsInteger;
}

void TDBOrder::GetCategoryList(Database::TDBTransaction &DBTransaction,TStringList *CategoryList)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT CATEGORY_KEY,CATEGORY "
		"FROM ARCCATEGORIES "
		"WHERE VISIBLE = 'T' "
		"ORDER BY CATEGORY";
		IBInternalQuery->ExecQuery();

		for (;!IBInternalQuery->Eof ; IBInternalQuery->Next())
		{
			UnicodeString CatName = IBInternalQuery->FieldByName("CATEGORY")->AsString;
			CategoryList->AddObject(CatName, (System::TObject*)(IBInternalQuery->FieldByName("CATEGORY_KEY")->AsInteger));
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------------
Currency TDBOrder::LoadCreditFromOrders(Database::TDBTransaction &DBTransaction,TList *Orders)
{
	Currency RetVal = 0;
	try
	{
		std::set<__int64> SelectedTabs;
		GetTabKeysFromOrders(Orders, SelectedTabs);

		// Sum Credits.
		if(!SelectedTabs.empty())
		{
			TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

			UnicodeString SQL			= "SELECT SUM(CREDIT) FROM TAB WHERE TAB_KEY IN (";
			bool First				= true;
			for(std::set<__int64>::iterator CrntTabKey = SelectedTabs.begin(); CrntTabKey != SelectedTabs.end(); advance(CrntTabKey,1))
			{
				if (First)
				{
					SQL += IntToStr((int)*CrntTabKey);
					First = false;
				}
				else
				{
					SQL += "," + IntToStr((int)*CrntTabKey);
				}
			}
			IBInternalQuery->SQL->Text = SQL + ")";
			IBInternalQuery->ExecQuery();
			RetVal = IBInternalQuery->Fields[0]->AsCurrency;
			IBInternalQuery->Close();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
	return RetVal;
}

void TDBOrder::GetCNaOTFromTableNum(Database::TDBTransaction &DBTransaction, int TableNumber, std::pair<int, AnsiString> &CustNameAndOrderType)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT ORDERS.TABLE_NUMBER, ORDERS.ORDER_TYPE_MESSAGE, ORDERS.CONTACTS_KEY "
		"FROM ORDERS "
		"WHERE TABLE_NUMBER = :TABLE_NUMBER";
		IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = TableNumber;
		IBInternalQuery->ExecQuery();
		if (!IBInternalQuery->Eof)
		{
			CustNameAndOrderType.first  = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
			CustNameAndOrderType.second = IBInternalQuery->FieldByName("ORDER_TYPE_MESSAGE")->AsString;
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

int TDBOrder::GetTimeKey( Database::TDBTransaction &DBTransaction,int TableNumber)
{
	int TimeKey = 0;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "SELECT  first(1) a.TIME_KEY FROM ORDERS a where a.TAB_KEY = :TAB_KEY";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TableNumber;
	IBInternalQuery->ExecQuery();
	if(!IBInternalQuery->Eof)
	{
		TimeKey = IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
	}
	return TimeKey;
}

void TDBOrder::UpdateOrderForDelayedPayment(Database::TDBTransaction &DBTransaction,
                                            TItemComplete * Order,
                                            AnsiString sourceName,
                                            int TabKey,
                                            AnsiString TabName,
                                            AnsiString InvoiceNumber,
                                            int UserKey,
                                            TDateTime TransferTime)
{
    try
    {
            if(Order->OrderKey != 0)
            {
                    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

                    IBInternalQuery->Close();
                    IBInternalQuery->SQL->Text =
                    "UPDATE "
                    "ORDERS "
                    "SET "
                    "TAB_KEY = :TAB_KEY, "
                    "TABLE_NUMBER = :TABLE_NUMBER, "
                    "TABLE_NAME = :TABLE_NAME, "
                    "TAB_NAME = :TAB_NAME, "
                    "TAB_TYPE = :TAB_TYPE, "
                    "QTY = :QTY, "
                    " SEATNO = 0 , "
                    "DELAYED_PAYMENT_PRICE = :DELAYED_PAYMENT_PRICE, "
                    "DELAYED_INVOICE_NUMBER  = :DELAYED_INVOICE_NUMBER "
                    "WHERE "
                    "ORDER_KEY = :ORDER_KEY";
                    IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
                    IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = 0;
                    IBInternalQuery->ParamByName("TABLE_NAME")->AsString = TabName;
                    IBInternalQuery->ParamByName("TAB_NAME")->AsString = TabName;
                    IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabDelayedPayment;
                    IBInternalQuery->ParamByName("DELAYED_INVOICE_NUMBER")->AsString = InvoiceNumber;
                    IBInternalQuery->ParamByName("DELAYED_PAYMENT_PRICE")->AsCurrency = Order->BillCalcResult.FinalPrice;
                    IBInternalQuery->ParamByName("QTY")->AsCurrency = Order->GetQty();
                    IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order->OrderKey;
                    IBInternalQuery->ExecQuery();
                    TSecurityReference *SecRef = new TSecurityReference;
                    SecRef->UserKey = UserKey;
                    SecRef->Event =   SecurityTypes[secTransferredPrelim];
                    SecRef->From =    sourceName;
                    SecRef->Note =    "";
                    SecRef->To =      TabName;
                    SecRef->TimeStamp = TransferTime;
                    Order->Security->SecurityAddUniqueRef(DBTransaction,Order->OrderKey,SecRef);
                    for (int i = 0; i < Order->SubOrders->Count ; i++)
                    {
                            TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);

                            IBInternalQuery->Close();
                            IBInternalQuery->SQL->Text =
                            "UPDATE "
                            "ORDERS "
                            "SET "
                            "TAB_KEY = :TAB_KEY, "
                            "TABLE_NUMBER = :TABLE_NUMBER, "
                            "TABLE_NAME = :TABLE_NAME, "
                            "TAB_NAME = :TAB_NAME, "
                            "TAB_TYPE = :TAB_TYPE, "
                            "QTY = :QTY, "
                            "SEATNO = 0 , "
                            "DELAYED_PAYMENT_PRICE = :DELAYED_PAYMENT_PRICE, "
                            "DELAYED_INVOICE_NUMBER  = :DELAYED_INVOICE_NUMBER "
                            "WHERE "
                            "ORDER_KEY = :ORDER_KEY";
                            IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
                            IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = 0;
                            IBInternalQuery->ParamByName("TABLE_NAME")->AsString = TabName;
                            IBInternalQuery->ParamByName("TAB_NAME")->AsString = TabName;
                            IBInternalQuery->ParamByName("TAB_TYPE")->AsInteger = TabDelayedPayment;
                            IBInternalQuery->ParamByName("DELAYED_INVOICE_NUMBER")->AsString = InvoiceNumber;
                            IBInternalQuery->ParamByName("DELAYED_PAYMENT_PRICE")->AsCurrency = SubOrder->BillCalcResult.FinalPrice;
                            IBInternalQuery->ParamByName("QTY")->AsCurrency = SubOrder->GetQty();
                            IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = SubOrder->OrderKey;
                            IBInternalQuery->ExecQuery();
                            TSecurityReference *SecRef = new TSecurityReference;
                            SecRef->UserKey = UserKey;
                            SecRef->Event =   SecurityTypes[secTransferredPrelim];
                            SecRef->From =    sourceName;
                            SecRef->Note =    "";
                            SecRef->To =      TabName;
                            SecRef->TimeStamp = TransferTime;
                            SubOrder->Security->SecurityAddUniqueRef(DBTransaction,SubOrder->OrderKey,SecRef);
                    }
                    TDBSecurity::ProcessSecurity(DBTransaction,Order->Security);
                    for (int j = 0; j < Order->SubOrders->Count; j++)
                    {
                            TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
                            if(SubOrder)
                            {
                                    TDBSecurity::ProcessSecurity(DBTransaction,SubOrder->Security);
                            }
                    }
            }
    }
    catch(Exception &E)
    {
            TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
            throw;
    }
}
//--------------------------------------------------------------------------------------------------------
bool TDBOrder::IsItemUsingPCD(Database::TDBTransaction &DBTransaction, int itemKey, AnsiString sizeName)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT a.AVAILABLE_QUANTITY FROM ITEMSIZE a "
        "WHERE a.ITEM_KEY = :ITEM_KEY AND a.SIZE_NAME = :SIZE_NAME ";
        IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemKey;
		IBInternalQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
		IBInternalQuery->ExecQuery();

        if( IBInternalQuery->FieldByName("AVAILABLE_QUANTITY")->AsInteger > 0)
        {
             return true;
        }
        else
        {
            return false;
        }

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}

bool TDBOrder::CheckItemHasSide(Database::TDBTransaction &DBTransaction, int Order_key)
{
  //
    bool IsSide = false;
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = "select a.SIDE_ORDER_KEY from Orders a where a.SIDE_ORDER_KEY = :ORDER_KEY ";
	IBInternalQuery->ParamByName("ORDER_KEY")->AsInteger = Order_key;
	IBInternalQuery->ExecQuery();
	if(!IBInternalQuery->Eof)
	{
		IsSide = true;
	}
	return IsSide;
}

UnicodeString TDBOrder::GetContactName(Database::TDBTransaction &DBTransaction, int tab_key)
{
  //
    UnicodeString staffName = "";
	TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
	IBInternalQuery->Close();
	IBInternalQuery->SQL->Text = " SELECT  TAB.TAB_KEY, CONTACTS.NAME NAME "
                                 " FROM TAB INNER JOIN CONTACTS ON CONTACTS.CONTACTS_KEY = TAB.CONTACTS_KEY  WHERE "
                                 " TAB.TAB_TYPE = 1 and Tab.TAB_KEY = :TAB_KEY   ORDER BY  CONTACTS.NAME";
	IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tab_key;
	IBInternalQuery->ExecQuery();
    if (!(IBInternalQuery->RecordCount == 0))
    {
       staffName = IBInternalQuery->FieldByName("NAME")->AsString;
    }
    return staffName;
}

void TDBOrder::UpdateOrderTableForWebOrders(Database::TDBTransaction &DBTransaction, TItemComplete *Order)
{
   try
   {
       TIBSQL *IBUpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBUpdateQuery->Close();
       IBUpdateQuery->SQL->Text =
                "UPDATE "
                "ORDERS "
                "SET "
                "ACTIVECHITNUMBER_KEY = :ACTIVECHITNUMBER_KEY "
                "WHERE "
                "ORDER_KEY = :ORDER_KEY";

            if(Order->GetActiveChitNumberKey() == 0)
            {
                IBUpdateQuery->ParamByName("ACTIVECHITNUMBER_KEY")->Clear();
            }
            else
            {
                IBUpdateQuery->ParamByName("ACTIVECHITNUMBER_KEY")->AsInteger = Order->GetActiveChitNumberKey();
            }
            IBUpdateQuery->ParamByName("ORDER_KEY")->AsString = Order->OrderKey;

            IBUpdateQuery->ExecQuery();
            IBUpdateQuery->Close();

            for (int i = 0; i < Order->SubOrders->Count; i++)
            {
                TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(i);

                if(Order->GetActiveChitNumberKey() == 0)
                {
                    IBUpdateQuery->ParamByName("ACTIVECHITNUMBER_KEY")->Clear();
                }
                else
                {
                    IBUpdateQuery->ParamByName("ACTIVECHITNUMBER_KEY")->AsInteger = Order->GetActiveChitNumberKey();
                }
                IBUpdateQuery->ParamByName("ORDER_KEY")->AsString = SubOrder->OrderKey;

                IBUpdateQuery->ExecQuery();
                IBUpdateQuery->Close();
            }
        //}
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

}

Currency TDBOrder::GetPriceForPoints(Database::TDBTransaction &DBTransaction,TItemComplete *Order)
{
    Currency RetVal = 0;
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    IBInternalQuery->Close();
    IBInternalQuery->ParamCheck = true;
    IBInternalQuery->SQL->Text =
    "SELECT "
    "PRICE_FOR_POINTS "
    "FROM "
    "ITEMSIZE "
    "WHERE ITEM_KEY = :ITEM_KEY AND "
    "SIZE_NAME = :SIZE_NAME";
    IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = Order->ItemKey ;
    IBInternalQuery->ParamByName("SIZE_NAME")->AsString = Order->Size ;
    IBInternalQuery->ExecQuery();
    if(!IBInternalQuery->Eof)
    {
        RetVal = IBInternalQuery->FieldByName("PRICE_FOR_POINTS")->AsCurrency;
    }
    return RetVal;
}
////---------------------------------------------------------------------------
void TDBOrder::UpdateOrderTableDlinkingWithClipp(Database::TDBTransaction &dbTransaction,long SourceKey)
{
    if(SourceKey <= 0)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG, "Source Key doesn't exist");
        throw;
    }
    TIBSQL *updateQuery = dbTransaction.Query(dbTransaction.AddQuery());
    try
    {
                updateQuery->Close();
                updateQuery->SQL->Text =
                                        " UPDATE ORDERS "
                                        " SET "
                                        " ORDERS.TABLE_NUMBER = :TABLE_NUMBER, "
                                        " ORDERS.SEATNO = :SEATNO, "
                                        " ORDERS.PARTY_NAME = :PARTY_NAME, "
                                        " ORDERS.TABLE_NAME = :TABLE_NAME "
                                        " WHERE "
                                        " ORDERS.TAB_KEY = :TAB_KEY;";
                updateQuery->ParamByName("TAB_KEY")->AsInteger = SourceKey;
                updateQuery->ParamByName("TABLE_NUMBER")->AsInteger = 0;
                updateQuery->ParamByName("SEATNO")->AsInteger = 0;
                updateQuery->ParamByName("TABLE_NAME")->AsString = "";
                updateQuery->ParamByName("PARTY_NAME")->AsString = "";
                updateQuery->ExecQuery();
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        throw;
    }

}
///-------------------------------------------------------------------------------------
void TDBOrder::UpdateTabNameInOrder(Database::TDBTransaction &DBTransaction,long SourceKey, UnicodeString clipTabName,bool isTabSelected)
{
  TIBSQL *IBUpdateQuery = DBTransaction.Query(DBTransaction.AddQuery());
  if(isTabSelected)
  {

   IBUpdateQuery->Close();
   IBUpdateQuery->SQL->Text =
			"UPDATE "
			"ORDERS "
			"SET "
			"TAB_NAME = :TAB_NAME "
			"WHERE "
			"TAB_KEY = :TAB_KEY ";

    IBUpdateQuery->ParamByName("TAB_KEY")->AsInteger = SourceKey;
    IBUpdateQuery->ParamByName("TAB_NAME")->AsString = clipTabName;
  	IBUpdateQuery->ExecQuery();

  }
  else
  {

   IBUpdateQuery->Close();
   IBUpdateQuery->SQL->Text =
			"UPDATE "
			"ORDERS "
			"SET "
			"TAB_NAME = :TAB_NAME "
			"WHERE "
			"ORDER_KEY = :ORDER_KEY ";

    IBUpdateQuery->ParamByName("ORDER_KEY")->AsInteger = SourceKey;
    IBUpdateQuery->ParamByName("TAB_NAME")->AsString = clipTabName;
  	IBUpdateQuery->ExecQuery();
  }

}

int TDBOrder::CheckItemAvailability(Database::TDBTransaction &DBTransaction, int itemKey, AnsiString sizeName)
{
	try
	{
        int itm_qty;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
		"SELECT a.AVAILABLE_QUANTITY FROM ITEMSIZE a "
        "WHERE a.ITEM_KEY = :ITEM_KEY AND a.SIZE_NAME = :SIZE_NAME ";
        IBInternalQuery->ParamByName("ITEM_KEY")->AsInteger = itemKey;
		IBInternalQuery->ParamByName("SIZE_NAME")->AsString = sizeName;
		IBInternalQuery->ExecQuery();

        if(!IBInternalQuery->Eof)
        {
            itm_qty = IBInternalQuery->FieldByName("AVAILABLE_QUANTITY")->AsInteger;
        }
        /*if( IBInternalQuery->FieldByName("AVAILABLE_QUANTITY")->AsInteger > 0)
        {
             return true;
        }
        else
        {
            return false;
        }*/
        return itm_qty;

	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//------------------------------------------------------------------------------------------------------------
bool TDBOrder::IsSCDOrPWDDiscountConfigured(Database::TDBTransaction &DBTransaction, UnicodeString discountGroup)
{
    bool isDiscountConfigured = false;
    try
    {
        TIBSQL *SelectDiscountGroup = DBTransaction.Query(DBTransaction.AddQuery());
        SelectDiscountGroup->Close();
        SelectDiscountGroup->SQL->Clear();
        SelectDiscountGroup->SQL->Text =
                                    "SELECT  a.NAME, "
                                    "DGDT.DISCOUNTTYPE_KEY, "
                                    "DGDT.DISCOUNTGROUPS_KEY, "
                                    "DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY "
                                    "FROM DISCOUNTS a "
                                    "INNER JOIN DISCOUNTGROUPS_DISCOUNTTYPES DGDT ON A.DISCOUNT_KEY = DGDT.DISCOUNTTYPE_KEY "
                                    "INNER JOIN DISCOUNT_GROUPS ON  DISCOUNT_GROUPS.DISCOUNTGROUPS_KEY = DGDT.DISCOUNTGROUPS_KEY "
                                    "WHERE DISCOUNT_GROUPS.DISCOUNTGROUP_NAME = :DISCOUNTGROUP_NAME_1 ";
        SelectDiscountGroup->ParamByName("DISCOUNTGROUP_NAME_1")->AsString = discountGroup;
        SelectDiscountGroup->ExecQuery();

        if(SelectDiscountGroup->RecordCount)
            isDiscountConfigured = true;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

    return isDiscountConfigured;
}
//-------------------------------------------------------------------------------------------------------------------
void TDBOrder::LoadOrderKeysWIthSCDOrPWDDiscount(Database::TDBTransaction &DBTransaction, int tabKey, std::set<__int64> &orderKeysWithSCDOrPWDDiscount,
                                                UnicodeString discountGroup)
{
    try
    {
        TIBSQL *selectOrderKeysWithSCDOrPWDDiscount = DBTransaction.Query(DBTransaction.AddQuery());
        selectOrderKeysWithSCDOrPWDDiscount->Close();
        selectOrderKeysWithSCDOrPWDDiscount->SQL->Clear();
        selectOrderKeysWithSCDOrPWDDiscount->SQL->Text =
                                    "SELECT a.ORDER_KEY "
                                    "FROM ORDERS a inner join SIZES b "
                                        "ON a.SIZE_NAME = b.SIZE_NAME "
                                    "INNER JOIN ORDERDISCOUNTS OD "
                                        "ON a.ORDER_KEY = OD.ORDER_KEY "
                                    "INNER JOIN DISCOUNTGROUPS_DISCOUNTTYPES DGD "
                                        "ON OD.DISCOUNT_KEY = DGD.DISCOUNTTYPE_KEY "
                                    "INNER JOIN DISCOUNT_GROUPS DG "
                                        "ON DGD.DISCOUNTGROUPS_KEY = DG.DISCOUNTGROUPS_KEY "
                                    "WHERE a.TAB_KEY = :TAB_KEY AND "
                                        "DG.DISCOUNTGROUP_NAME = :DISCOUNTGROUP_NAME "
                                    "GROUP by 1 "
                                    "ORDER BY a.ORDER_KEY ";

        selectOrderKeysWithSCDOrPWDDiscount->ParamByName("TAB_KEY")->AsString = tabKey;
        selectOrderKeysWithSCDOrPWDDiscount->ParamByName("DISCOUNTGROUP_NAME")->AsString = discountGroup;
        selectOrderKeysWithSCDOrPWDDiscount->ExecQuery();

        for (; !selectOrderKeysWithSCDOrPWDDiscount->Eof  ;selectOrderKeysWithSCDOrPWDDiscount->Next())
        {
            orderKeysWithSCDOrPWDDiscount.insert(selectOrderKeysWithSCDOrPWDDiscount->FieldByName("ORDER_KEY")->AsInteger);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//--------------------------------------------------------------------------------------------------------------------
void TDBOrder::LoadOrderKeysWIthoutDiscount(Database::TDBTransaction &DBTransaction, int tabKey, std::set<__int64> &orderKeysWithoutDiscount)
{
    try
    {
        TIBSQL *selectOrderKeysWithoutDiscount = DBTransaction.Query(DBTransaction.AddQuery());
        selectOrderKeysWithoutDiscount->Close();
        selectOrderKeysWithoutDiscount->SQL->Clear();
        selectOrderKeysWithoutDiscount->SQL->Text = "SELECT a.ORDER_KEY  "
                                                    "FROM ORDERS a "
                                                    "WHERE a.ORDER_KEY NOT IN (SELECT a.ORDER_KEY "
                                                                            "FROM ORDERS a "
                                                                            "INNER JOIN ORDERDISCOUNTS OD "
                                                                                "ON a.ORDER_KEY = OD.ORDER_KEY "
                                                                            "LEFT JOIN DISCOUNTGROUPS_DISCOUNTTYPES DGD "
                                                                                "ON OD.DISCOUNT_KEY = DGD.DISCOUNTTYPE_KEY "
                                                                            "LEFT JOIN DISCOUNT_GROUPS DG "
                                                                                "ON DGD.DISCOUNTGROUPS_KEY = DG.DISCOUNTGROUPS_KEY "
                                                                            "WHERE a.TAB_KEY = :TAB_KEY  "
                                                                            ") AND a.TAB_KEY = :TAB_KEY "
                                                "GROUP by 1 "
                                                "ORDER BY a.ORDER_KEY ";

        selectOrderKeysWithoutDiscount->ParamByName("TAB_KEY")->AsString = tabKey;
        selectOrderKeysWithoutDiscount->ExecQuery();

        for (; !selectOrderKeysWithoutDiscount->Eof  ;selectOrderKeysWithoutDiscount->Next())
        {
            orderKeysWithoutDiscount.insert(selectOrderKeysWithoutDiscount->FieldByName("ORDER_KEY")->AsInteger);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------------
void TDBOrder::SortOrders(TList * Orders)
{
    Orders->Sort(&SortByFinalPrice);
}
//-----------------------------------------------------------------------------
void TDBOrder::DeleteOrdersForreatructure(TList *Orders)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    UnicodeString orderKeysList = getOrderKeysList(Orders);
    try
    {
        TIBSQL *deleteQuery = DBTransaction.Query(DBTransaction.AddQuery());
        deleteQuery->Close();
        deleteQuery->SQL->Clear();
        deleteQuery->SQL->Text =
                                    "DELETE FROM ORDERS a WHERE "
                                    "a.ORDER_KEY in (" + orderKeysList + ") ";
        deleteQuery->ExecQuery();
        DBTransaction.Commit();
    }
    catch(Exception &Ex)
    {
       MessageBox(Ex.Message,"Exception",MB_OK);
       DBTransaction.Rollback();
       TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//-----------------------------------------------------------------------------
UnicodeString TDBOrder::getOrderKeysList(TList *Orders)
{
    UnicodeString orderKeyList = "";
    for(int index = 0; index < Orders->Count; index++)
    {
        TItemComplete* ic = (TItemComplete*)Orders->Items[index];
        if(orderKeyList == "")
            orderKeyList = ic->OrderKey;
        else
            orderKeyList += ", " + IntToStr(ic->OrderKey);
    }
    return orderKeyList;
}
//-----------------------------------------------------------------------------
void TDBOrder::SetMemberEmailLoyaltyKeyForTable(Database::TDBTransaction &DBTransaction,int tableNumber,int loyaltyKey, UnicodeString email)
{
    try
    {   
       TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = "UPDATE ORDERS SET LOYALTY_KEY =:LOYALTY_KEY, EMAIL = :EMAIL "
                                      " WHERE TABLE_NUMBER=:TABLE_NUMBER";

       IBInternalQuery->ParamByName("TABLE_NUMBER")->AsInteger = tableNumber;
       IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = loyaltyKey;
       IBInternalQuery->ParamByName("EMAIL")->AsString = email;
       IBInternalQuery->ExecQuery();
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//-----------------------------------------------------------------------------
void TDBOrder::SetMemberEmailLoyaltyKeyForTab(Database::TDBTransaction &DBTransaction,int tabKey,int loyaltyKey, UnicodeString email)
{
    try
    {
       TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
       IBInternalQuery->Close();
       IBInternalQuery->SQL->Text = "UPDATE ORDERS SET LOYALTY_KEY =:LOYALTY_KEY, EMAIL = :EMAIL "
                                      " WHERE TAB_KEY=:TAB_KEY";

       IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = tabKey;
       IBInternalQuery->ParamByName("LOYALTY_KEY")->AsInteger = loyaltyKey;
       IBInternalQuery->ParamByName("EMAIL")->AsString = email;
       IBInternalQuery->ExecQuery();
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
