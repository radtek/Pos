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
        public SaleToPOIResponse PingTerminal(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                var webRequest = CreateWebRequest(details);
                response = PostRequest(envelop, webRequest, RequestType.ePingTerminal);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal " + ex.Message);
            }
            return response; 
        }

        public SaleToPOIResponse LoginToSystem(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                var webRequest = CreateWebRequest(details);
                envelop.SaleToPOIRequest.LoginRequest.DateTime = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eLoginToSystem);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal " + ex.Message);
            }
            return response;
        }


        public SaleToPOIResponse LogoutSystem(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                var webRequest = CreateWebRequest(details);
                response = PostRequest(envelop, webRequest, RequestType.eLogoutSystem);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Ping Terminal " + ex.Message);
            }
            return response;
        }

        public SaleToPOIResponse Purchase(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                var webRequest = CreateWebRequest(details);
                envelop.SaleToPOIRequest.PaymentRequest.SaleData.SaleTransactionID.TimeStamp = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eProcessSale);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Purchase " + ex.Message);
            }
            return response; 
        }

        public SaleToPOIResponse Refund(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                var webRequest = CreateWebRequest(details);
                response = PostRequest(envelop, webRequest, RequestType.eProcessRefund);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in Refund " + ex.Message);
            }
            return response; 
        }
        public SaleToPOIResponse GetTransactionStatus(Envelop envelop, ResourceDetails details)
        {
            SaleToPOIResponse response = null;
            try
            {
                var webRequest = CreateWebRequest(details);
                envelop.SaleToPOIRequest.PaymentRequest.SaleData.SaleTransactionID.TimeStamp = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
                response = PostRequest(envelop, webRequest, RequestType.eTransactionStatus);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in GetTransactionStatus " + ex.Message);
            }
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
            return httpWebRequest;
        }

        private SaleToPOIResponse PostRequest(Envelop envelop, HttpWebRequest httpWebRequest, RequestType requestType)
        {
            try
            {
                ResponseEnvelop responseEnvelop = new ResponseEnvelop();
                var requestData = JSonUtility.Serialize<Envelop>(envelop);
                using (var streamWriter = new StreamWriter(httpWebRequest.GetRequestStream()))
                {
                    streamWriter.Write(requestData);
                    streamWriter.Flush();
                    streamWriter.Close();
                }
                var encoding = Encoding.ASCII;
                var webResponse = (HttpWebResponse)httpWebRequest.GetResponse();

                using (var reader = new StreamReader(webResponse.GetResponseStream(), encoding))
                {
                    var responseText = reader.ReadToEnd();
                    responseEnvelop = JSonUtility.Deserialize<ResponseEnvelop>(responseText);
                    if ( CanReceiptsBePresent(requestType,responseEnvelop)/*requestType == RequestType.eProcessSale) && responseEnvelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.Count() > 0*/)
                    {
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
                return responseEnvelop.SaleToPOIResponse;
            }
            catch (Exception ex)
            {
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
                return receipts;
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in CreateFormattedReceipts  " + ex.Message);
                throw;
            }
        }

        private bool CanReceiptsBePresent(RequestType requestType, ResponseEnvelop envelop)
        {
            return ((requestType == RequestType.eProcessSale) && (envelop.SaleToPOIResponse.PaymentResponse.PaymentReceipt.Count() > 0));
        }
    }
}
