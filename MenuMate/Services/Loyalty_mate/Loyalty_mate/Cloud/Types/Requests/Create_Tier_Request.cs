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

namespace Loyalty_mate.Cloud.Types.Requests
{
    public sealed class Create_Tier_Request : Authenticated_request
    {

        Tier _tier;
        Tier _created_tier = null;
        public Tier CreatedTier
        {
            get
            {
                return _created_tier;
            }
        }


        public Create_Tier_Request(string base_request_address, Tier tier, Token token)
            : base(base_request_address, null, token)
        {
            _tier = tier;
        }

        override protected void On_execute()
        {
            HttpWebResponse wr = null;
            try
            {
                wr = (HttpWebResponse)Create_request().GetResponse();
                _created_tier = JSON.Deserialize_from_stream<Tier>(new StreamReader(wr.GetResponseStream()));
            }
            catch (WebException we)
            {
                wr = (HttpWebResponse)we.Response;
                if (wr == null
                    || (int)wr.StatusCode != 422)
                    throw we;
                Handle_errors_if_present(wr);
            }
            finally
            {
                if (wr != null)
                    wr.Close();
            }
        }

        HttpWebRequest
        Create_request()
        {
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        Request_addresses.Tiers.Create_tier,
                        new Query().Element("access_token",
                                            Token.Access_token)));
            JSON.Serialize_to<Tier>(_tier, request, WebRequestMethods.Http.Post);
            return request;
        }

        /// <summary>
        ///   There are several casees that we have to check for when determining whether
        ///   or not the member was created succesfully in the cloud. This function checks
        ///   for them all and throws the relevant exceptions if neccessary.
        /// </summary>
        /// <param name="wr">
        ///   The response to check.
        /// </param>
        void
        Handle_errors_if_present(HttpWebResponse wr)
        {
            string encoded = new StreamReader(wr.GetResponseStream()).ReadToEnd();
            Error error;
            List<string> error_messages;

            // Otherwise, it'll reply with the usual error response. If not then we have a
            // serious issue.
            if (!JSON.Try_and_deserialize<Error>(encoded, out error))
                throw new Unknown_API_response_exception(wr);

            // Search the error for any messages relating to accounts.
            error_messages =
                error.Get_all_error_messages_for_matching_error_types("account.*");

            // Did a member already exist with that email?
            if (error_messages.Count > 0
                && error_messages.Contains("has already been taken"))
                throw new Tier_already_exists_exception(_tier);

            // Otherwise we assume it's some issue with member details.
            throw new Invalid_tier_details_exception(_tier);
        }


    }
}
