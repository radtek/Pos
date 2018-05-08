using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AdyenIntegration.Utility;
using AdyenIntegration.Domain;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain.Constants;
using AdyenIntegration.Domain.Diagnosis;
using AdyenIntegration.Domain.RequestEnvelop;
using AdyenIntegration.Domain.ResponseEnvelop;
using System.Net;
using System.IO;
using AdyenIntegration.Tools;


namespace AdyenIntegration
{
    public enum RequestType { ePingTerminal, eTransactionStatus, eProcessSale, eProcessRefund, eLoginToSystem, eLogoutSystem };

    public class AdyenIntegrationController
    {
        List<string> stringList = new List<string>();
        public SaleToPOIResponse PingTerminal(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                stringList.Add("Request to Get Terminal Status");
                var webRequest = CreateWebRequest(details);
                response = PostRequest(envelop, webRequest, RequestType.ePingTerminal);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal " + ex.Message);
                stringList.Add("Exception in PingTerminal at:-                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            WriteToFile(stringList);
            return response; 
        }

        public SaleToPOIResponse LoginToSystem(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                stringList.Add("Request to Login");
                var webRequest = CreateWebRequest(details);
                envelop.SaleToPOIRequest.LoginRequest.DateTime = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eLoginToSystem);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal " + ex.Message);
                stringList.Add("Exception in Login at:-                                         " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            WriteToFile(stringList);
            return response;
        }


        public SaleToPOIResponse LogoutSystem(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                stringList.Add("Request to Logout");
                var webRequest = CreateWebRequest(details);
                response = PostRequest(envelop, webRequest, RequestType.eLogoutSystem);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal " + ex.Message);
                stringList.Add("Exception in Logout at:-                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            WriteToFile(stringList);
            return response;
        }

        public SaleToPOIResponse Purchase(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                stringList.Add("Request to Purchase");
                var webRequest = CreateWebRequest(details);
                envelop.SaleToPOIRequest.PaymentRequest.SaleData.SaleTransactionID.TimeStamp = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eProcessSale);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Purchase " + ex.Message);
                stringList.Add("Exception in Purchase at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            WriteToFile(stringList);
            return response; 
        }

        public SaleToPOIResponse Refund(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                stringList.Add("Request to Refund");
                var webRequest = CreateWebRequest(details);
                envelop.SaleToPOIRequest.PaymentRequest.SaleData.SaleTransactionID.TimeStamp = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eProcessRefund);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Refund " + ex.Message);
                stringList.Add("Exception in Refund at:-                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            WriteToFile(stringList);
            return response; 
        }
        public SaleToPOIResponse GetTransactionStatus(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                stringList.Add("Request to GetTransaction Status");
                var webRequest = CreateWebRequest(details);
                //envelop.SaleToPOIRequest.PaymentRequest.SaleData.SaleTransactionID.TimeStamp = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eTransactionStatus);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetTransactionStatus " + ex.Message);
                stringList.Add("Exception at GetTransactionStatus at:-                          " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            WriteToFile(stringList);
            return response; 
        }

        private HttpWebRequest CreateWebRequest(ResourceDetails details)
        {
            var url = details.URL.Trim();
            var httpWebRequest = (HttpWebRequest)WebRequest.Create(url);
            httpWebRequest.Method = "POST";
            httpWebRequest.ContentType = "application/json";
            httpWebRequest.Headers.Add("x-api-key", details.APIKey);
            httpWebRequest.Headers.Add("Accept-Charset", "UTF-8");
            httpWebRequest.Headers.Add("Cache-Control", "no-cache");
            httpWebRequest.Timeout = 100000;
            stringList.Add("Web Request Created at :-                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            return httpWebRequest;
        }

        private SaleToPOIResponse PostRequest(Envelop envelop, HttpWebRequest httpWebRequest, RequestType requestType)
        {
            try
            {
                ResponseEnvelop responseEnvelop = new ResponseEnvelop();
                var requestData = JSonUtility.Serialize<Envelop>(envelop);
                stringList.Add("JSon Data prepared at:-                                         " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("JSon Data is :-                                        " + requestData);
                using (var streamWriter = new StreamWriter(httpWebRequest.GetRequestStream()))
                {
                    streamWriter.Write(requestData);
                    streamWriter.Flush();
                    streamWriter.Close();
                }
                stringList.Add("Data written to stream at :-                                     " + DateTime.Now.ToString("hh:mm:ss tt"));
                var encoding = Encoding.ASCII;
                stringList.Add("Asking for response at :-                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                var webResponse = (HttpWebResponse)httpWebRequest.GetResponse();
                stringList.Add("Data received at :-                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                using (var reader = new StreamReader(webResponse.GetResponseStream(), encoding))
                {
                    var responseText = reader.ReadToEnd();
                    stringList.Add("Data received is :-                                              " + responseText);
                    responseEnvelop = JSonUtility.Deserialize<ResponseEnvelop>(responseText);
                    stringList.Add("Data deserialized at :-                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                    if (requestType != RequestType.eTransactionStatus)
                    {
                        if (CanReceiptsBePresent(requestType, responseEnvelop))
                        {
                            stringList.Add("Receipts Found & count is                                       " + responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.Count());
                            if (responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.Count() >= 1)
                            {
                                responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceiptUsable1 =
                                CreateFormattedReceipts(responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.ElementAt(0));
                            }
                            if (responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.Count() >= 2)
                            {
                                responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceiptUsable2 =
                                    CreateFormattedReceipts(responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.ElementAt(1));
                            }
                        }
                    }
                    else if (requestType == RequestType.eTransactionStatus)
                    {
                        if (CanReceiptsBePresentInEnquiry(requestType, responseEnvelop))
                        {
                            stringList.Add("Receipts Found & count is                                       " + responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceipt.Count());
                            if (responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceipt.Count() >= 1)
                            {
                                responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceiptUsable1 =
                                CreateFormattedReceipts(responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceipt.ElementAt(0));
                            }
                            if (responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceipt.Count() >= 2)
                            {
                                responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceiptUsable2 =
                                    CreateFormattedReceipts(responseEnvelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceipt.ElementAt(1));
                            }
                        }
                    }
                }
                return responseEnvelop.SaleToPOIResponse;
            }
            catch (Exception ex)
            {
                stringList.Add("Exception Occured  at  :-                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception is :-                                                  " + ex.Message);
                ServiceLogger.Log("Exception in PostRequest   " +  ex.Message);
                throw;
            }
        }

        private List<string> CreateFormattedReceipts(PaymentReceipt receiptArray)
        {
            try
            {
                List<string> receipts = new List<string>();
                FormatReciepts format = new FormatReciepts();
                //int index = 0;
                //foreach (var item in receiptArray.OutputContent.OutputText)
                //{
                //    receipts[index] = format.FormatReceipt(receiptArray[index].OutputContent.OutputText.ToList());
                //}
                receipts = format.FormatReceipt(receiptArray.OutputContent.OutputText.ToList());
                stringList.Add("Receipt Extracted :-                                                  ");
                return receipts;
            }
            catch (Exception ex)
            {
                stringList.Add("Exception Occured in receipt extraction at  :-                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception is :-                                                  " + ex.Message);
                ServiceLogger.Log("Exception in CreateFormattedReceipts  " + ex.Message);
                throw;
            }
        }

        private bool CanReceiptsBePresent(RequestType requestType, ResponseEnvelop envelop)
        {
            bool retValue = false;
            if ((requestType == RequestType.eProcessSale) || (requestType == RequestType.eProcessRefund))
            {
                if (envelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt != null)
                {
                    retValue = (envelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.Count() > 0);
                }
            }
            return retValue;
        }
        private bool CanReceiptsBePresentInEnquiry(RequestType requestType, ResponseEnvelop envelop)
        {
            if (envelop.SaleToPOIResponse.TransactionStatusResponse.RepeatedMessageResponse.RepeatedResponseMessageBody.PaymentResponse.PaymentReceipt != null)
                return true;
            else
                return false;
        }
        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Adyen Post Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "AdyenPosts " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
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
            stringList.Clear();
        }
    }
}