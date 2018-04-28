//---------------------------------------------------------------------------


#pragma hdrstop

#include "SeatOrders.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TSeatOrders::TSeatOrders(int inSeatNumber)
{
	SeatNumber = inSeatNumber;
	Orders = new TContainerOrders;
    isDefaultGuest = false;
    wasGuestSelected = false;
}

TSeatOrders::~TSeatOrders()
{
	delete Orders;
}