using System;
using System.IO;
using System.Linq;
using System.Xml;

using XMLManager;

namespace MenumateServices.Utilities
{
    public class MenumateServicesConfigurationManager
    {
        private string _filePath = @"";
        private readonly XmlDocument _configurationFileDocument = new XmlDocument();

        private bool _modified;

        private bool _uniqueDatabaseServer = true;
        private string _uniqueDatabaseServerName = DefaultDatabaseServerName;
        private string _uniqueDatabaseServerFolder = DefaultDatabaseFolder;

        private bool _databaseServer;
        private string _webmateDatabaseServerName = DefaultDatabaseServerName;
        private string _webmateDatabaseServerFolder = DefaultDatabaseFolder;

        static readonly string ProgramFilesPath = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
        private const string ConfigurationFileName = @"MenumateServicesConfig.xml";

        static readonly string DefaultConfigFilePath = string.Format(@"{0}\{1}", ServiceInfo.Instance.StartPath, ConfigurationFileName);

        private const string DefaultDatabaseServerName = @"localhost";
        static readonly string DefaultDatabaseFolder = string.Format(@"{0}\{1}", ProgramFilesPath, @"Menumate");

        /// <summary>
        /// Sets config file URI to default.
        /// If the config file does not exist it will be created
        /// </summary>
        internal MenumateServicesConfigurationManager()
        {
            FilePath = DefaultConfigFilePath;
        }

        /// <summary>
        /// 
        /// </summary>
        private class MenumateServicesConfigurationManagerNested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static MenumateServicesConfigurationManagerNested()
            { }

            internal static readonly MenumateServicesConfigurationManager MenumateServicesConfigurationManagerInstance = new MenumateServicesConfigurationManager();
        }

        public static MenumateServicesConfigurationManager Instance
        {
            get
            {
                return MenumateServicesConfigurationManagerNested.MenumateServicesConfigurationManagerInstance;
            }
        }

        public string FilePath
        {
            set
            {
                if (InitializeAndLoadFile(value))
                    _filePath = value;
            }
            get
            {
                return _filePath;
            }
        }

        public bool Modified
        {
            get
            {
                return _modified;
            }
        }

        public bool UniqueDatabaseServer
        {
            set
            {
                _uniqueDatabaseServer = value;
                _modified = true;
            }
            get
            {
                return _uniqueDatabaseServer;
            }
        }

        public string UniqueDatabaseServerName
        {
            set
            {
                _uniqueDatabaseServerName = value;
                _modified = true;
            }
            get
            {
                return _uniqueDatabaseServerName;
            }
        }

        public string UniqueDatabaseServerFolder
        {
            set
            {
                _uniqueDatabaseServerFolder = value;
                _modified = true;
            }
            get
            {
                return _uniqueDatabaseServerFolder;
            }
        }

        public bool DatabaseServers
        {
            set
            {
                _databaseServer = value;
                _modified = true;
            }
            get
            {
                return _databaseServer;
            }
        }

        public string WebmateDatabaseServerName
        {
            set
            {
                _webmateDatabaseServerName = value;
                _modified = true;
            }
            get
            {
                return _webmateDatabaseServerName;
            }
        }

        public string WebmateDatabaseServerFolder
        {
            set
            {
                _webmateDatabaseServerFolder = value;
                _modified = true;
            }
            get
            {
                return _webmateDatabaseServerFolder;
            }
        }

        public void Reset()
        {
            _modified = false;
        }

        public void Reload()
        {
            InitializeAndLoadFile(_filePath);
        }

