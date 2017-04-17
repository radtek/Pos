using System.ServiceModel;
using System;
using System.Diagnostics;
using MenumateServices.WCFServices;
using MenumateServices.WebMate.InternalClasses;

namespace MenumateServices.MenumateRunners
{
    public class WebmateRunner : MenumateWebServiceRunner
    {
        public WebmateRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In WebmateRunner WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 180, short.MaxValue);
            }
        }

        public override void Start()
        {
            try
            {
                WorkerThread.Start();
                Paused = false;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Start WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }
        }

        public override void Stop()
        {
            try
            {
                StopRunner();
                Paused = true;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Stop WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 182, short.MaxValue);
            }
        }

        public override void Pause()
        {
            try
            {
                StopRunner();
                Paused = true;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Pause WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 183, short.MaxValue);
            }
        }

        public override void Resume()
        {
            try
            {
                InitializeRunner();
                Start();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Resume WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 184, short.MaxValue);
            }
        }

        protected override bool StartServiceTask()
        {
            try
            {
                return StartRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In StartServiceTask WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 185, short.MaxValue);
            }
            return false;
        }

        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(WCFServiceWebMate));
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In CreateServiceHost WebmateRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 186, short.MaxValue);
            }
            return null;
        }

        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Webmate endpoint ...");
            if (result = OpenServiceHost())
            {
                ServiceLogger.Log(@"Webmate endpoint is running ...");

                WebOrderProcessor.Instance.Init();

                ServiceInfo serviceInfo = ServiceInfo.Instance;

                ServiceLogger.Log(string.Format(@"Webmate: Database server URL: {0}", serviceInfo.WebmateDatabaseServerPath()));
                ServiceLogger.Log(string.Format(@"Webmate: Database location: {0}", serviceInfo.WebmateDatabaseLocation()));

                ServiceLogger.Log(@"Webmate runner: Webmate has started");
            }
            else
            {
                ServiceLogger.Log(@"Webmate endpoint failed to run ...");
            }

            //...........................................

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Stopping WebMate endpoint ...");

            WebOrderProcessor.Instance.Done();
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"WebMate endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"WebMate endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
    }
}
