using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;

using System.ServiceModel;
//using System.ServiceModel.Description;

using MenumateServices.WCFServices;


using MenumateServices.MenumateRunners;

namespace MenumateServices.Tools
{
    //public delegate bool StartServiceTask();

    public class MenumateServiceRunner : IMenumateRunner
    {
        MenumateServiceHostRunner serviceHostRunner_;


        public string ServiceName;

        bool paused_ = true;
        bool failedToStart_ = true;

        Thread mThread_;
        ManualResetEvent mShutdownEvent_;
        TimeSpan mDelay_;

        DateTime mStartTickCount_;
        DateTime mTickCount_;

        /// <summary>
        /// 
        /// </summary>
        public MenumateServiceRunner()
        {
            initRunner();
        }

        /// <summary>
        /// 
        /// </summary>
        public void Start()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public void Stop()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public void Pause()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public void Resume()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public bool Paused
        {
            set { paused_ = value; }
            get { return paused_; }
        }

        #region Protected
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inThreadStart"></param>
        protected void initRunner()
        {
            ServiceName = "Menumate's Service";

            // create a new timespan object
            // with a default of 10 seconds delay.
            mDelay_ = new TimeSpan(0, 0, 0, 10, 0);

            ThreadStart ts = new ThreadStart(serviceMain);

            // create the manual reset event and
            // set it to an initial state of unsignaled
            mShutdownEvent_ = new ManualResetEvent(false);

            // create the worker thread
            mThread_ = new Thread(ts);

            // go ahead and start the worker thread
            mThread_.Start();

            paused_ = false;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void serviceMain()
        {
            bool signaled = false;
            int  returnCode = 0;

            while(true)
            {
                // wait for the event to be signaled
                // or for the configured delay
                signaled = mShutdownEvent_.WaitOne(mDelay_, true);

                // if we were signaled to shutdown, exit the loop
                if(signaled) break;

                // let's do some work
                returnCode = Execute();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected virtual int Execute()
        {
            if(!paused_ && failedToStart_)
            {
                int timeDiff = mTickCount_.CompareTo(DateTime.Now);

                if (timeDiff <= 0)
                {
                    try
                    {
                        failedToStart_ = !startServiceTask();

                        if(failedToStart_)
                        {
                            setTickCountInMin(15); // This make startService() to be called every minute if it fails
                        }
                    }
                    catch (Exception e)
                    {
                        ServiceLogger.LogException(String.Format(@"Error starting {0} service", ServiceName), e);
                    }
                }
            }

            return -1;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="seconds"></param>
        protected void setTickCountInSec(double seconds)
        {
            mStartTickCount_ = DateTime.Now;
            mTickCount_ = mStartTickCount_.AddSeconds(seconds);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="minutes"></param>
        protected void setTickCountInMin(double minutes)
        {
            mStartTickCount_ = DateTime.Now;
            mTickCount_ = mStartTickCount_.AddMinutes(minutes);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool startServiceTask()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"WebMate endpoint is starting ...");
            if (result = runWebMate())
            {
                ServiceLogger.Log(@"WebMate endpoint is running ...");

                runWebMateWatcher();
            }
            else
            {
                ServiceLogger.Log(@"WebMate endpoint failed to run ...");
            }

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool stopServiceTask()
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            ServiceLogger.Log(@"Stopping WebMate ...");
            if (result = pauseWebMate())
            {
                ServiceLogger.Log(@"WebMate has stopped ...");

                pauseWebMateWatcher();
            }
            else
            {
                ServiceLogger.Log(@"WebMate failed to stop in shared folder mode ...");
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool pauseServiceTask()
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            ServiceLogger.Log(@"Pausing WebMate ...");
            if (result = pauseWebMate())
            {
                ServiceLogger.Log(@"WebMate has paused ...");

                pauseWebMateWatcher();
            }
            else
            {
                ServiceLogger.Log(@"WebMate failed to pause in shared folder mode ...");
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool resumeServiceTask()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Resuming WebMate ...");
            if (result = resumeWebMate())
            {
                ServiceLogger.Log(@"WebMate has resumed ...");

                resumeWebMateWatcher();
            }
            else
            {
                ServiceLogger.Log(@"WebMate failed to resume in shared folder mode ...");
            }

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        bool runWebMate()
        {
            bool result = false;

            //...........................................

            result = openServiceHost();

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool stopWebMate()
        {
            bool result = false;

            //...........................................

            result = stopServiceHost() && (this.Paused = true);

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool pauseWebMate()
        {
            bool result = false;

            //...........................................

            result = pauseServiceHost() && (this.Paused = true);

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool resumeWebMate()
        {
            bool result = false;

            //...........................................

            result = resumeServiceHost() && !(this.Paused = false);

            //...........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool openServiceHost()
        {
            bool result = false;

            try
            {
                serviceHostRunner_ = new MenumateServiceHostRunner(createServiceHost());
                serviceHostRunner_.Open();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Service Host Service failed to open: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool stopServiceHost()
        {
            bool result = false;

            try
            {
                serviceHostRunner_.Stop();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Service Host Service failed to stop: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool pauseServiceHost()
        {
            bool result = false;

            try
            {
                serviceHostRunner_.Pause();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Service Host Service failed to pause: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool resumeServiceHost()
        {
            bool result = false;

            try
            {
                serviceHostRunner_.Open();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Service Host Service failed to resume: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        ServiceHost createServiceHost()
        {
            return new ServiceHost(typeof(WCFServiceWebMate));
        }

        /// <summary>
        /// 
        /// </summary>
        bool runWebMateWatcher()
        {
            bool result = false;

            try
            {
                result = openWebMateFolderWatcher();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Webmate watcher failed to open: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool stopWebMateWatcher()
        {
            bool result = false;

            try
            {
                webOrderFileSupplier_.Stop();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Webmate watcher failed to stop: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool pauseWebMateWatcher()
        {
            bool result = false;

            try
            {
                webOrderFileSupplier_.Stop();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Webmate watcher failed to stop: {1}", @"Web Mate", e.Message));
            }

            return result; 
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool resumeWebMateWatcher()
        {
            bool result = false;

            try
            {
                webOrderFileSupplier_.Start();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Webmate watcher failed to stop: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        bool openWebMateFolderWatcher()
        {
            bool result = false;

            try
            {
                List<string> folderList = readWatchedFolders(webOrderXMLManager_.CompleteOrderRootURL);

                webOrderFileSupplier_.AddWatchedFolders(folderList.ToArray());
                webOrderFileSupplier_.Start();

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.Log(String.Format("{0} Web order file supplier failed to open: {1}", @"Web Mate", e.Message));
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        List<string> readWatchedFolders(string inRootFolder)
        {
            List<string> result = new List<string>();

            //...............................................

            System.IO.DirectoryInfo dir = new System.IO.DirectoryInfo(inRootFolder);

            foreach (System.IO.DirectoryInfo g in dir.GetDirectories())
            {
                result.Add(g.FullName);
            }

            //...............................................

            return result;
        }
        #endregion
    }
}
