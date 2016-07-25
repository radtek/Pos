//---------------------------------------------------------------------------


#pragma hdrstop

#include "SmartCardException.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Smart Card Exception Handler.
TSCException::TSCException(TSCExceptionType inType,UnicodeString inErrorMsg) : Exception(inErrorMsg)
{
   Type = inType;
}