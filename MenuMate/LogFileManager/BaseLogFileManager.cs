using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using System.Xml;

using NLog;
using NLog.Config;
using NLog.Targets;

using XMLManager;

namespace LogFileManager
{
    public sealed class FileLogger
    {
        public enum LoggerType
        {
            loggerScreen, loggerFile, loggerEmail, loggerScreenErr, loggerFileErr
        };

        public enum LogErrorLevel
        {
            None, AppFailedToStart, SiteIDMapFailed, InvalidSiteID, InternalXMLError, CreateZIPFailed, UploadFailed, ServerStartListeningFailed
        };

        public enum LogErrorLevelFile
        {
            None, ConfigFile, TransactionXMLFile, UpdateMemberXMLFile, ListCategoriesXMLFile, ListProductTypesXMLFile,
            ListPaymentTypesXMLFile, ListGroupsXMLFile, ListStaffXMLFile, ListDiscountsXMLFile, ListCalculatedXMLFile,
            TotalsDiscountsXMLFile, ListMemberGroupsXMLFile, ListRosterXMLFile, RequestXMLFile, VersionXMLFile, UnknownXMLFile
        };

        //:::::::::::::::::::::::::::::::::

        #region Logger Levels

        /// Trace
        /// Debug
        /// Info
        /// Warning
        /// Error

        #endregion

        private List<Logger> _loggers = new List<Logger>();
        private XmlDocument _errorMailBox = new XmlDocument();  //Output error mail box
        private string _errorMailBoxPath = @".\logs\errorMailBox.xml";
        private List<XmlNode> _errorMailBoxEntries = new List<XmlNode>();

        private string _archiveMailBoxFolder = @".\logs\archive\";

        //private static DateTime eMailNoticeTime = DateTime.Now.AddDays(-1); // 1 day back   
        //private static DateTime eMailNoticeTime = DateTime.Now.AddMinutes(-11); // 11 minutes back   

        public delegate void NotifFuncDelegate();
        
        public delegate void NotifLogErrorFuncDelegate(LogErrorLevel inErrorLevel, string inTimeStr, LogErrorLevelFile inErrorLevelFile, string inErrorStr, string inErrorStrDetail, bool inSent);
        private NotifLogErrorFuncDelegate onErrorFunc;

        private bool _notificationsEnabled = false;

        FileLogger()
        {
        }

        public static FileLogger Instance
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

            internal static readonly FileLogger instance = new FileLogger();
        }

        //:::::::::::::::::::::::::::::::::

        #region Error Mail Box

        public bool NotificationsEnabled
        {
            get
            {
                return _notificationsEnabled;
            }
            set
            {
                _notificationsEnabled = value;
            }
        }

        public XmlDocument CurrentErrorMailBox
        {
            get
            {
                XmlDocument mailBox = new XmlDocument();

                OpenErrorMailBox(mailBox);

                return mailBox;
            }
        }

        private void OpenErrorMailBox(XmlDocument inMailBox)
        {
            if (!Directory.Exists(_archiveMailBoxFolder))
            {
                Directory.CreateDirectory(_archiveMailBoxFolder);
            }

            if (File.Exists(_errorMailBoxPath))
            {
                XmlTextReader reader = new XmlTextReader(_errorMailBoxPath);
                reader.Read();
                inMailBox.Load(reader);

                reader.Close();
            }
            else
            {
                InitErrorMailBox(inMailBox);
            }
        }

        private void CloseErrorMailBox(XmlDocument inMailBox)
        {
            inMailBox = null;

            GC.Collect();

            inMailBox = new XmlDocument();
        }

        private bool ErrorMailBoxIsOpen()
        {    
            return _errorMailBox.DocumentElement != null;
        }

        private void InitErrorMailBox(XmlDocument inErrorMailBox)
        {
            //::::::::::::::::::::::::::::::

            // Create the XML Declaration, and append it to XML document
            XMLDocManager.CreateXmlDeclaration(inErrorMailBox, @"1.0", null, null);

            //::::::::::::::::::::::::::::::

            // Create the root element
             XMLDocManager.CreateRoot(inErrorMailBox, @"ErrorMailBox");

            //::::::::::::::::::::::::::::::

             AddEntries(inErrorMailBox);

            //_errorMailBoxEntries.Add(XMLDocManager.AddNode(inErrorMailBox, root, @"ServerStartListeningFailed"));
        }

