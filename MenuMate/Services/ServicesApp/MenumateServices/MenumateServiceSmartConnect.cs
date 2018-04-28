using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using MenumateServices.MenumateRunners;

namespace MenumateServices
{
    partial class MenumateServiceSmartConnect : ServiceBase
    {

        SmartConnectRunner _smartConnectRunner;
        public MenumateServiceSmartConnect()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            ServiceLogger.Log(String.Format(@"Starting {0}.", ServiceName));
            startRunner();
            ServiceLogger.Log(String.Format(@"{0} started.", ServiceName));
        }

        protected override void OnStop()
        {
            ServiceLogger.Log(String.Format(@"Stopping {0}.", ServiceName));
            stopRunner();
            ServiceLogger.Log(String.Format(@"{0} stopped.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnPause()
        {
            ServiceLogger.Log(String.Format(@"Pausing {0}.", ServiceName));
            pauseRunner();
            ServiceLogger.Log(String.Format(@"{0} paused.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnContinue()
        {
            ServiceLogger.Log(String.Format(@"Resuming {0}.", ServiceName));
            resumeRunner();
            ServiceLogger.Log(String.Format(@"{0} resumed.", ServiceName));
        }

        #region Protected
        /// <summary>
        /// 
        /// </summary>
        protected void startRunner()
        {
            _smartConnectRunner = createRunner(ServiceName);
            _smartConnectRunner.Start();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected void stopRunner()
        {
            _smartConnectRunner.Stop();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void pauseRunner()
        {
            _smartConnectRunner.Pause();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resumeRunner()
        {
            _smartConnectRunner.Resume();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected SmartConnectRunner createRunner(string inServiceName)
        {
            SmartConnectRunner result = new SmartConnectRunner();
            result.ServiceName = inServiceName;
            return result;
        }
        #endregion
    }
}

