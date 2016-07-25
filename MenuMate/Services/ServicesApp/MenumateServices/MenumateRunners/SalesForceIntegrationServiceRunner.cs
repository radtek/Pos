using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class SalesForceIntegrationServiceRunner : MenumateWebServiceRunner
    {
        public SalesForceIntegrationServiceRunner()
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
            return new ServiceHost(typeof(SalesForceIntegrationWebService));
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
