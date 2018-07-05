using System.ServiceModel;
using System;
using System.Diagnostics;
using MenumateServices.WCFServices;

namespace MenumateServices.MenumateRunners
{
    public class PaymentSenseRunner : MenumateWebServiceRunner
    {
        #region PUBLIC

        public PaymentSenseRunner()
        {
            try
            {
                InitializeRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In PaymentSenseRunner PaymentSenseRunner", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 167, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Start()
        {
            try
            {
                WorkerThread.Start();
                Paused = false;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Start Start", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 168, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Stop()
        {
            try
            {
                StopRunner();
                Paused = true;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Stop Start", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 169, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Pause()
        {
            try
            {
                StopRunner();
                Paused = true;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Pause Start", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 170, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Resume()
        {
            try
            {
                InitializeRunner();
                Start();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In Resume Start", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 171, short.MaxValue);
            }
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
            try
            {
                return StartRunner();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In StartServiceTask Start", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 172, short.MaxValue);
            }
            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected override ServiceHost CreateServiceHost()
        {
            try
            {
                return new ServiceHost(typeof(WCFServicePaymentSense));
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("In CreateServiceHost Start", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 173, short.MaxValue);
            }
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        protected override bool StartRunner()
        {
            bool result = false;

            //...........................................

            ServiceLogger.Log(@"Starting PaymentSense endpoint ...");
            result = OpenServiceHost();
            if (result)
            {
                ServiceLogger.Log(@"PaymentSense endpoint is running ...");
                ServiceInfo serviceInfo = ServiceInfo.Instance;
                ServiceLogger.Log(string.Format(@"PaymentSense: Server IpAddress: {0}", "127.0.0.1"));
                ServiceLogger.Log(string.Format(@"PaymentSense: Server Port: {0}", "11234"));
                // Any Chefmate initialization goes here!!!!!
                ServiceLogger.Log(@"PaymentSense runner: PaymentSense has started");
            }
            else
            {
                ServiceLogger.Log(@"PaymentSense endpoint failed to run ...");
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

            ServiceLogger.Log(@"Stopping PaymentSense endpoint ...");
            result = CloseServiceHost();
            if (result)
            {
                ServiceLogger.Log(@"PaymentSense endpoint stopped ...");
            }
            else
            {
                ServiceLogger.Log(@"PaymentSense endpoint failed to stop ...");
            }

            //...........................................

            return result;
        }
        #endregion
    }
}