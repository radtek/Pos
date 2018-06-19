using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AccountingIntegration.Xero
{
    using RunningApplications = Dictionary<string, XeroIntegrationApplicationRunner>;
    using AccountingIntegration.Model;
    using System.Net;

    class XeroIntegrationApplicationRunnerManager : IAccountingApllicationManager
    {
        string _errorMsg = @"";
        RunningApplications _runningApps = new RunningApplications();

        XeroIntegrationApplicationRunner _xeroAppRunner;

        private static volatile XeroIntegrationApplicationRunnerManager _instance;
        private static readonly object syncRoot = new Object();

        private XeroIntegrationApplicationRunnerManager()
        {
            ServicePointManager.SecurityProtocol = (SecurityProtocolType)3072;
        }

        public static XeroIntegrationApplicationRunnerManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new XeroIntegrationApplicationRunnerManager();
                    }
                }

                return _instance;
            }
        }


        #region Properties

         public string ErrorMessage
        {
            get
            {
                return _errorMsg;
            }
        }

        #endregion


         public AccountingIntegrationError RunAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.AppAlreadyRunning;
            ClearErrMsg();

            if (!AppAlreadyRunning(inCredentials))
            {
                result = AddRunningApp(inCredentials);
            }
            return result;
        }

         public AccountingIntegrationError StopAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.AppNotRunning;
            if (AppAlreadyRunning(inCredentials))
            {
                result = StopApp(inCredentials);
            }
            return result;
        }

         public AccountingIntegrationError PauseAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.AppNotRunning;
            if(AppAlreadyRunning(inCredentials))
            {
                result = PauseApp(inCredentials);
            }
            return result;
        }

         public AccountingIntegrationError ResumeAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.AppAlreadyRunning;

            if(!AppAlreadyRunning(inCredentials))
            {
                result = ResumeApp(inCredentials);
            }
            return result;
        }
        
        #region Private
   
        private void ClearErrMsg()
        {
            _errorMsg = @"";
        }

        private bool AppAlreadyRunning(Credentials inCredentials)
        {
            return _runningApps.ContainsKey(inCredentials.AppID);
        }

        private AccountingIntegrationError AddRunningApp(Credentials inCredentials)
        {
            try
            {
                _runningApps.Add(inCredentials.AppID, CreateAppRunner(inCredentials));
                ServiceLogger.Log(string.Format(@"Connected to Xero: {0}", inCredentials.Organisation));
                return AccountingIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return AccountingIntegrationError.FailedToRunApp;
            }
        }

        private AccountingIntegrationError StopApp(Credentials inCredentials)
        {
            try
            {
                _xeroAppRunner.StopActivityMonitoring();
                return AccountingIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return AccountingIntegrationError.FailedToPauseApp;
            }
        }

        private AccountingIntegrationError PauseApp(Credentials inCredentials)
        {
            try
            {
                _xeroAppRunner.StopActivityMonitoring();
                return AccountingIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return AccountingIntegrationError.FailedToPauseApp;
            }
        }

        private AccountingIntegrationError ResumeApp(Credentials inCredentials)
        {
            try
            {
                _xeroAppRunner.StartActivityMonitoring();
                return AccountingIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return AccountingIntegrationError.FailedToResumeApp;
            }
        }

        private XeroIntegrationApplicationRunner CreateAppRunner(Credentials inCredentials)
        {
            return _xeroAppRunner = XeroIntegrationApplicationRunner.CreateAppRunner(inCredentials);
        }

        #endregion
    }
}
