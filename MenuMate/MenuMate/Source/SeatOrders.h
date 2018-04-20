//---------------------------------------------------------------------------

#ifndef SeatOrdersH
#define SeatOrdersH
//---------------------------------------------------------------------------
#include "ContainerOrders.h"


struct TCustomerDisp
{   UnicodeString       changeStr  ;
	bool		FirstVisit;	   // Null if local printer.
	bool		HappyBirthDay;
    int		TierLevel;

};

class TSeatOrders
{
private:
public:
	TSeatOrders(int inSeatNumber);
	~TSeatOrders();
	UnicodeString SeatName;
	int SeatNumber;
	TContainerOrders *Orders;
    UnicodeString RoomNumber;
};

#endif
