﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SmartConnectIntegration.Domain;
using System.Diagnostics;
using System.ServiceModel;
using SmartConnectIntegration.Enums;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using SmartConnectIntegration;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;

namespace MenumateServices.WCFServices
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
    public class WCFServiceSmartConnect : IWCFSmartConnect
    {
        private SmartConnectResponse _response;
        private bool _waitflag;
        private readonly HttpClientHandler _httpClientHandler;
        private string SampleText = "****** SAMPLE ******\n" +
            "Terminal print\n" +
            "function test\n" +
            "The following two\n" +
            "lines should be\n" +
            "blank.\n" +
            "                    \n" +
            "\n" +
            "********************\n" +
            "************************\n" +
            "\n\n\n\n\n\n\n\n\n";

        public WCFServiceSmartConnect()
        {
            _waitflag = false;
            _response = new SmartConnectResponse();
        }

        public SmartConnectResponse PingTerminal(string ipAddress)
        {
            var response = new SmartConnectResponse();
            try
            {
                using (var ping = new System.Net.NetworkInformation.Ping())
                {
                    System.Net.NetworkInformation.PingReply pingReply = ping.Send(ipAddress);
                    response.ResponseSuccessful = (pingReply.Status == System.Net.NetworkInformation.IPStatus.Success);
                    if (!response.ResponseSuccessful)
                    {
                        response.ResponseMessage = "Login Failed - " + Convert.ToString(pingReply.Status);
                    }
                }
            }
            catch (Exception ex)
            {
                response.ResponseSuccessful = false;
                response.ResponseMessage = ex.Message;
                EventLog.WriteEntry("In PingTerminal SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 18, short.MaxValue);
                ServiceLogger.LogException("Exception in PingTerminal", ex);
            }
            return response;
        }

        public SmartConnectResponse Pairing(PairingTerminal param)
        {
            var smartConnectResponse = new SmartConnectResponse();    
            string requesturl = SmartConnectConstraints.PairingBaseAddress + "/" + param.PairingCode;

            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("POSRegisterID", param.PosRegisterId);
            parameters.Add("POSRegisterName", param.PosRegisterName);
            parameters.Add("POSBusinessName", param.PosBusinessName);
            parameters.Add("POSVendorName", param.PosVendorName);
            string putData = GetPutOrPostData(parameters);
            string response = PutOrPostResponse(requesturl, putData, false);

            JObject jo = new JObject();
            jo = (JObject)JsonConvert.DeserializeObject(response);

            if (jo["errno"] != null)
            {
                if (int.Parse(jo["errno"].ToString()) == 0)
                {
                    var info = (JObject)JsonConvert.DeserializeObject(jo["results"].ToString());
                    if (info != null)
                    {
                        smartConnectResponse.ResponseSuccessful = true;
                    }
                }
                else
                {
                    smartConnectResponse.ResponseSuccessful = false;
                    smartConnectResponse.ResponseMessage = "Errcode: " + jo["errno"].ToString() + " " + jo["message"].ToString();
                }

            }
            return smartConnectResponse; ;
        }

        public SmartConnectResponse Logon(TransactionTypes logonType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {             
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(logonType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);              

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Logon SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 1, short.MaxValue);
                ServiceLogger.LogException("Exception in Logon", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse SettlementInquiry(TransactionTypes settlementEnquiryType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(settlementEnquiryType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SettlementInquiry SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 2, short.MaxValue);
                ServiceLogger.LogException("Exception in SettlementInquiry", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse SettlementCutover(TransactionTypes settlementCutoverType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(settlementCutoverType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SettlementCutover SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 3, short.MaxValue);
                ServiceLogger.LogException("Exception in SettlementCutover", ex);
            }
            return _response;
        }

        public SmartConnectResponse Purchase(TransactionTypes purchaseType, double amount) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {                
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(purchaseType);
                parameters.Add("AmountTotal", System.Convert.ToString(amount)); 
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Purchase SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in Purchase", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse PurchasePlusCash(TransactionTypes purchasePlusCashType, double totalAmount, double cashAmount) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(purchasePlusCashType);
                parameters.Add("AmountTotal", System.Convert.ToString(totalAmount));
                parameters.Add("AmountCash", System.Convert.ToString(cashAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PurchasePlusCash SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 5, short.MaxValue);
                ServiceLogger.LogException("Exception in PurchasePlusCash", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse CashOutOnly(TransactionTypes cashOutOnlyType, double cashAmount)
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(cashOutOnlyType);
                parameters.Add("AmountTotal", System.Convert.ToString(cashAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In CashOutOnly SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 6, short.MaxValue);
                ServiceLogger.LogException("Exception in CashOutOnly", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Refund(TransactionTypes refundType, double refAmount) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(refundType);
                parameters.Add("AmountTotal", System.Convert.ToString(refAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Refund SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 7, short.MaxValue);
                ServiceLogger.LogException("Exception in Refund", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Authorise(TransactionTypes authoriseType, double amountAuth, string transactionRef) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(authoriseType);
                parameters.Add("AmountAuth", System.Convert.ToString(amountAuth));
                parameters.Add("TransactionReference", transactionRef);
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Authorise SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 8, short.MaxValue);
                ServiceLogger.LogException("Exception in Authorise", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Finalise(TransactionTypes finaliseType, double amountFinal, string transactionRef) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(finaliseType);
                parameters.Add("AmountFinal", System.Convert.ToString(amountFinal));
                parameters.Add("TransactionReference", transactionRef);
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Finalise SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 9, short.MaxValue);
                ServiceLogger.LogException("Exception in Finalise", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse GetTransactionResult(TransactionTypes transResultType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(transResultType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTransactionResult SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 10, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTransactionResult", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse ReprintLastReceipt(TransactionTypes reprintReceiptType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(reprintReceiptType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In ReprintLastReceipt SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 11, short.MaxValue);
                ServiceLogger.LogException("Exception in ReprintLastReceipt", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse GetTerminalStatus(TransactionTypes terminalStatusType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(terminalStatusType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTerminalStatus SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 12, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTerminalStatus", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse TerminalReadCard(TransactionTypes readCardType) 
        {
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(readCardType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);

                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In TerminalReadCard SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 13, short.MaxValue);
                ServiceLogger.LogException("Exception in TerminalReadCard", ex);
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse PrintReceipt(TransactionTypes printReceiptType)
        {
            try
            {

            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PrintReceipt SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in PrintReceipt", ex);
            }
            return _response;
        }

        private string GetPutOrPostData(IDictionary<string, string> parameters)
        {
            StringBuilder buffer = new StringBuilder();
            try
            {
                if (!(parameters == null || parameters.Count == 0))
                {
                    int i = 0;
                    foreach (string key in parameters.Keys)
                    {
                        if (i > 0)
                        {
                            buffer.AppendFormat("&{0}={1}", key, parameters[key]);
                        }
                        else
                        {
                            buffer.AppendFormat("{0}={1}", key, parameters[key]);
                        }
                        i++;
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetPutOrPostData SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetPutOrPostData", ex);
            }
            return buffer.ToString();
        }

        public string PutOrPostResponse(string url, string putData, bool isPostData)
        {
            try
            {
            /*  ASCIIEncoding encoding = new ASCIIEncoding();
              string postData =
                  "POSRegisterID=7444ae07-dc63-e49c-33e3-59a7c108cc80&POSRegisterName=MainHelloRegister&POSBusinessName=toys4nzABC&POSVendorName=Till2Go";


              byte[] data = encoding.GetBytes(postData);

              // Prepare web request...
              HttpWebRequest myRequest =
                  (HttpWebRequest)WebRequest.Create("https://api-dev.smart-connect.cloud/POS/Pairing/81517031");
              myRequest.Method = "PUT";
              myRequest.ContentType = "application/x-www-form-urlencoded";
              myRequest.ContentLength = data.Length;
              Stream newStream = myRequest.GetRequestStream();
              // Send the data.
              newStream.Write(data, 0, data.Length);
              var def = myRequest.GetResponse();
              newStream.Close();
              if (url.StartsWith("https"))
                  System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Ssl3;

              ASCIIEncoding encoding = new ASCIIEncoding();
              byte[] data = encoding.GetBytes(putData);

              HttpWebRequest myRequest = (HttpWebRequest)WebRequest.Create(url);
              myRequest.Method = "PUT";
              myRequest.ContentType = SmartConnectConstraints.ContentType;
              myRequest.ContentLength = data.Length;
              Stream newStream = myRequest.GetRequestStream();
              // Send the data.
              newStream.Write(data, 0, data.Length);
              var def = myRequest.GetResponse();
              newStream.Close();*/


                HttpContent httpContent = new StringContent(putData, Encoding.UTF8);
                httpContent.Headers.ContentType = new MediaTypeHeaderValue(SmartConnectConstraints.ContentType);

                using (HttpClient httpClient = new HttpClient())
                {
                    //httpClient.Timeout = 120000; 
                    HttpResponseMessage response;
                    if (isPostData)
                    {
                        response = httpClient.PostAsync(url, httpContent).Result;
                    }
                    else
                    {
                        response = httpClient.PutAsync(url, httpContent).Result;
                    }

                    if (response.IsSuccessStatusCode)
                    {
                        string result = response.Content.ReadAsStringAsync().Result;
                        return result;
                    }                    
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetPutOrPostData SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetPutOrPostData", ex);
            } 
            return null;
        }

        public IDictionary<string, string> AddApiParameters(TransactionTypes transactionParam)
        {
            IDictionary<string, string> parameters = new Dictionary<string, string>();
            parameters.Add("POSRegisterID", transactionParam.PosRegisterId);
            parameters.Add("POSBusinessName", transactionParam.PosBusinessName);
            parameters.Add("POSVendorName", transactionParam.PosVendorName);
            parameters.Add("TransactionType", transactionParam.Transactiontype);

            return parameters;
        }

        public SmartConnectResponse DeSerializeResponse(string response)
        {            
            var smartConnectResponse = new SmartConnectResponse();
            try
            {
                smartConnectResponse = JsonConvert.DeserializeObject<SmartConnectResponse>(response);

                if (smartConnectResponse.data.Result == "OK")
                {
                    smartConnectResponse.ResponseSuccessful = true;
                }
                else
                {
                    smartConnectResponse.ResponseSuccessful = false;
                }
                      /* if (int.Parse(jo["errno"].ToString()) == 0)
                       {
                           var info = (JObject)JsonConvert.DeserializeObject(jo["results"].ToString());
                           if (info != null)
                           {
                               //smartConnectResponse.TransactionId = info["transactionId"].ToString();
                               //smartConnectResponse.TransactionTime = (DateTimeOffset)info["transactionTimeStamp"];
                               //smartConnectResponse.MerchantId = info["merchantId"].ToString();
                               //smartConnectResponse.DeviceId = info["deviceID"].ToString();
                               //smartConnectResponse.SmartConnectData.TransactionResult = info["transactionResult"].ToString();
                               //smartConnectResponse.SmartConnectData.Receipt = info["Receipt"].ToString();
                               //smartConnectResponse.SmartConnectData.RequestId = info["RequestId"].ToString();
                               //smartConnectResponse.SmartConnectData.AcquirerRef = info["TransactionResult"].ToString();
                               //smartConnectResponse.SmartConnectData.AccountType = info["TransactionResult"].ToString();
                               //smartConnectResponse.SmartConnectData.Timestamp = (DateTime)info["Timestamp"];
                               //smartConnectResponse.SmartConnectData.Result = info["Result"].ToString();
                               //smartConnectResponse.SmartConnectData.Function = info["Function"].ToString();
                               //smartConnectResponse.SmartConnectData.AuthId = info["AuthId"].ToString();
                               //smartConnectResponse.SmartConnectData.CardPan = info["CardPan"].ToString();
                               //smartConnectResponse.SmartConnectData.AmountTotal = info["AmountTotal"].ToString();
                               //smartConnectResponse.SmartConnectData.Merchant = info["Merchant"].ToString();
                               //smartConnectResponse.SmartConnectData.CardType = info["CardType"].ToString();
                               //smartConnectResponse.SmartConnectData.TerminalRef = info["TerminalRef"].ToString();
                               //smartConnectResponse.SmartConnectData.AmountSurcharge = info["AmountSurcharge"].ToString();
                               //smartConnectResponse.SmartConnectData.AmountTip = info["AmountTip"].ToString();
                           }
                       }
                       else
                       {
                           smartConnectResponse.ResponseSuccessful = false;
                           smartConnectResponse.ResponseMessage = "Errcode: " + jo["errno"].ToString() + " " + jo["message"].ToString();
                       }
                   }*/
                
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Deserialize Response SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in Deserialize Response", ex);
            }
            return smartConnectResponse;
        }
    }
}
