//---------------------------------------------------------------------------


#pragma hdrstop

#include "Mall.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TMall::SetMallId(int mallID)
{
    _mallId = mallID;
}
//--------------------------------------------------------------------------
void TMall::SetDeviceKey(int deviceKey)
{
    _deviceKey = deviceKey;
}
//---------------------------------------------------------------------------
void TMall::SetMallName(UnicodeString mallName)
{
    _mallName = mallName;
}
//---------------------------------------------------------------------------
void TMall::SetActiveMall(UnicodeString isActive)
{
    _isActive = isActive;
}
//---------------------------------------------------------------------------
 void TMall::SetMallSettings(std::list<TMallExportSettings> mallSettings)
{
    _mallSettings = mallSettings;
}
//-----------------------------------------------------------------------------
