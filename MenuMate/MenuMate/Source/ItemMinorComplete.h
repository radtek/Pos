//---------------------------------------------------------------------------

#ifndef ItemMinorCompleteH
#define ItemMinorCompleteH

#include "ItemMinor.h"
#include <vector>
#include "Discount.h"
#include "ListSecurityRefContainer.h"
#include "WeightedPrice.h"
#include "ListSubOrderContainer.h"
#include "ListCategoryContainer.h"
#include "ListRecipeContainer.h"
#include "IBillCalculator.h"
#include "Item.h"

#include "EnumOrderStatus.h"

//enum OrderStatus {NormalOrder,Unused,CanceledOrder,CreditNonExistingOrder};

//---------------------------------------------------------------------------
class TItemMinorComplete : public TItemMinor
{
   private :

   double GetPointsPercent() const;


	public :
		/* Unary filters for use with the flatten_item_tree(...) function. */
		class i_flatten_predicate {
      public:
			i_flatten_predicate() { };
			virtual ~i_flatten_predicate() { };

			virtual bool operator()(const TItemMinorComplete &item) const = 0;
		};

		class identity_predicate : public i_flatten_predicate {
		public:
			identity_predicate() { };
			~identity_predicate() { };

			bool
			operator()(const TItemMinorComplete &item)
			const
			{
				return true;
			};
		};

		class i_grand_total_function {
		public:
			i_grand_total_function() { };
			virtual ~i_grand_total_function() { };

         virtual Currency operator()(const TItemMinorComplete &item) const = 0;
		};

		class grand_total_after_discount : public i_grand_total_function {
		public:
			grand_total_after_discount() { };
			~grand_total_after_discount() { };

			Currency
			operator()(const TItemMinorComplete &item)
			const
			{
				return item.GrandTotal();
			};
		};

	mutable std::vector<TDiscount> Discounts;

    static const identity_predicate identity_predicate_;

    /*
     * The functor referenced by this variable is used to calculate the
     * total value of an item that is used when tax is calculated.
     */
    const i_grand_total_function *grand_total_function_;

    /*
     * The functors used by grand_total_function.
    * Which functor grand_total_function references is determined when
    * the TItemMinorComplete instance is created.
     */
    static const grand_total_after_discount  grand_total_after_discount_fn_;


    TItemMinorComplete();
    virtual __fastcall ~TItemMinorComplete();

    TItemMinorComplete(const TItemMinorComplete &OtherItem);
    TItemMinorComplete& operator= (const TItemMinorComplete &OtherItem);

    TListSubOrderContainer *SubOrders;
    TListSecurityRefContainer *Security;

    /* If you are using Assign to create a COPY for a new
    order i.e a Split you will need to reassign the Security Ref
    with TDBSecurity::GetNextSecurityRef() */
    void virtual Assign(TItemMinor *RetItem);

    int 	OrderKey;            // Orders Key is returned in this feild.
    int OrderIdentificationNo;
    UnicodeString TransNo;
    TStringList *RemovedTaxes;
    long 	Item_ID;
    int 	Loyalty_Key;
    bool 	HappyHour;
    OrderStatus OrderType;            // Normal , Credit Etc.. (Status)
    TWeightedPrice WeightedPrice;
    AnsiString DelayedInvoiceNumber;
    TDateTime TimeStamp;

    TListCategoryContainer *Categories;
    TListRecipeContainer *SalesRecipesToApply;
    TListRecipeContainer *WastageRecipesToApply;

    AnsiString Reason;

    //Keys for saving customers name
    UnicodeString OrderTypeMessage;
    int ContactsKey;
    Currency AmountRedeemed;
    Currency PointRedeemed;

    bool PrintCancel;
    TItemType       SubItemType;
    // Cache of last successful BillCalcResult for this item
    BillCalculator::TBillCalcResult BillCalcResult;
    BillCalculator::TBillCalcResult CancelledBillCalcResult;

    virtual int GetMasterOrderKey(){return NULL;}

    void DiscountAddIncSides(TDiscount &Discount);
    void DiscountAdd(TDiscount &Discount);
    bool DiscountApplies(const TDiscount &Discount) const;
    void DiscountRemove(TDiscount &Discount);
    void DiscountRemoveInvalid();
    double GetGST() const;

