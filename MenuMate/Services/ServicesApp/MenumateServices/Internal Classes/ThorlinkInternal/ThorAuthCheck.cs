using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Thorlink.Net;
using System.Diagnostics;
using System.Threading.Tasks;
using System.IO;

namespace MenumateServices.InternalClasses.ThorlinkInternal
{
    public sealed class ThorAuthCheck
    {
        public string ThorlinkAccessToken;
        public ThorPingTimer Thorlink;
        private static volatile ThorAuthCheck instance;

        private static object syncRoot = new Object();
        private ThorAuthCheck() 
        {

        }

        public static ThorAuthCheck Instance
        {
            get 
            {
               if (instance == null) 
               {
                  lock (syncRoot) 
                  {
                     if (instance == null)
                         instance = new ThorAuthCheck();
                  }
               }

               return instance;
            }
         }

        /// <summary>
        /// 
        /// </summary>
        public void CallThorInitMethod(DTO.Thorlink.DTO_ThorAuth thorAuth)
        {
            if (thorAuth == null)
            {
                throw new Exception("CallThorInitMethod received a null argument!");
            }
            try
            {
                if (thorAuth != null)
                {
                    ThorConfig.AppKey = thorAuth.appKey;
                    ThorConfig.SiteNo = thorAuth.siteNo;
                    ThorConfig.MerchantCode = thorAuth.merchantCode;
                    ThorConfig.DeviceCode = thorAuth.deviceCode;
                    ThorConfig.OnSetToken = HandlerForToken;
                    ThorConfig.LocalDirectory = @"C:\Thorlink Logs";
                    string credentials = @"appId:- " + thorAuth.appKey;
                    credentials += @"siteNo:- " + thorAuth.siteNo;
                    credentials += @"siteNo:- " + thorAuth.siteNo;
                    credentials += @"siteNo:- " + thorAuth.siteNo;
                    credentials += @"path for directory:- " + ThorConfig.LocalDirectory;
                    ServiceLogger.Log(@"Values set for ThorConfiguration as ");
                    ThorConfig.Init();
                    ServiceLogger.Log(@"Thor Config Init() method called with "+ credentials);
                }
            }
            catch (Exception exception)
            {
                ServiceLogger.LogException("Exception in Calling Init Method ", exception);
            }

        }

        private void HandlerForToken(string token)
        {
            if (token == null)
            {
                throw new ArgumentNullException("HandlerForToken received a null argument!");
            }
            else
            {
                ThorlinkAccessToken = token;
            }
        }
    }
}
