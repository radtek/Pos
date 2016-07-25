//---------------------------------------------------------------------------

#ifndef TableSeatH
#define TableSeatH

#include <System.hpp>
//---------------------------------------------------------------------------
class TTableSeat
{
	public:
   int TableNo;
   int SeatNo;
   UnicodeString TableName;
   UnicodeString PartyName;
   UnicodeString SeatName;
   TTableSeat();
};
#endif