        //.................................

        public void ArchiveErrorMailBox(XmlDocument inErrorMailBox)
        {
            OpenErrorMailBox(inErrorMailBox);

            //::::::::::::::::::::::::::::::::

            XmlNodeList entries = XMLDocManager.GetNodeList(inErrorMailBox, @"Entries/Entry");

            //::::::::::::::::::::::::::::::::::::::

            foreach (XmlNode entry in entries)
            {
                CopyErrorMailBoxEntry(inErrorMailBox, (LogErrorLevel)Convert.ToInt32(entry.Attributes[@"id"].Value));

                //....................

                XmlNodeList errorList = XMLDocManager.GetNodeList(entry, @"Error");

                foreach (XmlNode error in errorList)
                {
                    entry.RemoveChild(error);
                }

                XMLDocManager.SetAttribute(entry, @"locked", @"false");
            }

            try
            {
                SaveErrorEmailBox(inErrorMailBox, _errorMailBoxPath);
            }
            catch(Exception e)
            {
                LogWarning(string.Format(@"ArchiveErrorMailBox: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
            }

            //::::::::::::::::::::::::::::::::

            CloseErrorMailBox(inErrorMailBox);
        }

        //.................................

        public void ArchiveErrorMailBoxEntry(XmlDocument inErrorMailBox, LogErrorLevel inErrorLevel)
        {
            OpenErrorMailBox(inErrorMailBox);

            //::::::::::::::::::::::::::::::::

            CopyErrorMailBoxEntry(inErrorMailBox, inErrorLevel);

            //::::::::::::::::::::::::::::::::::::::

            XmlNode entry = GetEntry(inErrorLevel);
            XmlNodeList errorList = XMLDocManager.GetNodeList(entry, @"Error");

            foreach (XmlNode error in errorList)
            {
                entry.RemoveChild(error);
                XMLDocManager.SetAttribute(entry, @"locked", @"false");
            }

            try
            {
                SaveErrorEmailBox(inErrorMailBox, _errorMailBoxPath);
            }
            catch (Exception e)
            {
                LogWarning(string.Format(@"ArchiveErrorMailBoxEntry: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
            }

            //::::::::::::::::::::::::::::::::

            CloseErrorMailBox(inErrorMailBox);
        }

        //.................................

        public void CopyErrorMailBoxEntry(XmlDocument inErrorMailBox, LogErrorLevel inErrorLevel)
        {
            //::::::::::::::::::::::::::::::

            XmlDocument errorMailBoxEntryArchive = new XmlDocument();

            // Create the XML Declaration, and append it to XML document
            XMLDocManager.CreateXmlDeclaration(errorMailBoxEntryArchive, @"1.0", null, null);

            //::::::::::::::::::::::::::::::

            string entryName = LogErrorLevelToName(inErrorLevel);
            string timeStr = DateTime.Now.ToString("yyyyMMddHHmmss");

            // Create the root element
            XMLDocManager.CreateRoot(errorMailBoxEntryArchive, string.Format(@"{0}{1}", timeStr, entryName));

            //::::::::::::::::::::::::::::::

            XmlNode archiveEntry = AddEntry(errorMailBoxEntryArchive, errorMailBoxEntryArchive.DocumentElement, entryName, inErrorLevel);
            
            //..............................

            XmlNode entry = GetEntry(inErrorLevel); 
            XmlNodeList errorList = XMLDocManager.GetNodeList(entry, @"Error");

            foreach(XmlNode error in errorList)
            {
                ArchiveMailBoxError(errorMailBoxEntryArchive, archiveEntry,
                                    error.Attributes[@"id"].Value,
                                    error.Attributes[@"file"].Value, 
                                    error.Attributes[@"time"].Value,
                                    error.Attributes[@"name"].Value, 
                                    error.Attributes[@"detail"].Value, 
                                    error.Attributes[@"sent"].Value);
            }

            //::::::::::::::::::::::::::::::::::::::

            try
            {
                SaveErrorEmailBox(errorMailBoxEntryArchive, string.Format(@"{0}{1}.xml", _archiveMailBoxFolder, errorMailBoxEntryArchive.DocumentElement.Name));

                LogWarning(string.Format(@"An error mailbox entry has been archived: {0}", errorMailBoxEntryArchive.DocumentElement.Name));
            }
            catch (Exception e)
            {
                LogWarning(string.Format(@"CopyErrorMailBoxEntry: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
            }
        }

        //.................................        

        private void SaveErrorEmailBox(XmlDocument inErrorMailBox, string inPath)
        {
            inErrorMailBox.Save(inPath);
        }

        //.................................

        private void AddEntries(XmlDocument inErrorMailBox)
        {
            XmlNode entries = XMLDocManager.AddNode(inErrorMailBox, inErrorMailBox.DocumentElement, @"Entries");

            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.AppFailedToStart), LogErrorLevel.AppFailedToStart);
            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.SiteIDMapFailed), LogErrorLevel.SiteIDMapFailed);
            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.InvalidSiteID), LogErrorLevel.InvalidSiteID);
            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.InternalXMLError), LogErrorLevel.InternalXMLError);
            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.CreateZIPFailed), LogErrorLevel.CreateZIPFailed);
            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.UploadFailed), LogErrorLevel.UploadFailed);
            AddEntry(inErrorMailBox, entries, LogErrorLevelToName(LogErrorLevel.ServerStartListeningFailed), LogErrorLevel.ServerStartListeningFailed);

            try
            {
                SaveErrorEmailBox(inErrorMailBox, _errorMailBoxPath);
            }
            catch (Exception e)
            {
                LogWarning(string.Format(@"AddEntries: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
            }
        }

        //.................................

        private XmlNode AddEntry(XmlDocument inErrorMailBox, XmlNode inEntries, string inEntryName, LogErrorLevel inErrorLevel)
        {
            XmlNode entry = XMLDocManager.AddNode(inErrorMailBox, inEntries, @"Entry");

            XMLDocManager.AddAttribute(inErrorMailBox, entry, @"name", inEntryName);
            XMLDocManager.AddAttribute(inErrorMailBox, entry, @"id", Convert.ToString((int)inErrorLevel));
            XMLDocManager.AddAttribute(inErrorMailBox, entry, @"locked", @"false");

            return entry;
        }

        //.................................

        private XmlNode GetEntry(LogErrorLevel inErrorLevel)
        {
            XmlNodeList entries = XMLDocManager.GetNodeList(_errorMailBox, @"Entries/Entry");

            foreach (XmlNode entry in entries)
            {
                if (Convert.ToInt32(entry.Attributes[@"id"].Value) == (int)inErrorLevel)
                {
                    return entry;
                }
            }

            return null;
        }

        //.................................

        private void LockEntry(XmlNode inEntry)
        {
            try
            {
                inEntry.Attributes[@"locked"].Value = @"true";

                try
                {
                    SaveErrorEmailBox(_errorMailBox, _errorMailBoxPath);
                }
                catch (Exception e)
                {
                    LogWarning(string.Format(@"LockEntry: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
                }
            }
            catch(Exception)
            {
            }
        }
  
        //.................................

        private void UnlockEntry(XmlNode inEntry)
        {
            try
            {
                inEntry.Attributes[@"locked"].Value = @"false";

                try
                {
                    SaveErrorEmailBox(_errorMailBox, _errorMailBoxPath);
                }
                catch (Exception e)
                {
                    LogWarning(string.Format(@"UnlockEntry: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
                }             
            }
            catch (Exception)
            {
            }
        }

        //.................................

        private void AddMailBoxEntryError(LogErrorLevel inErrorLevel, LogErrorLevelFile inErrorLevelFile, string inErrorStr, string inErrorStrDetail)
        {
            OpenErrorMailBox(_errorMailBox);

            //::::::::::::::::::::::::::::::::

            try
            {
                XmlNode entry = GetEntry(inErrorLevel);

                bool sent;
                if (!MailBoxEntryLocked(entry))
                {
                    sent = true;

                    XmlNode error = AddMailBoxError(entry, inErrorLevelFile, inErrorStr, inErrorStrDetail, sent);
                    SendErrorEmail(error);

                    LockEntry(entry);
                }
                else
                {
                    sent = false;
                    AddMailBoxError(entry, inErrorLevelFile, inErrorStr, inErrorStrDetail, sent);
                }

                try
                {
                    SaveErrorEmailBox(_errorMailBox, _errorMailBoxPath);
                }
                catch (Exception e)
                {
                    LogWarning(string.Format(@"AddMailBoxEntryError: Summa failed to saved error mailbox   Message: {0}   StackTrace: {1}", e.Message, e.StackTrace));
                }             
            }
            catch(Exception e)
            {
                LogWarning(e.Message);
            }

            //::::::::::::::::::::::::::::::::

            CloseErrorMailBox(_errorMailBox);
        }

        //.................................

        private bool MailBoxEntryLocked(XmlNode inEntry)
        {
            return inEntry.Attributes[@"locked"].Value == @"true" ? true : false;
        }

        //.................................

        private XmlNode AddMailBoxError(XmlNode inEntry, LogErrorLevelFile inErrorLevelFile, string inErrorStr, string inErrorStrDetail, bool inSent)
        {
            string timeStr = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss");

            XmlNode error = XMLDocManager.AddNode(_errorMailBox, inEntry, @"Error");
            XMLDocManager.AddAttribute(_errorMailBox, error, @"id", Convert.ToString((int)inErrorLevelFile));
            XMLDocManager.AddAttribute(_errorMailBox, error, @"file", GetLogErrorLevelFileStr(inErrorLevelFile));
            XMLDocManager.AddAttribute(_errorMailBox, error, @"time", timeStr);
            XMLDocManager.AddAttribute(_errorMailBox, error, @"sent", inSent ? @"true" : @"false");
            XMLDocManager.AddAttribute(_errorMailBox, error, @"name", inErrorStr);
            XMLDocManager.AddAttribute(_errorMailBox, error, @"detail", inErrorStrDetail);

            //:::::::::::::::::::::::::::::::

            if (onErrorFunc != null)
            {
                LogErrorLevel errorLevel = (LogErrorLevel)Convert.ToInt32(inEntry.Attributes[@"id"].Value);
                onErrorFunc(errorLevel, timeStr, inErrorLevelFile, inErrorStr, inErrorStrDetail, inSent);
            }

            //:::::::::::::::::::::::::::::::

            return error;
        }

        //.................................

        private XmlNode ArchiveMailBoxError(XmlDocument inErrorMailArchive, XmlNode inEntry, string inId, string inFile, string inTimeStr, string inErrorStr, string inErrorStrDetail, string inSent)
        {
            string timeStr = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss");

            XmlNode error = XMLDocManager.AddNode(inErrorMailArchive, inEntry, @"Error");
            XMLDocManager.AddAttribute(inErrorMailArchive, error, @"id", inId);
            XMLDocManager.AddAttribute(inErrorMailArchive, error, @"file", inFile);
            XMLDocManager.AddAttribute(inErrorMailArchive, error, @"time", inTimeStr);
            XMLDocManager.AddAttribute(inErrorMailArchive, error, @"sent", inSent);
            XMLDocManager.AddAttribute(inErrorMailArchive, error, @"name", inErrorStr);
            XMLDocManager.AddAttribute(inErrorMailArchive, error, @"detail", inErrorStrDetail);

            //:::::::::::::::::::::::::::::::

            return error;
        }

        //.................................

        private void SendErrorEmail(XmlNode inError)
        {
            if(_notificationsEnabled)
            {
                StringBuilder line = new StringBuilder();
                line.AppendFormat(@"{0} | ", inError.Attributes[@"time"].Value);
                line.AppendFormat(@"{0} | ", inError.ParentNode.Attributes[@"name"].Value);
                if (inError.Attributes[@"file"].Value != @"")
                {
                    line.AppendFormat(@"{0} | ", inError.Attributes[@"file"].Value);
                }
                line.AppendFormat(@"{0} | ", inError.Attributes[@"name"].Value);
                line.AppendFormat(@"{0} | ", inError.Attributes[@"detail"].Value);

                SendErrorNoticeEMail(line.ToString());
            }
        }

        public void SendErrorNoticeEMail(string inTxt)
        {
            try
            {
                getLoggerEmail().Log(LogLevel.Error, inTxt);

                LogWarning(@"Summa has failed. An email has been sent to the Menumate Ltd's support team");
            }
            catch (Exception exc)
            {
                LogWarning(exc.Message);
            }
        }


        #endregion

        //:::::::::::::::::::::::::::::::::

        public void createLoggers() 
        {
            _loggers.Add(LogManager.GetLogger(@"ScreenLog"));
            _loggers.Add(LogManager.GetLogger(@"FileLog"));

            //_loggers.Add(LogManager.GetLogger(@"ScreenError"));
            //_loggers.Add(LogManager.GetLogger(@"FileError"));

            _loggers.Add(_createEMailLogger());
        }

        //:::::::::::::::::::::::::::::::::

        public void _createLoggers()
        {
            _loggers.Add(LogManager.GetLogger(@"ScreenLog"));
            _loggers.Add(LogManager.GetLogger(@"FileLog"));

            //_loggers.Add(LogManager.GetLogger(@"ScreenError"));
            //_loggers.Add(LogManager.GetLogger(@"FileError"));
        }

        //:::::::::::::::::::::::::::::::::

        private void disposeLogger(Logger inLogger)
        {
            try
            {
                inLogger.Factory.Dispose();
            }
            catch (Exception)
            {
            }
        }

        public void disposeLoggers()
        {
            while (_loggers.Count > 0)
            {
                disposeLogger(_loggers[0]);
                _loggers.Remove(_loggers[0]);
            }
        }

        //:::::::::::::::::::::::::::::::::

        private Logger _createEMailLogger()
        {
            _setMailLoggerSubject(@"Summa failed in location: ");
            return LogManager.GetLogger(@"eMail");
        }

        //:::::::::::::::::::::::::::::::::

        private void _setMailLoggerSubject(string inSubject)
        {
            MailTarget target = new MailTarget();

            target.Name = @"errorGmail";
            target.SmtpServer = @"smtp.gmail.com";
            target.SmtpPort = 587;
            target.SmtpAuthentication = NLog.Targets.SmtpAuthenticationMode.Basic;
            target.EnableSsl = true;

            target.SmtpUserName = @"noreply@menumate.com";
            target.SmtpPassword = @"1202matemenu";
            //target.SmtpUserName = @"support@menumate.com";
            //target.SmtpPassword = @"matemenu1202";

            target.From = @"noreply@menumate.com";
//            target.To = @"jorgel@menumate.com";
//            target.From = @"support@menumate.com";
            target.To = @"support@menumate.com";

            //target.Subject = string.Format(@"{0}  Version 5.3.0.12", inSubject);
            target.Subject = string.Format(@"{0}  Version __MM_VERSION__", inSubject);
            target.Body = @"${message}";
            //target.Body = @"${date}: ${message}";

            LoggingConfiguration config = LogManager.Configuration;

            //......................................

            try
            {
                if (config.FindTargetByName(target.Name) != null)
                {
                    config.RemoveTarget(target.Name);
                    config = config.Reload();
                }

                //config.RemoveTarget(target.Name);
            }
            catch(Exception)
            {
            }

            //......................................

            config.AddTarget("errorGmail", target);
            config.LoggingRules.Add(new NLog.Config.LoggingRule("eMail", LogLevel.Error, target));

            //LogManager.Configuration.AddTarget("errorGmail", target);
            //LogManager.Configuration.LoggingRules.Add(new NLog.Config.LoggingRule("eMail", LogLevel.Error, target));

            LogManager.Configuration = config;
        }

        //:::::::::::::::::::::::::::::::::

        public void setMailLoggerSubject(string inSubject)
        {
            disposeLoggers();
            _createLoggers();

            try
            {
                _setMailLoggerSubject(inSubject);
                _loggers.Add(LogManager.GetLogger(@"eMail"));
            }
            catch(Exception)
            {

                //_logger.LogException(LogLevel.Error, @"Error setting email target: ", exc);
            }

        }

        //:::::::::::::::::::::::::::::::::

        /*
                static public void createEmailTarget
                {
                    MailTarget target = new MailTarget(); 
 
                    target.SmtpServer = "192.168.0.15"; 
                    target.From = "jaak@jkowalski.net"; 
                    target.To = "jaak@jkowalski.net"; 
                    target.Subject = "sample subject"; 
 
                    NLog.Config.SimpleConfigurator.ConfigureForTargetLogging(target, LogLevel.Debug); 
                }
        */

        //:::::::::::::::::::::::::::::::::

        private Logger getLoggerScr()
        {
            return _loggers[(int)LoggerType.loggerScreen];
        }

        //.................................

        private Logger getLoggerFile()
        {
            return _loggers[(int)LoggerType.loggerFile];
        }

        //.................................

        private Logger getLoggerScrErr()
        {
            return _loggers[(int)LoggerType.loggerScreenErr];
        }

        //.................................

        private Logger getLoggerFileErr()
        {
            return _loggers[(int)LoggerType.loggerFileErr];
        }

        //.................................

        private Logger getLoggerEmail()
        {
            return _loggers[(int)LoggerType.loggerEmail];
        }

        //::::::::::::::::::::::::::::::::::

        public void ResetErrorMailBox()
        {
            ArchiveErrorMailBox(_errorMailBox);
         }

        //.................................

        public void ResetErrorMailBoxEntry(FileLogger.LogErrorLevel inErrorLevel)
        {
            ArchiveErrorMailBoxEntry(_errorMailBox, inErrorLevel);
        }

        //.................................
 
        public void Log(string inTxt)
        {
            try
            {
                getLoggerScr().Log(LogLevel.Info, inTxt);
                getLoggerFile().Log(LogLevel.Info, inTxt);
            }
            catch(Exception)
            {

            }
        }

        //.................................

        public void LogWarning(string inTxt)
        {
            try
            {
                Log(string.Format("Warning: {0}", inTxt));

                //string str = string.Format("Warning: {0}", inTxt);

                //getLoggerScr().Log(LogLevel.Info, str);
                //getLoggerFile().Log(LogLevel.Info, str);
            }
            catch (Exception)
            {

            }
        }

        //.................................

        public void LogError(LogErrorLevel inErrorLevel, LogErrorLevelFile inErrorLevelFile, string inErrorStr, string inErrorStrDetail)
        {
            try
            {
                string str = GetLogErrorStr(inErrorLevel, inErrorLevelFile, inErrorStr);

                //getLoggerScrErr().Log(LogLevel.Error, str);
                //getLoggerFileErr().Log(LogLevel.Error, str);

                AddMailBoxEntryError(inErrorLevel, inErrorLevelFile, inErrorStr, inErrorStrDetail);
            }
            catch (Exception)
            {

            }
        }

        //.................................

        public void LogException(FileLogger.LogErrorLevel inErrorLevel, FileLogger.LogErrorLevelFile inErrorLevelFile,
                                 string inMessage, Exception inException)
        {
            try
            {
                string str = GetLogErrorStr(inErrorLevel, inErrorLevelFile, inMessage);

                //getLoggerScrErr().Log(LogLevel.Error, str);
                //getLoggerFileErr().LogException(LogLevel.Error, str, inException);

                string detail = @"";
                try
                {
                    detail = string.Format(@"{0}: {1}", inException.Message, inException.StackTrace);

                    //detail = inException.InnerException.Message;
                }
                catch(Exception)
                {
                }

                AddMailBoxEntryError(inErrorLevel, inErrorLevelFile, inMessage, detail);
            }
            catch (Exception)
            {

            }
        }

        //:::::::::::::::::::::::::::::::::

        private string GetLogErrorStr(LogErrorLevel inErrorLevel, LogErrorLevelFile inErrorLevelFile, string inErrorStr)
        {
            return string.Format("Error: {0} : {1} : {2}", GetLogErrorLevelStr(inErrorLevel), GetLogErrorLevelFileStr(inErrorLevelFile), inErrorStr);
        }

        //:::::::::::::::::::::::::::::::::

        private string LogErrorLevelToName(LogErrorLevel inErrorLevel)
        {
            switch (inErrorLevel)
            {
                case LogErrorLevel.None: return @"";
                case LogErrorLevel.AppFailedToStart: return @"SummaStartup";
                case LogErrorLevel.SiteIDMapFailed: return @"SiteIDMapping";
                case LogErrorLevel.InvalidSiteID: return @"SiteIDChecking";
                case LogErrorLevel.InternalXMLError: return @"MMXmlFilesProcessing";
                case LogErrorLevel.UploadFailed: return @"UploadFiles";
                case LogErrorLevel.CreateZIPFailed: return @"CreateZIPFiles";
                case LogErrorLevel.ServerStartListeningFailed: return @"SummaStartListeningIPPort";
                default: return @"";
            }
        }

        //:::::::::::::::::::::::::::::::::

        public string GetLogErrorLevelStr(LogErrorLevel inErrorLevel)
        {
            switch(inErrorLevel)
            {
                case LogErrorLevel.None:             return @"";
                case LogErrorLevel.AppFailedToStart: return @"Summa startup";
                case LogErrorLevel.SiteIDMapFailed:  return @"Site ID mapping";
                case LogErrorLevel.InvalidSiteID:    return @"Site ID checking";
                case LogErrorLevel.InternalXMLError: return @"Menumate XML files processing";
                case LogErrorLevel.UploadFailed:     return @"Upload files";
                case LogErrorLevel.CreateZIPFailed:  return @"Creating ZIP files";
                case LogErrorLevel.ServerStartListeningFailed: return @"Summa start listening IP port";
                default: return @"";
            }
        }

        //:::::::::::::::::::::::::::::::::

        public string GetLogErrorLevelFileStr(LogErrorLevelFile inErrorLevelFile)
        {
            switch (inErrorLevelFile)
            {
                case LogErrorLevelFile.None:                    return @"";
                case LogErrorLevelFile.ConfigFile:              return @"Configuration file";
                case LogErrorLevelFile.TransactionXMLFile:      return @"Transaction XML file";
                case LogErrorLevelFile.UpdateMemberXMLFile:     return @"UpdateMember XML file";
                case LogErrorLevelFile.ListCategoriesXMLFile:   return @"ListCategories XML file";
                case LogErrorLevelFile.ListProductTypesXMLFile: return @"ListProductTypes XML file";
                case LogErrorLevelFile.ListPaymentTypesXMLFile: return @"ListPaymentTypes XML file";
                case LogErrorLevelFile.ListGroupsXMLFile:       return @"ListGroups XML file";
                case LogErrorLevelFile.ListStaffXMLFile:        return @"ListStaff XML file";
                case LogErrorLevelFile.ListDiscountsXMLFile:    return @"ListDiscounts XML file";
                case LogErrorLevelFile.ListCalculatedXMLFile:   return @"ListCalculated XML file";
                case LogErrorLevelFile.TotalsDiscountsXMLFile:  return @"TotalDiscounts XML file";
                case LogErrorLevelFile.ListMemberGroupsXMLFile: return @"ListMemberGroups XML file";
                case LogErrorLevelFile.ListRosterXMLFile:       return @"ListRoster XML file";
                case LogErrorLevelFile.RequestXMLFile:          return @"Request XML file";
                case LogErrorLevelFile.UnknownXMLFile:          return @"Unknown XML file";
                default: return @"";
            }
        }

        //:::::::::::::::::::::::::::::::::

        public void setOnErrorFunc(NotifLogErrorFuncDelegate inOnErrorFunc)
        {
            onErrorFunc = inOnErrorFunc;
        }
    }
}
