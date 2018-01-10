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
                HttpWebResponse webResponse = null;
                Stream dataStream = null;
                StreamReader memberStream = null;
                List<string> stringList = new List<string>();
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
                    request.Timeout = 50000;
                    //request.ContentType = "text/plain";
                    stringList.Add("=============================================================================");
                    stringList.Add("Inquiry Request at:       " + DateTime.Now.ToString("ddMMMyyyy"));
                    stringList.Add("Inquiry Request Time:     " + DateTime.Now.ToString("hhmmss"));
                    // Get the request stream.  
                    dataStream = request.GetRequestStream();
                    // Write the data to the request stream.  
                    dataStream.Write(bytes, 0, bytes.Length);
                    // Close the Stream object.  

                    webResponse = (HttpWebResponse)request.GetResponse();
                    stringList.Add("Inquiry Response at Date:  " + DateTime.Now.ToString("ddMMMyyyy"));
                    stringList.Add("Inquiry Response at Time:  " + DateTime.Now.ToString("hhmmss"));
                    memberStream = new StreamReader(webResponse.GetResponseStream());
                    roomDetails = deserializer.DeserializeRoomResponse(memberStream.ReadToEnd());
                    stringList.Add("Room Number:  " + roomRequest.RoomNumber);
                    for (int guestList = 0; guestList < roomDetails.GuestDetailsList.Count; guestList++)
                    {
                        stringList.Add("Account Number:  " + roomDetails.GuestDetailsList[guestList].AccountNo);
                        stringList.Add("Name:            " + roomDetails.GuestDetailsList[guestList].FirstName + " "
                                                           + roomDetails.GuestDetailsList[guestList].LastName);
                    }
                    if(roomDetails.GuestDetailsList.Count == 0)
                        stringList.Add("Guest List:  " + "0");    
                }
                catch (Exception ex)
                {
                    ServiceLogger.Log("Exception in sending Room request" + ex.Message);
                    stringList.Add("exception Message:         " + ex.Message);
                }
                finally
                {
                    if (webResponse != null)
                        stringList.Add("webresponse Status Description: " + webResponse.StatusDescription);
                    if (memberStream != null)
                        memberStream.Close();
                    if (dataStream != null)
                        dataStream.Close();
                    if (webResponse != null)
                        webResponse.Close();
                    WriteToFile(stringList);
                }
            return roomDetails;
        }

        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails, int retryCount)
        {
            List<string> stringList = GetDetailsList(roomChargeDetails);
            RoomChargeResponse response = new RoomChargeResponse();
            SiHotSerializer serializer = new SiHotSerializer();
            SiHotDesrializer deserializer = new SiHotDesrializer();
            string responseText = "Unsuccessful";
            string exceptionMessage = "";
            HttpWebResponse responseNew = null;
            Stream dataStream = null;
            StreamReader reader = null;
            try
            {
                string uri = URIRoomChargePost(roomChargeDetails.IPAddress, roomChargeDetails.PortNumber);
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(new Uri(uri));
                request.ServicePoint.Expect100Continue = false;
                // Set the Method property of the request to POST.  
                request.Method = "POST";
                // Create POST data and convert it to a byte array.  
                List<byte> bytesList = serializer.GetRoomChargeContent(roomChargeDetails);
                byte[] bytes = bytesList.ToArray<byte>();
                request.ContentLength = bytes.Length;
                //request.Timeout = 5000;
                request.Timeout = 5000;
                request.ContentType = "text/plain";
                // Get the request stream.  
                dataStream = request.GetRequestStream();
                // Write the data to the request stream.  
                dataStream.Write(bytes, 0, bytes.Length);
                // Close the Stream object.  
                dataStream.Close();
                // Get the response.  
                responseNew = (HttpWebResponse)request.GetResponse();
                // Display the status.  
                var status = ((HttpWebResponse)responseNew).StatusDescription;
                // Get the stream containing content returned by the server.  
                dataStream = responseNew.GetResponseStream();
                // Open the stream using a StreamReader for easy access.  
                reader = new StreamReader(dataStream);

                response = deserializer.DesrializeRoomPostResponse(reader.ReadToEnd());
                if (response.IsSuccessful)
                    responseText = "Successful";
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in sending Room Post " + ex.Message);
                exceptionMessage = ex.Message;
            }
            finally
            {
                if (reader != null)
                    reader.Close();
                if (dataStream != null)
                    dataStream.Close();
                if(responseNew != null)
                    responseNew.Close();
                //-------------------------------------------------------------------------------------//
                stringList.Add("Post Response Date:                       " + DateTime.Now.ToString("ddMMMyyyy"));
                stringList.Add("Post Response Time:                       " + DateTime.Now.ToString("hhmmss"));
                stringList.Add("Post Response:                            " + responseText);
                stringList.Add("No of Times tried:                        " + retryCount);
                if (exceptionMessage.Length != 0)
                    stringList.Add("Post Exception message:                   " + exceptionMessage);
                if (!response.IsSuccessful)
                    stringList.Add("Unsuccessful reason:                      " + response.Response);
                WriteToFile(stringList);
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
                request.Timeout = 50000;
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
                stringList.Add("Invoice Number:                     " + invoiceNnumber);
                double value = 0.0;
                for (int i = 0; i < roomChargeDetails.ItemList.Count; i++)
                {
                    value += Double.Parse(roomChargeDetails.ItemList[i].PriceTotal);
                }
                stringList.Add("Invoice Amount:                     " + value.ToString());
                string paymentNames = "";
                for (int i = 0; i < roomChargeDetails.PaymentList.Count; i++)
                {
                    paymentNames += roomChargeDetails.PaymentList[i].Description + " " + roomChargeDetails.PaymentList[i].Amount;
                }
                stringList.Add("Payments:                        " + paymentNames);
                stringList.Add("Post Request Date:                       " + DateTime.Now.ToString("ddMMMyyyy"));
                stringList.Add("Post Request Time:                       " + DateTime.Now.ToString("hhmmss"));
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
