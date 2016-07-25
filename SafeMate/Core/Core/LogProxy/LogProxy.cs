using NL = NLog;

using SCC   = Safemate.Core.Crm;
using SCLPM = Safemate.Core.LogProxy.Mail;
using SD = System.Diagnostics;

using System;

namespace Safemate.Core.LogProxy
{
    public enum LogLevel {
        Debug,
        Error,
        Info,
        Warning
    };

    static public class LogProxy
    {
        delegate void LoggingFunction(String message);

        static LogProxy()
        {
            loggingFunctions = new LoggingFunction[4] {
                DebugLog, ErrorLog, InfoLog, WarnLog
            };

            eventLogInstance = null;
            loggerInstance = NL.LogManager.GetLogger("SafemateLogger");
            mailerInstance = null;
        }

        static void DebugLog(String message)
        {
            loggerInstance.Debug(String.Format("D@{0} {1}",
                                               DateTime.Now.ToString("yyyyMMddHHmm"), message));
        }

        static void ErrorLog(String message)
        {
            loggerInstance.Error(String.Format("E@{0} {1}",
                                               DateTime.Now.ToString("yyyyMMddHHmm"), message));
            if (eventLogInstance != null)
                eventLogInstance.WriteEntry(String.Format("{0} => {1}",
                                            DateTime.Now.ToString("yyyyMMddHHmm"), message));
        }

        static void InfoLog(String message)
        {
            loggerInstance.Info(String.Format("I@{0} {1}",
                                              DateTime.Now.ToString("yyyyMMddHHmm"), message));
        }

        static public void Log(LogLevel level, String message)
        {
            loggingFunctions[(int)level](message);
        }

        static public void LogToMail(String message, String subject)
        {
            mailerInstance.Send(message, subject);
        }

        static void WarnLog(String message)
        {
            loggerInstance.Warn(String.Format("W@{0} {1}",
                                              DateTime.Now.ToString("yyyyMMddHHmm"), message));
            if (eventLogInstance != null)
                eventLogInstance.WriteEntry(String.Format("{0} => {1}",
                                            DateTime.Now.ToString("yyyyMMddHHmm"), message));
        }

        static public SCLPM.Base MailerInstance
        {
            set {
                if (mailerInstance == null)
                    mailerInstance = value;
            }
        }

        static public SD.EventLog EventLogInstance
        {
            set {
                eventLogInstance = value;
            }
        }

        static LoggingFunction[] loggingFunctions;
        static NL.Logger loggerInstance;
        static SCLPM.Base mailerInstance;
        static SD.EventLog eventLogInstance;
    };
};
