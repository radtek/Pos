//---------------------------------------------------------------------------

#ifndef DBRoomsH
#define DBRoomsH
//---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include "ListRoomContainer.h"

enum eRoomDisplayFilter {eRoomFilterOccupied,eRoomFilterVacant,eRoomFilterAll};

class TDBRooms
{
	private :
	public:

	TDBRooms();
	virtual ~TDBRooms();

	static void SetRoomsUp(Database::TDBTransaction &DBTransaction);
	static int GetRoomCount(Database::TDBTransaction &DBTransaction);
	static void GetRoomData(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter);
	static void SetRoomName(Database::TDBTransaction &DBTransaction,int RoomNumber, UnicodeString RoomName);
	static void SetPartyName(Database::TDBTransaction &DBTransaction,int RoomNumber, UnicodeString RoomName);
	static void SetBookingID(Database::TDBTransaction &DBTransaction,int RoomNumber, int BookingID);
	static void SetRoomStatus(Database::TDBTransaction &DBTransaction,int RoomNumber,eGMRoomStatus inStatus);

	static UnicodeString GetRoomName(Database::TDBTransaction &DBTransaction,int RoomNumber);

	static UnicodeString GetPartyName(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static int GetRoomTab(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static int GetRoomNumber(Database::TDBTransaction &DBTransaction,int TabKey);
	static int GetBookingID(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static eGMRoomStatus GetRoomStatus(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static bool IsEmpty(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static int FindOrCreateRoomTab(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static int FindOrCreateRoom(Database::TDBTransaction &DBTransaction,int RoomNumber);
	static void DisconnectRoomTab(Database::TDBTransaction &DBTransaction,int RoomNumber);

	static void __fastcall Initialise(Database::TDBTransaction &DBTransaction);

	static int SelectRoom(Database::TDBTransaction &DBTransaction,eRoomDisplayFilter inFilter = eRoomFilterOccupied);
	static void GetList(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter = eRoomFilterOccupied);
	static void GetListNumeric(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter = eRoomFilterOccupied);
	static void GetListGuest(Database::TDBTransaction &DBTransaction,TListRoomContainer *RoomsList,eRoomDisplayFilter inFilter = eRoomFilterOccupied);
	static bool GetRoom(Database::TDBTransaction &DBTransaction,int RoomKey, TRoomData * TempRoom);
};
#endif
