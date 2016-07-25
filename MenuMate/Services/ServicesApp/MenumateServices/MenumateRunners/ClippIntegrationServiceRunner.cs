using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class ClippIntegrationServiceRunner : MenumateWebServiceRunner
    {
        public ClippIntegrationServiceRunner()
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
            return new ServiceHost(typeof(ClippIntergrationWebService));
        }

        protected override bool StartRunner()
        {
            bool result = false;

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

            return result;
        }

        protected override bool StopRunner()
        {
            bool result = false;

            ServiceLogger.Log(@"Stopping Clipp Integration Service endpoint ...");

            //TODO: Clear the Clipp Integration Server....
            result = CloseServiceHost();

            ServiceLogger.Log(result
                ? @"Clipp Integration Service endpoint stopped ..."
                : @"Clipp Integration Service endpoint failed to stop ...");

            return result;
        }
    }
}
