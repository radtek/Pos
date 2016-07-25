//---------------------------------------------------------------------------

#ifndef DeviceImageH
#define DeviceImageH
//---------------------------------------------------------------------------
#include "DeviceInfo.h"
#include <Classes.hpp>

class TDeviceImage
{
	public:
   TDeviceInfo ID;

   TDeviceImage();
   void Decode(TMemoryStream * Stream);
	void Encode(TMemoryStream * Stream);
   void Copy(TDeviceImage *Image);
	virtual void Initialise();
};

#endif
