using System;
using System.ServiceModel;
using MenumateServices.WCFServices;
using System.Diagnostics;

namespace MenumateServices.MenumateRunners
{
    public class ClippIntegrationServiceRunner : MenumateWebServiceRunner
    {
        public ClippIntegrationServiceRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In ClippIntegrationServiceRunner clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 133, short.MaxValue);
            }
        }

        public override void Start()
        {
            try
            {
                WorkerThread.Start();
                Paused = false;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In Start clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 134, short.MaxValue);
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
                EventLog.WriteEntry("In Stop clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 135, short.MaxValue);
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
                EventLog.WriteEntry("In Pause clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 136, short.MaxValue);
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
                EventLog.WriteEntry("In Resume clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 137, short.MaxValue);
            }
        }

        protected override bool StartServiceTask()
        {
            try
            {
                return StartRunner();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StartServiceTask clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 138, short.MaxValue);
            }
            return false;
        }


        protected override ServiceHost CreateServiceHost()
        {
            return new ServiceHost(typeof(ClippIntergrationWebService));
        }

        protected override bool StartRunner()
        {
            bool result = false;

            try
            {
                ServiceLogger.Log(@"Starting Clipp Server endpoint ...");

                result = OpenServiceHost();

                if (result)
                {
                    ServiceLogger.Log(@"Clipp Server endpoint is running ...");

                    //TODO: Initialize the Clipp Integration Server....

                    ServiceInfo serviceInfo = ServiceInfo.Instance;

                    ServiceLogger.Log(string.Format(@"Clipp Server: Database server URL: {0}", serviceInfo.WebmateDatabaseServerPath()));
                    ServiceLogger.Log(string.Format(@"Clipp Server: Database location: {0}", serviceInfo.WebmateDatabaseLocation()));

                    ServiceLogger.Log(@"Clipp Server runner: Clipp Integration Service has started");
                }
                else
                {
                    ServiceLogger.Log(@"Clipp Server endpoint failed to run ...");
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StartRunner clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 139, short.MaxValue);
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            try
            {
                ServiceLogger.Log(@"Stopping Clipp Integration Service endpoint ...");

                //TODO: Clear the Clipp Integration Server....
                result = CloseServiceHost();

                ServiceLogger.Log(result
                    ? @"Clipp Integration Service endpoint stopped ..."
                    : @"Clipp Integration Service endpoint failed to stop ...");
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In StopRunner clipprunner", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 140, short.MaxValue);
            }

            return result;
        }
    }
}
