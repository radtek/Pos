using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Xml;

using XMLManager;

namespace MenumateServices.Config
{
    class MMServicesConfigManager
    {
        static string programFilesPath = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ProgramFiles);
        static string configFileName = @".\MenumateServicesConfig.xml";

        static string DEFAULT_CONFIG_FILE_URI = configFileName; //ServiceInfo.Instance.GetFileURI(configFileName);
        //static string DEFAULT_CONFIG_FILE_URI = string.Format(@"{0}\{1}\{2}\{3}", programFilesPath, @"Menumate", @"", configFileName);

        static string DEFAULT_DB_SERVER_NAME = @"localhost";
        static string DEFAULT_DB_FOLDER = string.Format(@"{0}\{1}", programFilesPath, @"Menumate");
        //static string DEFAULT_DB_FOLDER = ServiceInfo.Instance.StartPath;

        /// <summary>
        /// Sets config file URI to default.
        /// If the config file does not exist it will be created
        /// </summary>
        internal MMServicesConfigManager()
        {
            FileURI = DEFAULT_CONFIG_FILE_URI;
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly MMServicesConfigManager instance = new MMServicesConfigManager();
        }

        /// <summary>
        /// 
        /// </summary>
        public static MMServicesConfigManager Instance
        {
            get { return Nested.instance; }
        }

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public string FileURI
        {
            set { if (loadFile(value)) file_uri = value; }
            get { return file_uri;  }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool Modified
        {
            get { return modified; }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool UniqueDBServer
        {
            set { unique_db_server = value; modified = true; }
            get { return unique_db_server; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string UniqueDBServerName
        {
            set { unique_db_server_name = value; modified = true; }
            get { return unique_db_server_name; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string UniqueDBServerFolder
        {
            set { unique_db_server_folder = value; modified = true; }
            get { return unique_db_server_folder; }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool DatabaseServers
        {
            set { database_servers = value; modified = true; }
            get { return database_servers; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string DBServerWebmateName
        {
            set { db_server_webmate_name = value; modified = true; }
            get { return db_server_webmate_name; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string DBServerWebmateFolder
        {
            set { db_server_webmate_folder = value; modified = true; }
            get { return db_server_webmate_folder; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string DBServerChefmateName
        {
            set { db_server_chefmate_name = value; modified = true; }
            get { return db_server_chefmate_name; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string DBServerChefmateFolder
        {
            set { db_server_chefmate_folder = value; modified = true; }
            get { return db_server_chefmate_folder; }
        }

        #endregion

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public void Reset()
        {
            modified = false;
        }

        /// <summary>
        /// 
        /// </summary>
        public void Reload()
        {
            loadFile(file_uri);
        }

        /// <summary>
        /// 
        /// </summary>
        public void SaveSettings()
        {
            if (file_uri != @"") // XML config was created already
            {
                // Databases
                XmlNode uniqueDBServerNode = getUniqueDBServerNode();
                XmlNode dbServersNode = getDBServersNode();

                XmlNode webmateDBServerNode = dbServersNode.FirstChild;
                XmlNode chefmateDBServerNode = dbServersNode.FirstChild.NextSibling;

                XMLDocManager.SetAttribute(uniqueDBServerNode, @"checked", Convert.ToString(unique_db_server));
                XMLDocManager.SetAttribute(uniqueDBServerNode, @"name", unique_db_server_name);
                XMLDocManager.SetAttribute(uniqueDBServerNode, @"folder", unique_db_server_folder);

                XMLDocManager.SetAttribute(dbServersNode, @"checked", Convert.ToString(database_servers));

                XMLDocManager.SetAttribute(webmateDBServerNode, @"name", db_server_webmate_name);
                XMLDocManager.SetAttribute(webmateDBServerNode, @"folder", db_server_webmate_folder);

                XMLDocManager.SetAttribute(chefmateDBServerNode, @"name", db_server_chefmate_name);
                XMLDocManager.SetAttribute(chefmateDBServerNode, @"folder", db_server_chefmate_folder);

                //::::::::::::::::::::::::::::::

                config_file_doc.Save(file_uri);

                modified = false;
            }
        }

        #endregion

        #region Protected

        string file_uri = @"";
        XmlDocument config_file_doc = new XmlDocument();

        bool modified = false;

        bool unique_db_server = true;
        string unique_db_server_name = DEFAULT_DB_SERVER_NAME;
        string unique_db_server_folder = DEFAULT_DB_FOLDER;

        bool database_servers = false;
        string db_server_webmate_name = DEFAULT_DB_SERVER_NAME;
        string db_server_webmate_folder = DEFAULT_DB_FOLDER;
        string db_server_chefmate_name = DEFAULT_DB_SERVER_NAME;
        string db_server_chefmate_folder = DEFAULT_DB_FOLDER;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileURI"></param>
        /// <returns></returns>
        protected bool loadFile(string inFileURI)
        {
            checkCurrentSettings();

            if( inFileURI.Trim() == @"" )
            {
                inFileURI = DEFAULT_CONFIG_FILE_URI;
            }

            if( !File.Exists( inFileURI ) )
            {
                try
                {
                    ServiceLogger.Log(string.Format(@"Config file not found: {0}. Create a new one", inFileURI));
                    createConfigFile(inFileURI);
                }
                catch
                {
                    return false;
                }
            }

            //..................................................

            XmlTextReader reader = new XmlTextReader(inFileURI );

            try
            {
                reader.Read();
                config_file_doc.Load(reader);
                reader.Close();

                loadSettings();
            }
            catch (Exception)
            {
                reader.Close();

                return false;
            }

            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void checkCurrentSettings()
        {
            if( modified )
            {
                SaveSettings();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void createConfigFile(string inFileURI)
        {
            // Create the XML Declaration, and append it to XML document
            XMLDocManager.CreateXmlDeclaration(config_file_doc, @"1.0", @"utf-8", null);

            //::::::::::::::::::::::::::::::

            // Create the root element
            XmlElement root = XMLDocManager.CreateRoot(config_file_doc, @"MenumateServices");

            //::::::::::::::::::::::::::::::

            // Databases
            XmlNode databasesNode = XMLDocManager.AddNode(config_file_doc, root, @"Databases");
            XmlNode uniqueDBServerNode = XMLDocManager.AddNode(config_file_doc, databasesNode, @"UniqueDBServer");
            XmlNode dbServersNode = XMLDocManager.AddNode(config_file_doc, databasesNode, @"DBServers");

            XmlNode webmateDBServerNode = XMLDocManager.AddNode(config_file_doc, dbServersNode, @"WebmateDBServer");
            XmlNode chefmateDBServerNode = XMLDocManager.AddNode(config_file_doc, dbServersNode, @"ChefmateDBServer");

            XMLDocManager.AddAttribute(config_file_doc, uniqueDBServerNode, @"checked", @"true");
            XMLDocManager.AddAttribute(config_file_doc, uniqueDBServerNode, @"name", DEFAULT_DB_SERVER_NAME);
            XMLDocManager.AddAttribute(config_file_doc, uniqueDBServerNode, @"folder", DEFAULT_DB_FOLDER);

            XMLDocManager.AddAttribute(config_file_doc, dbServersNode, @"checked", @"false");

            XMLDocManager.AddAttribute(config_file_doc, webmateDBServerNode, @"name", DEFAULT_DB_SERVER_NAME);
            XMLDocManager.AddAttribute(config_file_doc, webmateDBServerNode, @"folder", DEFAULT_DB_FOLDER);

            XMLDocManager.AddAttribute(config_file_doc, chefmateDBServerNode, @"name", DEFAULT_DB_SERVER_NAME);
            XMLDocManager.AddAttribute(config_file_doc, chefmateDBServerNode, @"folder", DEFAULT_DB_FOLDER);

            //::::::::::::::::::::::::::::::

            config_file_doc.Save(inFileURI);
        }

        /// <summary>
        /// 
        /// </summary>
        protected void loadSettings()
        {
            unique_db_server = readUniqueDBServer();
            unique_db_server_name = readUniqueDBServerName();
            unique_db_server_folder = readUniqueDBServerFolder();

            database_servers = readDatabaseServers();
            string db_server_webmate_name = readDBServerWebmateName();
            string db_server_webmate_folder = readDBServerWebmateFolder();
            string db_server_chefmate_name = readDBServerChefmateName();
            string db_server_chefmate_folder = readDBServerChefmateFolder();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool readUniqueDBServer()
        {
            bool result = true;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode uniqueDBServerNode = getUniqueDBServerNode();

                result = Convert.ToBoolean(XMLDocManager.GetAttribute(uniqueDBServerNode, @"checked", @"true"));
            }
            catch
            {
                result = true;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string readUniqueDBServerName()
        {
            string result = DEFAULT_DB_SERVER_NAME;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode uniqueDBServerNode = getUniqueDBServerNode();

                result = XMLDocManager.GetAttribute(uniqueDBServerNode, @"name", DEFAULT_DB_SERVER_NAME);
            }
            catch
            {
                result = DEFAULT_DB_SERVER_NAME;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string readUniqueDBServerFolder()
        {
            string result = DEFAULT_DB_FOLDER;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode uniqueDBServerNode = getUniqueDBServerNode();

                result = XMLDocManager.GetAttribute(uniqueDBServerNode, @"folder", DEFAULT_DB_FOLDER);
            }
            catch
            {
                result = DEFAULT_DB_FOLDER;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool readDatabaseServers()
        {
            bool result = false;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode dbServersNode = getDBServersNode();

                result = Convert.ToBoolean(XMLDocManager.GetAttribute(dbServersNode, @"checked", @"false"));
            }
            catch
            {
                result = false;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string readDBServerWebmateName()
        {
            string result = DEFAULT_DB_SERVER_NAME;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode dbServersNode = getDBServersNode();
                XmlNode webmateDBServerNode = dbServersNode.FirstChild;

                result = XMLDocManager.GetAttribute(webmateDBServerNode, @"name", DEFAULT_DB_SERVER_NAME);
            }
            catch
            {
                result = DEFAULT_DB_SERVER_NAME;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string readDBServerWebmateFolder()
        {
            string result = DEFAULT_DB_FOLDER;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode dbServersNode = getDBServersNode();
                XmlNode webmateDBServerNode = dbServersNode.FirstChild;

                result = XMLDocManager.GetAttribute(webmateDBServerNode, @"folder", DEFAULT_DB_FOLDER);
            }
            catch
            {
                result = DEFAULT_DB_FOLDER;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string readDBServerChefmateName()
        {
            string result = DEFAULT_DB_SERVER_NAME;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode dbServersNode = getDBServersNode();
                XmlNode webmateDBServerNode = dbServersNode.FirstChild.NextSibling;

                result = XMLDocManager.GetAttribute(webmateDBServerNode, @"name", DEFAULT_DB_SERVER_NAME);
            }
            catch
            {
                result = DEFAULT_DB_SERVER_NAME;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string readDBServerChefmateFolder()
        {
            string result = DEFAULT_DB_FOLDER;

            //:::::::::::::::::::::::::::::::::

            try
            {
                XmlNode dbServersNode = getDBServersNode();
                XmlNode webmateDBServerNode = dbServersNode.FirstChild.NextSibling;

                result = XMLDocManager.GetAttribute(webmateDBServerNode, @"folder", DEFAULT_DB_FOLDER);
            }
            catch
            {
                result = DEFAULT_DB_FOLDER;
            }

            //:::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private XmlNode getUniqueDBServerNode()
        {
            XmlNode dbNode = getDBNode();
            return dbNode.FirstChild;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private XmlNode getDBServersNode()
        {
            XmlNode dbNode = getDBNode();
            return dbNode.FirstChild.NextSibling;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private XmlNode getDBNode()
        {
            return XMLDocManager.GetNode(config_file_doc, @"Databases");
        }

        #endregion

    }
}
