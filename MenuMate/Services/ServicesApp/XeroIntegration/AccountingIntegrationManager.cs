using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AccountingIntegration.Model;
using AccountingIntegration.Xero;
using AccountingIntegration.MYOB;


namespace AccountingIntegration
{
    public enum AccountingIntegrationError
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


    public class AccountingIntegrationManager
    {
        bool _xiManagerInitialized = false;
        private static volatile AccountingIntegrationManager _instance;
        private static readonly object syncRoot = new Object();
        const string UserAgentStringXero = "Xero Integration";
        const string UserAgentStringMYOB = "MYOB Integration";
        IAccountingApllicationManager _iAccountingApllicationManager;
        private AccountingIntegrationManager()
        {

        }

        public static AccountingIntegrationManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new AccountingIntegrationManager();
                    }
                }

                return _instance;
            }
        }

        public string ErrorMessage
        {
            get
            {
                return "Error Occuurred in restarting app with same id";
            }
        }

        public AccountingIntegrationError Init()
        {
            AccountingIntegrationConfigManager.Instance.LoadConfiguration();
            if (string.IsNullOrWhiteSpace(AccountingIntegrationConfigManager.Instance.AccountingAppInUse))
                return AccountingIntegrationError.InitFailed;
            else
            {
                _xiManagerInitialized = true;
                return AccountingIntegrationError.NoError;
            }
        }

        public AccountingIntegrationError RunAppWithID(string inAppID)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NotInitialized;

            if (_xiManagerInitialized)
            {
                var credentials = AccountingIntegrationConfigManager.Instance.GetByAppId(inAppID);

                if (credentials != null)
                {
                    result = RunAppWithCredentials(credentials);
                }
                else
                {
                    result = AccountingIntegrationError.UnknownApp;
                }
            }
            return result;
        }

        public AccountingIntegrationError StopAppWithID(string inAppID)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NotInitialized;
            if (_xiManagerInitialized)
            {
                var credentials = AccountingIntegrationConfigManager.Instance.GetByAppId(inAppID);
                if (credentials != null)
                {
                    result = pStopAppWithCredentials(credentials);
                }
                else
                {
                    result = AccountingIntegrationError.UnknownApp;
                }
            }

            //::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        public AccountingIntegrationError PauseAppWithID(string inAppID)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NotInitialized;

            //:::::::::::::::::::::::::::::::::::::::::

            if (_xiManagerInitialized)
            {
                var credentials = AccountingIntegrationConfigManager.Instance.GetByAppId(inAppID);

                if (credentials != null)
                {
                    result = pPauseAppWithCredentials(credentials);
                }
                else
                {
                    result = AccountingIntegrationError.UnknownApp;
                }
            }

            //::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        public AccountingIntegrationError ResumeAppWithID(string inAppID)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NotInitialized;
            if (_xiManagerInitialized)
            {
                var credentials = AccountingIntegrationConfigManager.Instance.GetByAppId(inAppID);

                if (credentials != null)
                {
                    result = pResumeAppWithCredentials(credentials);
                }
                else
                {
                    result = AccountingIntegrationError.UnknownApp;
                }
            }
            return result;
        }

        #region Private



        private AccountingIntegrationError RunAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NoError;
            ExtractAppType(inCredentials);
            result = _iAccountingApllicationManager.RunAppWithCredentials(inCredentials);
            return result;
        }

        private AccountingIntegrationError pStopAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NoError;
            ExtractAppType(inCredentials);
            result = _iAccountingApllicationManager.StopAppWithCredentials(inCredentials);
            return result;
        }


        private AccountingIntegrationError pPauseAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NoError;
            ExtractAppType(inCredentials);
            result = _iAccountingApllicationManager.PauseAppWithCredentials(inCredentials);
            return result;
        }


        private AccountingIntegrationError pResumeAppWithCredentials(Credentials inCredentials)
        {
            AccountingIntegrationError result = AccountingIntegrationError.NoError;
            ExtractAppType(inCredentials);
            result = _iAccountingApllicationManager.ResumeAppWithCredentials(inCredentials);
            return result;
        }
        private void ExtractAppType(Credentials inCredentials)
        {
            if (inCredentials.UserAgentString == UserAgentStringXero)
                _iAccountingApllicationManager = XeroIntegrationApplicationRunnerManager.Instance;
            else
                _iAccountingApllicationManager = MYOBIntegrationApplicationRunnerManager.Instance;
        }
        #endregion
    }
}
