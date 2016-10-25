using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography.X509Certificates;
using System.IO;
using Xero.Api.Core;
using Xero.Api.Infrastructure.OAuth;
using Xero.Api.Infrastructure.RateLimiter;
using Xero.Api.Serialization;
using AccountingIntegration;
using AccountingIntegration.Tools;
using AccountingIntegration.Model;
using AccountingIntegration.Utility;
using Xero.Api.Core.Model.Status;
using AccountingIntegration.Xero;
using MYOB.AccountRight.SDK;
using MYOB.AccountRight.SDK.Services;
using MYOB.AccountRight.SDK.Contracts;
using System.Threading;


namespace AccountingIntegration.MYOB
{
    class XMLFileObject
    {
        public string xmlFileName = @"";
        public Timer xmlTimer;
    }

    public class MYOBIntegrationApplicationRunner
    {
        FileSystemWatcher _watchFolder = new FileSystemWatcher();
        Timer _retryTimer;
        int _retryPeriod = 5 * 60000; // 5 min
        string _cacheFolder;
        string _retryFolder;
        string _successfulFolder;
        string _failedFolder;
        List<XMLFileObject> _xmlObjectList;
        Int16 _processXmlFileTime = 3500; // 2000 mSec
        private IApiConfiguration _configurationCloud;
        private IOAuthKeyService _oAuthKeyService;
        private Credentials credentials;

        MYOBIntegrationApplicationRunner(Credentials inCredentials)
        {
            credentials = inCredentials;
            InitFolderVars();
            _retryTimer = createRetryTimer();
            _xmlObjectList = new List<XMLFileObject>();
            CheckOnFolders();
            FetchRetryInvoices();
            SendOldCachedInvoices(_cacheFolder);
            StartMonitoring(_cacheFolder);
        }

        public void StartActivityMonitoring()
        {
            StartMonitoring(_cacheFolder);
        }

        public void StopActivityMonitoring()
        {
            StopMonitoring();
        }

        public void GetToken(Credentials inCredentials, string inFilename)
        {
            try
            {
                _configurationCloud = new ApiConfiguration(inCredentials.ConsumerKey, inCredentials.ConsumerSecret, "http://desktop");
                _oAuthKeyService = new MYOBOAuthKeyService();
                var dateTimeNow = DateTime.UtcNow;
                var lastTokenTime = _oAuthKeyService.OAuthResponse.ReceivedTime;
                double days = (dateTimeNow - lastTokenTime).TotalDays;
                string days1 = days.ToString();
                ServiceLogger.Log(string.Format(@"days: {0}", days1));
                if (days < 365)
                {
                    if (_oAuthKeyService.OAuthResponse.HasExpired)
                    {
                        var oauthService = new OAuthService(_configurationCloud);
                        _oAuthKeyService.OAuthResponse = oauthService.RenewTokens(_oAuthKeyService.OAuthResponse);
                        ServiceLogger.Log("Token Renewed");
                    }
                    else
                    {
                        ServiceLogger.Log("Token not Expired");
                    }
                    MYOBCompanyFiles companyFiles = new MYOBCompanyFiles(inCredentials);
                    companyFiles.FetchFile(_configurationCloud, null, _oAuthKeyService, inFilename);
                }
            }
            catch (Exception exception)
            {
                ServiceLogger.LogException("Exception in GetTokens", exception);
            }
        }

        #region Private

