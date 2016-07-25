using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Cloud.Exceptions;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Utilities.URI;

namespace Loyalty_mate.Cloud.Types.Requests
{
    public sealed class Get_member_by_code : Authenticated_request
    {
        Member _memberDetail = null;
        string _memberCode;
        private string _siteId;
        public Member Details
        {
            get
            {
                return _memberDetail;
            }
        }

        public
        Get_member_by_code(string base_request_address,
                                   string memberCode,
                                   string siteId,
                                   Token token)
            : base(base_request_address,
                   null,
                   token)
        {
            _memberCode = memberCode;
            _siteId = siteId;
        }

        override protected void
        On_execute()
        {
            WebResponse response = null;
            try
            {
                response = Create_request().GetResponse();
                _memberDetail =
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
                    .Subdirectory(_memberCode).Subdirectory(_siteId);
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

        void
        Handle_errors_if_present(WebException exception)
        {
            HttpWebResponse response = (HttpWebResponse)exception.Response;

            if (response == null)
                throw exception;

            switch ((int)response.StatusCode)
            {
                case 404:
                    throw new Member_doesnt_exist_exception(_memberCode);
                default:
                    throw exception;
            }
        }

    }
}
