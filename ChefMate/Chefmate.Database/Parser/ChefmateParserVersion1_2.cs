using System.Runtime.InteropServices;
using Chefmate.Database.DbModels;
using ChefMate.Database;
using ChefMate.Database.Parser;

namespace Chefmate.Database.Parser
{
    class ChefmateParserVersion1_2 : VersionParser
    {
        private const string Version = "1.2";
        public override void RunParser()
        {
            if (!IsVersionExist(Version))
            {
                AddSettings();
                UpdateVersionInDatabase(Version);
            }
        }

        private void AddSettings()
        {
            var terminalKeys = DbTerminal.GetTerminalKeys();
            foreach (var terminalKey in terminalKeys)
            {
                if (!DbSettings.SettingExist("WebOrderTime", terminalKey))
                    DbSettings.AddSettingRow("WebOrderTime", 20, terminalKey);

            }
        }
    }
}
