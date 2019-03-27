using System;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.ServiceModel;
using System.IO;

using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class OnlineOrderingRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
        public OnlineOrderingRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In OnlineOrderingRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 141, short.MaxValue);
            }
        }

        public override void Start()
        {
            // go ahead and start the worker thread
            try
            {
                WorkerThread.Start();
                CreateOrderStatusDir();
                Paused = false;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Start OnlineOrderingRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 142, short.MaxValue);
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
                EventLog.WriteEntry("In Stop OnlineOrderingRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 143, short.MaxValue);
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
                EventLog.WriteEntry("In OnlineOrderingRunner Pause", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 144, short.MaxValue);
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
                EventLog.WriteEntry("In Resume OnlineOrderingRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
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
                EventLog.WriteEntry("In StartServiceTask OnlineOrderingRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 146, short.MaxValue);
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
                return new ServiceHost(typeof(WCFServiceOnlineOrdering));
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In CreateServiceHost OnlineOrderingRunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 147, short.MaxValue);
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

            ServiceLogger.Log(@"Starting OnlineOrdering endpoint ...");
            if (result = OpenServiceHost())
            {
                ServiceLogger.Log(@"OnlineOrdering endpoint is running ...");

                // Any OnlineOrdering initialization goes here!!!!!

                ServiceLogger.Log(@"OnlineOrdering runner: OnlineOrdering has started");
            }
            else
            {
                ServiceLogger.Log(@"OnlineOrdering endpoint failed to run ...");
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

            ServiceLogger.Log(@"Stopping OnlineOrdering endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"OnlineOrdering endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"OnlineOrdering endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
        
        #region Private
        private void CreateOrderStatusDir()
        {
            try
            {
                string path = System.IO.Path.GetDirectoryName(
                System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                string location = Path.Combine(path, "logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);
                string pendingOrderDir = Path.Combine(location, "Pending Orders");
                string successfulOrderDir = Path.Combine(location, "Successful Orders");
                string failedOrderDir = Path.Combine(location, "Failed Orders");

                if (!Directory.Exists(pendingOrderDir))
                    Directory.CreateDirectory(pendingOrderDir);

                if (!Directory.Exists(successfulOrderDir))
                    Directory.CreateDirectory(successfulOrderDir);

                if (!Directory.Exists(failedOrderDir))
                    Directory.CreateDirectory(failedOrderDir);
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In CreateOrderStatusDir", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
            }
        }
        #endregion
    }
}
