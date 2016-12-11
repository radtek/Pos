using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;
using FirebirdSql.Data.FirebirdClient;

namespace Chefmate.Database.DbModels
{
    public class DbOrderItemSide
    {
        public static void AddOrderItemSides(FbConnection inConnection, FbTransaction inTransaction, ObservableCollection<Side> sides)
        {
            foreach (var side in sides)
            {
                AddOrderItemSide(inConnection, inTransaction, side);
            }
        }
        public static void AddOrderItemSide(FbConnection inConnection, FbTransaction inTransaction, Side inSide)
        {
            inSide.SideKey = DbSide.GetOrCreateSide(inConnection, inTransaction, inSide);
            inSide.OrderSideKey = DatabaseCore.Instance.GetGeneratorValue("GEN_ORDERITEMSIDE");
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("ORDERITEMSIDE_KEY", inSide.OrderSideKey));
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", inSide.Item.OrderItemKey));
            fbParameters.Add(new QueryParameter("SIDE_KEY", inSide.SideKey));
            fbParameters.Add(new QueryParameter("ORDERITEMSIDE_POS_KEY", inSide.SidePosKey));
            fbParameters.Add(new QueryParameter("IS_CANCELED", inSide.IsCanceled));
            var queryString = DatabaseCore.Instance.BuildInsertQuery("ORDERITEMSIDES", fbParameters);
            DatabaseCore.Instance.ExecuteNonQuery(inConnection, inTransaction, queryString, fbParameters);
        }
        public static ObservableCollection<Side> GetOrderItemSides(Item item)
        {
            var queryString = @"SELECT * FROM ORDERITEMSIDES A
                                INNER JOIN SIDES B
                                ON A.SIDE_KEY = B.SIDE_KEY
                                WHERE A.ORDERITEM_KEY = @ORDERITEM_KEY";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("ORDERITEM_KEY", item.OrderItemKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            return ExtractSidesInformation(resultSet, item);
        }
        private static ObservableCollection<Side> ExtractSidesInformation(DataTable resultSet, Item item)
        {
            var sides = new ObservableCollection<Side>();
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                var side = ExtractSideInformation(resultSet.Rows[i]);
                side.Item = item;
                sides.Add(side);
            }
            return sides;
        }
        private static Side ExtractSideInformation(DataRow resultSet)
        {
            var side = new Side();
            side.SideKey = Convert.ToInt32(resultSet["SIDE_KEY"]);
            side.SidePosKey = Convert.ToInt32(resultSet["ORDERITEMSIDE_POS_KEY"]);
            side.OrderSideKey = Convert.ToInt32(resultSet["ORDERITEMSIDE_KEY"]);
            side.Name = Convert.ToString(resultSet["SIDE_NAME"]);
            side.IsCanceled = Convert.ToBoolean(resultSet["IS_CANCELED"]);
            side.OrderItemKey = Convert.ToInt32(resultSet["ORDERITEM_KEY"]);
            return side;
        }

    }
}
