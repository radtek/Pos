//---------------------------------------------------------------------------


#pragma hdrstop

#include "Rooms.h"
#include "DBRooms.h"
#include "SelectRoom.h"
#include <algorithm>
#include "MMLogging.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TRooms::TRooms()
{
	Enabled 									= false;
	SelectedRoom							= new TRoomData;
	MaxRememberedRooms					= 20;
}

TRooms::~TRooms()
{
	delete SelectedRoom;
}

void __fastcall TRooms::Initialise(Database::TDBTransaction &DBTransaction)
{
	Enabled = true;
	TDBRooms::SetRoomsUp(DBTransaction);
}


bool __fastcall TRooms::GetEnabled()
{
   return fEnabled;
}

void __fastcall TRooms::SetEnabled(bool value)
{
	fEnabled = value;
}

int TRooms::SelectRoom(Database::TDBTransaction &DBTransaction,eRoomDisplayFilter inFilter)
{
	int RetVal = 0;
	try
	{
		std::auto_ptr<TfrmSelectRoom> frmSelectRoom (TfrmSelectRoom::Create<TfrmSelectRoom>(Screen->ActiveForm,DBTransaction));

		frmSelectRoom->Filter = inFilter;
		RetVal = frmSelectRoom->ShowModal();
		if(RetVal == mrOk)
		{
			SetSelectedRoom(frmSelectRoom->SelectedRoom);
			if(find(LastRoomsAccessed.begin(),LastRoomsAccessed.end(),frmSelectRoom->SelectedRoom->RoomKey) == LastRoomsAccessed.end())
			{
				LastRoomsAccessed.insert(LastRoomsAccessed.begin(),frmSelectRoom->SelectedRoom->RoomKey);
			}
			while(LastRoomsAccessed.size() > MaxRememberedRooms)
			{
         	LastRoomsAccessed.pop_back();
			}
		}
		else
		{
      	ClearSelectedRoom();
		}
	}
	catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
	}
	return RetVal;
}

void TRooms::SetSelectedRoom(TRoomData *inRoom)
{
	SelectedRoom->Assign(inRoom);
}

void TRooms::ClearSelectedRoom()
{
	SelectedRoom->Clear();
}

TRoomData *TRooms::GetSelectedRoom()
{
	return SelectedRoom;
}
