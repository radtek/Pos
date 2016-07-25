using System;
using System.Timers;

namespace Safemate.Core.Crm
{
    public class Base
    {
        ~Base()
        {
        }

        public Base()
        {
            configurationMonitorHeartbeat = new Timer(updateInterval);
            menumateTargetDetails = new String[3];

            configurationMonitorHeartbeat.AutoReset = true;
            configurationMonitorHeartbeat.Elapsed +=
                (ElapsedEventHandler)ConfigurationMonitor;

            type = "Base";

            for (int i = 0; i < 3; i++)
                menumateTargetDetails[i] = null;
        }

        public virtual void Cleanup()
        {
            configurationMonitorHeartbeat.Stop();
        }

        protected virtual void ConfigurationMonitor(Object sender,
                EventArgs eventArgs)
        {
        }

        public virtual void LogBackupOperation(String detailedStatus,
                                               String name, String status, Double backupSize)
        {
        }

        public void Shutdown()
        {
            configurationMonitorHeartbeat.Stop();
        }

        public bool IsAccessible
        {
            get {
                return isAccessible;
            }
        }

        public String[] MenumateTargetDetails
        {
            get {
                return menumateTargetDetails;
            }
        }

        public String Type
        {
            get {
                return type;
            }
        }

        static public double UpdateInterval
        {
            set {
                updateInterval = value;
            }
        }

        protected String accountName;
        static protected double updateInterval;
        protected Timer configurationMonitorHeartbeat;
        protected bool isAccessible;
        protected String[] menumateTargetDetails;
        protected String type;
    };
};
