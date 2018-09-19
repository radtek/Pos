using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using Loyaltymate.Model.OnlineOrderingModel.OrderModels;
using MenumateServices.Tools;

namespace MenumateServices.DTO.OnlineOrdering.DBOrders
{
    public class OnlineOrderDB
    {

        // Singleton object: Povides with connections to the Menumate's dabase
        OnlineOrderDBConnection dbConnection_ = OnlineOrderDBConnection.Instance;
        OnlineOrderDBQueries dbQueries = OnlineOrderDBQueries.Instance;

        public FbTransaction transaction;
        public FbConnection connection;
        public OnlineOrderDB()
        {
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly OnlineOrderDB instance = new OnlineOrderDB();
        }

        #region Public

        public static OnlineOrderDB Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        public bool BeginTransaction()
        {
            try
            {
                connection = dbConnection_.Open();
                ServiceLogger.Log("In BeginTransaction() after dbConnection_.Open().");
                transaction = connection.BeginTransaction();
                ServiceLogger.Log("In BeginTransaction() after connection_.BeginTransaction().");
                return true;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in BeginTransaction " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 133, short.MaxValue);
                return false;
            }

        }

        public void EndTransaction()
        {
            try
            {
                if (transaction != null)
                {
                    transaction.Commit();
                    ServiceLogger.Log("In EndTransaction() after transaction_.Commit().");
                }
                if (connection != null)
                {
                    ServiceLogger.Log("In EndTransaction() before connection_.Close().");
                    connection.Close();
                    ServiceLogger.Log("In EndTransaction() after connection_.Close().");
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in EndTransaction " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 133, short.MaxValue);
            }
        }

        public FbConnection BeginConnection()
        {
            return dbConnection_.Open();
        }

        public FbTransaction BeginFBtransaction()
        {
            return connection.BeginTransaction();
        }

        public void RollbackTransaction()
        {
            try
            {
                if (transaction != null)
                    transaction.Rollback();
                if (connection != null)
                    connection.Close();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in RollbackTransaction " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 134, short.MaxValue);
            }
        }

        public string BuildConnectionString(string ipAddress, string dbpath)
        {
            string connectionString = "User=SYSDBA;Password=masterkey;Database=" + dbpath + ";DataSource=" + ipAddress +
                                      ";Port=3050;Dialect=3;Charset=NONE;Role=;Connection lifetime=15;Pooling=false;" +
                                      "MinPoolSize=0;MaxPoolSize=50;Packet Size=8192;ServerType=0;";
            return connectionString;
        }

        public void AddRecords(ref List<ApiSiteOrderViewModel> siteOrderViewModelList)
        {
            try
            {
                
                    foreach (var siteOrderViewModel in siteOrderViewModelList)
                    {
                        try
                        {
                            OrderAttributes orderRow = new OrderAttributes();

                            if (siteOrderViewModel.ContainerType == Loyaltymate.Enum.OrderContainerType.Table)
                            {
                                bool canOrderBeSaved = dbQueries.IsTableAlreadyOccupied(connection, transaction, siteOrderViewModel.UserEmailId, siteOrderViewModel.ContainerNumber);
                                if (!canOrderBeSaved)
                                    new Exception("Order can't be saved to this table because it already contains orders.");
                            }
                            orderRow.ContainerName = siteOrderViewModel.ContainerName;
                            orderRow.ContainerType = siteOrderViewModel.ContainerType;
                            orderRow.ContainerNumber = siteOrderViewModel.ContainerNumber;
                            orderRow.Location = siteOrderViewModel.Location;
                            orderRow.OrderType = siteOrderViewModel.OrderType;
                            orderRow.OrderGuid = siteOrderViewModel.OrderGuid;
                            orderRow.TerminalName = siteOrderViewModel.TerminalName;
                            orderRow.TransactionDate = siteOrderViewModel.TransactionDate;
                            orderRow.TransactionType = siteOrderViewModel.TransactionType;
                            orderRow.UserType = siteOrderViewModel.UserType;
                            orderRow.MembershipProfileId = siteOrderViewModel.UserReferenceId; //memberid
                            orderRow.Email = siteOrderViewModel.UserEmailId;
                            orderRow.SiteId = siteOrderViewModel.SiteId;
                            orderRow.OnlinerderId = siteOrderViewModel.OrderId;

                            if (orderRow.ContainerNumber < 1 || orderRow.ContainerNumber > 100)
                                orderRow.ContainerType = 0;

                            //generate tab key if tab not exist..
                            orderRow.TabKey = orderRow.ContainerType == 0 ? GetOrCreateTabForOnlineOrdering(orderRow.ContainerName, "1")
                                                : GetOrCreateTableForOnlineOrdering(orderRow.ContainerNumber, orderRow.ContainerName); //TODo 

                            //Generate Security ref..
                            orderRow.SecurityRef = GetNextSecurityRef();    

                            foreach (var item in siteOrderViewModel.OrderItems)
                            {
                                orderRow.Name = item.Name;
                                orderRow.OrderItemId = item.OrderItemId;
                                orderRow.SiteItemId = item.SiteItemId;
                                orderRow.ItemUniqueId = item.ItemUniqueId;
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
                                    orderRow.TimeKey = setTimeKey();
                                    orderRow.ItemSizeUniqueId = itemSize.ItemSizeUniqueId;
                                    orderRow.OrderItemSizeId = itemSize.OrderItemSizeId;
                                    orderRow.ReferenceOrderItemSizeId = itemSize.ReferenceOrderItemSizeId;
                                    orderRow.SideOrderKey = orderRow.ReferenceOrderItemSizeId > 0 ? GetSideParentOrderKey(itemSize.ReferenceOrderItemSizeId) : 0;
                                    orderRow.ItemUniqueId = GetItemUniqueID(orderRow.ItemSizeUniqueId);

                                    //Generate order id..
                                    orderRow.OrderId = GenerateKey("ORDERS");                                                                                                       

                                    //Load Item info like course, sc, kitchen name etc.
                                    LoadItemInfo(ref orderRow);

                                    //Generate tansaction number..
                                    if (orderRow.ReferenceOrderItemSizeId == 0)
                                    {
                                        orderRow.TramsNo = GenerateKey("PCINTERNALTRANSNUMBER");
                                        orderRow.MasterContainer = orderRow.SizeName;
                                    }

                                    //load And insert breakdown category into orderscategory..
                                    GetAndInsertBreakDownCategories(ref orderRow);

                                    //LoadTaxProfileKeys
                                    LoadItemSizeTaxProfileOrders(ref orderRow);

                                    //Insert records to orders..
                                    ExecuteOrderQuery(orderRow);

                                    //Insert Order tax profile info..
                                    ExecuteTaxProfileOrders(orderRow);                                    
                                }
                            }
                            siteOrderViewModel.IsConfirmed = true;
                        }
                        catch (Exception ex)
                        {
                            siteOrderViewModel.IsConfirmed = false;
                            //addDetailsTransaction.Rollback();
                            ServiceLogger.LogException(@"in AddRecords to orders table " + ex.Message, ex);
                            throw;
                        }

                    }
            }
            catch (Exception ex)
            {
                //addDetailsTransaction.Rollback();
                ServiceLogger.LogException(@"in AddRecords to orders table " + ex.Message, ex);
                throw;
            }
        }

