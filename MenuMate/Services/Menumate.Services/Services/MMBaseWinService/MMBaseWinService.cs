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

using Menumate.WinServices.Tools;

namespace Menumate.WinServices
{
    public partial class MMBaseWinService: ServiceBase
    {
        protected Thread m_thread;
        protected ManualResetEvent m_shutdownEvent;
        protected TimeSpan m_delay;

        protected DateTime m_startTickCount;
        protected DateTime m_tickCount;
        protected bool m_webServicesStarted = false;

        public MMBaseWinService()
        {
            InitializeComponent();

            // create a new timespan object
            // with a default of 10 seconds delay.
            m_delay = new TimeSpan(0, 0, 0, 10, 0);

        }

        protected override void OnStart(string[] args)
        {
            ServiceLogger.Log(String.Format(@"{0} started.", ServiceName));

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
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        /// <SUMMARY>
        /// Stop this service.
        /// </SUMMARY>
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

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        /// <SUMMARY>
        /// 
        /// </SUMMARY>
        protected override void OnPause()
        {
            ServiceLogger.Log(String.Format(@"{0} paused.", ServiceName));
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        /// <SUMMARY>
        /// 
        /// </SUMMARY>
        protected override void OnContinue()
        {
            ServiceLogger.Log(String.Format(@"{0} continued.", ServiceName));
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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
                    break;

                // let's do some work
                nReturnCode = Execute();
            }
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        /// <SUMMARY>
        /// 
        /// </SUMMARY>
        /// <RETURNS></RETURNS>
        protected virtual int Execute()
        {
            if (!m_webServicesStarted)
            {
                int timeDiff = m_tickCount.CompareTo(DateTime.Now);

                if (timeDiff <= 0) 
                {
                    try
                    {
                        m_webServicesStarted = StartWebServices();

                        if (!m_webServicesStarted)
                        {
                            setTickCountInMin(1); // This make StartWebService() to be called every minute if it fails
                        }
                    }
                    catch (Exception e)
                    {
                        ServiceLogger.LogException(String.Format(@"Error starting {0} web service", ServiceName), e);
                    }
                }
            }

            return -1;
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected void setTickCountInSec(double seconds)
        {
            m_startTickCount = DateTime.Now;
            m_tickCount = m_startTickCount.AddSeconds(seconds);
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected void setTickCountInMin(double minutes)
        {
            m_startTickCount = DateTime.Now;
            m_tickCount = m_startTickCount.AddMinutes(minutes);
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected virtual bool StartWebServices() // This methods should be defined in derived classes
        {
            return false;
        }
 
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected bool LoadWebService(ServiceHost inServiceHost)
        {
            bool result = false;

            try
            {
                inServiceHost.Open();
                LogServiceInformation(inServiceHost);

                result = true;
            }
            catch (SystemException e)
            {
                ServiceLogger.LogException(String.Format("{0} Web Service failed to open.", inServiceHost.Description.Name), e);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(String.Format("{0} Web Service failed to open.", inServiceHost.Description.Name), e);
            }
            finally
            {
                //inServiceHost.Close();
            }

            return result;
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        protected void LogServiceInformation<T>(T serviceHost) where T : ServiceHost
        {
            ServiceLogger.Log(String.Format("Starting {0} Web Service ...", serviceHost.Description.Name));

            if (serviceHost.BaseAddresses.Count == 0)
            {
                ServiceLogger.Log("No base addresses defined");
            }
            else
            {
                foreach (Uri uri in serviceHost.BaseAddresses)
                {
                    ServiceLogger.Log(uri.ToString());
                }
            }

            if (serviceHost.Description.Endpoints.Count > 0)
            {
                ServiceLogger.Log("Endpoints:");

                foreach (ServiceEndpoint se in serviceHost.Description.Endpoints)
                {
                    ServiceLogger.Log(String.Format("\t{0} ({1})", se.Name, se.ListenUri));
                }
            }

            ServiceLogger.Log(String.Format("{0} Web Service started", serviceHost.Description.Name));
        }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    }
}
