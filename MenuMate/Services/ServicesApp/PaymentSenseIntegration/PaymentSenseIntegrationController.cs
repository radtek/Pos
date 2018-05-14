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

        public TransactionData DoTransaction(AuthorizationDetails autorizationDetails, TransactionRequest request)
        {
            TransactionData transactionData = new TransactionData();
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
                    var serializedJson = JsonConvert.SerializeObject(request);

                  //  var stringContent = new StringContent(serializedJson, UnicodeEncoding.UTF8, "application/json");

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
                    transactionData = GetTransactionDataForRequestedId(autorizationDetails);
                }
            }
            catch (Exception ex)
            {
                EventLog.WriteEntry("In Purchase SmartConnect", ex.Message + "Trace" + ex.StackTrace, EventLogEntryType.Error, 4, short.MaxValue);
                //ServiceLogger.LogException("Exception in Purchase", ex);
                stringList.Add("Exception in  Purchase: ");
            }
            finally
            {
                WriteAndClearStringList();
            }
            return transactionData;
        }

        public TransactionData GetTransactionDataForRequestedId(AuthorizationDetails autorizationDetails)
        {
            TransactionData transactionData = new TransactionData();
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
                    transactionData = JsonConvert.DeserializeObject<TransactionData>(authorizationResponse);
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
