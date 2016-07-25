using System.Net;
using Loyalty_mate.Types;
using Loyalty_mate.Cloud.Exceptions;

namespace Loyalty_mate.Cloud.Types.Requests
{

/// <summary>
///   Represents a request that must have authentication information supplied.
/// </summary>
public abstract class Authenticated_request : Request {
        /// <summary>
        ///   Creates the request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="credentials">
        ///   Basic credentials of the requesting party. I.e. password, id, etc.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        protected
        Authenticated_request(string base_request_address,
                              Credentials credentials,
                              Token token = null)
            : base(base_request_address)
        {
                Credentials = credentials;
                Token = token;
        }

        public override void
        Execute()
        {
                try {
                        base.Execute();
                } catch (WebException we) {
                        HttpWebResponse wr = (HttpWebResponse)we.Response;

                        if (wr == null || wr.StatusCode != HttpStatusCode.Unauthorized)
                                throw we;
                        throw new Invalid_access_token_exception(Token);
                }
        }

        /// <summary>
        ///   Credentials of the requesting party. Think of it like therequesting
        ///   party's ID card.
        /// </summary>
        protected readonly Credentials Credentials;

        /// <summary>
        ///   The authentication to use during the request.
        /// </summary>
        protected readonly Token Token;
} /* Authenticated_request */

} /* Loyalty_mate.Cloud.Requests */
