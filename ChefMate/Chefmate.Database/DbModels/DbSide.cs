using System.Collections.Generic;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbSide
    {
        public static int GetOrCreateSide(Side side)
        {
            string query = "SELECT SIDE_KEY FROM SIDES WHERE SIDE_NAME=@SIDE_NAME";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("SIDE_NAME", side.Name));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddSide(side);
            }
        }

        public static int AddSide(Side side)
        {
            side.SideKey = DatabaseCore.Instance.GetGeneratorValue("GEN_SIDE");
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("SIDE_KEY", side.SideKey));
            queryParameters.Add(new QueryParameter("SIDE_NAME", side.Name));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("SIDES", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, queryParameters);
            return side.SideKey;
        }
    }

}
