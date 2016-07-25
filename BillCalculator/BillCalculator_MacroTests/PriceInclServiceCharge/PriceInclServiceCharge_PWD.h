//---------------------------------------------------------------------------

#ifndef PriceInclServiceCharge_PWDH
#define PriceInclServiceCharge_PWDH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "TestMMBillCalculator_Base.h"

using namespace BillCalculator;

/*
 * scenarios when pwd is applied
 */
class PriceInclServiceCharge_PWD : public TTestMMBillCalculator_Base
{
    public:

        __fastcall virtual PriceInclServiceCharge_PWD(System::String name);
        __fastcall virtual ~PriceInclServiceCharge_PWD();

    __published:

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge : Keeping service charge in the tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_PWD();
        void __fastcall Test_InclCompoundTax_InclSC_PWD();

        void __fastcall Test_ExclSimpleTax_InclSC_PWD();
        void __fastcall Test_ExclCompoundTax_InclSC_PWD();

        void __fastcall Test_ExemptTax_InclSC_PWD();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax : Keeping service charge in the tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_PWD();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT_PWD();

        //----------------------------------------------------------------------
/*
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax : No service charge in tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_PWD_NoSCInExemptPrice();
        void __fastcall Test_InclCompoundTax_InclSC_PWD_NoSCInExemptPrice();

        void __fastcall Test_ExclSimpleTax_InclSC_PWD_NoSCInExemptPrice();
        void __fastcall Test_ExclCompoundTax_InclSC_PWD_NoSCInExemptPrice();

        void __fastcall Test_ExemptTax_InclSC_PWD_NoSCInExemptPrice();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax : No service charge in tax exempt price
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_PWD_NoSCInExemptPrice();
        void __fastcall Test_InclCompoundTax_InclSC_InclSCT_PWD_NoSCInExemptPrice();
*/
};

#endif