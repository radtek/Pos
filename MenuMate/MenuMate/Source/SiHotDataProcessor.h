//---------------------------------------------------------------------------

#ifndef SiHotDataProcessorH
#define SiHotDataProcessorH
#include "PaymentTransaction.h"
#include "SiHotDataObjects.h"
//---------------------------------------------------------------------------

class TSiHotDataProcessor
{
    public:
       void CreateRoomRequest(int _roomNumber, int _transactionNumber, TRoomRequest &_roomRequest);
       void CreateRoomChargePost(TPaymentTransaction _paymentTransaction, TRoomCharge &_roomCharge);
};
#endif
