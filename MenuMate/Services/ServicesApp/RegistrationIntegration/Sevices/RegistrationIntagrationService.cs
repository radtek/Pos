using RegistrationIntegration.Sevices;
using RegistrationIntegration.ViewModels;
using System.Net;
using RegistrationIntegration.Exceptions;
using System.IO;
using RegistrationIntegration.Utility;

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