//---------------------------------------------------------------------------

#ifndef TestPartialPaymentsH
#define TestPartialPaymentsH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "MenuItemListManager.h"
#include "Math.h"

class TestPartialPayments : public TTestCase
{
    public:
        __fastcall virtual TestPartialPayments(System::String name) : TTestCase(name) {}
        virtual void __fastcall SetUp();
        virtual void __fastcall TearDown();

__published:

        void __fastcall TestPartialPayments_TaxExclusive();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTax();
        void __fastcall TestPartialPayments_TaxExclusive_SingleDiscount();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscount();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_A();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_B();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_C();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_D();

        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_E();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_F();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_G();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_H();

        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_I();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_J();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_K();
        void __fastcall TestPartialPayments_TaxExclusive_SingleTaxSingleDiscountServChargeServChargeTax_L();

        void __fastcall TestPartialPayments_TaxExclusive_MultiTaxMultiDiscounts();

    private:
        std::vector<MenuItem> menuItemList;
        MenuItemListManager menuItemListManager;

        void TestPartialPayments::doPartialPayment( Currency partialPayment );
        void TestPartialPayments::validateResults();
        void validate( std::map<AnsiString, CURRENCY_VECTOR> &results );
        void setPriceInfo(
                   TPriceInfo*   const inPriceInfo,
                   Currency           inValue,
                   bool                inTaxInclusivePrice,
                   bool                inTaxExclusiveServiceCharge,
                   bool                inTaxBeforeDiscount,
                   bool                inServiceChargeBeforeDiscount,
                   TMultiDiscountType  inMultiDiscountType,
                   TMultiTaxType       inMultiTaxType,
                   TAX_INFO_LIST*      inTaxInfoList,
                   DISCOUNT_INFO_LIST* inDiscountInfoList,
                   TServiceChargeInfo* inServiceChargeInfo,
                   bool                inPriceTaxExempt );
};

#endif
