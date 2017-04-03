//---------------------------------------------------------------------------

#ifndef SiHotInterfaceH
#define SiHotInterfaceH
#include "SiHotDataObjects.h"
#include "SiHotWSDL.h"
#include <System.hpp>
#include<list>
#include <memory>
//---------------------------------------------------------------------------
class TSiHotInterface
{
    private:
       _di_ISiHotIntegrationWebService siHotClient; // WCF Client
       void initSIClient();
       void ConvertSiHotRoomResponse(RoomDetails* _roomDetails, TRoomResponse &roomResponse);
       SiHotService* GetServices(std::vector<TSiHotService>::iterator it);
       SiHotPayment* GetPayment(std::vector<TSiHotPayments>::iterator it);
    public:
       TSiHotInterface();
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       TRoomChargeResponse SendRoomChargePost(TRoomCharge _roomCharge);
       bool ValidateIPAddressPort(AnsiString address,int port, int transno);
};
#endif
