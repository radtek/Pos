//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceStatusPalm.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TDeviceStatusPalm::setError(EProcessingErrorType inError)
{
	if(inError == proA_Ok) // You can Reset Error to Good.
   {
   	fError = inError;
   }
   else if(fError == proA_Ok) // Only if Good can you assgin Error Val.
   {
   	fError = inError;
   }
}
