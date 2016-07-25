//---------------------------------------------------------------------------


#pragma hdrstop

#include <System.hpp>
#include "TestRounding.h"

#define ERR_NoRound   "Currency is rounding when no rounding is required"
#define ERR_RoundDown "Currency is rounding down incorrectly"
#define ERR_RoundUp   "Currency is rounding up incorrectly"
#define ERR_RoundMid  "Currency is rounding from the mid point incorrectly"

__fastcall TTestRounding::TTestRounding(System::String name) : TTestCase(name)
{
}
//------------------------------------------------------------------------------

__fastcall TTestRounding::~TTestRounding()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::SetUp()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::TearDown()
{
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToOneCent_RoundMidDown()
{
    Currency RoundTo = Currency( 0.01 );

    // No Rounding

    Currency ExpectedResult = Currency( 10.50 );
    Currency ValueToRound = Currency( 10.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 3.33 );
    ValueToRound = Currency( 3.3333 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 4.50 );
    ValueToRound = Currency( 4.5049 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 8.40 );
    ValueToRound = Currency( 8.3999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 34.57 );
    ValueToRound = Currency( 34.5678 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 5.55 );
    ValueToRound = Currency( 5.5555 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 9.99 );
    ValueToRound = Currency( 9.9957 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToOneCent_RoundMidUp()
{
    Currency RoundTo = Currency( 0.01 );

    // No Rounding

    Currency ExpectedResult = Currency( 8.90 );
    Currency ValueToRound = Currency( 8.90 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 4.30 );
    ValueToRound = Currency( 4.3045 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 13.99 );
    ValueToRound = Currency( 13.9939 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 6.00 );
    ValueToRound = Currency( 5.9999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 16.51 );
    ValueToRound = Currency( 16.5061 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 7.50 );
    ValueToRound = Currency( 7.4950 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 12.35 );
    ValueToRound = Currency( 12.3456 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToTenCents_RoundMidDown()
{
    Currency RoundTo = Currency( 0.10 );

    // No Rounding

    Currency ExpectedResult = Currency( 5.00 );
    Currency ValueToRound = Currency( 5.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 3.30 );
    ValueToRound = Currency( 3.3333 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 6.40 );
    ValueToRound = Currency( 6.4399 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 10.00 );
    ValueToRound = Currency( 9.99 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 65.30 );
    ValueToRound = Currency( 65.2648 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 7.60 );
    ValueToRound = Currency( 7.6583 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 80.00 );
    ValueToRound = Currency( 80.0575 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToTenCents_RoundMidUp()
{
    Currency RoundTo = Currency( 0.10 );

    // No Rounding

    Currency ExpectedResult = Currency( 6.50 );
    Currency ValueToRound = Currency( 6.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 15.60 );
    ValueToRound = Currency( 15.6250 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 70.00 );
    ValueToRound = Currency( 70.0475 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 33.00 );
    ValueToRound = Currency( 32.96 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 3.50 );
    ValueToRound = Currency( 3.4825 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 100.00 );
    ValueToRound = Currency( 99.95 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 6.40 );
    ValueToRound = Currency( 6.3581 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToFiveDollars_RoundMidDown()
{
    Currency RoundTo = Currency( 5.00 );

    // No Rounding

    Currency ExpectedResult = Currency( 10.00 );
    Currency ValueToRound = Currency( 10.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 45.00 );
    ValueToRound = Currency( 46.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 105.00 );
    ValueToRound = Currency( 107.25 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 35.00 );
    ValueToRound = Currency( 33.33 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 90.00 );
    ValueToRound = Currency( 89.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 25.00 );
    ValueToRound = Currency( 27.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 50.00 );
    ValueToRound = Currency( 52.85 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToFiveDollars_RoundMidUp()
{
    Currency RoundTo = Currency( 5.00 );

    // No Rounding

    Currency ExpectedResult = Currency( 95.00 );
    Currency ValueToRound = Currency( 95.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 40.00 );
    ValueToRound = Currency( 41.45 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 15.00 );
    ValueToRound = Currency( 17.49 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 75.00 );
    ValueToRound = Currency( 73.20 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 120.00 );
    ValueToRound = Currency( 119.45 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 40.00 );
    ValueToRound = Currency( 37.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 180.00 );
    ValueToRound = Currency( 177.55 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToZero_RoundMidDown()
{
    Currency RoundTo = Currency( 0 );

    // No Rounding

    Currency ExpectedResult = Currency( 10.50 );
    Currency ValueToRound = Currency( 10.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 3.33 );
    ValueToRound = Currency( 3.3333 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 4.50 );
    ValueToRound = Currency( 4.5049 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 8.40 );
    ValueToRound = Currency( 8.3999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 34.57 );
    ValueToRound = Currency( 34.5678 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 5.55 );
    ValueToRound = Currency( 5.5555 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 9.99 );
    ValueToRound = Currency( 9.9957 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToZero_RoundMidUp()
{
    Currency RoundTo = Currency( 0.01 );

    // No Rounding

    Currency ExpectedResult = Currency( 8.90 );
    Currency ValueToRound = Currency( 8.90 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 4.30 );
    ValueToRound = Currency( 4.3045 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 13.99 );
    ValueToRound = Currency( 13.9939 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 6.00 );
    ValueToRound = Currency( 5.9999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 16.51 );
    ValueToRound = Currency( 16.5061 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 7.50 );
    ValueToRound = Currency( 7.4950 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 12.35 );
    ValueToRound = Currency( 12.3456 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToFactions_RoundMidDown()
{
    Currency RoundTo = Currency( 0.0589 );

    // No Rounding

    Currency ExpectedResult = Currency( 10.50 );
    Currency ValueToRound = Currency( 10.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 3.30 );
    ValueToRound = Currency( 3.3223 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 4.50 );
    ValueToRound = Currency( 4.5149 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 8.40 );
    ValueToRound = Currency( 8.3999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 34.60 );
    ValueToRound = Currency( 34.5878 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 5.55 );
    ValueToRound = Currency( 5.5750 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 9.90 );
    ValueToRound = Currency( 9.9252 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_SmallNumbers_RoundToFactions_RoundMidUp()
{
    Currency RoundTo = Currency( 0.0589 );

    // No Rounding

    Currency ExpectedResult = Currency( 8.90 );
    Currency ValueToRound = Currency( 8.90 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 4.30 );
    ValueToRound = Currency( 4.3245 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 13.95 );
    ValueToRound = Currency( 13.9639 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 6.00 );
    ValueToRound = Currency( 5.9999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 16.55 );
    ValueToRound = Currency( 16.5312 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 7.50 );
    ValueToRound = Currency( 7.4750 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 12.35 );
    ValueToRound = Currency( 12.3259 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_LargeNumbers_RoundMidDown()
{
    Currency RoundTo = Currency( 1000000 );

    // No Rounding

    Currency ExpectedResult = Currency( 30000000 );
    Currency ValueToRound = Currency( 30000000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 25000000 );
    ValueToRound = Currency( 25480000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 82000000 );
    ValueToRound = Currency( 82399000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 71000000 );
    ValueToRound = Currency( 70752000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 48000000 );
    ValueToRound = Currency( 47860000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 12000000 );
    ValueToRound = Currency( 12500000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 47000000 );
    ValueToRound = Currency( 47591284 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_LargeNumbers_RoundMidUp()
{
    Currency RoundTo = Currency( 1000000 );

    // No Rounding

    Currency ExpectedResult = Currency( 25000000 );
    Currency ValueToRound = Currency( 25000000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 61000000 );
    ValueToRound = Currency( 61340000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 54000000 );
    ValueToRound = Currency( 54447000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 92000000 );
    ValueToRound = Currency( 91743000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 35000000 );
    ValueToRound = Currency( 34750000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 47000000 );
    ValueToRound = Currency( 46512500 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 100000000 );
    ValueToRound = Currency( 99500000 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_ExtraLargeNumbers_RoundMidDown()
{
    Currency RoundTo = Currency( 1000000000000.00 );

    // No Rounding

    Currency ExpectedResult = Currency( 8000000000000.00 );
    Currency ValueToRound = Currency( 8000000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 3000000000000.00 );
    ValueToRound = Currency( 3333000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 54000000000000.00 );
    ValueToRound = Currency( 54321000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 1000000000000.00 );
    ValueToRound = Currency( 800000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 68000000000000.00 );
    ValueToRound = Currency( 67990000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 4000000000000.00 );
    ValueToRound = Currency( 4500000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 51000000000000.00 );
    ValueToRound = Currency( 51562500000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_ExtraLargeNumbers_RoundMidUp()
{
    Currency RoundTo = Currency( 1000000000000.00 );

    // No Rounding

    Currency ExpectedResult = Currency( 50000000000000.00 );
    Currency ValueToRound = Currency( 50000000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( 82000000000000.00 );
    ValueToRound = Currency( 82451250000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( 2000000000000.00 );
    ValueToRound = Currency( 2100000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( 25000000000000.00 );
    ValueToRound = Currency( 24990000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( 57000000000000.00 );
    ValueToRound = Currency( 56789876543212.34 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( 13000000000000.00 );
    ValueToRound = Currency( 12500000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( 5000000000000.00 );
    ValueToRound = Currency( 4585000000000.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_NegativeNumbers_RoundToOneCent_RoundMidDown()
{
    Currency RoundTo = Currency( 0.01 );

    // No Rounding

    Currency ExpectedResult = Currency( -10.50 );
    Currency ValueToRound = Currency( -10.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( -3.33 );
    ValueToRound = Currency( -3.3333 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( -4.50 );
    ValueToRound = Currency( -4.5049 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( -8.40 );
    ValueToRound = Currency( -8.3999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( -34.57 );
    ValueToRound = Currency( -34.5678 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( -5.55 );
    ValueToRound = Currency( -5.5555 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( -9.99 );
    ValueToRound = Currency( -9.9957 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_NegativeNumbers_RoundToOneCent_RoundMidUp()
{
    Currency RoundTo = Currency( 0.01 );

    // No Rounding

    Currency ExpectedResult = Currency( -8.90 );
    Currency ValueToRound = Currency( -8.90 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( -4.30 );
    ValueToRound = Currency( -4.3045 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( -13.99 );
    ValueToRound = Currency( -13.9939 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( -6.00 );
    ValueToRound = Currency( -5.9999 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( -16.51 );
    ValueToRound = Currency( -16.5061 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( -7.50 );
    ValueToRound = Currency( -7.4950 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( -12.35 );
    ValueToRound = Currency( -12.3456 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_NegativeNumbers_RoundToTenCents_RoundMidDown()
{
    Currency RoundTo = Currency( 0.10 );

    // No Rounding

    Currency ExpectedResult = Currency( -5.00 );
    Currency ValueToRound = Currency( -5.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( -3.30 );
    ValueToRound = Currency( -3.3333 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( -6.40 );
    ValueToRound = Currency( -6.4399 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( -10.00 );
    ValueToRound = Currency( -9.99 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( -65.30 );
    ValueToRound = Currency( -65.2648 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( -7.60 );
    ValueToRound = Currency( -7.6583 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( -80.00 );
    ValueToRound = Currency( -80.0575 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_NegativeNumbers_RoundToTenCents_RoundMidUp()
{
    Currency RoundTo = Currency( 0.10 );

    // No Rounding

    Currency ExpectedResult = Currency( -6.50 );
    Currency ValueToRound = Currency( -6.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( -15.60 );
    ValueToRound = Currency( -15.6250 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( -70.00 );
    ValueToRound = Currency( -70.0475 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( -33.00 );
    ValueToRound = Currency( -32.96 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( -3.50 );
    ValueToRound = Currency( -3.4825 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( -100.00 );
    ValueToRound = Currency( -99.95 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( -6.40 );
    ValueToRound = Currency( -6.3581 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_NegativeNumbers_RoundToFiveDollars_RoundMidDown()
{
    Currency RoundTo = Currency( 5.00 );

    // No Rounding

    Currency ExpectedResult = Currency( -10.00 );
    Currency ValueToRound = Currency( -10.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( -45.00 );
    ValueToRound = Currency( -46.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    ExpectedResult = Currency( -105.00 );
    ValueToRound = Currency( -107.25 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( -35.00 );
    ValueToRound = Currency( -33.33 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    ExpectedResult = Currency( -90.00 );
    ValueToRound = Currency( -89.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( -25.00 );
    ValueToRound = Currency( -27.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );

    ExpectedResult = Currency( -50.00 );
    ValueToRound = Currency( -52.85 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_NegativeNumbers_RoundToFiveDollars_RoundMidUp()
{
    Currency RoundTo = Currency( 5.00 );

    // No Rounding

    Currency ExpectedResult = Currency( -95.00 );
    Currency ValueToRound = Currency( -95.00 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_NoRound );

    // Round Down

    ExpectedResult = Currency( -40.00 );
    ValueToRound = Currency( -41.45 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    ExpectedResult = Currency( -15.00 );
    ValueToRound = Currency( -17.49 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundDown );

    // Round Up

    ExpectedResult = Currency( -75.00 );
    ValueToRound = Currency( -73.20 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    ExpectedResult = Currency( -120.00 );
    ValueToRound = Currency( -119.45 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundUp );

    // Round Mid

    ExpectedResult = Currency( -40.00 );
    ValueToRound = Currency( -37.50 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );

    ExpectedResult = Currency( -180.00 );
    ValueToRound = Currency( -177.55 );

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, false ).Val, ERR_RoundMid );
}
//------------------------------------------------------------------------------

void __fastcall TTestRounding::Test_EdgeNumbers()
{
    Currency RoundTo = Currency( 0.01 );

    Currency ExpectedResult = Currency( 0 );
    Currency ValueToRound = Currency( 0 );
    ExpectedResult.Val = 9223372036854775800;
    ValueToRound.Val = 9223372036854775807;

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );

    ExpectedResult.Val = -9223372036854775800;
    ValueToRound.Val = -9223372036854775807;

    CheckEquals( ExpectedResult.Val, RoundToNearest( ValueToRound, RoundTo, true ).Val, ERR_NoRound );
}
//------------------------------------------------------------------------------

static void registerTests()
{
  _di_ITestSuite iSuite;
  TTestSuite* testSuite = new TTestSuite("Testing_Rounding");
  if (testSuite->GetInterface(iSuite)) {
    testSuite->AddTests(__classid(TTestRounding));
    Testframework::RegisterTest(iSuite);
  } else {
    delete testSuite;
  }
}
#pragma startup registerTests 3
