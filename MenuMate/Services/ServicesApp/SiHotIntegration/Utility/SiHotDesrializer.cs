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
                    string t = response.Substring(0, response.IndexOf(groupSeparator));
                    t = t.Replace("transno:", "");
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
                        }
                        roomDetails.GuestDetailsList.Add(guestDetails);
                    }
                    if (roomDetails.GuestDetailsList.Count > 0)
                    {
                        roomDetails.IsSuccessful = true;
                        roomDetails.ResponseMessage = "Successful";
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Deserializing the response for room request" + ex.Message);
            }
            return roomDetails;
        }
    }
}
