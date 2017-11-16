﻿using System;
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
            System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls | SecurityProtocolType.Ssl3;
        }

        public string URIRoomRequest(string ipAddress, int portNumber)
        {
            //string uri = @"";
            //if(!ipAddress.Contains(":"))
            //     uri = @"http://"+ ipAddress + @":" + portNumber + @"/RMS" + @"/roomrequest" + @"/";
            //else
            //     uri = @"http://" + ipAddress + @"/roomrequest" + @"/";
            string uri = ipAddress + @"/roomrequest" + @"/";
            return uri;
        }
        public string URIRoomChargePost(string ipAddress, int portNumber)
        {
            //string uri = @"";
            //if(!ipAddress.Contains(":"))
            //    uri = @"http://"+ ipAddress + @":" + portNumber + @"/RMS" + @"/accountbook" + @"/";
            //else
            //    uri = @"http://" + ipAddress + @"/accountbook" + @"/";
            string uri = ipAddress + @"/accountbook" + @"/";
            return uri;
        }
        public string URIValidate(string ipAddress, int portNumber)
        {
            //string uri = @"";
            //if(!ipAddress.Contains(":"))
            //    uri = @"http://" + ipAddress + @":" + portNumber + @"/RMS" + @"/paymenttype" + @"/";
            //else
            //    uri = @"http://" + ipAddress + @"/paymenttype" + @"/";
            string uri = ipAddress + @"/paymenttype" + @"/";
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
                    request.ServicePoint.Expect100Continue = false;
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
                    request.Timeout = 20000;
                    //request.ContentType = "text/plain";

                    // Get the request stream.  
                    Stream dataStream = request.GetRequestStream();
                    // Write the data to the request stream.  
                    dataStream.Write(bytes, 0, bytes.Length);
                    // Close the Stream object.  
                    dataStream.Close();

                    HttpWebResponse wr = (HttpWebResponse)request.GetResponse();
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
            List<string> stringList = GetDetailsList(roomChargeDetails);
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
                request.Timeout = 20000;
                request.ContentType = "text/plain";
                request.GetRequestStream().Write(bytes, 0, bytes.Length);
                WebResponse webResponse = request.GetResponse();
                var memberStream = new StreamReader(webResponse.GetResponseStream());
                response = deserializer.DesrializeRoomPostResponse(memberStream.ReadToEnd());
                //-------------------------------------------------------------------------------------//
                stringList.Add("Response Date                        " + DateTime.Now.ToString("ddMMMyyyy"));
                stringList.Add("Response Time                        " + DateTime.Now.ToString("hhmmss"));
                string responseText = "Unsuccessful";
                if (response.IsSuccessful)
                    responseText = "Successful";
                stringList.Add("Response                             " + responseText);
                WriteToFile(stringList);
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
                request.Timeout = 20000;
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
        private List<string> GetDetailsList(RoomChargeDetails roomChargeDetails)
        {
            List<string> stringList = new List<string>();
            try
            {
                stringList.Add("=============================================================================");
                string invoiceNnumber = "0";
                for (int i = 0; i < roomChargeDetails.ItemList.Count; i++)
                {
                    invoiceNnumber = roomChargeDetails.ItemList[0].Billno;
                    break;
                }
                stringList.Add("Invoice Number                      " + invoiceNnumber);
                double value = 0.0;
                for (int i = 0; i < roomChargeDetails.ItemList.Count; i++)
                {
                    value += Double.Parse(roomChargeDetails.ItemList[i].PriceTotal);
                }
                stringList.Add("Invoice Amount                      " + value.ToString());
                string paymentNames = "";
                for (int i = 0; i < roomChargeDetails.PaymentList.Count; i++)
                {
                    paymentNames += roomChargeDetails.PaymentList[i].Description + " " + roomChargeDetails.PaymentList[i].Amount;
                }
                stringList.Add("Payments                         " + paymentNames);
                stringList.Add("Request Date                        " + DateTime.Now.ToString("ddMMMyyyy"));
                stringList.Add("Request Time                        " + DateTime.Now.ToString("hhmmss"));
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in preparing data for LOG" + ex.Message);
            }
            return stringList;
        }

        private void WriteToFile(List<string> list)
        {
            try
            {
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Sihot Post Logs");
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
