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
        private static Settings ExtractSettings(DataTable dataTable)
        {
            var settingCollection = new Dictionary<string, string>();
            for (int i = 0; i < dataTable.Rows.Count; i++)
            {
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
            return true;
        }
        private static void AddSettingRow(string key, string value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildInsertQuery("TERMINALSETTINGS", parmeters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        private static void AddSettingRow(string key, int value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildInsertQuery("TERMINALSETTINGS", parmeters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        private static void AddSettingRow(string key, bool value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildInsertQuery("TERMINALSETTINGS", parmeters);
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
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
        }
        private static void SaveSettingRow(string key, string value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            var whereCondition = " WHERE SETTINGNAME=@SETTINGNAME AND TERMINALKEY=@TERMINALKEY";
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALSETTINGS", parmeters, whereCondition);
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        private static void SaveSettingRow(string key, int value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            var whereCondition = " WHERE SETTINGNAME=@SETTINGNAME AND TERMINALKEY=@TERMINALKEY";
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALSETTINGS", parmeters, whereCondition);
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
        private static void SaveSettingRow(string key, bool value, int terminakKey)
        {
            var parmeters = new List<QueryParameter>();
            var whereCondition = " WHERE SETTINGNAME=@SETTINGNAME AND TERMINALKEY=@TERMINALKEY";
            parmeters.Add(new QueryParameter("SETTINGVALUE", value));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALSETTINGS", parmeters, whereCondition);
            parmeters.Add(new QueryParameter("TERMINALKEY", terminakKey));
            parmeters.Add(new QueryParameter("SETTINGNAME", key));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, parmeters);
        }
    }




}