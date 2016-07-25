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
        private string _chefmateDatabaseServerName = DefaultDatabaseServerName;
        private string _chefmateDatabaseServerFolder = DefaultDatabaseFolder;
        private string _clippIpAddress;
        private string _clippPort;

        static readonly string ProgramFilesPath = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
        private const string ConfigurationFileName = @"MenumateServicesConfig.xml";

        static readonly string DefaultConfigFilePath = string.Format(@"{0}\{1}", ServiceInfo.Instance.StartPath, ConfigurationFileName);

        private const string DefaultClippIpAddress = @"localhost";
        private const string DefaultClippPort = @"8081";
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

        public string ChefmateDatabaseServerName
        {
            set
            {
                _chefmateDatabaseServerName = value;
                _modified = true;
            }
            get
            {
                return _chefmateDatabaseServerName;
            }
        }

        public string ChefmateDatabaseServerFolder
        {
            set
            {
                _chefmateDatabaseServerFolder = value;
                _modified = true;
            }
            get
            {
                return _chefmateDatabaseServerFolder;
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
                XmlNode chefmateDatabaseServerNode = dbServersNode.FirstChild.NextSibling;

                XMLDocManager.SetAttribute(uniqueDatabaseServerNode, @"checked", Convert.ToString(_uniqueDatabaseServer));
                XMLDocManager.SetAttribute(uniqueDatabaseServerNode, @"name", _uniqueDatabaseServerName);
                XMLDocManager.SetAttribute(uniqueDatabaseServerNode, @"folder", _uniqueDatabaseServerFolder);

                XMLDocManager.SetAttribute(dbServersNode, @"checked", Convert.ToString(_databaseServer));

                XMLDocManager.SetAttribute(webmateDatabaseServerNode, @"name", _webmateDatabaseServerName);
                XMLDocManager.SetAttribute(webmateDatabaseServerNode, @"folder", _webmateDatabaseServerFolder);

                XMLDocManager.SetAttribute(chefmateDatabaseServerNode, @"name", _chefmateDatabaseServerName);
                XMLDocManager.SetAttribute(chefmateDatabaseServerNode, @"folder", _chefmateDatabaseServerFolder);

                //IpAddresses
                XmlNode clippPortNode = GetClippPortNode();
                XmlNode clippIpAddressNode = GetClippIpAddressNode();

                XMLDocManager.SetAttribute(clippPortNode, @"value", _clippPort);
                XMLDocManager.SetAttribute(clippIpAddressNode, @"value", _clippIpAddress);

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
            XmlNode chefmateDatabaseServerNode = XMLDocManager.AddNode(_configurationFileDocument, dbServersNode, @"ChefmateDBServer");

            XMLDocManager.AddAttribute(_configurationFileDocument, uniqueDatabaseServerNode, @"checked", @"true");
            XMLDocManager.AddAttribute(_configurationFileDocument, uniqueDatabaseServerNode, @"name", DefaultDatabaseServerName);
            XMLDocManager.AddAttribute(_configurationFileDocument, uniqueDatabaseServerNode, @"folder", DefaultDatabaseFolder);

            XMLDocManager.AddAttribute(_configurationFileDocument, dbServersNode, @"checked", @"false");

            XMLDocManager.AddAttribute(_configurationFileDocument, webmateDatabaseServerNode, @"name", DefaultDatabaseServerName);
            XMLDocManager.AddAttribute(_configurationFileDocument, webmateDatabaseServerNode, @"folder", DefaultDatabaseFolder);

            XMLDocManager.AddAttribute(_configurationFileDocument, chefmateDatabaseServerNode, @"name", DefaultDatabaseServerName);
            XMLDocManager.AddAttribute(_configurationFileDocument, chefmateDatabaseServerNode, @"folder", DefaultDatabaseFolder);

            XmlNode ipAddressesNode = XMLDocManager.AddNode(_configurationFileDocument, root, @"IPAddresses");
            XmlNode clippIpAddressNode = XMLDocManager.AddNode(_configurationFileDocument, ipAddressesNode, @"ClippIPAddress");
            XmlNode clippPortNode = XMLDocManager.AddNode(_configurationFileDocument, ipAddressesNode, @"ClippPort");

            XMLDocManager.AddAttribute(_configurationFileDocument, clippIpAddressNode, @"value", DefaultClippIpAddress);
            XMLDocManager.AddAttribute(_configurationFileDocument, clippPortNode, @"value", DefaultClippPort);

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
            _chefmateDatabaseServerName = ReadChefmateDatabaseServerName();
            _chefmateDatabaseServerFolder = ReadChefmateDatabaseServerFolder();

            _clippIpAddress = ReadClippIpAddress();
            _clippPort = ReadClippPort();
        }

        private string ReadClippPort()
        {
            string result = DefaultClippPort;

            try
            {
                XmlNode clippPortNode = GetClippPortNode();
                result = XMLDocManager.GetAttribute(clippPortNode, @"value", DefaultClippPort);
            }
            catch
            {
                result = DefaultClippPort;
            }

            return result;
        }

        private string ReadClippIpAddress()
        {
            string result = DefaultClippIpAddress;

            try
            {
                XmlNode clippIpAddressNode = GetClippIpAddressNode();
                result = XMLDocManager.GetAttribute(clippIpAddressNode, @"value", DefaultClippIpAddress);
            }
            catch
            {
                result = DefaultClippIpAddress;
            }

            return result;
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

        private string ReadChefmateDatabaseServerName()
        {
            string result = DefaultDatabaseServerName;

            try
            {
                XmlNode databaseServerNode = GetDatabaseServerNode();
                XmlNode chefmateDatabaseServerNode = databaseServerNode.FirstChild.NextSibling;

                result = XMLDocManager.GetAttribute(chefmateDatabaseServerNode, @"name", DefaultDatabaseServerName);
            }
            catch
            {
                result = DefaultDatabaseServerName;
            }

            return result;
        }

        private string ReadChefmateDatabaseServerFolder()
        {
            string result = DefaultDatabaseFolder;

            try
            {
                XmlNode databaseServerNode = GetDatabaseServerNode();
                XmlNode chefmateDatabaseServerNode = databaseServerNode.FirstChild.NextSibling;

                result = XMLDocManager.GetAttribute(chefmateDatabaseServerNode, @"folder", DefaultDatabaseFolder);
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

        private XmlNode GetClippPortNode()
        {

            XmlNode ipAddressesNode = GetIpAddressesNode();
            var clippPortNode = ipAddressesNode.ChildNodes.Cast<XmlNode>()
                .SingleOrDefault(cn => cn.Name == @"ClippPort");

            return clippPortNode;
        }

        private XmlNode GetClippIpAddressNode()
        {
            XmlNode ipAddressesNode = GetIpAddressesNode();
            var clippIpAddressNode = ipAddressesNode.ChildNodes.Cast<XmlNode>()
                .SingleOrDefault(cn => cn.Name == @"ClippIPAddress");

            return clippIpAddressNode;
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
                XmlNode clippIpAddressNode = XMLDocManager.AddNode(_configurationFileDocument, ipAddressesNode, @"ClippIPAddress");
                XmlNode clippPortNode = XMLDocManager.AddNode(_configurationFileDocument, ipAddressesNode, @"ClippPort");

                XMLDocManager.AddAttribute(_configurationFileDocument, clippIpAddressNode, @"value", DefaultClippIpAddress);
                XMLDocManager.AddAttribute(_configurationFileDocument, clippPortNode, @"value", DefaultClippPort);

                _configurationFileDocument.Save(_filePath);
            }

            return ipAddressesNode;
        }

        private XmlNode GetDatabaseNode()
        {
            return XMLDocManager.GetNode(_configurationFileDocument, @"Databases");
        }


        public string ClippIpAddress
        {
            get
            {
                return _clippIpAddress;
            }
            set
            {
                _clippIpAddress = value;
                _modified = true;
            }
        }

        public string ClippPort
        {
            get
            {
                return _clippPort;
            }
            set
            {
                _clippPort = value;
                _modified = true;
            }
        }
    }
}
