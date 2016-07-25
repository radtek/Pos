using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography.X509Certificates;
using System.IO;
using System.Threading;
using XeroIntegration.Utility;
using XeroIntegration.Model;
using Xero.Api.Core;
using Xero.Api.Infrastructure.OAuth;
using Xero.Api.Infrastructure.RateLimiter;
using Xero.Api.Serialization;
using XeroIntegration.Tools;

namespace XeroIntegration
{
    class XMLFileObject
    {
        public string xmlFileName = @"";
        public Timer xmlTimer;
    }

    public class XeroIntegrationApplicationRunner
    {
        XeroCoreApi _repository;
        FileSystemWatcher _watchFolder = new FileSystemWatcher();
        private DefaultMapper Mapper = new DefaultMapper();
        Timer _retryTimer;
        int _retryPeriod = 5 * 60000; // 5 min
        string _cacheFolder;
        string _retryFolder;
        string _successfulFolder;
        string _failedFolder;
        List<XMLFileObject> _xmlObjectList;
        Int16 _processXmlFileTime = 500; // 500 mSec

        XeroIntegrationApplicationRunner(Credentials inCredentials)
        {
            InitFolderVars();
            _retryTimer = createRetryTimer();
            _xmlObjectList = new List<XMLFileObject>();
            CheckOnFolders();
            _repository = CreateRepository(inCredentials);
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

        #region Private

        XeroCoreApi CreateRepository(Credentials inCredentials)
        {
            try
            {
                switch (inCredentials.AppType)
                {
                    case @"private": return CreatePrivateRepository(inCredentials);
                }

                throw new XeroIntegrationException(string.Format(@"Application Type [{0}] not implemented.", inCredentials.AppType));
            }
            catch (Exception exc)
            {
                throw new XeroIntegrationException(string.Format(@"Application {0} with type [{1}] failed to start. Error Message: {2}", inCredentials.UserAgentString, inCredentials.AppType, exc.Message));
            }
        }

        XeroCoreApi CreatePrivateRepository(Credentials credentials, bool includeRateLimiter = false)
        {
            var pfxFilePath = ServiceInfo.StartPath + @"\" + credentials.PfxFileName;
            var xeroApi = new XeroCoreApi(credentials.APIEndpointURL,
                new XeroAuthenticator(pfxFilePath, credentials.PfxPassword),
                new Consumer(credentials.ConsumerKey, credentials.ConsumerSecret),
                null,
                Mapper,
                Mapper,
                includeRateLimiter ? new RateLimiter() : null);
            xeroApi.UserAgent = credentials.UserAgentString;
            xeroApi.Invoices.UseFourDecimalPlaces(true);
            xeroApi.Items.UseFourDecimalPlaces(true);
            return xeroApi;
        }


        void InitFolderVars()
        {
            _cacheFolder = XeroFolder(XeroIntegration.Properties.Settings.Default.XeroIntegrationCacheFolder);
            _retryFolder = XeroFolder(XeroIntegration.Properties.Settings.Default.XeroIntegrationRetryFolder);
            _successfulFolder = XeroFolder(XeroIntegration.Properties.Settings.Default.XeroIntegrationSuccessfulFolder);
            _failedFolder = XeroFolder(XeroIntegration.Properties.Settings.Default.XeroIntegrationFailedFolder);
        }

        string XeroFolder(string inFolder)
        {
            return ServiceInfo.GetFileURI(inFolder);
        }

        void CheckOnFolders()
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
                ServiceLogger.Log(string.Format(@"Failed to move a [{0}] to [{1}]. Xero Server Validation Error: {2}", inSourceFilename, inTargetFileName, exc.Message));
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
            KillFileTimerFromList();

            try
            {
                ProcessXmlFile(PopXMLFileFromList());
            }
            catch (Exception)
            {
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

        private void ProcessXmlFile(string inFilename)
        {
            try
            {
                var invoice = XeroHelper.DeSerializeFromFilePath<XeroInvoice>(inFilename);
                if (invoice.Type == "ACCPAYCREDIT" || invoice.Type == "ACCRECCREDIT")
                {
                    CreateCreditNode(inFilename, invoice);
                }
                else
                {
                    CreateInvoice(inFilename, invoice);
                }

            }
            catch (XeroIntegrationException xiExc)
            {
                MoveFileToFailedFolder(inFilename); // Failed. XML Invoice with an invalid structure or NOT Found is moved to the Failed folder for further human action

                ServiceLogger.Log(string.Format(@"Xero Server Validation Error: {0}  [Invoice filename: {1}]", xiExc.Message, inFilename));
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Xero Server Error: {0}  [Invoice filename: {1}]", exc.Message, inFilename));

                MoveFileToRetryFolder(inFilename);
            }
        }


        private void CreateCreditNode(string inFilename, XeroInvoice invoice)
        {
            var xeroCreditNote = XeroInvoiceController.Instance.CreateCreditNote(invoice);
            var createdCreditNote = _repository.Create(xeroCreditNote);

            if (createdCreditNote.ValidationStatus == Xero.Api.Core.Model.Status.ValidationStatus.Error)
            {
                MoveFileToFailedFolder(inFilename);

                foreach (var message in createdCreditNote.Warnings)
                {
                    ServiceLogger.Log(string.Format(@"Xero Server Validation Error: {0}  [Invoice Reference: {1}]", message.Message, invoice.Reference));
                }
            }
            else
            {
                if (invoice.Payments != null && invoice.Payments.Item != null && invoice.Payments.Item.Count > 0)
                {
                    var invoicePayments = XeroInvoiceController.Instance.GetCreditNotePayments(invoice.Payments, createdCreditNote);
                    _repository.Create(invoicePayments);
                }
                MoveFileToSuccesfulFolder(inFilename);
                ServiceLogger.Log(string.Format(@"Invoice: {0}  has been sent with reference number: {1}", invoice.Number, invoice.Reference));
            }
        }

        private void CreateInvoice(string inFilename, XeroInvoice invoice)
        {
            var xeroInvoice = XeroInvoiceController.Instance.CreateInvoice(invoice);
            var createdInvoice = _repository.Create(xeroInvoice);

            if (createdInvoice.ValidationStatus == Xero.Api.Core.Model.Status.ValidationStatus.Error)
            {
                MoveFileToFailedFolder(inFilename);

                foreach (var message in createdInvoice.Warnings)
                {
                    ServiceLogger.Log(string.Format(@"Xero Server Validation Error: {0}  [Invoice Reference: {1}]", message.Message, xeroInvoice.Reference));
                }
            }
            else
            {

                if (invoice.Payments != null && invoice.Payments.Item != null && invoice.Payments.Item.Count > 0)
                {
                    var invoicePayments = XeroInvoiceController.Instance.GetInvoicePayments(invoice.Payments, createdInvoice);
                    _repository.Create(invoicePayments);
                }
                MoveFileToSuccesfulFolder(inFilename);
                ServiceLogger.Log(string.Format(@"Invoice: {0}  has been sent with reference number: {1}", xeroInvoice.Number, xeroInvoice.Reference));
            }
        }


        #endregion

        #region Static


        public static XeroIntegrationApplicationRunner CreateAppRunner(Credentials inCredentials)
        {
            return new XeroIntegrationApplicationRunner(inCredentials);
        }

        #endregion
    }
}
