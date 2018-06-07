using System;
using System.Collections.Generic;
using System.Text;
using PaymentSenseIntegration.Domain;
using System.IO;
using System.Diagnostics;
using Newtonsoft.Json;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading;
using PaymentSenseIntegration.Tools;
using PaymentSenseIntegration.Domain.SignatureRequest;
using PaymentSenseIntegration.Domain.ResponseData;

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
                stringList.Add("Request to Get All Terminals");
                TransactionRequest request = new TransactionRequest();
                var response = DoAuthorization(autorizationDetails);
                deSerializeResponse = JsonConvert.DeserializeObject<PACTerminalWrapper>(response);
                stringList.Add("Response Deserialized in PACTerminalWrapper class");
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetAllCardTerminals", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetAllCardTerminals", ex);
                stringList.Add("Exception in GetAllCardTerminals()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            WriteAndClearStringList();
            return deSerializeResponse;
        }

        private string DoAuthorization(AuthorizationDetails autorizationDetails)
        {
            string authorizationResponse = "";
            try
            {
                string apiUrl = autorizationDetails.URL;
                stringList.Add("Creating Authorization Header in DoAuthorization() function.");
                stringList.Add("URL is :-                           " + autorizationDetails.URL);
                stringList.Add("User Name is :-                     " + autorizationDetails.UserName);
                stringList.Add("API Key/ Password is :-             " + autorizationDetails.Password);
                using (HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));
                    stringList.Add("Authorization request created at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    authorizationResponse = client.GetStringAsync(apiUrl).Result;
                    stringList.Add("Authorization response  Got at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In DoAuthorizatione PaymentSense", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in DoAuthorization", ex);
                stringList.Add("Exception in DoAuthorization()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            return authorizationResponse;
        }

        public PACTerminal PingTerminal(AuthorizationDetails autorizationDetail)
        {
            PACTerminal terminalDetails = new PACTerminal();
            try
            {
                stringList.Add("Request to Get PDQ Terminal Status");
                TransactionRequest request = new TransactionRequest();
                var response = DoAuthorization(autorizationDetail);
                terminalDetails = JsonConvert.DeserializeObject<PACTerminal>(response);
                stringList.Add("Response Deserialized in PACTerminal class with terminal Details");
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetAllCardTerminals", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetAllCardTerminals", ex);
                stringList.Add("Exception in GetAllCardTerminals()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            WriteAndClearStringList();
            return terminalDetails;
        }

        public PostRequestResponse DoTransaction(AuthorizationDetails autorizationDetails, TransactionRequest request)
        {
            PostRequestResponse postedResponse = new PostRequestResponse();
            try
            {
                stringList.Add("====================DoTransaction=========================================================");
                stringList.Add("====================Authorization Details are=========================================================");
                stringList.Add("URL is :-                           " + autorizationDetails.URL);
                stringList.Add("User Name is :-                     " + autorizationDetails.UserName);
                stringList.Add("API Key/ Password is :-             " + autorizationDetails.Password);
                stringList.Add("====================Transaction Details are=========================================================");
                stringList.Add("Transaction Type  is :-                     " + request.transactionType);
                stringList.Add("Transaction Amount s :-                           " + request.amount);                
                stringList.Add("Currency is :-             " + request.currency);
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
                    stringList.Add("Post Request for transaction created at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    HttpResponseMessage response = client.PostAsync(apiUrl, httpContent).Result;
                    stringList.Add("Response got at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    string result = "";
                    if (response.IsSuccessStatusCode)
                    {
                        result = response.Content.ReadAsStringAsync().Result;
                        stringList.Add("Response is succesfull and is :-                                      " + result);
                        postedResponse = JsonConvert.DeserializeObject<PostRequestResponse>(result);
                        stringList.Add("Response Deserialized.");
                    }
                    else
                    {
                        stringList.Add("Response is Unsuccesfull and response message is: " + response.ReasonPhrase);
                        stringList.Add("Unavailable terminal or no data available. Please Check that PDQ is available.");
                    }                
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In DoTransaction PaymentSense", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in DoTransaction", ex);
                stringList.Add("Exception in  DoTransaction: ");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            WriteAndClearStringList();
            return postedResponse;
        }

        public ReportResponseData PrintReports(AuthorizationDetails autorizationDetails, Reports report)
        {
            ReportResponseData reportData = new ReportResponseData();
            reportData.Report = new List<string>();
            try
            {
                stringList.Add("====================PrintReports=========================================================");
                stringList.Add("====================Authorization Details are=========================================================");
                stringList.Add("URL is :-                           " + autorizationDetails.URL);
                stringList.Add("User Name is :-                     " + autorizationDetails.UserName);
                stringList.Add("API Key/ Password is :-             " + autorizationDetails.Password);
                stringList.Add("reportType Type  is :-                     " + report.reportType);
                string apiUrl = autorizationDetails.URL;

                using (HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));

                    var serializedJson = JsonConvert.SerializeObject(report);
                    HttpContent httpContent = new StringContent(serializedJson, Encoding.UTF8);
                    httpContent.Headers.ContentType = new MediaTypeHeaderValue("application/json");
                    stringList.Add("Post Request for report generation created at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    HttpResponseMessage response = client.PostAsync(apiUrl, httpContent).Result;
                    stringList.Add("Response got at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    string result = "";
                    if (response.IsSuccessStatusCode)
                    {
                        //result = response.Content.ReadAsStringAsync().Result;
                        //stringList.Add("Response is succesfull and is :-                                      " + result);
                        //retVal = true;
                        result = response.Content.ReadAsStringAsync().Result;
                        stringList.Add("Response is succesfull and is :-                                      " + result);
                        PostRequestResponse deSerializedResponse = JsonConvert.DeserializeObject<PostRequestResponse>(result);
                        stringList.Add("Response Deserialized.");
                        autorizationDetails.URL = autorizationDetails.URL + "/" + deSerializedResponse.RequestId;

                        stringList.Add("====================Get Transaction Data correspoding to above created request id. Details of id and URL are=========================================================");
                        stringList.Add("New URL is :-                     " + autorizationDetails.URL);
                        stringList.Add("Request Id is :-                           " + deSerializedResponse.RequestId);
                        //wait for a period of time or untill you get the transaction finished response.
                        reportData = WaitAndGetReport(autorizationDetails);

                       // int value = string.Compare(reportData.TransactionResult.ToUpper(), "SUCCESSFUL", true);
                        if (reportData.reportLines.Length > 1)
                        {
                            ArrangeAndAssignReport(reportData);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In PrintReports", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                ServiceLogger.LogException("Exception in PrintReports", ex);
                stringList.Add("Exception in  PrintReports: ");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            WriteAndClearStringList();
            return reportData;
        }       

        public TransactionDataResponse GetTransactionDataForRequestedId(AuthorizationDetails autorizationDetails)
        {
            TransactionDataResponse transactionData = new TransactionDataResponse();
            try
            {
                stringList.Add("====================GetTransactionDetailsForRequestedId=========================================================");
                string apiUrl = autorizationDetails.URL;
                stringList.Add("==================== api url=============================" + apiUrl);
                using (HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));
                     
                    stringList.Add("==================== Before authorization=============================" );
                    var authorizationResponse = client.GetStringAsync(apiUrl).Result;
                    stringList.Add("Response For Requested Id is as follows:-                                  ");
                    stringList.Add(authorizationResponse);
                    transactionData = JsonConvert.DeserializeObject<TransactionDataResponse>(authorizationResponse);
                    stringList.Add("Response Deserialized.  ");

                    if (transactionData.TransactionResult != null)
                    {
						int value = 1;
                        value = string.Compare(transactionData.TransactionResult, "SUCCESSFUL", true);
                        stringList.Add("value " + value);
                        if (value == 0)
                        {
                            ConvertInToFinalValue(ref transactionData);
                            ArrangeAndAssignReceipts(ref transactionData);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetTransactionDetailsForRequestedId", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetTransactionDetailsForRequestedId", ex);
                stringList.Add("Exception in GetTransactionDetailsForRequestedId()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            WriteAndClearStringList();
            return transactionData;
        }
        private ReportResponseData WaitAndGetReport(AuthorizationDetails autorizationDetails)
        {
            ReportResponseData reportData = new ReportResponseData();
            try
            {
                stringList.Add("====================Inside WaitAndGetResponse()=========================================================");
                bool waitflag = true;
                Stopwatch watch = new Stopwatch();
                watch.Reset();
                watch.Start();
                while (waitflag)
                {
                    Thread.Sleep(1000);
                    //Get Response from Eftpos Terminal to check  transaction is in which state.
                    reportData = GetReportDataForRequestID(autorizationDetails);

                    //If transation is finished then return.
                    if (IsReportCompleted(reportData.notifications))
                    {
                        stringList.Add("====================Got Transaction Finished Notification ========================================================");
                        break;
                    }

                    //If didn't get response in the mentioned time then also return;
                    if (watch.Elapsed.TotalMinutes > 2.00)
                    {
                        reportData.reportResult = "TIMED_OUT";
                        stringList.Add("====================Didn't get reponse in specified time interval. SO Time out occured. ========================================================");
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In WaitAndGetReport", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in WaitAndGetReport", ex);
                stringList.Add("Exception in WaitAndGetReport()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            return reportData;
        }
        private ReportResponseData GetReportDataForRequestID(AuthorizationDetails autorizationDetails)
        {
            ReportResponseData reportData = new ReportResponseData();
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
                    stringList.Add("Response For Requested Id is as follows:-                                  ");
                    stringList.Add(authorizationResponse);
                    reportData = JsonConvert.DeserializeObject<ReportResponseData>(authorizationResponse);
                    stringList.Add("Response Deserialized.  ");
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In GetReportDataForTransactionID", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in GetReportDataForTransactionID", ex);
                stringList.Add("Exception in GetReportDataForTransactionID()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            return reportData;
        }
        public TransactionDataResponse SignatureVerificationForRequestedId(AuthorizationDetails autorizationDetails, SignatureRequest signRequest)
        {
            TransactionDataResponse transactionData = new TransactionDataResponse();
            try
            {
                stringList.Add("====================SignatureVerificationForRequestedId=========================================================");
                stringList.Add("URL is :-                           " + autorizationDetails.URL);
                stringList.Add("User Name is :-                     " + autorizationDetails.UserName);
                stringList.Add("API Key/ Password is :-             " + autorizationDetails.Password);
                stringList.Add("signature accepted is :-                     " + signRequest.accepted);

                string apiUrl = autorizationDetails.URL;
                using ( HttpClient client = new HttpClient())
                {
                    client.BaseAddress = new Uri(apiUrl);
                    client.DefaultRequestHeaders.Accept.Clear();
                    client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic",
                                                                                    Convert.ToBase64String(System.Text.ASCIIEncoding.ASCII.GetBytes(string.Format("{0}:{1}",
                                                                                                    autorizationDetails.UserName, autorizationDetails.Password))));
                    client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/connect.v1+json"));

                    var serializedJson = JsonConvert.SerializeObject(signRequest);
                    HttpContent httpContent = new StringContent(serializedJson, Encoding.UTF8);
                    httpContent.Headers.ContentType = new MediaTypeHeaderValue("application/json");
                    stringList.Add("Post Request for signature verification created at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    HttpResponseMessage response = client.PutAsync(apiUrl, httpContent).Result;
                    stringList.Add("Response got at:-                                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                    string result = "";
                    if (response.IsSuccessStatusCode)
                    {
                        result = response.Content.ReadAsStringAsync().Result;
                        stringList.Add("Response is succesfull and is :-                                      " + result);
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In SignatureVerificationForRequestedId", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in SignatureVerificationForRequestedId", ex);
                stringList.Add("Exception in SignatureVerificationForRequestedId()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
            WriteAndClearStringList();
            return transactionData;
        }

        private bool IsReportCompleted(string[] Notifications)
        {
            bool retval = false;
            try
            {
                foreach (string eventNotification in Notifications)
                {
                    int value = string.Compare(eventNotification, "REPORT_FINISHED", true);
                    if (value == 0)
                    {
                        stringList.Add("Report Transaction found in function IsReportCompleted().  ");
                        retval = true;
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("IsReportCompleted", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 14, short.MaxValue);
                ServiceLogger.LogException("Exception in IsReportCompleted", ex);
                stringList.Add("Exception in IsReportCompleted()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
                throw;
            }
            return retval;
        }

        private void ConvertInToFinalValue(ref TransactionDataResponse responseData)
        {
            try
            {
                responseData.AmountBase = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountBase) / 100);
                responseData.AmountTotal = System.Convert.ToString((System.Convert.ToDecimal(responseData.AmountTotal) - System.Convert.ToDecimal(responseData.AmountCashBack)) / 100);
                responseData.AmountCashBack = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountCashBack) / 100);
                responseData.AmountGratuity = System.Convert.ToString(System.Convert.ToDecimal(responseData.AmountGratuity) / 100);                                
            }
            catch (Exception ex)
            {
                stringList.Add("Exception in ConvertInToFinalValue()");
                stringList.Add("Exception is :-                                                  " + ex.Message);
                throw;
            }
        }

        private void ArrangeAndAssignReceipts(ref TransactionDataResponse data)
        {
            try
            {
                data.ReceiptLines.MerchantReceipt = new List<string>();
                data.ReceiptLines.CustomerReceipt = new List<string>();
                if (data.ReceiptLines.Merchant != null)
                {
                    foreach (var item in data.ReceiptLines.Merchant)
                    {
                        data.ReceiptLines.MerchantReceipt.Add(item.Value.TrimStart());
                    }
                }
                if (data.ReceiptLines.CustomerReceipt != null)
                {
                    foreach (var item in data.ReceiptLines.Customer)
                    {
                        data.ReceiptLines.CustomerReceipt.Add(item.Value.TrimStart());
                    }
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception Occured in Arranging Receipts at  :-                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
        }
        private void ArrangeAndAssignReport(ReportResponseData reportData)
        {
            try
            {
                reportData.Report = new List<string>();
                if (reportData.reportLines != null)
                {
                    foreach (var item in reportData.reportLines)
                    {
                        reportData.Report.Add(item.Value.TrimStart());
                    }
                }
            }
            catch (Exception ex)
            {
                stringList.Add("Exception Occured in Arranging Reports at  :-                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Exception is :-                                                  " + ex.Message);
            }
        }
        private void WriteToFile(List<string> list)
        {
            try
            {
                list.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Payment Sense Logs");
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
                list.Add("  ");
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
    }
}
