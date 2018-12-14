using System;
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
using Smartpay.Eftpos;

namespace MenumateServices.WCFServices
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
    public class WCFServiceSmartConnect : IWCFSmartConnect
    {
        private SmartConnectResponse smartConnectResponse;
        private List<string> stringList;
 
        public WCFServiceSmartConnect()
        {
            smartConnectResponse = new SmartConnectResponse();
            stringList = new List<string>();
        }        

        public SmartConnectResponse Pairing(PairingTerminal param)
        {
            try
            {
                stringList.Add("====================Pairing=========================================================");
                string requesturl = SmartConnectConstraints.PairingBaseAddress + "/" + param.PairingCode;
                stringList.Add("URL is:                     " + requesturl);
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters.Add("POSRegisterID", param.PosRegisterId);
                parameters.Add("POSRegisterName", param.PosRegisterName);
                parameters.Add("POSBusinessName", param.PosBusinessName);
                parameters.Add("POSVendorName", param.PosVendorName);
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(requesturl, putData, false);
                stringList.Add("pairing Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In pairing  SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 1, short.MaxValue);
                ServiceLogger.LogException("Exception in Pairing", ex);
                stringList.Add("Exception in pairing " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();    
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Logon(TransactionTypes logonType) 
        {
            try
            {
                stringList.Add("====================Logon=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                logonType.Transactiontype = Function.Logon;
                parameters = AddApiParameters(logonType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("Logon Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);                
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Logon SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 1, short.MaxValue);
                ServiceLogger.LogException("Exception in Logon", ex);
                stringList.Add("Exception in Logon " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse SettlementInquiry(TransactionTypes settlementEnquiryType) 
        {
            try
            {
                stringList.Add("====================SettlementInquiry=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                settlementEnquiryType.Transactiontype = Function.SettlementInquiry;
                parameters = AddApiParameters(settlementEnquiryType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("SettlementInquiry Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SettlementInquiry SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 2, short.MaxValue);
                ServiceLogger.LogException("Exception in SettlementInquiry", ex);
                stringList.Add("Exception in SettlementInquiry " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse SettlementCutover(TransactionTypes settlementCutoverType) 
        {
            try
            {
                stringList.Add("====================SettlementCutover=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                settlementCutoverType.Transactiontype = Function.SettlementCutover;
                parameters = AddApiParameters(settlementCutoverType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("SettlementCutover Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SettlementCutover SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 3, short.MaxValue);
                ServiceLogger.LogException("Exception in SettlementCutover", ex);
                stringList.Add("Exception in  SettlementCutover: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Purchase(TransactionTypes purchaseType, double amount) 
        {
            try
            {
                stringList.Add("====================Purchase=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                purchaseType.Transactiontype = Function.Purchase;
                parameters = AddApiParameters(purchaseType);
                parameters.Add("AmountTotal", GetAmount(amount)); 
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Purchase Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Purchase SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in Purchase", ex);
                stringList.Add("Exception in  Purchase: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse PurchasePlusCash(TransactionTypes purchasePlusCashType, double totalAmount, double cashAmount) 
        {
            try
            {
                stringList.Add("====================PurchasePlusCash=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                purchasePlusCashType.Transactiontype = Function.PurchasePlusCash;
                parameters = AddApiParameters(purchasePlusCashType);
                parameters.Add("AmountTotal", GetAmount(totalAmount + cashAmount));
                parameters.Add("AmountCash", GetAmount(cashAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("PurchasePlusCash Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PurchasePlusCash SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 5, short.MaxValue);
                ServiceLogger.LogException("Exception in PurchasePlusCash", ex);
                stringList.Add("Exception in  PurchasePlusCash: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse CashOutOnly(TransactionTypes cashOutOnlyType, double cashAmount)
        {
            try
            {
                stringList.Add("====================CashOutOnly=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                cashOutOnlyType.Transactiontype = Function.CashAdvance;
                parameters = AddApiParameters(cashOutOnlyType);
                parameters.Add("AmountTotal", GetAmount(cashAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("CashOutOnly Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
                WriteToFile(stringList);
                stringList.Clear();
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In CashOutOnly SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 6, short.MaxValue);
                ServiceLogger.LogException("Exception in CashOutOnly", ex);
                stringList.Add("Exception in  CashOutOnly: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Refund(TransactionTypes refundType, double refAmount) 
        {
            try
            {
                stringList.Add("====================Refund=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                refundType.Transactiontype = Function.Refund;
                parameters = AddApiParameters(refundType);
                parameters.Add("AmountTotal", GetAmount(refAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Refund Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Refund SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 7, short.MaxValue);
                ServiceLogger.LogException("Exception in Refund", ex);
                stringList.Add("Exception in  Refund: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Authorise(TransactionTypes authoriseType, double amountAuth, string transactionRef) 
        {
            try
            {
                stringList.Add("====================Authorise=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(authoriseType);
                parameters.Add("AmountAuth", GetAmount(amountAuth));
                parameters.Add("TransactionReference", transactionRef);
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Authorise Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Authorise SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 8, short.MaxValue);
                ServiceLogger.LogException("Exception in Authorise", ex);
                stringList.Add("Exception in  Authorise: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse Finalise(TransactionTypes finaliseType, double amountFinal, string transactionRef) 
        {
            try
            {
                stringList.Add("====================Finalise=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(finaliseType);
                parameters.Add("AmountFinal", GetAmount(amountFinal));
                parameters.Add("TransactionReference", transactionRef);
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Finalise Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Finalise SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 9, short.MaxValue);
                ServiceLogger.LogException("Exception in Finalise", ex);
                stringList.Add("Exception in  Finalise: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse GetTransactionResult(TransactionTypes transResultType) 
        {
            try
            {
                stringList.Add("====================GetTransactionResult=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                transResultType.Transactiontype = Function.GetTransResult;
                parameters = AddApiParameters(transResultType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("GetTransactionResult Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTransactionResult SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 10, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTransactionResult", ex);
                stringList.Add("Exception in  GetTransactionResult: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse ReprintLastReceipt(TransactionTypes reprintReceiptType) 
        {
            try
            {
                stringList.Add("====================ReprintLastReceipt=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                reprintReceiptType.Transactiontype = Function.ReprintReceipt;
                parameters = AddApiParameters(reprintReceiptType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("ReprintLastReceipt Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In ReprintLastReceipt SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 11, short.MaxValue);
                ServiceLogger.LogException("Exception in ReprintLastReceipt", ex);
                stringList.Add("Exception in  ReprintLastReceipt: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse GetTerminalStatus(TransactionTypes terminalStatusType) 
        {
            try
            {
                stringList.Add("====================GetTerminalStatus=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                terminalStatusType.Transactiontype = Function.GetStatus;
                parameters = AddApiParameters(terminalStatusType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("GetTerminalStatus Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTerminalStatus SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 12, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTerminalStatus", ex);
                stringList.Add("Exception in  GetTerminalStatus: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse TerminalReadCard(TransactionTypes readCardType) 
        {
            try
            {
                stringList.Add("====================TerminalReadCard=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                parameters = AddApiParameters(readCardType);
                string postData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, postData, true);
                stringList.Add("TerminalReadCard Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In TerminalReadCard SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 13, short.MaxValue);
                ServiceLogger.LogException("Exception in TerminalReadCard", ex);
                stringList.Add("Exception in  GetTerminalStatus: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
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
                stringList.Add("Exception in  PrintReceipt: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse MerchantPurchaseWithQRCode(TransactionTypes purchaseType, double amount)
        {
            try
            {
                stringList.Add("====================PurchaseWithQRCode=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                purchaseType.Transactiontype = "QR.Merchant.Purchase";
                parameters = AddApiParameters(purchaseType);
                parameters.Add("AmountTotal", GetAmount(amount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Purchase Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PurchaseWithQRCode SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in Purchase", ex);
                stringList.Add("Exception in  Purchase: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse ConsumerPurchaseWithQRCode(TransactionTypes purchaseType, double amount)
        {
            try
            {
                stringList.Add("====================PurchaseWithQRCode=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                purchaseType.Transactiontype = "QR.Consumer.Purchase";
                parameters = AddApiParameters(purchaseType);
                parameters.Add("AmountTotal", GetAmount(amount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Purchase Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PurchaseWithQRCode SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in Purchase", ex);
                stringList.Add("Exception in  Purchase: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        public SmartConnectResponse QRCodeRefund(TransactionTypes refundType, double refAmount)
        {
            try
            {
                stringList.Add("====================Refund=========================================================");
                IDictionary<string, string> parameters = new Dictionary<string, string>();
                refundType.Transactiontype = "QR.Refund";
                parameters = AddApiParameters(refundType);
                parameters.Add("AmountTotal", GetAmount(refAmount));
                string putData = GetPutOrPostData(parameters);
                string response = PutOrPostResponse(SmartConnectConstraints.TransactionBaseAddress, putData, true);
                stringList.Add("Refund Response: " + response);
                smartConnectResponse = DeSerializeResponse(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Refund SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 7, short.MaxValue);
                ServiceLogger.LogException("Exception in Refund", ex);
                stringList.Add("Exception in  Refund: " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            return smartConnectResponse;
        }

        private string GetPutOrPostData(IDictionary<string, string> parameters)
        {
            StringBuilder buffer = new StringBuilder();
            try
            {
                stringList.Add("Inside GetPutOrPostData()");
                if (!(parameters == null || parameters.Count == 0))
                {
                    int i = 0;
                    foreach (string key in parameters.Keys)
                    {
                        if (i > 0)
                        {
                            if(String.Compare(key,"POSBusinessName") == 0)
                            {
                                buffer.AppendFormat("&{0}={1}", key,UrlEncode(parameters[key]));
                            }
                            else
                            {
                                buffer.AppendFormat("&{0}={1}", key, parameters[key]);
                            }
                        }
                        else
                        {
                            buffer.AppendFormat("{0}={1}", key, parameters[key]);
                        }
                        i++;
                    }
                    stringList.Add("String to post: " + buffer.ToString());
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetPutOrPostData SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetPutOrPostData", ex);
                stringList.Add("Exception in GetPutOrPostData()    " + ex.Message);
            }            
            finally
            {
                WriteAndClearStringList();    
            }
            return buffer.ToString();
        }

        public string PutOrPostResponse(string url, string putData, bool isPostData)
        {
            string result = "";
            try
            {
                stringList.Add("Inside PutOrPostResponse()");
                HttpContent httpContent = new StringContent(putData, Encoding.UTF8);           
                httpContent.Headers.ContentType = new MediaTypeHeaderValue(SmartConnectConstraints.ContentType);
			
                using (HttpClient httpClient = new HttpClient())
                {
                    stringList.Add("put or post Request at:       " + DateTime.Now.ToString("ddMMMyyyy"));
                    stringList.Add("put or post Request Time:     " + DateTime.Now.ToString("hhmmss"));
                    TimeSpan ts = TimeSpan.FromMinutes(2);
                    httpClient.Timeout = ts;
                    HttpResponseMessage response;
                    if (isPostData)
                    {
                        response = httpClient.PostAsync(url, httpContent).Result;
                    }
                    else
                    {
                        response = httpClient.PutAsync(url, httpContent).Result;
                    }
                    stringList.Add("Response  at :       " + DateTime.Now.ToString("ddMMMyyyy"));
                    stringList.Add("Response Time :     " + DateTime.Now.ToString("hhmmss"));
                    if (response.IsSuccessStatusCode)
                    {
                        result = response.Content.ReadAsStringAsync().Result;
                    }                   
                }
                stringList.Add("Result of put or post response is: " + result);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetPutOrPostData SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetPutOrPostData", ex);
                stringList.Add("Exception in PutOrPostResponse()   " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }
            
            return result;
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
            var deSerializeResponse = new SmartConnectResponse();
            try
            {
                stringList.Add("Inside DeSerializeResponse() parameter is => " + response);    
                deSerializeResponse.ResponseSuccessful = false;
                if (!string.IsNullOrEmpty(response))
                {
                    deSerializeResponse = JsonConvert.DeserializeObject<SmartConnectResponse>(response);

                    if ((deSerializeResponse.Data != null && deSerializeResponse.Data.TransactionResult.ToUpper() == "OK-ACCEPTED") || (deSerializeResponse.Result != null && deSerializeResponse.Result.ToLower() == "success"))
                    {
                        if (deSerializeResponse.Data != null)
                        {
                            deSerializeResponse.Data.AmountTotal = Convert.ToString(Convert.ToDouble(deSerializeResponse.Data.AmountTotal) / 100);
                            deSerializeResponse.Data.AmountTip = Convert.ToString(Convert.ToDouble(deSerializeResponse.Data.AmountTip) / 100);
                            deSerializeResponse.Data.AmountSurcharge = Convert.ToString(Convert.ToDouble(deSerializeResponse.Data.AmountSurcharge) / 100);
                        }
                        deSerializeResponse.ResponseSuccessful = true;
                    }
                }
                stringList.Add("DeSerializeResponse: IsResponseSuccessful=> " + deSerializeResponse.ResponseSuccessful); 
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Deserialize Response SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in Deserialize Response", ex);
                stringList.Add("Exception in DeSerializeResponse() " + ex.Message);
            }
            finally
            {
                WriteAndClearStringList();
            }           
            return deSerializeResponse;
        }

        private string GetAmount(double amount)
        {
            uint retAmount = 0;
            try
            {
                Decimal decAmount = Convert.ToDecimal(amount);
                retAmount = (uint)(decAmount * 100);                
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException("Exception in GetAmount", ex);
            }
            return System.Convert.ToString(retAmount);
        }

        private void WriteToFile(List<string> list)
        {
            try
            {
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                string location = Path.Combine(path, "logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                location = Path.Combine(location, "Smart Connect Logs");

                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "SmartConnectLogs " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
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

        private void WriteAndClearStringList()
        {
            WriteToFile(stringList);
            stringList.Clear();
        }

        private readonly static string reservedCharacters = "!*'();:@&=+$,/?%#[]";
        public static string UrlEncode(string value)
        {
            if (String.IsNullOrEmpty(value))
                return String.Empty;

            var sb = new StringBuilder();

            foreach (char @char in value)
            {
                if (reservedCharacters.IndexOf(@char) == -1)
                    sb.Append(@char);
                else
                    sb.AppendFormat("%{0:X2}", (int)@char);
            }
            return sb.ToString();
        }
    }
}
