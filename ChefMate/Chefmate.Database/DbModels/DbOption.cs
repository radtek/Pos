using System.Collections.Generic;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbOption
    {
        public static int GetOrCreateOption(Option inOption)
        {
            string query = "SELECT OPTION_KEY FROM OPTIONS WHERE OPTION_NAME = @OPTION_NAME ";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("OPTION_NAME", inOption.Name));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddOption(inOption);
            }
        }

        private static int AddOption(Option inOption)
        {
            inOption.OptionKey = DatabaseCore.Instance.GetGeneratorValue("GEN_OPTION");
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("OPTION_NAME", inOption.Name));
            queryParameters.Add(new QueryParameter("OPTION_KEY", inOption.OptionKey));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("OPTIONS", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, queryParameters);
            return inOption.OptionKey;
        }
    }
}
