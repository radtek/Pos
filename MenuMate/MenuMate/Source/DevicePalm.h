//---------------------------------------------------------------------------

#ifndef DevicePalmH
#define DevicePalmH
//---------------------------------------------------------------------------
#include "DeviceDB.h"
#include "DeviceStatusPalm.h"
#include "MMSettings.h"
#include "Modules.h"
#include "MMContactInfo.h"

class TDevicePalm : public TDeviceDB
{
	public:
	TMMSettings System;
	TModules Modules;
	TDeviceStatusPalm Status;
	TMMContactInfo User;
};

#endif
