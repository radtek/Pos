using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Microsoft.AspNet.SignalR.Client;
using SystemTrayApp.OnlineOrdering;
using OracleTCPServer;

namespace SystemTrayApp.SignalR
{
    public static class Client
    {
        private static IHubProxy _hubProxy { get; set; }
        //const string ServerURI = "http://localhost/MenumateOrdering/";
        const string ServerURI = "http://lmonlineordering.azurewebsites.net/";
        public static HubConnection Connection { get; set; }
        private static NotifyIcon notifyIcon;
        private static List<string> logsList;

        public static bool InitializeClient()
        {
            logsList = new List<string>();
            string siteCode = "";
            string syndicateCode = "";
            GetSiteDetails(ref siteCode, ref syndicateCode);
            return Connect(siteCode, syndicateCode);
        }

        private static void GetSiteDetails(ref string siteCode, ref string syndicateCode)
        {
            List<string> siteDeatils = new List<string>();
            OnlineOrderingDetailsManager onlineOrderingDetailsManager = new OnlineOrderingDetailsManager();
            siteDeatils = onlineOrderingDetailsManager.GetDetailsFromFile();
            for(int i = 0; i < siteDeatils.Count; i++)
            {
                if(i == 0)
                    siteCode = siteDeatils[i];
                else if(i== 1)
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

                if(isConnected)
                    helper.ToggleOnConnect();
                else
                    helper.ToggleOnDisconnect();
            }
            catch (Exception exception)
            {
                isConnected = false;
            }
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
                loyaltyMateClient.GetOrdersFromWeb(order);
            }
            catch (Exception exception)
            {

            }
        }

        public static bool Disconnect()
        {
            bool isDisconnected = false;
            try
            {
                Connection.Stop();
                isDisconnected = Connection.State == ConnectionState.Connected;
                ContextMenuHelper helper = new ContextMenuHelper();

                if (isDisconnected)
                    helper.ToggleOnConnect();
                else
                    helper.ToggleOnDisconnect();
            }
            catch (Exception e)
            {
                isDisconnected = false;
            }
            return isDisconnected;
        }
    }
}
