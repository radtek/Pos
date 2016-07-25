using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using LogFileManager;

using System.Xml;

using XMLManager;

namespace ConfigFileManager
{
    using SiteIDMap = Dictionary<string, string>;

    public sealed class ConfigurationFileManager
    {
        public enum ConfigFileError
        {
            None,
            InvalidFile
        };

        FileLogger _singletonLogger = FileLogger.Instance;

        XmlDocument _configFileDoc;

        string _isDoorAccessCopyOperation = @"Door Access Upload Proccess";
      
        string _summaUploadCopyOperation = @"Summa Upload Proccess";
        string _summaUploadTarget        = @"summaBITarget";
        string _summaUploadArchiveTarget = @"summaArchive";

        string _version                          = @"1.03";

        string _enableNotificationsDefault       = @"false";

        string _isDoorAccessDefault              = @"false";
        string _isDoorAccessSourceFolderDefault  = @".\upload.dooraccess\";
        string _isDoorAccessFolderDefault        = @".\export.dooraccess\";
        string _isDoorAccessArchiveFolderDefault = @".\archive.dooraccess\{date}";
        string _isDoorAccessScheduleDefault      = @"5m";
        string _isDoorAccessPatternDefault       = @"*.csv";
        string _locationNameDefault              = @"";

        string _isDoorAccessTarget = @"IsDoorAccessTarget";
        string _isDoorAccessArchiveTarget = @"IsDoorAccessArchiveTarget";

        string _configFolder        = @".\config";

        string _listenPortDefault   = @"4444";
        string _sourceFolderDefault = @".\upload\";
        
        string _siteIDDefault      = @"99"; 
        string _summaSiteIDDefault = @"3100";

        string _ftpURLDefault      = @"ftp://ftp.summa.co.nz/";
        string _ftpUsernameDefault = @"menumate";
        string _ftpPasswordDefault = @"aceking21";

        string _summaBITargetDefault = @"summaBITarget";
        string _summaArchiveDefault  = @"summaArchive";

        string _archiveURLDefault = @".\archive\{date}";

        string _scheduleDefault    = @"5m";
        string _filePatternDefault = @"*.zip";

        ConfigFileError _configError;
        string          _configErrorStr;
        public delegate void NotifErrorFuncDelegate(ConfigFileError outError, string outErrorStr);

        private static FileLogger.NotifFuncDelegate notifNewConfigFileCreatedFunc;
        private static NotifErrorFuncDelegate       notifConfigFileLoadedFunc;
        private static NotifErrorFuncDelegate       notifConfigFileLoadFailedFunc;

        ConfigurationFileManager()
        {
        }

        public static ConfigurationFileManager Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly ConfigurationFileManager instance = new ConfigurationFileManager();
        }

        //:::::::::::::::::::::::::::::::::

        #region Properties

        public string ConfigPath
        {
            get
            {
                return string.Format(@"{0}\SummaConfig.xml", _configFolder);
            }
        }

        //.................................

        public string LocationName
        {
            get
            {
                return _configFileDoc.DocumentElement.Attributes[@"LocationName"].Value;
            }
            set
            {
                _configFileDoc.DocumentElement.Attributes[@"LocationName"].Value = value;
            }
        }

        //.................................

        public string EnableNotifications
        {
            get
            {
                return _configFileDoc.DocumentElement.Attributes[@"EnableNotifications"].Value;
            }
            set
            {
                _configFileDoc.DocumentElement.Attributes[@"EnableNotifications"].Value = value;
            }
        }

        //.................................

        public string IsDoorAccessCopyOperation
        {
            get
            {
                return _isDoorAccessCopyOperation;
            }
        }

        //.................................

        public string SummaUploadCopyOperation
        {
            get
            {
                return _summaUploadCopyOperation;
            }
        }

        //.................................

        public string IsDoorAccess
        {
            get
            {
                return _configFileDoc.DocumentElement.Attributes[@"IsDoorAccess"].Value;
            }
            set
            {
                _configFileDoc.DocumentElement.Attributes[@"IsDoorAccess"].Value = value;

                setCopyOperationEnable(_isDoorAccessCopyOperation, value);
                setCopyOperationEnable(_summaUploadCopyOperation, NotString(value));
            }
        }

        //.................................

