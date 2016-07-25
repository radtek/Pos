//---------------------------------------------------------------------------

#ifndef ListRoomContainerH
#define ListRoomContainerH
//---------------------------------------------------------------------------
#include "RoomData.h"

class TListRoomContainer : public TList
{
   public:
   void __fastcall Clear();
   void Delete(TRoomData *in);
   int Add(TRoomData *in);
	TRoomData *Get(int Index);
	TRoomData *GetByRoomNo(int inRoomNo);
	TRoomData *GetByRoomKey(int inRoomKey);
	TListRoomContainer();
	void SortByRoomNumber();
	void SortByRoomName();
	void SortByGuest();
	__fastcall virtual ~TListRoomContainer();
};
#endif
