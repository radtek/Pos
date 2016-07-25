using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

using MenumateServices.Tools;

namespace MenumateServices.WebMate.InternalClasses
{
    public delegate void WebOrderFileSuppliedEventHandler(object sender, string fileURI);

    public class WebOrderFileSupplier
    {
        SortedDictionary<string, FileSupplier> file_suppliers;

        public event WebOrderFileSuppliedEventHandler WebOrderFileSupplied;

        /// <summary>
        /// 
        /// </summary>
        public static WebOrderFileSupplier Instance
        {
            get
            {
                return Nested.instance;
            }
        }
        //.............................................................................................

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly WebOrderFileSupplier instance = new WebOrderFileSupplier();
        }
        //.............................................................................................

        /// <summary>
        /// 
        /// </summary>
        internal WebOrderFileSupplier()
        {
        }

        #region PUBLIC
        /// <summary>
        /// 
        /// </summary>
        public void Init()
        {
            file_suppliers = new SortedDictionary<string, FileSupplier>();
        }

        /// <summary>
        /// 
        /// </summary>
        public void Start()
        {
            foreach (KeyValuePair<string, FileSupplier> kvp in file_suppliers)
            {
                StartSupplier(kvp.Value);
                //kvp.Value.Start();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHandle"></param>
        public void StartSupplier(object inHandle)
        {
            try
            {
                FileSupplier supplier = (FileSupplier)inHandle;

                supplier.Start();
            }
            catch
            {
                ServiceLogger.Log(@"WebOrderFileSupplier.StartSupplier(object inHandle): FAILED");
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public void Stop()
        {
            foreach (KeyValuePair<string, FileSupplier> kvp in file_suppliers)
            {
                kvp.Value.Stop();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFolderList"></param>
        public void AddWatchedFolders(string[] inFolderList)
        {
            foreach (string folderName in inFolderList)
            {
                AddWatchedFolder(folderName);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFolder"></param>
        public object AddWatchedFolder(string inFolder)
        {
            object supplier = 0;

            try
            {
                FileSupplier fileSupplier = createNewFileSupplier(inFolder, FileSupplier.FileType.XML);
                file_suppliers.Add(inFolder, fileSupplier);

                supplier = fileSupplier;
            }
            catch (ArgumentNullException nexc)
            {
                // The key is null
                ServiceLogger.Log(string.Format(@"Failed to monitor folder {0}: The  key is null: {1}", inFolder, nexc.Message));
            }
            catch (ArgumentException aexc)
            {
                // The key already exists in the dictionary
                // Prevent from folder duplication
                ServiceLogger.Log(string.Format(@"Failed to monitor folder {0}: The key already exists in the dictionary: {1}", inFolder, aexc.Message));
            }

            return supplier;
        }
        #endregion

        #region PRIVATE        
        /// <summary>
        /// 
        /// </summary>
        FileSupplier createNewFileSupplier(string inFolder, FileSupplier.FileType inFileType)
        {
            FileSupplier result = new FileSupplier();

            result.WatchedFolder = inFolder;
            result.WatchedFileType = inFileType;
            result.FileSupplied += fileSupplied;

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void fileSupplied(object sender, FileEventArgs inEArgs)
        {
            if (WebOrderFileSupplied != null)
            {
                WebOrderFileSupplied(this, inEArgs.FileURI);
            }
       }
        #endregion
    }
}
