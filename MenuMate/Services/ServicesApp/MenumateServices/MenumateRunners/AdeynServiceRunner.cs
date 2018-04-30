using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class AdyenServiceRunner: MenumateWebServiceRunner
    {
        public AdyenServiceRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In AdyenServiceRunner AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 153, short.MaxValue);
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
                EventLog.WriteEntry("In Start AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 154, short.MaxValue);
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
                EventLog.WriteEntry("In Stop AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 155, short.MaxValue);
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
                EventLog.WriteEntry("In Pause AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 156, short.MaxValue);
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
                EventLog.WriteEntry("In Resume AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 157, short.MaxValue);
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
                EventLog.WriteEntry("In StartServiceTask AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 158, short.MaxValue);
            }
            return false;
        }


        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(AdyenIntegrationWebService));
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In StartServiceTask AdyenRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 159, short.MaxValue);
            }
            return null;
        }

        protected override bool StartRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Starting AdyenRunner Server endpoint ...");

            result = OpenServiceHost();

            if (result)
            {
                ServiceLogger.Log(@"AdyenRunner Server endpoint is running ...");

                //TODO: Initialize the Pocket Voucher Server....

                ServiceInfo serviceInfo = ServiceInfo.Instance;

                //ServiceLogger.Log(string.Format(@"AdyenRunner Server: Database server URL: {0}", serviceInfo.WebmateDatabaseServerPath()));
                //ServiceLogger.Log(string.Format(@"AdyenRunner Server: Database location: {0}", serviceInfo.WebmateDatabaseLocation()));

                ServiceLogger.Log(@"Pocket Voucher Server runner: Pocket Voucher Service has started");
            }
            else
            {
                ServiceLogger.Log(@"AdyenRunner Server endpoint failed to run ...");
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Stopping AdyenRunner Service endpoint ...");

            //TODO: Clear the Adyen Server....
            result = CloseServiceHost();

            ServiceLogger.Log(result
                ? @"AdyenRunner Service endpoint stopped ..."
                : @"AdyenRunner Service endpoint failed to stop ...");

            return result;
        }
    }
}
