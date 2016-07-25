using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using Loyalty_mate.Cloud.Constants;
using Loyalty_mate.Utilities.URI;
using Loyalty_mate.Utilities.JSON;
using Loyalty_mate.Cloud.Exceptions;
using System.IO;

namespace Loyalty_mate.Cloud.Types.Requests
{
    public sealed class Get_Points_Detail_Request : Authenticated_request
    {
        PointsInfo _pointsInfo;
        PointsInfo _fetched_Points;
        public PointsInfo PointDetails
        {
            get
            {
                return _fetched_Points;
            }
        }

        public Get_Points_Detail_Request(string base_request_address, PointsInfo pointsInfo, Token token)
            : base(base_request_address, null, token)
        {
            _pointsInfo = pointsInfo;
        }

        override protected void
        On_execute()
        {
            WebResponse response = null;
            try
            {
                response =(HttpWebResponse)Create_request().GetResponse();
                var balance = JSON.Deserialize_from_stream<PointsResponse>(
                        new StreamReader(response.GetResponseStream())); 
                _fetched_Points = new PointsInfo()
                {
                    point_balance = balance != null ? balance.points : "0"
                };
               
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
                                 .Subdirectory(Request_addresses.Transactions.Get_PointsInRange)
                                 .Subdirectory(_pointsInfo.uuid).
                                 Subdirectory(_pointsInfo.start_date.Value.ToString("yyyy-MM-dd HH:mm:ss")).
                                 Subdirectory(_pointsInfo.end_date.Value.ToString("yyyy-MM-dd HH:mm:ss")).
                                 Subdirectory(Convert.ToString(_pointsInfo.points_type));
            HttpWebRequest request =
                (HttpWebRequest)WebRequest.Create(
                    URI_builder.Create(
                        Uri.UriSchemeHttps,
                        Base_request_address,
                        path,
                        new Query().Element("access_token",
                                            Token.Access_token)));
            return request;
        }



        void Handle_errors_if_present(WebException exception)
        {
            HttpWebResponse response = (HttpWebResponse)exception.Response;

            if (response == null)
                throw exception;

            switch ((int)response.StatusCode)
            {
                case 404:
                    throw new Member_doesnt_exist_exception("");
                default:
                    throw exception;
            }
        }
    }
}
