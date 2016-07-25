using System;
namespace IntaMate
{
    interface IPMSInterface
    {
        bool Connect();
        PMSRoomChargeResponse Charge(PhoenixPacketChargeToRoom ChargeToRoom);
        PMSRoomStatusResponse Inquiry(PMSRoomStatus Status);
        void SaveConfig();
        bool IsConnected();
    }
}
