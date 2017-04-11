using System;
using System.Linq;
using System.Collections.Generic;
using System.Net;
using SiHotIntegration.Domain;
using SiHotIntegration.Utility;
using System.Text;
using System.IO;
using SiHotIntegration.Tools;

namespace SiHotIntegration
{
    public class SiHotCommunicationController
    {
        public SiHotCommunicationController()
        { 
        }

        public string URIRoomRequest(string ipAddress, int portNumber)
        {
            string uri = @"http://"+ ipAddress + @":" + portNumber + @"/RMS" + @"/roomrequest" + @"/";
            return uri;
        }
        public string URIRoomChargePost(string ipAddress, int portNumber)
        {
            string uri = @"http://"+ ipAddress + @":" + portNumber + @"/RMS" + @"/accountbook" + @"/";
            return uri;
        }
        public string URIValidate(string ipAddress, int portNumber)
        {
            string uri = @"http://" + ipAddress + @":" + portNumber + @"/RMS" + @"/paymenttype" + @"/";
            return uri;
        }
        public RoomDetails GetRoomDetails(RoomRequest roomRequest)
        {
                SiHotSerializer serializer = new SiHotSerializer();
                SiHotDesrializer deserializer = new SiHotDesrializer();
                RoomDetails roomDetails = new RoomDetails();
                try 
	            {	        
                    string uri = URIRoomRequest(roomRequest.IPAddress, roomRequest.PortNumber);
                    var request = (HttpWebRequest)WebRequest.Create(new Uri(uri));
                    request.Method = WebRequestMethods.Http.Post;
                    request.ContentType = "text/plain"; 
                    List<string> detailsList = serializer.GetRoomRequestContent(roomRequest);

                    byte seperator = 29;
                    var byteList = new List<byte>();
                    for (int i = 0; i < detailsList.Count; i++)
                    {
                        var str = detailsList[i];
                        byteList.AddRange(Encoding.UTF8.GetBytes(str).ToList<byte>());
                        byteList.Add(seperator); 
                    }
                    var bytes = byteList.ToArray<byte>();
                    
                    request.ContentLength = bytes.Length;
                    request.Timeout = 500000;
                    request.ContentType = "text/plain";
                    request.GetRequestStream().Write(bytes, 0, bytes.Length);
                    WebResponse wr = request.GetResponse();
                    var memberStream = new StreamReader(wr.GetResponseStream());
                    roomDetails = deserializer.DeserializeRoomResponse(memberStream.ReadToEnd());
	            }
	            catch (Exception ex)
	            {
		            ServiceLogger.Log("Exception in sending Room request" + ex.Message);
	            }
            return roomDetails;
        }

        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails)
        {
		    RoomChargeResponse response = new RoomChargeResponse();
            SiHotSerializer serializer = new SiHotSerializer();
            SiHotDesrializer deserializer = new SiHotDesrializer();            
            try 
            {
                string uri = URIRoomChargePost(roomChargeDetails.IPAddress, roomChargeDetails.PortNumber);
                var request = (HttpWebRequest)WebRequest.Create(new Uri(uri));
                request.Method = WebRequestMethods.Http.Post;
                request.ContentType = "text/plain";
                List<byte> bytesList = serializer.GetRoomChargeContent(roomChargeDetails);
                byte[] bytes = bytesList.ToArray<byte>();
                request.ContentLength = bytes.Length;
                request.Timeout = 500000;
                request.ContentType = "text/plain";
                request.GetRequestStream().Write(bytes, 0, bytes.Length);
                WebResponse webResponse = request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = deserializer.DesrializeRoomPostResponse(memberStream.ReadToEnd());
            }
            catch (Exception ex)
            {
		        ServiceLogger.Log("Exception in sending Room request" + ex.Message);
            }
            return response;
        }

        public bool ValidateCredentials(string address, int port, int transno)
        {
            bool value = false;
            SiHotSerializer serializer = new SiHotSerializer();
            SiHotDesrializer deserializer = new SiHotDesrializer();
            try
            {
                string uri = URIValidate(address, port);
                var request = (HttpWebRequest)WebRequest.Create(new Uri(uri));
                request.Method = WebRequestMethods.Http.Post;
                request.ContentType = "text/plain";
                List<byte> bytesList = serializer.GetValidateContent(transno);
                byte[] bytes = bytesList.ToArray<byte>();
                request.ContentLength = bytes.Length;
                request.Timeout = 500000;
                request.ContentType = "text/plain";
                request.GetRequestStream().Write(bytes, 0, bytes.Length);
                WebResponse wr = request.GetResponse();
                var memberStream = new StreamReader(wr.GetResponseStream());
                value = deserializer.DeserializeValidateResponse(memberStream.ReadToEnd());
                return value;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in sending Room request" + ex.Message);
            }
            return value;
        }
    }
}
