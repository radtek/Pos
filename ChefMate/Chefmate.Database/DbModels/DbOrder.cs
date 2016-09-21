using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbOrder
    {
        public static void AddOrder(Order order, int terminalKey, bool addItems = true)
        {
            order.OrderKey = DatabaseCore.Instance.GetGeneratorValue("GEN_ORDERS");
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("ORDER_KEY", order.OrderKey));
            fbParameters.Add(new QueryParameter("ORDER_NUMBER", order.OrderNumber));
            fbParameters.Add(new QueryParameter("ORDER_POS_KEY", order.OrderPosKey));
            fbParameters.Add(new QueryParameter("PATRON_COUNT", order.PatronCount));
            fbParameters.Add(new QueryParameter("CHIT_VALUE", order.ChitValue));
            fbParameters.Add(new QueryParameter("TABLE_TAB_NAME", order.TableTabName));
            fbParameters.Add(new QueryParameter("SOURCE_TABLE_TAB_NAME", order.SourceTableName));
            fbParameters.Add(new QueryParameter("PARTY_NAME", order.PartyName));
            fbParameters.Add(new QueryParameter("SERVER_NAME", order.ServerName));
            fbParameters.Add(new QueryParameter("CUSTOMER_NAME", order.CustomerName));
            fbParameters.Add(new QueryParameter("ORDER_TYPE", order.OrderType));
            fbParameters.Add(new QueryParameter("ORDER_STATUS", order.OrderStatus));
            fbParameters.Add(new QueryParameter("ORDER_SALE_START_TIME", order.SaleStartTime));
            fbParameters.Add(new QueryParameter("ORDER_SALE_FINISH_TIME", order.SaleFinishTime));
            fbParameters.Add(new QueryParameter("DELIVERY_TIME", order.DeliveryTime));
            fbParameters.Add(new QueryParameter("ARRIVAL_TIME", order.ArrivalTime));
            fbParameters.Add(new QueryParameter("IS_BUMPED", false));
            fbParameters.Add(new QueryParameter("CUSTOMER_PHONE", order.CustomerPhone));
            fbParameters.Add(new QueryParameter("CUSTOMER_EMAIL", order.CustomerEmail));
            fbParameters.Add(new QueryParameter("CUSTOMER_ADDRESS", order.CustomerAddress));
            fbParameters.Add(new QueryParameter("PAYMENT_STATUS", order.PaymentStatus));
            fbParameters.Add(new QueryParameter("ORDER_ACTION", order.OrderAction));
            var queryString = DatabaseCore.Instance.BuildInsertQuery("ORDERS", fbParameters);
            bool result = DatabaseCore.Instance.ExecuteNonQuery(queryString, fbParameters);
            if (result)
            {
                //Add Order Groups
                DbOrderGroup.AddOrderGroups(order.ServingCourseGroups);
                DbOrderGroup.AddOrderGroups(order.CourseGroups);
                if (addItems)
                    DbOrderItem.AddOrderItems(order.Items, terminalKey, order.OrderKey);
            }

        }
        public static ObservableCollection<Order> GetAllOrders(int terminalKey)
        {
            var totalOrders = new ObservableCollection<Order>();
            var orderKeys = GetOrderKeys(terminalKey);
            foreach (var orderKey in orderKeys)
            {
                if (orderKey > 0)
                    totalOrders.Add(GetOrder(orderKey, terminalKey));
            }
            return totalOrders;
        }
        public static ObservableCollection<Order> GetBumpedOrders(int skipCount, int orderCount, ref bool canLoadMore)
        {
            var totalOrders = new ObservableCollection<Order>();
            var orderKeys = GetBumpedOrderKeys();
            int endIndex = 0;
            if (orderKeys.Count > (skipCount + orderCount))
            {
                endIndex = skipCount + orderCount;
                canLoadMore = true;
            }
            else
            {
                endIndex = orderKeys.Count;
                canLoadMore = false;
            }

            for (int i = skipCount; i < endIndex; i++)
            {
                if (orderKeys[i] > 0)
                    totalOrders.Add(GetOrder(orderKeys[i]));
            }
            return totalOrders;
        }
        public static Order GetOrder(int inOrderKey, int terminalKey, bool isBumped = false)
        {
            var order = GetOrder(inOrderKey);
            if (order != null)
            {
                order.ServingCourseGroups = DbOrderGroup.GetOrderGroups(order, GroupType.ServingCourse);
                order.CourseGroups = DbOrderGroup.GetOrderGroups(order, GroupType.Course);
                var items = GetItemsByGroups(order.ServingCourseGroups, GroupType.ServingCourse, terminalKey, isBumped);
                FilterItems(order, items);
            }
            return order;
        }
        public static Order GetOrderFromTablename(string tableName)
        {
            var whereCondition = " WHERE TABLE_TAB_NAME=@TABLE_TAB_NAME";
            var queryString = DatabaseCore.Instance.BuildSelectAllQuery("ORDERS", whereCondition);
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("TABLE_TAB_NAME", tableName));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, parameters);
            if (resultSet.Rows.Count > 0)
                return ExtractOrderInformation(resultSet.Rows[0]);
            return null;
        }
        public static void UpdateOrderTableName(int orderKey, string tableName)
        {
            var whereCondition = " WHERE ORDER_KEY=@ORDER_KEY";
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("TABLE_TAB_NAME", tableName));
            var queryString = DatabaseCore.Instance.BuildUpdateQuery("ORDERS", parameters, whereCondition);
            parameters.Add(new QueryParameter("ORDER_KEY", orderKey));
            DatabaseCore.Instance.ExecuteNonQuery(queryString, parameters);
        }
        public static Order GetOrderFromOrderItemKey(int inOrderIemKey, int terminalKey)
        {
            var item = DbOrderItem.GetOrderItemFromItemKey(inOrderIemKey);
            var sCoursegroup = DbOrderGroup.GetOrderGroup(item.ServingCourseKey);
            var courseGroup = DbOrderGroup.GetOrderGroup(item.CourseKey);
            var order = GetOrder(sCoursegroup.OrderKey);
            order.ServingCourseGroups.Add(sCoursegroup);
            order.CourseGroups.Add(courseGroup);
            FilterItem(order, item);
            return order;
        }
        public static Order GetOrderFromOrderGroupKey(int inOrderGroupKey, int terminalKey)
        {
            var group = DbOrderGroup.GetOrderGroup(inOrderGroupKey);
            var order = GetOrder(group.OrderKey);
            group.Order = order;
            if (group.GroupType == GroupType.ServingCourse)
            {
                order.ServingCourseGroups.Add(group);
                var items = GetItemsByGroups(order.ServingCourseGroups, GroupType.ServingCourse, terminalKey);
                List<int> courseGroupKeys = new List<int>();
                items.ForEach(s => courseGroupKeys.Add(s.CourseKey));
                order.CourseGroups = DbOrderGroup.GetOrderGroups(order, courseGroupKeys);
                FilterItems(order, items);
            }
            else
            {
                order.CourseGroups.Add(group);
                var items = GetItemsByGroups(order.ServingCourseGroups, GroupType.ServingCourse, terminalKey);
                List<int> sCourseGroupKeys = new List<int>();
                items.ForEach(s => sCourseGroupKeys.Add(s.ServingCourseKey));
                order.ServingCourseGroups = DbOrderGroup.GetOrderGroups(order, sCourseGroupKeys);
                FilterItems(order, items);
            }
            return order;
        }
        public static void OrderBumped(int orderKey, DateTime dateTime)
        {
            var whereCondition = " Where ORDER_KEY = @ORDER_KEY";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("BUMP_TIME", dateTime));
            fbParameters.Add(new QueryParameter("IS_BUMPED", true));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("ORDERS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("ORDER_KEY", orderKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }
        private static Order GetOrder(int orderKey)
        {
            var whereCondition = " WHERE ORDER_KEY=@ORDER_KEY";
            var queryString = DatabaseCore.Instance.BuildSelectAllQuery("ORDERS", whereCondition);
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("ORDER_KEY", orderKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, parameters);
            if (resultSet.Rows.Count > 0)
            {
                var order = ExtractOrderInformation(resultSet.Rows[0]);
                return order;
            }
            return null;
        }
        private static Order ExtractOrderInformation(DataRow dataRow)
        {
            var order = new Order();
            order.OrderKey = Convert.ToInt32(dataRow["ORDER_KEY"]);
            order.OrderNumber = Convert.ToInt32(dataRow["ORDER_NUMBER"]);
            order.OrderPosKey = Convert.ToInt32(dataRow["ORDER_POS_KEY"]);
            order.PatronCount = Convert.ToInt32(dataRow["PATRON_COUNT"]);
            order.ChitValue = Convert.ToString(dataRow["CHIT_VALUE"]);
            order.TableTabName = Convert.ToString(dataRow["TABLE_TAB_NAME"]);
            order.SourceTableName = Convert.ToString(dataRow["SOURCE_TABLE_TAB_NAME"]);
            order.PartyName = Convert.ToString(dataRow["PARTY_NAME"]);
            order.ServerName = Convert.ToString(dataRow["SERVER_NAME"]);
            order.CustomerName = Convert.ToString(dataRow["CUSTOMER_NAME"]);
            order.OrderType = Convert.ToString(dataRow["ORDER_TYPE"]);
            order.OrderStatus = (OrderStatus)Convert.ToInt32(dataRow["ORDER_STATUS"]);
            order.SaleStartTime = Convert.ToString(dataRow["ORDER_SALE_START_TIME"]).ToDateTime();
            order.SaleFinishTime = Convert.ToString(dataRow["ORDER_SALE_FINISH_TIME"]).ToDateTime();
            order.DeliveryTime = Convert.ToString(dataRow["DELIVERY_TIME"]).ToDateTime();
            order.ArrivalTime = Convert.ToString(dataRow["ARRIVAL_TIME"]).ToDateTime();
            order.BumpTime = Convert.ToString(dataRow["BUMP_TIME"]).ToDateTime();
            order.CustomerPhone = Convert.ToString(dataRow["CUSTOMER_PHONE"]);
            order.CustomerAddress = Convert.ToString(dataRow["CUSTOMER_ADDRESS"]);
            order.CustomerEmail = Convert.ToString(dataRow["CUSTOMER_EMAIL"]);
            order.PaymentStatus = Convert.ToString(dataRow["PAYMENT_STATUS"]);
            order.OrderAction = Convert.ToString(dataRow["ORDER_ACTION"]);
            return order;
        }
        private static List<int> GetOrderKeys(int terminalKey)
        {
            var orderkeys = new List<int>();
            var queryString = @"SELECT DISTINCT(A.ORDER_KEY) FROM ORDERS A
                                INNER JOIN ORDERGROUPS B 
                                ON A.ORDER_KEY = B.ORDER_KEY
                                INNER JOIN ORDERITEMS C 
                                ON B.ORDERGROUP_KEY = C.SERVINGCOURSE_KEY
                                WHERE C.ORDER_ITEM_STATUS <> @ORDER_ITEM_STATUS AND C.TERMINAL_KEY =@TERMINAL_KEY";
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("ORDER_ITEM_STATUS", OrderStatus.Bumped));
            parameters.Add(new QueryParameter("TERMINAL_KEY", terminalKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, parameters);
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                orderkeys.Add(Convert.ToInt32(resultSet.Rows[i][0]));
            }
            return orderkeys;
        }
        private static List<int> GetBumpedOrderKeys()
        {
            var orderkeys = new List<int>();
            var queryString = @"SELECT DISTINCT(A.ORDER_KEY) FROM ORDERS A
                                INNER JOIN ORDERGROUPS B 
                                ON A.ORDER_KEY = B.ORDER_KEY
                                INNER JOIN ORDERITEMS C 
                                ON B.ORDERGROUP_KEY = C.SERVINGCOURSE_KEY
                                WHERE C.TERMINAL_KEY = 0 AND A.BUMP_TIME > DATEADD(-4 HOUR TO timestamp 'NOW') AND A.IS_BUMPED = @IS_BUMPED
                                ORDER BY A.BUMP_TIME DESC";
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("IS_BUMPED", true));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, parameters);
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                orderkeys.Add(Convert.ToInt32(resultSet.Rows[i][0]));
            }
            return orderkeys;
        }
        private static ObservableCollection<Item> GetItemsByGroups(ObservableCollection<Group> groups,
            GroupType groupType, int terminalKey, bool isBumped = false)
        {
            var groupKeys = new List<int>();
            groups.ForEach(s => groupKeys.Add(s.OrderGroupKey));
            var items = new ObservableCollection<Item>();
            items = !isBumped ? DbOrderItem.GetOrderItems(groupKeys, groupType, terminalKey) : DbOrderItem.GetBumpedOrderItems(groupKeys, groupType, terminalKey);
            return items;
        }

        private static void FilterItems(Order order, ObservableCollection<Item> items)
        {
            foreach (var item in items)
            {
                FilterItem(order, item);
            }
        }
        private static void FilterItem(Order order, Item item)
        {
            item.OrderKey = order.OrderKey;
            bool servingCourseFound = false;
            bool courseFound = false;
            var servingCourse = order.ServingCourseGroups.FirstOrDefault(s => s.OrderGroupKey == item.ServingCourseKey);
            if (servingCourse != null)
            {
                item.SCourseGroup = servingCourse;
                servingCourse.Items.Add(item);
                servingCourseFound = true;
            }
            var course = order.CourseGroups.FirstOrDefault(s => s.OrderGroupKey == item.CourseKey);
            if (course != null)
            {
                item.CourseGroup = course;
                course.Items.Add(item);
                courseFound = true;
            }
            if (servingCourseFound && courseFound)
                order.Items.Add(item);
        }
    }
}
