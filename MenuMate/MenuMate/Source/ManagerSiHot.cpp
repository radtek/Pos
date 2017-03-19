//---------------------------------------------------------------------------


#pragma hdrstop

#include "ManagerSiHot.h"
#include "SiHotDataProcessor.h"
#include "SiHotInterface.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
TManagerSiHot::TManagerSiHot()
{

}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::GetRoom(int _roomNumber, int _transactionNumber)
{
      // Call to SiHotDataProcessor for making Room Request
      std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
      TRoomRequest roomRequest;
      siHotDataProcessor->CreateRoomRequest(_roomNumber, _transactionNumber, roomRequest);
      TRoomResponse roomResponse;
      roomResponse = SendRoomRequest(roomRequest);
      return roomResponse;
}
//---------------------------------------------------------------------------
bool TManagerSiHot::RoomChargePost(TPaymentTransaction _paymentTransaction)
{
     // Call to SiHotDataProcessor for making Room Charge Post
      std::auto_ptr<TSiHotDataProcessor> siHotDataProcessor(new TSiHotDataProcessor());
      TRoomCharge roomCharge;
      siHotDataProcessor->CreateRoomChargePost(_paymentTransaction, roomCharge);

      return SendRoomChargePost(roomCharge);
}
//---------------------------------------------------------------------------
TRoomResponse TManagerSiHot::SendRoomRequest(TRoomRequest _roomRequest)
{
     // Call to SiHotInterface for sending Room Request
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     return siHotInterface->SendRoomRequest(_roomRequest);
}
//---------------------------------------------------------------------------
bool TManagerSiHot::SendRoomChargePost(TRoomCharge _roomCharge)
{
     // Call to SiHotInterface for sending Room Charge Post
     std::auto_ptr<TSiHotInterface> siHotInterface(new TSiHotInterface());
     TRoomChargeResponse roomResponse = siHotInterface->SendRoomChargePost(_roomCharge);
     return true;
}
//---------------------------------------------------------------------------
