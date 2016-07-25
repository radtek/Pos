using System;
using System.ServiceProcess;

using MenumateServices.MenumateRunners;

namespace MenumateServices
{
    partial class MenumateServiceChefMate : ServiceBase
    {
        ChefmateRunner chefmateRunner_;

        /// <summary>
        /// 
        /// </summary>
        public MenumateServiceChefMate()
        {
            //ServiceName = ServiceNameManager.Instance.ServiceNameWithID(MenumateServicesID.WebMate);
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
            chefmateRunner_ = createMRunner(ServiceName);
            chefmateRunner_.Start();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected void stopMRunner()
        {
            chefmateRunner_.Stop();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void pauseMRunner()
        {
            chefmateRunner_.Pause();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resumeMRunner()
        {
            chefmateRunner_.Resume();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected ChefmateRunner createMRunner(string inServiceName)
        {
            ChefmateRunner result = new ChefmateRunner();
            result.ServiceName = inServiceName;

            return result;
        }
        #endregion
    }
}
