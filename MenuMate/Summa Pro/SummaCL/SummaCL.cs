using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Xml;

using NLog;

using FileMate.Core;
using FCSH = FileMate.Core.EventHub;
using FCE = FileMate.Core.Exceptions;

using LogFileManager;
using ConfigFileManager;

namespace SummaCL
{
    public class Server
    {
        public bool stopped = true;
        private bool _forceToStop = false;

        #region Settings

        private readonly IPAddress _serverAddress;
        private int _port;
        private string _outputFilePath;
        private string _outputFilePathIsAccessDoor;

        //private readonly int _port;
        //private readonly string _outputFilePath;

        private bool _doorAccessFlag = false;

        private TcpListener _tcpListener;
        private CopyGroup _cg = null;
        private Thread _thread;

        private FileLogger _singletonLogger = FileLogger.Instance;
        private ConfigurationFileManager _singletonConfigFile = ConfigurationFileManager.Instance;

        #endregion

        public Server(XmlDocument settings)
        {
            _serverAddress = IPAddress.Any;

            try
            {
                _initServer();
                _initCopyGroups();
                _initCopyGroupErrorHandlers();
                _initTcpListener();
            }
            catch (Exception e)
            {
                _singletonLogger.LogException(FileLogger.LogErrorLevel.AppFailedToStart,
                                              FileLogger.LogErrorLevelFile.None,
                                              @"InitServer failed: ",
                                              e);

                _singletonLogger.LogWarning(@"Summa failed to initiate.  See the log file for details");
            }
        }

        public bool ServerReady
        {
            get
            {
                return _tcpListener != null;
            }
        }

        private void _initServer()
        {
            _port = Int32.Parse(_singletonConfigFile.TcpPort);
            _outputFilePath = _singletonConfigFile.SourceFolder;
            _outputFilePathIsAccessDoor = _singletonConfigFile.IsDoorAccessSourceFolder;

            //::::::::::::::::::::::::::::::

            // If the directory doesn't exist, create it.
            if (!Directory.Exists(_outputFilePath))
            {
                Directory.CreateDirectory(_outputFilePath);
            }

            //.................................

            if (!Directory.Exists(_singletonConfigFile.IsDoorAccessSourceFolder))
            {
                Directory.CreateDirectory(_singletonConfigFile.IsDoorAccessSourceFolder);
            }

            //.................................

            if (!Directory.Exists(_singletonConfigFile.IsDoorAccessTargetFolder))
            {
                Directory.CreateDirectory(_singletonConfigFile.IsDoorAccessTargetFolder);
            }

            //.................................

            //if (!Directory.Exists(_singletonConfigFile.IsDoorAccessArchiveTargetFolder))
            //{
            //    Directory.CreateDirectory(_singletonConfigFile.IsDoorAccessArchiveTargetFolder);
            //}

            //.................................

            string[] tmp = _singletonConfigFile.ArchiveFolder.Split('{');

            if (!Directory.Exists(tmp[0]))
            {
                Directory.CreateDirectory(tmp[0]);
            }

            //:::::::::::::::::::::::::::::::::

            _doorAccessFlag = _singletonConfigFile.IsDoorAccess == @"true";
        }

        private void _initTcpListener()
        {
            _tcpListener = new TcpListener(_serverAddress, _port);
        }

        private void _initCopyGroups()
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(_singletonConfigFile.ConfigPath);

            //doc.Load(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase), @"FileMateConfig.xml"));

            _cg = new CopyGroup(doc);

            // set up schedule event
            if (!_doorAccessFlag)
            {
                _cg.CopyOperations[0].ExecuteEvent += new CopyOperation.ExecuteEventDelegate(Server_ScheduleEvent);
                _singletonLogger.Log(string.Format(@"{0} loaded", @"Summa Upload Proccess"));
            }
            else
            {
                _cg.CopyOperations[0].ExecuteEvent += new CopyOperation.ExecuteEventDelegate(Local_ScheduleEvent);
                _singletonLogger.Log(string.Format(@"{0} loaded", @"Door Access Upload Proccess"));
            }
        }

        #region FileMate Copy Group's error handlers

        private void _initCopyGroupErrorHandlers()
        {
            FCSH.EventHub.AddHandler(FCSH.EhHandlerType.CopyOperationCreation, CopyOperationCreationEEH);
            FCSH.EventHub.AddHandler(FCSH.EhHandlerType.CopyOperationShutdown, CopyOperationShutdownEEH);
            FCSH.EventHub.AddHandler(FCSH.EhHandlerType.BackupCompletion, BackupCompletionEEH);
            FCSH.EventHub.AddHandler(FCSH.EhHandlerType.BackupInitiation, BackupInitiationEEH);
            FCSH.EventHub.AddHandler(FCSH.EhHandlerType.Failure, FailureEEH);
            //FCSH.EventHub.AddHandler(FCSH.EhHandlerType.NumberOfHandlerTypes,  NumberOfHandlerTypesEEH);
        }

