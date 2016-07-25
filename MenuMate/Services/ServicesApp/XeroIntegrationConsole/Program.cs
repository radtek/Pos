using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Configuration;
using XeroIntegration;
using System.Xml;
using XMLManager;

namespace XeroIntegrationConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            string appID = InitXeroIntegration();

            if (appID.Trim() != @"")
            {
                XeroIntegrationError error = XeroIntegrationManager.Instance.RunAppWithID(appID);

                if (error == XeroIntegrationError.NoError)
                {
                    Console.WriteLine(@"Xero Integration is running ...");
                    WaitForKey();
                }
                else
                {
                    Console.WriteLine(XeroIntegrationManager.Instance.ErrorMessage);
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static string InitXeroIntegration()
        {
            if(XeroIntegrationManager.Instance.Init() == XeroIntegrationError.NoError)
            {
                return AppID();
            }

            return @"";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static string AppID()
        {
            //return XeroIntegrationConsole.Properties.Settings.Default.XeroAppID;
            return XeroIntegrationManager.Instance.XeroAppInUse;
        }

        /// <summary>
        /// 
        /// </summary>
        static void WaitForKey()
        {
            ConsoleKeyInfo keyInfo = Console.ReadKey(true);
        }
    }
}
