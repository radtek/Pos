using System.Collections.Generic;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;
using FirebirdSql.Data.FirebirdClient;

namespace Chefmate.Database.DbModels
{
    public class DbGroup
    {
        public static int GetOrCreateGroup(Group inGroup)
        {
            string query = "SELECT GROUP_KEY FROM GROUPS WHERE GROUP_NAME = @GROUP_NAME AND GROUP_POS_KEY = @GROUP_POS_KEY AND GROUP_TYPE=@GROUP_TYPE";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("GROUP_NAME", inGroup.GroupName));
            queryParameters.Add(new QueryParameter("GROUP_POS_KEY", inGroup.GroupPosKey));
            queryParameters.Add(new QueryParameter("GROUP_TYPE", inGroup.GroupType));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddGroup(inGroup);
            }
        }
        private static int AddGroup(Group inGroup)
        {
            inGroup.GroupKey = DatabaseCore.Instance.GetGeneratorValue("GEN_GROUPS");
            //Write query to insert in Groups table
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("GROUP_KEY", inGroup.GroupKey));
            queryParameters.Add(new QueryParameter("GROUP_NAME", inGroup.GroupName));
            queryParameters.Add(new QueryParameter("GROUP_POS_KEY", inGroup.GroupPosKey));
            queryParameters.Add(new QueryParameter("GROUP_TYPE", inGroup.GroupType));
            queryParameters.Add(new QueryParameter("GROUP_DISPLAY_ORDER", inGroup.DisplayOrder));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("GROUPS", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, queryParameters);
            return inGroup.GroupKey;
        }
        public static int GetOrCreateGroup(FbConnection inConnection, FbTransaction inTransaction, Group inGroup)
        {
            string query = "SELECT GROUP_KEY FROM GROUPS WHERE GROUP_NAME = @GROUP_NAME AND GROUP_POS_KEY = @GROUP_POS_KEY AND GROUP_TYPE=@GROUP_TYPE";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("GROUP_NAME", inGroup.GroupName));
            queryParameters.Add(new QueryParameter("GROUP_POS_KEY", inGroup.GroupPosKey));
            queryParameters.Add(new QueryParameter("GROUP_TYPE", inGroup.GroupType));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(inConnection, inTransaction, query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddGroup(inConnection, inTransaction, inGroup);
            }
        }
        private static int AddGroup(FbConnection inConnection, FbTransaction inTransaction, Group inGroup)
        {
            inGroup.GroupKey = DatabaseCore.Instance.GetGeneratorValue("GEN_GROUPS");
            //Write query to insert in Groups table
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("GROUP_KEY", inGroup.GroupKey));
            queryParameters.Add(new QueryParameter("GROUP_NAME", inGroup.GroupName));
            queryParameters.Add(new QueryParameter("GROUP_POS_KEY", inGroup.GroupPosKey));
            queryParameters.Add(new QueryParameter("GROUP_TYPE", inGroup.GroupType));
            queryParameters.Add(new QueryParameter("GROUP_DISPLAY_ORDER", inGroup.DisplayOrder));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("GROUPS", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(inConnection, inTransaction, commandText, queryParameters);
            return inGroup.GroupKey;
        }

    }
}
