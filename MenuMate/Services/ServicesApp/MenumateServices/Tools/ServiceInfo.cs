using System.IO;
using System.Reflection;
using System.Xml;

using MenumateServices.Utilities;

namespace MenumateServices
{
    /// <summary>
    /// 
    /// </summary>
    public sealed class ServiceInfo
    {
        public ServiceInfo()
        { }

        public static ServiceInfo Instance
        {
            get
            {
                return NestedServiceInfo.ServiceInfoInstance;
            }
        }

        private class NestedServiceInfo
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static NestedServiceInfo()
            { }

            internal static readonly ServiceInfo ServiceInfoInstance = new ServiceInfo();
        }

        public string StartPath
        {
            get
            {
                return Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetName().CodeBase).Substring(6);
            }
        }

        public string GetFilePath(string fileName)
        {
            return StartPath + @"\" + fileName;
        }

        public XmlDocument GetXmlDocumentFromFile(string configFilename)
        {
            var result = new XmlDocument();

            XmlReader reader = new XmlTextReader(GetFilePath(configFilename));
            reader.Read();
            result.Load(reader);

            return result;
        }

        public string NotNullString(string inStr)
        {
            return inStr ?? @"";
        }

        public string CinfigurationFilePath()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;
            return configurationManager.FilePath;
        }

        public void ReloadConfigurationFile()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;
            configurationManager.Reload();
        }

        public string WebmateDatabaseLocation()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;

            ReloadConfigurationFile();

            return configurationManager.UniqueDatabaseServer ?
                configurationManager.UniqueDatabaseServerFolder : configurationManager.WebmateDatabaseServerFolder;
        }

        public string ChefmateDatabaseLocation()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;

            ReloadConfigurationFile();

            return configurationManager.UniqueDatabaseServer ?
                configurationManager.UniqueDatabaseServerFolder : configurationManager.ChefmateDatabaseServerFolder;
        }

        public string WebmateDatabaseServerPath()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;

            ReloadConfigurationFile();

            return configurationManager.UniqueDatabaseServer ?
                configurationManager.UniqueDatabaseServerName : configurationManager.WebmateDatabaseServerName;
        }

        public string ChefmateDatabaseServerPath()
        {
            MenumateServicesConfigurationManager configurationManager = MenumateServicesConfigurationManager.Instance;

            ReloadConfigurationFile();

            return configurationManager.UniqueDatabaseServer ?
                configurationManager.UniqueDatabaseServerName : configurationManager.ChefmateDatabaseServerName;
        }
    }
}
