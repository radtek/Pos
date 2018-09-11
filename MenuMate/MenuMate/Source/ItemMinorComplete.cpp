// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ItemMinorComplete.h"
#include <algorithm>
#include "rounding.h"
#include "ItemCompleteSub.h"
#include "math.h"
#include "GlobalSettings.h";
// ---------------------------------------------------------------------------

#include "Item.h"
#include "DeviceRealTerminal.h"
#include "ItemSize.h"
#include <cassert>
#include <i_item_definition.hh>
#include <i_item_manager.hh>
#include <item_management_booter.hh>

#include "db_item_size.h"

#include "IBillCalculator.h"

#pragma package(smart_init)

const TItemMinorComplete::identity_predicate
  TItemMinorComplete::identity_predicate_;

using item_management::i_item_manager;
using item_management::i_item_definition;

int TItemMinorComplete::DefaultPatronCount()
const
{
        return db_item_size_t::default_patron_count(ItemKey, Size);
}

int TItemMinorComplete::PatronCount()
const
{
        return patron_count_;
}

int TItemMinorComplete::PatronCount(
    const int new_count,
    const bool scale_by_quantity)
{
        const Currency scaled_count =
            Currency(new_count) * Currency(Qty);

        if (scale_by_quantity == false)
                return patron_count_ = new_count;

        return patron_count_ = (static_cast<int>(scaled_count)
                                + (scaled_count % 1 != 0));
}

void TItemMinorComplete::SetPatronCount(int new_count)
{
    patron_count_ = new_count;
}

void TItemMinorComplete::RunBillCalculator()
{
    BillCalculator::TMMBillCalculatorAdapter billCalcAdapter;
    billCalcAdapter.CalculateOrderItemPrices(this);
}

double TItemMinorComplete::GetAvailableQuantity()
const
{
   i_item_manager &im = item_management::kickstarter::get_item_manager();

   return im.get_available_quantity(
              ItemKey, im.translate_size_name_to_key(ItemKey, Size));
}

bool TItemMinorComplete::ClaimAvailability(
  double n_units,
  std::vector<UnicodeString> *items_that_couldnt_be_acquired)
{
   i_item_manager &im = item_management::kickstarter::get_item_manager();
   std::vector<TItemMinorComplete *> claimed;
   std::vector<UnicodeString> dummy;
   std::vector<TItemMinorComplete *> unclaimed;
   std::vector<TItemMinorComplete *> waiting_to_be_processed =
     flatten_item_tree(identity_predicate_);

   if (items_that_couldnt_be_acquired == NULL)
       items_that_couldnt_be_acquired = &dummy;
   items_that_couldnt_be_acquired->clear();

   for (std::vector<TItemMinorComplete *>::iterator i =
            waiting_to_be_processed.begin();
        i != waiting_to_be_processed.end(); ++i) {
      if (im.claim_units(
            (*i)->ItemKey,
            im.translate_size_name_to_key((*i)->ItemKey, (*i)->Size),
            n_units)) {
          claimed.push_back(*i);
          continue;
      }
      items_that_couldnt_be_acquired->push_back(
        (*i)->Item
        + " ("
        + (*i)->Size
        + ") "
        + UnicodeString::FloatToStrF((*i)->GetQty()
                                     + (*i)->GetAvailableQuantity(),
                                     UnicodeString::sffFixed,
                                     4,
                                     2));
      unclaimed.push_back(*i);
   }

   if (unclaimed.size() == 0)
       return true;

   for (std::vector<TItemMinorComplete *>::iterator j = claimed.begin();
        j != claimed.end(); ++j)
      im.release_units(
        (*j)->ItemKey,
        im.translate_size_name_to_key((*j)->ItemKey, (*j)->Size),
        n_units);

   return false;
}

std::vector<const TItemMinorComplete *> TItemMinorComplete::flatten_item_tree(const i_flatten_predicate &predicate)
const
{
	std::vector<const TItemMinorComplete *> items;

	if (predicate(*this))
		items.push_back(this);

	for (unsigned long i = 0, j = SubOrders->Count; i < j; i++) {
		const TItemMinorComplete *suborder = SubOrders->SubOrderGet(i);
		if (predicate(*suborder))
			items.push_back(suborder);
	}

	return items;
}

std::vector<TItemMinorComplete *> TItemMinorComplete::flatten_item_tree(const i_flatten_predicate &predicate)
{
	std::vector<TItemMinorComplete *> items;

	if (predicate(*this))
		items.push_back(this);

	for (unsigned long i = 0, j = SubOrders->Count; i < j; i++) {
		TItemMinorComplete *suborder = SubOrders->SubOrderGet(i);

		if (predicate(*suborder))
			items.push_back(suborder);
	}

	return items;
}

void ShallowReturnToAvailability(
  item_key_type item_key,
  const string_type &size_name,
  double n_units = 1.0)
{
        i_item_manager &im = item_management::kickstarter::get_item_manager();

        im.release_units(item_key,
                         im.translate_size_name_to_key(item_key, size_name),
                         n_units);
}

void TItemMinorComplete::ReturnToAvailability(
  double n_units)
{
   std::vector<TItemMinorComplete *> items =
     flatten_item_tree(identity_predicate_);
   i_item_manager &im = item_management::kickstarter::get_item_manager();

   for (std::vector<TItemMinorComplete *>::iterator i = items.begin();
        i != items.end(); ++i) {
        if((*i)->ItemKey!=0)
      im.release_units((*i)->ItemKey,
                       im.translate_size_name_to_key((*i)->ItemKey, (*i)->Size),
                       n_units);
   }
}

TItemMinorComplete::TItemMinorComplete() {
	SubOrders = new TListSubOrderContainer;
	Security = new TListSecurityRefContainer;
	Categories = new TListCategoryContainer;
	SalesRecipesToApply = new TListRecipeContainer;
	WastageRecipesToApply = new TListRecipeContainer;
    RemovedTaxes = new TStringList;
	HappyHour = false;
	Loyalty_Key = 0;
	Item_ID = 0;
	OrderKey = 0;
	OrderType = NormalOrder;
	TimeStamp = Now();
	TransNo = "";
	OrderTypeMessage = "";
	ContactsKey = -1;
    is_being_returned_to_stock_ = false;
    AmountRedeemed = 0;
    PointRedeemed = 0;
    PrintCancel = false;
    DelayedInvoiceNumber = "";
    IsSide = false;
    OrderIdentificationNo = 0;
    LastAddedItem = false;
    IsPayByPoints = false;
    ItemPriceForPoints = 0;
    ItemPriceForPointsOriginal = 0;
    printFreeSideForKitchen = false;
    printFreeSideForReceipt = false;
    wasOpenItem = false;
    SubItemType = eFoodItem;
    RoomNo = 0;
    AccNo = "";
    FirstName = "";
    LastName = "";
    TabKey = 0;
    RoomNoStr = "";
    SplitMessage = "";
    OnlineChitType = 0;
    OnlineOrderId = "";
    SiteId = 0;
    OrderItemId = 0;
    OrderItemSizeId = 0;
    ReferenceOrderItemSizeId = 0;
    Email = "";
}

__fastcall TItemMinorComplete::~TItemMinorComplete() {
	delete SubOrders;
	delete Security;
	delete Categories;
	delete SalesRecipesToApply;
	delete WastageRecipesToApply;
}

