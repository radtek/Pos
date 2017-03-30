using System;
using System.Collections.Generic;
using SiHotIntegration.Domain;

namespace SiHotIntegration.Utility
{
    public class SiHotSerializer
    {
        private const string groupSeparatorHex = "1C";
        private const string fieldSeparatorHex = "1D"; 
        
        public SiHotSerializer()
        {
 
        }

        private string GetFieldSeparator()
        {
            string fieldSeparator = "";
            for (int i = 0; i < fieldSeparatorHex.Length / 2; i++)
            {
                string hexChar = fieldSeparatorHex.Substring(i * 2, 2);
                int hexValue = Convert.ToInt32(hexChar, 16);
                fieldSeparator += Char.ConvertFromUtf32(hexValue);
            }
            return fieldSeparator;
        }

        private string GetGroupSeparator()
        {
            string groupSeparatorHex = "";
            for (int i = 0; i < groupSeparatorHex.Length / 2; i++)
            {
                string hexChar = groupSeparatorHex.Substring(i * 2, 2);
                int hexValue = Convert.ToInt32(hexChar, 16);
                groupSeparatorHex += Char.ConvertFromUtf32(hexValue);
            }
            return groupSeparatorHex;
        }

        public string GetRoomRequestContent(RoomRequest roomRequest)
        {
            string content = "";
            string groupSeparator = GetGroupSeparator();
            content = "transno:" + roomRequest.TransactionNumber;
            content += groupSeparator;
            content += "roomreq:" + roomRequest.RoomNumber;
            content += groupSeparator;
            return content;
        }

        public string GetRoomChargeContent(RoomChargeDetails roomDetails)
        {
            string content = "";
            string groupSeparator = GetGroupSeparator();
            string fieldSeparator = GetFieldSeparator();
            return content;
        }
        

    }
}
