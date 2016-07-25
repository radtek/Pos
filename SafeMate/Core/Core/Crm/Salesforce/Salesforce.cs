#define __Safemate_Debug_Crm_Salesforce__

using CCSFB = Core.CrmSalesforceBinding;

using SCC = Safemate.Core.Crm;
using SCCE = Safemate.Core.Crm.Exceptions;
using SCLP = Safemate.Core.LogProxy;

using System;
#if __Safemate_Debug_Crm_Salesforce__
using System.Collections.Generic;
#endif
using System.Net;
using System.Web.Services.Protocols;
using System.Xml;

namespace Safemate.Core.Crm.Salesforce
{
    public class Salesforce : SCC.Base
    {
        enum SettingType {
            Password,
            Uri,
            Username
        };

        static public SCC.Base Create(XmlNode configuration, String account)
        {
            return new Salesforce(configuration, account);
        }

        void CheckCreationResult(CCSFB.SaveResult[] sr)
        {
            try {
                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                  "Checking whether object creation in CRM was successful...");
                foreach (CCSFB.SaveResult sre in sr)
                    if (!sre.success) {
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          "Couldn't create object in CRM. Detected errors:");
                        foreach (CCSFB.Error e in sre.errors)
                            SCLP.LogProxy.Log(SCLP.LogLevel.Error, e.message);
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          "Rolling back CRM changes...");
                    }
            } catch {
                List<String> ids = new List<String>();

                foreach (CCSFB.SaveResult sre in sr)
                    if (sre.success)
                        ids.Add(sre.id);
                binding.delete(ids.ToArray());
                SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                  "Rollback completed.");
            }
        }

        override protected void ConfigurationMonitor(Object sender, EventArgs eventArgs)
        {
            configurationMonitorHeartbeat.Stop();

            try {
                Initialize();
                UpdateMenumateTargetDetails();
            } catch (SCCE.NoPermission np) {
                monitoredSettings.Clear();
                isAccessible = false;
                return;
            }

            configurationMonitorHeartbeat.Start();
        }

        void CreateSafemateAsset()
        {
            CCSFB.Asset safemateAsset;
            CCSFB.SaveResult[] sr;
            CCSFB.QueryResult qr = binding.query(String.Format(
                    "select Id, Name from Asset where Name = 'Safemate' and AccountId = '{0}'",
                    menumateAccountId));
            if (qr.size > 0) {
                safemateAsset = (CCSFB.Asset)qr.records[0];
                safemateAssetId = safemateAsset.Id;
                return;
            }

            safemateAsset = new CCSFB.Asset();
            safemateAsset.AccountId = menumateAccountId;
            safemateAsset.Name = "Safemate";

            sr = binding.create(new CCSFB.sObject[1] {safemateAsset});
            CheckCreationResult(sr);
            safemateAssetId = sr[0].id;
        }

        void CreateSafemateSetting(String name, String value)
        {
            CCSFB.SaveResult[] sr;
            CCSFB.QueryResult qr = binding.query(String.Format(
                    "select Name, Value__c from Safemate_Setting__c where Name = '{0}' and SettingGroup__c = '{1}'",
                    name, safemateSettingGroupId));

            if (qr.size == 0) {
                CCSFB.Safemate_Setting__c s = new CCSFB.Safemate_Setting__c();

                s.Name = name;
                s.SettingGroup__c = safemateSettingGroupId;
                s.Value__c = value;

                sr = binding.create(new CCSFB.sObject[1] {s});
                CheckCreationResult(sr);
                monitoredSettings.Add(name, value);
                return;
            }

            monitoredSettings.Add(
                ((CCSFB.Safemate_Setting__c)qr.records[0]).Name,
                ((CCSFB.Safemate_Setting__c)qr.records[0]).Value__c);
        }

        void CreateSafemateSettingsGroup()
        {
            CCSFB.Safemate_SettingGroup__c sg;
            CCSFB.SaveResult[] sr;
            CCSFB.QueryResult qr = binding.query(String.Format(
                    "select Id from Safemate_SettingGroup__c where Name = 'Safemate' and Asset__c = '{0}'",
                    safemateAssetId));
            if (qr.size > 0) {
                safemateSettingGroupId =
                    (sg = (CCSFB.Safemate_SettingGroup__c)qr.records[0]).Id;
                return;
            }

            sg = new CCSFB.Safemate_SettingGroup__c();
            sg.Asset__c = safemateAssetId;
            sg.Name = "Safemate";

            sr = binding.create(new CCSFB.sObject[1] {sg});
            CheckCreationResult(sr);
            safemateSettingGroupId = sr[0].id;
        }

