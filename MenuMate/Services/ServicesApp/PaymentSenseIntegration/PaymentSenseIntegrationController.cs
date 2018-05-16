using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PaymentSenseIntegration.Domain;
using System.IO;
using System.Diagnostics;
using Newtonsoft.Json;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Timers;
using System.Threading;

namespace PaymentSenseIntegration
{
    public class PaymentSenseIntegrationController
    {    
        private List<string> stringList;        
        public PaymentSenseIntegrationController()
        {
            stringList = new List<string>();
        }

        public PACTerminalWrapper GetAllTerminals(AuthorizationDetails autorizationDetails)
        {
            PACTerminalWrapper deSerializeResponse = new PACTerminalWrapper();
            try
            {
                TransactionRequest request = new TransactionRequest();
                var response = DoAuthorization(autorizationDetails);
                deSerializeResponse = JsonConvert.DeserializeObject<PACTerminalWrapper>(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetAllCardTerminals", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                //ServiceLogger.LogException("Exception in GetAllCardTerminals", ex);
                stringList.Add("Exception in GetAllCardTerminals()");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return deSerializeResponse;
        }

        private string DoAuthorization(AuthorizationDetails autorizationDetails)
        {
            string authorizationResponse = "";
            try
            {
                string apiUrl = autorizationDetails.URL;

                using (HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));
                    authorizationResponse = client.GetStringAsync(apiUrl).Result;
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In DoAuthorizatione PaymentSense", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
               // ServiceLogger.LogException("Exception in DoAuthorization", ex);
                //stringList.Add("Exception in DoAuthorization()");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return authorizationResponse;
        }

        public PACTerminal PingTerminal(AuthorizationDetails autorizationDetail)
        {
            PACTerminal terminalDetails = new PACTerminal();
            try
            {
                TransactionRequest request = new TransactionRequest();
                var response = DoAuthorization(autorizationDetail);
                terminalDetails = JsonConvert.DeserializeObject<PACTerminal>(response);
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetAllCardTerminals", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                //ServiceLogger.LogException("Exception in GetAllCardTerminals", ex);
                stringList.Add("Exception in GetAllCardTerminals()");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return terminalDetails;
        }

        public TransactionDataResponse DoTransaction(AuthorizationDetails autorizationDetails, TransactionRequest request)
        {
            TransactionDataResponse transactionData = new TransactionDataResponse();
            try
            {
                stringList.Add("====================DoTransaction=========================================================");
                string apiUrl = autorizationDetails.URL;     
                        
                using (HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));

                    request.amount *= 100; 
                    var serializedJson = JsonConvert.SerializeObject(request);
                    HttpContent httpContent = new StringContent(serializedJson, Encoding.UTF8);
                    httpContent.Headers.ContentType = new MediaTypeHeaderValue("application/json");

                    HttpResponseMessage response = client.PostAsync(apiUrl, httpContent).Result;
                    string result = "";
                    if (response.IsSuccessStatusCode)
                    {
                        result = response.Content.ReadAsStringAsync().Result;
                    }
                    PaymentSenseResponse deSerializedResponse = JsonConvert.DeserializeObject<PaymentSenseResponse>(result);// await responseNew.Content.ReadAsStringAsync());
                    autorizationDetails.URL = autorizationDetails.URL + "/" + deSerializedResponse.RequestId;

                    //wait for a period of time or untill you get the transaction finished response.
                    transactionData = WaitAndGetResponse(autorizationDetails);

                    int value = string.Compare(transactionData.TransactionResult, "SUCCESSFUL", true);
                    if (value== 0)
                    {
                        ConvertInToFinalValue(ref transactionData);
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Purchase PaymentSense", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                //ServiceLogger.LogException("Exception in Purchase", ex);
                stringList.Add("Exception in  Purchase: ");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return transactionData;
        }

        private TransactionDataResponse WaitAndGetResponse(AuthorizationDetails autorizationDetails)
        {
            TransactionDataResponse response = new TransactionDataResponse();
            try
            {
                bool waitflag = true;

                Stopwatch watch = new Stopwatch();
                watch.Reset();
                watch.Start();
                while (waitflag)
                {
                    Thread.Sleep(2000);
                    //Get Response from Eftpos Terminal to check  transaction is in which state.
                    response = GetTransactionDataForRequestedId(autorizationDetails);

                    //If transation is finished then return.
                    if (IsCardTransactionCompleted(response.Notifications))
                        break;

                    //If didn't get response in the mentioned time then also return;
                    if (watch.Elapsed.TotalMinutes > 3.00)
                        break; //to do make response as false;
                }
            }
            catch (Exception ex)
            {
                //EventLog.WriteEntry("In WaitForResponse Smartlink", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 28, short.MaxValue);
                // ServiceLogger.LogException("Exception in WaitForResponse", ex);
            }
            return response;
        }

        private TransactionDataResponse GetTransactionDataForRequestedId(AuthorizationDetails autorizationDetails)
        {
            TransactionDataResponse transactionData = new TransactionDataResponse();
            try
            {
                stringList.Add("====================GetTransactionDetailsForRequestedId=========================================================");
                string apiUrl = autorizationDetails.URL;
                using (HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));
                    var authorizationResponse = client.GetStringAsync(apiUrl).Result;
                    transactionData = JsonConvert.DeserializeObject<TransactionDataResponse>(authorizationResponse);
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTransactionDetailsForRequestedId", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                //ServiceLogger.LogException("Exception in GetTransactionDetailsForRequestedId", ex);
                stringList.Add("Exception in GetTransactionDetailsForRequestedId()");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return transactionData;
        }

        private bool IsCardTransactionCompleted(string[] Notifications)
        {
            bool retval = false;
            try
            {
                foreach(string eventNotification in Notifications)
                {
                    int value = string.Compare(eventNotification, "TRANSACTION_FINISHED", true);
                    if (value == 0)
                    {
                        retval = true;
                        break;
                    }                     
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("IsCardTransactionCompleted", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                //ServiceLogger.LogException("Exception in GetTransactionDetailsForRequestedId", ex);
                stringList.Add("Exception in IsCardTransactionCompleted()");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return retval;
        }

        private void ConvertInToFinalValue(ref TransactionDataResponse responseData)
        {
            responseData.AmountBase = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountBase) / 100);
            responseData.AmountCashBack = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountCashBack) / 100);
            responseData.AmountGratuity = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountGratuity) / 100);
            responseData.AmountTotal = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountTotal) / 100);
        }

        private void ArrangeAndAssignReceipts(ReceiptLines receiptLines)
        {
            try
            {
                foreach (var item in receiptLines.Merchant)
                {
                    receiptLines.MerchantReceipt.Add(item.Value);
                }
                foreach (var item in receiptLines.Customer)
                {
                    receiptLines.CustomerReceipt.Add(item.Value);
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception Occured in Arranging Receipts at  :-                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
        }

        private void WriteToFile(List<string> list)
        {
            try
            {
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Smart Connect Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "PaymentSenseLogs " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
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
               // ServiceLogger.Log("Exception in Making File" + ex.Message);
            }
        }

        private void WriteAndClearStringList()
        {
            WriteToFile(stringList);
            stringList.Clear();
        }
    }
}
