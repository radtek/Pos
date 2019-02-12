using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Timers;
using System.Windows.Forms;
using Microsoft.AspNet.SignalR.Client;
using SystemTrayApp.OnlineOrdering;
using OnlineOrdersManager;

namespace SystemTrayApp.SignalR
{
    public static class Client
    {
        private static IHubProxy _hubProxy { get; set; }
        const string ServerURI = "http://menumatepos.com";
        public static HubConnection Connection { get; set; }
        private static NotifyIcon notifyIcon;
        private static List<string> logsList;
        private static System.Timers.Timer aTimer;

        public static bool InitializeClient()
        {
            bool retValue = false;
            logsList = new List<string>();
            string siteCode = "";
            string syndicateCode = "";
            GetSiteDetails(ref siteCode, ref syndicateCode);
            retValue = Connect(siteCode, syndicateCode);
            SetTimer();
            return retValue;
        }

        private static void SetTimer()
        {
            aTimer = new System.Timers.Timer(60000);
            aTimer.Elapsed += OnTimedEvent;
            aTimer.AutoReset = true;
            aTimer.Enabled = true;
        }

        private static void OnTimedEvent(Object source, ElapsedEventArgs arg)
        {
            string siteCode = "";
            string syndicateCode = "";
            GetSiteDetails(ref siteCode, ref syndicateCode);
            if (Connection != null)
            {
                if (Connection.State != ConnectionState.Connected)
                    ReConnect(siteCode, syndicateCode);
            }
        }