#if __Safemate_Debug_Crm_Salesforce__
        void ListSafemateDetails()
        {
            CCSFB.QueryResult qr;

            Console.WriteLine("Is Safemate Asset present?");
            qr = binding.query(
                     "select Name from Asset where Name = 'Safemate'");
            if (qr.size == 0) {
                Console.WriteLine("no");
            }

            Console.WriteLine("yes\nIs Safemate Settings Group present?");
            qr = binding.query(String.Format(
                                   "select Name from Safemate_SettingGroup__c where Asset__c = '{0}'", safemateAssetId));
            if (qr.size == 0) {
                Console.WriteLine("no");
            }

            Console.WriteLine("yes\nAre the Menumate target details present?");
            qr = binding.query(String.Format(
                                   "select Name, Value__c from Safemate_Setting__c where SettingGroup__c = '{0}'",
                                   safemateSettingGroupId));
            /*
            			if (qr.size == 0) {
            				Console.WriteLine("no");
            				return;
            			}

            			Console.WriteLine("yes\nDetails are:\n");
            			foreach (CCSFB.Safemate_Setting__c s in qr.records)
            				Console.WriteLine("\t{0} => {1}\n",
            				    s.Name, s.Value__c);
            */
            qr = binding.query(String.Format(
                                   "select Id from Safemate_Backup_Log__c",
                                   safemateAssetId));
            Console.WriteLine("Searching for Backup Logs...");
            if (qr.size == 0) {
                Console.WriteLine("\nNo logs found.\n");
                //return;
            }

            foreach (CCSFB.Safemate_Backup_Log__c _bl in qr.records)
                Console.WriteLine(_bl.Id);
        }
#endif

        public override void LogBackupOperation(String detailedStatus,
                                                String name, String status, Double backupSize)
        {
            if (allowedRemoteBackup) {
                CCSFB.SaveResult[] sr;

                SCLP.LogProxy.Log(SCLP.LogLevel.Info, String.Format(
                                      "Attempting to log operation {0} status to CRM...", name));
                try {
                    CCSFB.Safemate_Backup_Log__c bl;
                    CCSFB.QueryResult _qr = binding.query(String.Format(
                            "select Id, Last_transfer_result__c, Name, Result_Description__c, Time_of_backup__c, LastTransferSize__c from Safemate_Backup_Log__c where BelongsToAsset__c = '{0}' and Name = '{1}'",
                            safemateAssetId, name));
                    if (_qr.size == 1)
                        bl = (CCSFB.Safemate_Backup_Log__c)_qr.records[0];
                    else bl = new CCSFB.Safemate_Backup_Log__c();

                    bl.Last_transfer_result__c = status;
                    bl.Result_Description__c = detailedStatus;
                    bl.Time_of_backup__c = DateTime.Now;
                    bl.Time_of_backup__cSpecified = true;
                    bl.LastTransferSize__c = backupSize;
                    bl.LastTransferSize__cSpecified = true;

                    if (_qr.size == 0) {
                        bl.Name = name;
                        bl.BelongsToAsset__c = safemateAssetId;
                        sr = binding.create(new CCSFB.sObject[1] {bl});
                        if (!sr[0].success)
                            ;
                    }  else sr = binding.update(new CCSFB.sObject[1] {bl});

                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      "Successfully logged backup status to CRM.");
                } catch {
                    SCLP.LogProxy.Log(SCLP.LogLevel.Error, "Couldn't log to CRM.");
                }
            }
        }

        void UpdateAccountsMenumateVersion()
        {
            CCSFB.Account a;
            CCSFB.Menumate_Registration__c mmr;
            string mmv = "__MM_VERSION__";
            string mmvid;
            CCSFB.QueryResult qr;

            qr = binding.query(String.Format("select Id, Menumate_Version__c from Menumate_Registration__c where Customer__c = '{0}'",
                                             menumateAccountId));
            if (qr.size == 0) {
                qr = binding.query(String.Format("select Id, Menumate_Version__c from Account where Id = '{0}'",
                                                 menumateAccountId));
                a = (CCSFB.Account)qr.records[0];
                a.Menumate_Version__c = mmv;
                binding.update(new CCSFB.sObject[] {a});
                return;
            }

            mmr = (CCSFB.Menumate_Registration__c)qr.records[0];

            qr = binding.query(String.Format("select Id, Name from Menumate_Version__c where Name = '{0}'", mmv));
            mmvid = ((CCSFB.Menumate_Version__c)qr.records[0]).Id;

            mmr.Menumate_Version__c = mmvid;
            binding.update(new CCSFB.sObject[] {mmr});
        }

        void Login(String password, String username)
        {
            CCSFB.Account a;
            CCSFB.LoginResult lr =
                binding.login(username, password);
            CCSFB.QueryResult qr;

            if (binding.SessionHeaderValue == null)
                binding.SessionHeaderValue = new CCSFB.SessionHeader();
            binding.SessionHeaderValue.sessionId = lr.sessionId;
            binding.Url = lr.serverUrl;

            qr = binding.query(String.Format(
                                   "select Id from Account where Name = '{0}'", accountName));
            if (qr.size == 0)
                throw new SCCE.CannotLogin(String.Format("Account {0} doesn't exist.", accountName));
            menumateAccountId =
                (a = (CCSFB.Account)qr.records[0]).Id;
        }

        void Initialize()
        {
            CCSFB.DescribeSObjectResult[] dsor;

            if (binding == null)
                binding = new CCSFB.SforceService();
            else if (isAccessible)
                return;

            Login(password, username);
            UpdateAccountsMenumateVersion();

            dsor = binding.describeSObjects(
                       new String[4] {"Account",
                                      "Asset",
                                      "Safemate_Setting__c",
                                      "Safemate_SettingGroup__c"
                                     });

            CreateConfigurationDetailsIfNecessary();
            isAccessible = true;

            if (!(allowedRemoteBackup = DisableIfCustomerDoesntHavePermission())) {
                SCLP.LogProxy.Log(SCLP.LogLevel.Info, String.Format(
                                      "Account {0} doesn't have permission to use remote logging functionality.",
                                      accountName));
                throw new SCCE.NoPermission("Remote logging functionality inhibited.");
            }
        }

