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
            AddSettings();
            AlterTableOrders();
            UpdateVersionInDatabase(Version);
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
        private void AlterTableOrders()
        {
            var query = @"ALTER TABLE ORDERS
		                        ADD CUSTOMER_PHONE Varchar(25),
                                ADD CUSTOMER_EMAIL Varchar(50),
                                ADD CUSTOMER_ADDRESS Varchar(250),
                                ADD PAYMENT_STATUS Varchar(20),
                                ADD ORDER_ACTION Varchar(20);";
            CheckAndCreateColumn("ORDERS", "CUSTOMER_PHONE", query);
            CheckAndCreateColumn("ORDERS", "CUSTOMER_EMAIL", query);
            CheckAndCreateColumn("ORDERS", "CUSTOMER_ADDRESS", query);
            CheckAndCreateColumn("ORDERS", "PAYMENT_STATUS", query);
            CheckAndCreateColumn("ORDERS", "ORDER_ACTION", query);
        }
    }
}
