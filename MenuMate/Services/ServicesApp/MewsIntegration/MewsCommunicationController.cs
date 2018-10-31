﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using MewsIntegration.Domain;
using MewsIntegration.Utilities;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace MewsIntegration
{
    public class MewsCommunicationController
    {
        public MewsCommunicationController()
        {
            System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls | SecurityProtocolType.Ssl3 | (SecurityProtocolType)3072;
        }
        public List<Service> GetMewsServices(string platformAddress, BasicInquiry basicInquiry, List<string> logsList)
        {
            basicInquiry.ClientToken = MewsRequestAddress.ClientToken;
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.ServicesAll;
            logsList.Add("**************Request to get Services from Mews at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            HttpWebResponse webResponse = null;
            List<Service> services = new List<Service>();
            try
            {
                var request = GetWebRequest(url, basicInquiry, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Response is                                        " + retValue);
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                if (rootObject.Services != null)
                    services = rootObject.Services;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return services;
        }

        public List<AccountingCategory> GetMewsAccountingCategories(string platformAddress, BasicInquiry basicInquiry, List<string> logsList)
        {
            basicInquiry.ClientToken = MewsRequestAddress.ClientToken;
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.AccountingCategoriesAll;
            logsList.Add("**************Request to categories from Mews at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            HttpWebResponse webResponse = null;
            List<AccountingCategory> accountingCategories = new List<AccountingCategory>();
            try
            {
                var request = GetWebRequest(url, basicInquiry, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Response is                                        " + retValue);
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                if (rootObject.AccountingCategories != null)
                    accountingCategories = rootObject.AccountingCategories;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return accountingCategories;
        }

        public List<Outlet> GetMewsOutlets(string platformAddress, BasicInquiry basicInquiry, List<string> logsList)
        {
            basicInquiry.ClientToken = MewsRequestAddress.ClientToken;
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.OutletsAll;
            logsList.Add("**************Request to get Outlets from Mews at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            HttpWebResponse webResponse = null;
            List<Outlet> outlets = new List<Outlet>();
            try
            {
                var request = GetWebRequest(url, basicInquiry, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Response is                                        " + retValue);
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);

                if (rootObject.Outlets != null)
                    outlets = rootObject.Outlets;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return outlets;
        }

        public List<CustomerDetailsMews> GetCustomerSearchResult(string platformAddress, CustomerSearch customerSearch, List<string> logsList)
        {
            customerSearch.ClientToken = MewsRequestAddress.ClientToken;
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.CustomerSearch;
            logsList.Add("**************Request to get Customer from Mews at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            HttpWebResponse webResponse = null;
            Customers customers = new Customers();
            List<CustomerDetailsMews> customerDetailsMewsList = new List<CustomerDetailsMews>();
            try
            {
                var request = GetWebRequest(url, customerSearch, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Response is                                        " + retValue);
                var rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                if (rootObject != null)
                {
                    if (rootObject.Customers != null)
                    {
                        foreach (var customer in rootObject.Customers)
                        {
                            if (customer.CustomerDetails.Classifications == null)
                                customer.CustomerDetails.Classifications = new List<string>();

                            customers.CustomersList = rootObject.Customers;
                            if (customers.CustomersList.Count > 0)
                            {
                                customerDetailsMewsList = ConvertRoomInquiryResponse(customers.CustomersList);
                            }
                        }
                    }
                }
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return customerDetailsMewsList;
        }

        public SpaceDetails GetSpaceIds(string platformAddress, BasicInquiry basicInquiry, List<string> logsList)
        {
            basicInquiry.ClientToken = MewsRequestAddress.ClientToken;
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.SpaceIds;
            logsList.Add("**************Request to get Spaces from Mews at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            HttpWebResponse webResponse = null;
            SpaceDetails spaceDetails = new SpaceDetails
            {
                Spaces = new List<Space>(),
            };
            try
            {
                var request = GetWebRequest(url, basicInquiry, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Response is                                        " + retValue);
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);

                if (rootObject.Spaces != null)
                    spaceDetails.Spaces = rootObject.Spaces;
            }
            catch (WebException we)
            {
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return spaceDetails;
        }

        public string PostOrder(string platformAddress, Order order, List<string> logsList)
        {
            order.ClientToken = MewsRequestAddress.ClientToken;
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.AddOrder;
            logsList.Add("**************Request to PostOrder at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            order.Notes = order.ConsumptionUtc;
            order.ConsumptionUtc = DateTime.UtcNow.ToString("s") + "Z";
            HttpWebResponse webResponse = null;
            try
            {
                var request = GetWebRequest(url, order, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                logsList.Add("Response is                                        " + retValue);
            }
            catch (WebException we)
            {
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Web Exception Message is                           " + we.InnerException.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                retValue = we.InnerException.Message;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                retValue = ex.Message;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            if (webResponse != null)
            {
                if (webResponse.StatusCode == HttpStatusCode.OK)
                {
                    retValue = "Successful";
                    logsList.Add("Status is                                          " + "Successful");
                }
                else
                    logsList.Add("Status is                                          " + "UnSuccessful");
            }
            else
                logsList.Add("Status is                                          " + "UnSuccessful");
            return retValue;
        }

        public string PostBill(string platformAddress, Order order, List<string> logsList)
        {
            order.ClientToken = MewsRequestAddress.ClientToken;
            string responseString = "";
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.AddBill;
            string dateTimeUTC = DateTime.UtcNow.ToString("s") + "Z";

            foreach (var bill in order.Bills)
            {
                bill.ClosedUtc = dateTimeUTC;
                foreach (var item in bill.Items)
                {
                    item.ConsumedUtc = dateTimeUTC;
                    item.AccountingCategory = new AccountingCategory();
                    item.AccountingCategory.IsActive = true;
                    item.AccountingCategory.Name = item.Category.Name;
                }
            }
            logsList.Add("**************Request to PostBill at " + DateTime.Now.ToString("hh:mm:ss tt") + "**************");
            HttpWebResponse webResponse = null;

            try
            {
                var request = GetWebRequest(url, order, logsList);
                logsList.Add("Request Created ");
                webResponse = (HttpWebResponse)request.GetResponse();
                logsList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                responseString = servicesStream.ReadToEnd();
                logsList.Add("Response is                                        " + responseString);
            }
            catch (WebException we)
            {
                logsList.Add("Web Exception is                                   " + we.Message);
                logsList.Add("Web Exception Message is                           " + we.InnerException.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                retValue = we.InnerException.Message;
            }
            catch (Exception ex)
            {
                logsList.Add("Exception is                                       " + ex.Message);
                logsList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                retValue = ex.Message;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            if (webResponse != null)
            {
                if (webResponse.StatusCode == HttpStatusCode.OK)
                {
                    retValue = "Successful";
                    logsList.Add("Status is                                          " + "Successful");
                }
                else
                    logsList.Add("Status is                                          " + "UnSuccessful");
            }
            else
                logsList.Add("Status is                                          " + "UnSuccessful");
            return retValue;
        }
        private HttpWebRequest GetWebRequest<T>(string url, T obj, List<string> logsList)
        {
            var request = (HttpWebRequest)WebRequest.Create(new Uri(url));
            logsList.Add("Creating Web Request");
            var jsonString = JsonUtility.Serialize(obj);
            string jsonStringLogs = jsonString;
            JObject jObj = JObject.Parse(jsonStringLogs);
            if (jsonStringLogs.Contains("ClientToken"))
                jObj["ClientToken"] = "";
            if (jsonStringLogs.Contains("AccessToken"))
                jObj["AccessToken"] = "";
            jsonStringLogs = JsonConvert.SerializeObject(jObj, Formatting.None,
                         new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
            logsList.Add("Request Data is                                    " + jsonStringLogs);
            byte[] bytes = Encoding.UTF8.GetBytes(jsonString);

            request.Method = WebRequestMethods.Http.Post;
            request.ContentType = "application/json";
            request.ContentLength = bytes.Length;
            request.Timeout = 120000;
            using (var stream = request.GetRequestStream())
            {
                stream.Write(bytes, 0, bytes.Length);
                logsList.Add("Data written at                                    " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            return request;
        }
        private List<CustomerDetailsMews> ConvertRoomInquiryResponse(List<Customer> customers)
        {
            List<CustomerDetailsMews> listDetails = new List<CustomerDetailsMews>();
            try
            {
                foreach (var customer in customers)
                {
                    CustomerDetailsMews details = new CustomerDetailsMews
                    {
                        Classifications = new List<string>()
                    };
                    details.FirstName = customer.FirstName;
                    details.LastName = customer.LastName;
                    details.Id = customer.Id;
                    details.Number = customer.RoomNumber;
                    details.Phone = customer.CustomerDetails.Phone;
                    details.Title = customer.CustomerDetails.Title;
                    details.Gender = customer.CustomerDetails.Gender;
                    details.Email = customer.CustomerDetails.Email;
                    foreach (var classification in customer.CustomerDetails.Classifications)
                    {
                        details.Classifications.Add(classification);
                    }
                    listDetails.Add(details);
                }
            }
            catch (Exception ex)
            {

            }
            return listDetails;
        }
    }
}
