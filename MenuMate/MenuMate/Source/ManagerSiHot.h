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
       void GetRoomStatus(std::vector<TSiHotAccounts> &siHotAccounts,AnsiString PMSIPAddress,int PMSPort);
       void Initialise();
       bool ExportData(TPaymentTransaction &PaymentTransaction, int StaffID);
    private :
       bool RoomChargePost(TPaymentTransaction &_paymentTransaction);
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       bool SendRoomChargePost(TPaymentTransaction &PaymentTransaction);
       bool CheckIPAddressPort(AnsiString tcpIPAddress,int tcpPort);
       bool GetRoundingandDefaultAccount();
};
#endif
