//---------------------------------------------------------------------------


#pragma hdrstop

#include "WeightStreamConverter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TWSCException::TWSCException(TWSCExceptionType inType,AnsiString inErrorMsg) : Exception(inErrorMsg)
{
   Type = inType;
}
