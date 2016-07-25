using System;
using System.Linq;

using XeroIntegration;

namespace MenumateServices.MenumateRunners
{
    public class XeroIntegrationRunner : MenumateRunner
    {
        #region PUBLIC
        public XeroIntegrationRunner()
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

            ServiceLogger.Log(@"Starting Xero Integration ...");
            if (result = runXero())
            {
                ServiceLogger.Log(@"Xero Integration is running in shared folder mode ...");
            }
            else
            {
                ServiceLogger.Log(@"Xero Integration failed to run in shared folder mode ...");
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Stopping Xero Integration ...");
            if (result = stopXero())
            {
                ServiceLogger.Log(@"Xero Integration stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"Xero Integration failed to stop in shared folder mode ...");
            }

            //...........................................

            return result;
        }

        protected bool runXero()
        {

            bool result;
            _appID = InitXeroIntegration();

            if (_appID.Trim() != @"")
            {
                XeroIntegrationError error = XeroIntegrationManager.Instance.RunAppWithID(_appID);

                if (error == XeroIntegrationError.NoError)
                {
                    result = true;
                }
                else
                {
                    ServiceLogger.Log(XeroIntegrationManager.Instance.ErrorMessage);
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

        public bool stopXero()
        {
            bool result;

            //...........................................

            if (_appID.Trim() != @"")
            {
                XeroIntegrationError error = XeroIntegrationManager.Instance.StopAppWithID(_appID);

                if (error == XeroIntegrationError.NoError)
                {
                    result = true;
                }
                else
                {
                    ServiceLogger.Log(XeroIntegrationManager.Instance.ErrorMessage);
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

        private string InitXeroIntegration()
        {
            string result = @"";

            if (XeroIntegrationManager.Instance.Init() == XeroIntegrationError.NoError)
            {
                result = AppID();
            }

            return result;
        }

        private string AppID()
        {
            return XeroIntegrationConfigManager.Instance.XeroAppInUse;
        }

        #endregion
    }
}
