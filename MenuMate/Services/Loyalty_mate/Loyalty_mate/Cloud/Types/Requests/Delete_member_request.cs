using System;
using System.Collections.Generic;
using System.IO;
using System.Net;

using Loyalty_mate.Cloud.Responses;
using Loyalty_mate.Types;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Cloud.Exceptions;

namespace Loyalty_mate.Cloud.Types.Requests
{

    /// <summary>
    ///   Deletes a member in the cloud.
    /// </summary>
    public sealed class Delete_member_request : Authenticated_request
    {
        /// <summary>
        ///   Creates an instance of this request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="uuid">
        ///   The UUID of the member to be deleted.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        public
        Delete_member_request(string base_request_address,
                              string uuid,
                              Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            uuid_ = uuid;
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
                                 .Subdirectory(Request_addresses.Members.Delete_member)
                                 .Subdirectory(uuid_);
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

            throw new Member_doesnt_exist_exception(uuid_);
        }

        /// <summary>
        ///   The UUID of the member to delete.
        /// </summary>
        readonly string uuid_;
    } /* Delete_member_request */

} /* Loyalty_mate.Cloud.Requests */
