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

namespace Loyalty_mate.Cloud.Types.Requests
{

/// <summary>
///   Gets the details of every member currently stored in the cloud.
/// </summary>
public sealed class Get_all_member_details_request : Authenticated_request {
        /// <summary>
        ///   Creates an instance of this request.
        /// </summary>
        /// <param name="base_request_address">
        ///   The base address to be used when making the request. I.e. the request
        ///   address without query components or fragments appended.
        /// </param>
        /// <param name="token">
        ///   An authentication token to be used during the request. This alongside
        ///   the basic credentials grants access to the API.
        /// </param>
        public 
        Get_all_member_details_request(string base_request_address,
                                       Token token)
            : base(base_request_address,
                   null,
                   token)
        {
        }

        override protected void
        On_execute()
        {
                WebResponse response = null;
                try
                {
                    response = Create_request().GetResponse();
                    List<Member> all_members =
                        JSON.Deserialize_from_stream<List<Member>>(
                            new StreamReader(response.GetResponseStream()));
                    all_member_details_ = new Dictionary<string, Member>();
                    foreach (Member m in all_members)
                        all_member_details_.Add(m.uuid, m);
                }
                catch
                {
                    throw;
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
                HttpWebRequest request =
                    (HttpWebRequest)WebRequest.Create(
                        URI_builder.Create(
                            Uri.UriSchemeHttps,
                            Base_request_address,
                            Request_addresses.Members.Get_all_member_details,
                            new Query().Element("access_token",
                                                Token.Access_token)));
                request.Method = WebRequestMethods.Http.Get;
                return request;
        }

        /// <summary>
        ///   Null if the request failed otherwise it stores the details of
        ///   every member in the cloud.
        /// </summary>
        public Dictionary<string, Member> All_member_details {
                get {
                        return all_member_details_;
                }
        }

        /// <summary>
        ///   Null if the request failed otherwise it stores the details of
        ///   every member in the cloud.
        /// </summary>
        Dictionary<string, Member> all_member_details_ = null;
} /* Get_all_member_details_request */

} /* Loyalty_mate.Cloud.Requests */
