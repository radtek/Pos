using System;
using System.Collections.Generic;
using System.Linq;

using System.ServiceModel;

using MenumateServices.WCFServices;
using MenumateServices.WebMate.InternalClasses;

namespace MenumateServices.MenumateRunners
{
    public class ChefmateRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
        public ChefmateRunner()
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
            return new ServiceHost(typeof(WCFServiceChefMate));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Chefmate endpoint ...");
            if (result = OpenServiceHost())
            {
                ServiceLogger.Log(@"Chefmate endpoint is running ...");

                ServiceInfo serviceInfo = ServiceInfo.Instance;

                ServiceLogger.Log(string.Format(@"Chefmate: Database server URL: {0}", serviceInfo.ChefmateDatabaseServerPath()));
                ServiceLogger.Log(string.Format(@"Chefmate: Database location: {0}",   serviceInfo.ChefmateDatabaseLocation()));

                // Any Chefmate initialization goes here!!!!!

                ServiceLogger.Log(@"Chefmate runner: Chefmate has started");
            }
            else
            {
                ServiceLogger.Log(@"Chefmate endpoint failed to run ...");
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

            ServiceLogger.Log(@"Stopping ChefMate endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"ChefMate endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"ChefMate endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}
