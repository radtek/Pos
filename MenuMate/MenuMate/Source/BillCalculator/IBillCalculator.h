//---------------------------------------------------------------------------

#ifndef IBillCalculatorH
#define IBillCalculatorH

//---------------------------------------------------------------------------

#include <vector>
#include <map>
#include <Classes.hpp>

//---------------------------------------------------------------------------

namespace BillCalculator
{

typedef AnsiString string;

enum TTaxType { ttSale, ttLocal, ttProfit };

struct TTaxInfo
{
    TTaxInfo();

    TTaxType TaxType;
    string   Name;
    Currency Percentage;

	int Priority;
};

enum TDiscountType { dtDiscount,   dtSurcharge };
enum TDiscountWay  { dwPercentage, dwMoney, dwSetPrice };

struct TDiscountInfo
{
    TDiscountInfo();

    int      DiscountKey;

	string   Name;
    string   Description;
	Currency   Percentage;
	Currency Value;

	int  Priority;

    // Only apply if the Price is Tax Inclusive
	bool TaxExclusiveDiscount;

    // Set to TRUE is Person With Disability (PWD) discount is applied
    bool RecalcPriceWithTaxAfterDiscount;

	TDiscountType DiscountType;
	TDiscountWay  DiscountWay;
    // dwPercentage: Discount Percentage (%)
    // dwMoney:      Discount Value ($)

    std::vector<UnicodeString> DiscountGroupList;
    //std::vector<void*> DiscountGroupList;
};

struct TServiceChargeInfo
{
    TServiceChargeInfo();

    Currency Percentage;
    Currency TaxPercentage;
 };

 struct TPriorityComp
{
  bool operator()(const int& lhs, const int& rhs) const
  {
	return lhs < rhs;
  }
};

typedef std::vector<TTaxInfo> TAX_INFO_LIST;
typedef std::vector<TTaxInfo>::iterator TAX_INFO_LIST_ITERATOR;
typedef std::vector<TDiscountInfo> DISCOUNT_INFO_LIST;
typedef std::vector<TDiscountInfo>::iterator DISCOUNT_INFO_LIST_ITERATOR;

typedef std::map<int, TAX_INFO_LIST,      TPriorityComp> TAX_GROUP_MAP;
typedef std::map<int, DISCOUNT_INFO_LIST, TPriorityComp> DISCOUNT_GROUP_MAP;

enum TMultiDiscountType { mdtSimple, mdtCompound };
enum TMultiTaxType {      mttSimple, mttCompound };

struct TPriceInfo
{
    TPriceInfo();

    Currency Value;
    Currency MaxRetailPrice;
    bool     TaxInclusive;
    bool     ServiceChargeInclusive;
    bool     ServiceChargeTaxInclusive;

    // Only apply if the Price is Tax Inclusive
    bool TaxExclusiveServiceCharge;

    bool TaxBeforeDiscount;
    bool ServiceChargeBeforeDiscount;

    TMultiDiscountType MultiDiscountType;
    TMultiTaxType      MultiTaxType;

    TAX_INFO_LIST      TaxInfoList;
    DISCOUNT_INFO_LIST DiscountInfoList;
    TServiceChargeInfo ServiceChargeInfo;

    bool PriceTaxExempt;

    TServiceChargeInfo RemovedServiceChargeInfo;
    TAX_INFO_LIST      RemovedTaxInfoList;

    bool CalcDWTOnBasePrice;

    // The following two properties were implemented, however not used until the requirements are made clear.
    bool TaxExemptPriceExclusiveOfServiceCharge;    // when the price is tax exempt ( removed ), it should not contain service charge
    bool AllowServiceChargesOnTaxExemptPrice;       // when the price is tax exempt ( SCD applied ), it allows or not SC and SCT calculation.
};

//.........................................................................................

struct TTaxResult
{
    TTaxType TaxType;
    string   Name;
    Currency Percentage;
    Currency Value; // Tax Content
	int  Priority;
};

struct TDiscountResult
{
    int      DiscountKey;
    string   Name;
    string   Description;
    Currency Percentage;
    Currency Value;
    Currency Content;
    int  Priority;
    // Only apply if the Price is Tax Inclusive
    bool TaxExclusiveDiscount;
    // Set to TRUE is Person With Disability (PWD) discount is applied
    bool RecalcPriceWithTaxAfterDiscount;
    TDiscountType DiscountType;
    TDiscountWay  DiscountWay;
    // dwPercentage: Discount Percentage (%)
    // dwMoney:      Discount Value ($)

