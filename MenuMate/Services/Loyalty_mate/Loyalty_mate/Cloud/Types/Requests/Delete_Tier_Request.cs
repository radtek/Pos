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
    public sealed class Delete_Tier_Request : Authenticated_request
    {
        Tier _tier;
        public
        Delete_Tier_Request(string base_request_address,
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
                                 .Subdirectory(Request_addresses.Tiers.Delete_tier)
                                 .Subdirectory(_tier.tierid);
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        path,
                        new Query().Element("access_token",
                                            Token.Access_token)));
            request.Method = "DELETE";
            return request;
        }

        void
        Handle_errors_if_present(WebException exception)
        {
            HttpWebResponse response = (HttpWebResponse)exception.Response;
            string json = new StreamReader(response.GetResponseStream()).ReadToEnd();
            Error errors;

            if (response == null
                || response.StatusCode != HttpStatusCode.NotFound
                || (JSON.Try_and_deserialize<Error>(json, out errors) == false)
                || errors.error != "Not found")
                throw exception;

            throw new Tier_doesnt_exist_exception(_tier.tierid);
        }

    }
}
