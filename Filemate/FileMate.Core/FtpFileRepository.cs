using FCE = FileMate.Core.Exceptions;
using System.Text.RegularExpressions;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using NLog;

using FileMate.Core.Xml;

namespace FileMate.Core
{
    class FtpFileRepository : IFileRepository
    {

        protected string _pattern = null;
        protected string _name;
        protected string _url;
        protected string _username;
        protected string _password;
        protected FtpWebRequest _ftpSession = null;
        protected bool _allowOverwrite;

        protected OperationFileType _operationFileType;
        public String TargetName { get { return _name; } }

        public FtpFileRepository(string name, string url, string uname, string pwd, string filePattern, OperationFileType operationFileType, bool allowOverwrite = false)
        {
            _name = name;
            _url = url;
            _username = uname;
            _password = pwd;
            _pattern = string.IsNullOrEmpty(filePattern) ? "*.*" : filePattern.Trim();

            _operationFileType = operationFileType;
            _allowOverwrite = allowOverwrite;
        }

        #region ISourceFileRepository Members

        public string Name
        {
            get { return _name; }
        }

        public string Url
        {
            get { return Helper.ExpandTargetUrl(_url, DateTime.Now); }
        }

        public bool AllowOverwrite
        {
            get { return _allowOverwrite; }
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Do initial setup for a repository before an operation is executed.
        /// </summary>
        public void Initialise() { }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Return list of files that match file pattern.
        /// </summary>
        /// <returns></returns>
        /// 

        String[] GetDirectoryList()
        {
            String[] processedDirectoryList;
            String rawDirectoryList;
            StreamReader reader;
            FtpWebResponse response;
            Stream responseStream;

            SetupFtpSession(Url);

            _ftpSession.Method = WebRequestMethods.Ftp.ListDirectory;
            response = (FtpWebResponse)_ftpSession.GetResponse();

            responseStream = response.GetResponseStream();
            reader = new StreamReader(responseStream);

            rawDirectoryList = reader.ReadToEnd();

            reader.Close();
            responseStream.Close();

            processedDirectoryList = rawDirectoryList.Split(new String[] { "\r\n" },
                StringSplitOptions.RemoveEmptyEntries);

            return processedDirectoryList;
        }

        public void CreateDirectory(String directory)
        {
            FtpWebResponse response = null;
            String updatedUrl = String.Format("{0}{1}/", Url, directory);

            for (int i = 0; i < 8; i++)
            {
                try
                {
                    SetupFtpSession(updatedUrl);
                    _ftpSession.Method = WebRequestMethods.Ftp.MakeDirectory;
                    _url = updatedUrl;
                    response = (FtpWebResponse)_ftpSession.GetResponse();
                }
                catch (WebException we)
                {
                    LogManager.GetLogger("FileMate").Info(
                        "FTP Server reports 550. Assuming upload directory already exists.");
                }
            }
        }

        public string[] GetFileList()
        {

            List<string> files = new List<string>();

            try
            {
                LogManager.GetLogger("FileMate").Debug("getting file list ...");

                String[] allFilesFolders = GetDirectoryList();

                if (_pattern.Contains("*"))  // is pattern a wildcard spec i.e., *.csv or a regular expression
                {
                    string extension = "";
                    string fileName = "";
                    // wildcard
                    foreach (string f in allFilesFolders)
                    {
                        extension = Path.GetExtension(f); 
                        fileName = Path.GetFileNameWithoutExtension(f);
                        bool selectFile = false;

                        if (_pattern.Equals("*.*") && !string.IsNullOrEmpty(extension))
                            selectFile = true;
                        else
                        {
                            if ( _pattern.StartsWith("*") && extension.Equals(_pattern.Substring(1, _pattern.Length - 1)))    // matches *.pdf
                                selectFile = true;
                            else if(_pattern.EndsWith("*") && fileName == _pattern.Substring(0,_pattern.IndexOf('.')))  // matches menumate.*
                                selectFile = true;
                            else // matches menumate*.pdf
                            {
                                selectFile = Regex.IsMatch(f, _pattern.Replace("*",".*"));
                            }
                        }

                        if (selectFile)
                            files.Add(f);
                    }
                }
                else // user specified an actual filename
                {

                    foreach (string f in allFilesFolders)
                    {
                        if (f.Equals(_pattern))
                        {
                            files.Add(f);
                        }
                    }
                }

                LogManager.GetLogger("FileMate").Debug(string.Format("done, returning {0} files", files.Count));

            }
            catch (SystemException se)
            {
                LogManager.GetLogger("FileMate").ErrorException(string.Format("Exception while getting file list : {0}", se.Message), se);
                throw new FCE.FileOperation(String.Format(
                    "Couldn't retrieve file list from {0}.", _url), Name, se);
            }

            return files.ToArray<string>();
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Get a stream to allow transfer of the file from the ftp site.
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public Stream GetFileStream(string fileName)
        {
            Stream s = null;

            try
            {
                SetupFtpSession(Url + fileName);
                _ftpSession.Method = WebRequestMethods.Ftp.DownloadFile;
                FtpWebResponse r = (FtpWebResponse)_ftpSession.GetResponse();

                LogFtpResponseCode(r);

                s = r.GetResponseStream();
            }
            catch (Exception e)
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while opening stream", e);
                throw new FCE.FileOperation(String.Format(
                    "Couldn't download {0} from {1}.", fileName, _url), Name, e);
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

            Stream targetFtpStream = null;
            try
            {
                string transferringFName = transferringFileName(fileName);

                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

                SetupFtpSession(Url + transferringFName);

                if (AllowOverwrite)
                    _ftpSession.Method = WebRequestMethods.Ftp.UploadFile;
                else
                    _ftpSession.Method = WebRequestMethods.Ftp.AppendFile;  // NOTE!: AppendFile method is important!

                targetFtpStream = _ftpSession.GetRequestStream();

                StreamCopy.Copy(_operationFileType, sourceFileStream, targetFtpStream);

                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

                closeStream(sourceFileStream);
                closeAndFlushFtpStream(targetFtpStream);

                renameTargetFileName(transferringFName, fileName);

                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::

                using (FtpWebResponse response = (FtpWebResponse)_ftpSession.GetResponse())
                {
                    LogFtpResponseCode(response);
                }
                result = true;

            }
            catch (Exception e)
            {
                closeStream(sourceFileStream);
                closeAndFlushFtpStream(targetFtpStream);

                LogManager.GetLogger("FileMate").ErrorException(string.Format("Exception while transferring file;{0}{1}{2}{3}{4}",
                    Environment.NewLine, e.Message, Environment.NewLine,
                    _url + fileName, Environment.NewLine), e);
                throw new FCE.TransferOperation(String.Format(
                    "Couldn't transfer {0} to url {1} : {2}.", fileName, _url, e.Message), Name, e);
            }
            finally
            {
            }

            return result;
        }

        ///////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Delete files from source.
        /// </summary>
        /// <returns></returns>
        public void DeleteFiles()
        {

            string[] files = this.GetFileList();

            LogManager.GetLogger("FileMate").Debug(string.Format("deleteing {0} files", files.Length));



            foreach (string file in files)
            {
                try
                {

                    LogManager.GetLogger("FileMate").Debug(string.Format("deleting {0}", file));

                    string fileName = Path.GetFileName(file);

                    SetupFtpSession(this.Url + fileName);
                    _ftpSession.Method = WebRequestMethods.Ftp.DeleteFile;
                    FtpWebResponse response = (FtpWebResponse)_ftpSession.GetResponse();
                    LogFtpResponseCode(response);
                }
                catch (Exception e)
                {
                    LogManager.GetLogger("FileMate").ErrorException("Exception while deleting files", e);
                    throw new FCE.FileOperation(String.Format(
                        "Couldn't delete files from {0}.", _url), Name, e);
                }
            }
            LogManager.GetLogger("FileMate").Debug("done deleting files.");
        }

        #endregion

        #region Private methods

        private void SetupFtpSession(string url)
        {
            try
            {
                _ftpSession = (FtpWebRequest)FtpWebRequest.Create(new Uri(url));
                _ftpSession.Credentials = new NetworkCredential(_username, _password);
                _ftpSession.UsePassive = true;
                _ftpSession.KeepAlive = true;
                _ftpSession.UseBinary = true;
            }
            catch (Exception e)
            {
                LogManager.GetLogger("FileMate").ErrorException("Exception while setting up ftp session", e);
            }
        }

        private void LogFtpResponseCode(FtpWebResponse response)
        {
            LogManager.GetLogger("FileMate").Debug(string.Format("[{0}] {1}",
                response.StatusCode, response.StatusDescription));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        string transferringFileName(string inFileName)
        {
            return inFileName + @".partial";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sourceName"></param>
        /// <param name="inNewName"></param>
        void renameTargetFileName(string inTargetName, string inNewName)
        {
            SetupFtpSession(Url + inTargetName);

            _ftpSession.Method = WebRequestMethods.Ftp.Rename;
            _ftpSession.RenameTo = inNewName;

            FtpWebResponse response = (FtpWebResponse)_ftpSession.GetResponse();
            Stream  ftpStream = response.GetResponseStream();

            ftpStream.Close();
            response.Close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStream"></param>
        void closeStream(Stream inStream)
        {
             if (inStream != null)
            {
                inStream.Close();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFtpStream"></param>
        void closeAndFlushFtpStream(Stream inFtpStream)
        {
            if (inFtpStream != null)
            {
                inFtpStream.Flush();
                inFtpStream.Close();
            }
        }

        #endregion
    }
}
