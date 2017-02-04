//---------------------------------------------------------------------------

#ifndef SiHotInterfaceH
#define SiHotInterfaceH
#include "SiHotDataObjects.h"
#include <System.hpp>
//---------------------------------------------------------------------------
class TSiHotInterface
{
    public:
       TRoomResponse SendRoomRequest(TRoomRequest _roomRequest);
       TRoomChargeResponse SendRoomChargePost(TRoomCharge _roomCharge);
};
#endif
