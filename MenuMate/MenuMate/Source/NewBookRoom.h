//---------------------------------------------------------------------------

#ifndef NewBookRoomH
#define NewBookRoomH
//---------------------------------------------------------------------------

#include <System.hpp>
//---------------------------------------------------------------------------
class TNewBookRoom
{
	public :
		TNewBookRoom();
		void Clear();
        int RoomCount;
		UnicodeString RoomNo;
		UnicodeString Guest_Name;
		bool Allow_Charge;
};

bool operator == (const TNewBookRoom& x , const TNewBookRoom& y)
{
   if (x.RoomCount == y.RoomCount) return true;
   else return false;
}
#endif
