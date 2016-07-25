//---------------------------------------------------------------------------

#ifndef TestRoundingH
#define TestRoundingH
//---------------------------------------------------------------------------

#include <TestFramework.hpp>
#include "rounding.h"

class TTestRounding : public TTestCase
{
public:
    __fastcall virtual TTestRounding(System::String name);
    __fastcall virtual ~TTestRounding();

    virtual void __fastcall SetUp();
    virtual void __fastcall TearDown();

__published:
    void __fastcall Test_SmallNumbers_RoundToOneCent_RoundMidDown();
    void __fastcall Test_SmallNumbers_RoundToOneCent_RoundMidUp();

    void __fastcall Test_SmallNumbers_RoundToTenCents_RoundMidDown();
    void __fastcall Test_SmallNumbers_RoundToTenCents_RoundMidUp();

    void __fastcall Test_SmallNumbers_RoundToFiveDollars_RoundMidDown();
    void __fastcall Test_SmallNumbers_RoundToFiveDollars_RoundMidUp();

    void __fastcall Test_SmallNumbers_RoundToZero_RoundMidDown();
    void __fastcall Test_SmallNumbers_RoundToZero_RoundMidUp();

    void __fastcall Test_SmallNumbers_RoundToFactions_RoundMidDown();
    void __fastcall Test_SmallNumbers_RoundToFactions_RoundMidUp();

    void __fastcall Test_LargeNumbers_RoundMidDown();
    void __fastcall Test_LargeNumbers_RoundMidUp();

    void __fastcall Test_ExtraLargeNumbers_RoundMidDown();
    void __fastcall Test_ExtraLargeNumbers_RoundMidUp();

    void __fastcall Test_NegativeNumbers_RoundToOneCent_RoundMidDown();
    void __fastcall Test_NegativeNumbers_RoundToOneCent_RoundMidUp();

    void __fastcall Test_NegativeNumbers_RoundToTenCents_RoundMidDown();
    void __fastcall Test_NegativeNumbers_RoundToTenCents_RoundMidUp();

    void __fastcall Test_NegativeNumbers_RoundToFiveDollars_RoundMidDown();
    void __fastcall Test_NegativeNumbers_RoundToFiveDollars_RoundMidUp();

    void __fastcall Test_EdgeNumbers();
};
#endif
