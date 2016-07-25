//---------------------------------------------------------------------------


#pragma hdrstop

#include "RoomData.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TRoomData::TRoomData()
{
	RoomNo 		= 0;
	BookingID 	= 0;
	RoomKey 		= 0;
	Card 			= "";
	Name			= "";
	Caption		= "";
	PartyName	= "";
	LastOrder = Now();
	Status = eVacant;
}

void TRoomData::Assign(TRoomData * inRoom)
{
	if(inRoom != NULL)
	{
		Name			= inRoom->Name;
		PartyName	= inRoom->PartyName;
		Caption		= inRoom->Caption;
		Card			= inRoom->Card;
		LastOrder	= inRoom->LastOrder;
		RoomNo		= inRoom->RoomNo;
		BookingID	= inRoom->BookingID;
		RoomKey		= inRoom->RoomKey;
		Status		= inRoom->Status;
	}
}

void TRoomData::Clear()
{
	RoomNo 		= 0;
	BookingID 	= 0;
	RoomKey 		= 0;
	Card 			= "";
	Name			= "";
	Caption		= "";
	PartyName	= "";
	LastOrder = Now();
	Status = eVacant;
}