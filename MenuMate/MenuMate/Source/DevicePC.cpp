//---------------------------------------------------------------------------
#pragma hdrstop
#include "DevicePC.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void TDevicePC::Initialise(Database::TDBTransaction &DBTransaction)
{
	TDeviceDB::Initialise(DBTransaction);
	OS.Initialise(DBTransaction);
	TManagerVariable::Instance().SetDeviceStr(DBTransaction,vmVersionNumber,OS.MMSoftwareVersion);
}
