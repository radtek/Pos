//---------------------------------------------------------------------------

#ifndef ManagerSiHotH
#define ManagerSiHotH
#include "PaymentTransaction.h"
#include "SiHotDataObjects.h"
#include "BasePMS.h"
#include <System.hpp>
//---------------------------------------------------------------------------

class TManagerSiHot : public TBasePMS
{
	public :
       TManagerSiHot();
       ~TManagerSiHot();
       void GetRoomStatus(TPhoenixRoomStatusExt &Status,AnsiString PMSIPAddress,int PMSPort);
       bool RoomChargePost(TPaymentTransaction _paymentTransaction);
       void Initialise();
       bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
    private :
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       bool SendRoomChargePost(TRoomCharge _roomCharge);
};
#endif
