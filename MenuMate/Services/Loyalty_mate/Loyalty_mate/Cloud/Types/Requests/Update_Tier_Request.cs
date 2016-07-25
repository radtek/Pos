using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

using Loyalty_mate.Cloud.Responses;
using Loyalty_mate.Types;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Cloud.Exceptions;
using Loyalty_mate.Utilities.JSON.Converters;

namespace Loyalty_mate.Cloud.Types.Requests
{
    public sealed class Update_Tier_Request : Authenticated_request
    {
        Tier _tier;
        public Update_Tier_Request(string base_request_address,
                              Tier tier,
                              Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            _tier = tier;
        }

        override protected void
        On_execute()
        {
            WebResponse response = null;
            try
            {
                response = Create_request().GetResponse();
            }
            catch (WebException we)
            {
                Handle_errors_if_present(we);
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
                    .Subdirectory(Request_addresses.Tiers.Update_tier_details)
                    .Subdirectory(_tier.tierid);
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        path,
                        new Query().Element("access_token",
                                            Token.Access_token)));
            JSON.Serialize_to<Tier>(_tier, request, WebRequestMethods.Http.Put);
            return request;
        }

        /// <summary>
        ///   Translates the error returned by the cloud to an error mapped
        ///   more precisely to our program.
        /// </summary>
        /// <param name="exception">
        ///   The exception that was thrown during the request.
        /// </param>
        void
        Handle_errors_if_present(WebException exception)
        {
            Error error;
            HttpWebResponse response = (HttpWebResponse)exception.Response;

            if (response == null)
                throw exception;

            switch ((int)response.StatusCode)
            {
                case 404:
                    if ((JSON.Try_and_deserialize_from_stream<Error>(
                           new StreamReader(response.GetResponseStream()),
                           out error)
                         == false)
                        || error.error != "Not found")
                        throw exception;
                    throw new Tier_doesnt_exist_exception(_tier.tierid);
                case 422:
                    throw new Invalid_tier_details_exception(_tier);
                default:
                    throw exception;
            }
        }



    }
}