        private static void ReConnect(string siteCode, string syndicateCode)
        {
            bool isConnected = false;
            logsList.Add("SignalR is going to get reconnected        " + Connection.ConnectionId);
            try
            {
                Connection.Start().Wait();
                isConnected = Connection.State == ConnectionState.Connected;
                ContextMenuHelper helper = new ContextMenuHelper();

                if (isConnected)
                {
                    helper.ToggleOnConnect();
                    logsList.Add("SignalR client is connected with connection Id        " + Connection.ConnectionId);
                }
                else
                {
                    helper.ToggleOnDisconnect();
                    logsList.Add("SignalR client is not connected");
                }
            }
            catch (Exception exception)
            {
                isConnected = false;
                logsList.Add("Exception in making re-connection                        " + exception.Message);
                logsList.Add("Exception at                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            MakeLogFile();
        }

        private static void GetSiteDetails(ref string siteCode, ref string syndicateCode)
        {
            List<string> siteDeatils = new List<string>();
            OnlineOrderingDetailsManager onlineOrderingDetailsManager = new OnlineOrderingDetailsManager();
            siteDeatils = onlineOrderingDetailsManager.GetDetailsFromFile();
            for (int i = 0; i < siteDeatils.Count; i++)
            {
                if (i == 0)
                    siteCode = siteDeatils[i];
                else if (i == 1)
                    syndicateCode = siteDeatils[i];
            }
        }
        public static NotifyIcon GetNotifyIcon()
        {
            return notifyIcon;
        }

        public static void SetNotifyIcon(NotifyIcon icon)
        {
            notifyIcon = icon;
        }

        public static bool Connect(string siteCode, string syndicateCode)
        {
            bool isConnected = false;
            try
            {
                // Get Site Code & Syndicate code from Database 
                //var querystringData = new Dictionary<string, string>
                //{
                //    {"siteCode", "1"},
                //    {"syndicateCode", "MTIzQUJDNDU2Nw=="}
                //};
                logsList.Add("Trying to Connect at                                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("SiteCode is                                           " + siteCode);
                logsList.Add("Syndicate Code is                                     " + syndicateCode);
                var isoEncoding = Encoding.GetEncoding("iso-8859-1");
                var byteArray = isoEncoding.GetBytes(syndicateCode.ToCharArray());
                var baseString = Convert.ToBase64String(byteArray);
                var querystringData = new Dictionary<string, string>
                {
                    {"siteCode", siteCode},
                    {"syndicateCode", baseString}
                };
                Connection = new HubConnection(ServerURI, querystringData);
                Connection.Closed += Connection_Closed;
                _hubProxy = Connection.CreateHubProxy("OrdersHub");
                //Handle incoming event from server: use Invoke to write to console from SignalR's thread
                _hubProxy.On<string>("ReceiveLength", order => SendOrderToService(order));
                Connection.Start().Wait();
                isConnected = Connection.State == ConnectionState.Connected;
                ContextMenuHelper helper = new ContextMenuHelper();

                if (isConnected)
                {
                    helper.ToggleOnConnect();
                    logsList.Add("SignalR client is connected with connection Id        " + Connection.ConnectionId);
                }
                else
                {
                    helper.ToggleOnDisconnect();
                    logsList.Add("SignalR client is not connected");
                }
            }
            catch (Exception exception)
            {
                isConnected = false;
                logsList.Add("Exception in making connection                        " + exception.Message);
                logsList.Add("Exception at                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            MakeLogFile();
            return isConnected;
        }

        private static void Connection_Closed()
        {
            try
            {
            }
            catch (Exception exception)
            {
            }
        }

        private static void SendOrderToService(string order)
        {
            try
            {
                WCFServiceLoyaltyMateClient loyaltyMateClient = new WCFServiceLoyaltyMateClient();
                string siteCode = "";
                string syndicateCode = "";
                logsList.Add("Received Order from LM at                             " + DateTime.Now.ToString("hh:mm:ss tt"));
                logsList.Add("Order Received is                                     " + order);
                GetSiteDetails(ref siteCode, ref syndicateCode);
                logsList.Add("siteCode is      " + siteCode + "     and syndicateCode is       " + syndicateCode);
                loyaltyMateClient.GetOrdersFromWeb(syndicateCode, order);
            }
            catch (Exception exception)
            {
                logsList.Add("Exception in SendOrderToService                       " + exception.Message);
                logsList.Add("Exception at                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            MakeLogFile();
        }

        public static bool Disconnect()
        {
            bool isDisconnected = false;
            try
            {
                logsList.Add("Trying to Disconnect at                               " + DateTime.Now.ToString("hh:mm:ss tt"));
                Connection.Stop();
                isDisconnected = Connection.State == ConnectionState.Connected;
                ContextMenuHelper helper = new ContextMenuHelper();

                if (isDisconnected)
                {
                    helper.ToggleOnConnect();
                    logsList.Add("SignalR client is disconnected                        " + Connection.ConnectionId);
                }
                else
                {
                    helper.ToggleOnDisconnect();
                    logsList.Add("SignalR client is not disconnected                    " + Connection.ConnectionId);
                }
            }
            catch (Exception exception)
            {
                isDisconnected = false;
                logsList.Add("Exception in Disconnect                               " + exception.Message);
                logsList.Add("Exception at                                          " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            MakeLogFile();
            return isDisconnected;
        }

        static void MakeLogFile()
        {
            try
            {
                logsList.Add("=================================================================================");
                string path = System.IO.Path.GetDirectoryName(
                          System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);


                string location = Path.Combine(path, "Signal Client Logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name2 = "SignalR " + DateTime.Now.ToString("ddMMMyyyy") + ".txt";
                string fileName = Path.Combine(location, name2);

                if (fileName.Contains(@"file:\"))
                {
                    fileName = fileName.Replace(@"file:\", "");
                }
                if (!File.Exists(fileName))
                {

                    using (StreamWriter sw = File.CreateText(fileName))
                    {
                        for (int i = 0; i < logsList.Count; i++)
                        {
                            sw.WriteLine(logsList[i]);
                        }
                    }
                }
                else
                {
                    using (var sw = File.AppendText(fileName))
                    {
                        for (int i = 0; i < logsList.Count; i++)
                        {
                            sw.WriteLine(logsList[i]);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
            }
            logsList.Clear();
        }
    }
}