        public int GenerateKey(string inTableName)
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
                ServiceLogger.LogException(@"in _generateKey " + e.Message, e);
                throw;
            }
            return Convert.ToInt32(commandResult);
        }

        private class KeyGeneratorException : Exception
        {
            public KeyGeneratorException() : base() { }
            public KeyGeneratorException(string message) : base(message) { }
            public KeyGeneratorException(string message, System.Exception inner) : base(message, inner) { }
        }

        private int GetOrCreateTabForOnlineOrdering(string tabName, string id_number)
        {
            int tabKey = FindTabKeyForOnlineOrderTab(tabName);
            try
            {
                if (tabKey == 0)
                    tabKey = createTabForOnlineOrder(tabName, id_number);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
            return tabKey;
        }

        private int GetOrCreateTableForOnlineOrdering(int tableNumber, string containerName)
        {
            if (tableNumber < 1 || tableNumber > 100)
                throw new Exception("Table number must be between 1 and 100.");

            int tableKey = FindTableKeyForOnlineOrder(tableNumber);
            try
            {
                if (tableKey == 0)
                    tableKey = CreateOnlineOrderTableInDB(tableNumber);
                int seatKey = GetOrCreateSeatForOnlineOrdering(tableKey);
                tableKey = GetTabKey(seatKey, containerName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
            return tableKey;
        }

        private int GetTabKey(int seatKey, string containerName)
        {
            int tabKey = 0;
            try
            {
                FbCommand command = dbQueries.GetTabKeyBySeatKey(connection, transaction, seatKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        tabKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "TAB_KEY",
                                                        0));
                }
                if (!dbQueries.GetTabExists(connection, transaction, tabKey))
                {
                    if (tabKey == 0)
                        tabKey = GenerateKey("TAB");
                    command = dbQueries.CreateTab(connection, transaction, tabKey, containerName, 3);
                    command.ExecuteNonQuery();
                    SetSeatTab(tabKey, seatKey);
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
            return tabKey;
        }

        private void SetSeatTab(int tabKey, int seatKey)
        {
            try
            {
                FbCommand command = dbQueries.SetSeatTabKey(connection, transaction, seatKey, tabKey);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
        }

        private int GetOrCreateSeatForOnlineOrdering(int tableKey)
        {
            FbCommand command = dbQueries.GetSeatForTable(connection, transaction, tableKey);
            int seatKey = 0;
            try
            {
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        seatKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "SEAT_KEY",
                                                        0));
                }

                if (seatKey == 0)
                {
                    seatKey = GenerateKey("SEAT");
                    command = dbQueries.CreateSeatForTable(connection, transaction, tableKey, seatKey);
                    command.ExecuteNonQuery();
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
            return seatKey;
        }
        
        private int FindTabKeyForOnlineOrderTab(string tabName)
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
                ServiceLogger.LogException(@"in findTabKeyForWebOrderKey " + e.Message, e);
                throw;
            }

            return tabKey;
        }

        private int FindTableKeyForOnlineOrder(int tableNumber)
        {
            int tableKey = 0;

            try
            {
                FbCommand command = dbQueries.GetTableKeyByTableNumber(connection, transaction, tableNumber);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        tableKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "TABLE_KEY",
                                                        0));
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in FindTableKeyForOnlineOrde " + e.Message, e);
                throw;
            }

            return tableKey;
        }

        private int createTabForOnlineOrder(string tabName, string id_number)
        {
            int tabKey = CreateOnlineOrderTabInDB(tabName, id_number);
            return tabKey;
        }

        private int CreateOnlineOrderTabInDB(string tabName, string id_number)
        {
            int tabKey = GenerateKey("TAB");

            try
            {
                FbCommand command = dbQueries.CreateTab(connection, transaction, tabKey, tabName, 0);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in createWebOrderTabInDB " + e.Message, e);
                throw;
            }

            return tabKey;
        }

        private int CreateOnlineOrderTableInDB(int tableNumber)
        {
            int tableKey = GenerateKey("TABLES");

            try
            {
                FbCommand command = dbQueries.CreateTable(connection, transaction, tableKey, tableNumber);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in createWebOrderTabInDB " + e.Message, e);
                throw;
            }

            return tableKey;
        }

        object getReaderColumnValue(FbDataReader reader, string columnName, object defaultValue)
        {
            int ordinal = reader.GetOrdinal(columnName);
            if (reader.IsDBNull(ordinal))
                return defaultValue;
            else
                return reader[ordinal];
        }

        private void GetAndInsertBreakDownCategories(ref OrderAttributes orderInfo)
        {
            try
            {
                FbCommand command = dbQueries.GetItemSizeInfo(connection, transaction, orderInfo.ItemSizeUniqueId);

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
                        orderInfo.PointsPercent = reader.GetDouble(reader.GetOrdinal("POINTS_PERCENT"));
                        orderInfo.ItemSizeKey = reader.GetInt32(reader.GetOrdinal("ITEMSIZE_KEY"));
                        orderInfo.SizeName = reader.GetString(reader.GetOrdinal("SIZE_NAME"));
                        orderInfo.SizeKitchenName = reader.GetString(reader.GetOrdinal("SIZE_KITCHEN_NAME"));
                        orderInfo.SizeKitchenName = orderInfo.SizeKitchenName.Trim() == "" ? orderInfo.SizeName : orderInfo.SizeKitchenName;
                        LoadItemSizeBreakDownCategories(orderInfo.OrderId, reader.GetInt32(reader.GetOrdinal("ITEMSIZE_KEY")));
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException(@"in GetAndInsertBreakDownCategories while inserting records " + ex.Message, ex);
                throw;
            }

        }

        private void LoadItemSizeBreakDownCategories(long orderKey, int itemSizeId)
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
                ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                throw;
            }
        }

        private void LoadItemSizeTaxProfileOrders(ref OrderAttributes orderInfo)
        {
            try
            {
                FbCommand command = dbQueries.GetItemSizeTaxProfileKey(connection, transaction, orderInfo.ItemSizeKey);
                orderInfo.TaxProfileKeys = new List<int>();
                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        orderInfo.TaxProfileKeys.Add(reader.GetInt32(reader.GetOrdinal("PROFILE_KEY")));
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                throw;
            }
        }

        private void ExecuteOrderQuery(OrderAttributes orderRow)
        {
            try
            {
                FbCommand command = dbQueries.InsertRecordToOrders(connection, transaction, orderRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                throw;
            }
        }

        private void ExecuteTaxProfileOrders(OrderAttributes orderRow)
        {
            try
            {
                foreach (var takProfileKey in orderRow.TaxProfileKeys)
                {
                    long tpoKey = GenerateKey("TAXPROFILES_ORDERS");
                    FbCommand command = dbQueries.InsertTaxProfilesOrders(connection, transaction, tpoKey, takProfileKey, orderRow.OrderId);
                    command.ExecuteNonQuery();
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExecuteTaxProfileOrders " + e.Message, e);
                throw;
            }
        }

        private void LoadItemInfo(ref OrderAttributes orderInfo)
        {
            try
            {
                FbCommand command = dbQueries.GetItemInfo(connection, transaction, orderInfo.ItemUniqueId);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        orderInfo.ItemCategory = reader.GetString(reader.GetOrdinal("ITEM_CATEGORY"));
                        orderInfo.Name = reader.GetString(reader.GetOrdinal("ITEM_NAME"));
                        orderInfo.ItemKitchenName = reader.GetString(reader.GetOrdinal("ITEM_KITCHEN_NAME"));
                        orderInfo.ItemKitchenName = orderInfo.ItemKitchenName.Trim() == "" ? orderInfo.Name : orderInfo.ItemKitchenName;                        
                        orderInfo.CourseName = reader.GetString(reader.GetOrdinal("COURSE_NAME"));
                        orderInfo.CourseKitchenName = reader.GetString(reader.GetOrdinal("COURSE_KITCHEN_NAME"));
                        orderInfo.CourseKitchenName = orderInfo.CourseKitchenName.Trim() == "" ? orderInfo.CourseName : orderInfo.CourseKitchenName;
                        orderInfo.MenuName = reader.GetString(reader.GetOrdinal("MENU_NAME"));
                        orderInfo.SetvingCourseKey = reader.GetInt32(reader.GetOrdinal("SERVINGCOURSES_KEY"));
                        orderInfo.ItemId = reader.GetInt32(reader.GetOrdinal("ITEM_ID"));                        
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadItemInfo " + e.Message, e);
                throw;
            }

        }

        private string GetItemUniqueID(string itemSizeUniqueId)
        {
            string ItemUniqueId = "";
            try
            {
                FbCommand command = dbQueries.GetItemUniqueIdByItemSIzeUniqueID(connection, transaction, itemSizeUniqueId);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        ItemUniqueId = Convert.ToString(getReaderColumnValue(reader, "ITEM_IDENTIFIER", ""));
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSideParentOrderKey " + e.Message, e);
                throw;
            }
            return ItemUniqueId;
        }

        private long GetSideParentOrderKey(long ordreferenceOrderItemSizeId)
        {
            long ParentSideOrderKey = 0;
            try
            {
                FbCommand command = dbQueries.GetSideParentOrderKey(connection, transaction, ordreferenceOrderItemSizeId);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        ParentSideOrderKey = Convert.ToInt32(getReaderColumnValue(reader, "ORDER_KEY", 0));
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSideParentOrderKey " + e.Message, e);
                throw;
            }
            return ParentSideOrderKey;
        }

        private int setTimeKey()
        {
            int timeKey = GenerateKey("TURN_AROUND_TIMES");
            try
            {
                openSaleStartTime(timeKey);
                closeSaleStartTime(timeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in setTimeKey " + e.Message, e);
                throw;
            }

            return timeKey;
        }

        private void openSaleStartTime(int timeKey)
        {
            try
            {
                FbCommand command = dbQueries.OpenSaleStartTimeCmd(connection, transaction, timeKey);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in openSaleStartTime " + e.Message, e);
                throw;
            }
        }

        private void closeSaleStartTime(int timeKey)
        {
            try
            {
                FbCommand command = dbQueries.CloseSaleStartTimeCmd(connection, transaction, timeKey);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in closeSaleStartTime " + e.Message, e);
                throw;
            }
        }

        private long GetNextSecurityRef()
        {
            return GenerateKey("SECURITY_REF");
        }
        #endregion
    }

}
