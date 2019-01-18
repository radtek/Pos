using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class RegistrationServiceRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
        public RegistrationServiceRunner()
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
            return new ServiceHost(typeof(RegistrationIntegrationWebService));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Registration endpoint ...");
            result = OpenServiceHost();
            if (result)
            {
                ServiceLogger.Log(@"Registration endpoint is running ...");

                // Any SiHot initialization goes here!!!!!

                ServiceLogger.Log(@"Registration runner: Registration Service has started");
            }
            else
            {
                ServiceLogger.Log(@"Registration service endpoint failed to run ...");
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

            ServiceLogger.Log(@"Stopping Registration endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"Registration service stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"Registration service endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}
