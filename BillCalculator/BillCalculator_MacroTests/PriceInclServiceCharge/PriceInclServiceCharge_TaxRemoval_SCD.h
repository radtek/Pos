//---------------------------------------------------------------------------

#ifndef PriceInclServiceCharge_TaxRemoval_SCDH
#define PriceInclServiceCharge_TaxRemoval_SCDH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "TestMMBillCalculator_Base.h"

using namespace BillCalculator;

/*
 * all the test cases listed in this file includes service charge in the price.
 * uses different scenarios with regular discounts, compound taxes, scd and pwd discounts
 * the test cases in this file is very similar to PriceInclusiveServiceCharge test case, however with tax removal feature
 */
class PriceInclServiceCharge_TaxRemoval_SCD : public TTestMMBillCalculator_Base
{
    public:

        __fastcall virtual PriceInclServiceCharge_TaxRemoval_SCD(System::String name);
        __fastcall virtual ~PriceInclServiceCharge_TaxRemoval_SCD();

    __published:

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Senior citizen discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_SCD();
        void __fastcall Test_InclCompoundTax_InclSC_SCD();

        void __fastcall Test_ExemptTax_InclSC_SCD();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Senior citizen discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_SCD();
};

#endif