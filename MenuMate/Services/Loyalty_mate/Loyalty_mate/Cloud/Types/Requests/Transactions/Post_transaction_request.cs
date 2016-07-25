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
    ///   Posts a transaction on to the cloud.
    /// </summary>
    public sealed class Post_transaction_request : Authenticated_request
    {
        /// <summary>
        ///   Creates an instance of this request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="member">
        ///   Transaction to be posted.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        public
        Post_transaction_request(string base_request_address,
                              Transaction transaction,
                              Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            transaction_ = transaction;
        }

        override protected void
        On_execute()
        {
            HttpWebResponse wr = null;
            try
            {
                wr =
                    (HttpWebResponse)Create_request().GetResponse();
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

        HttpWebRequest
        Create_request()
        {
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        Request_addresses.Transactions.Post_transaction,
                        new Query()
                            .Element("uuid", transaction_.uuid)
                            .Element("access_token", Token.Access_token)));
            JSON.Serialize_to<Transaction>(transaction_, request, WebRequestMethods.Http.Post);
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
            switch ((int)wr.StatusCode)
            {
                //TODO: More exception types
                case 404:
                    throw new Member_doesnt_exist_exception(transaction_.uuid);
                case 422:
                    throw new Invalid_request_exception();
                case 500:
                    throw new API_down_exception();
                default:
                    throw new Unknown_API_response_exception(wr);
            }
        }

        /// <summary>
        ///   The transaction to be posted
        /// </summary>
        Transaction transaction_;

    } /* Post_transaction_request */

} /* Loyalty_mate.Cloud.Requests */