    void DiscountsClear();
    void DiscountsClearByFilter(std::vector<TDiscountMode> exemptFilter);
    void ThorVouchersDiscountsClear();
    void DiscountByTypeRemove(TDiscountSource DiscountSource);
    void DiscountByTypeRemove(TDiscountMode DiscountMode);
    void DiscountByTypeLevelRemove(TDiscountSource DiscountSource);
    void ThorlinkDiscountByTypeLevelRemove(TDiscountSource DiscountSource);
    void DiscountCredit();
    int DiscountSize();
    bool DiscountApplied(int DiscountKey);
    TDiscount DiscountFind(int DiscountKey);
    bool DiscountFind(TDiscountMode Mode);
    Currency DiscountGet(TDiscountMode Mode);
    std::vector<TDiscount>::iterator DiscountsBegin();
    std::vector<TDiscount>::iterator DiscountsEnd();
    TDiscount& Discount_Back();
    Currency DiscountValue(std::vector<TDiscount>::const_iterator &Discount);
    Currency DiscountValue(TDiscount Discount);


    // Pricing Functions.

    Currency TotalPriceRedeemed() const;
    Currency TotalPriceRedeemedExclGST() const;
    Currency TotalPriceRedeemedGSTContent() const;

    Currency TotalPriceAdjustment() const;
    Currency TotalPriceAdjustmentExcCombo() const;
    Currency TotalAdjustmentExcCombo() const;
    Currency TotalPriceAdjustmentSides() const;

    Currency TotalPriceRedeemedSides() const;
    Currency TotalPriceAdjustmentSidesExclGST() const;
    Currency TotalPriceRedeemedSidesGSTContent() const;

    // Grand Total is the Total Price + Adjustment + Redeemed
    Currency GrandTotalExcCombo() const;
    Currency GrandTotal() const;
    Currency GrandTotalSides() const;
    Currency GrandTotalExclGST() const;
    Currency GrandTotalExclGSTSides() const;
    Currency GrandTotalGSTContent() const;

    Currency GrandTotalGSTContentSides() const;

    Currency TotalPriceSides() const;
    Currency TotalPriceSidesGSTContent() const;

    /*   Currency TotalGSTContent();
    Currency TotalGSTContentSides();*/


    Currency TotalAdjustment() const;
    Currency TotalAdjustmentExcGST() const;
    Currency TotalAdjustmentGSTContent() const;

    Currency TotalAdjustmentSides() const;
    Currency TotalAdjustmentSidesExcGST() const;
    Currency TotalAdjustmentSidesGSTContent() const;

    Currency TotalDiscount() const;
    Currency TotalDiscountSides() const;
    Currency TotalDiscountExcGST() const;
    Currency TotalDiscountGSTContent() const;

    Currency TotalSurcharge() const;
    Currency TotalSurchargeGSTContent() const;
    Currency TotalSurchargeExcGST() const;

    Currency TotalSurchargeSides() const;
    Currency TotalSurchargeSidesExcGST() const;
    Currency TotalSurchargeSidesGSTContent() const;

    Currency TotalDiscountSidesExcGST() const;
    Currency TotalDiscountSidesGSTContent() const;

    Currency TotalSystemDiscount() const;
    Currency TotalSystemDiscountSides() const;
    Currency TotalUserDiscount() const;

    AnsiString DiscountReasons();

    void Cancel(bool ReturnStock);
    void Credit(bool WriteOffStock);
    void ReverseCredit();
    void SetPriceLevel0();
    void SetPriceLevel1();
    void SetPriceLevelCustom();
    void SetPriceLevelCustom(Currency NewPrice);
    void ResetPrice();

    bool Add(TItemMinorComplete *Item);
    void SetQty(const double inQty,
                const bool scale_patron_count = true);  //MM-6306 : double data type in place of currency
    void SetQtyCustom(const double inQty,
                      const bool scale_patron_count = true);    //MM-6306 : double data type in place of currency
    void SetWeight(TWeight &inWeight);
    void SplitOrder(int WaysToSplit);
    void SplitOrder(double quantityToSplit);
    Currency GetPriceRRP();

