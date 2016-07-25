using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using System.IO;

namespace Loyalty_mate.Utilities.DB
{
    /// <summary>
    /// Singleton Class: Creates Menumate Database connections
    /// </summary>
    public class LoyaltyMateDBConnection
    {
        private static Properties.Settings settings = Properties.Settings.Default;

        string username_;
        string password_;
        string databaseURI_;
        string dataSource_;

        /// <summary>
        /// 
        /// </summary>
        internal LoyaltyMateDBConnection()
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

            internal static readonly LoyaltyMateDBConnection instance = new LoyaltyMateDBConnection();
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public static LoyaltyMateDBConnection Instance
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
            inDatabaseURI = Path.Combine(settings.DatabaseLocation, settings.DatabaseName);
            inDataSource = settings.DatabaseIPAddress;
        }

        #endregion
    }
}
