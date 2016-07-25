using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading.Tasks;
using System.IO;
using System.Timers;

namespace MenumateServices.Tools
{
    // A delegate type for hooking up notifications.
    public delegate void FileSuppliedEventHandler(object sender, FileEventArgs e);

    /// <summary>
    /// 
    /// </summary>
    class FileSupplier
    {
        public event FileSuppliedEventHandler FileSupplied;

        public enum FileType
        {
            XML,
            Unknonw
        };

        string watched_folder = @"";
        FileType file_type;
        bool _stopped;

        FileSystemWatcher file_watcher;

        System.Timers.Timer file_timer;
        List<string> order_file_list;

        /// <summary>
        /// 
        /// </summary>
        public FileSupplier()
        {
            initSupplier();
        }
        //.............................................................................................

        #region PROPERTIES
        /// <summary>
        /// 
        /// </summary>
        public bool Stopped
        {
            get { return _stopped; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string WatchedFolder
        {
            get { return watched_folder; }
            set { setWatchedFolder(value); }
        }

        /// <summary>
        /// 
        /// </summary>
        public FileType WatchedFileType
        {
            get { return file_type; }
            set { setFileType(value); }
        }
        #endregion

        #region PUBLIC
        /// <summary>
        /// 
        /// </summary>
        public void Start()
        {
            _stopped = false;
            startTimer(true);

            //_fileWatcher.EnableRaisingEvents = true;

            ServiceLogger.Log(string.Format(@"Webmate: Start monitoring folder: {0}", WatchedFolder));
        }

        /// <summary>
        /// 
        /// </summary>
        public void Stop()
        {
            _stopped = true;
            startTimer(false);

            //_fileWatcher.EnableRaisingEvents = false;

            ServiceLogger.Log(string.Format(@"Webmate: Stop monitoring folder: {0}", WatchedFolder));
        }
        #endregion

        #region PRIVATE
        /// <summary>
        /// 
        /// </summary>
        void initSupplier()
        {
            ServiceLogger.Log(@"Webmate: File supplier initiated");

            order_file_list = new List<string>();

            setFileType(FileType.XML);
            initTimer();
        }
        //.............................................................................................

        /// <summary>
        /// 
        /// </summary>
        protected void initTimer()
        {
            ServiceLogger.Log(@"Webmate: File supplier timer initiated");

            // Create a timer with a 1 second interval.
            file_timer = new System.Timers.Timer(600);

            // Hook up the Elapsed event for the timer. 
            file_timer.Elapsed += onOrderTimedEvent;

            _stopped = true;
            file_timer.Enabled = false;

        }

        /// <summary>
        /// 
        /// </summary>
        protected void startTimer(bool inStart)
        {
            //ServiceLogger.Log(string.Format(@"FileSupplier startTimer:{0}", Convert.ToString(inStart)));
            file_timer.Enabled = inStart;
        }

        /// <summary>
        /// NO LONGER USED!!!
        /// </summary>
        void initFileWatcher(string inWatchedFolder)
        {
            file_watcher = new FileSystemWatcher(inWatchedFolder);

            //  Watch for changes in LastAccess and LastWrite times, and 
            //  the renaming of files or directories. 
            file_watcher.NotifyFilter = NotifyFilters.LastAccess | NotifyFilters.LastWrite | NotifyFilters.FileName | NotifyFilters.DirectoryName;

            //  Register a handler that gets called when a  
            //  file is created, changed, or deleted.
            file_watcher.Created += new FileSystemEventHandler(onCreated);
            // _fileWatcher.Changed += new FileSystemEventHandler(onChanged);
            //_fileWatcher.Renamed += new RenamedEventHandler(onChanged);

            //  Register a handler that gets called if the  
            //  FileSystemWatcher needs to report an error.
            file_watcher.Error += new ErrorEventHandler(onError);
        }
        //.............................................................................................

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFolder"></param>
        void setWatchedFolder(string inFolder)
        {
            watched_folder = inFolder;

            ServiceLogger.Log(string.Format(@"Watched folder set: {0}", watched_folder));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileType"></param>
        void setFileType(FileType inFileType)
        {
            file_type = inFileType;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void onOrderTimedEvent(Object source, ElapsedEventArgs e)
        {
            System.Timers.Timer timer = (System.Timers.Timer)source;
            timer.Enabled = false;

            //:::::::::::::::::::::::::::::::::::::::::

            if (order_file_list.Count == 0)
            {
                readOrderFiles(order_file_list);
            }

            //.........................................

            string orderFileURI = popOrderFile(order_file_list);

            if (!string.IsNullOrEmpty(orderFileURI) && File.Exists(orderFileURI))
            {
                ServiceLogger.Log(string.Format(@"FileSupplier orderFileURI: {0}", orderFileURI));

                supplyFile(orderFileURI);
            }

            //:::::::::::::::::::::::::::::::::::::::::

            timer.Enabled = true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderFileList"></param>
        protected virtual void readOrderFiles(List<string> inOrderFileList)
        {
            try
            {
                string[] allFiles = Directory.GetFiles(watched_folder);

                inOrderFileList.Clear();

                foreach (string fileNameURI in allFiles)
                {
                    if (fileType(fileNameURI) == this.WatchedFileType)
                    {
                        inOrderFileList.Add(fileNameURI);
                    }
                }
            }
            catch
            {
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderFileList"></param>
        /// <returns></returns>
        protected string popOrderFile(List<string> inOrderFileList)
        {
            try
            {
                string orderFile = "";
                if (inOrderFileList.Count > 0)
                {
                    orderFile = inOrderFileList[0];
                    inOrderFileList.RemoveAt(0);
                }
                return orderFile;
            }
            catch (Exception)
            {
                return @"";
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void onCreated(object sender, FileSystemEventArgs e)
        {
            System.Threading.Thread.Sleep(500);

            if (fileType(e.FullPath) == this.WatchedFileType)
            {
                supplyFile(e.FullPath);
            }
        }

        /// <summary>
        ///  This method is called when a file is created, changed, or deleted.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void onChanged(object sender, FileSystemEventArgs e)
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void onError(object sender, ErrorEventArgs e)
        {
            notifyError(e.ToString());
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="e"></param>
        protected virtual void supplyFile(string inFileURI)
        {
            if (FileSupplied != null)
            {
                FileEventArgs eArgs = new FileEventArgs(inFileURI);
                FileSupplied(this, eArgs);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        FileType fileType(string inFileName)
        {
            FileType result = FileType.Unknonw;

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            string ext = Path.GetExtension(inFileName);

            switch (ext.ToUpper())
            {
                case ".XML": result = FileType.XML; break;
            }

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inErrorDescription"></param>
        void notifyError(string inErrorDescription)
        {
        }
        #endregion
    }

    /// <summary>
    /// 
    /// </summary>
    public class FileEventArgs : EventArgs
    {
        string _fileURI;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        public FileEventArgs(string inFileURI)
        {
            _fileURI = inFileURI;
        }

        /// <summary>
        /// 
        /// </summary>
        public string FileURI
        {
            get { return _fileURI; }
            set { _fileURI = value; }
        }
    }
}
