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
        public bool SyncSiteMenu(string inSyndicateCode, ApiSiteMenuViewModel siteMenuViewModel)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SyncSiteMenu, inSyndicateCode, null, WebRequestMethods.Http.Post, siteMenuViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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
        public bool SyncSiteTaxSettings(string inSyndicateCode, ApiSiteTaxSettings siteTaxSettings)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.SyncSiteTaxSettings, inSyndicateCode, null, WebRequestMethods.Http.Post, siteTaxSettings);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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
        public bool UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.UpdateOrderStatus, inSyndicateCode, null, WebRequestMethods.Http.Post, siteOrderViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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
        public bool PostOnlineOrderInvoiceInfo(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.PostOnlineOrderInvoiceInfo, inSyndicateCode, null, WebRequestMethods.Http.Post, siteOrderViewModel);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
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
        public ApiOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int siteCode)
        {
            ApiOnlineOrderingResponse response = null;
            string requestAddress = RequestAddress.IsOnlineOrderingEnableBySiteId + @"/" + siteCode.ToString();
            var request = Utility.WebUtility.CreateRequest(requestAddress, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                response = CreateOnlineOrderingResponse(webResponse, webResponse.StatusDescription);
            }
            catch (WebException we)
            {
                response = CreateOnlineOrderingResponse(webResponse, we.Message);
            }
            catch (Exception ex)
            {
                response = CreateOnlineOrderingResponse(webResponse, ex.Message);
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
        private ApiOnlineOrderingResponse CreateOnlineOrderingResponse(HttpWebResponse webResponse, string message)
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

        public bool UnsetOrderingDetails(string inSyndicateCode, int siteCode)
        {
            bool isSuccessful = false;
            string requestAddress = RequestAddress.UnsetOrderingDetails + @"/" + siteCode.ToString();
            var request = Utility.WebUtility.CreateRequest(requestAddress, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                if (webResponse.StatusCode == HttpStatusCode.OK)
                    isSuccessful = true;
                else
                    isSuccessful = false;
            }
            catch (WebException we)
            {

            }
            catch (Exception ex)
            {

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