        void CopyOperationCreationEEH(Object sender, Object data, ref int status)
        {
            string str = string.Format(@"{0} open", ((CopyOperation)sender).Name);

            _singletonLogger.Log(str);

            this._stop();
        }

        void CopyOperationShutdownEEH(Object sender, Object data, ref int status)
        {
            string str = string.Format(@"{0} closed", ((CopyOperation)sender).Name);

            _singletonLogger.Log(str);

            if (!_forceToStop)
            {
                this.Start();
            }
        }

        void BackupInitiationEEH(Object sender, Object data, ref int status)
        {
            if ((string)data == _singletonConfigFile.SummaUploadCopyOperation)
            {
                _singletonLogger.Log(string.Format(@"{0} files will be both uploaded and archived to the Summa BI FTP Server", _singletonConfigFile.Pattern));
                return;
            }
        }

        void BackupCompletionEEH(Object sender, Object data, ref int status)
        {
            if ((string)data == _singletonConfigFile.DefaultSummaBITarget)
            {
                _singletonLogger.Log(string.Format(@"A file has been uploaded to the Summa BI FTP Server", _singletonConfigFile.Pattern));
                return;
            }

            if ((string)data == _singletonConfigFile.DefaultSummaArchive)
            {
                _singletonLogger.Log(string.Format(@"A file has been archived in {1} folder", _singletonConfigFile.Pattern, _singletonConfigFile.ArchiveFolder));
            }

            //.......................................

            if ((string)data == _singletonConfigFile.DefaulIsDoorAccessArchiveTarget)
            {
                _singletonLogger.Log(string.Format(@"A file has been moved to {1} folder", _singletonConfigFile.IsDoorAccessPattern, _singletonConfigFile.IsDoorAccessArchiveTargetFolder));
            }
        }

        void FailureEEH(Object sender, Object data, ref int status)
        {
            string targetName = ((CopyOperation)sender).Name;
            Exception exception = (Exception)(((Object[])data)[1]);

            _singletonLogger.LogException(FileLogger.LogErrorLevel.UploadFailed,
                                          FileLogger.LogErrorLevelFile.None,
                                          string.Format(@"{0} has failed.", targetName),
                                          exception);


            //::::::::::::::::::::::::::::::::::::::::::::

            if (targetName == _singletonConfigFile.SummaUploadCopyOperation)
            {
                _singletonLogger.LogWarning(@"Summa upload process failed.  See the log file for details");
            }

            //.............................

            if (targetName == _singletonConfigFile.IsDoorAccessCopyOperation)
            {
                _singletonLogger.LogWarning(@"IsDoorAccess file process process failed.  See the log file for details");
            }

            //::::::::::::::::::::::::::::::::::::::::::::

            status = (int)OperationStatus.ForceToTerminate;

            if (!_forceToStop)
            {
                this.Start();
            }
        }

        void NumberOfHandlerTypesEEH(Object sender, Object data, ref int status)
        {
        }

        #endregion

        public void ResetServer()
        {
            _flushAll();

            _initServer();
            _initTcpListener();
            _initCopyGroups();
        }

        void _flushAll()
        {
            this.Stop();

            foreach (CopyOperation co in _cg.CopyOperations)
            {
                co.Cleanup();
            }

            _tcpListener = null;
            _cg = null;

            GC.Collect();
        }

        void Server_ScheduleEvent()
        {
            createZIPFiles();
        }

        void Local_ScheduleEvent()
        {
        }

        void createZIPFiles()
        {
            try
            {
                if (SummaZIP.SaveAndClean(_outputFilePath, SummaFileName.GetZIPName(_outputFilePath)))
                {
                    _singletonLogger.Log("... New ZIP file created");
                }
                else
                {
                    _singletonLogger.Log("... No new ZIP file created");
                }
            }
            catch (Exception e)
            {
                _singletonLogger.LogException(FileLogger.LogErrorLevel.CreateZIPFailed,
                                              FileLogger.LogErrorLevelFile.None,
                                              @"Creating ZIP file returned an error",
                                              e);
                _singletonLogger.LogWarning(@"Summa failed creating a ZIP file.  See the log file for details");
            }
        }

        public void Upload()
        {
            try
            {
                //.................................

                _cg.CopyOperations[0].ExecuteOperation();

                //.................................
            }
            catch (Exception e)
            {
                int status = 0;
                FCSH.EventHub.FailureHandler(_cg.CopyOperations[0], new Object[2] { @"", e }, ref status);
            }

        }

