using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using MenumateServices.WCFServices;
using Thorlink.Net;

namespace MenumateServices.MenumateRunners
{
    public class ThorlinkServiceRunner: MenumateWebServiceRunner
    {
        public ThorlinkServiceRunner()
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
            var thorPingTimer = new ThorPingTimer();
            return StartRunner();
        }

        
        protected override ServiceHost CreateServiceHost()
        {
            return new ServiceHost(typeof(WCFServiceThorlink));
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
