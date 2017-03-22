using System.ServiceModel;
using System;
using System.Diagnostics;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class PocketVoucherServiceRunner : MenumateWebServiceRunner
    {
        public PocketVoucherServiceRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In PocketVoucherServiceRunner PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 153, short.MaxValue);
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
                EventLog.WriteEntry("In Start PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 154, short.MaxValue);
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
                EventLog.WriteEntry("In Start PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 155, short.MaxValue);
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
                EventLog.WriteEntry("In Pause PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 156, short.MaxValue);
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
                EventLog.WriteEntry("In Resume PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 157, short.MaxValue);
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
                EventLog.WriteEntry("In StartServiceTask PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 158, short.MaxValue);
            }
            return false;
        }


        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(PocketVoucherWebService));
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In StartServiceTask PVRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 159, short.MaxValue);
            }
            return null;
        }

        protected override bool StartRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Starting Pocket Voucher Server endpoint ...");

            result = OpenServiceHost();

            if (result)
            {
                ServiceLogger.Log(@"Pocket Voucher Server endpoint is running ...");

                //TODO: Initialize the Pocket Voucher Server....

                ServiceInfo serviceInfo = ServiceInfo.Instance;

                ServiceLogger.Log(string.Format(@"Pocket Voucher Server: Database server URL: {0}", serviceInfo.WebmateDatabaseServerPath()));
                ServiceLogger.Log(string.Format(@"Pocket Voucher Server: Database location: {0}", serviceInfo.WebmateDatabaseLocation()));

                ServiceLogger.Log(@"Pocket Voucher Server runner: Pocket Voucher Service has started");
            }
            else
            {
                ServiceLogger.Log(@"Pocket Voucher Server endpoint failed to run ...");
            }

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Stopping Pocket Voucher Service endpoint ...");

            //TODO: Clear the Pocket Voucher Server....
            result = CloseServiceHost();

            ServiceLogger.Log(result
                ? @"Pocket Voucher Service endpoint stopped ..."
                : @"Pocket Voucher Service endpoint failed to stop ...");

            return result;
        }
    }
}