TItemMinorComplete::TItemMinorComplete(
  const TItemMinorComplete &initializer)
  : TItemMinor(initializer)
{
    SubOrders = new TListSubOrderContainer;
	Security = new TListSecurityRefContainer;
	Categories = new TListCategoryContainer;
	SalesRecipesToApply = new TListRecipeContainer;
	WastageRecipesToApply = new TListRecipeContainer;
    RemovedTaxes = new TStringList;

    TransNo = initializer.TransNo;
    Item_ID = initializer.Item_ID;
    Loyalty_Key = initializer.Loyalty_Key;
    OrderKey = initializer.OrderKey;
    HappyHour = initializer.HappyHour;
    Discounts = initializer.Discounts;
    TimeStamp = initializer.TimeStamp;
    Security = new TListSecurityRefContainer();
    SubOrders = new TListSubOrderContainer();
    Categories = new TListCategoryContainer();
    SalesRecipesToApply = new TListRecipeContainer();
    WastageRecipesToApply = new TListRecipeContainer();
    RemovedTaxes = initializer.RemovedTaxes;
    Security->SetSecurityRefNumber(initializer.Security->GetSecurityRefNumber());
    Security->SecurityCopyList(initializer.Security);
    SubOrders->SubOrderCopyList(initializer.SubOrders);
    SalesRecipesToApply->RecipeCopyList(initializer.SalesRecipesToApply);
    WastageRecipesToApply->RecipeCopyList(initializer.WastageRecipesToApply);
    Categories->CategoryCopyList(initializer.Categories);
    OrderTypeMessage = initializer.OrderTypeMessage;
    ContactsKey = initializer.ContactsKey;
    is_being_returned_to_stock_ = initializer.is_being_returned_to_stock_;
    BillCalcResult = initializer.BillCalcResult;
    CancelledBillCalcResult = initializer.CancelledBillCalcResult;
    AmountRedeemed = initializer.AmountRedeemed;
    PointRedeemed = initializer.PointRedeemed;
    PrintCancel =  initializer.PrintCancel; //MM-4327
    DelayedInvoiceNumber =  initializer.DelayedInvoiceNumber;
    OrderIdentificationNo = initializer.OrderIdentificationNo;
    LastAddedItem = initializer.LastAddedItem; //
    IsPayByPoints = initializer.IsPayByPoints; //
    ItemPriceForPoints = initializer.ItemPriceForPoints;
    ItemPriceForPointsOriginal = initializer.ItemPriceForPointsOriginal;
    SubItemType = initializer.SubItemType;
    RoomNo = initializer.RoomNo;
    AccNo = initializer.AccNo;
    FirstName = initializer.FirstName;
    LastName = initializer.LastName;
    TabKey = initializer.TabKey;
    RoomNoStr = initializer.RoomNoStr;
    SplitMessage = initializer.SplitMessage;
    OnlineChitType = initializer.OnlineChitType;
    OnlineOrderId = initializer.OnlineOrderId;
    SiteId = initializer.SiteId;
    OrderItemId = initializer.OrderItemId;
    OrderItemSizeId = initializer.OrderItemSizeId;
    ReferenceOrderItemSizeId = initializer.ReferenceOrderItemSizeId;
    Email = initializer.Email;
}

TItemMinorComplete &TItemMinorComplete::operator=(const TItemMinorComplete &rhs)
{
    if (this == &TItemMinor::operator=(rhs))
    {
        return *this;
    }

   	SubOrders = new TListSubOrderContainer;
	Security = new TListSecurityRefContainer;
	Categories = new TListCategoryContainer;
	SalesRecipesToApply = new TListRecipeContainer;
	WastageRecipesToApply = new TListRecipeContainer;
    RemovedTaxes = new TStringList;

    TransNo = rhs.TransNo;
    Item_ID = rhs.Item_ID;
    Loyalty_Key = rhs.Loyalty_Key;
    OrderKey = rhs.OrderKey;
    HappyHour = rhs.HappyHour;
    Discounts = rhs.Discounts;
    TimeStamp = rhs.TimeStamp;
    RemovedTaxes = rhs.RemovedTaxes;
    Security->SetSecurityRefNumber(rhs.Security->GetSecurityRefNumber());
    Security->SecurityCopyList(rhs.Security);
    SubOrders->SubOrderCopyList(rhs.SubOrders);
    SalesRecipesToApply->RecipeCopyList(rhs.SalesRecipesToApply);
    WastageRecipesToApply->RecipeCopyList(rhs.WastageRecipesToApply);
    Categories->CategoryCopyList(rhs.Categories);
    OrderTypeMessage = rhs.OrderTypeMessage;
    ContactsKey = rhs.ContactsKey;
    is_being_returned_to_stock_ = rhs.is_being_returned_to_stock_;
    BillCalcResult = rhs.BillCalcResult;
    CancelledBillCalcResult = rhs.CancelledBillCalcResult;
    AmountRedeemed = rhs.AmountRedeemed;
    PointRedeemed = rhs.PointRedeemed;
    PrintCancel = rhs.PrintCancel;
    DelayedInvoiceNumber =  rhs.DelayedInvoiceNumber;
    OrderIdentificationNo = rhs.OrderIdentificationNo;
    LastAddedItem = rhs.LastAddedItem;
    IsPayByPoints = rhs.IsPayByPoints; //
    ItemPriceForPoints = rhs.ItemPriceForPoints;
    ItemPriceForPointsOriginal = rhs.ItemPriceForPointsOriginal;
    SubItemType = rhs.SubItemType;
    RoomNo = rhs.RoomNo;
    AccNo = rhs.AccNo;
    FirstName = rhs.FirstName;
    LastName = rhs.LastName;
    TabKey = rhs.TabKey;
    RoomNoStr = rhs.RoomNoStr;
    SplitMessage = rhs.SplitMessage;
    OnlineChitType = rhs.OnlineChitType;
    OnlineOrderId = rhs.OnlineOrderId;
    SiteId = rhs.SiteId;
    OrderItemId = rhs.OrderItemId;
    OrderItemSizeId = rhs.OrderItemSizeId;
    ReferenceOrderItemSizeId = rhs.ReferenceOrderItemSizeId;
    Email = rhs.Email;
}

