//---------------------------------------------------------------------------

#ifndef RequestH
#define RequestH
//---------------------------------------------------------------------------
#include "DeviceImage.h"
#include "DeviceDB.h"
#include "MMLogging.h"

class TRequest
{
public :
   struct SDataHeader
   {
      bool AlterDB; // This PC is to Make Alterations to the DB with Reguards to this Message;
		EProcessingErrorType Error;
		UnicodeString ErrorMsg;
		TDeviceImage *RequestingDevice;
      TDeviceDB *Device; // This is a Blank ptr Becareful with it.
   }Header;
	__fastcall TRequest(TDeviceImage *Requester);
	__fastcall~TRequest();
};

#endif
