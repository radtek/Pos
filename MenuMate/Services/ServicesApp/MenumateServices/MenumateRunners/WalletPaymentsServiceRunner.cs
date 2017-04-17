using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class WalletPaymentsServiceRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
    
        public WalletPaymentsServiceRunner()
        {
            InitializeRunner();
        }

        public override void Start()
        {
            // go ahead and start the worker thread
            WorkerThread.Start();

            Paused = false;
        }

        public override void Stop()
        {
            StopRunner();
            Paused = true;
        }

        public override void Pause()
        {
            StopRunner();
            Paused = true;
        }


        public override void Resume()
        {
            InitializeRunner();
            Start();
        }
 
        #endregion

        #region PROTECTED

        protected override bool StartServiceTask()
        {
            return StartRunner();
        }

        protected override ServiceHost CreateServiceHost()
        {
            return new ServiceHost(typeof(WCFServiceWalletPayments));
        }

        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Wallet Payments Service endpoint ...");
            if (result = OpenServiceHost())
            {
                ServiceLogger.Log(@"Wallet Payments Service endpoint is running ...");

                // Any Chefmate initialization goes here!!!!!

                ServiceLogger.Log(@"Wallet Payments Service runner: Loyaltymate has started");
            }
            else
            {
                ServiceLogger.Log(@"Wallet Payments Service endpoint failed to run ...");
            }

            //...........................................

            return result;
        }
 
        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Stopping Wallet Payments Service endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"Wallet Payments Service endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"Wallet Payments Service endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
  
        #endregion
    }
}