void TItemMinorComplete::Assign(TItemMinor * BaseItem)
{
    TItemMinor::Assign(BaseItem);

    TItemMinorComplete *RetItem = dynamic_cast<TItemMinorComplete*>(BaseItem);
    if (RetItem != NULL)
    {
       	RetItem->SubOrders = new TListSubOrderContainer;
	    RetItem->Security = new TListSecurityRefContainer;
     	RetItem->Categories = new TListCategoryContainer;
	    RetItem->SalesRecipesToApply = new TListRecipeContainer;
	    RetItem->WastageRecipesToApply = new TListRecipeContainer;
        RetItem->RemovedTaxes = new TStringList;


        RetItem->TransNo = TransNo;
        RetItem->Item_ID = Item_ID;
        RetItem->Loyalty_Key = Loyalty_Key;
        RetItem->OrderKey = OrderKey;
        RetItem->HappyHour = HappyHour;
        RetItem->Discounts = Discounts;
        RetItem->TimeStamp = TimeStamp;

        RetItem->Security->SetSecurityRefNumber(Security->GetSecurityRefNumber());
        RetItem->Security->SecurityCopyList(Security);
        RetItem->SubOrders->SubOrderCopyList(SubOrders);
        RetItem->SalesRecipesToApply->RecipeCopyList(SalesRecipesToApply);
        RetItem->WastageRecipesToApply->RecipeCopyList(WastageRecipesToApply);
        RetItem->Categories->CategoryCopyList(Categories);
        RetItem->OrderTypeMessage = OrderTypeMessage;
        RetItem->ContactsKey = ContactsKey;
        RetItem->is_being_returned_to_stock_ = is_being_returned_to_stock_;
        RetItem->BillCalcResult = BillCalcResult;
        RetItem->CancelledBillCalcResult = CancelledBillCalcResult;
        RetItem->AmountRedeemed = AmountRedeemed;
        RetItem->PointRedeemed = PointRedeemed;
        RetItem->PrintCancel = PrintCancel;
        RetItem->DelayedInvoiceNumber =  DelayedInvoiceNumber;
        RetItem->OrderIdentificationNo = OrderIdentificationNo;
        RetItem->LastAddedItem = LastAddedItem;
        RetItem->IsPayByPoints = IsPayByPoints; //
        RetItem->ItemPriceForPoints = ItemPriceForPoints;
        RetItem->ItemPriceForPointsOriginal = ItemPriceForPointsOriginal;
        RetItem->SubItemType = SubItemType;
        RetItem->RoomNo = RoomNo;
        RetItem->AccNo = AccNo;
        RetItem->FirstName = FirstName;
        RetItem->LastName = LastName;
        RetItem->TabKey = TabKey;
        RetItem->RoomNoStr = RoomNoStr;
        RetItem->SplitMessage = SplitMessage;
        RetItem->OnlineChitType = OnlineChitType;
        RetItem->OnlineOrderId = OnlineOrderId;
        RetItem->SiteId = SiteId;
        RetItem->OrderItemId = OrderItemId;
        RetItem->OrderItemSizeId = OrderItemSizeId;
        RetItem->ReferenceOrderItemSizeId = ReferenceOrderItemSizeId;
        RetItem->Email = Email;
    }
}

Currency TItemMinorComplete::TotalAdjustment() const
{
    return TotalAdjustment_BillCalc();
}