    std::vector<UnicodeString> DiscountGroupList;
};

struct TServiceChargeResult
{
    Currency   Percentage;
    Currency Value;
    
    Currency   TaxPercentage;
    Currency TaxValue;
 };

typedef std::vector<TTaxResult>      TAX_RESULT_LIST;
typedef std::vector<TDiscountResult> DISCOUNT_RESULT_LIST;

struct TBillCalcResult
{
	Currency              BasePrice;
	Currency			  TaxExemptPrice;	// price without the tax portion. ( it may or may not contain service charges )
    TAX_RESULT_LIST       Tax;
    DISCOUNT_RESULT_LIST  Discount;
    TServiceChargeResult  ServiceCharge;
    Currency              GrossPrice;      // Tax Inclusive Price ? Menu Price : Menu Price + Total Tax
    Currency              DiscountedPrice; // Price after Final Discount
    Currency FinalPrice;    // BasePrice + TAX + DISCOUNT + SERVICE CHARGE + SERVICE CHARGE TAX
    Currency TotalTax;      // Total Tax Content
    Currency TotalDiscount; // positive: surcharge | negative: discount
    bool PriceTaxExempt;
    Currency DiscountWithoutTax;
    Currency TaxOnDiscount;
    Currency ProfitTax;
    Currency PriceIncl;    //BasePrice + All Taxes
};

const bcr_NO_ERROR = 0;

struct TBillCalcResponse
{
    bool            Succesful;
    __int32         ErrorCode;
    string          Message;
    string          Description;
    TBillCalcResult Result; 
};

typedef std::map<string, Currency> DISCOUNT_CONTENTS_LIST;     // discount name and percentage from the whole amount
typedef std::map<string, Currency> TAX_CONTENTS_LIST;          // tax name and percentage from the whole amount

struct TCalcContentInput
{
    TCalcContentInput();

    Currency                Value;
    TAX_CONTENTS_LIST       TaxPercentages;
    DISCOUNT_CONTENTS_LIST  DiscountPercentages;
    Currency                ServiceChargePercentage;
    Currency                ServiceChargeTaxPercentage;

    void Reset();
    void InitializeFromBillCalcResult( const TBillCalcResult* const billCalcResult );
};

struct TCalcContentOutput
{
    Currency ValueContent;
    TAX_CONTENTS_LIST TaxContents;
    DISCOUNT_CONTENTS_LIST DiscountContents;
    Currency ServiceChargeContent;
    Currency ServiceChargeTaxContent;

    Currency PartialTax();
    Currency PartialTotalTax();
    Currency PartialDiscount();
    Currency PartialValue();
    Currency PartialServiceCharge();
    Currency PartialServiceChargeTax();
};

//---------------------------------------------------------------------------

class IBillCalculator
{
public:
    virtual ~IBillCalculator(){};

    virtual TBillCalcResponse* Execute( const TPriceInfo* const inPriceInfo ) = 0;

    //virtual Currency Tax(           const TPriceInfo* const inPriceInfo ) = 0;
    //virtual Currency Discount(      const TPriceInfo* const inPriceInfo ) = 0;
    //virtual Currency ServiceCharge( const TPriceInfo* const inPriceInfo ) = 0;

    virtual TCalcContentOutput CalculatePartialPaymentContent( const TCalcContentInput* const inInputInfo, Currency partialPayment ) = 0;
};

//---------------------------------------------------------------------------

} // namespace BillCalculator

#endif
