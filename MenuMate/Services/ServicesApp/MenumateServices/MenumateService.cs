using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;

using System.Threading;

using System.ServiceModel;
using System.ServiceModel.Description;

namespace MenumateServices
{
    /// <summary>
    /// Base Class: Implements a MenumateService. It tries to launch its main thread's task until it succeeds
    /// startServiceTask() is a abstract method that must be implemented in every directly derived class
    /// </summary>
    public partial class MenumateService : ServiceBase
    {
        //AQUI
        protected bool _failedToStart = true;

        protected Thread m_thread;
        protected ManualResetEvent m_shutdownEvent;
        protected TimeSpan m_delay;

        protected DateTime m_startTickCount;
        protected DateTime m_tickCount;

        /// <summary>
        /// 
        /// </summary>
        public MenumateService()
        {
            //AQUI
            ServiceLogger.Log(String.Format(@"Creating {0}.", @"MenumateService"));

            InitializeComponent();

            // create a new timespan object
            // with a default of 10 seconds delay.
            m_delay = new TimeSpan(0, 0, 0, 10, 0);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <returns></returns>
        protected string serviceNameWithID(MenumateServicesID inID)
        {
            return ServiceNameManager.Instance.ServiceNameWithID(inID);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        protected override void OnStart(string[] args)
        {
            //AQUI
            ServiceLogger.Log(String.Format(@"Starting {0}.", ServiceName));

            setTickCountInSec(0.001); // 1 milisec

            // create our threadstart object to wrap our delegate method
            ThreadStart ts = new ThreadStart(this.ServiceMain);

            // create the manual reset event and
            // set it to an initial state of unsignaled
            m_shutdownEvent = new ManualResetEvent(false);

            // create the worker thread
            m_thread = new Thread(ts);

            // go ahead and start the worker thread
            m_thread.Start();

            // call the base class so it has a chance
            // to perform any work it needs to
            base.OnStart(args);

            ServiceLogger.Log(String.Format(@"{0} started.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnStop()
        {
            // signal the event to shutdown
            m_shutdownEvent.Set();

            // wait for the thread to stop giving it 10 seconds
            m_thread.Join(10000);

            // call the base class 
            base.OnStop();

            ServiceLogger.Log(String.Format(@"{0} stopped.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnPause()
        {
            ServiceLogger.Log(String.Format(@"{0} paused.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnContinue()
        {
            ServiceLogger.Log(String.Format(@"{0} continued.", ServiceName));
        }

        /// <summary>
        /// 
        /// </summary>
        protected void ServiceMain()
        {
            bool bSignaled = false;
            int nReturnCode = 0;

            while (true)
            {
                // wait for the event to be signaled
                // or for the configured delay
                bSignaled = m_shutdownEvent.WaitOne(m_delay, true);

                // if we were signaled to shutdown, exit the loop
                if (bSignaled == true)
                {
                    break;
                }

                // let's do some work
                nReturnCode = Execute();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected virtual int Execute()
        {
            if( _failedToStart )
            {
                int timeDiff = m_tickCount.CompareTo(DateTime.Now);

                if (timeDiff <= 0)
                {
                    try
                    {
                        _failedToStart = !startServiceTask();

                        if(_failedToStart)
                        {
                            setTickCountInMin(1); // This make startService() to be called every minute if it fails
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
        ///  Virtual method: This method must be implemented in every directly derived class
        /// </summary>
        /// <returns></returns>
        protected virtual bool startServiceTask()
        {
            //AQUI
            ServiceLogger.Log(String.Format(@"virtual startServiceTask {0}.", @"MenumateService"));

            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="seconds"></param>
        protected void setTickCountInSec(double seconds)
        {
            m_startTickCount = DateTime.Now;
            m_tickCount = m_startTickCount.AddSeconds(seconds);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="minutes"></param>
        protected void setTickCountInMin(double minutes)
        {
            m_startTickCount = DateTime.Now;
            m_tickCount = m_startTickCount.AddMinutes(minutes);
        }
    }
}