Currency TItemMinorComplete::TotalAdjustmentExcCombo() const
{
	// Sort the discounts by priority before they are applied.
	std::sort(Discounts.begin(), Discounts.end());

	Currency RetVal = 0;
	// Add up the Total Value.
	Currency OrdersTotal = Price();
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
		ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
        {
		Currency CurrentDiscountValue = 0;
		TDiscount CurrentDiscount = *ptrDiscount;
		if ( DiscountApplies(CurrentDiscount)
             && CurrentDiscount.Mode != DiscModeCombo
             && CurrentDiscount.Mode != DiscModeDeal)
             {

			if (CurrentDiscount.Mode == DiscModePercent)
            {
                CurrentDiscountValue = -CurrentDiscount.PercentAmount*double
                     (OrdersTotal) / double(100.0);
                CurrentDiscountValue = RoundToNearest(CurrentDiscountValue,
                    CurrentDiscount.Rounding,
                    TGlobalSettings::Instance().MidPointRoundsDown);
			}
			else if (CurrentDiscount.Mode == DiscModeSetPrice || CurrentDiscount.Mode == DiscModeCombo || CurrentDiscount.Mode == DiscModeDeal)
            {
				   CurrentDiscountValue = RoundToNearest(CurrentDiscount.Amount - OrdersTotal,CurrentDiscount.Rounding,	TGlobalSettings::Instance().MidPointRoundsDown);
			}
			else if (CurrentDiscount.Mode == DiscModeCurrency)
            {
				if (CurrentDiscount.Amount > OrdersTotal && ! (OrderType == CreditNonExistingOrder))
                {
					CurrentDiscountValue = -OrdersTotal;
				}
				else
                {
					CurrentDiscountValue = -CurrentDiscount.Amount;
				}
			}
              //************4278*********************/
            	else if (CurrentDiscount.Mode == DiscModeItem)
            {
		     	CurrentDiscountValue = RoundToNearest
					 (CurrentDiscount.Amount - OrdersTotal,
					CurrentDiscount.Rounding,
					TGlobalSettings::Instance().MidPointRoundsDown);


			}
              //************4278*********************/
			// All future discounts will be based on the reduced price.
			OrdersTotal += CurrentDiscountValue;
			RetVal += CurrentDiscountValue;
		}
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalSystemDiscount() const
{
	std::sort(Discounts.begin(), Discounts.end());

	Currency CurrentDiscountValue = 0;
	Currency OrdersTotal = Price();
	Currency TotalDiscount = 0;

	for (std::vector <TDiscount> ::iterator ptrDiscount = Discounts.begin();
	     ptrDiscount != Discounts.end(); ++ptrDiscount) {
		CurrentDiscountValue = 0;
		if (ptrDiscount->Source >= dsMMSystem) {
			if (DiscountApplies(*ptrDiscount)) {
				CurrentDiscountValue =
				  ptrDiscount->DiscountValue(OrdersTotal, OrderType);

				OrdersTotal += CurrentDiscountValue;
				TotalDiscount += CurrentDiscountValue;
			}
		}
	}
	return TotalDiscount;
}

Currency TItemMinorComplete::TotalUserDiscount() const
{
	// Sort the discounts by priority before they are applied.
	std::sort(Discounts.begin(), Discounts.end());

	// Add up the Total Value.
	/* System discount always have a higher priority than User discounts and the User
	discount needs to take system Discounts into account in there calculation
	incase for instance the item has been split or partially paid. */

	/* This may mean it is now poss to get a negitivly priced item (high $ Discount + split) this will need to be lokoed at
	very carefully */

	Currency CurrentDiscountValue = 0;
	Currency OrdersTotal = Price() + TotalSystemDiscount();
	Currency TotalDiscount = 0;

	for (std::vector <TDiscount> ::iterator ptrDiscount = Discounts.begin();
	     ptrDiscount != Discounts.end(); ++ptrDiscount) {
		CurrentDiscountValue = 0;
		if (ptrDiscount->Source < dsMMSystem) {
			if (DiscountApplies(*ptrDiscount)) {
				CurrentDiscountValue =
				  ptrDiscount->DiscountValue(OrdersTotal, OrderType);

				// All future discounts will be based on the reduced price.
				OrdersTotal += CurrentDiscountValue;
				TotalDiscount += CurrentDiscountValue;
			}
		}
	}
	return TotalDiscount;
}

Currency TItemMinorComplete::TotalAdjustmentSides() const
{
    return TotalAdjustmentSides_BillCalc();
}

Currency TItemMinorComplete::TotalSystemDiscountSides() const
{
	Currency RetVal = 0;
	RetVal += TotalSystemDiscount();
	for (int j = 0; j < SubOrders->Count; j++) {
		TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
		RetVal += SubOrder->TotalSystemDiscount();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceAdjustmentSides() const
{
    return TotalPriceAdjustmentSides_BillCalc();
}

Currency TItemMinorComplete::TotalPriceSides()const
{
    return TotalPriceSides_BillCalc();
}

Currency TItemMinorComplete::TotalPriceRedeemedExclGST()const
{
	Currency RetVal = 0;
	Currency OrderTotal = TotalPriceRedeemed();
	if (OrderTotal != 0) {
//		RetVal += RoundToNearest(((OrderTotal * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceRedeemedGSTContent()const {
	Currency RetVal = 0;
	Currency OrderTotal = TotalPriceRedeemed();
	if (OrderTotal != 0) {
//		RetVal += RoundToNearest
//			 (OrderTotal - ((OrderTotal * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceRedeemedSidesGSTContent()const {
	Currency RetVal = 0;
	Currency OrderTotal = TotalPriceRedeemed();
	if (OrderTotal != 0) {
//		RetVal += RoundToNearest
//			 (OrderTotal - ((OrderTotal * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}

	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		Currency SubOrderTotal = SubOrderImage->TotalPriceRedeemed();
		if (SubOrderTotal != 0) {
//			RetVal += RoundToNearest
//				 (SubOrderTotal - ((SubOrderTotal * 100) /
//					(SubOrderImage->GSTPercent + 100.0)), MIN_CURRENCY_VALUE,
//				TGlobalSettings::Instance().MidPointRoundsDown);
		}
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalAdjustmentExcGST()
const
{
	Currency RetVal = TotalSurchargeExcGST();
	return RetVal;
}

Currency TItemMinorComplete::TotalAdjustmentGSTContent()
const
{
	Currency RetVal = TotalSurchargeGSTContent();
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceRedeemed()const
{
	Currency RetVal = 0;
	RetVal = Price() - Redeemed;
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceAdjustment() const
{
    return TotalPriceAdjustment_BillCalc();
}

Currency TItemMinorComplete::TotalPriceAdjustmentExcCombo() const
{
	Currency RetVal = 0;
	RetVal = Price() + TotalAdjustmentExcCombo();
	return RetVal;
}

Currency TItemMinorComplete::GrandTotal() const
{
    return GrandTotal_BillCalc();

}

Currency TItemMinorComplete::GrandTotalExcCombo() const
{
	Currency RetVal = 0;
	RetVal = TotalPriceAdjustmentExcCombo() - Redeemed;
	return RetVal;
}

Currency TItemMinorComplete::GrandTotalExclGST() const
{
	Currency RetVal = 0;
	Currency OrderTotal = GrandTotal();
	if (OrderTotal != 0) {
//		RetVal += RoundToNearest(((OrderTotal * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
		RetVal += (OrderTotal * 100) / (GSTPercent + 100.0);
	}
	return RetVal;
}

Currency TItemMinorComplete::GrandTotalSides() const
{
	Currency RetVal = 0;
	RetVal = GrandTotal();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->GrandTotal();
	}
	return RetVal;
}

Currency TItemMinorComplete::GrandTotalGSTContent() const          //todo: needs to be modified accordingly : used in POS_XMLTransaction and XeroInvoiceBuilder
{
	Currency RetVal = 0;
	Currency OrderTotal = GrandTotal();
	if (OrderTotal != 0) {
//		RetVal += RoundToNearest
//			 (OrderTotal - ((OrderTotal * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
		RetVal +=
			 OrderTotal - ((OrderTotal * 100) / (GSTPercent + 100.0));
	}
	return RetVal;
}

Currency TItemMinorComplete::GrandTotalGSTContentSides() const
{
    return GrandTotalGSTContentSides_BillCalc();
}

double TItemMinorComplete::GetGST() const
{
	return 0;//GSTPercent;
}

Currency TItemMinorComplete::GrandTotalExclGSTSides() const
{
    return GrandTotalExclGSTSides_BillCalc();
}

Currency TItemMinorComplete::TotalAdjustmentSidesExcGST() const
{
	Currency RetVal = 0;
	RetVal = TotalAdjustmentExcGST();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalAdjustmentExcGST();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalAdjustmentSidesGSTContent() const
{
	Currency RetVal = 0;
	RetVal = TotalAdjustmentGSTContent();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalAdjustmentGSTContent();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceRedeemedSides() const
{
	Currency RetVal = 0;
	RetVal = TotalPriceRedeemed();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalPriceRedeemed();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalPriceAdjustmentSidesExclGST() const
{
    return TotalPriceAdjustmentSidesExclGST_BillCalc();
}

AnsiString TItemMinorComplete::DiscountReasons()
{
	// Sort the discounts by priority before they are applied.
	AnsiString DiscountReasons = "";
	std::sort(Discounts.begin(), Discounts.end());
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
		ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1)) {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (DiscountApplies(CurrentDiscount)) {
			if (CurrentDiscount.Description != "") {
				DiscountReasons += " " + CurrentDiscount.Description;
			}
			else {
				DiscountReasons += " " + CurrentDiscount.Name;
			}
		}
	}
	return DiscountReasons;
}

void TItemMinorComplete::Cancel(bool ReturnStock)
{
   OrderStatus original_order_type = OrderType;
   BillCalculator::TMMBillCalculatorAdapter billCalcAdapter;
   billCalcAdapter.CalculateOrderItemPrices(this);
   CancelledBillCalcResult = BillCalcResult;

	OrderType = CanceledOrder;
	ClearPrice();
	Discounts.clear();

	if (ReturnStock)
	{
		if (original_order_type == NormalOrder)
		{
            // Check the item key here for invalids. This ItemKey is the key from items table and if the item is deleted this key can be zero which we ignore.
            if(ItemKey > 0)
            {
    			ShallowReturnToAvailability(ItemKey, Size, GetQty());
            }
		}
		SalesRecipesToApply->Inverse();
		SalesRecipesToApply->Enable();
		SalesRecipesToApply->AdjustmentType = eStockSale;
	}
	else
	{
		WastageRecipesToApply->RecipeCopyList(SalesRecipesToApply);
		WastageRecipesToApply->Enable();
		WastageRecipesToApply->AdjustmentType = eStockWriteOffs;

		SalesRecipesToApply->Enable();
		SalesRecipesToApply->Inverse();
		SalesRecipesToApply->AdjustmentType = eStockSale;
	}
}

bool TItemMinorComplete::IsBeingReturnedToStock()
const
{
        return OrderType == CreditNonExistingOrder
                            && is_being_returned_to_stock_;
}

#pragma warning -wpia

void TItemMinorComplete::Credit(bool dont_writeoff_stock)
{
        CreditQty();
	OrderType = CreditNonExistingOrder;
	Cost = -Cost;
	DiscountCredit();

        if ((is_being_returned_to_stock_ = !dont_writeoff_stock))
        {
		WastageRecipesToApply->RecipeCopyList(SalesRecipesToApply);
		WastageRecipesToApply->AdjustmentType = eStockRefund;
	}
        else
        { WastageRecipesToApply->RecipeCopyList(SalesRecipesToApply);
          WastageRecipesToApply->AdjustmentType = eStockWriteOffs;
          Cost=0;      //when we are not returning item to stock it means we are not getting it's cost so make it to zeo.

        }
	SalesRecipesToApply->Inverse();
	SalesRecipesToApply->Disable();
	SalesRecipesToApply->AdjustmentType = eStockSale;
}

#pragma warning .wpia

void TItemMinorComplete::ReverseCredit() {
	ResetQty();
        OrderType = NormalOrder;
	Cost = fabs(Cost);
	DiscountCredit();

	SalesRecipesToApply->Inverse();
	SalesRecipesToApply->Enable();
	SalesRecipesToApply->AdjustmentType = eStockSale;
	WastageRecipesToApply->RecipeClear();
}

void TItemMinorComplete::SetPriceLevel0()
{
	ClearPrice();
	TMMPrice Price(PriceLevel0, TIMCPriceLevel0, amSetPrice, psMMDB, 0);
	SetPrice(Price);
}

void TItemMinorComplete::SetPriceLevel1()
{
	ClearPrice();
	TMMPrice Price(PriceLevel1, TIMCPriceLevel1, amSetPrice, psMMDB, 0);
	SetPrice(Price);
}

void TItemMinorComplete::SetPriceLevelCustom()
{
	ClearPrice();
	TMMPrice Price(PriceLevelCustom, TIMCPriceLevelCustom, amSetPrice, psMMDB,0);
	SetPrice(Price);
	if (WeightedPrice.WeightedItem)
    {
		WeightedPrice.Assign(PriceLevelCustom);
		SetQty(WeightedPrice.Weight.GetWeight());
	}
}

void TItemMinorComplete::SetPriceLevelCustom(Currency NewPrice)
 {
	ClearPrice();
	PriceLevelCustom = NewPrice;
	TMMPrice Price(PriceLevelCustom, TIMCPriceLevelCustom, amSetPrice, psMMDB,0);
	SetPrice(Price);
	if (WeightedPrice.WeightedItem)
    {
		WeightedPrice.Assign(NewPrice);
		SetQty(WeightedPrice.Weight.GetWeight());
	}
}

void TItemMinorComplete::SetWeight(TWeight &inWeight)
{
	WeightedPrice.Assign(inWeight, Price());
	SetQty(WeightedPrice.Weight.GetWeight());
}

void TItemMinorComplete::ResetPrice() {

	if (SetMenuGroup != 0 && !SetMenuMaster && SetMenu) {
		SetPriceLevelCustom(0);
	}
	else if (FPrice.PriceLevel == TIMCPriceLevelCustom) {
		SetPriceLevelCustom();
	}
	else if (HappyHour) {
		if (TGlobalSettings::Instance().HappyHourMembersOnly) {
			if (Loyalty_Key != 0) {
				SetPriceLevel1();
			}
			else {
				SetPriceLevel0();
			}
		}
		else {
			SetPriceLevel1();
		}
	}
	else {
		SetPriceLevel0();
	}

	// If this Order is being Credited then correct the Price "Sign"
	if (OrderType == CreditNonExistingOrder) {
		CreditQty();
	}

	DiscountRemoveInvalid();
}

void TItemMinorComplete::SetQtyCustom(
    const double inQty,
    const bool scale_patron_count)  //MM-6306 : double data type in place of currency
{
        std::vector<TItemMinorComplete *> items =
            flatten_item_tree(identity_predicate_);

        TItemMinor::SetQty(inQty);

        PatronCount(DefaultPatronCount(), scale_patron_count);

        if (WeightedPrice.WeightedItem == true)
        {

          return;
        }
        for (std::vector<TItemMinorComplete *>::iterator i = items.begin() + 1;
                 i != items.end(); ++i)
                (*i)->SetQty(inQty);
}

void TItemMinorComplete::SetQty(
    const double inQty,
    const bool scale_patron_count)  //MM-6306 : double data type in place of currency
{
	if (FPrice.PriceLevel != TIMCPriceLevelCustom)
		ResetPrice();

        SetQtyCustom(inQty, scale_patron_count);
}

bool TItemMinorComplete::Add(TItemMinorComplete *Item) {
	// TODO : Make this a proper add , Check to see if the items are addable and return false if not.
	Qty = Qty + Item->GetQty();
	return true;
}

void TItemMinorComplete::DiscountAdd(TDiscount &Discount)
{
	if (DiscountApplies(Discount))
    {
		Discounts.push_back(Discount);
		if (Discount.Description != "")
        {
			DiscountReason = Discount.Description;
		}
		else
        {
			DiscountReason = Discount.Name;
		}
        RunBillCalculator();
	}
}

bool TItemMinorComplete::DiscountApplies(const TDiscount &Discount) const
{
	bool RetVal = true;

    if(!Discount.CategoryFilterKeys.empty())
    {
    	if(!Discount.ContainsCatKey(Categories))
        {
        	RetVal = false;
        }
    }

	if (Discount.MembersExempt && Loyalty_Key != 0)
		RetVal = false;

	if (Discount.MembersOnly && Loyalty_Key == 0)
		RetVal = false;

    if(Discount.Mode == DiscModePoints && !IsCanBePaidForUsingPoints)
      return false;
	return RetVal;
}

void TItemMinorComplete::DiscountRemoveInvalid()
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
		ptrDiscount != Discounts.end(); )
    {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (!DiscountApplies(CurrentDiscount))
        {
			DiscountRemove(CurrentDiscount);
			ptrDiscount = Discounts.begin();
		}
		else
        {
			std::advance(ptrDiscount, 1);
		}
	}
}

void TItemMinorComplete::DiscountAddIncSides(TDiscount &Discount)
{
	if (DiscountApplies(Discount))
    {
        DiscountAdd(Discount);
		/* TODO : This might not be the discount funtionality they would expect. Test this with sides. */
		for (int j = 0; j < SubOrders->Count; j++)
         {
			TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
			if (Discount.Mode == DiscModeItem)
            {
				TDiscount SideDiscount = Discount;
				SideDiscount.Amount = 0;
				SubOrder->DiscountAdd(SideDiscount);
			}
			else
            {
                SubOrder->DiscountAdd(Discount);
			}
		}
	}
}

bool TItemMinorComplete::DiscountApplied(int DiscountKey)
{
	bool discount_present = false;
    int size = Discounts.size();
    for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
		ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
    {
        discount_present = ptrDiscount->DiscountKey == DiscountKey;
        if(discount_present)
           break;
    }
	return discount_present;
}

void TItemMinorComplete::DiscountCredit()
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
		ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
         {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (ptrDiscount->Mode == DiscModePercent)
        {
		}
		else if (ptrDiscount->Mode == DiscModeSetPrice)
         {
			ptrDiscount->Amount = -ptrDiscount->Amount;
		}
		else if (CurrentDiscount.Mode == DiscModeCurrency)
        {
			ptrDiscount->Amount = -ptrDiscount->Amount;
		}
		else if (ptrDiscount->Mode == DiscModeCombo)
         {
			ptrDiscount->Amount = -ptrDiscount->Amount;
		}
        else if (ptrDiscount->Mode == DiscModeDeal)
        {
            ptrDiscount->Amount = -ptrDiscount->Amount;
        }
         //************4278*********************/
         else if (ptrDiscount->Mode == DiscModeItem)
          {
            ptrDiscount->Amount = -ptrDiscount->Amount;
        }
          //************4278*********************/
	}
}

TDiscount TItemMinorComplete::DiscountFind(int DiscountKey)
{
	TDiscount ReturnDiscount;
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();	ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
    {
            TDiscount CurrentDiscount = *ptrDiscount;
            if (CurrentDiscount.DiscountKey == DiscountKey) {
                ReturnDiscount = *ptrDiscount;
		}
	}

	for (int j = 0; j < SubOrders->Count; j++)
    {
		TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
		ReturnDiscount = SubOrder->DiscountFind(DiscountKey);
	}
	return ReturnDiscount;
}

bool TItemMinorComplete::DiscountFind(TDiscountMode Mode)
{
	std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
	for (; ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
    {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.Mode == Mode)
        {
			return true;
		}
	}
	return false;
}

Currency TItemMinorComplete::DiscountGet(TDiscountMode Mode)
{
	Currency temp = 0;
	std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
	for (; ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
    {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.Mode == Mode)
        {
			temp += CurrentDiscount.ComboAmount;
		}
	}
	return temp;
}

void TItemMinorComplete::DiscountRemove(TDiscount &Discount)
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin(); ptrDiscount != Discounts.end(); )
    {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.DiscountKey == Discount.DiscountKey && (!CurrentDiscount.IsThorBill))
        {
            if(CurrentDiscount.Mode == DiscModePoints)
               ItemPriceForPoints = ItemPriceForPointsOriginal;
			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
		}
		else
        {
			ptrDiscount++;
		}
	}

	for (int j = 0; j < SubOrders->Count; j++)
    {
		TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
		SubOrder->DiscountRemove(Discount);
	}

    RunBillCalculator();
}

// Returns the Value of the current Discount.

Currency TItemMinorComplete::DiscountValue
	 (std::vector<TDiscount>::const_iterator &Discount)
{
	Currency current_balance = Price();
	Currency current_discount_value = 0;

	std::vector<TDiscount>::iterator i = DiscountsBegin();
	std::vector<TDiscount>::iterator j = DiscountsEnd();

	for (; i != j; ++i) {
		current_discount_value = 0;
		if (DiscountApplies(*i)) {
			current_discount_value =
			    i->DiscountValue(current_balance,
				             OrderType);
		}

		current_balance += current_discount_value;
		if (i == Discount)
			break;
	}

	return current_discount_value;
}

Currency TItemMinorComplete::DiscountValue(TDiscount Discount)
{
	const Currency current_balance = Price();
    Currency RetVal = 0;
	if (DiscountApplies(Discount))
    {
		RetVal = Discount.DiscountValue(current_balance, OrderType);
	}

	return RetVal;
}

void TItemMinorComplete::ThorVouchersDiscountsClear()
{

	ThorlinkDiscountByTypeLevelRemove(dsMMSystem);
	for (int j = 0; j < SubOrders->Count; j++)
    {
		TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
		SubOrder->ThorlinkDiscountByTypeLevelRemove(dsMMSystem);
	}
}

void TItemMinorComplete::ClearAllDiscounts()
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin(); ptrDiscount != Discounts.end(); )
	{
		TDiscount CurrentDiscount = *ptrDiscount;
        if(CurrentDiscount.Mode == DiscModePoints)
           ItemPriceForPoints = ItemPriceForPointsOriginal;
        if(CurrentDiscount.Mode != DiscModeDeal)
        {
            Discounts.erase(ptrDiscount);
            ptrDiscount = Discounts.begin();
            SelectedItems = 0;
            PrevSelectedItems = 0;
        }
        else
          std::advance(ptrDiscount, 1);
	}
    RunBillCalculator();
}

void TItemMinorComplete::DiscountsClear()
{
	//Discounts.clear();
	DiscountByTypeLevelRemove(dsMMSystem);
	for (int j = 0; j < SubOrders->Count; j++)
    {
		TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
		SubOrder->DiscountByTypeLevelRemove(dsMMSystem);
	}
}

void TItemMinorComplete::DiscountsClearByFilter(std::vector<TDiscountMode> exemptFilter)
{
   if(exemptFilter.empty())
   {
     DiscountsClear();
   }
   else
   {
     for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin(); ptrDiscount != Discounts.end(); )
	  {
		TDiscount CurrentDiscount = *ptrDiscount;
        bool processRemove = true;
        for (std::vector<TDiscountMode>::iterator ptrDiscountMode = exemptFilter.begin(); ptrDiscountMode != exemptFilter.end();std::advance(ptrDiscountMode,1))
          {
                if(CurrentDiscount.Mode == *ptrDiscountMode || CurrentDiscount.Source == dsMMLocationReward ||
                CurrentDiscount.Source == dsMMMembershipReward)
                {
                   processRemove = false;
                   break;
                }
          }

		if (processRemove)
		{
 			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
		}
		else
		{
			ptrDiscount++;
		}

        for (int j = 0; j < SubOrders->Count; j++)
        {
            TItemMinorComplete *SubOrder = (TItemMinorComplete*)SubOrders->Items[j];
            SubOrder->DiscountsClearByFilter(exemptFilter);
        }
	  }
    RunBillCalculator();
   }
}


void TItemMinorComplete::ThorlinkDiscountByTypeLevelRemove(TDiscountSource DiscountSource)
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin(); ptrDiscount != Discounts.end(); )
	{
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.IsThorBill)
		{
			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
			SelectedItems = 0;
			PrevSelectedItems = 0;
		}
		else
		{
			ptrDiscount++;
		}
	}

    RunBillCalculator();
}