        public void Start()
        {
            if (ServerReady && stopped)
            {
                try
                {
                    _singletonLogger.Log("Starting server ...");
                    _tcpListener.Start();
                    _thread = new Thread(new ThreadStart(AcceptClients));
                    _thread.Start();

                    _singletonLogger.Log("Server Ready - Listening for new connections ...");

                    if (!_doorAccessFlag)
                    {
                        _singletonLogger.Log(string.Format(@"TCP port: {0}. Source: {1}.     Target: {2}.     Pattern: {3}", _singletonConfigFile.TcpPort, _singletonConfigFile.SourceFolder, _singletonConfigFile.Target, _singletonConfigFile.Pattern));
                    }
                    else
                    {
                        _singletonLogger.Log(string.Format(@"TCP port: {0}. Source: {1}.     Target: {2}.     Pattern: {3}", _singletonConfigFile.TcpPort, _singletonConfigFile.SourceFolder, _singletonConfigFile.IsDoorAccessTargetFolder, _singletonConfigFile.IsDoorAccessPattern));
                    }

                    _singletonLogger.Log("Server started OK.");

                    stopped = false;
                    _forceToStop = false;
                }
                catch (Exception e)
                {
                    _singletonLogger.LogException(FileLogger.LogErrorLevel.ServerStartListeningFailed,
                                                  FileLogger.LogErrorLevelFile.None,
                                                  @"Start failed: ",
                                                  e);
                    _singletonLogger.LogWarning(@"Summa failed to start.  See the log file for details");
                }
            }
        }

        /// <summary>Stop server listening</summary>
        private void _stop()
        {
            try
            {
                _singletonLogger.Log("Stopping server ...");

                _tcpListener.Stop();
                _thread.Abort();
                _thread.Join();
                _thread = null;

                _singletonLogger.Log("Server stopped OK.");

                stopped = true;
            }
            catch (Exception e)
            {
                //_singletonLogger.LogWarning(string.Format(@"Exception while trying to shut backgroup thread down: {0}", e.Message));
            }
        }

        /// <summary>Stop server listening</summary>
        public void Stop()
        {
            _forceToStop = true;

            if (!stopped)
            {
                this._stop();
            }
        }

        private void AcceptClients()
        {
            _singletonLogger.Log("Accepting connection ...");

            //_logger.Log(LogLevel.Debug, "Accepting connection ...");

            try
            {
                while (_thread.ThreadState == System.Threading.ThreadState.Running)
                {
                    // AcceptSocket blocks until new connection has established
                    Socket socket = _tcpListener.AcceptSocket();

                    if (socket.Connected)
                    {
                        // Read in and examine the Data for Phoenix or XML Data.
                        NetworkStream networkStream = new NetworkStream(socket);
                        try
                        {
                            StringBuilder data = new StringBuilder();

                            byte[] CharSize = new byte[10];
                            networkStream.Read(CharSize, 0, 10);
                            String StringSize = System.Text.Encoding.ASCII.GetString(CharSize, 0, CharSize.Length).ToString();
                            int size = int.Parse(StringSize);

                            int ByteCount = 0;
                            int inbytes = 0;
                            int NoDataCount = 0;
                            DateTime StartTime = DateTime.Now;
                            TimeSpan duration = StartTime - StartTime;
                            while (ByteCount != size && duration.Seconds < 10)
                            {
                                int ReminaingBytes = size - ByteCount;
                                byte[] buffer = new byte[ReminaingBytes];
                                inbytes = networkStream.Read(buffer, 0, buffer.Length);
                                if (inbytes > 0)
                                {
                                    //char[] cArray = System.Text.Encoding.ASCII.GetString(buffer, 0, inbytes).ToString(); ToCharArray();
                                    //data.Append(cArray, 0, cArray.Length);                                   
                                    data.Append(System.Text.Encoding.ASCII.GetString(buffer, 0, inbytes).ToString());
                                    StartTime = DateTime.Now;
                                }
                                ByteCount += inbytes;
                                duration = DateTime.Now - StartTime;
                            }

                            XMLProcessor xmlProcessor = new XMLProcessor(networkStream);
                            if (data.Length == size)
                            {
                                xmlProcessor.Process(data.ToString(), _outputFilePath, _outputFilePathIsAccessDoor, _doorAccessFlag);
                            }
                            else
                            {
                                StreamWriter streamWriter = new StreamWriter(networkStream);
                                streamWriter.AutoFlush = true;
                                streamWriter.WriteLine(xmlProcessor.BuildResponse("1", "NAK", "Incomplete Message").OuterXml);
                            }
                        }
                        catch (IOException ioe)
                        {
                            _singletonLogger.LogWarning(string.Format(@"IOException when processing client: {0}", ioe.Message));
                        }
                        catch (Exception e)
                        {
                            _singletonLogger.LogWarning(string.Format(@"Exception when processing client: {0}", e.Message));
                        }
                        finally
                        {
                            Thread.Sleep(new TimeSpan(0, 0, 5)); // Sleep for 5 seconds. Giving the client (Menumate) some time to finish the transaction

                            networkStream.Close();
                            socket.Close();
                        }
                    }
                }
            }
            catch (SocketException ex)
            {
                // An exception will be thrown when tcpListerner.Stop() is called	
                //_singletonLogger.LogException("SocketException: {0}", ex);


                //_logger.Fatal("SocketException: {0}", ex.Message);
            }
        }
    }
}
