using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AccountingIntegration.MYOB
{
    using RunningApplications = Dictionary<string, MYOBIntegrationApplicationRunner>;
    using AccountingIntegration.Model;

    class MYOBIntegrationApplicationRunnerManager : IAccountingApllicationManager
    {
        string _errorMsg = @"";
        RunningApplications _runningApps = new RunningApplications();

        MYOBIntegrationApplicationRunner _myobAppRunner;

        private static volatile MYOBIntegrationApplicationRunnerManager _instance;
        private static readonly object syncRoot = new Object();

        private MYOBIntegrationApplicationRunnerManager()
        {
        }

        public static MYOBIntegrationApplicationRunnerManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new MYOBIntegrationApplicationRunnerManager();
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
                ServiceLogger.Log(string.Format(@"Connected to MYOB: {0}", inCredentials.Organisation));
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
                _myobAppRunner.StopActivityMonitoring();
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
                _myobAppRunner.StopActivityMonitoring();
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
                _myobAppRunner.StartActivityMonitoring();
                return AccountingIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return AccountingIntegrationError.FailedToResumeApp;
            }
        }

        private MYOBIntegrationApplicationRunner CreateAppRunner(Credentials inCredentials)
        {
            return _myobAppRunner = MYOBIntegrationApplicationRunner.CreateAppRunner(inCredentials);
        }

        #endregion
    }
}