        public string IsDoorAccessSourceFolder
        {
            get
            {
                return getSourceUrl(_isDoorAccessCopyOperation);
            }
            set
            {
                setSourceUrl(_isDoorAccessCopyOperation, value);
            }
        }

        //.................................

        public string IsDoorAccessTargetFolder
        {
            get
            {
                return _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings").Attributes[_isDoorAccessTarget].Value;
            }
            set
            {
                _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings").Attributes[_isDoorAccessTarget].Value = value;
            }

/*
            get
            {
                return getTargetUrl(_isDoorAccessCopyOperation, _isDoorAccessTarget);
            }
            set
            {
                setTargetUrl(_isDoorAccessCopyOperation, _isDoorAccessTarget, value);
            }
*/
        }

        //.................................

        public string IsDoorAccessArchiveTargetFolder
        {
            get
            {
                return getTargetUrl(_isDoorAccessCopyOperation, _isDoorAccessArchiveTarget);
            }
            set
            {
                setTargetUrl(_isDoorAccessCopyOperation, _isDoorAccessArchiveTarget, value);
            }
        }

        //.................................

        public string IsDoorAccessSchedule
        {
            get
            {
                return getSchedule(_isDoorAccessCopyOperation);
            }
            set
            {
                setSchedule(_isDoorAccessCopyOperation, value);
            }
        }

        //.................................

        public string IsDoorAccessPattern
        {
            get
            {
                return getPattern(_isDoorAccessCopyOperation);
            }
            set
            {
                setPattern(_isDoorAccessCopyOperation, value);
            }
        }

        //.................................

        public string TcpPort
        {
            get
            {
               return _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings").Attributes[@"ListenPort"].Value;
            }
            set
            {
               _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings").Attributes[@"ListenPort"].Value = value;
            }
        }

        //.................................

        public SiteIDMap[] SiteIDMapInfo
        {
            get
            {
                List<SiteIDMap> result = new List<SiteIDMap>();

                //:::::::::::::::::::::::::::::

                XmlNodeList summaSiteIdMapEntries = _configFileDoc.DocumentElement.SelectNodes(@"./SummaSiteIdMap/SummaIdMapEntry");

                foreach (XmlNode mapEntry in summaSiteIdMapEntries)
                {
                    SiteIDMap siteIDMap = new SiteIDMap();
                    siteIDMap.Add(mapEntry.Attributes[@"siteId"].Value, mapEntry.Attributes[@"summaSiteId"].Value);

                    result.Add(siteIDMap);
                }

                //:::::::::::::::::::::::::::::

                return result.ToArray();
            }
            set
            {
                XmlNode summaSiteIdMap = _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSiteIdMap");
                summaSiteIdMap.RemoveAll();

                XmlNode siteIDMapNode = _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSiteIdMap");

                foreach (SiteIDMap siteIDMap in value)
                {
                    XmlNode siteIDMapEntryNode = XMLDocManager.AddNodeChild(_configFileDoc, siteIDMapNode, @"SummaIdMapEntry");

                    XMLDocManager.AddAttribute(_configFileDoc, siteIDMapEntryNode, @"siteId", siteIDMap.Keys.ElementAt(0));
                    XMLDocManager.AddAttribute(_configFileDoc, siteIDMapEntryNode, @"summaSiteId", siteIDMap.Values.ElementAt(0));
                }
            }
        }

        //.................................

        public string Schedule 
        {
            get
            {
                return getSchedule(_summaUploadCopyOperation);
            }
            set
            {
                setSchedule(_summaUploadCopyOperation, value);
            }
        }

        //.................................
                    
        public string Pattern
        {
            get
            {
                return getPattern(_summaUploadCopyOperation);
            }
            set
            {
                setPattern(_summaUploadCopyOperation, value);
            }
        }

        //.................................

