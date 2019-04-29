using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using OnlineOrdering.Model;
using OnlineOrdering.Model.MenuModels;
using OnlineOrdering.Model.TaxSettingsModels;
using OnlineOrdering.Model.OrderModels;
using OnlineOrdering.Utility;
using OnlineOrdering.Exceptions;

namespace OnlineOrdering.Services
{
    public class OnlineOrderingService : IOnlineOrderingService
    {
        public bool SyncSiteMenu(string inSyndicateCode, ApiSiteMenuViewModel siteMenuViewModel, List<string> stringList)
        {
            bool response = false;
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SyncSiteMenu, inSyndicateCode, null, WebRequestMethods.Http.Post, siteMenuViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                string requestStr = JsonUtility.Serialize(siteMenuViewModel);
                stringList.Add("Request is                                         " + requestStr);
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                HandleExceptions(webResponse);
                return false;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                    response = true;
                }
            }
            return response;
        }
        
        public bool SyncSiteTaxSettings(string inSyndicateCode, ApiSiteTaxSettings siteTaxSettings, List<string> stringList)
        {
            bool response = false;
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SyncSiteTaxSettings, inSyndicateCode, null, WebRequestMethods.Http.Post, siteTaxSettings);
            HttpWebResponse webResponse = null;
            try
            {
                string requestStr = JsonUtility.Serialize(siteTaxSettings);
                stringList.Add("Request is                                         " + requestStr);
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                HandleExceptions(webResponse);
                return false;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                    response = true;
                }
            }
            return response;
        }
        
        public bool UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel, List<string> stringList)
        {
            bool response = false;
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.UpdateOrderStatus, inSyndicateCode, null, WebRequestMethods.Http.Post, siteOrderViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                string requestStr = JsonUtility.Serialize(siteOrderViewModel);
                stringList.Add("Request is                                         " + requestStr);
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                HandleExceptions(webResponse);
                return false;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                    response = true;
                }
            }
            return response;
        }
        
        public bool PostOnlineOrderInvoiceInfo(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel, List<string> stringList)
        {
            bool response = false;
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(RequestAddress.PostOnlineOrderInvoiceInfo, inSyndicateCode, null, WebRequestMethods.Http.Post, siteOrderViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                string requestStr = JsonUtility.Serialize(siteOrderViewModel);
                stringList.Add("Request is                                         " + requestStr);
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                HandleExceptions(webResponse);
                return false;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                    response = true;
                }
            }
            return response;
        }
        
        public ApiOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            ApiOnlineOrderingResponse response = null;
            string requestAddress = RequestAddress.IsOnlineOrderingEnableBySiteId + @"/" + siteCode.ToString();
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(requestAddress, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                response = CreateOnlineOrderingResponse(webResponse, webResponse.StatusDescription, stringList);
                stringList.Add("CreateOnlineOrderingResponse at                    " + DateTime.Now.ToString("hh:mm:ss tt"));

            }
            catch (WebException we)
            {
                response = CreateOnlineOrderingResponse(webResponse, we.Message, stringList);
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                response = CreateOnlineOrderingResponse(webResponse, ex.Message, stringList);
                stringList.Add("Exception                                          " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }
        
        public OnlineOrderingDetailsResponse GetOnlineOrderingDetails(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            OnlineOrderingDetailsResponse response = null;
            string requestAddress = RequestAddress.GetOnlineOrderingDetailsBySiteCode + @"/" + siteCode.ToString();
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(requestAddress, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                response = CreateOnlineOrderingDetailsResponse(webResponse, webResponse.StatusDescription, stringList);
                stringList.Add("CreateOnlineOrderingResponse at                    " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (WebException we)
            {
                response = CreateOnlineOrderingDetailsResponse(webResponse, we.Message, stringList);
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                response = CreateOnlineOrderingDetailsResponse(webResponse, ex.Message, stringList);
                stringList.Add("Exception                                          " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return response;
        }
        
        private ApiOnlineOrderingResponse CreateOnlineOrderingResponse(HttpWebResponse webResponse, string message, List<string> stringList)
        {
            ApiOnlineOrderingResponse apiOnlineOrderingResponse = new ApiOnlineOrderingResponse();
            if (webResponse == null)
            {
                apiOnlineOrderingResponse.IsSuccessful = false;
                apiOnlineOrderingResponse.Message = "Could not communicate with online ordering";
                return apiOnlineOrderingResponse;
            }
            if (webResponse.StatusCode == HttpStatusCode.OK)
            {
                if (webResponse.ResponseUri != null)
                {
                    string localPath = webResponse.ResponseUri.LocalPath;
                    if (localPath.Contains("Index") && localPath.Contains("Login"))
                        apiOnlineOrderingResponse.IsSuccessful = false;
                    else
                    {
                        apiOnlineOrderingResponse.IsSuccessful = true;
                        apiOnlineOrderingResponse.Message = "";
                    }
                }
            }
            if (apiOnlineOrderingResponse.IsSuccessful)
                apiOnlineOrderingResponse.Message = "";
            else
            {
                if (message.Contains("(406)"))
                    apiOnlineOrderingResponse.Message =
                        "Online Ordering is Disabled for the site in LoyaltyMate.\nPlease Enable it first in LoyaltyMate.";
                else
                    apiOnlineOrderingResponse.Message =
                        "Unsuccessful sync for online ordering.\nPlease check if syndicatecode and site id are correct.";
            }
            return apiOnlineOrderingResponse;
        }
        
        private OnlineOrderingDetailsResponse CreateOnlineOrderingDetailsResponse(HttpWebResponse webResponse, string message, List<string> stringList)
        {
            OnlineOrderingDetailsResponse onlineOrderingDetailsResponse = new OnlineOrderingDetailsResponse();
            if (webResponse == null)
            {
                onlineOrderingDetailsResponse.IsSuccessful = false;
                onlineOrderingDetailsResponse.Message = "Could not communicate with online ordering";
                return onlineOrderingDetailsResponse;
            }
            if (webResponse.StatusCode == HttpStatusCode.OK)
            {
                if (webResponse.ResponseUri != null)
                {
                    string localPath = webResponse.ResponseUri.LocalPath;
                    if (localPath.Contains("Index") && localPath.Contains("Login"))
                        onlineOrderingDetailsResponse.IsSuccessful = false;
                    else
                    {
                        onlineOrderingDetailsResponse.IsSuccessful = true;
                        onlineOrderingDetailsResponse.Message = "";
                        var orderStream = new StreamReader(webResponse.GetResponseStream());
                        string responseStr = orderStream.ReadToEnd();
                        onlineOrderingDetailsResponse = JsonUtility.Deserialize<OnlineOrderingDetailsResponse>(responseStr);
                    }
                }
            }
            if (onlineOrderingDetailsResponse.IsSuccessful)
                onlineOrderingDetailsResponse.Message = "";
            else
            {
                if (message.Contains("(406)"))
                    onlineOrderingDetailsResponse.Message =
                        "Online Ordering is Disabled for the site in LoyaltyMate.\nPlease Enable it first in LoyaltyMate.";
                else
                    onlineOrderingDetailsResponse.Message =
                        "Unsuccessful sync for online ordering.\nPlease check if syndicatecode and site id are correct.";
            }
            return onlineOrderingDetailsResponse;
        }

        public bool UnsetOrderingDetails(string inSyndicateCode, int siteCode, List<string> stringList)
        {
            bool isSuccessful = false;
            string requestAddress = RequestAddress.UnsetOrderingDetails + @"/" + siteCode.ToString();
            stringList.Add("Creating Web Request                               " + DateTime.Now.ToString("hh:mm:ss tt"));
            var request = Utility.WebUtility.CreateRequest(requestAddress, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                stringList.Add("Web response Status Code is                        " + webResponse.StatusCode.ToString());
                stringList.Add("Response at                                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                if (webResponse.StatusCode == HttpStatusCode.OK)
                    isSuccessful = true;
                else
                    isSuccessful = false;
                
            }
            catch (WebException we)
            {
                stringList.Add("WebException                                       " + we.Message);
                stringList.Add("WebException Status                                " + we.Status.ToString());
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            catch (Exception ex)
            {
                stringList.Add("Exception                                          " + ex.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return isSuccessful;
        }
        
        private void HandleExceptions(HttpWebResponse webResponse)
        {
            if (webResponse != null)
            {
                if ((int)webResponse.StatusCode == 401)
                {
                    throw new AuthenticationFailedException();
                }
                else if ((int)webResponse.StatusCode == 300)
                {
                    throw new MultipleGUIDException();
                }
                else if ((int)webResponse.StatusCode == 404)
                {
                    throw new GUIDNotFoundException();
                }
                else
                {
                    var memberStream = new StreamReader(webResponse.GetResponseStream());
                    string message = memberStream.ReadToEnd();
                    var customException = JsonUtility.Deserialize<CustomException>(message);
                    throw new LoyaltymateOperationException(customException.ExceptionMessage);
                }
            }
            else
            {
                throw new LoyaltymateOperationException("Not able to connect with server.");
            }
        }
    }
}