#if __Safemate_Debug_Crm_Salesforce__
        void RemoveSafemateDetails()
        {
            CCSFB.DeleteResult[] dr;
            List<String> objectsToRemove = new List<String>();

            CCSFB.QueryResult qr = binding.query(String.Format(
                    "select Id from Safemate_SettingGroup__c where Asset__c = '{0}'",
                    safemateAssetId));

            if (qr.size > 0)
                foreach (CCSFB.Safemate_SettingGroup__c sg in qr.records)
                    objectsToRemove.Add(sg.Id);

            qr = binding.query(String.Format(
                                   "select Id from Safemate_Setting__c where SettingGroup__c = '{0}'",
                                   safemateSettingGroupId));

            if (qr.size > 0)
                foreach (CCSFB.Safemate_Setting__c s in qr.records)
                    objectsToRemove.Add(s.Id);

            qr = binding.query(String.Format(
                                   "select Id from Safemate_Backup_Log__c where BelongsToAsset__c = '{0}'",
                                   safemateAssetId));

            if (qr.size > 0)
                foreach (CCSFB.Safemate_Backup_Log__c bl in qr.records)
                    objectsToRemove.Add(bl.Id);

            qr = binding.query(String.Format(
                                   "select Id from Asset where Id = '{0}'",
                                   safemateAssetId));

            if (qr.size > 0)
                foreach (CCSFB.Asset a in qr.records)
                    objectsToRemove.Add(a.Id);

            if (objectsToRemove.Count > 0)
                dr = binding.delete(objectsToRemove.ToArray());
        }
#endif

        void UpdateMenumateTargetDetails()
        {
            String[] fetch = new String[3] {"Password",
                                            "Uri",
                                            "Username"
                                           };
            CCSFB.QueryResult qr;

            foreach (String n in fetch) {
                qr = binding.query(String.Format(
                                       "select Name, Value__c from Safemate_Setting__c where SettingGroup__c = '{0}'",
                                       safemateSettingGroupId));
                foreach (CCSFB.Safemate_Setting__c s in qr.records)
                    monitoredSettings[s.Name] = s.Value__c;
            }

            menumateTargetDetails[0] = monitoredSettings["Password"];
            menumateTargetDetails[1] = monitoredSettings["Uri"];
            menumateTargetDetails[2] = monitoredSettings["Username"];
        }

        void CreateConfigurationDetailsIfNecessary()
        {
            CreateSafemateAsset();
            CreateSafemateSettingsGroup();
            CreateSafemateSetting(
                "Password", "ysAhARUNA2E%ugYXu@AmedEdydyBe@e#");
            CreateSafemateSetting(
                "Uri", "ftp://ftp.menumate.com/");
            CreateSafemateSetting(
                "Username", "safemate");
        }

        ~Salesforce()
        {
        }

        bool DisableIfCustomerDoesntHavePermission()
        {
            CCSFB.QueryResult qr = binding.query(String.Format(
                    "select Remote_Backup__c from Account where Id = '{0}'",
                    menumateAccountId));

            return ((CCSFB.Account)qr.records[0]).Remote_Backup__c.Value == true;
        }

        public Salesforce(XmlNode configuration, String account)
        {
            type = "Salesforce";

            monitoredSettings = new Dictionary<String, String>();

            accountName = account;
            password = configuration.Attributes["password"].Value;
            username = configuration.Attributes["username"].Value;

            ConfigurationMonitor(null, null);
        }

        CCSFB.SforceService binding;
        String menumateAccountId;
        Dictionary<String, String> monitoredSettings;
        String password;
        String safemateAssetId;
        String safemateSettingGroupId;
        String username;
        bool allowedRemoteBackup;
    };
};