        public string UserName
        {
            get
            {
               XmlNode copyOperation = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup/CopyOperation");

               XmlNode     targets    = copyOperation.SelectSingleNode(@"./targets");
               XmlNodeList targetList = targets.SelectNodes(@"./target");

               string result = @"";

               foreach (XmlNode target in targetList)
               {
                    if (target.Attributes[@"name"].Value == @"summaBITarget")
                    {
                        result = target.SelectSingleNode(@"./credentials").Attributes[@"username"].Value;
                        break;
                    }
               }

               return result;
            }
            set
            {
                XmlNode copyOperation = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup/CopyOperation");

                XmlNode targets = copyOperation.SelectSingleNode(@"./targets");
                XmlNodeList targetList = targets.SelectNodes(@"./target");

                foreach (XmlNode target in targetList)
                {
                    if (target.Attributes[@"name"].Value == @"summaBITarget")
                    {
                        target.SelectSingleNode(@"./credentials").Attributes[@"username"].Value = value;
                        break;
                    }
                }
            }
        }

        //.................................

        public string Password
        {
            get
            {
               XmlNode copyOperation = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup/CopyOperation");

               XmlNode     targets    = copyOperation.SelectSingleNode(@"./targets");
               XmlNodeList targetList = targets.SelectNodes(@"./target");

               string result = @"";

               foreach (XmlNode target in targetList)
               {
                    if (target.Attributes[@"name"].Value == @"summaBITarget")
                    {
                        result = target.SelectSingleNode(@"./credentials").Attributes[@"password"].Value;
                        break;
                    }
               }

               return result;
            }
            set
            {
               XmlNode copyOperation = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup/CopyOperation");

               XmlNode     targets    = copyOperation.SelectSingleNode(@"./targets");
               XmlNodeList targetList = targets.SelectNodes(@"./target");

               foreach (XmlNode target in targetList)
               {
                    if (target.Attributes[@"name"].Value == @"summaBITarget")
                    {
                        target.SelectSingleNode(@"./credentials").Attributes[@"password"].Value = value;
                        break;
                    }
               }
            }
        }

        //.................................

        public string SourceFolder
        {
            get
            {
               return _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings").Attributes[@"OutputFilePath"].Value;
            }
            set
            {
                _configFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings").Attributes[@"OutputFilePath"].Value = value;
                setSourceUrl(_summaUploadCopyOperation, value);

                //_configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup/CopyOperation/source").Attributes[@"url"].Value = value;
            }
        }

        //.................................

        public string Target
        {
            get
            {
                return getTargetUrl(_summaUploadCopyOperation, _summaUploadTarget);
            }
        }

        //.................................

        public string ArchiveFolder
        {
            get
            {
                return getTargetUrl(_summaUploadCopyOperation, _summaUploadArchiveTarget);
            }
            set
            {
                setTargetUrl(_summaUploadCopyOperation, _summaUploadArchiveTarget, value);
            }
        }

        //::::::::::::::::::::::::::::::::::

        public string getCopyOperationEnable(string inCopyOperationName)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::::::

            XmlNode     copyGroup      = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    result = copyOperation.Attributes[@"enabled"].Value;
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        //.................................

