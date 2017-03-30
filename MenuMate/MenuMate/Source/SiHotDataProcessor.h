//---------------------------------------------------------------------------

#ifndef SiHotDataProcessorH
#define SiHotDataProcessorH
#include "PaymentTransaction.h"
#include "DeviceRealTerminal.h"
#include "SiHotDataObjects.h"
//---------------------------------------------------------------------------

class TSiHotDataProcessor
{
    public:
       TRoomRequest CreateRoomRequest(TPhoenixRoomStatusExt status,AnsiString pmsIPAddress,int pmsPort);
       void CreateRoomChargePost(TPaymentTransaction _paymentTransaction, TRoomCharge &_roomCharge);
       void PrepareRoomStatus(TPhoenixRoomStatusExt &status,TRoomResponse roomResponse);
    private:
       int GetTransNumber();
};
#endif
