using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using fiskaltrust.ifPOS.v0;
using System.ServiceModel;
using System.Net;
using Newtonsoft.Json;

namespace AustriaFiscalIntegration.AustriaFiscalApplication
{
    public class AustriaFiscalAppManager
    {
        private static volatile AustriaFiscalAppManager _instance;
        private static readonly object syncRoot = new Object();
        public IPOS proxy;
        private AustriaFiscalAppManager()
        {
        }

        public static AustriaFiscalAppManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new AustriaFiscalAppManager();
                    }
                }
                return _instance;
            }
        }
        // Method for Soap implementation
        /**/
        #region SoapImplemenation
        public string Init(string url, string cashBoxId, List<string> logsList)
        {
            string responseCode = "";
            logsList.Add("Url is:-                                  " + url);
            logsList.Add("Cash Box Id is:-                          " + cashBoxId);
            try
            {
                Guid _tempCashBoxID;
                if (!Guid.TryParse(cashBoxId, out _tempCashBoxID))
                {
                    responseCode = "Cashboxid is not a guid!";
                    logsList.Add("**************Cashboxid is not a guid!*****************");
                    return responseCode;
                }

                System.ServiceModel.Channels.Binding binding = null;

                if (url.StartsWith("http://"))
                {
                    var b = new BasicHttpBinding(BasicHttpSecurityMode.None);
                    b.MaxReceivedMessageSize = 16 * 1024 * 1024;
                    binding = b;
                }
                else if (url.StartsWith("https://"))
                {
                    var b = new BasicHttpBinding(BasicHttpSecurityMode.Transport);
                    b.MaxReceivedMessageSize = 16 * 1024 * 1024;
                    binding = b;
                }
                else if (url.StartsWith("net.pipe://"))
                {
                    var b = new NetNamedPipeBinding(NetNamedPipeSecurityMode.None);
                    b.MaxReceivedMessageSize = 16 * 1024 * 1024;
                    binding = b;
                }
                else if (url.StartsWith("net.tcp://"))
                {
                    var b = new NetTcpBinding(SecurityMode.None);
                    b.MaxReceivedMessageSize = 16 * 1024 * 1024;
                    binding = b;
                }
                var endpoint = new EndpointAddress(url);
                logsList.Add("EndPoint is created                     ");
                var factory = new ChannelFactory<IPOS>(binding, endpoint);
                logsList.Add("Factory created                           " );
                proxy = factory.CreateChannel();
                logsList.Add("Channel created                          " );
                // use echo for communication test
                logsList.Add("Request for Echo is:-                          " + cashBoxId);
                var message = proxy.Echo("message");
                if (message != "message")
                    responseCode = "Echo failed for details provided.";
                else
                    responseCode = "Echo is Successful for details provided.";
                logsList.Add("Response for Echo is:-                    " + message);
            }
            catch (Exception ex)
            {
                responseCode = ex.Message;
                logsList.Add("Exception Occured is:-                    " + ex.Message);
                logsList.Add("Exception Occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            return responseCode;
        }
        #endregion

        #region RestImplemenation
        public string Init(string url, string cashBoxId, string accessToken, List<string> logsList)
        {
            ServicePointManager.DefaultConnectionLimit = 65535;
            string responseCode = "";
            logsList.Add("Url is:-                                  " + url);
            logsList.Add("Cash Box Id is:-                          " + cashBoxId);
            logsList.Add("Access Token is:-                         " + accessToken);

            try
            {
                if (!url.EndsWith("/"))
                    url += "/";                
                Guid _tempCashBoxID = Guid.Empty;
                
                if (!Guid.TryParse(cashBoxId, out _tempCashBoxID))
                {
                    responseCode = "Cashboxid is not a guid!";
                    logsList.Add("**************Cashboxid is not a guid!*****************");
                    return responseCode;
                }
                string message = "Hi Austria Fiscal";
                responseCode = SendEcho(url, _tempCashBoxID, accessToken, message, logsList);
                logsList.Add("Response for Echo is:-                    " + message);
            }
            catch (Exception ex)
            {
                responseCode = ex.Message;
                logsList.Add("Exception Occured is:-                    " + ex.Message);
                logsList.Add("Exception Occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            return responseCode;
        }
        private string SendEcho(string url, Guid cashboxid, string accesstoken, string message, List<string> logsList)
        {
            string echoResponse = "";
            try
            {
                var webreq = (HttpWebRequest)HttpWebRequest.Create(url + "json/echo");
                webreq.Method = "POST";
                webreq.ContentType = "application/json;charset=utf-8";

                webreq.Headers.Add("cashboxid", cashboxid.ToString());
                webreq.Headers.Add("accesstoken", accesstoken);

                byte[] reqecho = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(message));
                webreq.ContentLength = reqecho.Length;
                using (var reqStream = webreq.GetRequestStream())
                {
                    reqStream.Write(reqecho, 0, reqecho.Length);
                }

                var webresp = (HttpWebResponse)webreq.GetResponse();
                if (webresp.StatusCode == HttpStatusCode.OK)
                {
                    using (var respReader = new System.IO.StreamReader(webresp.GetResponseStream(), Encoding.UTF8))
                    {
                        var json = respReader.ReadToEnd();
                        echoResponse = JsonConvert.DeserializeObject<string>(json);
                        if (echoResponse != null && echoResponse.Trim() == message.Trim())
                            echoResponse = "Echo is Successful for details provided.";
                        else
                            echoResponse = "Echo failed for details provided.";
                    }
                }
                else
                {
                    echoResponse = webresp.StatusCode +" "+ webresp.StatusDescription;
                }
            }
            catch (Exception ex)
            {
                echoResponse = ex.Message;
                logsList.Add("Exception Occured is:-                    " + ex.Message);
                logsList.Add("Exception Occured at:-                    " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            return echoResponse;
        }
        #endregion
    }
}
