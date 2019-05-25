using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using OnlineOrdering.Model.OrderModels;
using OnlineOrdering.Model.NotificationModels;
using OnlineOrdering.Enum;
using MenumateServices.Tools;
using System.IO;
using OnlineOrdering.Utility;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
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

        public bool AddRecords(ApiSiteOrderViewModel siteOrderViewModel, bool IsOrderFromWaiterApp)
        {
            bool retValue = false;
            try
            {
                OrderAttributes orderRow = new OrderAttributes();
                orderRow.ContainerNumber = 0;
                orderRow.ContainerType = siteOrderViewModel.ContainerType;
                orderRow.ContainerName = siteOrderViewModel.ContainerName;

                if (siteOrderViewModel.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table && !IsOrderFromWaiterApp)
                {
                    int containerNumber = 0;
                    string tableName = "";
                    bool isTableNumberInteger = int.TryParse(siteOrderViewModel.ContainerNumber, out containerNumber);

                    if (isTableNumberInteger)
                        orderRow.ContainerNumber = containerNumber;
                    else
                        orderRow.TableName = tableName = siteOrderViewModel.ContainerNumber;
                    bool isTabOrder = false;
                    //if (IsFloorPlanEnabled())
                    //{
                    if (IsTableMarkedForOnlineordering(ref containerNumber, ref tableName)) //CheckTableExistAndGetTableInfo removed for china changes
                    {
                        orderRow.ContainerNumber = containerNumber;
                        orderRow.TableName = tableName;

                        bool retVal = IsTableBusy(orderRow.ContainerNumber, orderRow.TableName, siteOrderViewModel.UserEmailId);
                            if (IsFloorPlanEnabled())
                            {
                                siteOrderViewModel.ContainerName = orderRow.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ? orderRow.TableName : orderRow.Email;
                            }
                            else
                            {
                                siteOrderViewModel.ContainerName = orderRow.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ? " #" + orderRow.ContainerNumber : orderRow.Email;
                                orderRow.TableName = "Table #" + orderRow.ContainerNumber;
                            }
                        

                        if (retVal)
                            throw new Exception("Order can't be saved to this table because it already contains orders.");
                    }
                    else
                    {
                        throw new Exception("Order can't be saved to this table because it is not marked for online ordering.");
                    }
                    //}
                    //else
                    //{
                    //if (orderRow.ContainerNumber < 1 || orderRow.ContainerNumber >= 100)
                    //{
                    //    isTabOrder = true;
                    //}
                    //else
                    //{
                    //    bool retVal = IsTableBusy(orderRow.ContainerNumber, orderRow.TableName, siteOrderViewModel.UserEmailId);
                    //    if (retVal)
                    //        throw new Exception("Order can't be saved to this table because it already contains orders.");
                    //    siteOrderViewModel.ContainerName = orderRow.ContainerType == Loyaltymate.Enum.OrderContainerType.Table ? " #" + orderRow.ContainerNumber : orderRow.Email;
                    //    orderRow.TableName = "Table #" + orderRow.ContainerNumber;
                    //}
                    //}

                    if (isTabOrder)
                    {
                        orderRow.ContainerType = siteOrderViewModel.ContainerType = 0;
                        orderRow.TransactionType = siteOrderViewModel.TransactionType = OnlineOrdering.Enum.SiteSettingType.PickUp;
                        siteOrderViewModel.ContainerNumber = "0";
                    }
                }
                //orderRow.ContainerNumber = siteOrderViewModel.ContainerNumber;
                orderRow.Location = siteOrderViewModel.Location;
                orderRow.OrderType = siteOrderViewModel.OrderType;
                orderRow.OrderGuid = siteOrderViewModel.OrderGuid;
                if (siteOrderViewModel.ApiOrderDevicesViewModel != null)
                    orderRow.TerminalName = siteOrderViewModel.ApiOrderDevicesViewModel.DeviceName;
                else
                    orderRow.TerminalName = null;
                orderRow.TransactionDate = siteOrderViewModel.TransactionDate;
                orderRow.TransactionType = siteOrderViewModel.TransactionType;
                orderRow.UserType = siteOrderViewModel.UserType;
                orderRow.MembershipProfileId = siteOrderViewModel.UserReferenceId; //memberid
                orderRow.Email = siteOrderViewModel.UserEmailId;
                orderRow.SiteId = siteOrderViewModel.SiteId;
                orderRow.OnlinerderId = siteOrderViewModel.OrderId;
                //to do happy hour implementation

                //generate tab key if tab not exist..
                if (IsOrderFromWaiterApp)
                {
                    int containerNoInt = 0;
                    int.TryParse(siteOrderViewModel.ContainerNumber,out containerNoInt);

                    orderRow.TabKey = orderRow.ContainerType == 0 ? GetOrCreateTabForOnlineOrdering(siteOrderViewModel.ContainerName)
                                    : GetOrCreateTableForOnlineOrdering(containerNoInt, "Guest 1", siteOrderViewModel.ContainerNumber);
                    orderRow.TableName = siteOrderViewModel.ContainerName;
                    orderRow.ContainerNumber = containerNoInt;
                }
                else
                {
                    orderRow.TabKey = orderRow.ContainerType == 0 ? GetOrCreateTabForOnlineOrdering(orderRow.ContainerName)
                                    : GetOrCreateTableForOnlineOrdering(orderRow.ContainerNumber, orderRow.ContainerName, orderRow.TableName);
                }
                 //TODo 

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

                        //load ItemSize info..
                        GetAndLoadItemSizeInfo(ref orderRow);

                        //LoadTaxProfileKeys
                        LoadItemSizeTaxProfileOrders(ref orderRow);

                        //Insert records to orders..
                        ExecuteOrderQuery(orderRow);

                        //load And insert breakdown category into orderscategory..
                        LoadAndInsertItemSizeBreakDownCategories(orderRow.OrderId, orderRow.ItemSizeKey);

                        //Insert Order tax profile info..
                        ExecuteTaxProfileOrders(orderRow);
                    }
                    siteOrderViewModel.IsHappyHourApplied = CanHappyHourBeApplied();
                    siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.IsConfirmed;
                    siteOrderViewModel.IsConfirmed = true;
                    retValue = true;
                }
            }
            catch (Exception ex)
            {
                siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.InvalidTable;
                siteOrderViewModel.IsConfirmed = false;
                siteOrderViewModel.IsHappyHourApplied = false;
                //addDetailsTransaction.Rollback();
                ServiceLogger.LogException(@"in AddRecords to orders table " + ex.Message, ex);
                throw;
            }
            return retValue;
            //   }
            // }
            /*catch (Exception ex)
            {
                //addDetailsTransaction.Rollback();
                ServiceLogger.LogException(@"in AddRecords to orders table " + ex.Message, ex);
                throw;
            }*/
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

        private int GetOrCreateTabForOnlineOrdering(string tabName)
        {
            int tabKey = FindTabKeyForOnlineOrderTab(tabName);
            try
            {
                if (tabKey == 0)
                    tabKey = createTabForOnlineOrder(tabName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
            return tabKey;
        }

        private int GetOrCreateTableForOnlineOrdering(int tableNumber, string containerName, string tableName)
        {
            int tableKey = FindTableKeyForOnlineOrder(tableNumber);
            try
            {
                if (tableKey == 0)
                    tableKey = CreateOnlineOrderTableInDB(tableNumber, tableName);
                if (IsFloorPlanEnabled())
                    SetTableName(tableKey, tableName);
                int seatKey = GetOrCreateSeatForOnlineOrdering(tableKey);
                tableKey = GetTabKey(seatKey, containerName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTableForWebOrder " + e.Message, e);
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
                if (!IsTabKeyExist(tabKey))
                {
                    if (tabKey == 0)
                        tabKey = GenerateKey("TAB");
                    command = dbQueries.CreateTab(connection, transaction, tabKey, containerName, 3);
                    command.ExecuteNonQuery();
                    SetSeatTab(tabKey, seatKey);
                }
                else
                {
                    command = dbQueries.UpdateTabName(connection, transaction, tabKey, containerName);
                    command.ExecuteNonQuery();
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                throw;
            }
            return tabKey;
        }

        private bool IsTabKeyExist(int tabKey)
        {
            int TabId = 0;
            try
            {
                FbCommand command = dbQueries.GetTabExists(connection, transaction, tabKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        TabId = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "TAB_KEY",
                                                        0));
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTabKeyExist " + e.Message, e);
                throw;
            }
            return TabId > 0;
        }

        private bool IsTableBusy(int tableNumber, string tableName, string email)
        {
            int orderKey = 0;
            try
            {
                FbCommand command;

                if (tableNumber > 0)
                {
                    command = dbQueries.CheckTableAlreadyOccupied(connection, transaction, email, tableNumber);
                }
                else
                    command = dbQueries.CheckTableAlreadyOccupied(connection, transaction, email, tableName);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        orderKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "ORDER_KEY",
                                                        0));
                }

                if (orderKey > 0)
                {
                    if (tableNumber > 0)
                    {
                        command = dbQueries.TableWithSameMemberAlreadyExist(connection, transaction, email, tableNumber);
                    }
                    else
                        command = dbQueries.TableWithSameMemberAlreadyExist(connection, transaction, email, tableName);

                    using (FbDataReader reader = command.ExecuteReader())
                    {
                        int secondOrderKey = 0;
                        if (reader.Read())
                            secondOrderKey = Convert.ToInt32(
                                            getReaderColumnValue(
                                                            reader,
                                                            "ORDER_KEY",
                                                            0));
                        if (secondOrderKey > 0)
                            orderKey = 0;
                    }
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableBusy " + e.Message, e);
                throw;
            }

            return orderKey > 0;
        }

        private bool IsFloorPlanEnabled()
        {
            int variableVal = 0;
            try
            {
                List<string> logsList = new List<string>();
                logsList.Add("Inside IsFloorPlanEnabled");
                if (connection != null)
                {
                    if (connection.ConnectionString != null)
                        logsList.Add("inUsername " + connection.ConnectionString);

                }
                logsList.Add("======================================================================================================================");
                FileWriter.WriteToFile(logsList, "Online Ordering Logs", "OnlineOrderingLogs ");
                FbCommand command = dbQueries.ReadFloorPlanProperty(connection, transaction, 2021, 1);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        variableVal = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "INTEGER_VAL",
                                                        0));
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableBusy " + e.Message, e);
                throw;
            }
            return variableVal > 0;
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

        private int createTabForOnlineOrder(string tabName)
        {
            int tabKey = CreateOnlineOrderTabInDB(tabName);
            return tabKey;
        }

        private int CreateOnlineOrderTabInDB(string tabName)
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

        private int CreateOnlineOrderTableInDB(int tableNumber, string tableName)
        {
            int tableKey = GenerateKey("TABLES");

            try
            {
                FbCommand command = dbQueries.CreateTable(connection, transaction, tableKey, tableNumber, tableName);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in createWebOrderTabInDB " + e.Message, e);
                throw;
            }

            return tableKey;
        }

        private void SetTableName(int tableKey, string tableName)
        {
            try
            {
                FbCommand command = dbQueries.UpdateTableName(connection, transaction, tableKey, tableName);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetTableName " + e.Message, e);
                throw;
            }
        }

        object getReaderColumnValue(FbDataReader reader, string columnName, object defaultValue)
        {
            int ordinal = reader.GetOrdinal(columnName);
            if (reader.IsDBNull(ordinal))
                return defaultValue;
            else
                return reader[ordinal];
        }

        private void GetAndLoadItemSizeInfo(ref OrderAttributes orderInfo)
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
                        //LoadItemSizeBreakDownCategories(orderInfo.OrderId, reader.GetInt32(reader.GetOrdinal("ITEMSIZE_KEY")));
                    }
                }
            }
            catch (Exception ex)
            {
                ServiceLogger.LogException(@"in GetAndInsertBreakDownCategories while inserting records " + ex.Message, ex);
                throw;
            }

        }

        private void LoadAndInsertItemSizeBreakDownCategories(long orderKey, int itemSizeId)
        {
            try
            {
                FbCommand command = dbQueries.LoadBreakDownCategoriesCmd(connection, transaction, itemSizeId);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        InsertBreakDownCategoryToDB(orderKey, reader.GetInt32(reader.GetOrdinal("Category_Key")));
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                throw;
            }
        }

        private void InsertBreakDownCategoryToDB(long orderKey, int categoryKey)
        {
            try
            {
                FbCommand command = dbQueries.InsertBreakDownCategoryToDB(connection, transaction, orderKey, categoryKey);
                command.ExecuteNonQuery();
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

        private bool CheckTableExistAndGetTableInfo(ref int tableNumber, ref string tableName)
        {
            bool isTableExist = false;
            try
            {
                List<string> dbDeatils = new List<string>();
                dbDeatils = FileReader.GetDetailsFromFile("MenumateDBPath\\DBPathAndIP.txt");
                string floorPlanDBPath = "", inDataSource = "";
                for (int i = 0; i < dbDeatils.Count; i++)
                {
                    if (i == 0)
                        inDataSource = dbDeatils[i];
                    else if (i == 2)
                    {
                        floorPlanDBPath = dbDeatils[i];
                        floorPlanDBPath = floorPlanDBPath + "Floorplan\\Service\\Reservations.fdb";
                    }
                }

                FbConnection selectDetailsConnection = new FbConnection(BuildConnectionString(inDataSource, floorPlanDBPath));
                selectDetailsConnection.Open();

                FbTransaction selectTableTransaction = selectDetailsConnection.BeginTransaction();
                string SQLCommandText = "";

                if (tableNumber > 0)
                    SQLCommandText = "SELECT a.NUMBER, a.NAME FROM TABLES a WHERE a.NUMBER = @NUMBER ";
                else
                    SQLCommandText = "SELECT a.NUMBER, a.NAME FROM TABLES a WHERE a.NAME = @NAME ";

                FbCommand selectTableCommand = new FbCommand(SQLCommandText, selectDetailsConnection, selectTableTransaction);
                if (tableNumber > 0)
                    selectTableCommand.Parameters.Add("@NUMBER", tableNumber);
                else
                    selectTableCommand.Parameters.Add("@NAME", tableName);

                selectTableCommand.ExecuteNonQuery();

                using (FbDataReader reader = selectTableCommand.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        tableNumber = reader.GetInt32(reader.GetOrdinal("NUMBER"));
                        tableName = reader.GetString(reader.GetOrdinal("NAME"));
                        isTableExist = true;
                    }
                }

                selectTableTransaction.Commit();
            }
            catch (Exception ex)
            {
                isTableExist = false;
                ServiceLogger.LogException(@"in CheckTableExistAndGetTableInfo " + ex.Message, ex);
                throw;
            }
            return isTableExist;
        }

        private bool CanHappyHourBeApplied()
        {
            bool isHappyHourRunning = false;
            try
            {
                FbCommand command = dbQueries.IsOnlineOrderingEnabled(connection, transaction); //Check online ordering is enabled..
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        int profileKey = Convert.ToInt32(getReaderColumnValue(reader, "PROFILE_KEY", 0));
                        command = dbQueries.IsVariableKeyExist(connection, transaction, 4129, profileKey);
                        using (FbDataReader reader1 = command.ExecuteReader())
                        {
                            if (reader1.Read())
                            {
                                isHappyHourRunning = IsForceHappyHourEnabled(profileKey);
                            }
                            else
                            {
                                isHappyHourRunning = IsHappyHourRunning(profileKey);
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSideParentOrderKey " + e.Message, e);
                throw;
            }
            return isHappyHourRunning;
        }

        private bool IsForceHappyHourEnabled(int profileKey)
        {
            bool isForceHappyHourEnabled = false;
            try
            {

                FbCommand command = dbQueries.IsVariableKeyExist(connection, transaction, 5015, profileKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        isForceHappyHourEnabled = true;
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsForceHappyHourEnabled " + e.Message, e);
                throw;
            }
            return isForceHappyHourEnabled;

        }

        private bool IsHappyHourRunning(int profileKey)
        {
            bool isHHInCurrentTimeSpan = false;
            try
            {

                if (IsForceHappyHourEnabled(profileKey))
                {
                    isHHInCurrentTimeSpan = true;
                }
                else
                {
                    isHHInCurrentTimeSpan = GetTerminalHHProfiles(profileKey);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsHappyHourRunning " + e.Message, e);
                throw;
            }
            return isHHInCurrentTimeSpan;

        }

        private bool GetTerminalHHProfiles(int profileKey)
        {
            bool isHHInCurrentTimeSpan = false;
            try
            {

                FbCommand command = dbQueries.GetTerminalHHProfiles(connection, transaction, profileKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        isHHInCurrentTimeSpan = LoadHHProfileInfo(Convert.ToInt32(getReaderColumnValue(reader, "HAPPYHOURPROFILES_KEY", 0)));
                        if (isHHInCurrentTimeSpan)
                            break;
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsHappyHourRunning " + e.Message, e);
                throw;
            }
            return isHHInCurrentTimeSpan;

        }

        private bool LoadHHProfileInfo(int hhProfileKey)
        {
            bool retVal = false;
            bool isProfileDay = false;
            try
            {
                FbCommand command = dbQueries.LoadHHProfileInfo(connection, transaction, hhProfileKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        isProfileDay = false;
                        DateTime profileDate = Convert.ToDateTime(getReaderColumnValue(reader, "HAPPYHOURDAY_PROFILEDATE", 0));
                        DateTime startTime = Convert.ToDateTime(getReaderColumnValue(reader, "HAPPYHOURDAY_STARTTIME", 0));
                        DateTime endTime = Convert.ToDateTime(getReaderColumnValue(reader, "HAPPYHOURDAY_ENDTIME", 0));

                        List<int> hhProfileDays = LoadDaysInfoForSelectedProfile(Convert.ToInt32(getReaderColumnValue(reader, "HAPPYHOURPROFILES_KEY", 0)), profileDate);
                        DateTime currentTime = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second, 000);


                        bool inTime = false;
                        bool isValidProfile = false;

                        //check whether any profile fall under the current time stamp

                        if (profileDate.Year != 1899)   // if date is set
                        {
                            //Dateutils::CompareDateTime(hhProfile->StartTime,Now()) == GreaterThanValue
                            if ((!(startTime > DateTime.Now)) && (!(endTime < DateTime.Now)))
                            {
                                isValidProfile = true;
                            }
                        }
                        else   //  if No Date is Set
                        {
                            //check if current day is present  in profile day settings
                            for (int day = 0; day < hhProfileDays.Count; day++)
                            {
                                if (hhProfileDays[day] == 0)
                                    hhProfileDays[day] = 7;
                                int dayOfWeek = (int)currentTime.DayOfWeek;

                                if (dayOfWeek == hhProfileDays[day])
                                {
                                    isProfileDay = true;
                                }
                            }

                            if (isProfileDay)  //if current day is profile day then check the time
                            {
                                if (currentTime >= startTime && currentTime <= endTime)
                                {
                                    isValidProfile = true;
                                }
                            }
                        }
                        if (isValidProfile)
                        {
                            retVal = true;
                            break;
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadHHProfileInfo " + e.Message, e);
                throw;
            }
            return retVal;

        }

        private List<int> LoadDaysInfoForSelectedProfile(int hhProfileKey, DateTime profileDate)
        {
            List<int> hhProfileDays = new List<int>();
            try
            {
                FbCommand command = dbQueries.LoadDaysInfoForSelectedProfile(connection, transaction, hhProfileKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        hhProfileDays.Add(Convert.ToInt32(getReaderColumnValue(reader, "HAPPYHOURDAYS_KEY", 0)));
                        //DateTime currentTime = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second, 000);
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadDaysInfoForSelectedProfile " + e.Message, e);
                throw;
            }
            return hhProfileDays;
        }

        private bool IsTableMarkedForOnlineordering(ref int tableNumber, ref string tableName)
        {
            bool isTableOOMarked = false;
            try
            {
                FbCommand selectTableCommand = dbQueries.IsTableMarkedForOnlineordering(connection, transaction, tableNumber, tableName);
                using (FbDataReader reader = selectTableCommand.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        tableNumber = reader.GetInt32(reader.GetOrdinal("TABLE_NUMBER"));
                        tableName = reader.GetString(reader.GetOrdinal("TABLE_NAME"));
                        isTableOOMarked = true;
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableMarkedForOnlineordering extracting tablenumber " + e.Message, e);
                throw;
            }
            return isTableOOMarked;
        }
        public void AddWaiterTerminal(string terminalName, string deviceId)
        {
            try
            {
                long profileKey = GenerateKey("PROFILE");
                FbCommand command = dbQueries.InsertProfileForWaiterApp(connection, transaction, profileKey, terminalName);
                command.ExecuteNonQuery();
                long deviceKey = GenerateKey("DEVICES");
                command = dbQueries.InsertTerminalForWaiterApp(connection, transaction, deviceKey, profileKey, terminalName, deviceId);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in AddWaiterTerminal adding Waiter Terminal " + e.Message, e);
                throw;
            }
            
        }
        public void AddWaiterStaff()
        {
            try
            {
                // Check there should not be any conflict with same name present in DB either created by OO code or POS code
                long contactKey = GenerateKey("CONTACTS");
                FbCommand command = dbQueries.InsertStaffForWaiterApp(connection, transaction, contactKey);
                command.ExecuteNonQuery();

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in AddWaiterStaff adding Staff " + e.Message, e);
                throw;
            }

        }
        /*public bool ValidateTable(string tableNo, ref string tableNameFromDB, ref int tableNoFromDB)
        {
            bool retValue = false;
            int tableNoContainer = 0;
            string tableNameContainer = "";

            try
            {
                bool isTableNumberInteger = int.TryParse(tableNo, out tableNoContainer);
                if (!isTableNumberInteger)
                    tableNameContainer = tableNo;
                FbCommand selectTableCommand = dbQueries.IsTableAvailable(connection, transaction, tableNoContainer, tableNameContainer);
                using (FbDataReader reader = selectTableCommand.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        tableNoFromDB = reader.GetInt32(reader.GetOrdinal("TABLE_NUMBER"));
                        tableNameFromDB = reader.GetString(reader.GetOrdinal("TABLE_NAME"));
                        retValue = true;
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckIfTableExist " + e.Message, e);
                throw;
            }
            return retValue;

        }*/
        public bool ValidateTable(string tableNo, ref string tableNameFromDB, ref int tableNoFromDB)
        {
            bool retValue = false;
            try
            {
                int containerNumber = 0;
                string tableName = "";
                int containerNumberAux = 0;
                string containerNameAux = "";
                bool isTableNumberInteger = int.TryParse(tableNo, out containerNumber);

                if (isTableNumberInteger)
                    containerNumberAux = containerNumber;
                else
                    containerNameAux = tableName = tableNo;

                bool isTabOrder = false;
                if (IsFloorPlanEnabled())
                {
                    if (CheckTableExistAndGetTableInfo(ref containerNumber, ref tableName))
                    {
                        tableNoFromDB = containerNumber;
                        tableNameFromDB = tableName;
                        retValue = true;
                    }
                    else
                    {
                        isTabOrder = true;
                    }
                }
                else
                {
                    if (containerNumberAux < 1 || containerNumberAux >= 100)
                    {
                        retValue = false;
                    }
                    else
                    {
                        tableNameFromDB = "Table #" + tableNo;
                        tableNoFromDB = containerNumberAux;
                        retValue = true;
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckIfTableExist " + e.Message, e);
                throw;
            }
            return retValue;
        }
        public bool ValidateItemsInOrders(List<ApiOrderItemViewModel> OrderViewModelList, ref string notFoundItemName, ref string notFoundItemSizeName)
        {
            bool retValue = false;
            long itemSizeUniqueKey = 0;

            try
            {
                foreach (var item in OrderViewModelList)
                {
                    foreach (var itemSize in item.OrderItemSizes)
                    {
                        long.TryParse(itemSize.ItemSizeUniqueId, out itemSizeUniqueKey);
                        if (!CheckIfItemExist(itemSizeUniqueKey))
                        {
                            retValue = false;
                            notFoundItemSizeName = itemSize.Name;
                            notFoundItemName = item.Name;
                            break;
                        }
                        else
                        {
                            retValue = true;
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ValidateItemsInOrders " + e.Message, e);
                throw;
            }
            return retValue;

        }
        public bool CheckIfItemExist(long itemSizeUniqueKey)
        {
            bool retValue = false;

            try
            {
                FbCommand command = dbQueries.IsItemAvailable(connection, transaction, itemSizeUniqueKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        retValue = true;
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckIfItemExist " + e.Message, e);
                throw;
            }
            return retValue;

        }
        public string SearchOrderJSONFile(string orderGUID)
        {
            string fileName = "";
            try
            {
                string path = System.IO.Path.GetDirectoryName(
                System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                if (path.Contains(@"file:\"))
                {
                    path = path.Replace(@"file:\", "");
                }
                path = Path.Combine(path, "logs", "Pending Orders");

                string[] dirs = Directory.GetFiles(@path, orderGUID + "*");
                foreach (string dir in dirs)
                {
                    fileName = dir;
                    break;
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SearchOrderJSONFile " + e.Message, e);
                throw;
            }
            return fileName;

        }
        public ApiSiteOrderViewModel GetOrderJSONFile(string filePath)
        {
            ApiSiteOrderViewModel siteOrderViewModelList = new ApiSiteOrderViewModel();
            try
            {
                using (StreamReader readJSON = new StreamReader(filePath))
                {
                    string orderJSON = readJSON.ReadToEnd();
                    siteOrderViewModelList = JsonUtility.Deserialize<ApiSiteOrderViewModel>(orderJSON);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetOrderJSONFile " + e.Message, e);
                throw;
            }
            return siteOrderViewModelList;
        }
        public void MoveOrderJSONFile(string sourceFilePath)
        {
            string destinationFilePath = "";
            try
            {
                //Add code to check dir before moving
                destinationFilePath = sourceFilePath;
                destinationFilePath = destinationFilePath.Replace(@"Pending Orders", "Successful Orders");

                File.Move(@sourceFilePath, @destinationFilePath);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in MoveOrderJSONFile " + e.Message, e);
                throw;
            }

        }
        public void ProcessOrders(ref List<ApiSiteOrderViewModel> siteOrderViewModelList, List<string> stringList)
        {
            int tableNoFromDB = 0;
            string tableNameFromDB = "";
            try
            {
                foreach (var siteOrderViewModel in siteOrderViewModelList)
                {

                    if (siteOrderViewModel.UserType == OnlineOrdering.Enum.UserType.Staff)
                    {
                        stringList.Add("Before Validating Table                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                        if (ValidateTable(siteOrderViewModel.ContainerNumber, ref tableNameFromDB, ref tableNoFromDB))
                        {
                            string notFoundItemName = "";
                            string notFoundItemSizeName = "";
                            stringList.Add("After Validating Table                             ");
                            if (ValidateItemsInOrders(siteOrderViewModel.OrderItems, ref notFoundItemName, ref notFoundItemSizeName))
                            {
                                //Assigning Correct Table Number And Table Name
                                siteOrderViewModel.ContainerName = tableNameFromDB;
                                siteOrderViewModel.ContainerNumber = tableNoFromDB.ToString();
                                //Perfoming Pre-Checks Before Inserting Processing Order
                                PerformPreRequisiteWaiterAppOperation(stringList, siteOrderViewModel.ApiOrderDevicesViewModel.MacAddress, siteOrderViewModel.ApiOrderDevicesViewModel.DeviceName);
                                stringList.Add("Validating Items In Orders                         ");
                                if (siteOrderViewModel.PaymentType == OnlineOrdering.Enum.PaymentType.PayNow)
                                {
                                    //Order Processing - Insert orders details in tables
                                    stringList.Add("Before ArchiveTransaction call                     ");
                                    if (ArchiveTransaction(siteOrderViewModel, stringList))
                                    {
                                        stringList.Add("After ArchiveTransaction call                      ");
                                        siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.IsConfirmed;
                                        siteOrderViewModel.IsConfirmed = true;
                                    }
                                    else
                                    {
                                        siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.PosPaymentError;
                                        stringList.Add("Exception in ArchiveTransaction                    " + DateTime.Now.ToString("hh:mm:ss tt"));
                                        throw new Exception("Error while Saving Order and Payment into DB.");
                                    }

                                }
                                else
                                {
                                    if (AddRecords(siteOrderViewModel, true))
                                    {
                                        stringList.Add("After AddRecords                                   ");
                                        siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.IsConfirmed;
                                        siteOrderViewModel.IsConfirmed = true;
                                    }
                                    else
                                    {
                                        siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.PosOrderError;
                                        stringList.Add("Exception in Adding Records                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                                        throw new Exception("Error while Saving Order into DB.");
                                    }

                                }

                            }
                            else
                            {
                                siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.ItemNotFound;
                                siteOrderViewModel.ErrorContent = GetErrorContent(notFoundItemName, notFoundItemSizeName);
                                stringList.Add("Exception while Validating Items in orders         " + DateTime.Now.ToString("hh:mm:ss tt"));
                                throw new Exception("Items ordered are not available.");
                            }
                        }
                        else
                        {
                            siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.InvalidTable;
                            stringList.Add("Exception while Validating Table                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                            throw new Exception("Invalid Table.");
                        }

                    }
                    else
                    {
                        if (AddRecords(siteOrderViewModel, false))
                        {
                            siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.IsConfirmed;
                            siteOrderViewModel.IsConfirmed = true;
                        }
                        else
                        {
                            siteOrderViewModel.OrderStatus = OnlineOrdering.Enum.OrderStatus.PosOrderError;
                            throw new Exception("Error while Saving Order into DB.");
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ProcessOrders " + e.Message, e);
                stringList.Add("Exception in Process order                         " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                throw;
            }

        }
        public bool ArchiveTransaction(ApiSiteOrderViewModel siteOrderViewModel, List<string> stringList)
        {
            bool retVaule = false;
            try
            {
                long invoiceNumber = 0;
                long orderSecurityRef = GetNextSecurityRef();
                long BillSecurityRef = GetNextSecurityRef();
                long dayArcBillKey = ArchiveBill(siteOrderViewModel, BillSecurityRef, stringList,ref invoiceNumber);
                //Archive Data For Patron Count
                stringList.Add("Before ArchivePatronCount call                     " + DateTime.Now.ToString("hh:mm:ss tt"));
                ArchivePatronCount(dayArcBillKey, stringList);
                stringList.Add("Before ArchiveReferences call                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                //Archive Data For References 
                ArchiveReferences(dayArcBillKey, stringList);
                stringList.Add("Before ArchiveOrder call                           " + DateTime.Now.ToString("hh:mm:ss tt"));
                //Archive Data For DayArchiveOrder and DayArcOrderTaxes
                ArchiveOrder(dayArcBillKey, siteOrderViewModel, BillSecurityRef, stringList);
                //Processing Data for Security Table
                stringList.Add("Before ProcessSecurity call                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                ProcessSecurity(siteOrderViewModel.ApiOrderDevicesViewModel.DeviceName, orderSecurityRef, BillSecurityRef, stringList);
                //Adding Record In OnlineOrder
                AddRecordInOnlineOrder(siteOrderViewModel, invoiceNumber);
                
                retVaule = true;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ArchiveTransaction " + e.Message, e);
                stringList.Add("Exception in Archive Transaction                   " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                throw;
            }
            return retVaule;
        }
        public long ArchiveBill(ApiSiteOrderViewModel siteOrderViewModel, long securityRef, List<string> stringList,ref long invoiceNo)
        {
            long dayArcBillKey = 0;
            long invoiceNumber = 0;
            DayArcBillAttributes dayArcBillRow = new DayArcBillAttributes();
            DayArcBillPayAttributes dayArcBillPayRow = new DayArcBillPayAttributes();
            try
            {
                dayArcBillKey = GenerateKey("DAYARCBILL");
                invoiceNumber = GenerateKey("INVOICENUMBER");
                invoiceNo = invoiceNumber;
                //Creating Row For DayArcBill
                stringList.Add("Creating Row For DayArcBill                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                dayArcBillRow = CreateDayArcBillRow(dayArcBillKey, invoiceNumber, siteOrderViewModel, securityRef);
                //Inserting Data Into DayAcBill
                stringList.Add("Inserting Data Into DayAcBill                      " + DateTime.Now.ToString("hh:mm:ss tt"));
                ExecuteDayArcBillQuery(dayArcBillRow);
                foreach (var orderPayment in siteOrderViewModel.ApiSiteOrderPaymentViewModels)
                {
                    //Creating Row for DayArcBillPay
                    stringList.Add("Creating Row for DayArcBillPay                     " + DateTime.Now.ToString("hh:mm:ss tt"));
                    dayArcBillPayRow = CreateDayArcBillPayRow(dayArcBillKey, orderPayment, false);
                    //Inserting Data Into DayArcBillPay
                    stringList.Add("Before ExecuteDayArcBillPayQuery                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                    ExecuteDayArcBillPayQuery(dayArcBillPayRow);
                }
                ApiSiteOrderPaymentViewModel adjustmentRow = new ApiSiteOrderPaymentViewModel();
                //Creating Cash Adjustment Row For Change
                stringList.Add("Creating Cash Adjustment Row For Change            " + DateTime.Now.ToString("hh:mm:ss tt"));
                dayArcBillPayRow = CreateDayArcBillPayRow(dayArcBillKey, adjustmentRow, true);
                //Insert Cash Adjustment Row For Change
                stringList.Add("Inserting Cash Adjustment Row For Change           " + DateTime.Now.ToString("hh:mm:ss tt"));
                ExecuteDayArcBillPayQuery(dayArcBillPayRow);
            }
            catch (Exception e)
            {
                stringList.Add("Exception in Archive Bill                          " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(@"in ArchiveBill " + e.Message, e);
                throw;
            }
            return dayArcBillKey;
        }
        public DayArcBillAttributes CreateDayArcBillRow(long dayArcBillKey, long invoiceNumber, ApiSiteOrderViewModel siteOrderViewModel, long securityRef)
        {
            DayArcBillAttributes dayArcBillRow = new DayArcBillAttributes();
            try
            {
                dayArcBillRow.ArcBillId = dayArcBillKey;
                dayArcBillRow.TerminalName = siteOrderViewModel.ApiOrderDevicesViewModel.DeviceName;
                dayArcBillRow.StaffName = "WAITER";//Need to Confirm
                dayArcBillRow.Total = siteOrderViewModel.TotalAmount;
                dayArcBillRow.Discount = 0;
                dayArcBillRow.PatronCount = 1;
                dayArcBillRow.SecurityRef = securityRef;
                dayArcBillRow.RoundingAdjustment = 0;
                dayArcBillRow.BilledLocation = siteOrderViewModel.Location;//Need to confirm
                dayArcBillRow.InvoiceNumber = invoiceNumber;
                dayArcBillRow.SaleType = 2; // Need to Confirm
                dayArcBillRow.RoundingAdjustment = 0;
                dayArcBillRow.IsPrintRequired = true;
                dayArcBillRow.OnlinOrderId = siteOrderViewModel.OrderId;
                dayArcBillRow.OrderGuid = siteOrderViewModel.OrderGuid;
                dayArcBillRow.ApplicationType = Enum.AppType.devWaiter;
                dayArcBillRow.SiteId = siteOrderViewModel.SiteId;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateDayArcBillRow " + e.Message, e);
                throw;
            }
            return dayArcBillRow;
        }
        public void ExecuteDayArcBillQuery(DayArcBillAttributes dayArcBillRow)
        {
            try
            {
                FbCommand command = dbQueries.InsertDataIntoDayArcBill(connection, transaction, dayArcBillRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExecuteDayArcBillQuery " + e.Message, e);
                throw;
            }
        }
        public DayArcBillPayAttributes CreateDayArcBillPayRow(long dayArcBillKey, ApiSiteOrderPaymentViewModel orderPayment, bool isChangeAdjRow)
        {
            DayArcBillPayAttributes dayArcBillPayRow = new DayArcBillPayAttributes();
            try
            {
                dayArcBillPayRow.DayArcBillPayId = GenerateKey("DAYARCBILLPAY");
                dayArcBillPayRow.DayArcBillId = dayArcBillKey;
                dayArcBillPayRow.PayType = "WAITERAPP";
                dayArcBillPayRow.Rounding = 0;
                if (isChangeAdjRow)
                {
                    dayArcBillPayRow.SubTotal = 0;
                    dayArcBillPayRow.Note = "Total Change.";
                    dayArcBillPayRow.Tip = 0;
                }
                else
                {
                    dayArcBillPayRow.SubTotal = orderPayment.Amount;
                    dayArcBillPayRow.Note = orderPayment.Note;
                    dayArcBillPayRow.Tip = orderPayment.Tip;
                }
                dayArcBillPayRow.PayTypeDetails = "";
                dayArcBillPayRow.Properties = "-3-19-";
                dayArcBillPayRow.PaymentCardType = orderPayment.CardType;
                dayArcBillPayRow.PayGroup = ""; // Need to Confirm

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateDayArcBillPayRow " + e.Message, e);
                throw;
            }
            return dayArcBillPayRow;
        }
        public void ExecuteDayArcBillPayQuery(DayArcBillPayAttributes dayArcBillPayRow)
        {
            try
            {
                FbCommand command = dbQueries.InsertDataIntoDayArcBillPay(connection, transaction, dayArcBillPayRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExecuteDayArcBillPayQuery " + e.Message, e);
                throw;
            }
        }
        public void ArchivePatronCount(long dayArcBillKey, List<string> stringList)
        {
            try
            {
                stringList.Add("Inside ArchivePatronCount                          ");
                long patronCountKey = GenerateKey("DAYPATRONCOUNT");
                FbCommand command = dbQueries.InsertDataIntoDayPatronCount(connection, transaction, patronCountKey, dayArcBillKey, stringList);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ArchivePatronCount " + e.Message, e);
                stringList.Add("Exception in ArchivePatronCount is                 " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                throw;
            }
        }
        public void ArchiveReferences(long dayArcBillKey, List<string> stringList)
        {
            try
            {
                stringList.Add("Inside ArchiveReferences                           " + DateTime.Now.ToString("hh:mm:ss tt"));
                long dayArcRefKey = GenerateKey("DAYARCREF");
                FbCommand command = dbQueries.InsertDataIntoDayArcRef(connection, transaction, dayArcRefKey, dayArcBillKey, stringList);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                stringList.Add("Exception in ArchiveReferences is                  " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(@"in ArchiveReferences " + e.Message, e);
                throw;
            }
        }
        public void ArchiveOrder(long arcBillKey, ApiSiteOrderViewModel siteOrderViewModel, long securityRef, List<string> stringList)
        {
            DayArchiveAttributes dayArchiveRow = new DayArchiveAttributes();

            try
            {
                stringList.Add("ArchiveOrder                                       " + DateTime.Now.ToString("hh:mm:ss tt"));
                int tableNumber = 0;
                int.TryParse(siteOrderViewModel.ContainerNumber, out tableNumber);
                dayArchiveRow.ArcBillId = arcBillKey;
                dayArchiveRow.TerminalName = siteOrderViewModel.ApiOrderDevicesViewModel.DeviceName;
                dayArchiveRow.TableNumber = tableNumber;
                dayArchiveRow.TableName = siteOrderViewModel.ContainerName;
                dayArchiveRow.TabName = "Guest 1";//Need to confirm
                dayArchiveRow.OrderLocation = siteOrderViewModel.Location;// Need to confirm
                dayArchiveRow.OrderGuid = siteOrderViewModel.OrderGuid;
                dayArchiveRow.OnlineOrderId = siteOrderViewModel.OrderId;
                dayArchiveRow.SecurityRef = securityRef;
                dayArchiveRow.OrderType = 1;//Need to Confirm
                dayArchiveRow.LoyaltyName = "";
                dayArchiveRow.LoyaltyId = 0;
                dayArchiveRow.Discount = 0;
                foreach (var item in siteOrderViewModel.OrderItems)
                {
                    dayArchiveRow.ItemName = item.Name;
                    dayArchiveRow.ItemShortName = "";//Neew to Confirm
                    foreach (var itemSize in item.OrderItemSizes)
                    {
                        //dayArchiveRow.ItemShortName = "";//Need to Confirm
                        dayArchiveRow.SizeName = itemSize.Name;
                        dayArchiveRow.ServerName = "";
                        dayArchiveRow.Note = "";
                        dayArchiveRow.BasePrice = itemSize.BasePrice;
                        dayArchiveRow.PriceAdjust = 0;
                        dayArchiveRow.Price = itemSize.Price;
                        dayArchiveRow.PriceLevel0 = itemSize.Price;
                        dayArchiveRow.PriceLevel1 = itemSize.Price;
                        dayArchiveRow.Qty = itemSize.Quantity;
                        dayArchiveRow.PriceIncl = itemSize.PriceInclusive;
                        dayArchiveRow.TimeKey = 0;//Need to Confirm
                        dayArchiveRow.Redeemed = 0;
                        dayArchiveRow.ItemSizeIdentifier = itemSize.ItemSizeUniqueId;
                        dayArchiveRow.ArchiveId = GenerateKey("DAYARCHIVE");
                        //Loading Item Info
                        stringList.Add("Loading Item Info                                  ");
                        LoadItemInfoForDayArchiveRow(ref dayArchiveRow, itemSize.ItemSizeUniqueId, stringList);
                        //Insert row into DayArchive
                        stringList.Add("Inserting row into DayArchive                      ");
                        ExecuteDayArchiveQuery(dayArchiveRow);
                        //Insert row into DayArcCategory
                        ExecuteDayArcCategoryQuery(dayArchiveRow.ArchiveId, dayArchiveRow.CategoryId);
                        stringList.Add("Inserting row into DayArcOrderTaxes                ");
                        //Insert row into DayArcOrderTaxes
                        if (itemSize.OrderItemSizeTaxProfiles != null)
                            ExcecuteDayArcOrderTaxesQuery(dayArchiveRow.ArchiveId, itemSize.OrderItemSizeTaxProfiles);
                    }

                }

            }
            catch (Exception e)
            {
                stringList.Add("Exception in ArchiveOrder is                       " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(@"in ArchiveOrder " + e.Message, e);
                throw;
            }
        }
        public void LoadItemInfoForDayArchiveRow(ref DayArchiveAttributes dayArchiveRow, string itemSizeUniqueId, List<string> stringList)
        {
            try
            {
                int itemSizeIdentifier = 0;
                int.TryParse(itemSizeUniqueId, out itemSizeIdentifier);
                stringList.Add("Inside LoadItemInfoForDayArchiveRow                " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("ArchiveOrder                                       " + DateTime.Now.ToString("hh:mm:ss tt"));
                FbCommand command = dbQueries.GetItemInfoForDayArchiveRow(connection, transaction, itemSizeIdentifier);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        dayArchiveRow.MenuName = reader.GetString(reader.GetOrdinal("Menu_Name"));
                        dayArchiveRow.CourseName = reader.GetString(reader.GetOrdinal("Course_Name"));
                        dayArchiveRow.ItemId = reader.GetInt32(reader.GetOrdinal("Item_Key"));
                        dayArchiveRow.GstPercent = reader.GetDouble(reader.GetOrdinal("GST_Percent"));
                        dayArchiveRow.Cost = reader.GetDouble(reader.GetOrdinal("Cost"));
                        dayArchiveRow.CostGstPercent = reader.GetDouble(reader.GetOrdinal("Cost_GST_Percent"));
                        dayArchiveRow.PointsPercent = reader.GetDouble(reader.GetOrdinal("Points_Percent"));
                        dayArchiveRow.ItemCategory = reader.GetString(reader.GetOrdinal("Category"));
                        dayArchiveRow.CategoryId = reader.GetInt32(reader.GetOrdinal("Category_Key"));
                        dayArchiveRow.ThirdPartyCodesId = reader.GetInt32(reader.GetOrdinal("ThirdPartyCodes_Key"));
                        dayArchiveRow.ServingCousesId = reader.GetInt32(reader.GetOrdinal("ServingCourses_Key"));
                    }
                }

            }
            catch (Exception e)
            {
                stringList.Add("Exception in LoadItemInfoForDayArchiveRow          " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(@"in LoadItemInfoForDayArchiveRow " + e.Message, e);
                throw;
            }
        }
        public void ExecuteDayArchiveQuery(DayArchiveAttributes dayArchiveRow)
        {
            try
            {
                FbCommand command = dbQueries.InsertDataIntoDayArchive(connection, transaction, dayArchiveRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExecuteDayArchiveQuery " + e.Message, e);
                throw;
            }
        }
        public void ExecuteDayArcCategoryQuery(long archiveKey, long categoryKey)
        {
            try
            {
                FbCommand command = dbQueries.InsertDataIntoDayArcCategory(connection, transaction, archiveKey, categoryKey);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExecuteDayArcCategoryQuery " + e.Message, e);
                throw;
            }
        }
        public void ExcecuteDayArcOrderTaxesQuery(long dayArchiveKey, List<ApiOrderItemSizeTaxProfileViewModel> itemSizeTaxInfo)
        {
            try
            {
                FbCommand command;
                DayArcOrderTaxesAttributes dayArcOrderTaxesRow = new DayArcOrderTaxesAttributes();
                foreach (var items in itemSizeTaxInfo)
                {
                    dayArcOrderTaxesRow.ArcOrderTaxesID = GenerateKey("DAYARCORDERTAXES");
                    dayArcOrderTaxesRow.ArchiveId = dayArchiveKey;
                    dayArcOrderTaxesRow.TaxName = items.Name;
                    dayArcOrderTaxesRow.TaxType = items.TaxProfileType;
                    dayArcOrderTaxesRow.TaxValue = items.Value;
                    //Inserting Row into DayArcOrderTaxes
                    command = dbQueries.InsertDataIntoDayArcOrderTaxes(connection, transaction, dayArcOrderTaxesRow);
                    command.ExecuteNonQuery();
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExcecuteDayArcOrderTaxesQuery " + e.Message, e);
                throw;
            }

        }
        public void SaveOrderJSON(ApiSiteOrderViewModel siteOrderViewModel)
        {
            try
            {
                string orderGUID = siteOrderViewModel.OrderGuid;
                var requestData = JsonUtility.Serialize<ApiSiteOrderViewModel>(siteOrderViewModel);

                string path = System.IO.Path.GetDirectoryName(
                System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                string location = Path.Combine(path, "logs");
                if (location.Contains(@"file:\"))
                {
                    location = location.Replace(@"file:\", "");
                }
                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string folderName = "Pending Orders";
                location = Path.Combine(location, folderName);

                if (!Directory.Exists(location))
                    Directory.CreateDirectory(location);

                string name = orderGUID + "_" + DateTime.Now.ToString("ddMMyyyyHHmmss") + ".json";
                string fileName = Path.Combine(location, name);

                if (!File.Exists(fileName))
                {
                    using (StreamWriter sw = File.CreateText(fileName))
                    {
                        sw.WriteLine(requestData.ToString());
                    }
                }
                //stringList.Add("File saved with new Details for orderGUID " + orderGUID);

            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
        }
        public string GetSyndicateCode()
        {
            string syndicateCode = "";
            try
            {
                FbCommand command = dbQueries.GetSyndicateCodeQuery(connection, transaction);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        syndicateCode = reader.GetString(reader.GetOrdinal("NAME"));
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
            return syndicateCode;
        }
        public void ProcessSecurity(string terminalName, long orderSecurityRef, long billSecurityRef, List<string> stringList)
        {
            try
            {
                stringList.Add("Processing Security at                             " + DateTime.Now.ToString("hh:mm:ss tt"));
                SecurityAttributes securityRow = new SecurityAttributes();
                string contactInitials = "";
                long contactId = 0;
                stringList.Add("Before LoadSecurityRowInfo call                    ");
                LoadSecurityRowInfo("WAITER", ref contactInitials, ref contactId);
                //Creating And Inserting Row In Security For Order BY
                securityRow = CreateSecurityRow(terminalName, orderSecurityRef, false, contactInitials, contactId);
                stringList.Add("Before ExecuteSecurityQuery call                   ");
                ExecuteSecurityQuery(securityRow);
                //Creating And Inserting Row In Security For Billed BY
                securityRow = CreateSecurityRow(terminalName, billSecurityRef, true, contactInitials, contactId);
                ExecuteSecurityQuery(securityRow);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in Process Security                      " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }

        }
        public SecurityAttributes CreateSecurityRow(string terminalName, long securityRef, bool isBill, string contactInitials, long contactId)
        {
            SecurityAttributes securityRow = new SecurityAttributes();
            try
            {
                securityRow.SecurityId = GenerateKey("SECURITY_KEY");
                securityRow.SecurityRef = securityRef;
                if (isBill)
                    securityRow.SecurityEvent = "Billed by";
                else
                    securityRow.SecurityEvent = "Ordered by";
                securityRow.FromVal = "WAITER";
                securityRow.ToVal = contactInitials;
                securityRow.Note = "";
                securityRow.TerminalName = terminalName;
                securityRow.UserId = contactId;

            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
            return securityRow;

        }

        public void LoadSecurityRowInfo(string contactName, ref string contactInitials, ref long contactId)
        {
            try
            {
                FbCommand command = dbQueries.GetSecurityInfoQuery(connection, transaction, contactName);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        contactInitials = reader.GetString(reader.GetOrdinal("INITIALS"));
                        contactId = reader.GetInt32(reader.GetOrdinal("CONTACTS_KEY"));
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadSecurityRowInfo " + e.Message, e);
                throw;
            }
        }

        public void ExecuteSecurityQuery(SecurityAttributes securityRow)
        {

            try
            {
                FbCommand command = dbQueries.InsertDataIntoSecurity(connection, transaction, securityRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoSecurity " + e.Message, e);
                throw;
            }
        }
                 
        public OnlineOrderAttributes CreateOnlineOrderRow(ApiSiteOrderViewModel siteOrderViewModel, long invoiceNo)
        {
            OnlineOrderAttributes onlineOrderRow = new OnlineOrderAttributes();
            try
            {
                long onlineOrderKey = 0;
                onlineOrderKey = GenerateKey("ONLINEORDERS_ID");

                onlineOrderRow.InvoiceNumber = invoiceNo ;
                onlineOrderRow.OnlineOrderId = onlineOrderKey;
                onlineOrderRow.TerminalName = siteOrderViewModel.ApiOrderDevicesViewModel.DeviceName;
                onlineOrderRow.IsPosted = false;
                onlineOrderRow.AppType = DTO.Enum.AppType.devWaiter;
                onlineOrderRow.ProfileId = GetProfileKey(siteOrderViewModel.ApiOrderDevicesViewModel.MacAddress);
                if (siteOrderViewModel.ApiSiteOrderPaymentViewModels != null)
                {
                    foreach (var orderPaymentViewModel in siteOrderViewModel.ApiSiteOrderPaymentViewModels)
                    {
                        if (orderPaymentViewModel.PaymentReceipt != null)
                        {
                            onlineOrderRow.EftposReceipt = orderPaymentViewModel.PaymentReceipt;
                            break;
                        }
                    }
                }
                else
                {
                    onlineOrderRow.EftposReceipt = null;
 
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateOnlineOrderRow " + e.Message, e);
                throw;
            }
            return onlineOrderRow;
        }

        public void ExecuteOnlineOrderQuery(OnlineOrderAttributes onlineOrderRow)
        {

            try
            {
                FbCommand command = dbQueries.InsertDataIntoOnlineOrders(connection, transaction, onlineOrderRow);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ExecuteOnlineOrderQuery " + e.Message, e);
                throw;
            }
        }
        public long GetProfileKey(string deviceKey)
        {
            long profileKey = 0;
            try
            {
                FbCommand command = dbQueries.GetProfileKeyQuery(connection, transaction, deviceKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        profileKey = reader.GetInt32(reader.GetOrdinal("PROFILE_KEY"));
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetProfileKey " + e.Message, e);
                throw;
            }

            return profileKey;
        }

        public string CheckIfWaiterTerminalExist(string deviceId)
        {
            string result = "";
            try
            {
                FbCommand command = dbQueries.CheckIfWaiterAppTerminalExist(connection, transaction, deviceId);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        result = reader.GetString(reader.GetOrdinal("DEVICE_NAME"));
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return result;
        }


        public void UpdateWaiterTerminal(string deviceKey, string terminalName)
        {
            try
            {
                FbCommand command = dbQueries.UpdateProfileForWaiterApp(connection, transaction, deviceKey, terminalName);
                command.ExecuteNonQuery();
                command = dbQueries.UpdateTerminalForWaiterApp(connection, transaction, deviceKey, terminalName);
                command.ExecuteNonQuery();

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateAppZedRow " + e.Message, e);
                throw;
            }
        }

        public bool CheckIfZedRequestExist(string terminalName)
        {
            bool result = false;
            try
            {
                FbCommand command = dbQueries.CheckIfZedRequestExistQuery(connection, transaction, terminalName);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        result = true;
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return result;
        }

        public void InsertAppZedRow(string terminalName, string deviceKey)
        {
            try
            {
                long profileKey = GetProfileKey(deviceKey);
                long appZedKey = GenerateKey("APPZEDSTATUS_ID");
                FbCommand command = dbQueries.InsertAppZedRowQuery(connection, transaction, terminalName, appZedKey, profileKey);
                command.ExecuteNonQuery();
                
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateAppZedRow " + e.Message, e);
                throw;
            }
        }

        public void AddRecordInOnlineOrder(ApiSiteOrderViewModel siteOrderViewModel, long invoiceNumber)
        {
            try
            {
                OnlineOrderAttributes onlineOrderAttributes = new OnlineOrderAttributes();
                onlineOrderAttributes = CreateOnlineOrderRow(siteOrderViewModel, invoiceNumber);
                ExecuteOnlineOrderQuery(onlineOrderAttributes);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in AddRecordInOnlineOrder " + e.Message, e);
                throw;
            }
        }

        public bool CheckIfWaiterStaffExist()
        {
            bool result = false;
            try
            {
                FbCommand command = dbQueries.CheckIfWaiterStaffExistQuery(connection, transaction);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        result = true;
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return result;
        }
        public List<string> GetErrorContent(string notFoundItemName, string notFoundItemSizeName)
        {
            List<string> errorContent = new List<string>();
            try
            {
                string content = notFoundItemName + " ( " + notFoundItemSizeName + " ) ";
                errorContent.Add(content);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return errorContent;
        }

        public void PerformPreRequisiteWaiterAppOperation(List<string> stringList, string deviceId, string terminalName)
        {
            try
            {
                stringList.Add("Checking If Waiter Terminal Exist              ");
                string oldTerminalName = CheckIfWaiterTerminalExist(deviceId);
                if (oldTerminalName.Trim() == "")
                {
                    stringList.Add("Adding Waiter Terminal                         ");
                    AddWaiterTerminal(terminalName, deviceId);
                }
                else
                {
                    if (!string.Equals(oldTerminalName, terminalName))
                    {
                        //Updating DeviceName In Devices And Profile Tables
                        UpdateWaiterTerminal(deviceId, terminalName);

                        //Updating Terminal Name In Orders Table
                        UpdateTerminalNameInOrders(oldTerminalName, terminalName);
                    }
                }

                stringList.Add("Checking If Waiter Staff Exist                 ");
                bool IsWaiterStaffExist = CheckIfWaiterStaffExist();
                if (!IsWaiterStaffExist)
                {
                    stringList.Add("Adding Waiter Staff                            ");
                    AddWaiterStaff();
                }
                //Check If Payment Type For Waiter App Is Created Or Not
                if(!CheckIfWaiterAppPaymentTypeExist())
                {
                    AddWaiterAppPaymentType();
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
        }

        public void UpdateTerminalNameInOrders(string oldTerminalName, string newTerminalName)
        {
            try
            {
                FbCommand command = dbQueries.UpdateTerminalNameInOrdersQuery(connection, transaction, oldTerminalName, newTerminalName);
                command.ExecuteNonQuery();
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
        }
        public bool CheckIfWaiterAppPaymentTypeExist()
        {
            bool retValue = false;
            try
            {
                FbCommand command = dbQueries.CheckIfWaiterAppPaymentTypeExistQuery(connection, transaction);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        retValue = true;
                    }
                }

            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return retValue;
        }
        public void AddWaiterAppPaymentType()
        {
            try
            {
                long paymentTypeKey = GenerateKey("PAYMENTTYPES");
                FbCommand command = dbQueries.InsertWaiterAppPaymentTypeQuery(connection, transaction, paymentTypeKey);
                command.ExecuteNonQuery();
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
        }
        public void CreateApiWaiterAppPosTerminalViewModel(List<string> stringList, ref ApiZedRequestNotificationViewModel apiZedRequestNotificationViewModel, ref string inSyndicateCode)
        {
            try
            {
                apiZedRequestNotificationViewModel.ProcessingTerminal = GetWaiterAppZedProcessingTerminalName();
                if (apiZedRequestNotificationViewModel.ZedRequestStatus != OnlineOrdering.Enum.ZedRequestStatus.ZedRequestInsertionError)
                {
                    if (!CheckIfWaiterAppZedProcessingEnabled())
                    {
                        apiZedRequestNotificationViewModel.ZedRequestStatus = OnlineOrdering.Enum.ZedRequestStatus.ZedRequestNotEnabled;
                    }
                }
                apiZedRequestNotificationViewModel.Message = "";
                inSyndicateCode = GetSyndicateCode();
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                throw;
            }
        }
        public string GetWaiterAppZedProcessingTerminalName()
        {
            string retValue = "";
            try
            {
                FbCommand command = dbQueries.GetWaiterAppZedProcessingTerminalName(connection, transaction);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        retValue = reader.GetString(reader.GetOrdinal("DEVICE_NAME"));
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return retValue;
        }

        public bool CheckIfWaiterAppZedProcessingEnabled()
        {
            bool retValue = false;
            try
            {
                FbCommand command = dbQueries.CheckIfWaiterAppZedProcessingEnabledQuery(connection, transaction);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        retValue = true;
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
            }
            return retValue;
        }
        #endregion
    }

}