void TItemMinorComplete::DiscountByTypeLevelRemove (TDiscountSource DiscountSource)
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin(); ptrDiscount != Discounts.end(); )
	{
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.Source <= DiscountSource)
		{
            if(CurrentDiscount.Mode == DiscModePoints)
               ItemPriceForPoints = ItemPriceForPointsOriginal;
			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
			SelectedItems = 0;
			PrevSelectedItems = 0;
		}
		else
		{
			ptrDiscount++;
		}
	}

    RunBillCalculator();
}

Currency TItemMinorComplete::TotalDiscount() const
{
	return TotalDiscount_BillCalc();
}

Currency TItemMinorComplete::TotalDiscountSides()
const
{
	return TotalDiscountSides_BillCalc();
}

Currency TItemMinorComplete::TotalSurcharge()const { // Returns the totals of the Discounts, excluding surcharges.
	return TotalSurcharge_BillCalc();
}

Currency TItemMinorComplete::TotalSurchargeGSTContent()const {
	Currency RetVal = 0;
	Currency Total = TotalSurcharge();
	if (Total != 0) {
//		RetVal += RoundToNearest(Total - ((Total * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalDiscountGSTContent()const {
	Currency RetVal = 0;
	Currency Total = TotalDiscount();
	if (Total != 0) {
//		RetVal += RoundToNearest(Total - ((Total * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	return RetVal;

}

Currency TItemMinorComplete::TotalSurchargeExcGST()const {
	Currency RetVal = 0;
	Currency Total = TotalSurcharge();
	if (Total != 0) {
//		RetVal += RoundToNearest(((Total * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalDiscountExcGST()const {
	Currency RetVal = 0;
	Currency Total = TotalDiscount();
	if (Total != 0) {
//		RetVal += RoundToNearest(((Total * 100) / (GSTPercent + 100.0)),
//			MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalSurchargeSides()const {
	return TotalSurchargeSides_BillCalc();
}

Currency TItemMinorComplete::TotalSurchargeSidesExcGST()const {
	Currency RetVal = 0;
	RetVal = TotalSurchargeExcGST();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalSurchargeExcGST();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalDiscountSidesExcGST()const {
	Currency RetVal = 0;
	RetVal = TotalDiscountExcGST();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalDiscountExcGST();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalSurchargeSidesGSTContent()const {
	Currency RetVal = 0;
	RetVal = TotalSurchargeGSTContent();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalSurchargeGSTContent();
	}
	return RetVal;
}

Currency TItemMinorComplete::TotalDiscountSidesGSTContent()const {
	Currency RetVal = 0;
	RetVal = TotalDiscountGSTContent();
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalDiscountGSTContent();
	}
	return RetVal;
}

std::vector<TDiscount>::iterator TItemMinorComplete::DiscountsBegin() {
	std::sort(Discounts.begin(), Discounts.end());
	return Discounts.begin();
}

std::vector<TDiscount>::iterator TItemMinorComplete::DiscountsEnd() {
	return Discounts.end();
}

TDiscount& TItemMinorComplete::Discount_Back()
{
    return Discounts.back();
}

void TItemMinorComplete::DiscountByTypeRemove(TDiscountSource DiscountSource)
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();	ptrDiscount != Discounts.end(); ) {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.Source == DiscountSource)
        {
            if(CurrentDiscount.Mode == DiscModePoints)
               ItemPriceForPoints = ItemPriceForPointsOriginal;
			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
		}
		else {
			ptrDiscount++;
		}
	}
	RunBillCalculator();
}

void TItemMinorComplete::DiscountByTypeRemove(TDiscountMode DiscountMode)
{
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();
		ptrDiscount != Discounts.end(); )
        {
		TDiscount CurrentDiscount = *ptrDiscount;
		if (CurrentDiscount.Mode == DiscountMode)
        {
            if(CurrentDiscount.Mode == DiscModePoints)
               ItemPriceForPoints = ItemPriceForPointsOriginal;
			Discounts.erase(ptrDiscount);
			ptrDiscount = Discounts.begin();
		}
		else {
			ptrDiscount++;
		}
	}
	RunBillCalculator();
}

