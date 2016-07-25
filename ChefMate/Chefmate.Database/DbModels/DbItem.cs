using System.Collections.Generic;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbItem
    {
        public static int GetOrCreateItem(Item item)
        {
            string query = "SELECT ITEM_KEY FROM ITEMS WHERE ITEM_POS_KEY = @ITEM_POS_KEY";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("ITEM_POS_KEY", item.ItemPosKey));
            var resultSet = DatabaseCore.Instance.ExecuteScalar<int>(query, queryParameters);
            if (resultSet != 0)
            {
                return resultSet;
            }
            else
            {
                return AddItem(item);
            }
        }
        private static int AddItem(Item item)
        {
            item.ItemKey = DatabaseCore.Instance.GetGeneratorValue("GEN_ITEM");
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("ITEM_KEY", item.ItemKey));
            queryParameters.Add(new QueryParameter("ITEM_NAME", item.Name));
            queryParameters.Add(new QueryParameter("ITEM_POS_KEY", item.ItemPosKey));
            string commandText = DatabaseCore.Instance.BuildInsertQuery("ITEMS", queryParameters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, queryParameters);
            return item.ItemKey;
        }
    }
}
