using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using FirebirdSql.Data.FirebirdClient;

namespace MenumateServices.Internal_Classes.OnlineOrdering
{
    /// <summary>
    /// Singleton Class: Creates Menumate Database connections
    /// </summary>
    public class OnlineOrderDBConnection
    {
        private static Properties.Settings settings_ = Properties.Settings.Default;

        string username_;
        string password_;
        string databaseURI_;
        string dataSource_;

        /// <summary>
        /// 
        /// </summary>
        internal OnlineOrderDBConnection()
        {
            resetDBSettings(ref username_, ref password_, ref databaseURI_, ref dataSource_);
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

            internal static readonly OnlineOrderDBConnection instance = new OnlineOrderDBConnection();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static OnlineOrderDBConnection Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public FbConnection Open()
        {
            FbConnection result = makeFbConnection(username_, password_, databaseURI_, dataSource_);

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            result.Open();

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }
        #endregion

        #region Private
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inUsername"></param>
        /// <param name="inPassword"></param>
        /// <param name="inDatabaseURI"></param>
        /// <param name="inDataSource"></param>
        /// <returns></returns>
        internal static FbConnection makeFbConnection(
                                                    string inUsername,
                                                    string inPassword,
                                                    string inDatabaseURI,
                                                    string inDataSource)
        {
            string connectionString = "";

            connectionString += "User=" + inUsername + "; ";
            connectionString += "Password=" + inPassword + "; ";
            connectionString += "Database=" + inDatabaseURI + "; ";
            connectionString += "Datasource=" + inDataSource + "; ";

            connectionString += "Charset=NONE;";
            connectionString += "Connection lifetime=15;";
            connectionString += "Packet Size=8192;";
            connectionString += "Pooling=false";
            
            return new FbConnection(connectionString);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inUsername"></param>
        /// <param name="inPassword"></param>
        /// <param name="inDatabaseURI"></param>
        /// <param name="inDataSource"></param>
        internal static void resetDBSettings(
                                      ref string inUsername,
                                      ref string inPassword,
                                      ref string inDatabaseURI,
                                      ref string inDataSource)
        {
            inUsername = "SYSDBA";
            inPassword = "masterkey";

            ServiceInfo serviceInfo = ServiceInfo.Instance;

            inDatabaseURI = Path.Combine(serviceInfo.WebmateDatabaseLocation(), settings_.DatabaseName);
            inDataSource = serviceInfo.WebmateDatabaseServerPath();

            //inDatabaseURI = Path.Combine(settings_.DatabaseLocation, settings_.DatabaseName);
            //inDataSource = settings_.DatabaseIPAddress;
        }

        #endregion
    }
}