int TItemMinorComplete::DiscountSize() {
	return Discounts.size();
}

void TItemMinorComplete::SplitOrder(int WaysToSplit)
{
	// The Price is the each Price so you no longer need to Split it down just change the QTY.
	// FPrice.Value = RoundToNearest(FPrice.Value / WaysToSplit, MIN_CURRENCY_VALUE, TGlobalSettings::Instance().MidPointRoundsDown);
	Qty = Qty / WaysToSplit;
	//Cost = RoundToNearest(Cost / WaysToSplit, MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
	Redeemed = Redeemed / WaysToSplit;

	for (std::vector<TDiscount>::iterator ptrDiscounts = Discounts.begin();
		ptrDiscounts != Discounts.end(); std::advance(ptrDiscounts, 1))
        {
     		if (ptrDiscounts->Mode == DiscModeDeal)
            {
			    ptrDiscounts->Amount = RoundToNearest (ptrDiscounts->Amount / WaysToSplit, MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
	     	}
	    }

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		SubOrderImage->SplitOrder(WaysToSplit);
	}
}

void TItemMinorComplete::SplitOrder(double quantityToSplit)
{
	for (std::vector<TDiscount>::iterator ptrDiscounts = Discounts.begin();
		ptrDiscounts != Discounts.end(); std::advance(ptrDiscounts, 1))
        {
     		if (ptrDiscounts->Mode == DiscModeDeal)
            {
			    ptrDiscounts->Amount = RoundToNearest (ptrDiscounts->Amount  * quantityToSplit/ Qty, MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
	     	}
	    }

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		SubOrderImage->SplitOrder(quantityToSplit);
	}
    //Cost = RoundToNearest(Cost * quantityToSplit/ Qty, MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
	Redeemed = Redeemed * quantityToSplit / Qty;
    Qty = quantityToSplit;
}

