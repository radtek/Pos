using System.Collections.Generic;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;
using FirebirdSql.Data.FirebirdClient;

namespace Chefmate.Database.DbModels
{
    public class DbOption
    {
        public static int GetOrCreateOption(FbConnection inConnection, FbTransaction inTransaction, Option inOption)
        {
            string query = "SELECT OPTION_KEY FROM OPTIONS WHERE OPTION_NAME = @OPTION_NAME ";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("OPTION_NAME", inOption.Name));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(inConnection, inTransaction, query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddOption(inConnection, inTransaction, inOption);
            }
        }

        private static int AddOption(FbConnection inConnection, FbTransaction inTransaction, Option inOption)
        {
            inOption.OptionKey = DatabaseCore.Instance.GetGeneratorValue("GEN_OPTION");
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("OPTION_NAME", inOption.Name));
            queryParameters.Add(new QueryParameter("OPTION_KEY", inOption.OptionKey));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("OPTIONS", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(inConnection, inTransaction, commandText, queryParameters);
            return inOption.OptionKey;
        }
    }
}
