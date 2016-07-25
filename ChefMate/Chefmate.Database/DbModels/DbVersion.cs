using System;
using System.Collections.Generic;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbVersion
    {
        public static string GetDatabaseVersion()
        {
            var query = "SELECT VERSION_NUMBER FROM VERSIONS WHERE  VERSION_KEY = (SELECT MAX(VERSION_KEY) FROM VERSIONS)";
            var retVal = DatabaseCore.Instance.ExecuteScalar<string>(query);
            return retVal;
        }

        public static bool CheckVersionExist(string versionNumber)
        {
            var query = "SELECT COUNT(*) FROM VERSIONS WHERE  VERSION_NUMBER = @VERSION_NUMBER";
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("VERSION_NUMBER", versionNumber));
            var retVal = DatabaseCore.Instance.ExecuteScalar<int>(query, parameters);
            return retVal == 1;
        }

        public static bool SaveVersion(string versionNumber)
        {
            int versionKey = DatabaseCore.Instance.GetGeneratorValue("GEN_VERSION");
            var fbParameters = new List<QueryParameter>
            {
                new QueryParameter("VERSION_KEY", versionKey),
                new QueryParameter("VERSION_TIME_STAMP", DateTime.Now),
                new QueryParameter("VERSION_NUMBER", versionNumber)
            };
            var queryString = DatabaseCore.Instance.BuildInsertQuery("VERSIONS", fbParameters);
            var result = DatabaseCore.Instance.ExecuteNonQuery(queryString, fbParameters);
            return result;
        }

    }
}
