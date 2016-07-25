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

    /// <summary>
    ///   Creates a member in the cloud.
    /// </summary>
    public sealed class Create_member_request : Authenticated_request
    {
        /// <summary>
        ///   Creates an instance of this request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="member">
        ///   The member to be created.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        public
        Create_member_request(string base_request_address,
                              Member member,
                              Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            member_ = member;
        }

        override protected void
        On_execute()
        {
            HttpWebResponse wr = null;
            try
            {
                wr =
                    (HttpWebResponse)Create_request().GetResponse();
                created_member_ =
                    JSON.Deserialize_from_stream<Member>(
                        new StreamReader(wr.GetResponseStream()));
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
                        Request_addresses.Members.Create_member,
                        new Query().Element("access_token",
                                            Token.Access_token)));
            JSON.Serialize_to<Member>(member_, request, WebRequestMethods.Http.Post);
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
                throw new Member_already_exists_exception(member_);

            // Otherwise we assume it's some issue with member details.
            throw new Invalid_member_details_exception(member_);
        }

        /// <summary>
        ///   The member details returned by the cloud. I.e. what it actually created.
        /// </summary>
        public Member Member
        {
            get
            {
                return created_member_;
            }
        }

        /// <summary>
        ///   The member to create.
        /// </summary>
        Member member_;

        /// <summary>
        ///   The member definition returned by the cloud. I.e. what it actually created.
        /// </summary>
        Member created_member_ = null;
    } /* Create_member_request */

} /* Loyalty_mate.Cloud.Requests */
