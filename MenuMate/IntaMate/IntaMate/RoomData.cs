using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.Specialized;

namespace IntaMate
{
     
    public class RoomData
    {
        public String RegisterNumber;
        public String RoomNumber;
        public String Establishment;
        public String StaffID;
        public String Covers;

        public String ErrorResponse;
        public String PMSRef;

        // Postings.
        public String PostingTotal;
        public String PostingFoilo;
        public char PostingFoiloNumber;
        public String PostingResponse;

        public Dictionary<String, int> Catagories = new Dictionary<String, int>();
        
        public RoomData()
        {
            Establishment = "MenuMate";
            
            RegisterNumber = "";
            RoomNumber = "";
            ErrorResponse = "";
            PMSRef = "";

            PostingFoilo = "";
            PostingFoiloNumber = '0';
        }

        internal void Import(PMSRoomStatus Status)
        {
            RoomNumber = Status.RoomNumber;
            RegisterNumber = Status.RegisterNumber;
        }

        internal void Import(Dictionary<string, List<string>> cache,PhoenixPacketChargeToRoom status)
        {
            RoomNumber = status.RoomNumber;
            RegisterNumber = status.RegisterNo;
            StaffID = status.ServerNo;
            Covers = status.Covers;
            Catagories = status.Catagories;

            if (IntaMate.Properties.Settings.Default.PMSPassThough)
            {
                PMSRef = status.PMSReference;
            }

            if(cache.ContainsKey(status.RoomNumber))
            {
                List<String> Foilos = cache[status.RoomNumber];
                if (status.FoiloIndex <= Foilos.Count)
                {
                    switch (status.FoiloIndex)
                    {
                        case 1:
                            PostingFoilo = Foilos[0];
                            PostingFoiloNumber = '1';
                            break;
                        case 2:
                            PostingFoilo = Foilos[1];
                            PostingFoiloNumber = '2';
                            break;
                        case 3:
                            PostingFoilo = Foilos[2];
                            PostingFoiloNumber = '3';
                            break;
                        case 4:
                            PostingFoilo = Foilos[3];
                            PostingFoiloNumber = '4';
                            break;
                        default:
                            PostingFoilo = "".PadLeft(16,' ');
                            PostingFoiloNumber = '0';                
                            break;
                    }
                }
                else
                {
                    PostingFoilo = "".PadLeft(16,' ');
                    PostingFoiloNumber = '0';                
                }
            }
            else
            {
                PostingFoilo = "".PadLeft(16, ' ');
                PostingFoiloNumber = '0';
            }
            PostingTotal = status.Total;
        }

        internal PMSRoomStatusResponse ExportPMSRoomStatusResponse(Dictionary<string, List<string>> cache)
        {
            PMSRoomStatusResponse ExRoomStatusResponse = new PMSRoomStatusResponse();
            ExRoomStatusResponse.RoomNumber = RoomNumber;
            ExRoomStatusResponse.RegisterNo = RegisterNumber;
            ExRoomStatusResponse.ErrorResponse = ErrorResponse;

            ExRoomStatusResponse.Foilos.Clear();
            ExRoomStatusResponse.Foilos = cache[RoomNumber];
            return ExRoomStatusResponse;
        }

        internal PMSRoomStatus ExportPMSRoomStatus()
        {
            PMSRoomStatus ExRoomStatus = new PMSRoomStatus();
            ExRoomStatus.RegisterNumber = RegisterNumber;
            ExRoomStatus.RoomNumber = RoomNumber;
            return ExRoomStatus;
        }

        internal PMSRoomChargeResponse ExportPMSRoomChargeResponse()
        {
            PMSRoomChargeResponse Response = new PMSRoomChargeResponse();
            Response.RoomNumber = RoomNumber;
            Response.RegisterNo = RegisterNumber;
           
            if (PostingResponse == "ACK")
            {
                Response.ACK();
            }
            else
            {
                Response.Response = PostingResponse;
            }
            return Response;
        }
    }
}
