using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NLog;
#region Logger Levels

/// Trace
/// Debug
/// Info
/// Warning
/// Error

#endregion

namespace PocketVoucherIntegration.Tools
{
    class ServiceLogger
    {
        //:::::::::::::::::::::::::::::::::

        private static Logger _logger = LogManager.GetLogger("Trace");
        private static Logger _dubugger = LogManager.GetLogger("Debug");
        private static bool _debugging = true;
        private static bool _tracing = true;

        ServiceLogger()
        {
        }

        public static ServiceLogger Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly ServiceLogger instance = new ServiceLogger();
        }

        //:::::::::::::::::::::::::::::::::

        static public void Debug(string inTxt)
        {
            if (_debugging)
            {
                _dubugger.Debug(inTxt);
            }
        }

        //:::::::::::::::::::::::::::::::::

        static public void Log(string inTxt)
        {
            if (_tracing)
            {
                _logger.Log(LogLevel.Info, inTxt);
            }
        }

        //:::::::::::::::::::::::::::::::::

        static public void LogError(string inTxt)
        {
            _logger.Log(LogLevel.Error, inTxt);
        }

        //:::::::::::::::::::::::::::::::::

        static public void LogException(string inMessage, Exception inException)
        {
            _logger.LogException(LogLevel.Error, inMessage, inException);
        }

        //:::::::::::::::::::::::::::::::::
    }
}
