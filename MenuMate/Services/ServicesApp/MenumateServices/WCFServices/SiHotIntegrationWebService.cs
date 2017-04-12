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
            try
            {
                SiHotCommunicationController siCommController = new SiHotCommunicationController();
                roomDetails = siCommController.GetRoomDetails(roomRequest);
                return roomDetails;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in getting Room Details" + ex.Message);
            }
            return roomDetails;

        }
        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails)
        {
            RoomChargeResponse roomChargeReponse = new RoomChargeResponse();
            try
            {
                SiHotCommunicationController siCommController = new SiHotCommunicationController();
                roomChargeReponse = siCommController.PostRoomCharge(roomChargeDetails);
                return roomChargeReponse;
            }
            catch (Exception ex )
            {
                ServiceLogger.Log("Exception in Posting Room Charge" + ex.Message);
            }
            return roomChargeReponse;
        }
        public bool ValidateCreadentials(string address, int port, int transno)
        {
            SiHotCommunicationController siCommController = new SiHotCommunicationController();
            return siCommController.ValidateCredentials(address, port, transno);
        }


    }
}