        public void SaveSettings()
        {
            //This condition means XML config was created already
            if (_filePath != @"")
            {
                // Databases
                XmlNode uniqueDatabaseServerNode = GetUniqueDatabaseServerNode();
                XmlNode dbServersNode = GetDatabaseServerNode();

                XmlNode webmateDatabaseServerNode = dbServersNode.FirstChild;

                XMLDocManager.SetAttribute(uniqueDatabaseServerNode, @"checked", Convert.ToString(_uniqueDatabaseServer));
                XMLDocManager.SetAttribute(uniqueDatabaseServerNode, @"name", _uniqueDatabaseServerName);
                XMLDocManager.SetAttribute(uniqueDatabaseServerNode, @"folder", _uniqueDatabaseServerFolder);

                XMLDocManager.SetAttribute(dbServersNode, @"checked", Convert.ToString(_databaseServer));

                XMLDocManager.SetAttribute(webmateDatabaseServerNode, @"name", _webmateDatabaseServerName);
                XMLDocManager.SetAttribute(webmateDatabaseServerNode, @"folder", _webmateDatabaseServerFolder);

                _configurationFileDocument.Save(_filePath);

                _modified = false;
            }
        }

        protected bool InitializeAndLoadFile(string filePath)
        {
            CheckCurrentSettings();

            if (filePath.Trim() == @"")
            {
                filePath = DefaultConfigFilePath;
            }

            if (!File.Exists(filePath))
            {
                try
                {
                    CreateConfigurationFile(filePath);
                }
                catch
                {
                    return false;
                }
            }

            var xmlTextReader = new XmlTextReader(filePath);

            try
            {
                xmlTextReader.Read();
                _configurationFileDocument.Load(xmlTextReader);
                xmlTextReader.Close();

                LoadSettings();
            }
            catch (Exception exception)
            {
                xmlTextReader.Close();
                return false;
            }

            return true;
        }

        protected void CheckCurrentSettings()
        {
            if (_modified)
            {
                SaveSettings();
            }
        }

        protected void CreateConfigurationFile(string filePath)
        {
            // Create the XML Declaration, and append it to XML document
            XMLDocManager.CreateXmlDeclaration(_configurationFileDocument, @"1.0", @"utf-8", null);

            // Create the root element
            XmlElement root = XMLDocManager.CreateRoot(_configurationFileDocument, @"MenumateServices");

            // Databases
            XmlNode databasesNode = XMLDocManager.AddNode(_configurationFileDocument, root, @"Databases");
            XmlNode uniqueDatabaseServerNode = XMLDocManager.AddNode(_configurationFileDocument, databasesNode, @"UniqueDBServer");
            XmlNode dbServersNode = XMLDocManager.AddNode(_configurationFileDocument, databasesNode, @"DBServers");

            XmlNode webmateDatabaseServerNode = XMLDocManager.AddNode(_configurationFileDocument, dbServersNode, @"WebmateDBServer");

            XMLDocManager.AddAttribute(_configurationFileDocument, uniqueDatabaseServerNode, @"checked", @"true");
            XMLDocManager.AddAttribute(_configurationFileDocument, uniqueDatabaseServerNode, @"name", DefaultDatabaseServerName);
            XMLDocManager.AddAttribute(_configurationFileDocument, uniqueDatabaseServerNode, @"folder", DefaultDatabaseFolder);

            XMLDocManager.AddAttribute(_configurationFileDocument, dbServersNode, @"checked", @"false");

            XMLDocManager.AddAttribute(_configurationFileDocument, webmateDatabaseServerNode, @"name", DefaultDatabaseServerName);
            XMLDocManager.AddAttribute(_configurationFileDocument, webmateDatabaseServerNode, @"folder", DefaultDatabaseFolder);

            XmlNode ipAddressesNode = XMLDocManager.AddNode(_configurationFileDocument, root, @"IPAddresses");

            _configurationFileDocument.Save(filePath);
        }

        protected void LoadSettings()
        {
            _uniqueDatabaseServer = ReadUniqueDatabaseServer();
            _uniqueDatabaseServerName = ReadUniqueDatabaseServerName();
            _uniqueDatabaseServerFolder = ReadUniqueDatabaseServerFolder();

            _databaseServer = ReadDatabaseServers();
            _webmateDatabaseServerName = ReadWebmateDatabaseServerName();
            _webmateDatabaseServerFolder = ReadWebmateDatabaseServerFolder();

        }

