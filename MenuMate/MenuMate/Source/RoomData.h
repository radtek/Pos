//---------------------------------------------------------------------------

#ifndef RoomDataH
#define RoomDataH
//---------------------------------------------------------------------------
#include <vcl.h>

class TListRoomContainer;

enum eGMRoomStatus {eVacant,eOccupied};

class TRoomData : public TObject
{
private:
	public:
	TListRoomContainer *Owner;
	UnicodeString Name;
	UnicodeString PartyName;
	UnicodeString Caption;
   TDateTime LastOrder;
	UnicodeString Card;
	int RoomNo;
   int BookingID;
   eGMRoomStatus Status;
	int RoomKey;
	TRoomData();
	void Assign(TRoomData * inRoom);
	void Clear();
};

#endif
