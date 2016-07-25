//---------------------------------------------------------------------------


#pragma hdrstop

#include "RMSRoom.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TRMSRoom::TRMSRoom()
{
	Clear();
}

void TRMSRoom::Clear()
{
	RoomNo = "";
	AccountNo = -1;
	Name= "";
	Active = false;
}
