using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.IO;

using XMLManager;
using System.Configuration;
using System.Windows.Forms;
using AccountingIntegration.Model;
using AccountingIntegration.Utility;
using AccountingIntegration.MYOB;

namespace AccountingIntegration
{
    public class AccountingIntegrationConfigManager
    {
        private AccountingAuthConfiguration _accountingConfiguration;
        private static volatile AccountingIntegrationConfigManager _instance;
        private static readonly object syncRoot = new Object();
        string _accountingAppInUse;

        private AccountingIntegrationConfigManager()
        {
            LoadConfiguration();
            _accountingAppInUse = "";
        }

        public static AccountingIntegrationConfigManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new AccountingIntegrationConfigManager();
                    }
                }

                return _instance;
            }
        }

        public string AccountingAppInUse
        {
            get
            {

                if (!string.IsNullOrWhiteSpace(_accountingConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse))
                {
                    _accountingAppInUse = _accountingConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse;
                }
                else
                {
                    _accountingAppInUse = _accountingConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.AppInUse;
                }
                return _accountingAppInUse;
            }
        }

        public void LoadConfiguration()
        {

            var configFilePath = ServiceInfo.GetFileURI(AccountingIntegration.Properties.Settings.Default.AccountingIntegrationConfigFilename);
            if (File.Exists(configFilePath))
            {
                _accountingConfiguration = XeroHelper.DeSerializeFromFilePath<AccountingAuthConfiguration>(configFilePath);
            }
            else
            {
                _accountingConfiguration = new AccountingAuthConfiguration
                {
                    AccountingSystemList = new List<AccountingSystem>()
                    {
                       new AccountingSystem()
                       {
                           Type = "Xero",
                           CredentialsList = new CredentialsList()
                           {
                                Credentials = new List<Credentials>()
                           }
                       },
                       new AccountingSystem()
                       {
                           Type = "MYOB",
                           CredentialsList = new CredentialsList()
                           {
                                Credentials = new List<Credentials>()
                           }
                       }
                    }
                };
            }
        }

        public AccountingAuthConfiguration GetConfiguaration()
        {
            return _accountingConfiguration;
        }

        public Credentials GetActiveOrganisation()
        {
            var activeCredential = _accountingConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.Credentials.SingleOrDefault(
                s => s.AppID == _accountingConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.AppInUse);
            return activeCredential;
        }

        public Credentials GetByAppId(string appId)
        {
            var activeCredential = _accountingConfiguration.AccountingSystemList.ElementAt(0).CredentialsList.Credentials.SingleOrDefault(
               s => s.AppID == appId);
            if (activeCredential == null)
                activeCredential = _accountingConfiguration.AccountingSystemList.ElementAt(1).CredentialsList.Credentials.SingleOrDefault(
               s => s.AppID == appId);
            return activeCredential;
        }

        public void SaveAccountingConfiguration(AccountingAuthConfiguration accountingAuthConfiguration)
        {
            _accountingConfiguration = accountingAuthConfiguration;
            var configFilePath = ServiceInfo.GetFileURI(AccountingIntegration.Properties.Settings.Default.AccountingIntegrationConfigFilename);
            var configStr = XeroHelper.Serialize<AccountingAuthConfiguration>(accountingAuthConfiguration);
            File.WriteAllText(configFilePath, configStr);
        }

        public void SaveCompanyFileCredentials(string userName, string password)
        {
            try
            {
                CompanyFileCredentialsCF credentialsMain = new CompanyFileCredentialsCF();
                credentialsMain.CredentialsDetails = new CredentialsCF();
                credentialsMain.CredentialsDetails.UserName = userName;
                credentialsMain.CredentialsDetails.Password = password;
                var companyFileCredentialsPath = ServiceInfo.GetFileURI((AccountingIntegration.Properties.Settings.Default.CompanyFileDetails));
                var companyFileCredentialsStr = XeroHelper.Serialize<CompanyFileCredentialsCF>(credentialsMain);
                File.WriteAllText(companyFileCredentialsPath, companyFileCredentialsStr);
            }
            catch (Exception ex)
            {
                ServiceLogger.Log("Exception in credentials file creation " + ex.Message);
            }
        }

    }
}

