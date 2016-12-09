using System;
using Chefmate.Database.DbModels;
using ChefMate.Database;
using Chefmate.Core.Enums;
using Chefmate.Logger;

namespace Chefmate.Infrastructure.Utility
{
    public class VersionChecker
    {
        private static VersionChecker _instance;
        private static volatile object _syncRoot = new Object();

        private VersionChecker()
        { }

        public static VersionChecker Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new VersionChecker();
                    }
                }
                return _instance;
            }
        }

        public DatabaseMode CheckVersionInformation(string currentVersion, ref string messageString)
        {
            var versionMode = DatabaseMode.Correct;
            try
            {
                string databaseVersion = DbVersion.GetDatabaseVersion();
                if (databaseVersion != currentVersion)
                {
                    versionMode = DatabaseMode.InCorrect;
                    messageString = "Incorrect database version please run the parser. \n Current database version: " +
                                    databaseVersion + "  Expected database version: " + currentVersion;
                }
            }
            catch(Exception ex)
            {
                versionMode = DatabaseMode.UnVersion;
                messageString = "Database unversioned please run the parser";
                ChefmateLogger.Instance.LogError("CheckVersionInformation : ", ex.Message);
            }
            return versionMode;
        }


    }
}