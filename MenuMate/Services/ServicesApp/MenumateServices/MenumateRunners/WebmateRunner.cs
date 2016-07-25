using System.ServiceModel;

using MenumateServices.WCFServices;
using MenumateServices.WebMate.InternalClasses;

namespace MenumateServices.MenumateRunners
{
    public class WebmateRunner : MenumateWebServiceRunner
    {
        public WebmateRunner()
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
            return new ServiceHost(typeof(WCFServiceWebMate));
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
