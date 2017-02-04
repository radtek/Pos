//---------------------------------------------------------------------------


#pragma hdrstop

#include "SiHotDataProcessor.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TSiHotDataProcessor::CreateRoomRequest(int _roomNumber, int _transactionNumber, TRoomRequest &_roomRequest)
{
    _roomRequest.TransactionNumber = _roomNumber;
    _roomRequest.RoomNumber = _transactionNumber;
}
void TSiHotDataProcessor::CreateRoomChargePost(TPaymentTransaction _paymentTransaction, TRoomCharge &_roomCharge)
{

}
