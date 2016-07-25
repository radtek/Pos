//---------------------------------------------------------------------------

#ifndef TestMMBillCalculatorH
#define TestMMBillCalculatorH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "MMBillCalculator.h"

using namespace BillCalculator;

class TTestTMMBillCalculator : public TTestCase
{
public:
  __fastcall virtual TTestTMMBillCalculator(System::String name);
  __fastcall virtual ~TTestTMMBillCalculator();

  virtual void __fastcall SetUp();
  virtual void __fastcall TearDown();

__published:
  void __fastcall TestExecute();
  void __fastcall TestTax();
  void __fastcall TestDiscount();
  void __fastcall TestServiceCharge();
  void __fastcall Test_Execute_Empty();

private:
    TMMBillCalculator billCalculator;
    TPriceInfo* priceInfoObj;
    TBillCalcResponse* billCalcResponseObj;

    Currency getTotalTaxForCurrentResult();
    Currency getTotalDiscountForCurrentResult();
    Currency getFinalPriceForCurrentResult();
    Currency getBasePriceForCurrentResult();
    Currency getServiceChargeForCurrenctResult();
    Currency getServiceChargeTaxForCurrentResult();

    Currency getTotalTax( TBillCalcResponse* response );
    Currency getTotalDiscount( TBillCalcResponse* response );
    Currency getFinalPrice( TBillCalcResponse* response );
    Currency getBasePrice( TBillCalcResponse* response );
    Currency getServiceCharge( TBillCalcResponse* response );
    Currency getServiceChargeTax( TBillCalcResponse* response );
};
#endif
