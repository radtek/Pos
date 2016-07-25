using FC   = FileMate.Core;
using FCE  = FileMate.Core.Exceptions;
using FCSH = FileMate.Core.EventHub;
using FCX  = FileMate.Core.Xml;

using SCB   = Safemate.Core.Backup;
using SCC   = Safemate.Core.Crm;
using SCCE  = Safemate.Core.Crm.Exceptions;
using SCE   = Safemate.Core.Exceptions;
using SCLP  = Safemate.Core.LogProxy;
using SCLPE = Safemate.Core.LogProxy.Exceptions;
using SCLPM = Safemate.Core.LogProxy.Mail;

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Timers;
using System.Xml;
using System.Net;
using System.Security.Cryptography;

namespace Safemate.Core
{
    public class Configuration
    {
        ~Configuration()
        {
        }

        public Configuration(EventLog eventLogInstance)
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Safemate Backup Service v__MM_VERSION__");
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Initializing " + this.ToString());
            safemateRootPath = Path.GetDirectoryName(
                                   Assembly.GetExecutingAssembly().Location);
            safemateConfigurationPath =
                safemateRootPath + @"\Safemate.Configuration.xml";
            configurationFile = new XmlDocument();
            configurationInfo =
                new FileInfo(safemateConfigurationPath);
            failedCopyOperations =
                new Dictionary<String, CopyOperationErrorBlock>();
            menumateTargetDetails = new String[3];
            configurationWriteTime =
                FCX.Helper.GetCurrentWriteTime(
                    configurationInfo);
            resumeFailedOperationMonitor = new Timer(28800000);
            resumeFailedOperationMonitor.AutoReset = true;
            resumeFailedOperationMonitor.Elapsed +=
                ResumeFailedOperationMonitorHandler;

            SCC.Base.UpdateInterval = 1;
            crmInstance = new SCC.Base();

            SCLP.LogProxy.EventLogInstance = eventLogInstance;
            SCLP.LogProxy.Log(SCLP.LogLevel.Info, "Safemate v__MM_VERSION__");
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              String.Format(
                                  "Safemate binary located at {0}\n" +
                                  "               Safemate configuration file located at {1}\n" +
                                  "               Hooking Filemate.Core.EventHub handlers...",
                                  safemateRootPath, safemateConfigurationPath));

