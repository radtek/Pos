//---------------------------------------------------------------------------

#ifndef PriceInclServiceCharge_GenericH
#define PriceInclServiceCharge_GenericH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "TestMMBillCalculator_Base.h"

using namespace BillCalculator;

/*
 * basic scenarios on price inclusive service charges
 */
class PriceInclServiceCharge_ServiceChargeRemoval : public TTestMMBillCalculator_Base
{
    public:

        __fastcall virtual PriceInclServiceCharge_ServiceChargeRemoval(System::String name);
        __fastcall virtual ~PriceInclServiceCharge_ServiceChargeRemoval();

    __published:

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // TestCases with only service charge inclusive

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Basic scenarios
        void __fastcall Test_InclSimpleTax_InclSC();
        void __fastcall Test_InclCompoundTax_InclSC();

        void __fastcall Test_ExclSimpleTax_InclSC();
        void __fastcall Test_ExclCompoundTax_InclSC();

        void __fastcall Test_ExemptTax_InclSC();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Simple regular discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_SimpleRegDiscounts();
        void __fastcall Test_InclCompoundTax_InclSC_SimpleRegDiscounts();

        void __fastcall Test_ExclSimpleTax_InclSC_SimpleRegDiscounts();
        void __fastcall Test_ExclCompoundTax_InclSC_SimpleRegDiscounts();

        void __fastcall Test_ExemptTax_InclSC_SimpleRegDiscounts();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Compound regular discount scenarios
        void __fastcall Test_InclSimpleTax_InclSC_CompoundRegDiscounts();
        void __fastcall Test_InclCompoundTax_InclSC_CompoundRegDiscounts();

        void __fastcall Test_ExclSimpleTax_InclSC_CompoundRegDiscounts();
        void __fastcall Test_ExclCompoundTax_InclSC_CompoundRegDiscounts();

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
