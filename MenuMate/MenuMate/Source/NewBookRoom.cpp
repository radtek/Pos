//---------------------------------------------------------------------------


#pragma hdrstop

#include "NewBookRoom.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



TNewBookRoom::TNewBookRoom()
{
	Clear();
}

void TNewBookRoom::Clear()
{
	RoomNo = "";
	Guest_Name = "";
	Allow_Charge = false;
}