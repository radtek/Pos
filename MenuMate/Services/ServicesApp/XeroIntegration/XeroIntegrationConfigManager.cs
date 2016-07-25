using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.IO;

using XMLManager;
using System.Configuration;
using XeroIntegration.Utility;
using XeroIntegration.Model;

namespace XeroIntegration
{
    public class XeroIntegrationConfigManager
    {
        private XeroAuthConfiguration _xeroAuthConfiguration;
        private static volatile XeroIntegrationConfigManager _instance;
        private static readonly object syncRoot = new Object();

        private XeroIntegrationConfigManager()
        {
            _xeroAuthConfiguration = LoadConfiguration();
        }

        public static XeroIntegrationConfigManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new XeroIntegrationConfigManager();
                    }
                }

                return _instance;
            }
        }

        public string XeroAppInUse
        {
            get
            {
                if(string.IsNullOrWhiteSpace(_xeroAuthConfiguration.OAuth.CredentialsList.AppInUse))
                    return string.Empty;
                return _xeroAuthConfiguration.OAuth.CredentialsList.AppInUse;
            }

        }

        public XeroAuthConfiguration LoadConfiguration()
        {
            XeroAuthConfiguration xeroConfiguration;
            var configFilePath = ServiceInfo.GetFileURI(XeroIntegration.Properties.Settings.Default.XeroIntegrationConfigFilename);
            if (File.Exists(configFilePath))
            {
                xeroConfiguration = XeroHelper.DeSerializeFromFilePath<XeroAuthConfiguration>(configFilePath);
            }
            else
            {
                xeroConfiguration = new XeroAuthConfiguration
                {
                    OAuth = new OAuth()
                    {
                        CredentialsList = new CredentialsList()
                        {
                            Credentials = new List<Credentials>()
                        }
                    }
                };
            }
            return xeroConfiguration;
        }

        public XeroAuthConfiguration GetConfiguaration()
        {
            return _xeroAuthConfiguration;
        }

        public Credentials GetActiveOrganisation()
        {
            var activeCredential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.SingleOrDefault(
                s => s.AppID == _xeroAuthConfiguration.OAuth.CredentialsList.AppInUse);
            return activeCredential;
        }

        public Credentials GetByAppId(string appId)
        {
            var activeCredential = _xeroAuthConfiguration.OAuth.CredentialsList.Credentials.SingleOrDefault(
               s => s.AppID == appId);
            return activeCredential;
        }

        public void SaveXeroConfiguration(XeroAuthConfiguration xeroAuthConfiguration)
        {
            _xeroAuthConfiguration = xeroAuthConfiguration;
            var configFilePath = ServiceInfo.GetFileURI(XeroIntegration.Properties.Settings.Default.XeroIntegrationConfigFilename);
            var configStr = XeroHelper.Serialize<XeroAuthConfiguration>(xeroAuthConfiguration);
            File.WriteAllText(configFilePath, configStr);
        }

    }
}

