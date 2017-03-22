using System.ServiceModel;
using System.Diagnostics;
using System;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class SalesForceIntegrationServiceRunner : MenumateWebServiceRunner
    {
        public SalesForceIntegrationServiceRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In SalesForceIntegrationServiceRunner SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 160, short.MaxValue);
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
                EventLog.WriteEntry("In Start SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 161, short.MaxValue);
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
                EventLog.WriteEntry("In Stop SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 162, short.MaxValue);
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
                EventLog.WriteEntry("In Pause SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 163, short.MaxValue);
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
                EventLog.WriteEntry("In Resume SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 164, short.MaxValue);
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
                EventLog.WriteEntry("In StartServiceTask SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 165, short.MaxValue);
            }
            return false;
        }


        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(SalesForceIntegrationWebService));
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In ServiceHost SFRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 166, short.MaxValue);
            }
            return null;
        }

        protected override bool StartRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Starting Sales Force Server endpoint ...");

            result = OpenServiceHost();

            if (result)
            {
                ServiceLogger.Log(@"Sales Force Server endpoint is running ...");

                //TODO: Initialize the Clipp Integration Server....

                ServiceInfo serviceInfo = ServiceInfo.Instance;

                ServiceLogger.Log(string.Format(@"Sales Force Server: Database server URL: {0}", serviceInfo.WebmateDatabaseServerPath()));
                ServiceLogger.Log(string.Format(@"Sales Force Server: Database location: {0}", serviceInfo.WebmateDatabaseLocation()));

                ServiceLogger.Log(@"Sales Force Server runner: Sales Force Integration Service has started");
            }
            else
            {
                ServiceLogger.Log(@"Sales Force Server endpoint failed to run ...");
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Stopping Sales Force Integration Service endpoint ...");

            //TODO: Clear the Clipp Integration Server....
            result = CloseServiceHost();

            ServiceLogger.Log(result
                ? @"Sales Force Integration Service endpoint stopped ..."
                : @"Sales Force Integration Service endpoint failed to stop ...");

            return result;
        }
    }
}
