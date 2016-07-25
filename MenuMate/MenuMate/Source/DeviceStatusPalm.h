//---------------------------------------------------------------------------

#ifndef DeviceStatusPalmH
#define DeviceStatusPalmH
//---------------------------------------------------------------------------
#include "DeviceStatusBase.h"
#include "MMLogging.h"

class TDeviceStatusPalm  : public TDeviceStatusBase
{
		private:
		EProcessingErrorType fError;
		void setError(EProcessingErrorType inError);
      public:
	   __property EProcessingErrorType Error = {read=fError, write=setError}; // property declaration
      unsigned char TransactionNumber;
};

#endif
