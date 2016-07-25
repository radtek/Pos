using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using XeroIntegration.Model;

namespace XeroIntegration
{
    public enum XeroIntegrationError
    {
        NoError,
        NotInitialized,
        InitFailed,
        InvalidAppInUse,
        AppAlreadyRunning,
        AppNotRunning,
        UnknownApp,
        FailedToRunApp,
        FailedToPauseApp,
        FailedToResumeApp
    };


    public class XeroIntegrationManager
    {
        bool _xiManagerInitialized = false;
        private static volatile XeroIntegrationManager _instance;
        private static readonly object syncRoot = new Object();

        private XeroIntegrationManager()
        {

        }

        public static XeroIntegrationManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new XeroIntegrationManager();
                    }
                }

                return _instance;
            }
        }

        public string ErrorMessage
        {
            get
            {
                return ErrorMessage;
            }
        }

        public XeroIntegrationError Init()
        {
            if (string.IsNullOrWhiteSpace(XeroIntegrationConfigManager.Instance.XeroAppInUse))
                return XeroIntegrationError.InitFailed;
            else
            {
                _xiManagerInitialized = true;
                return XeroIntegrationError.NoError;
            }
        }

        public XeroIntegrationError RunAppWithID(string inAppID)
        {
            XeroIntegrationError result = XeroIntegrationError.NotInitialized;

            if (_xiManagerInitialized)
            {
                var credentials = XeroIntegrationConfigManager.Instance.GetByAppId(inAppID);

                if (credentials != null)
                {
                    result = RunAppWithCredentials(credentials);
                }
                else
                {
                    result = XeroIntegrationError.UnknownApp;
                }
            }
            return result;
        }

        public XeroIntegrationError StopAppWithID(string inAppID)
        {
            XeroIntegrationError result = XeroIntegrationError.NotInitialized;
            if (_xiManagerInitialized)
            {
                var credentials = XeroIntegrationConfigManager.Instance.GetByAppId(inAppID);
                if (credentials != null)
                {
                    result = pStopAppWithCredentials(credentials);
                }
                else
                {
                    result = XeroIntegrationError.UnknownApp;
                }
            }

            //::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        public XeroIntegrationError PauseAppWithID(string inAppID)
        {
            XeroIntegrationError result = XeroIntegrationError.NotInitialized;

            //:::::::::::::::::::::::::::::::::::::::::

            if (_xiManagerInitialized)
            {
                var credentials = XeroIntegrationConfigManager.Instance.GetByAppId(inAppID);

                if (credentials != null)
                {
                    result = pPauseAppWithCredentials(credentials);
                }
                else
                {
                    result = XeroIntegrationError.UnknownApp;
                }
            }

            //::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        public XeroIntegrationError ResumeAppWithID(string inAppID)
        {
            XeroIntegrationError result = XeroIntegrationError.NotInitialized;
            if (_xiManagerInitialized)
            {
                var credentials = XeroIntegrationConfigManager.Instance.GetByAppId(inAppID);

                if (credentials != null)
                {
                    result = pResumeAppWithCredentials(credentials);
                }
                else
                {
                    result = XeroIntegrationError.UnknownApp;
                }
            }
            return result;
        }

        #region Private



        private XeroIntegrationError RunAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.NoError;
            result = XeroIntegrationApplicationRunnerManager.Instance.RunAppWithCredentials(inCredentials);
            return result;
        }

        private XeroIntegrationError pStopAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.NoError;
            result = XeroIntegrationApplicationRunnerManager.Instance.StopAppWithCredentials(inCredentials);
            return result;
        }


        private XeroIntegrationError pPauseAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.NoError;
            result = XeroIntegrationApplicationRunnerManager.Instance.PauseAppWithCredentials(inCredentials);
            return result;
        }


        private XeroIntegrationError pResumeAppWithCredentials(Credentials inCredentials)
        {
            XeroIntegrationError result = XeroIntegrationError.NoError;
            result = XeroIntegrationApplicationRunnerManager.Instance.ResumeAppWithCredentials(inCredentials);
            return result;
        }
        #endregion
    }
}
