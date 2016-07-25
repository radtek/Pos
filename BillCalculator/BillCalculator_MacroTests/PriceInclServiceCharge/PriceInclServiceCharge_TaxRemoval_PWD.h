//---------------------------------------------------------------------------

#ifndef PriceInclServiceCharge_TaxRemoval_PWDH
#define PriceInclServiceCharge_TaxRemoval_PWDH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "TestMMBillCalculator_Base.h"

using namespace BillCalculator;

/*
 * all the test cases listed in this file includes service charge in the price.
 * uses different scenarios with regular discounts, compound taxes, scd and pwd discounts
 * the test cases in this file is very similar to PriceInclusiveServiceCharge test case, however with tax removal feature
 */
class PriceInclServiceCharge_TaxRemoval_PWD : public TTestMMBillCalculator_Base
{
    public:

        __fastcall virtual PriceInclServiceCharge_TaxRemoval_PWD(System::String name);
        __fastcall virtual ~PriceInclServiceCharge_TaxRemoval_PWD();

    __published:

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge : Keeping service charge in the tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_PWD();
        void __fastcall Test_InclCompoundTax_InclSC_PWD();

        void __fastcall Test_ExemptTax_InclSC_PWD();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax : Keeping service charge in the tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_PWD();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT_PWD();
/*
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax : No service charge in tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_PWD_NoSCInExemptPrice();
        void __fastcall Test_InclCompoundTax_InclSC_PWD_NoSCInExemptPrice();

        void __fastcall Test_ExemptTax_InclSC_PWD_NoSCInExemptPrice();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax : No service charge in tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_PWD_NoSCInExemptPrice();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT_PWD_NoSCInExemptPrice();
*/
};

#endif
