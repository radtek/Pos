using RegistrationIntegration.Sevices;
using System.Net;
using RegistrationIntegration.Exceptions;
using System.IO;
using RegistrationIntegration.Utility;
using System;
using RegistrationIntegration.Models;

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

        public ApiCompanySiteViewModel ValidateCompanyInfo(string inSyndicateCode, int siteCode)
        {
            ApiCompanySiteViewModel response = null;
            string requestAddress = RequestAddress.ValidateCompanyInfo + @"/" + inSyndicateCode + @"/" + siteCode.ToString();
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

        private ApiCompanySiteViewModel CreateRegistrationIntegrationResponse(HttpWebResponse webResponse, string message)
        {
            ApiCompanySiteViewModel response = new ApiCompanySiteViewModel();
            if (webResponse == null)
            {
                response.IsSuccessful = false;
                response.Message = "Could not communicate with web.";
                return response;
            }
            if (webResponse.StatusCode == HttpStatusCode.OK)
            {
                if (webResponse.ResponseUri != null)
                {
                    string localPath = webResponse.ResponseUri.LocalPath;
                    if (localPath.Contains("Index") && localPath.Contains("Login"))
                        response.IsSuccessful = false;
                    else
                    {
                        var companyStream = new StreamReader(webResponse.GetResponseStream());
                        response = JsonUtility.Deserialize<ApiCompanySiteViewModel>(companyStream.ReadToEnd());
                        response.IsSuccessful = true;
                        response.Message = "";
                        //response.CompanyName = webResponse.
                    }
                }
            }
            else 
            {
                HandleExceptions(webResponse);
            }
            if (response.IsSuccessful)
                response.Message = "";
            else
            {
                if (message.Contains("(406)"))
                    response.Message =
                        "Registration could not be synced.\nPlease check site code.";
                else
                    response.Message =
                        "Unsuccessful sync for registration integration.\nPlease check if syndicatecode and site id are correct.";
            }
            return response;
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
                    throw new BadRequestException();
                }
                else if ((int)webResponse.StatusCode == 304)
                {
                    throw new NoSettingChangeException();
                }
                else if ((int)webResponse.StatusCode == 404)
                {
                    throw new CodeNotExistException();
                }
                else if ((int)webResponse.StatusCode == 406)
                {
                    throw new NotAccessibleException();
                }
                else
                {
                    var responseStream = new StreamReader(webResponse.GetResponseStream());
                    string message = responseStream.ReadToEnd();
                    var customException = JsonUtility.Deserialize<CustomException>(message);
                    throw new RegistrationUpdateException(customException.ExceptionMessage);
                }
            }
            else
            {
                throw new RegistrationUpdateException("Not able to connect with registration server.");//message to change
            }
        }  
    }
}