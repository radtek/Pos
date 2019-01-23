using RegistrationIntegration.Sevices;
using RegistrationIntegration.ViewModels;
using System.Net;
using RegistrationIntegration.Exceptions;
using System.IO;
using RegistrationIntegration.Utility;
using System;

namespace RegistrationIntegration.Sevices
{
    public class RegistrationIntagrationService : IRegistrationIntegrationService
    {
        public bool PostTerminalRegistrationInfo(string inSyndicateCode, ApiTerminalViewModel terminalViewModel)
        {
            bool response = false;
            var request = Utility.WebUtility.CreateRequest(RequestAddress.PosTerminalDetails, inSyndicateCode, null, WebRequestMethods.Http.Post, terminalViewModel);
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

        public ApiRegistrationResponse ValidateCompanyInfo(string inSyndicateCode, int siteCode)
        {
            ApiRegistrationResponse response = null;
            string requestAddress = RequestAddress.ValidateCompanyInfo + @"/" + siteCode.ToString();
            var request = Utility.WebUtility.CreateRequest(requestAddress, inSyndicateCode, null,
                WebRequestMethods.Http.Get);
            HttpWebResponse webResponse = null;
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                response = CreateRegistrationIntegrationResponse(webResponse, webResponse.StatusDescription);
            }
            catch (WebException we)
            {
                response = CreateRegistrationIntegrationResponse(webResponse, we.Message);
            }
            catch (Exception ex)
            {
                response = CreateRegistrationIntegrationResponse(webResponse, ex.Message);
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

        private ApiRegistrationResponse CreateRegistrationIntegrationResponse(HttpWebResponse webResponse, string message)
        {
            ApiRegistrationResponse apiRegistrationResponse = new ApiRegistrationResponse();
            if (webResponse == null)
            {
                apiRegistrationResponse.IsSuccessful = false;
                apiRegistrationResponse.Message = "Could not communicate with web.";
                return apiRegistrationResponse;
            }
            if (webResponse.StatusCode == HttpStatusCode.OK)
            {
                if (webResponse.ResponseUri != null)
                {
                    string localPath = webResponse.ResponseUri.LocalPath;
                    if (localPath.Contains("Index") && localPath.Contains("Login"))
                        apiRegistrationResponse.IsSuccessful = false;
                    else
                    {
                        apiRegistrationResponse.IsSuccessful = true;
                        apiRegistrationResponse.Message = "";
                    }
                }
            }
            if (apiRegistrationResponse.IsSuccessful)
                apiRegistrationResponse.Message = "";
            else
            {
                if (message.Contains("(406)"))
                    apiRegistrationResponse.Message =
                        "Registration is Disabled for the site in web.\nPlease Enable it first.";
                else
                    apiRegistrationResponse.Message =
                        "Unsuccessful sync for registration integration.\nPlease check if syndicatecode and site id are correct.";
            }
            return apiRegistrationResponse;
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