//---------------------------------------------------------------------------

#ifndef roundingH
#define roundingH
#include <System.hpp>
//---------------------------------------------------------------------------
#define MIN_CURRENCY_VALUE 0.01
//---------------------------------------------------------------------------

//Currency RoundToNearest(Currency Value, Currency RoundTo = Currency(MIN_CURRENCY_VALUE) );
Currency RoundToNearest(Currency Value, Currency RoundTo, bool RoundDown);

#endif
