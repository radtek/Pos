using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using MenumateServices.WCFServices;
using Thorlink.Net;
using System.Diagnostics;

namespace MenumateServices.MenumateRunners
{
    public class ThorlinkServiceRunner : MenumateWebServiceRunner
    {
        public ThorlinkServiceRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In ThorlinkServiceRunner ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 174, short.MaxValue);
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
                EventLog.WriteEntry("In Start ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 175, short.MaxValue);
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
                EventLog.WriteEntry("In Stop ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 175, short.MaxValue);
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
                EventLog.WriteEntry("In Pause ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 176, short.MaxValue);
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
                EventLog.WriteEntry("In Resume ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 177, short.MaxValue);
            }
        }

        protected override bool StartServiceTask()
        {
            try
            {
                var thorPingTimer = new ThorPingTimer();
                return StartRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In StartServiceTask ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 178, short.MaxValue);
            }
            return false;
        }


        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(WCFServiceThorlink));
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In CreateServiceHost ThorRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 179, short.MaxValue);
            }
            return null; ;
        }

        protected override bool StartRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Starting Thorlink Server endpoint ...");

            result = OpenServiceHost();

            if (result)
            {
                ServiceLogger.Log(@"Thorlink Server endpoint is running ...");

                //TODO: Initialize the Thorlink Integration Server....

                ServiceInfo serviceInfo = ServiceInfo.Instance;

                ServiceLogger.Log(string.Format(@"Thor Server: Database server URL: {0}", serviceInfo.WebmateDatabaseServerPath()));
                ServiceLogger.Log(string.Format(@"Thor Server: Database location: {0}", serviceInfo.WebmateDatabaseLocation()));

                ServiceLogger.Log(@"Thor Server runner: Thorlink Integration Service has started");
            }
            else
            {
                ServiceLogger.Log(@"Thor Server endpoint failed to run ...");
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Stopping Thorlink Integration Service endpoint ...");

            //TODO: Clear the Clipp Integration Server....
            result = CloseServiceHost();

            ServiceLogger.Log(result
                ? @"Thorlink Integration Service endpoint stopped ..."
                : @"Thorlink Integration Service endpoint failed to stop ...");

            return result;
        }
    }
}
