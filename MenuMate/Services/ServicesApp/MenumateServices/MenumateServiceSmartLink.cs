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
    partial class MenumateServiceSmartLink : ServiceBase
    {

        SmartLinkRunner _smartLinkRunner;
        public MenumateServiceSmartLink()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            ServiceLogger.Log(String.Format(@"Starting {0}.", ServiceName));
            startMRunner();
            ServiceLogger.Log(String.Format(@"{0} started.", ServiceName));
        }

        protected override void OnStop()
        {
            ServiceLogger.Log(String.Format(@"Stopping {0}.", ServiceName));
            stopMRunner();
            ServiceLogger.Log(String.Format(@"{0} stopped.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnPause()
        {
            ServiceLogger.Log(String.Format(@"Pausing {0}.", ServiceName));
            pauseMRunner();
            ServiceLogger.Log(String.Format(@"{0} paused.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnContinue()
        {
            ServiceLogger.Log(String.Format(@"Resuming {0}.", ServiceName));
            resumeMRunner();
            ServiceLogger.Log(String.Format(@"{0} resumed.", ServiceName));
        }

        #region Protected
        /// <summary>
        /// 
        /// </summary>
        protected void startMRunner()
        {
            _smartLinkRunner = createMRunner(ServiceName);
            _smartLinkRunner.Start();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected void stopMRunner()
        {
            _smartLinkRunner.Stop();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void pauseMRunner()
        {
            _smartLinkRunner.Pause();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resumeMRunner()
        {
            _smartLinkRunner.Resume();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected SmartLinkRunner createMRunner(string inServiceName)
        {
            SmartLinkRunner result = new SmartLinkRunner();
            result.ServiceName = inServiceName;
            return result;
        }
        #endregion
    }
}
