using System;
using System.Threading;

namespace MenumateServices.MenumateRunners
{
    public abstract class MenumateRunner : IMenumateRunner
    {
        public string ServiceName;

        protected bool Paused = true;
        protected bool FailedToStart = true;

        protected Thread WorkerThread;
        protected ManualResetEvent ShutdownEvent;
        protected TimeSpan Delay;

        protected DateTime StartTickCount;
        protected DateTime TickCount;

        protected void InitializeRunner()
        {
            // create a new timespan object
            // with a default of 10 seconds delay.
            Delay = new TimeSpan(0, 0, 0, 10, 0);
            var ts = new ThreadStart(StartServiceThread);

            // create the manual reset event and
            // set it to an initial state of unsignaled
            ShutdownEvent = new ManualResetEvent(false);

            // create the worker thread
            WorkerThread = new Thread(ts);
        }

        protected void StartServiceThread()
        {
            while (true)
            {
                // wait for the event to be signaled
                // or for the configured delay
                bool signaled = ShutdownEvent.WaitOne(Delay, true);

                // if we were signaled to shutdown, exit the loop
                if (signaled) 
                    break;

                // let's do some work
                Execute();
            }
        }

        protected virtual int Execute()
        {
            if (!Paused && FailedToStart)
            {
                int timeDiff = TickCount.CompareTo(DateTime.Now);

                if (timeDiff <= 0)
                {
                    try
                    {
                        FailedToStart = !StartServiceTask();

                        if (FailedToStart)
                        {
                            SetTickCountInMinutes(15); 
                            // This make startService() to be called every minute if it fails
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

        protected void SetTickCountInSeconds(double seconds)
        {
            StartTickCount = DateTime.Now;
            TickCount = StartTickCount.AddSeconds(seconds);
        }

        protected void SetTickCountInMinutes(double minutes)
        {
            StartTickCount = DateTime.Now;
            TickCount = StartTickCount.AddMinutes(minutes);
        }

        public abstract void Start();
        public abstract void Stop();
        public abstract void Pause();
        public abstract void Resume();

        protected abstract bool StartServiceTask();
        protected abstract bool StartRunner();
        protected abstract bool StopRunner();
    }
}