        public void setCopyOperationEnable(string inCopyOperationName, string inEnableValue)
        {
            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    copyOperation.Attributes[@"enabled"].Value = inEnableValue;
                    break;
                }
            }
        }

        //.................................

        public string getSourceUrl(string inCopyOperationName)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::::::

            XmlNode     copyGroup      = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    result = copyOperation.SelectSingleNode(@"./source").Attributes[@"url"].Value;
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        //.................................

        public void setSourceUrl(string inCopyOperationName, string inUrlValue)
        {
            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    copyOperation.SelectSingleNode(@"./source").Attributes[@"url"].Value = inUrlValue;
                    break;
                }
            }
        }

        //.................................

        public string getTargetUrl(string inCopyOperationName, string inTargetName)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::::::

            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    XmlNode targets = copyOperation.SelectSingleNode(@"./targets");
                    XmlNodeList targetList = targets.SelectNodes(@"./target");

                    foreach (XmlNode target in targetList)
                    {
                        if (target.Attributes[@"name"].Value == inTargetName)
                        {
                            result = target.Attributes[@"url"].Value;
                            break;
                        }
                    }
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        //.................................

        public void setTargetUrl(string inCopyOperationName, string inTargetName, string inUrlValue)
        {
            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    XmlNode targets = copyOperation.SelectSingleNode(@"./targets");
                    XmlNodeList targetList = targets.SelectNodes(@"./target");

                    foreach (XmlNode target in targetList)
                    {
                        if (target.Attributes[@"name"].Value == inTargetName)
                        {
                            target.Attributes[@"url"].Value = inUrlValue;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        //.................................

        public string getPattern(string inCopyOperationName)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::::::

            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    result = copyOperation.SelectSingleNode(@"./pattern").InnerText;
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        //.................................

        public void setPattern(string inCopyOperationName, string inPatternValue)
        {
            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    copyOperation.SelectSingleNode(@"./pattern").InnerText = inPatternValue;
                    break;
                }
            }
        }

        //.................................

        public string getSchedule(string inCopyOperationName)
        {
            string result = @"";

            //:::::::::::::::::::::::::::::::::

            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    result = copyOperation.SelectSingleNode(@"./schedule").InnerText;
                    break;
                }
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        //.................................

        public void setSchedule(string inCopyOperationName, string inScheduleValue)
        {
            XmlNode copyGroup = _configFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup");
            XmlNodeList copyOperations = copyGroup.SelectNodes(@"./CopyOperation");

            foreach (XmlNode copyOperation in copyOperations)
            {
                if (copyOperation.Attributes[@"name"].Value == inCopyOperationName)
                {
                    copyOperation.SelectSingleNode(@"./schedule").InnerText = inScheduleValue;
                    break;
                }
            }
        }

        #endregion

        //.................................

        #region Defaults

        public string DefaulIsDoorAccess
        {
            get
            {
                return _isDoorAccessDefault;
            }
        }

        //.................................

        public string DefaulIsDoorAccessTarget
        {
            get
            {
                return _isDoorAccessTarget;
            }
        }

        //.................................

        public string DefaulIsDoorAccessArchiveTarget
        {
            get
            {
                return _isDoorAccessArchiveTarget;
            }
        }

        //.................................

        public string DefaulIsDoorAccessSourceFolder
        {
            get
            {
                return _isDoorAccessSourceFolderDefault;
            }
         }

        //.................................

        public string DefaultIsDoorAccessTargetFolder
        {
            get
            {
                return _isDoorAccessFolderDefault;
            }
        }

        //.................................

        public string DefaultIsDoorAccessArchiveTargetFolder
        {
            get
            {
                return _isDoorAccessArchiveFolderDefault;
            }
        }

        //.................................

        public string DefaultIsDoorAccessSchedule
        {
            get
            {
                return _isDoorAccessScheduleDefault;
            }
         }

        //.................................

        public string DefaultIsDoorAccessPattern
        {
            get
            {
                return _isDoorAccessPatternDefault;
            }
         }

        //.................................

        public string DefaultLocationName
        {
            get
            {
                return _locationNameDefault;
            }
        }

        //.................................

        public string DefaultEnableNotifications
        {
            get
            {
                return _enableNotificationsDefault;
            }
        }

        //.................................

        public string DefaultTcpPort
        {
            get
            {
                return _listenPortDefault;
            }
        }

        //.................................

        public string DefaultSchedule
        {
            get
            {
                return _scheduleDefault;
            }
        }

        //.................................

        public string DefaultPattern
        {
            get
            {
                return _filePatternDefault;
            }
        }

        //.................................

        public SiteIDMap[] DefaultSiteIDMapInfo
        {
            get
            {
                List<SiteIDMap> result = new List<SiteIDMap>();

                //:::::::::::::::::::::::::::::

                SiteIDMap siteIDMap = new SiteIDMap();
                siteIDMap.Add(_siteIDDefault, _summaSiteIDDefault);

                result.Add(siteIDMap);

                //:::::::::::::::::::::::::::::

                return result.ToArray();
            }
        }

        //.................................

        public string DefaultUserName
        {
            get
            {
                return _ftpUsernameDefault;
            }
        }

        //.................................

        public string DefaultPassword
        {
            get
            {
                return _ftpPasswordDefault;
            }
        }

        //.................................

        public string DefaultSummaBITarget
        {
            get
            {
                return _summaBITargetDefault;
            }
        }

        //.................................

        public string DefaultSummaArchive
        {
            get
            {
                return _summaArchiveDefault;
            }
        }

        //.................................

        public string DefaultSourceFolder
        {
            get
            {
                return _sourceFolderDefault;
            }
        }

        //.................................

        public string DefaultArchiveFolder
        {
            get
            {
                return _archiveURLDefault;
            }
        }

        #endregion

        //.................................

        public void initConfigFileManager()
        {
            try
            {
                if (!Directory.Exists(_configFolder)) // Config Folder does not exitst
                {
                    Directory.CreateDirectory(_configFolder);
                    CreateDefaultConfigFile();

                    if (notifNewConfigFileCreatedFunc != null)
                    {
                        notifNewConfigFileCreatedFunc();
                    }
                }
                else
                {
                    if (!File.Exists(ConfigPath))  // Config File does not exitst
                    {
                        CreateDefaultConfigFile();

                        if (notifNewConfigFileCreatedFunc != null)
                        {
                            notifNewConfigFileCreatedFunc();
                        }
                    }
                    else
                    {
                        try
                        {
                            loadConfigFile();

                            if (double.Parse(_configFileDoc.DocumentElement.Attributes[@"Version"].Value) < double.Parse(_version))
                            {
                                _singletonLogger.LogWarning(@"An older Config File's version has been detected.  A new Config File will be create. The current one will be renamed.");

                                SiteIDMap[] siteIDMap = SiteIDMapInfo;

                                ReBuildConfigFile();

                                SiteIDMapInfo = siteIDMap;

                                if (notifNewConfigFileCreatedFunc != null)
                                {
                                    notifNewConfigFileCreatedFunc();
                                }
                            }
                            else
                            {
                                if (notifConfigFileLoadedFunc != null)
                                {
                                    notifConfigFileLoadedFunc(_configError, _configErrorStr);
                                }
                            }
                        }
                        catch(Exception)
                        {
                            if (notifConfigFileLoadFailedFunc != null)
                            {
                                notifConfigFileLoadFailedFunc(_configError, _configErrorStr);
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                _configError = ConfigFileError.InvalidFile;
                _configErrorStr = string.Format(@"Creating Config file folder", e.Message);

                if (notifConfigFileLoadFailedFunc != null)
                {
                    notifConfigFileLoadFailedFunc(_configError, _configErrorStr);
                }
            }
        }

        //.................................

        public void setNotifNewConfigFileCreatedFunc(FileLogger.NotifFuncDelegate inNotifNewConfigFileCreatedFunc)
        {
            notifNewConfigFileCreatedFunc = inNotifNewConfigFileCreatedFunc;
        }

        //.................................

        public void setNotifConfigFileLoadedFun(NotifErrorFuncDelegate inNotifConfigFileLoadedFunc)
        {
            notifConfigFileLoadedFunc = inNotifConfigFileLoadedFunc;
        }

        //.................................

        public void setNotifConfigFileLoadFailedFunc(NotifErrorFuncDelegate inNotifConfigFileLoadFailedFunc)
        {
            notifConfigFileLoadFailedFunc = inNotifConfigFileLoadFailedFunc;
        }

        //.................................

        public void saveAll(string inTcpPort, SiteIDMap[] inSiteIPMapInfo, string inSchedule, string inPattern, string inUsername, 
                            string inPassw, string inSourceFolder, string inArchiveFolder)
        {
            TcpPort       = inTcpPort;
            SiteIDMapInfo = inSiteIPMapInfo;
            Schedule      = inSchedule;
            Pattern       = inPattern;
            UserName      = inUsername;
            Password      = inPassw;
            SourceFolder  = inSourceFolder;
            ArchiveFolder = inArchiveFolder;

            _configFileDoc.Save(ConfigPath);
        }

        //.................................
        
        public void saveLocation(string inLocationName, string inIsDoorAccess, string inIsDoorAccessSourceFolder,
                                 string inIsDoorAccessTargetFolder, string inIsDoorAccessArchiveTargetFolder, 
                                 string inIsDoorAccessSchedule, string inIsDoorAccessPattern)
        {
            LocationName = inLocationName;
            IsDoorAccess = inIsDoorAccess;

            IsDoorAccessSourceFolder        = inIsDoorAccessSourceFolder;
            IsDoorAccessTargetFolder        = inIsDoorAccessTargetFolder;
            IsDoorAccessArchiveTargetFolder = inIsDoorAccessArchiveTargetFolder;
            IsDoorAccessSchedule            = inIsDoorAccessSchedule;
            IsDoorAccessPattern             = inIsDoorAccessPattern;

            _configFileDoc.Save(ConfigPath);
        }

        //.................................

        public void saveNotifications(string inEnableNotifications)
        {
            EnableNotifications = inEnableNotifications;

            _configFileDoc.Save(ConfigPath);
        }

        //:::::::::::::::::::::::::::::::::

        private void loadConfigFile()
        {
            XmlTextReader reader = new XmlTextReader(ConfigPath);

            try
            {
                reader.Read();

                _configFileDoc = new XmlDocument();
                _configFileDoc.Load(reader);

                checkConfigFile(_configFileDoc);

                reader.Close();
            }
            catch(Exception exc)
            {
                reader.Close();

                ReBuildConfigFile();

                _configError = ConfigFileError.InvalidFile;
                _configErrorStr = string.Format(@"Invalid Configuration File.  The current file has been renamed. A Default Configuration File has been created. Reason[{0}]", exc.Message);
            }
        }

        //:::::::::::::::::::::::::::::::::

        private void checkConfigFile(XmlDocument inConfigFileDoc)
        {
            checkSummaMapID(inConfigFileDoc);
            checkSummaSettings(inConfigFileDoc);
            checkSummaCopyOperation(inConfigFileDoc);
        }

        //:::::::::::::::::::::::::::::::::

        private void checkSummaMapID(XmlDocument inConfigFileDoc)
        {
            bool check = false;

            XmlNodeList summaSiteIdMapEntries = null;

            try
            {
                summaSiteIdMapEntries = inConfigFileDoc.DocumentElement.SelectNodes(@"./SummaSiteIdMap/SummaIdMapEntry");

                foreach (XmlNode mapEntry in summaSiteIdMapEntries)
                {
                    check = (mapEntry.Attributes[@"siteId"] != null) &&
                            (mapEntry.Attributes[@"summaSiteId"] != null);
                }
            }
            catch(Exception exc)
            {
                throw new Exception(string.Format(@"{0}: Invalid SummaID Map Entry", exc.Message));
            }

            if (!check)
            {
                throw new Exception(@"Invalid SummaID Map Entry");
            }
        }

        //:::::::::::::::::::::::::::::::::

        private void checkSummaSettings(XmlDocument inConfigFileDoc)
        {
            bool check = false;

            XmlNode summaSettings = null;

            try
            {
                summaSettings = inConfigFileDoc.DocumentElement.SelectSingleNode(@"./SummaSettings");

                check = (summaSettings.Attributes[@"ListenPort"] != null) &&
                        (summaSettings.Attributes[@"OutputFilePath"] != null);
            }
            catch (Exception exc)
            {
                throw new Exception(string.Format(@"{0}: Invalid Summa Settings", exc.Message));
            }

            if (!check)
            {
                throw new Exception(@"Invalid Summa Settings");
            }
        }

        //:::::::::::::::::::::::::::::::::

        private void checkSummaCopyOperation(XmlDocument inConfigFileDoc)
        {
            bool check = false;

            XmlNode copyOperation = null;

            try
            {
                copyOperation = inConfigFileDoc.DocumentElement.SelectSingleNode(@"./FileMate/CopyGroup/CopyOperation");

                check = (copyOperation.Attributes[@"name"] != null) &&
                        (copyOperation.Attributes[@"deleteSource"] != null) &&
                        (copyOperation.Attributes[@"calledFromExternal"] != null);

                if (!check)
                {
                    throw new Exception(@"Invalid FileMate Copy Operation");
                }

                //........................................

                XmlNode source = copyOperation.SelectSingleNode(@"./source");

                check = (source.Attributes[@"url"] != null) &&
                        (source.Attributes[@"name"] != null);


                if (!check)
                {
                    throw new Exception(@"Invalid FileMate Source");
                }

                //........................................

                try
                {
                    XmlNode     targets    = copyOperation.SelectSingleNode(@"./targets");
                    XmlNodeList targetList = targets.SelectNodes(@"./target");

                    foreach (XmlNode target in targetList)
                    {
                        check = (target.Attributes[@"url"] != null) &&
                                (target.Attributes[@"name"] != null);

                        if (target.Attributes[@"name"].Value == @"summaBITarget")
                        {
                            XmlNode credentials = target.SelectSingleNode(@"./credentials");

                            check = (credentials.Attributes[@"username"] != null) &&
                                    (credentials.Attributes[@"password"] != null);

                            if (!check)
                            {
                                throw new Exception(@"Invalid FileMate Target Credentials");
                            }
                        }
                    }
                }
                catch(Exception)
                {
                }

                if (!check)
                {
                    throw new Exception(@"Invalid FileMate Target");
                }

                //........................................

                XmlNode schedule = copyOperation.SelectSingleNode(@"./schedule");

                check = schedule.InnerText != @"";

                if (!check)
                {
                    throw new Exception(@"Invalid FileMate Schedule");
                }

                //........................................

                XmlNode pattern = copyOperation.SelectSingleNode(@"./pattern");

                check = pattern.InnerText != @"";

                if (!check)
                {
                    throw new Exception(@"Invalid FileMate File Pattern");
                }
            }
            catch (Exception exc)
            {
                throw new Exception(string.Format(@"{0}: Invalid Summa Copy Operation", exc.Message));
            }
        }

        //:::::::::::::::::::::::::::::::::

        private void ReBuildConfigFile()
        {
            if(File.Exists(string.Format(@"{0}.bck", ConfigPath)))
            {
                File.Delete(string.Format(@"{0}.bck", ConfigPath));
            }

            File.Move(ConfigPath, string.Format(@"{0}.bck", ConfigPath));
            CreateDefaultConfigFile();
        }

        //:::::::::::::::::::::::::::::::::

        private void CreateDefaultConfigFile()
        {
            // Create the xml document container
            _configFileDoc = new XmlDocument();

            //::::::::::::::::::::::::::::::

            // Create the XML Declaration, and append it to XML document
            XMLDocManager.CreateXmlDeclaration(_configFileDoc, @"1.0", null, null);

            //::::::::::::::::::::::::::::::

            // Create the root element
            XmlElement root = XMLDocManager.CreateRoot(_configFileDoc, @"SummaConfig");

            //::::::::::::::::::::::::::::::

            // Summa Config Version
            XMLDocManager.AddAttribute(_configFileDoc, root, @"Version", _version);
            XMLDocManager.AddAttribute(_configFileDoc, root, @"IsDoorAccess", _isDoorAccessDefault);
            XMLDocManager.AddAttribute(_configFileDoc, root, @"LocationName", _locationNameDefault);
            XMLDocManager.AddAttribute(_configFileDoc, root, @"EnableNotifications", _enableNotificationsDefault);

            // Summa Site ID Map
            XmlNode siteIDMap      = XMLDocManager.AddNode(_configFileDoc, root, @"SummaSiteIdMap");
                XmlNode siteIDMapEntry = XMLDocManager.AddNodeChild(_configFileDoc, siteIDMap, @"SummaIdMapEntry");
                XMLDocManager.AddAttribute(_configFileDoc, siteIDMapEntry, @"siteId", @"99");
                XMLDocManager.AddAttribute(_configFileDoc, siteIDMapEntry, @"summaSiteId", @"31000");

            // Summa Settings
            XmlNode summaSettings = XMLDocManager.AddNode(_configFileDoc, root, @"SummaSettings");
            XMLDocManager.AddAttribute(_configFileDoc, summaSettings, @"ListenPort", _listenPortDefault);
            XMLDocManager.AddAttribute(_configFileDoc, summaSettings, @"OutputFilePath", _sourceFolderDefault);
            XMLDocManager.AddAttribute(_configFileDoc, summaSettings, _isDoorAccessTarget, _isDoorAccessFolderDefault);

            // FileMate
            XmlNode fileMate = XMLDocManager.AddNode(_configFileDoc, root, @"FileMate");
                XmlNode fileMateCopyGroup = XMLDocManager.AddNodeChild(_configFileDoc, fileMate, @"CopyGroup");
                    XmlNode fileMateCopyOperation = XMLDocManager.AddNodeChild(_configFileDoc, fileMateCopyGroup, @"CopyOperation");
                    XMLDocManager.AddAttribute(_configFileDoc, fileMateCopyOperation, @"name", @"Summa Upload Proccess");
                    XMLDocManager.AddAttribute(_configFileDoc, fileMateCopyOperation, @"deleteSource", @"true");
                    XMLDocManager.AddAttribute(_configFileDoc, fileMateCopyOperation, @"calledFromExternal", @"true");
                    XMLDocManager.AddAttribute(_configFileDoc, fileMateCopyOperation, @"enabled", @"true");
                        XmlNode fileMateSource = XMLDocManager.AddNodeChild(_configFileDoc, fileMateCopyOperation, @"source");
                        XMLDocManager.AddAttribute(_configFileDoc, fileMateSource, @"url", _sourceFolderDefault);
                        XMLDocManager.AddAttribute(_configFileDoc, fileMateSource, @"name", @"summaSource");
                        XmlNode fileMateTargets = XMLDocManager.AddNodeChild(_configFileDoc, fileMateCopyOperation, @"targets");
                            XmlNode fileMateFTPTarget = XMLDocManager.AddNodeChild(_configFileDoc, fileMateTargets, @"target");
                            XMLDocManager.AddAttribute(_configFileDoc, fileMateFTPTarget, @"url", _ftpURLDefault);
                            XMLDocManager.AddAttribute(_configFileDoc, fileMateFTPTarget, @"name", _summaBITargetDefault);
                                XmlNode fileMateFTPCredentials = XMLDocManager.AddNodeChild(_configFileDoc, fileMateFTPTarget, @"credentials");
                                XMLDocManager.AddAttribute(_configFileDoc, fileMateFTPCredentials, @"username", _ftpUsernameDefault);
                                XMLDocManager.AddAttribute(_configFileDoc, fileMateFTPCredentials, @"password", _ftpPasswordDefault);
                            XmlNode fileMateArchiveTarget = XMLDocManager.AddNodeChild(_configFileDoc, fileMateTargets, @"target");
                            XMLDocManager.AddAttribute(_configFileDoc, fileMateArchiveTarget, @"url", _archiveURLDefault);
                            XMLDocManager.AddAttribute(_configFileDoc, fileMateArchiveTarget, @"name", _summaArchiveDefault);
                        XmlNode fileMateSchedule = XMLDocManager.AddNodeChild(_configFileDoc, fileMateCopyOperation, @"schedule");
                        fileMateSchedule.InnerText = _scheduleDefault;
                        XmlNode fileMatePattern = XMLDocManager.AddNodeChild(_configFileDoc, fileMateCopyOperation, @"pattern");
                        fileMatePattern.InnerText = _filePatternDefault;

                   XmlNode isDoorAccessCopyOperation = XMLDocManager.AddNodeChild(_configFileDoc, fileMateCopyGroup, @"CopyOperation");
                   XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessCopyOperation, @"name", _isDoorAccessCopyOperation);
                   XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessCopyOperation, @"deleteSource", @"true");
                   XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessCopyOperation, @"calledFromExternal", @"true");
                   XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessCopyOperation, @"enabled", _isDoorAccessDefault);
                        XmlNode isDoorAccessSource = XMLDocManager.AddNodeChild(_configFileDoc, isDoorAccessCopyOperation, @"source");
                        XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessSource, @"url", _isDoorAccessSourceFolderDefault);
                        XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessSource, @"name", @"isDoorAccessSource");
                        XmlNode isDoorAccessTargets = XMLDocManager.AddNodeChild(_configFileDoc, isDoorAccessCopyOperation, @"targets");
                            XmlNode isDoorAccessArchiveTarget = XMLDocManager.AddNodeChild(_configFileDoc, isDoorAccessTargets, @"target");
                            XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessArchiveTarget, @"url", _isDoorAccessArchiveFolderDefault);
                            XMLDocManager.AddAttribute(_configFileDoc, isDoorAccessArchiveTarget, @"name", _isDoorAccessArchiveTarget);
                        XmlNode isDoorAccessSchedule = XMLDocManager.AddNodeChild(_configFileDoc, isDoorAccessCopyOperation, @"schedule");
                        isDoorAccessSchedule.InnerText = _isDoorAccessScheduleDefault;
                        XmlNode isDoorAccessPattern = XMLDocManager.AddNodeChild(_configFileDoc, isDoorAccessCopyOperation, @"pattern");
                        isDoorAccessPattern.InnerText = _isDoorAccessPatternDefault;
            
            //::::::::::::::::::::::::::::::

            _configFileDoc.Save(ConfigPath);
        }

        string NotString(string inStr)
        {
           switch(inStr.ToUpper())
           {
               case @"TRUE":  return @"false";
               case @"FALSE": return @"true";
               default: return inStr;
           }
        }
    }
}
