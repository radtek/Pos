using System;
using System.Net;

using Loyalty_mate.Types;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Utilities.JSON.Exceptions;
using Loyalty_mate.Cloud.Exceptions;

namespace Loyalty_mate.Cloud.Types.Requests
{

/// <summary>
///   Creates or refreshes an authentication token to use with future requests
///   in the cloud.
/// </summary>
public sealed class Authenticate_request : Authenticated_request {
        /// <summary>
        ///   The response that the cloud gives is. It's deserialized from JSON
        ///   and contains the data we need to use the API.
        /// </summary>
        sealed class Response_message {
                /// <summary>
                ///   Authenticates us to the cloud API.
                /// </summary>
                public string access_token {
                        get;
                        set;
                }

                /// <summary>
                ///   The type of token returned; unused.
                /// </summary>
                public string token_type {
                        get;
                        set;
                }

                /// <summary>
                ///   How long until the token expires in seconds.
                /// </summary>
                public int expires_in {
                        get;
                        set;
                }

                /// <summary>
                ///   The token to use to refresh the access token; it identifies
                ///   the session that we want to continue using.
                /// </summary>
                public string refresh_token {
                        get;
                        set;
                }

                /// <summary>
                ///   Defines what we can access; unused.
                /// </summary>
                public string scope {
                        get;
                        set;
                }
        }

        public
        Authenticate_request(string base_request_address,
                             Credentials credentials,
                             Token token = null)
            : base(base_request_address, credentials, token)
        {
                Grant_type = Cloud.Constants.Grant_type.Refresh_existing_token;
                if (Token == null)
                    Grant_type = Cloud.Constants.Grant_type.New_token;
        }

        protected override void
        On_execute()
        {
                Interpret_response(Create_request());
        }

        /// <summary>
        ///   Creates the request used to create/refresh our access token.
        /// </summary>
        /// <returns>
        ///   The request object to be used.
        /// </returns>
        HttpWebRequest
        Create_request()
        {
                Query q = new Query();
                Path p = new Path();

                q.Element("client_id", Credentials.Client_ID)
                 .Element("client_secret", Credentials.Client_secret)
                 .Element("grant_type", Grant_type);

                switch (Grant_type) {
                case "password":
                        q.Element("username", Credentials.Username)
                         .Element("password", Credentials.Password);
                        p.Subdirectory(
                            Request_addresses.Authentication.Create_token);
                        break;
                case "refresh_token":
                        q.Element("refresh_token", base.Token.Refresh_token);
                        p.Subdirectory(
                            Request_addresses.Authentication.Refresh_token);
                        break;
                default:
                        break;
                }

                HttpWebRequest request = 
                    (HttpWebRequest)WebRequest.Create(
                           URI_builder.Create(
                               Uri.UriSchemeHttps,
                               Base_request_address,
                               p,
                               q));
                request.Method = WebRequestMethods.Http.Post;
                return request;
        }

        /// <summary>
        ///   Invokes the request and interprets its response. This is
        ///   split into its own function as this interpretation and
        ///   associated error handling is not trivial.
        /// </summary>
        /// <param name="request"></param>
        void
        Interpret_response(HttpWebRequest request)
        {
                HttpWebResponse response = null;
                try {
                        response =
                            (HttpWebResponse)request.GetResponse();
                        Response_message message =
                            JSON.Deserialize_from_stream<Response_message>(
                                new System.IO.StreamReader(
                                    response.GetResponseStream()));
                        TimeSpan expiry_span = new TimeSpan(0, 0, message.expires_in);
                        DateTime expiry_date = DateTime.Now + expiry_span;
                        token_ = new Token(message.access_token,
                                           message.refresh_token,
                                           expiry_date);
                } catch (Invalid_JSON_exception ije) {
                        throw new API_request_inaccessible_exception(ije);
                }
                finally
                {
                    if (response != null)
                        response.Close();
                }
        }

        /// <summary>
        ///   The token to be used for future authenticated requests.
        /// </summary>
        public new Token Token {
                get {
                        return token_;
                }
        }

        readonly string Grant_type;

        /// <summary>
        ///   The token to be used for future authenticated requests.
        /// </summary>
        Token token_;
} /* Authenticate_request */

} /* Loyalty_mate.Cloud.Requests */
