using System;
using System.Collections.Generic;
using SiHotIntegration.Domain;
using System.Linq;
using System.IO;
using System.Text.RegularExpressions;
using SiHotIntegration.Tools;


namespace SiHotIntegration.Utility
{
    public class SiHotDesrializer
    {
        private const string groupSeparator = "\u001d";
        private const string fieldSeparator = "\u001c";

        public SiHotDesrializer()
        { 
        }

        public RoomDetails DeserializeRoomResponse(string response)
        {
            GuestDetails guestDetails;
            RoomDetails roomDetails = new RoomDetails();
            roomDetails.GuestDetailsList = new List<GuestDetails>();
            try
            {
                if (response.Contains("block:guest"))
                {
                    //roomDetails.TransNo = response.Substring(0, response.IndexOf("accountblock")).Replace("transno:","");
                    string details = response.Substring(response.IndexOf("block:guest"), response.Length - response.IndexOf("block:guest"));
                    List<string> guestList = Regex.Split(details, "block:guest").ToList();

                    foreach (string value in guestList)
                    {
                        if (value == "")
                            continue;
                        List<string> guestInfo = new List<string>();
                        guestInfo = Regex.Split(value, fieldSeparator).ToList();
                        guestDetails = new GuestDetails();
                        if (guestInfo.Count > 0)
                        {
                            guestDetails.FirstName = guestInfo.FirstOrDefault(s => s.Contains("firstname:")).Replace("firstname:", "");
                            guestDetails.LastName = guestInfo.FirstOrDefault(s => s.Contains("lastname:")).Replace("lastname:", "");
                            guestDetails.Limit = guestInfo.FirstOrDefault(s => s.Contains("limit:")).Replace("limit:", "").Replace(",", ".");
                            guestDetails.AccountActive = guestInfo.FirstOrDefault(s => s.Contains("accountactive:")).Replace("accountactive:", "");
                            guestDetails.AccountNo = guestInfo.FirstOrDefault(s => s.Contains("accountno:")).Replace("accountno:", "");
                            string bedNo = guestInfo.FirstOrDefault(s => s.Contains("roomno:")).Replace("roomno:", "").Replace("\r\n", "");
                           
                            if(bedNo.Contains(groupSeparator))
                                bedNo = bedNo.Replace(groupSeparator,"");
                            if (bedNo.Contains(fieldSeparator))
                                bedNo = bedNo.Replace(fieldSeparator, "");
                            guestDetails.RoomBedNo = bedNo;
                        }
                        roomDetails.GuestDetailsList.Add(guestDetails);
                    }
                }
                if (roomDetails.GuestDetailsList.Count > 0)
                {
                    roomDetails.IsSuccessful = true;
                    roomDetails.ResponseMessage = "Successful";
                }
                else 
                {
                    roomDetails.IsSuccessful = false;
                    string message = "";
                    message = response.Replace(fieldSeparator,"");
                    roomDetails.ResponseMessage = message.Replace(groupSeparator, "");
                }
                return roomDetails;
                
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Deserializing the response for room request" + ex.Message);
            }
            return roomDetails;
        }
        public RoomChargeResponse DesrializeRoomPostResponse(string response)
        {
            RoomChargeResponse roomPostResponse = new RoomChargeResponse();
            try
            {
                if (response.Contains("transno:") && response.Contains("notok:") &&
                    (response.IndexOf("notok:") > response.IndexOf("transno:")))
                    roomPostResponse.IsSuccessful = false;
                else if (response.Contains("transno:") && response.Contains("ok"))
                    roomPostResponse.IsSuccessful = true;
                else if (!response.Contains("transno:") || response.Trim() == "")
                    roomPostResponse.IsSuccessful = false;
                int i = response.Length;
                List<string> stringList = new List<string>();
                stringList = Regex.Split(response, groupSeparator).ToList();
                stringList.RemoveAll(item => item == groupSeparator);
                stringList.RemoveAll(item => item == fieldSeparator);
                foreach (string stringvalue in stringList)
                {
                    roomPostResponse.Response += stringvalue;
                    roomPostResponse.Response += "\r\n";
                }
                return roomPostResponse;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Deserializing the response for room post" + ex.Message);
            }
            return roomPostResponse;
        }
        public bool DeserializeValidateResponse(string response)
        {
            bool value = false;
            try
            {
                if (response.Contains("block:paymenttype"))
                    value = true;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Deserializing the response for validate" + ex.Message);
            }
            return value;
        }
    }
}
