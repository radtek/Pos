using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using Castle.ActiveRecord;
using Castle.ActiveRecord.Framework.Config;

using Menumate.WinServices.Implementation.Contracts.BusinessObjects;
using NLog;


namespace Menumate.WinServices.Implementation.Contracts
{
    public static class ReservationsServiceInitHelper
    {

        private static Logger Log = LogManager.GetLogger("InitHelper");

        private static bool _hasInitialised = false;
        private static object _lockingObject = new object();

        /// <summary>
        /// Handle the initialisation of ActiveRecord data layer. This must only be done once in the
        /// application lifecycle so this class maintains a count to enforce this.
        /// </summary>
        public static void InitDB()
        {
            lock (_lockingObject)
            {
                if (!_hasInitialised)
                {
                    try
                    {
                        string data_source = ReservationsServiceInitHelper.BuildReservationsDataSourceString();
                        Log.Info(string.Format(" ... using => {0} as database", data_source));
                        Log.Info(string.Format(" ... initializing database connection"));

                        IDictionary<string, string> properties = new Dictionary<string, string>();
                        properties["connection.driver_class"] = "NHibernate.Driver.FirebirdClientDriver";
                        properties["dialect"] = "NHibernate.Dialect.FirebirdDialect";
                        properties["connection.provider"] = "NHibernate.Connection.DriverConnectionProvider";
                        properties["connection.connection_string"] = data_source;
                        //#if DEBUG
                        //                        properties["show_sql"] = "true";
                        //#endif
                        properties["proxyfactory.factory_class"] =
                            "NHibernate.ByteCode.Castle.ProxyFactoryFactory, NHibernate.ByteCode.Castle";

                        InPlaceConfigurationSource source = new InPlaceConfigurationSource();
                        source.Add(typeof(ActiveRecordBase), properties);
                        ActiveRecordStarter.Initialize(source,
                            typeof(Location),
                            typeof(Session),
                            typeof(Table),
                            typeof(Guest),
                            typeof(Reservation),
                            typeof(Terminal)
                        );

                        Log.Info(string.Format("... database initialised ok ***"));


                    }
                    catch (SystemException e)
                    {
                        Log.FatalException("*** database did NOT initialise ***", e);
                        throw;
                    }
                    _hasInitialised = true;
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private static string BuildReservationsDataSourceString()
        {
            // use the current working directory for the location of the reservations database.
            //string db_path =
            //Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase), "reservations.fdb");

            // TODO: make this a configurable setting
            string db_path =
                Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase), "RESERVATIONS.fdb");
            //Path.Combine(@"file:\c:\", "RESERVATIONS.fdb");

            StringBuilder data_source = new StringBuilder(db_path);
            data_source.Remove(0, @"file:\".Length);
            data_source.Insert(0, "Server=localhost;Database=");
            data_source.Append(";User=SYSDBA;password=masterkey;ServerType=0;Pooling=true");
            return data_source.ToString();
        }
    }
}
