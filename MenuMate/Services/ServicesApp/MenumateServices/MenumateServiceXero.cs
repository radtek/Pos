using System;
using System.Linq;
using System.ServiceProcess;

using MenumateServices.MenumateRunners;

namespace MenumateServices
{
    public partial class MenumateServiceXero : ServiceBase
    {
        XeroIntegrationRunner xeroIntegrationRunner_;

        /// <summary>
        /// 
        /// </summary>
        public MenumateServiceXero()
        {
            ServiceName = ServiceNameManager.Instance.ServiceNameWithID(MenumateServicesIdentifier.XeroIntegration);
            InitializeComponent();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        protected override void OnStart(string[] args)
        {
            ServiceLogger.Log(String.Format(@"Starting {0}.", ServiceName));
            startMRunner();
            ServiceLogger.Log(String.Format(@"{0} started.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
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
            xeroIntegrationRunner_ = createMRunner(ServiceName);
            xeroIntegrationRunner_.Start();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected void stopMRunner()
        {
            xeroIntegrationRunner_.Stop();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void pauseMRunner()
        {
            xeroIntegrationRunner_.Pause();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resumeMRunner()
        {
            xeroIntegrationRunner_.Resume();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected XeroIntegrationRunner createMRunner(string inServiceName)
        {
            XeroIntegrationRunner result = new XeroIntegrationRunner();
            result.ServiceName = inServiceName;

            return result;
        }
        #endregion
    }
}
