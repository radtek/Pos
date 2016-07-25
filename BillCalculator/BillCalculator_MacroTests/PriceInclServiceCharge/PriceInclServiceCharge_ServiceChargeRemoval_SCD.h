//---------------------------------------------------------------------------

#ifndef PriceInclServiceCharge_SCDH
#define PriceInclServiceCharge_SCDH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "TestMMBillCalculator_Base.h"

using namespace BillCalculator;

/*
 * basic scenarios on price inclusive service charges
 */
class PriceInclServiceCharge_ServiceChargeRemoval_SCD : public TTestMMBillCalculator_Base
{
    public:

        __fastcall virtual PriceInclServiceCharge_ServiceChargeRemoval_SCD(System::String name);
        __fastcall virtual ~PriceInclServiceCharge_ServiceChargeRemoval_SCD();

    __published:

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge
        void __fastcall Test_InclSimpleTax_InclSC_SCD();
        void __fastcall Test_InclCompoundTax_InclSC_SCD();

        void __fastcall Test_ExclSimpleTax_InclSC_SCD();
        void __fastcall Test_ExclCompoundTax_InclSC_SCD();

        void __fastcall Test_ExemptTax_InclSC_SCD();

        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // Price inclusive service charge and service charge tax
        void __fastcall Test_InclSimpleTax_InclSC_InclSCT_SCD();
};

#endif
