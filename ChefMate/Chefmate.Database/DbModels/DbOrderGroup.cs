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
    public class DbOrderGroup
    {
        public static void AddOrderGroups(ObservableCollection<Group> inGroups)
        {
            foreach (var inGroup in inGroups)
            {
                AddOrderGroup(inGroup);
            }
        }
        public static void AddOrderGroup(Group inGroup)
        {
            inGroup.GroupKey = DbGroup.GetOrCreateGroup(inGroup);
            inGroup.OrderGroupKey = DatabaseCore.Instance.GetGeneratorValue("GEN_ORDERGROUP");
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("ORDERGROUP_KEY", inGroup.OrderGroupKey));
            fbParameters.Add(new QueryParameter("ORDER_KEY", inGroup.Order.OrderKey));
            fbParameters.Add(new QueryParameter("GROUP_KEY", inGroup.GroupKey));
            fbParameters.Add(new QueryParameter("ORDERGROUP_CALLED_AWAY", inGroup.CalledAway));
            var queryString = DatabaseCore.Instance.BuildInsertQuery("ORDERGROUPS", fbParameters);
            DatabaseCore.Instance.ExecuteNonQuery(queryString, fbParameters);
        }
        public static ObservableCollection<Group> GetOrderGroups(Order order, GroupType inGroupType)
        {
            var queryString = @"SELECT * FROM ORDERGROUPS A
                                INNER JOIN GROUPS B
                                ON A.GROUP_KEY = B.GROUP_KEY
                                WHERE A.ORDER_KEY = @ORDER_KEY AND B.GROUP_TYPE = @GROUP_TYPE";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("GROUP_TYPE", inGroupType));
            queryParameters.Add(new QueryParameter("ORDER_KEY", order.OrderKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            return ExtractGroupsInformation(resultSet, order);
        }
        public static void CallAwayGroup(int orderGroupKey,bool callAwayStatus)
        {
            var whereCondition = " WHERE ORDERGROUP_KEY=@ORDERGROUP_KEY";
            var parameters = new List<QueryParameter>();
            parameters.Add(new QueryParameter("ORDERGROUP_CALLED_AWAY", callAwayStatus));
            var queryString = DatabaseCore.Instance.BuildUpdateQuery("ORDERGROUPS", parameters, whereCondition);
            parameters.Add(new QueryParameter("ORDERGROUP_KEY", orderGroupKey));
            DatabaseCore.Instance.ExecuteNonQuery(queryString, parameters);
        }
        public static int GetOrderKeyFromGroupKey(int inOrderGroupDbKey)
        {
            var queryString = @"SELECT ORDER_KEY FROM ORDERGROUPS  WHERE A.ORDERGROUP_KEY = @ORDERGROUP_KEY";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("ORDERGROUP_KEY", inOrderGroupDbKey));
            var orderKey = DatabaseCore.Instance.ExecuteScalar<int>(queryString, queryParameters);
            return orderKey;
        }
        public static Group GetOrderGroup(int orderGroupKey)
        {
            var queryString = @"SELECT * FROM ORDERGROUPS A
                                INNER JOIN GROUPS B
                                ON A.GROUP_KEY = B.GROUP_KEY
                                WHERE A.ORDERGROUP_KEY = @ORDERGROUP_KEY";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("ORDERGROUP_KEY", orderGroupKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            var group = ExtractGroupInformation(resultSet.Rows[0]);
            return group;
        }
        public static ObservableCollection<Group> GetOrderGroups(Order order, List<int> groupKeys)
        {
            var inClouse = " in (";
            foreach (var groupKey in groupKeys)
            {
                inClouse += groupKey + ",";
            }
            inClouse = inClouse.Substring(0, inClouse.Length - 1);

            var queryString = @"SELECT * FROM ORDERGROUPS A
                                INNER JOIN GROUPS B
                                ON A.GROUP_KEY = B.GROUP_KEY
                                WHERE A.ORDERGROUP_KEY ";
            queryString += inClouse + ")";
            var queryParameters = new List<QueryParameter>();
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            var items = ExtractGroupsInformation(resultSet, order);
            return items;
        }
        private static ObservableCollection<Group> ExtractGroupsInformation(DataTable resultSet, Order order)
        {
            var groups = new ObservableCollection<Group>();
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                var group = ExtractGroupInformation(resultSet.Rows[i]);
                group.Order = order;
                groups.Add(group);
            }
            return groups;
        }
        private static Group ExtractGroupInformation(DataRow resultSet)
        {
            var group = new Group();
            group.GroupKey = Convert.ToInt32(resultSet["GROUP_KEY"]);
            group.GroupPosKey = Convert.ToInt32(resultSet["GROUP_POS_KEY"]);
            group.GroupName = Convert.ToString(resultSet["GROUP_NAME"]);
            group.DisplayOrder = Convert.ToInt32(resultSet["GROUP_DISPLAY_ORDER"]);
            group.GroupType = (GroupType)Convert.ToInt32(resultSet["GROUP_TYPE"]); ;
            group.OrderGroupKey = Convert.ToInt32(resultSet["ORDERGROUP_KEY"]);
            group.CalledAway = Convert.ToBoolean(resultSet["ORDERGROUP_CALLED_AWAY"]);
            group.OrderKey = Convert.ToInt32(resultSet["ORDER_KEY"]);
            return group;
        }
    }
}
