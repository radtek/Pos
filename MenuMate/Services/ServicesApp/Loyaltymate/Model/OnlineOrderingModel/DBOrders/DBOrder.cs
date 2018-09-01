using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using Loyaltymate.Model.OnlineOrderingModel.DBOrders;
using Loyaltymate.Model.OnlineOrderingModel.OrderModels;


namespace Loyaltymate.Model.OnlineOrderingModel.DBOrders
{
    public class DBOrder
    {
        //public FbTransaction transaction;
        //public FbConnection connection;
        OnlineOrderDBQueries dbQueries = OnlineOrderDBQueries.Instance;

        public string BuildConnectionString(string ipAddress, string dbpath)
        {
            string connectionString = "User=SYSDBA;Password=masterkey;Database=" + dbpath + ";DataSource=" + ipAddress +
                                      ";Port=3050;Dialect=3;Charset=NONE;Role=;Connection lifetime=15;Pooling=false;" +
                                      "MinPoolSize=0;MaxPoolSize=50;Packet Size=8192;ServerType=0;";
            return connectionString;
        }

        public void AddOrdersToDB(ApiSiteOrderViewModel siteOrderViewModel)
        {
            //string dbpath = "C:\\Program Files (x86)\\MenuMate\\Menumate.fdb";
            //string ipAddress = "localhost";

            FbConnection addDetailsConnection = new FbConnection(BuildConnectionString("localhost", "C:\\Program Files (x86)\\MenuMate\\Menumate.fdb"));
            addDetailsConnection.Open();

            FbTransaction addDetailsTransaction = addDetailsConnection.BeginTransaction();
            string SQLCommandText = "INSERT INTO SALES_TYPE (SALES_TYPE_KEY, SALES_TYPE_NAME, DEFAULT_SALES_TYPE) VALUES(8, 'test', 'F') ";

            FbCommand addDetailsCommand = new FbCommand(SQLCommandText, addDetailsConnection, addDetailsTransaction);
            addDetailsCommand.ExecuteNonQuery();
            addDetailsTransaction.Commit();
        }

        public void AddRecords(ApiSiteOrderViewModel siteOrderViewModel)
        {
            FbConnection addDetailsConnection = new FbConnection(BuildConnectionString("localhost", "C:\\Program Files (x86)\\MenuMate\\Menumate.fdb"));
            addDetailsConnection.Open();

            FbTransaction addDetailsTransaction = addDetailsConnection.BeginTransaction();
            try
            {

                OrderAttributes orderRow = new OrderAttributes();
                orderRow.ContainerName = siteOrderViewModel.ContainerName;
                orderRow.ContainerType = siteOrderViewModel.ContainerType;
                orderRow.Location = siteOrderViewModel.Location;
                orderRow.OrderType = siteOrderViewModel.OrderType;
                orderRow.OrderGuid = siteOrderViewModel.OrderGuid;
                orderRow.TerminalName = siteOrderViewModel.TerminalName;
                orderRow.TransactionDate = siteOrderViewModel.TransactionDate;
                orderRow.TransactionType = siteOrderViewModel.TransactionType;
                orderRow.UserType = siteOrderViewModel.UserType;
                orderRow.MembershipProfileId = siteOrderViewModel.UserReferenceId; //memberid

                foreach (var item in siteOrderViewModel.OrderItems)
                {
                    orderRow.Name = item.Name;
                    orderRow.OrderId = item.OrderItemId;
                    orderRow.SiteItemId = item.SiteItemId;
                    foreach (var itemSize in item.OrderItemSizes)
                    {
                        orderRow.BasePrice = itemSize.BasePrice;
                        orderRow.ItemSizeId = itemSize.ItemSizeId;
                        orderRow.MenuPrice = itemSize.MenuPrice;
                        orderRow.SizeName = itemSize.Name;
                        orderRow.Price = itemSize.Price;
                        orderRow.PriceInclusive = itemSize.PriceInclusive;
                        orderRow.Quantity = itemSize.Quantity;
                        orderRow.ItemSizeId = itemSize.ItemSizeId;

                        //Generate order id..
                        orderRow.OrderId = GenerateKey(addDetailsTransaction, addDetailsConnection, "ORDERS");

                        //generate tab key if tab not exist..
                        orderRow.TabKey = GetOrCreateTabForOnlineOrdering(addDetailsTransaction, addDetailsConnection, 5, orderRow.OrderGuid, ""); //TODo

                        //Generate tansaction number..
                        orderRow.TramsNo = GenerateKey(addDetailsTransaction, addDetailsConnection, "TRANSNO");

                        //load And insert breakdown category into orderscategory..
                        GetAndInsertBreakDownCategories(addDetailsTransaction, addDetailsConnection, itemSize.ItemSizeId, ref orderRow);

                        //Insert records to orders..
                        ExecuteOrderQuery(addDetailsTransaction, addDetailsConnection, orderRow);

                        //insert security event to security..
                        ExecuteSecurityQuery(addDetailsTransaction, addDetailsConnection, orderRow);
                    }
                }
                addDetailsTransaction.Commit();
            }
            catch (Exception ex)
            {
                addDetailsTransaction.Rollback();
            }
        }

