using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.ServiceProcess;
using MenumateServices.MenumateRunners;

namespace MenumateServices
{
    public partial class MenumateServiceAdyenIntegration : ServiceBase
    {
        private AdyenServiceRunner _adyenServiceRunner;

        public MenumateServiceAdyenIntegration()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            ServiceLogger.Log(String.Format(@"Starting {0}.", ServiceName));
            StartRunner();
            ServiceLogger.Log(String.Format(@"{0} started.", ServiceName));
        }

        protected override void OnStop()
        {
            ServiceLogger.Log(String.Format(@"Stopping {0}.", ServiceName));
            StopRunner();
            ServiceLogger.Log(String.Format(@"{0} stopped.", ServiceName));
        }

        protected override void OnPause()
        {
            ServiceLogger.Log(String.Format(@"Pausing {0}.", ServiceName));
            PauseRunner();
            ServiceLogger.Log(String.Format(@"{0} paused.", ServiceName));
        }

        protected override void OnContinue()
        {
            ServiceLogger.Log(String.Format(@"Resuming {0}.", ServiceName));
            ResumeRunner();
            ServiceLogger.Log(String.Format(@"{0} resumed.", ServiceName));
        }

        protected void StartRunner()
        {
            _adyenServiceRunner = CreateRunner(ServiceName);
            _adyenServiceRunner.Start();
        }

        protected AdyenServiceRunner CreateRunner(string inServiceName)
        {
            return new AdyenServiceRunner
            {
                ServiceName = inServiceName
            };
        }

        protected void StopRunner()
        {
            _adyenServiceRunner.Stop();
        }

        protected void PauseRunner()
        {
            _adyenServiceRunner.Pause();
        }

        protected void ResumeRunner()
        {
            _adyenServiceRunner.Resume();
        }
    }
}
