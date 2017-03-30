using System;
using System.Text;
using System.Net;
using System.Net.Http;
using SiHotIntegration;
using SiHotIntegration.Domain;
using SiHotIntegration.Utility;
using System.Collections.Generic;

namespace MenumateServices.WCFServices
{
    public class SiHotIntegrationWebService : ISiHotIntegrationWebService
    {        
        public SiHotIntegrationWebService()
        {
        }
        public RoomDetails GetRoomDetails(RoomRequest roomRequest)
        {
            RoomDetails roomDetails = new RoomDetails();
            SiHotCommunicationController siCommController = new SiHotCommunicationController();
            roomDetails = siCommController.GetRoomDetails(roomRequest);
            return roomDetails;

        }
        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails)
        {
            RoomChargeResponse roomChargeReponse = new RoomChargeResponse();
            SiHotCommunicationController siCommController = new SiHotCommunicationController();
            return roomChargeReponse;
        }



    }
}
