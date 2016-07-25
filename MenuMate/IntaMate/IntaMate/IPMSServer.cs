using System;
using System.Collections.Generic;

namespace IntaMate
{
    interface IPMSInterface
    {
        bool Online();
        PMSRoomChargeResponse RoomCharge(PhoenixPacketChargeToRoom ChargeToRoom);
        PMSRoomStatusResponse RoomInquiry(PMSRoomStatus Status);
        bool TestConnection();
    }

    abstract class APMSInterface : IntaMate.SerialBase,IntaMate.IPMSInterface 
    {
        protected const int ESTABISHMENT_LENGTH = 14;
        protected const int STATION_LENGTH = 4;
        protected const int PMSREF_LENGTH = 14;
        protected const int DOLLARS_AMOUNT_LENGTH = 8;
        protected const int CENTS_AMOUNT_LENGTH = 2;
        protected const int FOILO_LENGTH = 16;
        protected const int COVERS_LENGTH = 4;
        
        abstract public bool Online();
        abstract public PMSRoomChargeResponse RoomCharge(PhoenixPacketChargeToRoom ChargeToRoom);
        abstract public PMSRoomStatusResponse RoomInquiry(PMSRoomStatus Status);
        abstract public bool TestConnection();

        abstract protected byte[] Posting();
        abstract protected void PostingResponse(List<byte> Buffer);
        abstract protected byte[] Inquiry();
        abstract protected void InquiryResponse(List<byte> Buffer);
    }
}
