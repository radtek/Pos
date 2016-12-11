using System;
using System.IO;
using System.Threading;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using ChefMate.Communication.Receiver;

namespace Chefmate.Infrastructure.Builder
{
    public class FileSystemInitializer
    {
        public delegate void FileCretaed(string fileUri);

        public event FileCretaed FileCreatedEvent;
        private static volatile FileSystemInitializer _instance;
        private static object _syncRoot = new Object();
        string _filePool;
        string _retryFolder;
        string _succesfulFolder;
        FileSystemWatcher _fileWatcher;
        StreamConverterPos _streamConverter;
        private FileSystemInitializer()
        {
            InitFileSystem();
        }

        public static FileSystemInitializer Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new FileSystemInitializer();
                    }
                }

                return _instance;
            }
        }

        #region PROPERTIES
        public bool Stopped
        {
            get { return _streamConverter.Stopped; }
        }

        #endregion

        #region PUBLIC
        public void Start()
        {
            _fileWatcher.EnableRaisingEvents = true;
            _streamConverter.Start();
        }
        public void Stop()
        {
            _streamConverter.Stop();
            _fileWatcher.EnableRaisingEvents = false;
        }
        #endregion

        #region Private Methods
        private void InitFileSystem()
        {
            InitDirectories();
            InitFileWatcher();
            InitReceiver();
        }
        private void InitDirectories()
        {
            _filePool = GetFullPath("FilePool");
            _retryFolder = GetFullPath("RetryFolder");
            _succesfulFolder = GetFullPath("SuccesfulFolder");
            CheckAndCreateFolders();
        }
        private void CheckAndCreateFolders()
        {
            checkAndCreateFolder(_filePool);
            checkAndCreateFolder(_retryFolder);
            checkAndCreateFolder(_succesfulFolder);
        }
        private void checkAndCreateFolder(string inFolderUri)
        {
            if (!Directory.Exists(inFolderUri))
            {
                Directory.CreateDirectory(inFolderUri);
            }
        }
        private void InitFileWatcher()
        {
            _fileWatcher = new FileSystemWatcher(_filePool);

            //  Watch for changes in LastAccess and LastWrite times, and 
            //  the renaming of files or directories. 
            _fileWatcher.NotifyFilter = NotifyFilters.LastAccess | NotifyFilters.LastWrite | NotifyFilters.FileName | NotifyFilters.DirectoryName;

            //  Register a handler that gets called when a  
            //  file is created, changed, or deleted.
            _fileWatcher.Created += new FileSystemEventHandler(OnCreated);

            //  Register a handler that gets called if the  
            //  FileSystemWatcher needs to report an error.
            _fileWatcher.Error += new ErrorEventHandler(OnError);
        }
        private void InitReceiver()
        {
            _streamConverter = new StreamConverterPos { FileFolder = _filePool };
        }
        private string GetFullPath(string inDirectoryName)
        {
            return Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase).Substring(6) + @"\" + inDirectoryName;
        }
        private void OnCreated(object sender, FileSystemEventArgs e)
        {
            Thread.Sleep(1000);
            if (FileCreatedEvent != null)
                FileCreatedEvent(e.FullPath);
            //SupplyFile(e.FullPath);
        }
        private void OnError(object sender, ErrorEventArgs e)
        {
            NotifyError(e.ToString());
        }
        private void DeleteUnknownFile(string inFileUri)
        {
            File.Delete(inFileUri);
            NotifyError(@"");
        }
        private FileType GetFileType(string inFileName)
        {
            FileType result = FileType.Unknonw;
            string ext = Path.GetExtension(inFileName);
            switch (ext.ToUpper())
            {
                case ".XML": result = FileType.Xml; break;
            }
            return result;
        }
        private void NotifyError(string inErrorDescription)
        {
        }
        private void MoveFileToSuccessfulFolder(string inFileUri)
        {
            File.Move(inFileUri, SuccessfulFileUri(inFileUri));
        }
        private void MoveFileToRetryFolder(string inFileUri)
        {
            File.Move(inFileUri, RetryFileUri(inFileUri));
        }
        private string RetryFileUri(string inFileUri)
        {
            string fileName = Path.GetFileName(inFileUri);

            return _retryFolder + @"\" + fileName;
        }
        private string SuccessfulFileUri(string inFileUri)
        {
            string fileName = Path.GetFileName(inFileUri);

            return _succesfulFolder + @"\" + fileName;
        }
        public void SupplyFile(string inFileUri)
        {
            switch (GetFileType(inFileUri))
            {
                case FileType.Xml: SupplyXmlFile(inFileUri); break;

                default: DeleteUnknownFile(inFileUri); break;
            }
        }
        private void SupplyXmlFile(string inXmluri)
        {
            int buildAttempt = 1;
            bool orderBuilt = false;
            string xmlUri = inXmluri;
            while (buildAttempt <= 5 && !orderBuilt)
            {
                try
                {
                    orderBuilt = FileOrderBuilder.Instance.BuildOrder(inXmluri);
                }
                catch (Exception e)
                {
                    LogError("SupplyXmlFile()", "Build Attempt " + buildAttempt + " Failed.");
                    LogError("SupplyXmlFile()", e.Message);
                    Thread.Sleep(1000);
                }
                buildAttempt++;
            }
            if (orderBuilt)
            {
                MoveFileToSuccessfulFolder(xmlUri);
            }
            else
            {
                MoveFileToRetryFolder(xmlUri);
            }
        }
        private bool AddOrderToListManager(Order inOrder)
        {
            try
            {
                if (inOrder != null)
                {
                    //ToDo:Change required 
                    // OrderListManager.Instance.AddOrder(inOrder);
                    return true;
                }
                else
                {
                    LogError(@"addOrderToListManager", @"Add Order to the order list manager. Order is null");
                    return false;
                }
            }
            catch (Exception e)
            {
                LogError(@"addOrderToListManager", e.Message + "\n" + e.StackTrace);
                return false;
            }
        }
        void LogError(string inFuncName, string inMsg)
        {
            string functURI = string.Format(@"ChefMate.Builder.BuilderFileSupplier.{0}", inFuncName);
            //Logger.CMLogger.Instance.logError(functURI, inMsg);
        }

        #endregion

    }
}
