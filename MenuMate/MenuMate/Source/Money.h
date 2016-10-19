// ---------------------------------------------------------------------------

#ifndef MoneyH
#define MoneyH
// ---------------------------------------------------------------------------
#include <set>
#include <map>
#include <system.hpp>
#include <Classes.hpp>

#include "MM_DBCore.h"
#include "Contact.h"
#include "Rounding.h"
#include "MMBillCalculatorAdapter.h"

class TPaymentTransaction;
class TPointsTransaction;

class TMoney
{
private:
   Currency RoundChangeTo;

   void Round();

   BillCalculator::TMMBillCalculatorAdapter billCalcAdapter;

public:
   Currency UnRoundedChange;
   void Clear(); // Clears all Currency Values.
   Currency ProductAmount, // Amount Owed in Product.
   ProductExcGST,
   ProductGSTContent,
   TotalProduct,              // Prodcut Amount Including Adjustments.
   ProductAdjustment, 			// Amount of Product discounts & Surcharges.
   ProductDiscount, // Amount of Product discounts.
   ProductSurcharge, // Amount of Product Surcharges.
   ProductSurchargeExcGST,
   ProductSurchargeGSTContent,
   ProductDiscountExcGST,
   ProductDiscountGSTContent,
   SystemDiscount, // Amount of System discounts.
   PaymentDue, // Amount Due.
   PaymentAmount, // Amount being Paid.
   PaymentRounding, // Any adjustments due to rounding.
   PaymentCashOut, // Amount being Paid For CashOut.
   PaymentAdjustments, // Tips etc.
   RefundPoints,
   PaymentSurcharges,  // Cheque Fess.
   PaymentSurchargesExcGST,
   PaymentSurchargesGSTContent,
   PaymentDiscounts,
   PaymentDiscountsExcGST,
   PaymentDiscountsGSTContent,
   // In this cas an Account is a Single Tab or a Single Invoice.
   InvoiceTotal, // Expected balance left to pay after this payment.
   InvoiceBalance,
   // Total of this Bill +
   // Any Tips or Surcharges less Discounts +
   // Rounding.
   GrandTotal,
   GrandTotalExclGST,
   GrandTotalGSTContent,
   Change, // Total change inc cashout.
   CreditRedeemed, CreditAvailable, // Static not effected by Recalc();
   CreditRemaining,
   // Totals bill excluding Discounts and Surcharges.
   Total,
   TotalExclGST,
   TotalGSTContent,
   //Total of Discounts and Surcharges for Products and Payments.
   TotalAdjustment,
   TotalAdjustmentExcGST,
   TotalAdjustmentGSTContent,
   // Total Discounts for Products and Payments.
   Discount,
   DiscountExcGST,
   DiscountGSTContent,
   // Total Surcharge for Products and payments.
   Surcharge,
   SurchargeExcGST,
   SurchargeGSTContent,
   TotalOwing, 	   // Total of this Bill.
   TotalTendered,  // Total of Payments Tendered.
   TotalRounding;
   Currency ServiceCharge;
   Currency ServiceChargeTax;
   Currency FinalPrice; // total price to be paid, incremented by each item
    Currency RoundedGrandTotal;
    Currency RoundedDiscount;
    Currency RoundedSurcharge;
    Currency RoundedServiceCharge;
    Currency RoundedServiceChargeTax;
    Currency RoundedFinalPrice;
    Currency RoundedCreditAvailable;
    Currency RoundedCreditRedeemed;
    Currency RoundedChange;
    Currency RoundedPaymentDue;
    Currency RoundedTotalAdjustment;
    Currency RoundedProductAmount;
    Currency RoundingAdjustment;
    Currency PaymentTip;
    bool IsBilling;
    bool IsSCD;
   void Recalc(TPaymentTransaction &Transaction, bool isBilling = true);
   TMoney();
};






class TPaymentTransaction;


#endif