        private bool ReadUniqueDatabaseServer()
        {
            bool result = true;
            try
            {
                XmlNode uniqueDatabaseServerNode = GetUniqueDatabaseServerNode();
                result = Convert.ToBoolean(XMLDocManager.GetAttribute(uniqueDatabaseServerNode, @"checked", @"true"));
            }
            catch
            {
                result = true;
            }
            return result;
        }

        private string ReadUniqueDatabaseServerName()
        {
            string result = DefaultDatabaseServerName;

            try
            {
                XmlNode uniqueDatabaseServerNode = GetUniqueDatabaseServerNode();
                result = XMLDocManager.GetAttribute(uniqueDatabaseServerNode, @"name", DefaultDatabaseServerName);
            }
            catch
            {
                result = DefaultDatabaseServerName;
            }

            return result;
        }

        private string ReadUniqueDatabaseServerFolder()
        {
            string result = DefaultDatabaseFolder;

            try
            {
                XmlNode uniqueDatabaseServerNode = GetUniqueDatabaseServerNode();
                result = XMLDocManager.GetAttribute(uniqueDatabaseServerNode, @"folder", DefaultDatabaseFolder);
            }
            catch
            {
                result = DefaultDatabaseFolder;
            }

            return result;
        }

        private bool ReadDatabaseServers()
        {
            bool result = false;

            try
            {
                XmlNode dbServersNode = GetDatabaseServerNode();
                result = Convert.ToBoolean(XMLDocManager.GetAttribute(dbServersNode, @"checked", @"false"));
            }
            catch
            {
                result = false;
            }

            return result;
        }

        private string ReadWebmateDatabaseServerName()
        {
            string result = DefaultDatabaseServerName;
            try
            {
                XmlNode databaseServerNode = GetDatabaseServerNode();
                XmlNode webmateDatabaseServerNode = databaseServerNode.FirstChild;

                result = XMLDocManager.GetAttribute(webmateDatabaseServerNode, @"name", DefaultDatabaseServerName);
            }
            catch
            {
                result = DefaultDatabaseServerName;
            }

            return result;
        }

        private string ReadWebmateDatabaseServerFolder()
        {
            string result = DefaultDatabaseFolder;

            try
            {
                XmlNode databaseServerNode = GetDatabaseServerNode();
                XmlNode webmateDatabaseServerNode = databaseServerNode.FirstChild;

                result = XMLDocManager.GetAttribute(webmateDatabaseServerNode, @"folder", DefaultDatabaseFolder);
            }
            catch
            {
                result = DefaultDatabaseFolder;
            }

            return result;
        }

        private XmlNode GetUniqueDatabaseServerNode()
        {
            XmlNode dbNode = GetDatabaseNode();
            return dbNode.FirstChild;
        }

        private XmlNode GetDatabaseServerNode()
        {
            XmlNode dbNode = GetDatabaseNode();
            return dbNode.FirstChild.NextSibling;
        }

        private XmlNode GetIpAddressesNode()
        {
            var ipAddressesNode = XMLDocManager.GetNode(_configurationFileDocument, @"IPAddresses");

            //If the ipAddress node is null means we are hitting an existing installation need to take care..silently
            if (ipAddressesNode == null)
            {
                var root = XMLDocManager.GetRoot(_configurationFileDocument);
                ipAddressesNode = XMLDocManager.AddNode(_configurationFileDocument, root, @"IPAddresses");

                _configurationFileDocument.Save(_filePath);
            }

            return ipAddressesNode;
        }

        private XmlNode GetDatabaseNode()
        {
            return XMLDocManager.GetNode(_configurationFileDocument, @"Databases");
        }
    }
}
