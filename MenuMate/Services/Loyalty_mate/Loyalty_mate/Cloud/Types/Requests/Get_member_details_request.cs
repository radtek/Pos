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

/// <summary>
///   Gets the details of a particular member.
/// </summary>
public sealed class Get_member_details_request : Authenticated_request {
        /// <summary>
        ///   Creates an instance of this request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="uuid">
        ///   The UUID identifying a particular member.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        public 
        Get_member_details_request(string base_request_address,
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
                details_ =
                    JSON.Deserialize_from_stream<Member>(
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
                        .Subdirectory(Request_addresses.Members.Get_member_details)
                        .Subdirectory(uuid_);
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

        /// <summary>
        ///   The details of the member. Null if the request failed.
        /// </summary>
        public Member Details {
                get {
                        return details_;
                }
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
            HttpWebResponse response = (HttpWebResponse)exception.Response;

            if (response == null)
                throw exception;

            switch ((int)response.StatusCode)
            {
                case 404:
                    throw new Member_doesnt_exist_exception(uuid_);
                default:
                    throw exception;
            }
        }

        /// <summary>
        ///   The details of the member. Null if the request failed.
        /// </summary>
        Member details_ = null;

        /// <summary>
        ///   The UUID that identifies the member.
        /// </summary>
        string uuid_;
} /* Get_member_details_request */

} /* Loyalty_mate.Cloud.Requests */
