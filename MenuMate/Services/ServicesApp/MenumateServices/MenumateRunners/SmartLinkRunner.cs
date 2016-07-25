using System.ServiceModel;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class SmartLinkRunner : MenumateWebServiceRunner
    {
          #region PUBLIC

        public SmartLinkRunner()
        {
            InitializeRunner();
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Start()
        {
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
            return new ServiceHost(typeof(WCFServiceSmartLink));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting SmartLink endpoint ...");
            result = OpenServiceHost();
            if (result)
            {
                ServiceLogger.Log(@"SmartLink endpoint is running ...");
                ServiceInfo serviceInfo = ServiceInfo.Instance;
                ServiceLogger.Log(string.Format(@"SmartLink: Server IpAddress: {0}", "127.0.0.1"));
                ServiceLogger.Log(string.Format(@"SmartLink: Server Port: {0}", "11234"));
                // Any Chefmate initialization goes here!!!!!
                ServiceLogger.Log(@"SmartLink runner: Chefmate has started");
            }
            else
            {
                ServiceLogger.Log(@"SmartLink endpoint failed to run ...");
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

            ServiceLogger.Log(@"Stopping SmartLink endpoint ...");
            result = CloseServiceHost();
            if (result)
            {
                ServiceLogger.Log(@"SmartLink endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"SmartLink endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}