Currency TItemMinorComplete::GetPriceRRP() {
	Currency RRP = PriceLevel0;
	if (HappyHour) {
		if (TGlobalSettings::Instance().HappyHourMembersOnly) {
			if (Loyalty_Key != 0) {
				RRP = PriceLevel1;
			}
			else {
				RRP = PriceLevel0;
			}
		}
		else {
			RRP = PriceLevel1;
		}
	}
	else {
		RRP = PriceLevel0;
	}
	return RRP * GetQty();
}

double TItemMinorComplete::GetPointsPercent() const {
	if (TGlobalSettings::Instance().NoPtsEarnedDuringHappyHour && HappyHour)
		return 0;
	else
		return fPointsPercent;
}

//--------------------------------------------------------------------------
// new methods using bill calculator. format: oldname_BillCalc
// old methods will be modified to use the new methods in time
//--------------------------------------------------------------------------
Currency TItemMinorComplete::PriceEach_BillCalc() const
{
    Currency RetVal = 0;

    Currency qty = Currency(GetQty());
    if(qty==0)
       return RetVal;
    RetVal = BillCalcResult.FinalPrice / qty;
    return RetVal;
}
//--------------------------------------------------------------------------

Currency TItemMinorComplete::Price_BillCalc() const
{
    Currency RetVal = 0;
    RetVal = Price();
    return RetVal;
}

// returns total price including sides
Currency TItemMinorComplete::TotalPriceSides_BillCalc() const
{
	Currency RetVal = 0;
	RetVal = Price_BillCalc();

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->Price_BillCalc();
	}
    return RetVal;
}

// returns price after adjustments for current item
Currency TItemMinorComplete::TotalPriceAdjustment_BillCalc() const
{
	Currency RetVal = 0;
	RetVal = Price_BillCalc() + TotalAdjustment_BillCalc();
    return RetVal;
}

// returns price after adjustments including sides
Currency TItemMinorComplete::TotalPriceAdjustmentSides_BillCalc() const
{
    Currency RetVal = 0;
    RetVal = TotalPriceSides_BillCalc() + TotalAdjustmentSides_BillCalc();
    return RetVal;
}

Currency TItemMinorComplete::TotalPriceAdjustmentSidesExclGST_BillCalc() const
{
	Currency RetVal = 0;
	Currency OrderTotal = TotalPriceAdjustment_BillCalc();
	if (OrderTotal != 0)
    {
		RetVal += OrderTotal - BillCalcResult.TotalTax;
	}
	for (int i = 0; i < SubOrders->Count; i++) {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		Currency SubOrderTotal = SubOrderImage->TotalPriceAdjustment_BillCalc();
		if (SubOrderTotal != 0)
        {
			RetVal += SubOrderTotal - SubOrderImage->BillCalcResult.TotalTax;
		}
	}
    return RetVal;
}

Currency TItemMinorComplete::TotalSurcharge_BillCalc() const
{
	Currency TotalSurchage = 0;

    std::vector<BillCalculator::TDiscountResult> SurchageResults = BillCalcResult.Discount;

    for (std::vector<BillCalculator::TDiscountResult>::iterator itSurchageResult = SurchageResults.begin(); itSurchageResult != SurchageResults.end(); ++itSurchageResult)
    {
        if (itSurchageResult->DiscountType == BillCalculator::dtSurcharge)
        {
            TotalSurchage += itSurchageResult->Content;
        }
	}

	return TotalSurchage;
}

Currency TItemMinorComplete::TotalSurchargeSides_BillCalc() const
{
    Currency RetVal = 0;
    RetVal += TotalSurcharge_BillCalc();

    for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalSurcharge_BillCalc();
	}
	return RetVal;
}

Currency TItemMinorComplete::GrandTotal_BillCalc() const
{
	Currency RetVal = 0;
	RetVal = TotalPriceAdjustment_BillCalc() - Redeemed;
	return RetVal;
}

Currency TItemMinorComplete::GrandTotalExclGSTSides_BillCalc()const
{
	Currency RetVal = 0;
	Currency OrderTotal = GrandTotal_BillCalc();

	if (OrderTotal != 0)
    {
        RetVal += OrderTotal - BillCalcResult.TotalTax;
	}

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		Currency SubOrderTotal = SubOrderImage->GrandTotal_BillCalc();
		if (SubOrderTotal != 0)
        {
			RetVal += SubOrderTotal - SubOrderImage->BillCalcResult.TotalTax;
		}
	}
//	return RoundToNearest(RetVal, 0.01, TGlobalSettings::Instance().MidPointRoundsDown);
    return RetVal;
}

Currency TItemMinorComplete::GrandTotalGSTContentSides_BillCalc() const
{
    Currency RetVal = 0;
    RetVal += BillCalcResult.TotalTax - BillCalcResult.ServiceCharge.TaxValue;

    for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
        RetVal += SubOrderImage->BillCalcResult.TotalTax  - SubOrderImage->BillCalcResult.ServiceCharge.TaxValue;
	}
    return RetVal;
}

bool TItemMinorComplete::HasSeniorCitizensDiscountApplied() const
{
    return Discounts.size() == 1 && Discounts[0].IsSeniorCitizensDiscount();
}