    bool IsTaxExempt() const;
    /*
    * Utility. Avoids descending the Item "tree" explicitly. Creates a vector
    * containing this item and it's suborders.
    */
    std::vector<const TItemMinorComplete *> flatten_item_tree(
    	const i_flatten_predicate &predicate) const;
    std::vector<TItemMinorComplete *> flatten_item_tree(
    	const i_flatten_predicate &predicate);

    int SubordersSize() { return SubOrders->Count; };

   bool ClaimAvailability(
       double n_units = 1.0,
       std::vector<UnicodeString> *items_that_couldnt_be_acquired = NULL);
   void ReturnToAvailability(double n_units = 1.0);

   double GetAvailableQuantity() const;

   bool IsBeingReturnedToStock() const;

   virtual int DefaultPatronCount() const;
   virtual int PatronCount(const int new_count,
                           const bool scale_by_qty = true);
   virtual int PatronCount() const;

   virtual void SetPatronCount(int new_count);

   void RunBillCalculator();

    //--------------------------------------------------------------------------
    // new methods using bill calculator. format: oldname_BillCalc
    // old methods will be modified to use the new methods in time
    //--------------------------------------------------------------------------
    Currency PriceEach_BillCalc() const;
    Currency Price_BillCalc() const;

    Currency TotalPriceSides_BillCalc() const;
    Currency TotalPriceAdjustment_BillCalc() const;
    Currency TotalPriceAdjustmentSides_BillCalc() const;
    Currency TotalPriceAdjustmentSidesExclGST_BillCalc() const;

    Currency TotalSurcharge_BillCalc() const;
    Currency TotalSurchargeSides_BillCalc() const;

    Currency GrandTotal_BillCalc() const;
    Currency GrandTotalExclGSTSides_BillCalc() const;
    Currency GrandTotalGSTContentSides_BillCalc() const;

    bool HasSeniorCitizensDiscountApplied() const;
    bool HasPWDApplied() const;
    Currency TotalDiscount_BillCalc() const;
    Currency TotalDiscountSides_BillCalc() const;

    Currency TotalAdjustment_BillCalc() const;
    Currency TotalAdjustmentSides_BillCalc() const;

    Currency TotalSalesTax_BillCalc() const;
    Currency TotalTax_BillCalc() const;

    Currency TotalSalesTaxSides_BillCalc() const;
    Currency TotalTaxSides_BillCalc() const;

    Currency ServiceCharge_BillCalc() const;
    Currency ServiceChargeSides_BillCalc() const;

    Currency ServiceChargeTax_BillCalc() const;
    Currency ServiceChargeTaxSides_BillCalc() const;

    Currency FinalPrice_BillCalc() const;         // final price to be paid for the item
    Currency FinalPriceSides_BillCalc() const;

    Currency DiscountValue_BillCalc(std::vector<TDiscount>::const_iterator &Discount);

    Currency TotalTaxType_BillCalc( BillCalculator::TTaxType taxType );
    Currency TotalTaxTypeSides_BillCalc( BillCalculator::TTaxType taxType );
    TDiscount GetLastPointDiscount();
    bool IsSide;
    bool GetIsManuallyEnteredWeight() const;
    bool LastAddedItem;
    bool IsPayByPoints;
    Currency ItemPriceForPoints;
    Currency ItemPriceForPointsOriginal;
    bool printFreeSideForKitchen;
    bool printFreeSideForReceipt;
    bool IsTaxTypeExist( BillCalculator::TTaxType taxType );
    bool wasOpenItem;
    void ClearAllDiscounts();
    int RoomNo;
    UnicodeString RoomNoStr;
    UnicodeString AccNo;
    UnicodeString FirstName;
    UnicodeString LastName;
    int TabKey;
    UnicodeString SplitMessage;
    int OnlineChitType;
    UnicodeString OnlineOrderId;
    int SiteId;
    int OrderItemId;
    int OrderItemSizeId;
    int ReferenceOrderItemSizeId;

private:
        bool is_being_returned_to_stock_;
        int  patron_count_;
};
#endif
