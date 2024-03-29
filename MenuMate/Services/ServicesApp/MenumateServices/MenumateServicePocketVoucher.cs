﻿using System;
using System.ServiceProcess;
using MenumateServices.MenumateRunners;

namespace MenumateServices
{
    partial class MenumateServicePocketVoucher : ServiceBase
    {
        private PocketVoucherServiceRunner _pocketVoucherServiceRunner;

        public MenumateServicePocketVoucher()
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
            _pocketVoucherServiceRunner = CreateRunner(ServiceName);
            _pocketVoucherServiceRunner.Start();
        }

        protected PocketVoucherServiceRunner CreateRunner(string inServiceName)
        {
            return new PocketVoucherServiceRunner
            {
                ServiceName = inServiceName
            };
        }

        protected void StopRunner()
        {
            _pocketVoucherServiceRunner.Stop();
        }

        protected void PauseRunner()
        {
            _pocketVoucherServiceRunner.Pause();
        }

        protected void ResumeRunner()
        {
            _pocketVoucherServiceRunner.Resume();
        }
    }
}