        void InitFolderVars()
        {
            _cacheFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationCacheFolder);
            _retryFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationRetryFolder);
            _successfulFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationSuccessfulFolder);
            _failedFolder = MYOBFolder(AccountingIntegration.Properties.Settings.Default.MYOBIntegrationFailedFolder);
        }

        string MYOBFolder(string inFolder)
        {
            return ServiceInfo.GetFileURI(inFolder);
        }

        private void CheckOnFolders()
        {
            CheckAndCreateFolder(_cacheFolder);
            CheckAndCreateFolder(_successfulFolder);
            CheckAndCreateFolder(_retryFolder);
            CheckAndCreateFolder(_failedFolder);
        }

        void CheckAndCreateFolder(string inFolderName)
        {
            if (!Directory.Exists(inFolderName))
            {
                Directory.CreateDirectory(inFolderName);
            }
        }

        private void StartMonitoring(string sPath)
        {
            // This is the path we want to monitor
            _watchFolder.Path = sPath;

            // Make sure you use the OR on each Filter because we need to monitor
            // all of those activities
            _watchFolder.NotifyFilter = NotifyFilters.FileName;

            // Now hook the triggers(events) to our handler (eventRaised)
            _watchFolder.Created += new FileSystemEventHandler(EventRaised);

            // And at last.. We connect our EventHandles to the system API (that is all
            // wrapped up in System.IO)
            try
            {
                _watchFolder.EnableRaisingEvents = true;
            }
            catch (ArgumentException)
            {
                StopMonitoring();
            }
        }

        private void StopMonitoring()
        {
            _watchFolder.EnableRaisingEvents = false;
        }

        private void ResumeActivityMonitoring()
        {
            _watchFolder.EnableRaisingEvents = true;
        }

        private void ProcessRetryFolder()
        {
            StopMonitoring();
            FetchRetryInvoices();
            SendOldCachedInvoices(_cacheFolder);
            ResumeActivityMonitoring();
        }

        private void FetchRetryInvoices()
        {
            string[] filenames = System.IO.Directory.GetFiles(_retryFolder);

            foreach (string filename in filenames)
            {
                if (filename.ToUpper().EndsWith(@".XML"))
                {
                    MoveFileToCacheFolder(filename);
                }
            }
        }

        private void SendOldCachedInvoices(string inPath)
        {
            string[] filenames = System.IO.Directory.GetFiles(inPath);

            foreach (string filename in filenames)
            {
                try
                {
                    if (filename.ToUpper().EndsWith(@".XML"))
                    {
                        ProcessXmlFile(filename);
                    }
                }
                catch (Exception exc)
                {
                    ServiceLogger.Log(string.Format(@"Xero Server Validation Error: {0}  [Invoice filename: {1}]", exc.Message, filename));
                }
            }
        }

        private void MoveFileToCacheFolder(string inFilename)
        {
            MoveFile(inFilename, GetFilePath(_cacheFolder, inFilename));
        }

        private void MoveFileToSuccesfulFolder(string inFilename)
        {
            MoveFile(inFilename, GetFilePath(_successfulFolder, inFilename));
        }

        private void MoveFileToRetryFolder(string inFilename)
        {
            MoveFile(inFilename, GetFilePath(_retryFolder, inFilename));
        }

        private void MoveFileToFailedFolder(string inFilename)
        {
            MoveFile(inFilename, GetFilePath(_failedFolder, inFilename));
        }

        private void MoveFile(string inSourceFilename, string inTargetFileName)
        {
            try
            {
                File.Move(inSourceFilename, inTargetFileName);
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Failed to move a [{0}] to [{1}]. MYOB Server Validation Error: {2}", inSourceFilename, inTargetFileName, exc.Message));
            }
        }

        private string GetFilePath(string baseDirectory, string inFilePath)
        {
            return string.Format(@"{0}\{1}", baseDirectory, Path.GetFileName(inFilePath));
        }


        /// <summary>
        /// Triggered when an event is raised from the folder activity monitoring.
        /// All types exists in System.IO
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e">containing all data send from the event that got executed.</param>
        private void EventRaised(object sender, System.IO.FileSystemEventArgs e)
        {
            switch (e.ChangeType)
            {
                case WatcherChangeTypes.Changed: break;
                case WatcherChangeTypes.Created: pQueueXmlFile(e.FullPath); break;
                case WatcherChangeTypes.Deleted: break;
                default: break;
            }
        }

        private void pQueueXmlFile(string inFilename)
        {
            _xmlObjectList.Add(pCreateXMLFileObject(inFilename, createProcessXmlFileTimer()));
        }

        private XMLFileObject pCreateXMLFileObject(string inFilename, Timer inTimer)
        {
            XMLFileObject result = new XMLFileObject();

            result.xmlFileName = inFilename;
            result.xmlTimer = inTimer;

            return result;
        }

        private Timer createRetryTimer()
        {
            return createTimer(retryTimer_Tick, _retryPeriod, _retryPeriod);
        }

        private Timer createProcessXmlFileTimer()
        {
            return createTimer(processXmlFileTimer_Tick, _processXmlFileTime, _processXmlFileTime);
        }

        delegate void TimerTick(object obj);

        private Timer createTimer(TimerTick inCallback, int inDueTime, int inPeriod)
        {
            TimerCallback callBack = new TimerCallback(inCallback);
            Timer result = new Timer(callBack, null, inDueTime, inPeriod);

            return result;
        }

        void retryTimer_Tick(object obj)
        {
            StopTimer(_retryTimer);

            try
            {
                ProcessRetryFolder();
            }
            catch (Exception)
            {
            }

            StartTimer(_retryTimer, _retryPeriod, _retryPeriod);
        }

        void processXmlFileTimer_Tick(object obj)
        {
            lock (lockObject)
            {
                KillFileTimerFromList();

                try
                {
                    ProcessXmlFile(PopXMLFileFromList());
                }
                catch (Exception)
                {
                }
            }
           
        }

        void KillFileTimerFromList()
        {
            try
            {
                KillTimer(_xmlObjectList[0].xmlTimer);
            }
            catch (Exception)
            {
            }
        }

        void StartTimer(Timer inTimer, int inDueTime, int inPeriod)
        {
            ChangeTimer(inTimer, inDueTime, inPeriod);
        }

        void StopTimer(Timer inTimer)
        {
            ChangeTimer(inTimer, Timeout.Infinite, Timeout.Infinite);
        }

        void KillTimer(Timer inTimer)
        {
            ChangeTimer(inTimer, Timeout.Infinite, Timeout.Infinite);
        }

        void ChangeTimer(Timer inTimer, int inDueTime, int inPeriod)
        {
            try
            {
                inTimer.Change(inDueTime, inPeriod);
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Xero Server Error: {0}  [Hanged timer]", exc.Message));
                throw;
            }
        }

        private string PopXMLFileFromList()
        {
            try
            {
                string result = _xmlObjectList[0].xmlFileName;

                _xmlObjectList.RemoveAt(0);

                GC.Collect(); // To make sure the timer's gone away

                return result;
            }
            catch (Exception)
            {
                throw;
            }
        }
        private static volatile object lockObject = new object();
        private void ProcessXmlFile(string inFilename)
        {
            try
            {
                GetToken(credentials, inFilename);
            }
            catch (Exception exc)
            {
                ServiceLogger.Log("Error in ProcessXmlFile " + exc.Message);
            }
           
        }

        #endregion

        #region Static
        public static MYOBIntegrationApplicationRunner CreateAppRunner(Credentials inCredentials)
        {
            return new MYOBIntegrationApplicationRunner(inCredentials);
        }

        #endregion
    }
}
