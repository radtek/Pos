//---------------------------------------------------------------------------

#ifndef RMSRoomH
#define RMSRoomH

#include <System.hpp>
//---------------------------------------------------------------------------
class TRMSRoom
{
	public :
		TRMSRoom();
		void Clear();
		UnicodeString RoomNo;
		int AccountNo;
		UnicodeString Name;
		bool Active;
};

bool operator == (const TRMSRoom& x , const TRMSRoom& y)
{
   if (x.AccountNo == y.AccountNo) return true;
   else return false;
}
#endif
