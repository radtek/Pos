using System;
using System.Threading;

namespace PDServer
{
    /***
     * This Class can be found here.
     * http://www.interact-sw.co.uk/iangblog/2004/03/23/locking
     * http://www.interact-sw.co.uk/iangblog/2004/04/26/yetmoretimedlocking
     * 
     * using (TimedLock.Lock(obj))
     * {
     * }
     * 
     * */

    public struct TimedLock : IDisposable
    {

        public static TimedLock Lock(object o)
        {
            return Lock(o, TimeSpan.FromSeconds(5));
        }

        public static TimedLock Lock(object o, TimeSpan timeout)
        {
            TimedLock tl = new TimedLock(o);
            if (!Monitor.TryEnter(o, timeout))
            {
#if DEBUG
                System.GC.SuppressFinalize(tl.leakDetector);
#endif
                throw new LockTimeoutException();
            }

            return tl;
        }

        private TimedLock(object o)
        {
            target = o;
#if DEBUG
            leakDetector = new Sentinel();
#endif
        }
        private object target;

        public void Dispose()
        {
            Monitor.Exit(target);

            // It's a bad error if someone forgets to call Dispose,
            // so in Debug builds, we put a finalizer in to detect
            // the error. If Dispose is called, we suppress the
            // finalizer.
#if DEBUG
            GC.SuppressFinalize(leakDetector);
#endif
        }

#if DEBUG
        // (In Debug mode, we make it a class so that we can add a finalizer
        // in order to detect when the object is not freed.)
        private class Sentinel
        {
            ~Sentinel()
            {
                // If this finalizer runs, someone somewhere failed to
                // call Dispose, which means we've failed to leave
                // a monitor!
            }
        }
        private Sentinel leakDetector;
#endif

    }
    public class LockTimeoutException : ApplicationException
    {
        public LockTimeoutException()
            : base("Timeout waiting for lock")
        {
        }
    }
}
