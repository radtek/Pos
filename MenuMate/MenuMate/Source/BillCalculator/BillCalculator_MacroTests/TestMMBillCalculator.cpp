#include <vcl.h>
#pragma hdrstop

#include "TestMMBillCalculator.h"

__fastcall TTestTMMBillCalculator::TTestTMMBillCalculator(System::String name) : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestTMMBillCalculator::~TTestTMMBillCalculator()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator::SetUp()
{
    priceInfoObj = new TPriceInfo();
    priceInfoObj->MultiDiscountType = mdtSimple;
	priceInfoObj->MultiTaxType      = mttSimple;

    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator::TearDown()
{
    delete priceInfoObj;
    delete billCalcResponseObj;
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator::Test_Execute_Empty()
{
    billCalcResponseObj = billCalculator.Execute( priceInfoObj );

    CheckEquals( getTotalTaxForCurrentResult(), 0.00 );
    CheckEquals( getTotalDiscountForCurrentResult(), 0.00 );
    CheckEquals( getFinalPriceForCurrentResult(), 0.00 );
    CheckEquals( getBasePriceForCurrentResult(), 0.00 );
    CheckEquals( getServiceChargeForCurrenctResult(), 0.00 );
    CheckEquals( getServiceChargeTaxForCurrentResult(), 0.00 );
}
//------------------------------------------------------------------------------

void __fastcall TTestTMMBillCalculator::TestExecute()
{
  //  TBillCalcResponse* Execute( const TPriceInfo* const inPriceInfo)
}

void __fastcall TTestTMMBillCalculator::TestTax()
{
  //  Currency Tax( const TPriceInfo* const inPriceInfo)
}

void __fastcall TTestTMMBillCalculator::TestDiscount()
{
  //  Currency Discount( const TPriceInfo* const inPriceInfo)
}

void __fastcall TTestTMMBillCalculator::TestServiceCharge()
{
  //  Currency ServiceCharge( const TPriceInfo* const inPriceInfo)
}

Currency TTestTMMBillCalculator::getTotalTaxForCurrentResult()
{
    return getTotalTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getTotalDiscountForCurrentResult()
{
    return getTotalDiscount( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getFinalPriceForCurrentResult()
{
    return getFinalPrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getBasePriceForCurrentResult()
{
    return getBasePrice( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getServiceChargeForCurrenctResult()
{
    return getServiceCharge( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getServiceChargeTaxForCurrentResult()
{
    return getServiceChargeTax( billCalcResponseObj );
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getTotalTax( TBillCalcResponse* response )
{
    return response->Result.TotalTax;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getTotalDiscount( TBillCalcResponse* response )
{
    return response->Result.TotalDiscount;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getFinalPrice( TBillCalcResponse* response )
{
    return response->Result.FinalPrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getBasePrice( TBillCalcResponse* response )
{
    return response->Result.BasePrice;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getServiceCharge( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.Value;
}
//------------------------------------------------------------------------------
Currency TTestTMMBillCalculator::getServiceChargeTax( TBillCalcResponse* response )
{
    return response->Result.ServiceCharge.TaxValue;
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("Testing MMBillCalculator.h");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TTestTMMBillCalculator));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 33


/* [For debug purposes only - To be removed soon!!]
GenerateHeaderComment=true
DefaultExtension=.cpp
FileName=C:\Code\Git\MenumateGit-LocalMirror\BillCalculator\BillCalculator_MacroTests\TestMMBillCalculator.cpp
TestFramework=DUnit / C++ Win32
OutputPersonality=CPlusPlusBuilder.Personality
TestProject=C:\Code\Git\MenumateGit-LocalMirror\BillCalculator\BillCalculator_MacroTests\BillCalc_MacroTests.cbproj
UnitUnderTest=C:\Code\Git\MenumateGit-LocalMirror\BillCalculator\MMBillCalculator.h
NameOfUnitUnderTest=MMBillCalculator.h
TestCaseBaseClass=TTestCase
TestCasePrefix=Test
UnitName=TestMMBillCalculator
Namespace=TestMMBillCalculator
TestClasses=
  <0>
    Name=TMMBillCalculator
    Methods=
      <0>
        Name=Execute
        Signature= TBillCalcResponse* Execute( const TPriceInfo* const inPriceInfo)
      <1>
        Name=Tax
        Signature= Currency Tax( const TPriceInfo* const inPriceInfo)
      <2>
        Name=Discount
        Signature= Currency Discount( const TPriceInfo* const inPriceInfo)
      <3>
        Name=ServiceCharge
        Signature= Currency ServiceCharge( const TPriceInfo* const inPriceInfo)
TestClass=
Method=
*/
