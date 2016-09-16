using System;
using System.Linq;
using AccountingIntegration;

namespace MenumateServices.MenumateRunners
{
    public class AccountingIntegrationRunner : MenumateRunner
    {
        #region PUBLIC
        public AccountingIntegrationRunner()
        {
            InitializeRunner();
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Start()
        {
            // go ahead and start the worker thread
            WorkerThread.Start();

            Paused = false;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Stop()
        {
            StopRunner();

            Paused = true;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Pause()
        {
            StopRunner();

            Paused = true;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Resume()
        {
            InitializeRunner();

            Start();
        }
        #endregion

        #region PROTECTED

        string _appID;

        protected override bool StartServiceTask()
        {
            return StartRunner();
        }

        protected override bool StartRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Starting Accounting Integration ...");
            if (result = runAccounting())
            {
                ServiceLogger.Log(@"Accounting Integration is running in shared folder mode ...");
            }
            else
            {
                ServiceLogger.Log(@"Accounting Integration failed to run in shared folder mode ...");
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Stopping Accounting Integration ...");
            if (result = stopAccounting())
            {
                ServiceLogger.Log(@"Accounting Accounting stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"Accounting Accounting failed to stop in shared folder mode ...");
            }

            //...........................................

            return result;
        }

        protected bool runAccounting()  // todo RunAccounting
        {

            bool result;
            _appID = InitAccountingIntegration();

            if (_appID.Trim() != @"")
            {
                AccountingIntegrationError error = AccountingIntegrationManager.Instance.RunAppWithID(_appID);

                if (error == AccountingIntegrationError.NoError)
                {
                    result = true;
                }
                else
                {
                    ServiceLogger.Log(AccountingIntegrationManager.Instance.ErrorMessage);
                    result = false;
                }
            }
            else
            {
                ServiceLogger.Log(string.Format(@"Invalid app ID: {0}", _appID));
                result = false;
            }
            return result;
        }

        public bool stopAccounting()   // todo StopAcounting
        {
            bool result;

            //...........................................

            if (_appID.Trim() != @"")
            {
                AccountingIntegrationError error = AccountingIntegrationManager.Instance.StopAppWithID(_appID);

                if (error == AccountingIntegrationError.NoError)
                {
                    result = true;
                }
                else
                {
                    ServiceLogger.Log(AccountingIntegrationManager.Instance.ErrorMessage);
                    result = false;
                }
            }
            else
            {
                ServiceLogger.Log(string.Format(@"Invalid app ID: {0}", _appID));
                result = false;
            }

            //...........................................

            return result;
        }

        private string InitAccountingIntegration()
        {
            string result = @"";
            if (AccountingIntegrationManager.Instance.Init() == AccountingIntegrationError.NoError)
            {
                result = AppID();
            }
            return result;
        }

        private string AppID()
        {
            return AccountingIntegrationConfigManager.Instance.AccountingAppInUse;
        }

        #endregion
    }
}
