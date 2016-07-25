using NLog;

namespace Chefmate.Logger
{
    public class ChefmateLogger
    {
        private static ChefmateLogger _instance;
        private static volatile object _syncRoot = new object();
        NLog.Logger _logger;
        NLog.Logger _timingLogger;
        ChefmateLogger()
        {
            CreateLoggers();
        }
        public static ChefmateLogger Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new ChefmateLogger();
                    }
                }
                return _instance;
            }
        }
        public void LogEvent(string inFuncName, string inEventDescription)
        {
#if DEBUG
            _logger.Log(LogLevel.Trace, string.Format(@"{0}: {1}: {2}", this.GetType().Name, inFuncName, inEventDescription));
#endif
        }
        public void LogTiming(string inMsg)
        {
#if DEBUG
            _timingLogger.Log(LogLevel.Debug, inMsg);
#endif
        }
        public void LogError(string inFuncName, string inErrorDescription)
        {
            _logger.Log(LogLevel.Error, string.Format(@"{0}: {1}: {2}", this.GetType().Name, inFuncName, inErrorDescription));
        }
        public void CreateLoggers()
        {
            _logger = LogManager.GetLogger(@"tracer");
            _timingLogger = LogManager.GetLogger(@"timing");
        }

    }
}
