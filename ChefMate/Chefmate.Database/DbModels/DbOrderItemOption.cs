using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using Chefmate.Core.Model;
using Chefmate.Database.Model;
using ChefMate.Database;

namespace Chefmate.Database.DbModels
{
    public class DbOrderItemOption
    {
        public static void AddOrderItemOptions(ObservableCollection<Option> options)
        {
            foreach (var option in options)
            {
                AddOrderItemOption(option);
            }
        }
        public static void AddOrderItemOption(Option inOption)
        {
            inOption.OptionKey = DbOption.GetOrCreateOption(inOption);
            inOption.OrderOptionKey = DatabaseCore.Instance.GetGeneratorValue("GEN_ORDERITEMOPTION");
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("ORDERITEMOPTION_KEY", inOption.OrderOptionKey));
            fbParameters.Add(new QueryParameter("ORDERITEM_KEY", inOption.Item.OrderItemKey));
            fbParameters.Add(new QueryParameter("OPTION_KEY", inOption.OptionKey));
            fbParameters.Add(new QueryParameter("IS_PLUS", inOption.IsPlus));
            var queryString = DatabaseCore.Instance.BuildInsertQuery("ORDERITEMOPTIONS", fbParameters);
            DatabaseCore.Instance.ExecuteNonQuery(queryString, fbParameters);
        }
        public static ObservableCollection<Option> GetOrderItemOptions(Item item)
        {
            var queryString = @"SELECT * FROM ORDERITEMOPTIONS A
                                INNER JOIN OPTIONS B
                                ON A.OPTION_KEY = B.OPTION_KEY
                                WHERE A.ORDERITEM_KEY = @ORDERITEM_KEY";
            var queryParameters = new List<QueryParameter>();
            queryParameters.Add(new QueryParameter("ORDERITEM_KEY", item.OrderItemKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(queryString, queryParameters);
            return ExtractOptionsInformation(resultSet, item);
        }
        private static ObservableCollection<Option> ExtractOptionsInformation(DataTable resultSet, Item item)
        {
            var options = new ObservableCollection<Option>();
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                var option = ExtractOptionInformation(resultSet.Rows[i]);
                option.Item = item;
                options.Add(option);
            }
            return options;
        }
        private static Option ExtractOptionInformation(DataRow resultSet)
        {
            var option = new Option();
            option.OptionKey = Convert.ToInt32(resultSet["OPTION_KEY"]);
            option.OrderOptionKey = Convert.ToInt32(resultSet["ORDERITEMOPTION_KEY"]);
            option.IsPlus = Convert.ToBoolean(resultSet["IS_PLUS"]);
            option.Name = Convert.ToString(resultSet["OPTION_NAME"]);
            option.OrderItemKey = Convert.ToInt32(resultSet["ORDERITEM_KEY"]);
            return option;
        }
    }
}
