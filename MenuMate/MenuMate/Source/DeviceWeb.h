//---------------------------------------------------------------------------

#ifndef DeviceWebH
#define DeviceWebH

#include "DeviceDB.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TDeviceWeb : public TDeviceDB
{
	public:
	TDeviceWeb();
   virtual __fastcall ~TDeviceWeb();
	void CheckLocation(Database::TDBTransaction & DBTransaction);
};

#endif