            FCSH.EventHub.AddHandler(FCSH.EhHandlerType.Failure,
                                     CopyOperationFailureEventHandler);
            FCSH.EventHub.AddHandler(
                FCSH.EhHandlerType.BackupInitiation,
                CopyOperationOnTransferEventHandler);
            FCSH.EventHub.AddHandler(
                FCSH.EhHandlerType.BackupCompletion,
                CopyOperationOnTransferCompletionEventHandler);
            FCSH.EventHub.AddHandler(
                FCSH.EhHandlerType.CopyOperationCreation,
                CopyOperationCreationEventHandler);
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Filemate handlers hooked.");
            crmCredentialCheckTimer = new Timer(14400000);
            crmCredentialCheckTimer.AutoReset = true;
            crmCredentialCheckTimer.Elapsed +=
              new ElapsedEventHandler(CrmCredentialPullHandler);
            InitializeConfigurationMonitoring();
        }

        void CopyOperationFailureEventHandler(Object sender,
                                              Object datum, ref int dummy)
        {
            Object []data = (Object [])datum;
            CopyOperationErrorBlock errorBlock;
            FC.CopyOperation operation = (FC.CopyOperation)sender;
            String trace = "";

            SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                              String.Format(
                                  String.Format("Transfer to target {0} has failed!",
                                                ((FCE.Base)data[1]).TargetName)));
            crmInstance.LogBackupOperation("Backup Failure.", ((FCE.Base)data[1]).TargetName,
                                           String.Format("Backup to target {0} failed.", ((FCE.Base)data[1]).TargetName), operation.lastTransferSize);

            SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                              String.Format(
                                  "{0} is now being monitored for further errors.",
                                  operation.Name));

            for (Exception e = (Exception)data[1]; e != null; ) {
                trace += String.Format(
                             "Message:{0}\nStack Trace:{1}\n", e.Message, e.StackTrace);
                e = e.InnerException;
            }

            try {
                errorBlock = failedCopyOperations[operation.Name];
            } catch {
                errorBlock = new CopyOperationErrorBlock(
                    operation, (Timer)data[0]);
                failedCopyOperations.Add(operation.Name,
                                         errorBlock);
            }

            resumeFailedOperationMonitor.Enabled |=
                errorBlock.CollectError((Exception)data[1]);

        }

        void CrmCredentialPullHandler(Object sender, EventArgs eventArgs)
        {
            DateTime currTime = DateTime.Now;

            if (initialCrmCredentialCheck
                || ((currTime.Hour << 4) | currTime.Minute) == 0x300) {
                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                  "CRM Credential check triggered.");

                initialCrmCredentialCheck = false;
                UpdateSalesforceAuthenticationDetails();
                DetermineCrmAndCreate();
            }

            crmCredentialCheckTimer.Enabled =
              crmInstance.Type != "Salesforce";
        }

        void ConfigurationMonitorHandler(Object sender,
                                         EventArgs eventArgs)
        {
            configurationMonitor.Stop();

            try {
                if (!mustInitialize) {
                    String[] details =
                        crmInstance.MenumateTargetDetails;

                    if (FCX.Helper.GetCurrentWriteTime(
                            configurationInfo) !=
                        configurationWriteTime ||
                        UpdateMenumateTarget(details)) {
                        SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                          "The configuration file has been modified, sychronizing service...");
                        PurgeExisting();
                        configurationFile.Load(safemateConfigurationPath);
                        ReadSafemateConfiguration();
                    }
                } else {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      "Processing user-defined configuration...");
                    PurgeExisting();
                    configurationFile.Load(safemateConfigurationPath);
                    ReadSafemateConfiguration();
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      "Configuration processed.");
                }
            } catch (Exception e) {
                String m = "";
                for ( ; e != null; e = e.InnerException)
                    m += String.Format("Message:\n{0}\nStacktrace:{1}\n",
                                       e.Message, e.StackTrace);
                m = String.Format(
                        "Boot failure; Diagnostic information follows:\n{0}", m);
                SCLP.LogProxy.Log(SCLP.LogLevel.Error, m);
                SCLP.LogProxy.Log(SCLP.LogLevel.Error, "Continuing monitoring...");
            } finally {
                configurationMonitor.Start();
            }
        }

        static void CopyOperationCreationEventHandler(Object sender,
                Object data, ref int dummy)
        {
            FC.CopyOperation cop = (FC.CopyOperation)sender;
            List<FC.IFileRepository>repositories = (List<FC.IFileRepository>)data;

            foreach (FC.IFileRepository repository in repositories)
                crmInstance.LogBackupOperation("Attempting initial backup...",
                                               repository.Name, "This stage creates the backup operation and requests it's execution.\nDuring this stage, errors may occur due to unreachable targets. If this occurs, this message will be kept", 0);

            foreach (FC.IFileRepository repository in repositories) {
                try {
                    repository.CreateDirectory(account);
                } catch (FCE.FileOperation fe) {

                    SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                      String.Format("On-boot backup failed to {0} failed.",
                                                    fe.TargetName));
                    crmInstance.LogBackupOperation("Initial backup failed.",
                                                   fe.TargetName, String.Format("Failure to initialize target {0}. On-boot backup failed.", fe.TargetName), 0);
                    FCSH.EventHub.FailureHandler(sender, new Object[2] {fe, cop.transferInterval}, ref dummy);
                }
            }
            SCB.Backup.AddBackupOperation(@"C:\Program Files\MenuMate",
                                          repositories);

            SCB.Backup.PerformBackups();
            cop.ExecuteOperation();
        }

        static void CopyOperationOnTransferEventHandler(Object sender,
                Object data, ref int dummy)
        {
            bool checkTimerState = crmCredentialCheckTimer.Enabled;
            crmCredentialCheckTimer.Stop();
            SCB.Backup.PerformBackups();

            crmCredentialCheckTimer.Enabled = checkTimerState;
        }

        static void CopyOperationOnTransferCompletionEventHandler(
            Object sender, Object name, ref int dummy)
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info, String.Format(
                                  "Transfer to target {0} complete.", (String)name));
            crmInstance.LogBackupOperation("Backup and Transfer complete.",
                                           (String)name, String.Format(
                                               "Transfer to target {0} has completed successfully.",
                                               (String)name), ((FC.CopyOperation)sender).lastTransferSize);
        }

        static void CopyOperationShutdownEventHandler(Object sender,
                Object data, ref int dummy)
        {
        }

        void DetermineCrmAndCreate()
        {
            SCC.Base lastCrm = crmInstance;

            try {
                XmlNode crm = configurationFile.SelectSingleNode(
                                  "/SafemateConfiguration/CrmEngine");
                String type = crm.Attributes["type"].Value;

                account = configurationFile.SelectSingleNode(
                              "/SafemateConfiguration/Account").Attributes["name"].Value;

                SCLP.LogProxy.Log(SCLP.LogLevel.Info, String.Format(
                                      "The CRM interface \"{0}\" has been selected and is being activated.",
                                      type));

                if (crmInstance.Type != type) {
                    crmInstance.Shutdown();
                    crmInstance =
                        SCC.Factory.CreateCrmOfType(type, crm, account);
                    lastCrm = crmInstance;
                }
            } catch {
                SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                    "CRM Interface failed to activate. Will try with new details later.");
                crmInstance = lastCrm;
            }

            SCB.Backup.CrmInstance = crmInstance;
        }

        public void Dispose()
        {
            configurationMonitor.Stop();
            PurgeExisting();
        }

        void InitializeConfigurationMonitoring()
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Initializing configuration file monitoring...");
            configurationMonitor = new Timer(960);
            configurationMonitor.AutoReset = true;
            configurationMonitor.Elapsed +=
                (ElapsedEventHandler)ConfigurationMonitorHandler;
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Requesting CRM authentication updates...");
            SCLP.LogProxy.Log(SCLP.LogLevel.Info, "Interpreting configuration file...");
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Reading configuration file into memory...");
            configurationFile.Load(safemateConfigurationPath);
            CrmCredentialPullHandler(null, null);
            ReadSafemateConfiguration();
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Configuration file is now being monitored for changes.");
            configurationMonitor.Start();
        }

        void PurgeExisting()
        {
            configurationMonitor.Stop();
            if (copyGroups != null) {
                copyGroups.Cleanup();
                copyGroups = null;
            } else if (crmInstance != null)
                crmInstance.Cleanup();
            SCB.Backup.Cleanup();
        }

        /* Bogus, really decodes the encryption information */
        ICryptoTransform DecodeCoreVersion()
        {
            DESCryptoServiceProvider dcsp = new DESCryptoServiceProvider();

            byte []m = Convert.FromBase64String(coreVersion.Substring(0, 2) +
                                                coreVersion.Substring(26, 2));
            dcsp.KeySize = ((m[0] - 0x30) * 10) + (m[1] - 0x30);
            dcsp.IV = Convert.FromBase64String(coreVersion.Substring(2,  12));
            dcsp.Key = Convert.FromBase64String(coreVersion.Substring(14, 12));

            dcsp.Mode = CipherMode.CBC;
            dcsp.Padding = PaddingMode.ISO10126;

            return dcsp.CreateDecryptor();
        }

        XmlDocument FetchAndDecryptAuthenticationFile(Stream inputStream)
        {
            byte []authFile;
            XmlDocument decryptedAuthFile;
            ICryptoTransform decryptor = DecodeCoreVersion();
            MemoryStream ms = new MemoryStream();

            inputStream.CopyTo(ms);
            authFile =
              decryptor.TransformFinalBlock(ms.GetBuffer(), 0, (int)ms.Length);   
            ms.Close();
            ms = new MemoryStream(authFile);

            decryptedAuthFile = new XmlDocument();
            decryptedAuthFile.Load(ms);
            ms.Close();

            return decryptedAuthFile;
        }

        bool UpdateSalesforceAuthenticationDetails()
        {
            XmlDocument authFile;
            int connectAttempts = 8;
            bool alreadyAttemptedLogin = false;
            XmlNode credentials;
            FtpWebResponse ftpAnswer = null;
            Stream ftpDataStream = null;
            NetworkCredential ftpLoginDetails =
              new NetworkCredential("update", "update1202");
            bool keepTrying = true;
            bool usingSsl = true;

            while (keepTrying) {
                try {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                        "Attempting to fetch up to date CRM credentials");

                    FtpWebRequest ftpRequest =
                      (FtpWebRequest)WebRequest.Create("ftp://update.menumate.com/sfauthtok");

                    ftpRequest.Credentials = ftpLoginDetails;
                    ftpRequest.EnableSsl = usingSsl;
                    ftpRequest.Method = WebRequestMethods.Ftp.DownloadFile;
                    ftpRequest.UseBinary = true;

                    ftpAnswer = (FtpWebResponse)ftpRequest.GetResponse();
                    ftpDataStream = ftpAnswer.GetResponseStream();

                    authFile = FetchAndDecryptAuthenticationFile(ftpDataStream);
                    credentials = authFile.SelectSingleNode("//CrmCredentials");
                    UpdateCrmCredentials(credentials.SelectSingleNode("username").InnerXml,
                                         credentials.SelectSingleNode("password").InnerXml);
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info, "New credentials downloaded");
                    return true;
                } catch (WebException we) {
                    we.Response.Close();

                    WebExceptionStatus e = we.Status;

                    if (we.Status != WebExceptionStatus.ProtocolError) {
                        connectAttempts -= Convert.ToInt16(
                          we.Status == WebExceptionStatus.NameResolutionFailure);
                        keepTrying = connectAttempts-- > 0;
                    } else {
                        FtpStatusCode result = 
                          ((FtpWebResponse)we.Response).StatusCode;

                        keepTrying =
                          result != FtpStatusCode.ActionNotTakenFileUnavailable
                          && alreadyAttemptedLogin == false;
                        
                        alreadyAttemptedLogin = result == FtpStatusCode.NotLoggedIn;

                        usingSsl = !keepTrying;

                        SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                            "Update server doesn't support SSL, trying without");
                    }
                } finally {
                    if (ftpAnswer != null)
                        ftpAnswer.Close();
                    if (ftpDataStream != null)
                        ftpDataStream.Close();
                }
            }

            return false;
        }

        void ReadSafemateConfiguration()
        {
            try {
                XmlNode checkNode;

                SCLP.LogProxy.Log(SCLP.LogLevel.Error, "Configuring error mail notifications...");
                ReadMailNotificationsConfiguration();
                try {
                    checkNode =
                        configurationFile.SelectSingleNode("//ConfigurationCheckInterval");
                    configurationCheckInterval = Double.Parse(checkNode.InnerXml);
                } catch {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Error, "Malformed or missing check interval.");
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info, "Defaulting to interval of 4 hours.");
                    configurationCheckInterval = 240;
                }

                if (configurationCheckInterval < 16)
                    configurationCheckInterval = 16;

                configurationMonitor.Interval = configurationCheckInterval * 60000;
                SCC.Base.UpdateInterval = configurationMonitor.Interval;
                SCLP.LogProxy.Log(SCLP.LogLevel.Error, "Attempting to update Menumate target details...");
                UpdateMenumateTarget(crmInstance.MenumateTargetDetails);
                copyGroups = new FC.CopyGroup(configurationFile);
                configurationWriteTime =
                    FCX.Helper.GetCurrentWriteTime(configurationInfo);
                mustInitialize = false;
            } catch (FCE.FileOperation foe) {
                Console.WriteLine(foe.ToString());
                SCLP.LogProxy.Log(SCLP.LogLevel.Error, String.Format("Couldn't create target directory for {0}.", foe.TargetName));
                crmInstance.LogBackupOperation("Couldn't create required directory for" + foe.TargetName,
                                               foe.TargetName, "Target initialization failure.", 0);
            } catch (Exception e) {
                String m = "";
                Console.WriteLine(e.ToString());
                PurgeExisting();
                for ( ; e != null; e = e.InnerException)
                    m += String.Format("Message:\n{0}\nStacktrace:{1}\n",
                                       e.Message, e.StackTrace);
                m = String.Format(
                        "Configuration failure; Diagnostic information follows:\n{0}", m);
                SCLP.LogProxy.Log(SCLP.LogLevel.Error, m);
                SCLP.LogProxy.Log(SCLP.LogLevel.Error, "Continuing monitoring...");
                mustInitialize = true;
                SCLP.LogProxy.Log(SCLP.LogLevel.Info, "Defaulting to interval of 30 minutes.");
                SCC.Base.UpdateInterval = configurationMonitor.Interval = 1800000;
            }
        }

        void ReadMailNotificationsConfiguration()
        {
            List<String> recipientAddresses;
            XmlNodeList recipients =
                configurationFile.SelectNodes("//recipient");
            XmlNode sender =
                configurationFile.SelectSingleNode("//Sender");
            String[] senderAttributes = new String[6] {
                "address",
                "password",
                "server",
                "port",
                "username",
                "useSsl"
            };

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Configuring Email error notifications...");

            if (recipients.Count == 0)
                throw new Exception("No recipients defined.");
            else if (sender == null)
                throw new Exception("No sending account information specified.");

            foreach (String a in senderAttributes)
                if (sender.Attributes[a].Value == null)
                    throw new Exception(String.Format(
                                            "No Sender {0} specified.", a));

            recipientAddresses = new List<String>();
            foreach (XmlNode r in recipients)
                if (r.InnerText != null) {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      String.Format(
                                          "Recipient \"{0}\" added.",
                                          r.InnerText));
                    recipientAddresses.Add(r.InnerText);
                } else SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                             "Recipient defined with no address specified, ignoring...");

            try {
                SCLPM.MailConfiguration configuration =
                    new SCLPM.MailConfiguration(
                    sender.Attributes["address"].Value,
                    sender.Attributes["password"].Value,
                    sender.Attributes["server"].Value,
                    Int32.Parse(sender.Attributes["port"].Value),
                    recipientAddresses.ToArray(),
                    sender.Attributes["username"].Value,
                    Boolean.Parse(
                        sender.Attributes["useSsl"].Value));
                mailer = SCLPM.Factory.CreateMailerOfType(
                             "Smtp", configuration);
            } catch (Exception e) {
                throw new Exception(
                    "Malformed Sender specification. Invalid Port or UseSsl setting.", e);
            } finally {
                recipientAddresses.Clear();
                SCLP.LogProxy.MailerInstance = mailer;
            }
        }

        void ResumeFailedOperationMonitorHandler(Object sender,
                EventArgs eventArgs)
        {
            List<String> operationsToRemove = new List<String>();

            resumeFailedOperationMonitor.Stop();

            foreach (KeyValuePair < String,
                     CopyOperationErrorBlock > coeb in
                     failedCopyOperations) {

                if (coeb.Value.Resume())
                    operationsToRemove.Add(coeb.Key);
            }

            foreach (String key in operationsToRemove)
                failedCopyOperations.Remove(key);

            resumeFailedOperationMonitor.Enabled =
                !(failedCopyOperations.Count == 0);
        }

        void UpdateCrmCredentials(string username, string password) {
            XmlNode credentials = configurationFile.SelectSingleNode("//CrmEngine");

            if (credentials.Attributes["username"].Value != username
                || credentials.Attributes["password"].Value != password) {
		        credentials.Attributes["username"].Value = username;
		        credentials.Attributes["password"].Value = password;

                configurationFile.Save(safemateConfigurationPath);
	        }
        }

        bool UpdateMenumateTarget(String[] details)
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Determining whether the Menumate target details are stale...");
            if (crmInstance.IsAccessible) {
                XmlNode target =
                    configurationFile.SelectSingleNode(
                        "//CopyOperation[@name='Menumate']//target[@name='Menumate Backup Store']");
                XmlNode credentials;

                if (target == null)
                    throw new Exception("FUCL!?");

                credentials = target.SelectSingleNode("credentials");

                if (details[0] !=
                    credentials.Attributes["password"].Value ||
                    details[1] !=
                    target.Attributes["url"].Value ||
                    details[2] !=
                    credentials.Attributes[
                        "username"].Value) {
                    credentials.Attributes["password"].Value =
                        details[0];
                    target.Attributes["url"].Value = details[1];
                    credentials.Attributes["username"].Value =
                        details[2];
                    configurationFile.Save(
                        safemateConfigurationPath);

                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      "Target information updated from CRM.");
                    return true;
                }
            } else {
                SCLP.LogProxy.Log(SCLP.LogLevel.Warning,
                                  "The CRM is inaccessible, using potentially stale target details.");
                return false;
            }

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Target information is up to date.");

            return false;
        }

        public SCC.Base CrmInstance
        {
            get {
                return crmInstance;
            }
        }

        static public String SafemateConfigurationPath
        {
            get {
                return safemateConfigurationPath;
            }
        }

        static public String SafemateRootPath
        {
            get {
                return safemateRootPath;
            }
        }

        bool initialCrmCredentialCheck = true;
        static String account;
        XmlDocument configurationFile;
        Double configurationCheckInterval;
        FileInfo configurationInfo;
        Timer configurationMonitor;
        DateTime configurationWriteTime;
        FC.CopyGroup copyGroups;
        static SCC.Base crmInstance;
        Dictionary<String, CopyOperationErrorBlock> failedCopyOperations;
        SCLPM.Base mailer;
        String[] menumateTargetDetails;
        Timer resumeFailedOperationMonitor;
        static String safemateConfigurationPath;
        static String safemateRootPath;
        bool mustInitialize = true;
        static Timer crmCredentialCheckTimer;
        static int dummy = 0;


        /* Bogus name... */
        const string coreVersion = "NjBRt14VSSS98=UV1e1SZsp9U=Q=";
    };

    public class Core
    {
        public Core(EventLog eventLogInstance)
        {
            configuration = new Configuration(eventLogInstance);
        }

        public void Dispose()
        {
            configuration.Dispose();
        }

        Configuration configuration;
    };
};
