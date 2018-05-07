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
        public RoomDetails GetRoomDetails(RoomRequest roomRequest, int timeOut)
        {
            RoomDetails roomDetails = new RoomDetails();
            try
            {
                SiHotCommunicationController siCommController = new SiHotCommunicationController();
                roomDetails = siCommController.GetRoomDetails(roomRequest, timeOut);
                return roomDetails;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in getting Room Details" + ex.Message);
            }
            return roomDetails;

        }
        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails, int timeOut)
        {
            int retryCount = 1;
            bool retryPosting = true;
            RoomChargeResponse roomChargeReponse = new RoomChargeResponse();
            try
            {
                while (retryPosting)
                {
                    SiHotCommunicationController siCommController = new SiHotCommunicationController();
                    roomChargeReponse = siCommController.PostRoomCharge(roomChargeDetails,retryCount,timeOut);
                    if (!roomChargeReponse.IsSuccessful &&
                        (roomChargeReponse.Response == "" || roomChargeReponse.Response == null || 
                        roomChargeReponse.Response == siCommController.connectFailedMessage || roomChargeReponse.Response == siCommController.siHotUnavailable) && 
                        retryCount < 3)
                    {
                        retryCount += 1;
                        retryPosting = true;
                        roomChargeReponse = new RoomChargeResponse();
                        System.Threading.Thread.Sleep(500); 
                    }
                    else
                    {
                        retryPosting = false;
                    }
                }
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
