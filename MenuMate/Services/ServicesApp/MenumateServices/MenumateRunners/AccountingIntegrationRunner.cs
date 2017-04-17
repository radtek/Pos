using System;
using System.Linq;
using AccountingIntegration;
using System.Diagnostics;

namespace MenumateServices.MenumateRunners
{
    public class AccountingIntegrationRunner : MenumateRunner
    {
        #region PUBLIC
        public AccountingIntegrationRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In AccountingIntegrationRunner acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 121, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Start()
        {
            // go ahead and start the worker thread
            try
            {
                WorkerThread.Start();

                Paused = false;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Start acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 122, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Stop()
        {
            try
            {
                StopRunner();

                Paused = true;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Stop acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 123, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Pause()
        {
            try
            {
                StopRunner();

                Paused = true;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Pause acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 124, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Resume()
        {
            try
            {
                InitializeRunner();

                Start();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Resume acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 125, short.MaxValue);
            }
        }
        #endregion

        #region PROTECTED

        string _appID;

        protected override bool StartServiceTask()
        {
            try
            {
                return StartRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StartServiceTask acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 126, short.MaxValue);
            }
            return false;
        }

        protected override bool StartRunner()
        {
            bool result = false;

            try
            {
                ServiceLogger.Log(@"Starting Accounting Integration ...");
                if (result = runAccounting())
                {
                    ServiceLogger.Log(@"Accounting Integration is running in shared folder mode ...");
                }
                else
                {
                    ServiceLogger.Log(@"Accounting Integration failed to run in shared folder mode ...");
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StartRunner acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 127, short.MaxValue);
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................
            try
            {

                ServiceLogger.Log(@"Stopping Accounting Integration ...");
                if (result = stopAccounting())
                {
                    ServiceLogger.Log(@"Accounting Accounting stopped ...");
                }
                else
                {
                    ServiceLogger.Log(@"Accounting Accounting failed to stop in shared folder mode ...");
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StopRunner acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 128, short.MaxValue);
            }

            //...........................................

            return result;
        }

        protected bool runAccounting()  // todo RunAccounting
        {

            bool result = false; ;
            try
            {
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
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In runAccounting acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 129, short.MaxValue);
            }
            return result;
        }

        public bool stopAccounting()   // todo StopAcounting
        {
            bool result = false;

            //...........................................

            try
            {
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
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In stopAccounting acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 130, short.MaxValue);
            }

            //...........................................

            return result;
        }

        private string InitAccountingIntegration()
        {
            string result = @"";
            try
            {
                if (AccountingIntegrationManager.Instance.Init() == AccountingIntegrationError.NoError)
                {
                    result = AppID();
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In InitAccountingIntegration acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 131, short.MaxValue);
            }
            return result;
        }

        private string AppID()
        {
            try
            {
                return AccountingIntegrationConfigManager.Instance.AccountingAppInUse;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In AppID acrunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 132, short.MaxValue);
            }
            return "";
        }

        #endregion
    }
}
