using FCE = FileMate.Core.Exceptions;
using System;
using System.IO;
using System.Text;
using FileMate.Core.Xml;
using NLog;

namespace FileMate.Core 
{
    public class DiskFileRepository : IFileRepository 
    {

        protected string _pattern = null;

        private string _url;
        private DateTime _lastTransfer = DateTime.MinValue;

        protected string _username;
        protected string _password;
	    protected string _name;
        protected bool _allowOverwrite;

        public String TargetName { get { return _name; } }

        protected FileMate.Core.Xml.OperationFileType _operationFileType;

        public DiskFileRepository(string name, string url, string uname, string pwd, string filePattern, OperationFileType operationFileType, bool allowOverwrite=false)
        {
	    _name = name;
            _url = url; // raw url with possible expandable tokens i.e., {date}
            _username = uname;
            _password = pwd;
            _pattern = string.IsNullOrEmpty(filePattern) ? "*.*" : filePattern.Trim();

            _operationFileType = operationFileType;
            _allowOverwrite = allowOverwrite;
        }

        #region IFileRepository Members


        /// <summary>
        /// Return the (expanded) url for the repository.
        /// </summary>
        public string Url 
        {
            get { return Helper.ExpandTargetUrl(_url, _lastTransfer); }
        }

	    public string Name 
        {
		    get { return _name; }
	    }

