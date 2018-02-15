using System;
using System.Linq;
using System.Collections.Generic;
using System.Net;
using SiHotIntegration.Domain;
using SiHotIntegration.Utility;
using System.Text;
using System.IO;
using SiHotIntegration.Tools;
using System.Net.Sockets;
using System.Net.Security;
using System.Security.Authentication;
using System.Security.Cryptography.X509Certificates;

namespace SiHotIntegration
{
    public class SiHotCommunicationController
    {
        public readonly string connectFailedMessage = "Unable to connect to SiHot";
        public readonly string siHotUnavailable = "SiHot is not available at the moment.";
        public SiHotCommunicationController()
        {
            System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls | SecurityProtocolType.Ssl3;
        }

        public string URIRoomRequest(string ipAddress, int portNumber)
        {
            return ipAddress + @"/roomrequest" + @"/"; 
        }
        public string URIRoomChargePost(string ipAddress, int portNumber)
        {
            return ipAddress + @"/accountbook" + @"/";
        }
        public string URIValidate(string ipAddress, int portNumber)
        {
            return ipAddress + @"/paymenttype" + @"/";
        }
        public RoomDetails GetRoomDetails(RoomRequest roomRequest)
        {
            RoomDetails roomDetails = new RoomDetails();
            List<string> stringList = new List<string>();
            SiHotSerializer serializer = new SiHotSerializer();
            SiHotDesrializer deserializer = new SiHotDesrializer();
            string strCompleteResponse = "";
            string strSiHotResponse = "";
            bool IsSecured = false;
            try
            {
                string uri = URIRoomRequest(roomRequest.IPAddress, roomRequest.PortNumber);
                Uri myUri = new Uri(uri);
                var host = Dns.GetHostAddresses(myUri.Host)[0];
                IsSecured = uri.Contains("https:");
                using (var tc = new TcpClient())
                {
                    tc.ReceiveTimeout = 5000;
                    tc.Connect(host, IsSecured ? 443 : 80);
                    if (tc.Connected)
                    {
                        stringList.Add("Connection Created On:-                   " + DateTime.Now.ToString("ddMMMyyyy"));
                        stringList.Add("Connection Created At Time:-              " + DateTime.Now.ToString("hh:mm:ss tt"));
                        using (var ns = tc.GetStream())
                        {
                            ns.ReadTimeout = 3000;
                            List<string> detailsList = serializer.GetRoomRequestContent(roomRequest);
                            var bytes = GetRoomByteArray(detailsList);
                            var strHttpRequest = GetHttpRequest(myUri, bytes.Length);
                            strHttpRequest += System.Text.Encoding.UTF8.GetString(bytes).Trim();
                            PrepareRoomRquestLogs(stringList, detailsList);
                            if (IsSecured)
                            {
                                using (var ssl = new SslStream(ns, false, ValidateServerCertificate, null))
                                {
                                    ssl.AuthenticateAsClient(host.ToString(), null, SslProtocols.Tls, false);
                                    using (var sw = new System.IO.StreamWriter(ssl))
                                    {
                                        using (var sr = new System.IO.StreamReader(ssl))
                                        {
                                            stringList.Add("Data writing at Time:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                                            sw.Write(strHttpRequest);
                                            sw.Flush();
                                            strCompleteResponse = sr.ReadToEnd();
                                            if (strCompleteResponse.Contains("transno:"))
                                                strSiHotResponse = strCompleteResponse.Substring(strCompleteResponse.IndexOf("transno:"),
                                                                   strCompleteResponse.Length - strCompleteResponse.IndexOf("transno:"));
                                        }
                                    }
                                }
                            }
                            else
                            {
                                using (var sw = new System.IO.StreamWriter(ns))
                                {
                                    using (var sr = new System.IO.StreamReader(ns))
                                    {
                                        stringList.Add("Data writing at Time:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                                        sw.Write(strHttpRequest);
                                        sw.Flush();
                                        strCompleteResponse = sr.ReadToEnd();
                                        if (strCompleteResponse.Contains("transno:"))
                                            strSiHotResponse = strCompleteResponse.Substring(strCompleteResponse.IndexOf("transno:"),
                                                               strCompleteResponse.Length - strCompleteResponse.IndexOf("transno:"));
                                    }
                                }
                            }
                            roomDetails = deserializer.DeserializeRoomResponse(strSiHotResponse);
                            PrepareRoomResponseLogs(strSiHotResponse, roomDetails, stringList);
                        }
                    }
                    else
                    {
                        stringList.Add(connectFailedMessage);
                        stringList.Add("Connection Failure at Time:-                " + DateTime.Now.ToString("hh:mm:ss tt"));
                        roomDetails.ResponseMessage = connectFailedMessage;
                        roomDetails.IsSuccessful = false;
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in sending Room request" + ex.Message);
                stringList.Add("exception Message:                       " + ex.Message);
                stringList.Add("exception Date:-                         " + DateTime.Now.ToString("ddMMMyyyy"));
                stringList.Add("exception Time:-                         " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                WriteToFile(stringList);
            }
            return roomDetails;
        }
        public RoomChargeResponse PostRoomCharge(RoomChargeDetails roomChargeDetails, int retryCount)
        {
            List<string> stringList = new List<string>();
            RoomChargeResponse response = new RoomChargeResponse();
            SiHotSerializer serializer = new SiHotSerializer();
            SiHotDesrializer deserializer = new SiHotDesrializer();
            string responseText = "Unsuccessful";
            string exceptionMessage = "";
            string strCompleteResponse = "";
            string strSiHotResponse = "";
            string exceptionTime = "";
            bool IsSecured = false;
            try
            {
                string uri = URIRoomChargePost(roomChargeDetails.IPAddress, roomChargeDetails.PortNumber);
                Uri myUri = new Uri(uri);
                var host = Dns.GetHostAddresses(myUri.Host)[0];
                IsSecured = uri.Contains("https:");
                using (var tc = new TcpClient())
                {
                    tc.ReceiveTimeout = 5000;
                    tc.Connect(host, IsSecured ? 443 : 80);
                    if (tc.Connected)
                    {
                        stringList.Add("Connection Created On:-                   " + DateTime.Now.ToString("ddMMMyyyy"));
                        stringList.Add("Connection Created At Time:-              " + DateTime.Now.ToString("hh:mm:ss tt"));
                        GetDetailsList(roomChargeDetails, stringList);
                        using (var ns = tc.GetStream())
                        {
                            ns.ReadTimeout = 3000;
                            List<byte> bytesList = serializer.GetRoomChargeContent(roomChargeDetails);
                            byte[] bytes = bytesList.ToArray<byte>();
                            var strHttpRequest = GetHttpRequest(myUri, bytes.Length);
                            strHttpRequest += System.Text.Encoding.UTF8.GetString(bytes).Trim();
                            if (IsSecured)
                            {
                                using (var ssl = new SslStream(ns, false, ValidateServerCertificate, null))
                                {
                                    ssl.AuthenticateAsClient(host.ToString(), null, SslProtocols.Tls, false);
                                    using (var sw = new System.IO.StreamWriter(ssl))
                                    {
                                        using (var sr = new System.IO.StreamReader(ssl))
                                        {
                                            stringList.Add("Data writing at Time:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                                            sw.Write(strHttpRequest);
                                            sw.Flush();
                                            strCompleteResponse = sr.ReadToEnd();
                                            if (strCompleteResponse.Contains("transno:"))
                                                strSiHotResponse = strCompleteResponse.Substring(strCompleteResponse.IndexOf("transno:"),
                                                                   strCompleteResponse.Length - strCompleteResponse.IndexOf("transno:"));
                                        }
                                    }
                                }
                            }
                            else
                            {
                                using (var sw = new System.IO.StreamWriter(ns))
                                {
                                    using (var sr = new System.IO.StreamReader(ns))
                                    {
                                        stringList.Add("Data writing at Time:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                                        sw.Write(strHttpRequest);
                                        sw.Flush();
                                        strCompleteResponse = sr.ReadToEnd();
                                        if (strCompleteResponse.Contains("transno:"))
                                            strSiHotResponse = strCompleteResponse.Substring(strCompleteResponse.IndexOf("transno:"),
                                                               strCompleteResponse.Length - strCompleteResponse.IndexOf("transno:"));
                                    }
                                }
                            }
                            response = deserializer.DesrializeRoomPostResponse(strSiHotResponse);
                            stringList.Add("SiHot Response Received :-                " + strSiHotResponse);
                            if (response.IsSuccessful)
                                responseText = "Successful";
                            if (response.Response.Trim() == "")
                                response.Response = siHotUnavailable;
                        }
                    }
                    else
                    {
                        stringList.Add(connectFailedMessage);
                        stringList.Add("Connection Failure at Time:-              " + DateTime.Now.ToString("hh:mm:ss tt"));
                        response.Response = connectFailedMessage;
                        response.IsSuccessful = false;
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in sending Room Post " + ex.Message);
                exceptionMessage = ex.Message;
                exceptionTime = DateTime.Now.ToString("hh:mm:ss tt");
                response.IsSuccessful = false;
            }
            finally
            {
                stringList.Add("Post Response Date:                       " + DateTime.Now.ToString("ddMMMyyyy"));
                stringList.Add("Post Response Time:                       " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Post Status:                              " + responseText);
                stringList.Add("No of Times tried:                        " + retryCount);
                if (exceptionMessage.Length != 0)
                    stringList.Add("Post Exception message:                   " + exceptionMessage);
                if (exceptionTime.Length != 0)
                    stringList.Add("Post Exception Time:                      " + exceptionTime);
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
                request.Timeout = 5000;
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
        private void GetDetailsList(RoomChargeDetails roomChargeDetails, List<string> stringList)
        {
            try
            {
                string invoiceNnumber = "0";
                stringList.Add("Transaction Number:                       " + roomChargeDetails.TransNo);
                for (int i = 0; i < roomChargeDetails.ItemList.Count; i++)
                {
                    invoiceNnumber = roomChargeDetails.ItemList[0].Billno;
                    break;
                }
                stringList.Add("Invoice Number:                           " + invoiceNnumber);
                double value = 0.0;
                for (int i = 0; i < roomChargeDetails.ItemList.Count; i++)
                {
                    //double thisValue = Math.Round(Double.Parse(roomChargeDetails.ItemList[i].Amount),2);
                    //thisValue = thisValue * Math.Round(Double.Parse(roomChargeDetails.ItemList[i].PricePerUnit),2);
                    //value += thisValue;
                    double thisValue = Math.Round(Double.Parse(roomChargeDetails.ItemList[i].PriceTotal), 2);
                    if (roomChargeDetails.ItemList[i].Amount.Contains("-"))
                        thisValue = thisValue * -1;
                    value += thisValue;
                }
                stringList.Add("Invoice Amount:                           " + value.ToString());
                string paymentNames = "";
                stringList.Add("*********Start Of Items**************");
                for (int itemIndex = 0; itemIndex < roomChargeDetails.ItemList.Count; itemIndex++)
                {
                    string ppu = roomChargeDetails.ItemList[itemIndex].PricePerUnit;
                    string total = roomChargeDetails.ItemList[itemIndex].PriceTotal;
                    if (roomChargeDetails.ItemList[itemIndex].PricePerUnit.Contains(".") &&
                        (roomChargeDetails.ItemList[itemIndex].PricePerUnit.IndexOf(".") + 3 < roomChargeDetails.ItemList[itemIndex].PricePerUnit.Length))
                    {
                        ppu =
                            roomChargeDetails.ItemList[itemIndex].PricePerUnit.Substring(0, (roomChargeDetails.ItemList[itemIndex].PricePerUnit.IndexOf(".") + 3));
                    }
                    if (roomChargeDetails.ItemList[itemIndex].PriceTotal.Contains(".") &&
                        (roomChargeDetails.ItemList[itemIndex].PriceTotal.IndexOf(".") + 3 < roomChargeDetails.ItemList[itemIndex].PriceTotal.Length))
                    {
                        total =
                             roomChargeDetails.ItemList[itemIndex].PriceTotal.Substring(0, (roomChargeDetails.ItemList[itemIndex].PriceTotal.IndexOf(".") + 3));
                    }
                    stringList.Add("Middle Category:- " + roomChargeDetails.ItemList[itemIndex].MiddleCategory + 
                                   "    Description:- " + roomChargeDetails.ItemList[itemIndex].MiddleCategory_Desc + 
                                   "    Quantity:-    " + roomChargeDetails.ItemList[itemIndex].Amount + 
                                   "    Price Unit:-  " + ppu + 
                                   "    Price Total:- " + total + 
                                   "    VAT Percent:- " + roomChargeDetails.ItemList[itemIndex].VATPercentage);
                   
                }
                stringList.Add("***********End Of Items**************");
                for (int i = 0; i < roomChargeDetails.PaymentList.Count; i++)
                {
                    if(i > 0)
                        paymentNames += "                                          ";
                    paymentNames += roomChargeDetails.PaymentList[i].Description + " " + roomChargeDetails.PaymentList[i].Amount;
                    paymentNames += "\r\n";
                }
                stringList.Add("Payments:                                 " + paymentNames);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in preparing data for LOG" + ex.Message);
            }
        }

        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
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
        private string GetHttpRequest(Uri inUri, int dataLength)
        {
            return @"POST " + inUri.LocalPath + " HTTP/1.1" + Environment.NewLine +
                                                "Time-out: 5000" + Environment.NewLine +
                                                "Content-Type: text/plain" + Environment.NewLine +
                                                "Host: " + inUri.Host + Environment.NewLine +
                                                "Content-Length: " + dataLength + Environment.NewLine
                                                + Environment.NewLine;
        }
        private static bool ValidateServerCertificate(object sender,X509Certificate certificate,X509Chain chain,
                                                      SslPolicyErrors sslPolicyErrors)
        {
            return true; // Accept all certs
        }
        private byte[] GetRoomByteArray(List<string> detailsList)
        {
            byte seperator = 29;
            var byteList = new List<byte>();
            for (int i = 0; i < detailsList.Count; i++)
            {
                var str = detailsList[i];
                byteList.AddRange(Encoding.UTF8.GetBytes(str).ToList<byte>());
                byteList.Add(seperator);
            }
            var bytes = byteList.ToArray<byte>();
            return bytes;
        }
        private void PrepareRoomRquestLogs(List<string> stringListLogs, List<string> detailsList)
        { 
            stringListLogs.Add("******Request Data Start*******");
            for (int detailsIndex = 0; detailsIndex < detailsList.Count; detailsIndex++)
            {
                stringListLogs.Add(detailsList[detailsIndex]);
            }
            stringListLogs.Add("******Request Data End*******");
        }
        private void PrepareRoomResponseLogs(string strSiHotResponse, RoomDetails roomDetails, List<string> stringListLogs)
        {
            stringListLogs.Add("Inquiry Response Date:                    " + DateTime.Now.ToString("ddMMMyyyy"));
            stringListLogs.Add("Inquiry Response Time:                    " + DateTime.Now.ToString("hh:mm:ss tt"));
            stringListLogs.Add("SiHot Response Received:-                 " + strSiHotResponse);
            if (roomDetails.GuestDetailsList.Count == 0)
                stringListLogs.Add("Guest List:                               " + "0"); 
        }
    }
}
