using System;
using System.Collections.Generic;
using System.Linq;

using System.ServiceModel;

using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class LoyaltymateRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
        public LoyaltymateRunner()
        {
            InitializeRunner();
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Start()
        {
            // go ahead and start the worker thread
            WorkerThread.Start();

            Paused = false;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Stop()
        {
            StopRunner();

            Paused = true;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Pause()
        {
            StopRunner();

            Paused = true;
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Resume()
        {
            InitializeRunner();

            Start();
        }
        #endregion

        #region PROTECTED
        /// <summary>
        /// This method is called from the thread's Execute method
        /// in the base class MenumateRunner 
        /// </summary>
        /// <returns></returns>
        protected override bool StartServiceTask()
        {
            return StartRunner();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected override ServiceHost CreateServiceHost()
        {
            return new ServiceHost(typeof(WCFServiceLoyaltyMate));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Loyaltymate endpoint ...");
            if (result = OpenServiceHost())
            {
                ServiceLogger.Log(@"Loyaltymate endpoint is running ...");

                // Any Chefmate initialization goes here!!!!!

                ServiceLogger.Log(@"Loyaltymate runner: Loyaltymate has started");
            }
            else
            {
                ServiceLogger.Log(@"Loyaltymate endpoint failed to run ...");
            }

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StopRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Stopping LoyaltyMate endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"LoyaltyMate endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"LoyaltyMate endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}
