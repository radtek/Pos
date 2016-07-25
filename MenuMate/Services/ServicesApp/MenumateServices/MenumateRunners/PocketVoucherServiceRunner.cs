using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class PocketVoucherServiceRunner : MenumateWebServiceRunner
    {
        public PocketVoucherServiceRunner()
        {
            InitializeRunner();
        }

        public override void Start()
        {
            WorkerThread.Start();
            Paused = false;
        }

        public override void Stop()
        {
            StopRunner();
            Paused = true;
        }

        public override void Pause()
        {
            StopRunner();
            Paused = true;
        }

        public override void Resume()
        {
            InitializeRunner();
            Start();
        }

        protected override bool StartServiceTask()
        {
            return StartRunner();
        }

        
        protected override ServiceHost CreateServiceHost()
        {
            return new ServiceHost(typeof(PocketVoucherWebService));
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