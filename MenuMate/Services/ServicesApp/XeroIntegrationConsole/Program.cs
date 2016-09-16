using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Configuration;
using AccountingIntegration;
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
                AccountingIntegrationError error = AccountingIntegrationManager.Instance.RunAppWithID(appID);

                if (error == AccountingIntegrationError.NoError)
                {
                    Console.WriteLine(@"Xero Integration is running ...");
                    WaitForKey();
                }
                else
                {
                    Console.WriteLine(AccountingIntegrationManager.Instance.ErrorMessage);
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static string InitXeroIntegration()
        {
            if (AccountingIntegrationManager.Instance.Init() == AccountingIntegrationError.NoError)
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
            return AccountingIntegrationConfigManager.Instance.AccountingAppInUse;
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
