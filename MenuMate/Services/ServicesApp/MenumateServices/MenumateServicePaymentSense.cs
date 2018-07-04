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
    public partial class MenumateServicePaymentSense : ServiceBase
    {
        PaymentSenseRunner _paymentSenseRunner;
        public MenumateServicePaymentSense()
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

        public MenumateServicePaymentSense(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }

        #region Protected
        /// <summary>
        /// 
        /// </summary>
        protected void startRunner()
        {
            _paymentSenseRunner = createRunner(ServiceName);
            _paymentSenseRunner.Start();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected void stopRunner()
        {
            _paymentSenseRunner.Stop();
        }

         /// <summary>
        /// 
        /// </summary>
        protected void pauseRunner()
        {
            _paymentSenseRunner.Pause();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void resumeRunner()
        {
            _paymentSenseRunner.Resume();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected PaymentSenseRunner createRunner(string inServiceName)
        {
            PaymentSenseRunner result = new PaymentSenseRunner();
            result.ServiceName = inServiceName;
            return result;
        }
        #endregion
    }
}