bool TItemMinorComplete::HasPWDApplied() const
{
    return Discounts.size() == 1 && Discounts[0].IsPersonWithDisabilityDiscount();
}

Currency TItemMinorComplete::TotalDiscount_BillCalc() const
{
	Currency TotalDiscount = 0;

    std::vector<BillCalculator::TDiscountResult> DiscountResults = BillCalcResult.Discount;

    for (std::vector<BillCalculator::TDiscountResult>::iterator itDiscountResult = DiscountResults.begin(); itDiscountResult != DiscountResults.end(); ++itDiscountResult)
    {
        if (itDiscountResult->DiscountType == BillCalculator::dtDiscount)
        {
            TotalDiscount -= itDiscountResult->Content;
        }
	}

	return TotalDiscount;
}

Currency TItemMinorComplete::TotalDiscountSides_BillCalc() const
{
    Currency RetVal = TotalDiscount_BillCalc();

    for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
        RetVal += SubOrderImage->TotalDiscount_BillCalc();
	}
    return RetVal;
}

Currency TItemMinorComplete::TotalAdjustment_BillCalc() const
{
	Currency TotalAdjustment = 0;

    std::vector<BillCalculator::TDiscountResult> DiscountResults = BillCalcResult.Discount;

    for (std::vector<BillCalculator::TDiscountResult>::iterator itDiscountResult = DiscountResults.begin(); itDiscountResult != DiscountResults.end(); ++itDiscountResult)
    {
        if (itDiscountResult->DiscountType == BillCalculator::dtDiscount)
        {
            TotalAdjustment -= itDiscountResult->Content;
        }
        else
        {
            TotalAdjustment += itDiscountResult->Content;
        }
	}
	return TotalAdjustment;
}

Currency TItemMinorComplete::TotalAdjustmentSides_BillCalc() const
{
    Currency RetVal = TotalAdjustment_BillCalc();

    for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
        RetVal += SubOrderImage->TotalAdjustment_BillCalc();
	}
    return RetVal;
}

Currency TItemMinorComplete::TotalSalesTax_BillCalc() const
{
    Currency TotalSalesTax = 0;
        std::vector<BillCalculator::TTaxResult> TaxResults = BillCalcResult.Tax;

        for (std::vector<BillCalculator::TTaxResult>::iterator itTaxResult = TaxResults.begin(); itTaxResult != TaxResults.end(); ++itTaxResult)
        {
            TotalSalesTax += itTaxResult->Value;
        }
	return TotalSalesTax;
}

Currency TItemMinorComplete::TotalTax_BillCalc() const
{
    Currency TotalSalesTax = 0;
    std::vector<BillCalculator::TTaxResult> TaxResults = BillCalcResult.Tax;

    for (std::vector<BillCalculator::TTaxResult>::iterator itTaxResult = TaxResults.begin(); itTaxResult != TaxResults.end(); ++itTaxResult)
    {
        TotalSalesTax += itTaxResult->Value;
	}
    return TotalSalesTax_BillCalc() + BillCalcResult.ServiceCharge.TaxValue;
}

Currency TItemMinorComplete::TotalSalesTaxSides_BillCalc() const
{
    Currency RetVal = TotalSalesTax_BillCalc();

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->TotalSalesTax_BillCalc();
	}
    return RetVal;
}

Currency TItemMinorComplete::TotalTaxSides_BillCalc() const
{
    Currency RetVal = 0;
     RetVal = TotalTax_BillCalc();

     for (int i = 0; i < SubOrders->Count; i++)
            {
                TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
                RetVal += SubOrderImage->TotalTax_BillCalc();
            }
    return RetVal;
}

Currency TItemMinorComplete::ServiceCharge_BillCalc() const
{
       return BillCalcResult.ServiceCharge.Value;
}

Currency TItemMinorComplete::ServiceChargeSides_BillCalc() const
{
    Currency RetVal = ServiceCharge_BillCalc();
	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->ServiceCharge_BillCalc();
	}
    return RetVal;
}

Currency TItemMinorComplete::ServiceChargeTax_BillCalc() const
{
    return BillCalcResult.ServiceCharge.TaxValue;
}

Currency TItemMinorComplete::ServiceChargeTaxSides_BillCalc() const
{
    Currency RetVal = ServiceChargeTax_BillCalc();

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->ServiceChargeTax_BillCalc();
	}
    return RetVal;
}

Currency TItemMinorComplete::FinalPrice_BillCalc() const
{
    return BillCalcResult.FinalPrice;
}

Currency TItemMinorComplete::FinalPriceSides_BillCalc() const
{
    Currency RetVal = FinalPrice_BillCalc();

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		RetVal += SubOrderImage->FinalPrice_BillCalc();
	}
    return RetVal;
}

Currency TItemMinorComplete::DiscountValue_BillCalc(std::vector<TDiscount>::const_iterator &Discount)
{
    Currency discountContent = 0;

    std::vector<BillCalculator::TDiscountResult>::iterator itDiscountResult = BillCalcResult.Discount.begin();


    for ( ; itDiscountResult != BillCalcResult.Discount.end(); itDiscountResult++ )
    {
        if( itDiscountResult->DiscountKey == Discount->DiscountKey  && (itDiscountResult->Name == Discount->Name))
        {
            discountContent = itDiscountResult->DiscountType == BillCalculator::TDiscountType::dtDiscount
                                ? -itDiscountResult->Content
                                : itDiscountResult->Content;
            break;
        }
    }

	return discountContent;
}

Currency TItemMinorComplete::TotalTaxType_BillCalc( BillCalculator::TTaxType taxType )
{
    Currency value = 0;
    BillCalculator::TAX_RESULT_LIST::const_iterator it = BillCalcResult.Tax.begin();

    for( it; it != BillCalcResult.Tax.end(); it++)
    {
        if( it->TaxType == taxType )
            value += it->Value;
    }

    return value;
}

Currency TItemMinorComplete::TotalTaxTypeSides_BillCalc( BillCalculator::TTaxType taxType )
{
    Currency value = TotalTaxType_BillCalc( taxType );

	for (int i = 0; i < SubOrders->Count; i++)
    {
		TItemCompleteSub *SubOrderImage = SubOrders->SubOrderGet(i);
		value += SubOrderImage->TotalTaxType_BillCalc( taxType );
	}

    return value;
}

bool TItemMinorComplete::GetIsManuallyEnteredWeight() const
{
      return fIsManuallyEnteredWeight;
}

TDiscount TItemMinorComplete::GetLastPointDiscount()
{
	TDiscount ReturnDiscount;
	for (std::vector<TDiscount>::iterator ptrDiscount = Discounts.begin();	ptrDiscount != Discounts.end(); std::advance(ptrDiscount, 1))
    {
            TDiscount CurrentDiscount = *ptrDiscount;
            if (CurrentDiscount.Mode == DiscModePoints)
            {
                ReturnDiscount = *ptrDiscount;
		    }
	}
	return ReturnDiscount;
}
//-----------------------------------------------------------------------------------------------------------------
bool TItemMinorComplete::IsTaxTypeExist( BillCalculator::TTaxType taxType )
{
    bool isTaxTypeSame = false;
    BillCalculator::TAX_RESULT_LIST::const_iterator it = BillCalcResult.Tax.begin();

    for( it; it != BillCalcResult.Tax.end(); it++)
    {
        if( it->TaxType == taxType )
        {
            isTaxTypeSame = true;
            break;
        }
    }

    return isTaxTypeSame;
}
