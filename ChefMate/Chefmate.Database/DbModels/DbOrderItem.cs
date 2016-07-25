using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbOrderItem
    {
        public static void AddOrderItems(ObservableCollection<Item> items, int terminalKey)
        {
            foreach (var item in items)
            {
                AddOrderItem(item, terminalKey);
            }
        }
        public static void AddOrderItem(Item item, int terminalKey)
        {
            item.ItemKey = DbItem.GetOrCreateItem(item);
            item.OrderItemKey = DatabaseCore.Instance.GetGeneratorValue("GEN_ORDERITEM");
            var fbParameters = new List<QueryParameter>(); 
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", item.OrderItemKey));
            fbParameters.Add(new QueryParameter("ITEM_KEY", item.ItemKey));
            fbParameters.Add(new QueryParameter("COURSE_KEY", item.CourseGroup.OrderGroupKey));
            fbParameters.Add(new QueryParameter("SERVINGCOURSE_KEY", item.SCourseGroup.OrderGroupKey));
            fbParameters.Add(new QueryParameter("ORDERITEM_POS_KEY", item.OrderItemPosKey));
            fbParameters.Add(new QueryParameter("TERMINAL_KEY", terminalKey));
            fbParameters.Add(new QueryParameter("ORDER_ITEM_STATUS", item.OrderStatus));
            fbParameters.Add(new QueryParameter("ORDERITEM_NOTE", item.Note));
            fbParameters.Add(new QueryParameter("ARRIVAL_TIME", item.ArrivalTime));
            var queryString = DatabaseCore.Instance.BuildInsertQuery("ORDERITEMS", fbParameters);
            DatabaseCore.Instance.ExecuteNonQuery(queryString, fbParameters);
            DbOrderItemSide.AddOrderItemSides(item.Sides);
            DbOrderItemOption.AddOrderItemOptions(item.Options);
        }
        public static ObservableCollection<Item> GetOrderItems(List<int> groupKeys, GroupType inGroupType, int terminalKey)
        {
            var inClouse = inGroupType == GroupType.ServingCourse ? " AND A.SERVINGCOURSE_KEY in (" : " AND A.COURSE_KEY in (";
            foreach (var groupKey in groupKeys)
            {
                inClouse += groupKey + ",";
            }
            inClouse = inClouse.Substring(0, inClouse.Length - 1);

            var queryString = @"SELECT * FROM ORDERITEMS A
                                INNER JOIN ITEMS B
                                ON A.ITEM_KEY = B.ITEM_KEY
                                WHERE A.TERMINAL_KEY = @TERMINAL_KEY AND ORDER_ITEM_STATUS <> @ORDER_ITEM_STATUS";
            queryString += inClouse + ")";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("TERMINAL_KEY", terminalKey));
            queryParameters.Add(new QueryParameter("ORDER_ITEM_STATUS", OrderStatus.Bumped));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            var items = ExtractItemsInformation(resultSet);
            foreach (var item in items)
            {
                item.Sides = DbOrderItemSide.GetOrderItemSides(item);
                item.Options = DbOrderItemOption.GetOrderItemOptions(item);
            }
            return items;
        }
        public static ObservableCollection<Item> GetBumpedOrderItems(List<int> groupKeys, GroupType inGroupType, int terminalKey)
        {
            var inClouse = inGroupType == GroupType.ServingCourse ? " AND A.SERVINGCOURSE_KEY in (" : " AND A.COURSE_KEY in (";
            foreach (var groupKey in groupKeys)
            {
                inClouse += groupKey + ",";
            }
            inClouse = inClouse.Substring(0, inClouse.Length - 1);

            var queryString = @"SELECT * FROM ORDERITEMS A
                                INNER JOIN ITEMS B
                                ON A.ITEM_KEY = B.ITEM_KEY
                                WHERE A.TERMINAL_KEY = @TERMINAL_KEY ";
            queryString += inClouse + ")";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("TERMINAL_KEY", terminalKey));
            queryParameters.Add(new QueryParameter("ORDER_ITEM_STATUS", OrderStatus.Bumped));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            var items = ExtractItemsInformation(resultSet);
            foreach (var item in items)
            {
                item.Sides = DbOrderItemSide.GetOrderItemSides(item);
                item.Options = DbOrderItemOption.GetOrderItemOptions(item);
            }
            return items;
        }
        private static ObservableCollection<Item> ExtractItemsInformation(DataTable resultSet)
        {
            var items = new ObservableCollection<Item>();
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                var item = ExtractItemInformation(resultSet.Rows[i]);
                items.Add(item);
            }
            return items;
        }
        private static Item ExtractItemInformation(DataRow resultSet)
        {
            var item = new Item();
            item.ItemKey = Convert.ToInt32(resultSet["ITEM_KEY"]);
            item.ItemPosKey = Convert.ToInt32(resultSet["ITEM_POS_KEY"]);
            item.OrderItemKey = Convert.ToInt32(resultSet["ORDERITEM_KEY"]);
            item.OrderItemPosKey = Convert.ToInt32(resultSet["ORDERITEM_POS_KEY"]);
            item.Name = Convert.ToString(resultSet["ITEM_NAME"]);
            item.Note = Convert.ToString(resultSet["ORDERITEM_NOTE"]);
            item.OrderStatus = (OrderStatus)Convert.ToInt32(resultSet["ORDER_ITEM_STATUS"]);
            item.ServingCourseKey = Convert.ToInt32(resultSet["SERVINGCOURSE_KEY"]);
            item.CourseKey = Convert.ToInt32(resultSet["COURSE_KEY"]);
            return item;
        }
        public static void UpdateOrderItemStatus(int orderItemKey, OrderStatus itemStatus)
        {
            var whereCondition = " Where ORDERITEM_KEY = @ORDERITEM_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("ORDER_ITEM_STATUS", itemStatus));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERITEMS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", orderItemKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
        public static void UpdateOrderItemTerminalKey(int orderItemKey, int terminalId)
        {
            var whereCondition = " Where ORDERITEM_KEY = @ORDERITEM_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("TERMINAL_KEY", terminalId));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERITEMS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", orderItemKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
        public static void UpdateItemOnTransfer(int orderItemKey, int terminalId, OrderStatus itemStatus)
        {
            var whereCondition = " Where ORDERITEM_KEY = @ORDERITEM_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("TERMINAL_KEY", terminalId));
            fbParameters.Add(new QueryParameter("ORDER_ITEM_STATUS", itemStatus));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERITEMS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", orderItemKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
        public static void ItemBumped(int orderItemKey, DateTime dateTime)
        {
            var whereCondition = " Where ORDERITEM_KEY = @ORDERITEM_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("BUMP_TIME", dateTime));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERITEMS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", orderItemKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
        public static void UpdateGroupInformation(Item item)
        {
            if (item.SCourseGroup.OrderGroupKey == 0)
            {
                DbOrderGroup.AddOrderGroup(item.SCourseGroup);
            }
            if (item.CourseGroup.OrderGroupKey == 0)
            {
                DbOrderGroup.AddOrderGroup(item.CourseGroup);
            }
            var whereCondition = " Where ORDERITEM_KEY = @ORDERITEM_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("COURSE_KEY", item.CourseGroup.OrderGroupKey));
            fbParameters.Add(new QueryParameter("SERVINGCOURSE_KEY", item.SCourseGroup.OrderGroupKey));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERITEMS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", item.OrderItemKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
        public static Item GetOrderItemFromItemKey(int inOrderIemKey)
        {
            var queryString = @"SELECT * FROM ORDERITEMS A
                                INNER JOIN ITEMS B
                                ON A.ITEM_KEY = B.ITEM_KEY
                                WHERE A.ORDERITEM_KEY = @ORDERITEM_KEY";
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("ORDERITEM_KEY", inOrderIemKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, parameters);
            var item = ExtractItemInformation(resultSet.Rows[0]);
            item.Sides = DbOrderItemSide.GetOrderItemSides(item);
            item.Options = DbOrderItemOption.GetOrderItemOptions(item);
            return item;
        }
        public static void UpdateOrderItemNote(int orderItemKey, string note)
        {
            var whereCondition = " Where ORDERITEM_KEY = @ORDERITEM_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("ORDERITEM_NOTE", note));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERITEMS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", orderItemKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
    }
}
