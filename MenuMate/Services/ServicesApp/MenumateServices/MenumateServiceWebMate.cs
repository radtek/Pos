using System;
using System.ServiceProcess;
using MenumateServices.MenumateRunners;

namespace MenumateServices
{
    partial class MenumateServiceWebMate : ServiceBase
    {
        private WebmateRunner _webmateRunner;

        public MenumateServiceWebMate()
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
            _webmateRunner = CreateMRunner(ServiceName);
            _webmateRunner.Start();
        }

        protected void StopRunner()
        {
            _webmateRunner.Stop();
        }

        protected void PauseRunner()
        {
            _webmateRunner.Pause();
        }

        protected void ResumeRunner()
        {
            _webmateRunner.Resume();
        }

        protected WebmateRunner CreateMRunner(string inServiceName)
        {
            return new WebmateRunner
            {
                ServiceName = inServiceName
            };
        }

    }
}
