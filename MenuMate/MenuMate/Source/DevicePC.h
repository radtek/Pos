//---------------------------------------------------------------------------

#ifndef DevicePCH
#define DevicePCH
//---------------------------------------------------------------------------
#include "DeviceDB.h"
#include "DeviceStatusBase.h"
#include "MMSettings.h"
#include "Modules.h"
#include "OpSys.h"

class TDevicePC : public TDeviceDB
{
	public:

   TMMSettings System;
   TModules Modules;
   OpSys OS;

	virtual void Initialise(Database::TDBTransaction &DBTransaction);

};

#endif
