//---------------------------------------------------------------------------

#ifndef MM_MathH
#define MM_MathH
//---------------------------------------------------------------------------
#include <system.hpp>
//---------------------------------------------------------------------------
namespace MMMath
{
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Currency RoundCurrency(const Currency Value, const int Decimals=-1);
Currency RoundCurrencyTo5c(const Currency Value);
Currency RoundCurrencyTo10c(const Currency Value);
Currency RoundCurrencyDownTo5c(const Currency Value);
AnsiString CurrencyString(const Currency Value, int Decimals=-1, bool ShowCurrencySymbol=true);
double RoundFloat(const double Value, int Decimals, int Precision=10);
AnsiString FloatString(const double Value, const int MinDecimals=0, const int MaxDecimals=4, const int Precision=10);
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
