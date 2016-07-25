using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XeroIntegration
{
    using RunningApplications = Dictionary<string, XeroIntegrationApplicationRunner>;
    using XeroIntegration.Model;

    class XeroIntegrationApplicationRunnerManager
    {
        string _errorMsg = @"";
        RunningApplications _runningApps = new RunningApplications();
       
        XeroIntegrationApplicationRunner _xeroAppRunner;


        private static volatile XeroIntegrationApplicationRunnerManager _instance;
        private static readonly object syncRoot = new Object();

        private XeroIntegrationApplicationRunnerManager()
        {

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


        public XeroIntegrationError RunAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.AppAlreadyRunning;
            ClearErrMsg();

            if (!AppAlreadyRunning(inCredentials))
            {
                result = AddRunningApp(inCredentials);
            }
            return result;
        }

        public XeroIntegrationError StopAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.AppNotRunning;
            if (AppAlreadyRunning(inCredentials))
            {
                result = StopApp(inCredentials);
            }
            return result;
        }

        public XeroIntegrationError PauseAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.AppNotRunning;
            if(AppAlreadyRunning(inCredentials))
            {
                result = PauseApp(inCredentials);
            }
            return result;
        }

        public XeroIntegrationError ResumeAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.AppAlreadyRunning;

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

        private XeroIntegrationError AddRunningApp(Credentials inCredentials)
        {
            try
            {
                _runningApps.Add(inCredentials.AppID, CreateAppRunner(inCredentials));
                ServiceLogger.Log(string.Format(@"Connected to Xero: {0}", inCredentials.Organisation));
                return XeroIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return XeroIntegrationError.FailedToRunApp;
            }
        }

        private XeroIntegrationError StopApp(Credentials inCredentials)
        {
            try
            {
                _xeroAppRunner.StopActivityMonitoring();
                return XeroIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return XeroIntegrationError.FailedToPauseApp;
            }
        }

        private XeroIntegrationError PauseApp(Credentials inCredentials)
        {
            try
            {
                _xeroAppRunner.StopActivityMonitoring();
                return XeroIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return XeroIntegrationError.FailedToPauseApp;
            }
        }

        private XeroIntegrationError ResumeApp(Credentials inCredentials)
        {
            try
            {
                _xeroAppRunner.StartActivityMonitoring();
                return XeroIntegrationError.NoError;
            }
            catch (XeroIntegrationException xiExc)
            {
                _errorMsg = xiExc.Message;
                return XeroIntegrationError.FailedToResumeApp;
            }
        }

        private XeroIntegrationApplicationRunner CreateAppRunner(Credentials inCredentials)
        {
            return _xeroAppRunner = XeroIntegrationApplicationRunner.CreateAppRunner(inCredentials);
        }

        #endregion
    }
}
