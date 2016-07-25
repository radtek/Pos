//---------------------------------------------------------------------------


#pragma hdrstop

#include "rounding.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//Currency RoundToNearest( Currency Value, Currency RoundTo )
//{
//    return RoundToNearest( Value, RoundTo, TGlobalSettings::Instance().MidPointRoundsDown );
//}


Currency RoundToNearest( Currency Value, Currency RoundTo, bool RoundDown )
{
    bool negativeValue = Value < 0;
    if( negativeValue )
    {
        Value *= -1;
    }

    if( RoundTo < 0 )
    {
        RoundTo *= -1;
    }

    RoundTo.Val -= RoundTo.Val % Currency( MIN_CURRENCY_VALUE ).Val;

    if( RoundTo < Currency( MIN_CURRENCY_VALUE ) )
    {
        RoundTo = Currency( MIN_CURRENCY_VALUE );
    }
    __int64 RoundToVal = RoundTo.Val;
    __int64 RoundingVal = Value.Val;
    __int64 Rounder = ( RoundDown ? 4 : 5 );

    RoundingVal /= ( RoundToVal / 10 );
    RoundingVal += Rounder;
    RoundingVal /= 10;
    RoundingVal *= RoundToVal;

    Currency RoundedValue;
    RoundedValue.Val = RoundingVal;

    return negativeValue ? -RoundedValue : RoundedValue;
}
// ---------------------------------------------------------------------------
