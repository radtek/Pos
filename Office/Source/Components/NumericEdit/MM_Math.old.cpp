//---------------------------------------------------------------------------
#pragma hdrstop

#include <sysutils.hpp>
#include "MM_Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//using namespace MMMath;
//---------------------------------------------------------------------------
Currency MMMath::RoundCurrency(const Currency Value, const int Decimals)
{
	int DecimalsUsed = Decimals;
	if (Decimals < 0)
	{
		DecimalsUsed = ::CurrencyDecimals;
	}
	int Mul = 1;
	for (int j=0; j<DecimalsUsed; j++, Mul *= 10);
	if (Value > 0)	return Currency(int(Value * Mul + 0.5)) / Mul;
	else				return Currency(int(Value * Mul - 0.5)) / Mul;
}
//---------------------------------------------------------------------------
Currency MMMath::RoundCurrencyTo5c(const Currency Value)
{
	if (Value > 0)	return Currency(int(Value * 20 + 0.5)) / 20;
	else				return Currency(int(Value * 20 - 0.5)) / 20;
}
//---------------------------------------------------------------------------
AnsiString MMMath::CurrencyString(const Currency Value, int Decimals, bool ShowCurrencySymbol)
{
	AnsiString Head;
	if (ShowCurrencySymbol)
	{
		Head = Sysutils::CurrencyString;
	}
	if (Decimals == 0 || (Decimals < 0 && ::CurrencyDecimals == 0))
	{
		return Head + AnsiString::FormatFloat("0", Value);
	}
	else if (Decimals < 0)
	{
		return Head + AnsiString::FormatFloat("0." + AnsiString::StringOfChar('0', CurrencyDecimals), MMMath::RoundCurrency(Value, CurrencyDecimals));
	}
	else
	{
		return Head + AnsiString::FormatFloat("0." + AnsiString::StringOfChar('0', Decimals), MMMath::RoundCurrency(Value, Decimals));
	}
}
//---------------------------------------------------------------------------
double MMMath::RoundFloat(const double Value, int Decimals, int Precision)
{
//	doubles are never precise. 1.35 could be 1.349999999456747. Precision should be 9. Add 0.000000001
	if (Decimals > 0)
	{
		int PrecisionUse = Precision;
		if (PrecisionUse <= Decimals)
		{
			PrecisionUse = Decimals + 1;
		}
		double PrecisionMul = 1;
		for (int j=0; j<PrecisionUse; j++, PrecisionMul *= 10);
		unsigned __int64 SignificantDigits;
		try
		{
			if (Value > 0)
			{
				SignificantDigits = Value * PrecisionMul + double(0.1);
			}
			else
			{
				SignificantDigits = -Value * PrecisionMul + double(0.1);
			}
		}
		catch (EInvalidOp &E)
		{
			return Value;
		}

		double Div = 1;
		for (int j=0; j<PrecisionUse - Decimals - 1; j++, Div *= 10);
		double Mul = 1;
		for (int j=0; j<Decimals + 1; j	++, Mul *= 10);
		if (Value > 0)
		{
			return double((unsigned __int64)((double((unsigned __int64)(SignificantDigits / Div)) + double(0.5)))) / double(Mul);
		}
		else
		{
			return -double(int((double(int(SignificantDigits / Div)) + double(0.5)))) / double(Mul);
		}
	}
	else
	{
		return Value;
	}
}
//---------------------------------------------------------------------------
AnsiString MMMath::FloatString(const double Value, const int MinDecimals, const int MaxDecimals, const int Precision)
{
	int MaxDecimalsUsed = MaxDecimals;
	if (MaxDecimals < MinDecimals)
	{
		MaxDecimalsUsed = MinDecimals;
	}
	if (MinDecimals <= 0)
	{
		if (MaxDecimalsUsed <= 0)
		{
			return AnsiString::FormatFloat("0", Value);
		}
		else
		{
			return AnsiString::FormatFloat("0." + AnsiString::StringOfChar('#', MaxDecimalsUsed), MMMath::RoundFloat(Value, MaxDecimalsUsed, Precision));
		}
	}
	else
	{
		if (MaxDecimalsUsed > MinDecimals)
		{
			return AnsiString::FormatFloat("0." + AnsiString::StringOfChar('0', MinDecimals) + AnsiString::StringOfChar('#', MaxDecimalsUsed - MinDecimals), MMMath::RoundFloat(Value, MaxDecimals, Precision));
		}
		else
		{
			return AnsiString::FormatFloat("0." + AnsiString::StringOfChar('0', MinDecimals), MMMath::RoundFloat(Value, MaxDecimals, Precision));
		}
	}
}
//---------------------------------------------------------------------------

