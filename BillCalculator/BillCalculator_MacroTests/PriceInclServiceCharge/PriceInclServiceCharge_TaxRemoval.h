//---------------------------------------------------------------------------

#ifndef PriceInclServiceCharge_TaxRemovalH
#define PriceInclServiceCharge_TaxRemovalH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "TestMMBillCalculator_Base.h"

using namespace BillCalculator;

/*
 * all the test cases listed in this file includes service charge in the price.
 * uses different scenarios with regular discounts, compound taxes, scd and pwd discounts
 * the test cases in this file is very similar to PriceInclusiveServiceCharge test case, however with tax removal feature
 */
class PriceInclServiceCharge_TaxRemoval : public TTestMMBillCalculator_Base
{
    public:

        __fastcall virtual PriceInclServiceCharge_TaxRemoval(System::String name);
        __fastcall virtual ~PriceInclServiceCharge_TaxRemoval();

    __published:

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // TestCases with only service charge inclusive

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Basic scenarios
        void __fastcall Test_InclSimpleTax_InclSC();
        void __fastcall Test_InclCompoundTax_InclSC();

        void __fastcall Test_ExemptTax_InclSC();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Simple regular discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_SimpleRegDiscounts();
        void __fastcall Test_InclCompoundTax_InclSC_SimpleRegDiscounts();

        void __fastcall Test_ExemptTax_InclSC_SimpleRegDiscounts();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Compound regular discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_CompoundRegDiscounts();
        void __fastcall Test_InclCompoundTax_InclSC_CompoundRegDiscounts();

        void __fastcall Test_ExemptTax_InclSC_CompoundRegDiscounts();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // TestCases with service charge and service charge tax inclusive

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Basic scenarios
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT();

        void __fastcall Test_ExemptTax_InclSC_InclSCT();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Simple regular discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_SimpleRegDiscounts();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT_SimpleRegDiscounts();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Compound regular discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_CompoundRegDiscounts();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT_CompoundRegDiscounts();
};
#endif
