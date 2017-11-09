using System;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.ServiceModel;

using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class LoyaltymateRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
        public LoyaltymateRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In LoyaltymateRunner LoyaltymateRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 141, short.MaxValue);
            }
        }

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
                EventLog.WriteEntry("In Start LoyaltymateRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 142, short.MaxValue);
            }
        }

        public override void Stop()
        {
            try
            {
                StopRunner();

                Paused = true;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Stop LoyaltymateRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 143, short.MaxValue);
            }
        }


        public override void Pause()
        {
            try
            {
                StopRunner();

                Paused = true;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In LoyaltymateRunner Pause", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 144, short.MaxValue);
            }
        }


        public override void Resume()
        {
            try
            {
                InitializeRunner();

                Start();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Resume LoyaltymateRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
            }
        }
        #endregion

        #region PROTECTED
        /// <summary>
        /// This method is called from the thread's Execute method
        /// in the base class MenumateRunner 
        /// </summary>
        /// <returns></returns>
        protected override bool StartServiceTask()
        {
            try
            {
                return StartRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StartServiceTask LoyaltymateRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 146, short.MaxValue);
            }
            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(WCFServiceLoyaltyMate));
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In CreateServiceHost LoyaltymateRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 147, short.MaxValue);
            }
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Loyaltymate endpoint ...");
            if (result = OpenServiceHost())
            {
                ServiceLogger.Log(@"Loyaltymate endpoint is running ...");

                // Any Loyaltymate initialization goes here!!!!!

                ServiceLogger.Log(@"Loyaltymate runner: Loyaltymate has started");
            }
            else
            {
                ServiceLogger.Log(@"Loyaltymate endpoint failed to run ...");
            }

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Stopping LoyaltyMate endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"LoyaltyMate endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"LoyaltyMate endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}
