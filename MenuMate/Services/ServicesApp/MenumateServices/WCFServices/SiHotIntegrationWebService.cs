﻿using System;
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
            int retryCount = 1;
            bool retryPosting = true;
            RoomChargeResponse roomChargeReponse = new RoomChargeResponse();
            try
            {
                SiHotCommunicationController siCommController = new SiHotCommunicationController();
                while (retryPosting)
                {
                    roomChargeReponse = siCommController.PostRoomCharge(roomChargeDetails,retryCount);
                    if (!roomChargeReponse.IsSuccessful && (roomChargeReponse.Response == "" || roomChargeReponse.Response == null) && retryCount < 3)
                    {
                        retryCount += 1;
                        retryPosting = true;
                        roomChargeReponse = new RoomChargeResponse();
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
