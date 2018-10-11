using Chefmate.Database.DbModels;
using ChefMate.Database;
using ChefMate.Database.Parser;

namespace Chefmate.Database.Parser
{
    public class ChefmateParserVersion1_4 : VersionParser
    {
        private const string Version = "1.4";
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
                if (!DbSettings.SettingExist("ItemSummaryBackgroundColor", terminalKey))
                    DbSettings.AddSettingRow("ItemSummaryBackgroundColor", "#C4D3C4", terminalKey);
                if (!DbSettings.SettingExist("ItemSummaryFontSize", terminalKey))
                    DbSettings.AddSettingRow("ItemSummaryFontSize", 28, terminalKey);
                if (!DbSettings.SettingExist("ItemSummaryFontFamily", terminalKey))
                    DbSettings.AddSettingRow("ItemSummaryFontFamily", "Tahoma", terminalKey);
            }
        }

    }
}
