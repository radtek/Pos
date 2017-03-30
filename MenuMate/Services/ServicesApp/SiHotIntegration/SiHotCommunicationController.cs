using System;
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
                    string content = serializer.GetRoomRequestContent(roomRequest);
                    byte[] bytes = Encoding.UTF8.GetBytes(content);
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
                string content = serializer.GetRoomChargeContent(roomChargeDetails);
                byte[] bytes = Encoding.UTF8.GetBytes(content);
                request.ContentLength = bytes.Length;
                request.Timeout = 500000;
                request.ContentType = "text/plain";
                request.GetRequestStream().Write(bytes, 0, bytes.Length);
                WebResponse wr = request.GetResponse();
                var memberStream = new StreamReader(wr.GetResponseStream());
            }
            catch (Exception ex)
            {
		        ServiceLogger.Log("Exception in sending Room request" + ex.Message);
            }
            return response;
        }
    }
}
