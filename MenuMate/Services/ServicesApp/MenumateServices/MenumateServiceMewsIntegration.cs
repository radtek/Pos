using System;
using System.ComponentModel;
using MenumateServices.MenumateRunners;
using System.ServiceProcess;

namespace MenumateServices
{
    public partial class MenumateServiceMewsIntegration : ServiceBase
    {
        private MewsIntegrationServiceRunner _mewsIntegrationServiceRunner;
        public MenumateServiceMewsIntegration()
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
            _mewsIntegrationServiceRunner = CreateRunner(ServiceName);
            _mewsIntegrationServiceRunner.Start();
        }

        protected MewsIntegrationServiceRunner CreateRunner(string inServiceName)
        {
            return new MewsIntegrationServiceRunner
            {
                ServiceName = inServiceName
            };
        }

        protected void StopRunner()
        {
            _mewsIntegrationServiceRunner.Stop();
        }

        protected void PauseRunner()
        {
            _mewsIntegrationServiceRunner.Pause();
        }

        protected void ResumeRunner()
        {
            _mewsIntegrationServiceRunner.Resume();
        }
    }
}
