using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class MewsIntegrationServiceRunner : MenumateWebServiceRunner
    {
        #region PUBLIC
        public MewsIntegrationServiceRunner()
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
            return new ServiceHost(typeof(MewsIntegrationWebService));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting Mews endpoint ...");
            result = OpenServiceHost();
            if (result)
            {
                ServiceLogger.Log(@"Mews endpoint is running ...");

                // Any Mews initialization goes here!!!!!

                ServiceLogger.Log(@"Mews runner: Mews has started");
            }
            else
            {
                ServiceLogger.Log(@"Mews endpoint failed to run ...");
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

            ServiceLogger.Log(@"Stopping Mews endpoint ...");
            if (result = CloseServiceHost())
            {
                ServiceLogger.Log(@"Mews endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"Mews endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}
