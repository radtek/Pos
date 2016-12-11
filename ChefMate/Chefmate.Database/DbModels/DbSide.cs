using System.Collections.Generic;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;
using FirebirdSql.Data.FirebirdClient;

namespace Chefmate.Database.DbModels
{
    public class DbSide
    {
        public static int GetOrCreateSide(FbConnection inConnection, FbTransaction inTransaction, Side side)
        {
            string query = "SELECT SIDE_KEY FROM SIDES WHERE SIDE_NAME=@SIDE_NAME";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("SIDE_NAME", side.Name));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(inConnection, inTransaction, query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddSide(inConnection, inTransaction, side);
            }
        }

        public static int AddSide(FbConnection inConnection, FbTransaction inTransaction, Side side)
        {
            side.SideKey = DatabaseCore.Instance.GetGeneratorValue("GEN_SIDE");
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("SIDE_KEY", side.SideKey));
            queryParameters.Add(new QueryParameter("SIDE_NAME", side.Name));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("SIDES", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(inConnection, inTransaction, commandText, queryParameters);
            return side.SideKey;
        }
    }

}
