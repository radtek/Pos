//---------------------------------------------------------------------------

#ifndef MM_MathH
#define MM_MathH
//---------------------------------------------------------------------------
namespace MM_Math
{
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Currency RoundCurrency(const Currency Value, const int Decimals=-1);
Currency RoundCurrencyTo5c(const Currency Value);
Currency RoundCurrencyDownTo5c(const Currency Value);
AnsiString CurrencyString(const Currency Value, int Decimals=-1, bool ShowCurrencySymbol=true);
double RoundFloat(const double Value, int Decimals, int Precision=15);
AnsiString FloatString(const double Value, const int MinDecimals=0, const int MaxDecimals=4, int Precision=15);
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
