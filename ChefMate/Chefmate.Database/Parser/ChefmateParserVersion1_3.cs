using Chefmate.Database.DbModels;
using ChefMate.Database;
using ChefMate.Database.Parser;

namespace Chefmate.Database.Parser
{
    public class ChefmateParserVersion1_3 : VersionParser
    {
        private const string Version = "1.3";
        public override void RunParser()
        {
            AddSettings();
            UpdateVersionInDatabase(Version);
        }

        private void AddSettings()
        {
            var terminalKeys = DbTerminal.GetTerminalKeys();
            foreach (var terminalKey in terminalKeys)
            {
                if (!DbSettings.SettingExist("ItemFontFamily", terminalKey))
                    DbSettings.AddSettingRow("ItemFontFamily", "Tahoma", terminalKey);

                if (!DbSettings.SettingExist("OrderFontSize", terminalKey))
                    DbSettings.AddSettingRow("OrderFontSize", 18, terminalKey);
                if (!DbSettings.SettingExist("OrderFontFamily", terminalKey))
                    DbSettings.AddSettingRow("OrderFontFamily", "Tahoma", terminalKey);


                if (!DbSettings.SettingExist("GroupFontSize", terminalKey))
                    DbSettings.AddSettingRow("GroupFontSize", 18, terminalKey);
                if (!DbSettings.SettingExist("GroupFontFamily", terminalKey))
                    DbSettings.AddSettingRow("GroupFontFamily", "Tahoma", terminalKey);
                if (!DbSettings.SettingExist("GroupFontBold", terminalKey))
                    DbSettings.AddSettingRow("GroupFontBold", true, terminalKey);

                if (!DbSettings.SettingExist("AnalysisFontSize", terminalKey))
                    DbSettings.AddSettingRow("AnalysisFontSize", 18, terminalKey);
                if (!DbSettings.SettingExist("AnalysisFontFamily", terminalKey))
                    DbSettings.AddSettingRow("AnalysisFontFamily", "Tahoma", terminalKey);
                if (!DbSettings.SettingExist("AnalysisFontBold", terminalKey))
                    DbSettings.AddSettingRow("AnalysisFontBold", false, terminalKey);


                if (!DbSettings.SettingExist("FirstWarningColor", terminalKey))
                    DbSettings.AddSettingRow("FirstWarningColor", "#FFF1DAB4", terminalKey);
                if (!DbSettings.SettingExist("SecondWarningColor", terminalKey))
                    DbSettings.AddSettingRow("SecondWarningColor", "#FFF4B4B4", terminalKey);
                if (!DbSettings.SettingExist("HeaderBackgroundColor", terminalKey))
                    DbSettings.AddSettingRow("HeaderBackgroundColor", "#F09422", terminalKey);
                if (!DbSettings.SettingExist("HeaderForegroundColor", terminalKey))
                    DbSettings.AddSettingRow("HeaderForegroundColor", "#FFFFFF", terminalKey);
                if (!DbSettings.SettingExist("NewOrderColor", terminalKey))
                    DbSettings.AddSettingRow("NewOrderColor", "#FFB4C7B4", terminalKey);
            }
        }

    }
}