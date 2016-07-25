//---------------------------------------------------------------------------

#ifndef RoomsH
#define RoomsH
//---------------------------------------------------------------------------
#include "MM_DBCore.h"

#include "DBRooms.h"
#include "RoomData.h"

class TRooms
{
	private :
		bool fEnabled;
		void __fastcall SetEnabled(bool value);
		bool __fastcall GetEnabled();
		int MaxRememberedRooms;
	public:

	TRooms();
	virtual ~TRooms();

  	TRoomData *SelectedRoom;

	static TRooms& Instance() {
        static TRooms singleton;
        return singleton;
    }

	__property bool Enabled  = { read=GetEnabled, write=SetEnabled };
	void __fastcall Initialise(Database::TDBTransaction &DBTransaction);

	int SelectRoom(Database::TDBTransaction &DBTransaction,eRoomDisplayFilter inFilter = eRoomFilterOccupied);

	void SetSelectedRoom(TRoomData *inRoom);
	TRoomData *GetSelectedRoom();

	void ClearSelectedRoom();

	std::vector<long> LastRoomsAccessed;
};
#endif
