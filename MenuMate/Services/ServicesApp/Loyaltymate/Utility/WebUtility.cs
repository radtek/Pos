using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using Loyaltymate.Sevices;
using System.IO;

namespace Loyaltymate.Utility
{
    public class WebUtility
    {
        public static HttpWebRequest CreateRequest<T>(string requestaddress, string syndicateCode, List<KeyValuePair<string, string>> parameters,
            string requestMode, T obj)
            where T : class
        {
            var request = WebUtility.CreateRequest(requestaddress, syndicateCode, parameters, requestMode);
            byte[] bytes = Encoding.UTF8.GetBytes(JsonUtility.Serialize(obj));
            request.ContentLength = bytes.Length;
            request.Timeout = 90000;
            request.ContentType = "text/plain";
            request.GetRequestStream().Write(bytes, 0, bytes.Length);
            return request;
        }

        public static HttpWebRequest CreateRequest(string requestaddress, string syndicateCode, List<KeyValuePair<string, string>> parameters,
            string requestMode)
        {
            //var requestUri = RequestAddress.BaseAddress + requestaddress;
            var requestUri = File.ReadAllText(@"C:\Program Files\MenuMate\Menumate Services\LoyaltyMateRequestAddress.txt") + requestaddress;
            if (parameters != null)
            {
                foreach (var keyValuePair in parameters)
                {
                    requestUri = requestUri + "/" + keyValuePair.Value;
                }
            }
            requestUri += "/";
            var request = (HttpWebRequest)WebRequest.Create(new Uri(requestUri));


            var isoEncoding = Encoding.GetEncoding("iso-8859-1");
            var byteArray = isoEncoding.GetBytes(syndicateCode.ToCharArray());
            var baseString = Convert.ToBase64String(byteArray);
            request.Headers.Add("Authorization", "Basic " + baseString);
            request.Method = requestMode;
            return request;
        }


    }


}
