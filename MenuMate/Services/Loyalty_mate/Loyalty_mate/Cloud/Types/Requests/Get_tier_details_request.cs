using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

using Loyalty_mate.Types;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Cloud.Responses;
using Loyalty_mate.Cloud.Exceptions;

namespace Loyalty_mate.Cloud.Types.Requests
{
  

    public sealed class Get_tier_details_request : Authenticated_request
    {
        string _tierId;
        Tier _tierDetails = null;

        public Tier TierDetails
        {
            get
            {
                return _tierDetails;
            }
        }

         public Get_tier_details_request(string base_request_address,
                                   string tierId,
                                   Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            _tierId = tierId;
        }

        override protected void
        On_execute()
        {
            WebResponse response = null;
            try
            {
                response = Create_request().GetResponse();
                _tierDetails =
                    JSON.Deserialize_from_stream<Tier>(
                        new StreamReader(response.GetResponseStream()));
            }
            catch (WebException ex)
            {
                Handle_errors_if_present(ex);
            }
            finally
            {
                if (response != null)
                    response.Close();
            }
        }

        HttpWebRequest
        Create_request()
        {
                string path =
                    new Utilities.URI.Path()
                        .Subdirectory(Request_addresses.Tiers.Get_tier_details)
                        .Subdirectory(_tierId);
                HttpWebRequest request =
                    (HttpWebRequest)WebRequest.Create(
                        URI_builder.Create(
                            Uri.UriSchemeHttps,
                            Base_request_address,
                            path,
                            new Query().Element("access_token",
                                                Token.Access_token))); 
                request.Method = WebRequestMethods.Http.Get;
                return request;
        }

        void
        Handle_errors_if_present(WebException exception)
        {
            HttpWebResponse response = (HttpWebResponse)exception.Response;

            if (response == null)
                throw exception;

            switch ((int)response.StatusCode)
            {
                case 404:
                    throw new Tier_doesnt_exist_exception(_tierId);
                default:
                    throw exception;
            }
        }

       
    }
}
