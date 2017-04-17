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
    partial class MenumateServiceWalletPayments : ServiceBase
    {
        WalletPaymentsServiceRunner _walletPaymentsServiceRunner;


        public MenumateServiceWalletPayments()
        {
            ServiceName = ServiceNameManager.Instance.ServiceNameWithID(MenumateServicesIdentifier.WalletPayments);
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
            _walletPaymentsServiceRunner = createMRunner(ServiceName);
            _walletPaymentsServiceRunner.Start();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected void stopMRunner()
        {
            _walletPaymentsServiceRunner.Stop();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void pauseMRunner()
        {
            _walletPaymentsServiceRunner.Pause();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resumeMRunner()
        {
            _walletPaymentsServiceRunner.Resume();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected WalletPaymentsServiceRunner createMRunner(string inServiceName)
        {
            WalletPaymentsServiceRunner result = new WalletPaymentsServiceRunner();
            result.ServiceName = inServiceName;

            return result;
        }
        #endregion

    }
}