        public int GenerateKey(FbTransaction transaction, FbConnection connection, string inTableName)
        {
            object commandResult = null;
            try
            {
                string generatorName = "GEN_" + inTableName;
                string sql = "SELECT GEN_ID(" + generatorName + ", 1) FROM RDB$DATABASE";

                FbCommand command = new FbCommand(sql, connection, transaction);
                if ((commandResult = command.ExecuteScalar()) == null)
                {
                    throw new KeyGeneratorException(inTableName + " key genertor failed to produce key.");
                }
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in _generateKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 164, short.MaxValue);
            }
            return Convert.ToInt32(commandResult);
        }

        private class KeyGeneratorException : Exception
        {
            public KeyGeneratorException() : base() { }
            public KeyGeneratorException(string message) : base(message) { }
            public KeyGeneratorException(string message, System.Exception inner) : base(message, inner) { }
        }

        private int GetOrCreateTabForOnlineOrdering(FbTransaction transaction, FbConnection connection, int onlineOrderKey, string tabName, string id_number)
        {
            int tabKey = FindTabKeyForOnlineOrderTab(transaction, connection, tabName);
            try
            {
                if (tabKey == 0)
                    tabKey = createTabForOnlineOrder(transaction, connection, onlineOrderKey, tabName, id_number);
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 133, short.MaxValue);
            }
            return tabKey;
        }

        private int FindTabKeyForOnlineOrderTab(FbTransaction transaction, FbConnection connection, string tabName)
        {
            int tabKey = 0;

            try
            {
                FbCommand command = dbQueries.GetTabKeyByTabName(connection, transaction, tabName);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        tabKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "TAB_KEY",
                                                        0));
                }
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in findTabKeyForWebOrderKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 269, short.MaxValue);
            }

            return tabKey;
        }

        private int createTabForOnlineOrder(FbTransaction transaction, FbConnection connection, int onlineOrderKey, string tabName, string id_number)
        {
            int tabKey = CreateOnlineOrderTabInDB(transaction, connection, tabName, id_number);
            //updateWebOrderWithTabKey(webOrderKey, tabKey);

            return tabKey;
        }

        private int CreateOnlineOrderTabInDB(FbTransaction transaction, FbConnection connection, string tabName, string id_number)
        {
            int tabKey = GenerateKey(transaction, connection, "TAB");

            try
            {
                FbCommand command = dbQueries.CreateTab(connection, transaction, tabKey, tabName, id_number);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in createWebOrderTabInDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 270, short.MaxValue);
            }

            return tabKey;
        }

        object getReaderColumnValue(FbDataReader reader, string columnName, object defaultValue)
        {
            int ordinal = reader.GetOrdinal(columnName);
            if (reader.IsDBNull(ordinal))
                return defaultValue;
            else
                return reader[ordinal];
        }

        private void GetAndInsertBreakDownCategories(FbTransaction transaction, FbConnection connection, long itemSizeUniqueId, ref OrderAttributes orderInfo)
        {
            try
            {
                FbCommand command = dbQueries.GetItemSizeInfo(connection, transaction, itemSizeUniqueId);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        orderInfo.PLU = reader.GetInt32(reader.GetOrdinal("PLU"));
                        orderInfo.MenuItemKey = reader.GetInt32(reader.GetOrdinal("MENU_ITEM_KEY"));
                        orderInfo.ThirdPartyCodeKey = reader.GetInt32(reader.GetOrdinal("THIRDPARTYCODES_KEY"));
                        orderInfo.CostGSTPercent = reader.GetDouble(reader.GetOrdinal("COST_GST_PERCENT"));
                        orderInfo.PatronCount = reader.GetInt32(reader.GetOrdinal("DEFAULT_PATRON_COUNT"));
                        orderInfo.GSTPercent = reader.GetDouble(reader.GetOrdinal("GST_PERCENT"));
                        orderInfo.Cost = reader.GetDouble(reader.GetOrdinal("COST"));
                        orderInfo.CategoryKey = reader.GetInt32(reader.GetOrdinal("CATEGORY_KEY"));
                        LoadItemSizeBreakDownCategories(transaction, connection, orderInfo.OrderId, reader.GetInt32(reader.GetOrdinal("ITEMSIZE_KEY")));
                    }
                }
            }
            catch (Exception ex)
            {
            }

        }

        private void LoadItemSizeBreakDownCategories(FbTransaction transaction, FbConnection connection, long orderKey, int itemSizeId)
        {
            try
            {
                FbCommand command = dbQueries.LoadBreakDownCategoriesCmd(connection, transaction, itemSizeId);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        FbCommand brkdcategoryInsertCmd = dbQueries.InsertBreakDownCategoryToDB(connection, transaction, orderKey, reader.GetInt32(reader.GetOrdinal("Category_Key")));
                    }
                }
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
            }
        }

        private void ExecuteOrderQuery(FbTransaction transaction, FbConnection connection, OrderAttributes orderRow)
        {
            try
            {
                FbCommand command = dbQueries.InsertRecordToOrders(connection, transaction, orderRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
            }
        }

        private void ExecuteSecurityQuery(FbTransaction transaction, FbConnection connection, OrderAttributes orderRow)
        {
            try
            {
                long securityKey = GenerateKey(transaction, connection, "SECURITY_KEY");
                FbCommand command = dbQueries.InsertIntoSecurity(connection, transaction, orderRow, securityKey);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                //ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
            }
        }

    }
}
