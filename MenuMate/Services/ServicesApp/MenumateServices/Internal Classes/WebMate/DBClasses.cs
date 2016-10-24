using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using FirebirdSql.Data.FirebirdClient;

using MenumateServices.WebMate.DTO;

/// <summary>
/// Classess in this file:
///     WebOrderDBValidate:   Singleton Class: Validates orders in the database by checking out the Order GUID
///     WebOrderDBConnection: Singleton Class: Creates Firebird Database connections
///     WebOrderDBCommit:     Singleton Class: Saves orders in the database
///     WebOrderDB:           Provides DB access.
/// </summary>

namespace MenumateServices.WebMate.InternalClasses.Database
{
    enum DBTimeOuts { Command = 10 };

    /// <summary>
    /// Singleton Class: Validates orders in the database by checking out the Order GUID
    /// </summary>
    public class WebOrderDBValidate : WebOrderDB
    {
        /// <summary>
        /// 
        /// </summary>
        internal WebOrderDBValidate() : base()
        {
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

            internal static readonly WebOrderDBValidate instance = new WebOrderDBValidate();
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public static WebOrderDBValidate Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        public void ValidateOrder(WebOrder inOrder)
        {
            if (orderGUIDExists(inOrder.HeaderSection.GUID))
            {
                throw new ExceptionOrderGUIDDuplicate(inOrder.HeaderSection.GUID);
            }
        }
        #endregion

        #region Private
        /// <summary>
        /// NOT USED SO FAR
        /// </summary>
        /// <param name="inMemberNumber"></param>
        /// <returns></returns>
        bool memberExists(string inMemberNumber)
        {
            return ExecuteBoolQuery(string.Format("SELECT * FROM Contacts WHERE member_Number = %@",  inMemberNumber));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderGUID"></param>
        /// <returns></returns>
        bool orderGUIDExists(string inOrderGUID)
        {
            return ExecuteBoolQuery(string.Format("SELECT * FROM WebOrders WHERE guid = %@", inOrderGUID));
        }
        #endregion
    }

    /// <summary>
    /// Singleton Class: Saves orders in the database
    /// </summary>
    public class WebOrderDBCommit : WebOrderDB
    {
        /// <summary>
        /// 
        /// </summary>
        internal WebOrderDBCommit() : base()
        {
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

            internal static readonly WebOrderDBCommit instance = new WebOrderDBCommit();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static WebOrderDBCommit Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        public void CommitOrder(WebOrder inOrder)
        {
            UInt32 webKey = saveToWebOrderTable(inOrder);
        }
        #endregion

        #region Private
        private UInt32 saveToWebOrderTable(WebOrder inOrder)
        {
            return 1;
        }
        #endregion
    }

    /// <summary>
    /// Provides DB access
    /// </summary>
    public class WebOrderDB 
    {
        // Singleton object: Povides with connections to the Menumate's dabase
        WebOrderDBConnection dbConnection_ = WebOrderDBConnection.Instance;

        /// <summary>
        /// 
        /// </summary>
        public WebOrderDB()
        {
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSQL"></param>
        /// <returns></returns>
        protected bool ExecuteBoolQuery(string inSQL)
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            using (FbConnection connection = dbConnection_.Open())
            {
                FbTransaction transaction = connection.BeginTransaction();

                FbCommand command = new FbCommand(inSQL, connection, transaction);
                result = executeBoolCommand(command);

                transaction.Commit();
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }
        #endregion

        #region Private
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCommand"></param>
        /// <returns></returns>
        bool executeBoolCommand(FbCommand inCommand)
        {
            bool result = false;

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            using(FbDataReader reader = inCommand.ExecuteReader())
            {
                result = reader.HasRows;
            }

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }
        #endregion
    }

    /// <summary>
    /// Singleton Class: Creates Menumate Database connections
    /// </summary>
    public class WebOrderDBConnection
    {
        private static Properties.Settings settings = Properties.Settings.Default;

        string username_;
        string password_ ;
        string databaseURI_ ;
        string dataSource_;

        /// <summary>
        /// 
        /// </summary>
        internal WebOrderDBConnection()
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

            internal static readonly WebOrderDBConnection instance = new WebOrderDBConnection();
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public static WebOrderDBConnection Instance
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
            inUsername     = "SYSDBA";
            inPassword      = "masterkey";

            ServiceInfo serviceInfo = ServiceInfo.Instance;

            inDatabaseURI = Path.Combine(serviceInfo.WebmateDatabaseLocation(), settings.DatabaseName);
            inDataSource = serviceInfo.WebmateDatabaseServerPath();
        }

        #endregion
    }
}
