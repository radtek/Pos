//#define __Safemate_Service_Debug__

using SC = Safemate.Core;
using SCB = Safemate.Core.Backup;
using SCLM = Safemate.Core.LogProxy.Mail;
using System.Collections.Generic;

using System;
using System.Diagnostics;
using System.ServiceProcess;
using System.Configuration.Install;
using System.Reflection;

namespace Safemate.Service
{
    public class Service : ServiceBase
    {
#if ! __Safemate_Service_Debug__
        protected override void OnContinue()
        {
            OnStart(null);
        }

        protected override void OnShutdown()
        {
            OnStop();
        }

        protected override void OnStart(String[] args)
        {
            core = new SC.Core(EventLog);
            base.OnStart(args);
        }

        protected override void OnStop()
        {
            base.OnStop();
        }

        public Service()
        {
            ServiceName = "SafemateBackupService";
            AutoLog = false;
            CanPauseAndContinue = CanStop = CanShutdown = true;
            CanHandlePowerEvent = CanHandleSessionChangeEvent = false;
        }
#else
        static public void Main(String[] args)
        {
            core = new SC.Core(null);
            for ( ; ; )
                ;
        }
#endif

        ~Service()
        {
            core.Dispose();
            core = null;
        }

#if __Safemate_Service_Debug__
        static SC.Core core;
#else
        SC.Core core;
#endif
    }
};
