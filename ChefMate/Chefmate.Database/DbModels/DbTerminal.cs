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
    public class DbTerminal
    {
        public static Terminal GetTerminal(string terminalName)
        {
            var whereCondition = " Where TERMINALNAME=@TERMINALNAME";
            var commandText = DatabaseCore.Instance.BuildSelectAllQuery("TERMINALS", whereCondition);
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("TERMINALNAME", terminalName));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(commandText, fbParameters);
            if (resultSet.Rows.Count > 0)
            {
                return ExtractTerminalInformation(resultSet.Rows[0]);
            }
            return null;
        }
        public static Terminal GetTerminal(int terminalKey)
        {
            var whereCondition = " Where TERMINALID=@TERMINALID";
            var commandText = DatabaseCore.Instance.BuildSelectAllQuery("TERMINALS", whereCondition);
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("TERMINALID", terminalKey));
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(commandText, fbParameters);
            if (resultSet.Rows.Count > 0)
            {
                return ExtractTerminalInformation(resultSet.Rows[0]);
            }
            return null;
        }
        private static Terminal ExtractTerminalInformation(DataRow dataRow)
        {
            var terminal = new Terminal();
            terminal.TerminalId = Convert.ToInt32(dataRow["TERMINALID"]);
            terminal.TerminalName = Convert.ToString(dataRow["TERMINALNAME"]);
            terminal.TerminalIpAddress = Convert.ToString(dataRow["TERMINALIPADDRESS"]);
            terminal.DisplayName = Convert.ToString(dataRow["DISPLAY_NAME"]);
            terminal.TerminalType = (TerminalType)Convert.ToInt32(dataRow["TERMINAL_TYPE"]);
            return terminal;
        }
        public static ObservableCollection<Terminal> GetAllTerminals()
        {
            var terminals = new ObservableCollection<Terminal>();
            var commandText = DatabaseCore.Instance.BuildSelectAllQuery("TERMINALS");
            var resultSet = DatabaseCore.Instance.ExecuteDataSetQuery(commandText);
            for (int i = 0; i < resultSet.Rows.Count; i++)
            {
                terminals.Add(ExtractTerminalInformation(resultSet.Rows[i]));
            }
            return terminals;
        }
        public static bool AddTerminal(Terminal terminal)
        {
            var existingTerminal = GetTerminal(terminal.TerminalName);
            if (existingTerminal == null)
            {
                terminal.TerminalId = DatabaseCore.Instance.GetGeneratorValue("GEN_TERMINAL");
                var fbParameters = new List<QueryParameter>();
                fbParameters.Add(new QueryParameter("TERMINALID", terminal.TerminalId));
                fbParameters.Add(new QueryParameter("TERMINALNAME", terminal.TerminalName));
                fbParameters.Add(new QueryParameter("TERMINALIPADDRESS", terminal.TerminalIpAddress));
                fbParameters.Add(new QueryParameter("DISPLAY_NAME", terminal.DisplayName));
                fbParameters.Add(new QueryParameter("TERMINAL_TYPE", terminal.TerminalType));
                var queryString = DatabaseCore.Instance.BuildInsertQuery("TERMINALS", fbParameters);
                DatabaseCore.Instance.ExecuteNonQuery(queryString, fbParameters);
                return true;
            }
            else
            {
                terminal.TerminalId = existingTerminal.TerminalId;
                UpdateTerminal(terminal);
                return false;
            }

        }
        public static void UpdateTerminal(Terminal terminal)
        {
            UpdateTerminal(terminal.TerminalIpAddress, terminal.DisplayName, terminal.TerminalType, terminal.TerminalId);
        }
        public static void UpdateTerminal(string terminalIpAddress, string displayName, TerminalType terminalType, int terminalKey)
        {
            var whereCondition = " Where TERMINALID=@TERMINALID";
            var fbParameters = new List<QueryParameter>();
            fbParameters.Add(new QueryParameter("TERMINALIPADDRESS", terminalIpAddress));
            fbParameters.Add(new QueryParameter("DISPLAY_NAME", displayName));
            fbParameters.Add(new QueryParameter("TERMINAL_TYPE", terminalType));
            var commandText = DatabaseCore.Instance.BuildUpdateQuery("TERMINALS", fbParameters, whereCondition);
            fbParameters.Add(new QueryParameter("TERMINALID", terminalKey));
            DatabaseCore.Instance.ExecuteNonQuery(commandText, fbParameters);
        }

    }
}
