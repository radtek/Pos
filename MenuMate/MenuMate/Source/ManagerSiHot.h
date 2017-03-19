//---------------------------------------------------------------------------

#ifndef ManagerSiHotH
#define ManagerSiHotH
#include "PaymentTransaction.h"
#include "SiHotDataObjects.h"
#include <System.hpp>
//---------------------------------------------------------------------------

class TManagerSiHot
{
	public :
       TManagerSiHot();
       ~TManagerSiHot();
       TRoomResponse GetRoom(int _roomNumber, int _transactionNumber);
       bool RoomChargePost(TPaymentTransaction _paymentTransaction);
    private :
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       bool SendRoomChargePost(TRoomCharge _roomCharge);
};
#endif
