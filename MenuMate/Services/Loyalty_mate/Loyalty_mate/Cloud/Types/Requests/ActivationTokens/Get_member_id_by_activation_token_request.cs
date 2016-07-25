using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Cloud.Exceptions;
using System.IO;

namespace Loyalty_mate.Cloud.Types.Requests.ActivationTokens
{
    /// <summary>
    /// Gets member information using the activation token
    /// </summary>
    public sealed class Get_member_id_by_activation_token_request : Authenticated_request
    {
        public Get_member_id_by_activation_token_request(
                                string base_request_address,
                                string activation_token,
                                Token token)
            : base(base_request_address, null, token)
        {
            _activation_token = activation_token;
            _tokenResponse = new MemberActivationInfo();
        }

        /// <summary>
        /// override od the on execute from the base class. Creates request, invoke and returns the output
        /// </summary>
        protected override void On_execute()
        {
            HttpWebResponse wr = null;
            try
            {
                wr =
                    (HttpWebResponse)Create_request().GetResponse();
                _tokenResponse = 
                    JSON.Deserialize_from_stream<MemberActivationInfo>(
                    new StreamReader(wr.GetResponseStream()));
            }
            catch (WebException we)
            {
                wr = (HttpWebResponse)we.Response;
                if (wr == null)
                    throw we;
                Handle_errors_if_present(wr);
            }
            finally
            {
                if (wr != null)
                    wr.Close();
            }
        }

        /// <summary>
        /// creates the web request object to be used to retrieve member uuid with activation token
        /// </summary>
        /// <returns></returns>
        HttpWebRequest
        Create_request()
        {
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        Request_addresses.ActivationTokens.Get_member_id_by_token + "/" + _activation_token,
                        new Query()
                            .Element("access_token", Token.Access_token)));
            request.Method = WebRequestMethods.Http.Get;
            return request;
        }

        /// <summary>
        /// error handling according to different error codes
        /// </summary>
        /// <param name="wr"></param>
        void
        Handle_errors_if_present(HttpWebResponse wr)
        {
            switch ((int)wr.StatusCode)
            {
                case 404:
                    throw new Activation_token_not_found_exception();
                case 422:
                    throw new Invalid_request_exception();
                case 500:
                    throw new API_down_exception();
                default:
                    throw new Unknown_API_response_exception(wr);
            }
        }

        string _activation_token;
        private MemberActivationInfo _tokenResponse;


        public MemberActivationInfo TokenResponse
        {
            get { return _tokenResponse; }
            internal set { _tokenResponse = value; } // todo: remove this. added as part of mock token structure
        }
    }
}
