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

    /// <summary>
    ///   Updates the details of a member in the cloud.
    /// </summary>
    public sealed class Update_member_request : Authenticated_request
    {
        /// <summary>
        ///   Creates an instance of this request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="member">
        ///   The updated member details.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        public
        Update_member_request(string base_request_address,
                              Member member,
                              Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            details_ = member;
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
                    .Subdirectory(Request_addresses.Members.Update_member_details)
                    .Subdirectory(details_.uuid);
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        path,
                        new Query().Element("access_token",
                                            Token.Access_token)));

            MemberProfileConverter profileConverter = new MemberProfileConverter();
            JSON.Register_Converters(new[] { profileConverter });
            JSON.Serialize_to<Member>(details_, request, WebRequestMethods.Http.Put);
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
                    throw new Member_doesnt_exist_exception(details_.uuid);
                case 422:
                    throw new Invalid_member_details_exception(details_);
                default:
                    throw exception;
            }
        }

        /// <summary>
        ///   The updated member details.
        /// </summary>
        Member details_;
    } /* Update_member_request */

} /* Loyalty_mate.Cloud.Requests */
