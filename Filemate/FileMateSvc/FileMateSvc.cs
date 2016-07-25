using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.ServiceProcess;
using System.Xml;
using FileMate.Core;
using System.Timers;
using System.Security.Cryptography;
using System.Collections;

namespace FileMateSvc
{
    public partial class FileMateSvc : ServiceBase
    {

        private CopyGroup _cg = null;
        private Timer _configChangedCheckTimer = null;
        private string _configFilePath = "";
        private string _currentHash = null;
        private double _configCheckInterval = 60000;

        public FileMateSvc()
        {
            InitializeComponent();

            this.ServiceName = "FileMate";
            this.EventLog.Log = "Application";
            this.AutoLog = false;

            this.CanHandlePowerEvent = false;
            this.CanHandleSessionChangeEvent = false;
            this.CanPauseAndContinue = false;
            this.CanShutdown = true;
            this.CanStop = true;
        }

        private void setupConfigurationTimer()
        {
            if (this._configChangedCheckTimer == null || !this._configChangedCheckTimer.Enabled)
            {
                this._configChangedCheckTimer = new Timer(_configCheckInterval);
                this._configChangedCheckTimer.Elapsed += checkConfigFileModifications;
                this._configChangedCheckTimer.Start();
            }
        }

        void checkConfigFileModifications(object sender, ElapsedEventArgs e)
        {
            try
            {
                string programFiles = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
                this._configFilePath = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "FileMateConfig.xml");
                if (File.Exists(this._configFilePath))
                {
                    _configChangedCheckTimer.Stop();
                    try
                    {
                        // config file is available.. if it is modified, re-initiate the copy operations
                        if (configFileModified())
                        {
                            XmlDocument doc = new XmlDocument();
                            doc.Load(_configFilePath);

                            if (_cg == null)
                                _cg = new CopyGroup(doc);
                            else
                            {
                                // re-load copy operations from xml document
                                this.EventLog.WriteEntry("Configurations modified..re-initializing transfers", EventLogEntryType.Information);
                                _cg.ReInitiateCopyOperations(doc);
                            }

                            // start file transfer
                            _cg.StartCopyOperationsSchedules();
                        }
                    }
                    catch (Exception ex)
                    {
                        this.EventLog.WriteEntry(string.Format("Failed to load FileMate configuration, invalid or missing settings file : {0}", ex.Message), EventLogEntryType.Error);
                    }
                    _configChangedCheckTimer.Start();
                }
                else
                    this.EventLog.WriteEntry("FileMate configuration file not found", EventLogEntryType.Error);
            }
            catch (Exception ex)
            {
                this.EventLog.WriteEntry(string.Format("An exception occurred : {0}", ex.Message), EventLogEntryType.Error);
            }
        }

        private bool configFileModified()
        {
            bool isModified = false;

            using (var md5 = MD5.Create())
            {
                using (var stream = File.OpenRead(_configFilePath))
                {
                    if (_currentHash == null)
                    {
                        _currentHash = Convert.ToBase64String(md5.ComputeHash(stream));
                        isModified = true;
                    }
                    else
                    {
                        string newHashStr = Convert.ToBase64String(md5.ComputeHash(stream));
                        isModified = Comparer.Default.Compare(newHashStr, _currentHash) != 0;
                        if (isModified)
                            _currentHash = newHashStr;
                    }
                }
            }

            return isModified;
        }

        protected override void OnStart(string[] args)
        {
            try
            {
                setupConfigurationTimer();
                this.EventLog.WriteEntry("FileMate started", EventLogEntryType.Information);
            }
            catch (Exception ex)
            {
                this.EventLog.WriteEntry(string.Format("FileMate failed to start : {0}", ex.Message), EventLogEntryType.Error);
            }
        }

        protected override void OnStop()
        {
            _cg = null;
            this.EventLog.WriteEntry("FileMate stopped", EventLogEntryType.Information);
        }

        protected override void OnShutdown()
        {
            _cg = null;
            this.EventLog.WriteEntry("FileMate stopped because of shutdown", EventLogEntryType.Information);
        }


    }
}
