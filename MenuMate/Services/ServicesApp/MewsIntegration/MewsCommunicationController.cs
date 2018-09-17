using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using MewsIntegration.Domain;
using MewsIntegration.Utilities;

namespace MewsIntegration
{
    public class MewsCommunicationController
    {
        public MewsCommunicationController()
        {
            System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls | SecurityProtocolType.Ssl3 | (SecurityProtocolType)3072;
        }
        public List<Service> GetMewsServices(string platformAddress, BasicInquiry basicInquiry)
        {
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.ServicesAll;
            var request = GetWebRequest(url, basicInquiry);
            HttpWebResponse webResponse = null;
            List<Service> services = new List<Service>();
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                if (rootObject.Services != null)
                    services = rootObject.Services;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return services;
        }

        public List<AccountingCategory> GetMewsAccountingCategories(string platformAddress, BasicInquiry basicInquiry)
        {
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.AccountingCategoriesAll;
            var request = GetWebRequest(url, basicInquiry);
            HttpWebResponse webResponse = null;
            List<AccountingCategory> accountingCategories = new List<AccountingCategory>();
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);                
                if (rootObject.AccountingCategories != null)
                    accountingCategories = rootObject.AccountingCategories;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return accountingCategories;
        }

        public List<Outlet> GetMewsOutlets(string platformAddress, BasicInquiry basicInquiry)
        {
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.OutletsAll;
            var request = GetWebRequest(url, basicInquiry);
            HttpWebResponse webResponse = null;
            List<Outlet> outlets = new List<Outlet>();
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                
                if (rootObject.Outlets != null)
                    outlets = rootObject.Outlets;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return outlets;
        }

        public List<Customer> GetCustomerSearchResult(string platformAddress, CustomerSearch customerSearch)
        {
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.CustomerSearch;
            var request = GetWebRequest(url, customerSearch);
            HttpWebResponse webResponse = null;
            List<Customer> customers = new List<Customer>();
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                if (rootObject.Customers != null)
                    customers = rootObject.Customers;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return customers;
        }

        public SpaceDetails GetSpaceIds(string platformAddress, BasicInquiry basicInquiry)
        {
            string retValue = "";
            string url = platformAddress + MewsRequestAddress.SpaceIds;
            var request = GetWebRequest(url, basicInquiry);
            HttpWebResponse webResponse = null;
            SpaceDetails spaceDetails = new SpaceDetails
            {
                Spaces = new List<Space>(),
                SpaceCategories = new List<SpaceCategory>()
            };
            try
            {
                webResponse = (HttpWebResponse)request.GetResponse();
                var servicesStream = new StreamReader(webResponse.GetResponseStream());
                retValue = servicesStream.ReadToEnd();
                RootObject rootObject = JsonUtility.Deserialize<RootObject>(retValue);
                
                if (rootObject.Spaces != null)
                    spaceDetails.Spaces = rootObject.Spaces;
                if (rootObject.SpaceCategories != null)
                    spaceDetails.SpaceCategories = rootObject.SpaceCategories;
            }
            catch (WebException we)
            {
                webResponse = (HttpWebResponse)we.Response;
            }
            finally
            {
                if (webResponse != null)
                {
                    webResponse.Close();
                }
            }
            return spaceDetails;
        }
        private HttpWebRequest GetWebRequest<T>(string url, T obj)
        {
            var request = (HttpWebRequest)WebRequest.Create(new Uri(url));
            try
            {
                var jsonString = JsonUtility.Serialize(obj);
                byte[] bytes = Encoding.UTF8.GetBytes(jsonString);

                request.Method = WebRequestMethods.Http.Post;
                request.ContentType = "application/json";
                request.ContentLength = bytes.Length;
                request.Timeout = 120000;
                //request.GetRequestStream().Write(bytes, 0, bytes.Length);
                using (var stream = request.GetRequestStream())
                {
                    stream.Write(bytes, 0, bytes.Length);
                }
            }
            catch (WebException exception)
            {
                int i = 0;
            }
            catch (Exception exception)
            {
                int j = 0;
            }
            return request;
        }
    }
}
