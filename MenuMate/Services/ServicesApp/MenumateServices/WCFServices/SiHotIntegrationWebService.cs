using System;
using SiHotIntegration;
using SiHotIntegration.Domain;
using System.Collections.Generic;
using System.IO;

namespace MenumateServices.WCFServices
{
    public class SiHotIntegrationWebService : ISiHotIntegrationWebService
    {
        public SiHotIntegrationWebService()
        {
        }
        public RoomDetails GetRoomDetails(RoomRequest roomRequest, int timeOut, string apiKey)
        {
            RoomDetails roomDetails = new RoomDetails();
            try
            {
                SiHotCommunicationController siCommController = new SiHotCommunicationController();
                roomDetails = siCommController.GetRoomDetails(roomRequest, timeOut, apiKey);
                return roomDetails;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in getting Room Details" + ex.Message);
            }
            return roomDetails;

        }
        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails, int timeOut, string apiKey)
        {
            int retryCount = 1;
            bool retryPosting = true;
            RoomChargeResponse roomChargeReponse = new RoomChargeResponse();
            try
            {
                while (retryPosting)
                {
                    roomChargeReponse = new RoomChargeResponse();
                    SiHotCommunicationController siCommController = new SiHotCommunicationController();
                    roomChargeReponse = siCommController.PostRoomCharge(roomChargeDetails, retryCount, timeOut, apiKey);
                    if (!roomChargeReponse.IsSuccessful &&
                        (roomChargeReponse.Response == "" || roomChargeReponse.Response == null ||
                        roomChargeReponse.Response == siCommController.connectFailedMessage || roomChargeReponse.Response == siCommController.siHotUnavailable) &&
                        retryCount < 3)
                    {
                        retryCount += 1;
                        retryPosting = true;
                        roomChargeReponse = new RoomChargeResponse();
                        System.Threading.Thread.Sleep(500);
                        List<string> stringList = new List<string>();
                        stringList.Add("==============================================================================");
                        stringList.Add("Retry detected:                       " + DateTime.Now.ToString("hh:mm:ss tt"));
                        stringList.Add("==============================================================================");
                        WriteToFile(stringList);
                    }
                    else
                    {
                        retryPosting = false;
                        if (retryCount >= 3 || roomChargeReponse.IsSuccessful)
                            break;
                    }
                }
            }
            catch (Exception ex)
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
        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                string location = Path.Combine(path, "logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                location = Path.Combine(location, "Sihot Post Logs");

                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "SiHotPosts " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
                string fileName = Path.Combine(location, name2);

                if (fileName.Contains(@"file:\"))
                {
                    fileName = fileName.Replace(@"file:\", "");
                }
                if (!File.Exists(fileName))
                {

                    using (StreamWriter sw = File.CreateText(fileName))
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            sw.WriteLine(list[i]);
                        }
                    }
                }
                else
                {
                    using (var sw = File.AppendText(fileName))
                    {
                        for (int i = 0; i < list.Count; i++)
                        {
                            sw.WriteLine(list[i]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Making File" + ex.Message);
            }
        }
    }
}
