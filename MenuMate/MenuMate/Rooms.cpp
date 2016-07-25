//---------------------------------------------------------------------------


#pragma hdrstop

#include "Rooms.h"
#include "DBRooms.h"
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
			if(find(LasTDBRoomsAccessed.begin(),LasTDBRoomsAccessed.end(),frmSelectRoom->SelectedRoom->RoomKey) == LasTDBRoomsAccessed.end())
			{
				LasTDBRoomsAccessed.insert(LasTDBRoomsAccessed.begin(),frmSelectRoom->SelectedRoom->RoomKey);
			}
			while(LasTDBRoomsAccessed.size() > MaxRememberedRooms)
			{
         	LasTDBRoomsAccessed.pop_back();
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
