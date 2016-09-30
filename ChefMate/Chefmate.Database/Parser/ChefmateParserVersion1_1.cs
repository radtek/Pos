using System.Runtime.InteropServices;
using Chefmate.Database.DbModels;

namespace ChefMate.Database.Parser
{
    public class ChefmateParserVersion1_1 : VersionParser
    {
        private const string Version = "1.1";
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
                if (!DbSettings.SettingExist("RecallCount", terminalKey))
                    DbSettings.AddSettingRow("RecallCount", 5, terminalKey);
                if (!DbSettings.SettingExist("FontSize", terminalKey))
                    DbSettings.AddSettingRow("FontSize", 15, terminalKey);

            }
        }

    }
}