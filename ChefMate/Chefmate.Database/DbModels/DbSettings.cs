using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using Chefmate.Core.Enums;
using Chefmate.Core.Extensions;
using Chefmate.Core.Model;
using Chefmate.Database.Model;

namespace ChefMate.Database
{
    public class DbSettings
    {
        public static Settings GetSttings(int terminalKey)
        {
            var whereCondition = " WHERE TERMINALKEY = @TERMINALKEY";
            var commandText = DatabaseCore.Instance.BuildSelectAllQuery("TERMINALSETTINGS", whereCondition);
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("TERMINALKEY", terminalKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(commandText, fbParameters);
            return ExtractSettings(resultSet);
        }
        public static Settings ExtractSettings(DataTable dataTable)
        {
            var settingCollection = new Dictionary<string, string>();
            for (int i = 0; i < dataTable.Rows.Count; i++)
            {
                if (!settingCollection.ContainsKey(Convert.ToString(dataTable.Rows[i]["SETTINGNAME"])))
                    settingCollection.Add(Convert.ToString(dataTable.Rows[i]["SETTINGNAME"]), Convert.ToString(dataTable.Rows[i]["SETTINGVALUE"]));
            }
            var settings = new Settings();
            settings.AutoHide = settingCollection["AutoHide"].ToBoolean();
            settings.BumpOnly = settingCollection["BumpOnly"].ToBoolean();
            settings.ConfirmOnRefund = settingCollection["ConfirmOnRefund"].ToBoolean();
            settings.ConfirmOnTransfer = settingCollection["ConfirmOnTransfer"].ToBoolean();
            settings.FirstWarningTime = settingCollection["FirstWarningTime"].ToInteger();
            settings.SecondWarningTime = settingCollection["SecondWarningTime"].ToInteger();
            settings.DbIpAddress = settingCollection["DbIpAddress"];
            settings.DbPath = settingCollection["DbPath"];
            settings.TerminalIpAddress = settingCollection["TerminalIpAddress"];
            settings.OutputTerminal = settingCollection["OutputTerminal"].ToInteger();
            settings.NewOrderMmSound = settingCollection["NewOrderMmSound"];
            settings.CancelNoticeMmSound = settingCollection["CancelNoticeMmSound"];
            settings.RefundNoticeMmSound = settingCollection["TransferNoticeMmSound"];
            settings.TransferNoticeMmSound = settingCollection["TransferNoticeMmSound"];
            settings.OrderArrivedCmSound = settingCollection["OrderArrivedCmSound"];
            settings.TransferOrderCmSound = settingCollection["TransferOrderCmSound"];
            settings.OutputTime = (OutputTime)settingCollection["OutputTime"].ToInteger();
            settings.OutputScope = (OutputScope)settingCollection["OutputScope"].ToInteger();
            settings.KeypadOperation = (KeypadOperation)settingCollection["KeypadOperation"].ToInteger();
            settings.TerminalType = (TerminalType)settingCollection["TerminalType"].ToInteger();
            settings.GroupType = (GroupType)settingCollection["GroupType"].ToInteger();
            settings.OrderInfoDisplay = (OrderInfoDisplay)settingCollection["OrderInfoDisplay"].ToInteger();
            settings.OutputType = (OutputType)settingCollection["OutputType"].ToInteger();
            settings.OrderLayout = (OrderLayout)settingCollection["OrderLayout"].ToInteger();
            settings.RecallCount = settingCollection["RecallCount"].ToInteger();
            settings.WebOrderTime = settingCollection["WebOrderTime"].ToInteger();

            settings.CmFontSize = settingCollection["FontSize"].ToInteger();
            settings.ItemFontFamily = settingCollection["ItemFontFamily"];

            settings.GroupFontSize = settingCollection["GroupFontSize"].ToInteger();
            settings.GroupFontBold = settingCollection["GroupFontBold"].ToBoolean();
            settings.GroupFontFamily = settingCollection["GroupFontFamily"];

            settings.AnalysisFontSize = settingCollection["AnalysisFontSize"].ToInteger();
            settings.AnalysisFontBold = settingCollection["AnalysisFontBold"].ToBoolean();
            settings.AnalysisFontFamily = settingCollection["AnalysisFontFamily"];

            settings.OrderHeaderFontSize = settingCollection["OrderFontSize"].ToInteger();
            settings.OrderFontFamily = settingCollection["OrderFontFamily"];

            settings.NewOrderColor = settingCollection["NewOrderColor"];
            settings.FirstWarningColor = settingCollection["FirstWarningColor"];
            settings.SecondWarningColor = settingCollection["SecondWarningColor"];
            settings.HeaderBackgroundColor = settingCollection["HeaderBackgroundColor"];
            settings.HeaderForegroundColor = settingCollection["HeaderForegroundColor"];


            return settings;
        }
        public static bool AddSettings(Settings settings, int terminakKey)
        {
            AddSettingRow("AutoHide", settings.AutoHide, terminakKey);
            AddSettingRow("BumpOnly", settings.BumpOnly, terminakKey);
            AddSettingRow("ConfirmOnRefund", settings.ConfirmOnRefund, terminakKey);
            AddSettingRow("ConfirmOnTransfer", settings.ConfirmOnTransfer, terminakKey);

            AddSettingRow("FirstWarningTime", settings.FirstWarningTime, terminakKey);
            AddSettingRow("SecondWarningTime", settings.SecondWarningTime, terminakKey);

            AddSettingRow("DbIpAddress", settings.DbIpAddress, terminakKey);
            AddSettingRow("DbPath", settings.DbPath, terminakKey);
            AddSettingRow("TerminalIpAddress", settings.TerminalIpAddress, terminakKey);
            AddSettingRow("OutputTerminal", settings.OutputTerminal, terminakKey);
            AddSettingRow("NewOrderMmSound", settings.NewOrderMmSound, terminakKey);
            AddSettingRow("CancelNoticeMmSound", settings.CancelNoticeMmSound, terminakKey);
            AddSettingRow("RefundNoticeMmSound", settings.RefundNoticeMmSound, terminakKey);
            AddSettingRow("TransferNoticeMmSound", settings.TransferNoticeMmSound, terminakKey);
            AddSettingRow("OrderArrivedCmSound", settings.OrderArrivedCmSound, terminakKey);
            AddSettingRow("TransferOrderCmSound", settings.TransferOrderCmSound, terminakKey);

            AddSettingRow("OutputTime", Convert.ToInt32(settings.OutputTime), terminakKey);
            AddSettingRow("OutputScope", Convert.ToInt32(settings.OutputScope), terminakKey);
            AddSettingRow("KeypadOperation", Convert.ToInt32(settings.KeypadOperation), terminakKey);
            AddSettingRow("TerminalType", Convert.ToInt32(settings.TerminalType), terminakKey);
            AddSettingRow("GroupType", Convert.ToInt32(settings.GroupType), terminakKey);
            AddSettingRow("OrderInfoDisplay", Convert.ToInt32(settings.OrderInfoDisplay), terminakKey);
            AddSettingRow("OutputType", Convert.ToInt32(settings.OutputType), terminakKey);
            AddSettingRow("OrderLayout", Convert.ToInt32(settings.OrderLayout), terminakKey);
            AddSettingRow("RecallCount", settings.RecallCount, terminakKey);
            AddSettingRow("WebOrderTime", settings.WebOrderTime, terminakKey);

            AddSettingRow("FontSize", settings.CmFontSize, terminakKey);
            AddSettingRow("ItemFontFamily", settings.ItemFontFamily, terminakKey);

            AddSettingRow("GroupFontSize", settings.GroupFontSize, terminakKey);
            AddSettingRow("GroupFontFamily", settings.GroupFontFamily, terminakKey);
            AddSettingRow("GroupFontBold", settings.GroupFontBold, terminakKey);

            AddSettingRow("AnalysisFontSize", settings.AnalysisFontSize, terminakKey);
            AddSettingRow("AnalysisFontFamily", settings.AnalysisFontFamily, terminakKey);
            AddSettingRow("AnalysisFontBold", settings.AnalysisFontBold, terminakKey);

            AddSettingRow("OrderFontSize", settings.OrderHeaderFontSize, terminakKey);
            AddSettingRow("OrderFontFamily", settings.OrderFontFamily, terminakKey);

            AddSettingRow("NewOrderColor", settings.NewOrderColor, terminakKey);
            AddSettingRow("FirstWarningColor", settings.FirstWarningColor, terminakKey);
            AddSettingRow("SecondWarningColor", settings.SecondWarningColor, terminakKey);
            AddSettingRow("HeaderBackgroundColor", settings.HeaderBackgroundColor, terminakKey);
            AddSettingRow("HeaderForegroundColor", settings.HeaderForegroundColor, terminakKey);

            return true;
        }
        public static void AddSettingRow(string key, string value, int terminakKey)
        {
            if (!SettingExist(key, terminakKey))
            {
                var parmeters = new List<QueryParameter>();
                parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
                parmeters.Add(new QueryParameter("SETTINGNAME", key));
                parmeters.Add(new QueryParameter("SETTINGVALUE", value));
                var commandText = DatabaseCore.Instance.BuildInsertQuery("TERMINALSETTINGS", parmeters);
                DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
            }
        }
        public static void AddSettingRow(string key, int value, int terminakKey)
        {
            if (!SettingExist(key, terminakKey))
            {
                var parmeters = new List<QueryParameter>();
                parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
                parmeters.Add(new QueryParameter("SETTINGNAME", key));
                parmeters.Add(new QueryParameter("SETTINGVALUE", value));
                var commandText = DatabaseCore.Instance.BuildInsertQuery("TERMINALSETTINGS", parmeters);
                DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
            }
        }
        public static void AddSettingRow(string key, bool value, int terminakKey)
        {
            if (!SettingExist(key, terminakKey))
            {
                var parmeters = new List<QueryParameter>();
                parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
                parmeters.Add(new QueryParameter("SETTINGNAME", key));
                parmeters.Add(new QueryParameter("SETTINGVALUE", value));
                var commandText = DatabaseCore.Instance.BuildInsertQuery("TERMINALSETTINGS", parmeters);
                DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
            }
        }
        public static void SaveSettings(Settings settings, int terminakKey)
        {
            SaveSettingRow("AutoHide", settings.AutoHide, terminakKey);
            SaveSettingRow("BumpOnly", settings.BumpOnly, terminakKey);
            SaveSettingRow("ConfirmOnRefund", settings.ConfirmOnRefund, terminakKey);
            SaveSettingRow("ConfirmOnTransfer", settings.ConfirmOnTransfer, terminakKey);
            SaveSettingRow("FirstWarningTime", settings.FirstWarningTime, terminakKey);
            SaveSettingRow("SecondWarningTime", settings.SecondWarningTime, terminakKey);
            SaveSettingRow("DbIpAddress", settings.DbIpAddress, terminakKey);
            SaveSettingRow("DbPath", settings.DbPath, terminakKey);
            SaveSettingRow("TerminalIpAddress", settings.TerminalIpAddress, terminakKey);
            SaveSettingRow("OutputTerminal", settings.OutputTerminal, terminakKey);
            SaveSettingRow("NewOrderMmSound", settings.NewOrderMmSound, terminakKey);
            SaveSettingRow("CancelNoticeMmSound", settings.CancelNoticeMmSound, terminakKey);
            SaveSettingRow("RefundNoticeMmSound", settings.RefundNoticeMmSound, terminakKey);
            SaveSettingRow("TransferNoticeMmSound", settings.TransferNoticeMmSound, terminakKey);
            SaveSettingRow("OrderArrivedCmSound", settings.OrderArrivedCmSound, terminakKey);
            SaveSettingRow("TransferOrderCmSound", settings.TransferOrderCmSound, terminakKey);
            SaveSettingRow("OutputTime", Convert.ToInt32(settings.OutputTime), terminakKey);
            SaveSettingRow("OutputScope", Convert.ToInt32(settings.OutputScope), terminakKey);
            SaveSettingRow("KeypadOperation", Convert.ToInt32(settings.KeypadOperation), terminakKey);
            SaveSettingRow("TerminalType", Convert.ToInt32(settings.TerminalType), terminakKey);
            SaveSettingRow("GroupType", Convert.ToInt32(settings.GroupType), terminakKey);
            SaveSettingRow("OrderInfoDisplay", Convert.ToInt32(settings.OrderInfoDisplay), terminakKey);
            SaveSettingRow("OutputType", Convert.ToInt32(settings.OutputType), terminakKey);
            SaveSettingRow("OrderLayout", Convert.ToInt32(settings.OrderLayout), terminakKey);
            SaveSettingRow("RecallCount", settings.RecallCount, terminakKey);
            SaveSettingRow("WebOrderTime", settings.WebOrderTime, terminakKey);

            SaveSettingRow("FontSize", settings.CmFontSize, terminakKey);
            SaveSettingRow("ItemFontFamily", settings.ItemFontFamily, terminakKey);

            SaveSettingRow("GroupFontSize", settings.GroupFontSize, terminakKey);
            SaveSettingRow("GroupFontFamily", settings.GroupFontFamily, terminakKey);
            SaveSettingRow("GroupFontBold", settings.GroupFontBold, terminakKey);

            SaveSettingRow("AnalysisFontSize", settings.AnalysisFontSize, terminakKey);
            SaveSettingRow("AnalysisFontFamily", settings.AnalysisFontFamily, terminakKey);
            SaveSettingRow("AnalysisFontBold", settings.AnalysisFontBold, terminakKey);

            SaveSettingRow("OrderFontSize", settings.OrderHeaderFontSize, terminakKey);
            SaveSettingRow("OrderFontFamily", settings.OrderFontFamily, terminakKey);

            SaveSettingRow("NewOrderColor", settings.NewOrderColor, terminakKey);
            SaveSettingRow("FirstWarningColor", settings.FirstWarningColor, terminakKey);
            SaveSettingRow("SecondWarningColor", settings.SecondWarningColor, terminakKey);
            SaveSettingRow("HeaderBackgroundColor", settings.HeaderBackgroundColor, terminakKey);
            SaveSettingRow("HeaderForegroundColor", settings.HeaderForegroundColor, terminakKey);
        }
        public static void SaveSettingRow(string key, string value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            var whereCondition = " WHERE SETTINGNAME=@SETTINGNAME AND TERMINALKEY=@TERMINALKEY";
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALSETTINGS", parmeters, whereCondition);
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        public static void SaveSettingRow(string key, int value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            var whereCondition = " WHERE SETTINGNAME=@SETTINGNAME AND TERMINALKEY=@TERMINALKEY";
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALSETTINGS", parmeters, whereCondition);
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        public static void SaveSettingRow(string key, bool value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            var whereCondition = " WHERE SETTINGNAME=@SETTINGNAME AND TERMINALKEY=@TERMINALKEY";
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALSETTINGS", parmeters, whereCondition);
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        public static bool SettingExist(string key, int terminakKey)
        {
            var whereCondition = " WHERE SETTINGNAME='" + key + "' AND TERMINALKEY=" + terminakKey;
            var commandText = DatabaseCore.Instance.BuildRecordCountQuery("TERMINALSETTINGS", whereCondition);
            return DatabaseCore.Instance.ExecuteScalar<int>(commandText) > 0;
        }


    }




}