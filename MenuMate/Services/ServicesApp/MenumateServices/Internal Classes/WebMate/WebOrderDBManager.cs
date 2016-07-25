using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;
using System.Timers;

namespace MenumateServices.WebMate.InternalClasses
{
    public class WebOrderDBManager
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        public WebOrderDBManager()
        {
            process_threads      = new Dictionary<WebOrderDBAccessProcess, Thread>();
            process_threads_poll = new Dictionary<WebOrderDBAccessProcess, Thread>();

            dead_lock_paused = false;

            initDeadLockPausedTimer();
            initTimeOutTimer();
            initPollTimer();

            max_concurrent_connections = 1;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public void Done()
        {
            process_time_out_timer.Enabled = false;
            process_dead_lock_paused_timer.Enabled = false;
            process_poll_timer.Enabled     = false;

            process_threads.Clear();
            process_threads_poll.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        public bool DeadLockPaused
        {
            get { return dead_lock_paused; }
        }

        /// <summary>
        /// 
        /// </summary>
        public void SetDeadLockPause()
        {
            dead_lock_paused = true;

            process_dead_lock_paused_timer.Interval = 60000; // 1 min
            process_dead_lock_paused_timer.Enabled = true;        
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileURI"></param>
        /// <returns></returns>
        public WebOrderDBAccessProcess AddProcess(out bool outAdded)
        {
            WebOrderDBAccessProcess result = new WebOrderDBAccessProcess();

            //::::::::::::::::::::::::::::::::::::::::::

            outAdded = false;

            if (!tooManyProcesses())
            {
                Thread thread = createProcessThread(result);

                process_threads.Add(result, thread);
                outAdded = true;
            }

            //::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProcess"></param>
        public void AddProcessToPoll(WebOrderDBAccessProcess inProcess)
        {
            Thread thread = createProcessThread(inProcess);
    
            process_threads_poll.Add(inProcess, thread);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProcess"></param>
        public void StartProcess(WebOrderDBAccessProcess inProcess)
        {
            try
            {
                Thread thread = process_threads[inProcess];

                thread.Start();
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProcess"></param>
        public void TerminateProcess(WebOrderDBAccessProcess inProcess)
        {
            try
            {
                // Removes process from the queue
                process_threads.Remove(inProcess);
            }
            catch
            {
            }
        }

        #endregion

        #region PRIVATE

        int max_concurrent_connections = 10;

        Dictionary<WebOrderDBAccessProcess, Thread> process_threads;
        Dictionary<WebOrderDBAccessProcess, Thread> process_threads_poll;

        bool dead_lock_paused;

        System.Timers.Timer process_time_out_timer;
        System.Timers.Timer process_dead_lock_paused_timer;
        System.Timers.Timer process_poll_timer;

        /// <summary>
        /// 
        /// </summary>
        protected void initTimeOutTimer()
        {
           // Create a timer with a 4 second interval.
            process_time_out_timer = new System.Timers.Timer(4000); // 4 sec

            // Hook up the Elapsed event for the timer. 
            process_time_out_timer.Elapsed += onTimeOutEvent;

            process_time_out_timer.Enabled = true;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void initDeadLockPausedTimer()
        {
             // Create a timer with a 15 second interval.
            process_dead_lock_paused_timer = new System.Timers.Timer(60000); // 1 min

            // Hook up the Elapsed event for the timer. 
            process_dead_lock_paused_timer.Elapsed += onDeadLockPausedTimedEvent;

            process_dead_lock_paused_timer.Enabled = false;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void initPollTimer()
        {
            // Create a timer with a 5 second interval.
            process_poll_timer = new System.Timers.Timer(5000); // 5 sec

            // Hook up the Elapsed event for the timer. 
            process_poll_timer.Elapsed += onProcessPollTimedEvent;

            process_poll_timer.Enabled = true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void onDeadLockPausedTimedEvent(Object source, ElapsedEventArgs e)
        {
            System.Timers.Timer timer = (System.Timers.Timer)source;
            timer.Enabled = false;

            dead_lock_paused = false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void onTimeOutEvent(Object source, ElapsedEventArgs e)
        {
            //ServiceLogger.Log(@"WebOrderDBAccessProcess.onTimeOutEvent(Object source, ElapsedEventArgs e) BEGIN");
            
            System.Timers.Timer timer = (System.Timers.Timer)source;
            timer.Enabled = false;

            //:::::::::::::::::::::::::::::::::::::::::

            foreach (KeyValuePair<WebOrderDBAccessProcess, Thread> pair in process_threads.OrderBy(key => key.Key.StartTime))
            {
                WebOrderDBAccessProcess process = pair.Key;
                Thread                   thread = pair.Value;

                DateTime now = DateTime.Now;
                //TimeSpan elapsedTime = now.Subtract(process.StartTime);

                long elapsedMinutes = totalMinutesElapsed(process.StartTime);

                //if (elapsedTime.TotalMinutes >= 1) // 1 min
                if(process.Started && ( elapsedMinutes >= 1 ) )
                {
                    TerminateProcess(process);
                }
                else if(elapsedMinutes < 1)
                {
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::::::::::

            timer.Enabled = true;

            //ServiceLogger.Log(@"WebOrderDBAccessProcess.onTimeOutEvent(Object source, ElapsedEventArgs e) END");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="dtYourDate"></param>
        /// <returns></returns>
        private long totalMinutesElapsed(DateTime dtYourDate)
        {
            long lTotalMinutesElapsed = 0;

            //Find Current Date and Time
            DateTime dtCurrent = DateTime.Now;

            //Find Time Difference details between current date and your given date
            TimeSpan tsDiff = dtCurrent.Subtract(dtYourDate);

            //Add Total Minutes for Days difference
            lTotalMinutesElapsed = lTotalMinutesElapsed + tsDiff.Days * (24 * 60);

            //Add Total Minutes for Hour difference
            lTotalMinutesElapsed = lTotalMinutesElapsed + tsDiff.Hours * 60;

            //Add Minutes 
            lTotalMinutesElapsed = lTotalMinutesElapsed + tsDiff.Minutes;

            return lTotalMinutesElapsed;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void onProcessPollTimedEvent(Object source, ElapsedEventArgs e)
        {
            System.Timers.Timer timer = (System.Timers.Timer)source;
            timer.Enabled = false;

            //:::::::::::::::::::::::::::::::::::::::::

            while (!dead_lock_paused && !tooManyProcesses() && (process_threads_poll.Count > 0))
            {
                KeyValuePair<WebOrderDBAccessProcess, Thread> pair = process_threads_poll.ElementAt(0);

                process_threads.Add(pair.Key, pair.Value);

                Thread thread = pair.Value;

                thread.Start();

                process_threads_poll.Remove(pair.Key);
            }

            //:::::::::::::::::::::::::::::::::::::::::

            timer.Enabled = true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProcess"></param>
        /// <returns></returns>
        private Thread createProcessThread(WebOrderDBAccessProcess inProcess)
        {
            return new Thread(inProcess.Start);
        }

        /// <summary>
        /// 
        /// </summary>
        private bool tooManyProcesses()
        {
            //ServiceLogger.Log(string.Format(@"tooManyProcesses() process_threads.Count: {0}", process_threads.Count));

            return process_threads.Count >= max_concurrent_connections;
        }

        #endregion
    }
}