        public bool AllowOverwrite
        {
            get { return _allowOverwrite; }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Do initial setup for a repository before an operation is executed.
        /// </summary>
        public void Initialise() 
        {
            LogManager.GetLogger("FileMate").Debug("Initialise DiskFileRespository: BEGIN");
            CheckAndCreateExpandedUrlPath();
            LogManager.GetLogger("FileMate").Debug("Initialise DiskFileRespository: END");
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Return a list of files that match the specified pattern.
        /// </summary>
        /// <returns></returns>
        public string[] GetFileList() 
        {
            string[] files = null;
            try 
            {
                LogManager.GetLogger("FileMate").Debug(String.Format("GetFileList: Folder {0}.", this.Url));

               files = Directory.GetFiles(this.Url, _pattern, SearchOption.TopDirectoryOnly);
            }
            catch (Exception e) 
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while getting file list", e);
		        throw new FCE.FileOperation(String.Format(
		        "Couldn't retrieve file list from {0}.", Url), Name, e);
            }

            if (files == null) 
            {
                files = new string[] { };
            }

            return files;
        }

        public void CreateDirectory(String directoryPath) 
        {
            if (_url[_url.Length - 1] != '\\')
                _url += "\\";
            
            //_url += directoryPath + @"\";
            //_url = directoryPath + @"\";
            //DirectoryInfo dir = new DirectoryInfo(directoryPath);

            DirectoryInfo dir = new DirectoryInfo(Url);

            dir.Create();
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        ///
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public Stream GetFileStream(string fileName) 
        {
            Stream s = null;

            try 
            {
                s = File.Open(Path.Combine(this.Url, fileName), FileMode.Open, FileAccess.Read, FileShare.Read);
            }
            catch (Exception e) 
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while opening stream", e);
		        throw new FCE.FileOperation(String.Format(
		        "Couldn't download {0} from {1}.", fileName, Url), Name, e);
            }

            return s;
        }

       /// <summary>
       /// 
       /// </summary>
       /// <param name="sourceFileStream"></param>
       /// <param name="fileName"></param>
       /// <returns></returns>
        public bool TransferFile(Stream sourceFileStream, string fileName) 
        {

            bool result = false;
             FileStream targetFileStream = null;
             BinaryWriter targetFileWriter = null;
             StreamReader sourceFileReader = null;

            string transferringFName = transferringFileName(fileName);
            string targetURI = Path.Combine(this.Url, transferringFName);

            try
            {
                if (AllowOverwrite)
                    targetFileStream = File.Open(targetURI, FileMode.Create, FileAccess.Write);
                else
                    targetFileStream = File.Open(targetURI, FileMode.CreateNew, FileAccess.Write);

/*
                 if (AllowOverwrite)
                    targetFileStream = File.Open(Path.Combine(this.Url, transferringFName), FileMode.Create, FileAccess.Write);
                else
                    targetFileStream = File.Open(Path.Combine(this.Url, transferringFName), FileMode.CreateNew, FileAccess.Write);
*/

                 StreamCopy.Copy(_operationFileType, sourceFileStream, targetFileStream);
  
                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

                closeReader(sourceFileReader );
                closeAndFlushWriter(targetFileWriter);

                closeStream(sourceFileStream);
                closeAndFlushFileStream(targetFileStream);
 
                renameTargetFileName(transferringFName, fileName);

                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

                result = true;
            }
            catch (Exception e) 
            {
                closeReader(sourceFileReader);
                closeAndFlushWriter(targetFileWriter);

                closeStream(sourceFileStream);
                closeAndFlushFileStream(targetFileStream);

                LogManager.GetLogger("FileMate").FatalException(string.Format("Exception occured while putting file => {0} : {1}", targetURI, e.Message), e);
               // LogManager.GetLogger("FileMate").FatalException(string.Format("Exception occured while putting file => {0} : {1}", fileName, e.Message), e);
		        throw new FCE.TransferOperation(String.Format(
		        "Couldn't transfer {0} to {1} : {2}.", fileName, Url, e.Message), Name, e);
            }
            finally 
            {
            }

            return result;
        }


        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        public void DeleteFiles()
        {

            string[] files = this.GetFileList();
            foreach (string file in files) 
            {
                try 
                {
                    File.Delete(file);
                }
                catch (Exception e) 
                {
                    LogManager.GetLogger("FileMate").ErrorException("Exception when deleting file", e);
		            throw new FCE.FileOperation(String.Format(
		            "Couldn't delete files from {0}.", Url), Name, e);
                }
            }
        }

        #endregion

        #region Private Methods

        private void CheckAndCreateExpandedUrlPath() 
        {
            try 
            {

                // set the last transfer time
                _lastTransfer = DateTime.Now;

                LogManager.GetLogger("FileMate").Debug(String.Format("CheckAndCreateExpandedUrlPath: Checking {0}.", Url));
                if (!Directory.Exists(Url)) 
                {
                    LogManager.GetLogger("FileMate").Debug(String.Format("CheckAndCreateExpandedUrlPath: Creating {0}.", Url));
                    CreateDirectory(Url);
                    LogManager.GetLogger("FileMate").Debug(String.Format("CheckAndCreateExpandedUrlPath: Created {0}.", Url));
                }
            }
            catch (Exception e) 
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception when creating directory", e);
		                    throw new FCE.FileOperation(String.Format(
		                    "Couldn't create directory {0}.", Url), Name, e);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        /// <returns></returns>
        string transferringFileName(string inFileName)
        {
            return inFileName + @".partial";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTargetFileName"></param>
        /// <param name="inNewFileName"></param>
        void renameTargetFileName(string inTargetFileName, string inNewFileName)
        {
            string targetURI = Path.Combine(this.Url, inTargetFileName);
            string newURI = Path.Combine(this.Url, inNewFileName);

            System.IO.File.Move(targetURI, newURI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inReader"></param>
        void closeReader(StreamReader inReader)
        {
            if (inReader != null)
            {
                inReader.Close();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWriter"></param>
        void closeAndFlushWriter(BinaryWriter inWriter)
        {
            if (inWriter != null)
            {
                inWriter.Flush();
                inWriter.Close();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStream"></param>
        void closeStream(Stream inStream)
        {
            // should be closed by closing the sourceFileReader
            if (inStream != null)
            {
                inStream.Close();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileStream"></param>
        void closeAndFlushFileStream(FileStream inFileStream)
        {
            // should be closed by closing the sourceFileReader
            if (inFileStream != null)
            {
                inFileStream.Flush();
                inFileStream.Close();
            }
        }

        #endregion
    }
}
