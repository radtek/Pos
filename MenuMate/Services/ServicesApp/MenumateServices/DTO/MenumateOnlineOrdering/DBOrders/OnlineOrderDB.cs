using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using OnlineOrdering.Model.OrderModels;
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

        public void AddRecords(ref List<ApiSiteOrderViewModel> siteOrderViewModelList)
        {
            try
            {

                foreach (var siteOrderViewModel in siteOrderViewModelList)
                {
                    bool test = ValidateItemsInOrders(siteOrderViewModel);
                    string name = SearchOrderJSONFile(siteOrderViewModel.OrderGuid);
                    ApiSiteOrderViewModel orderJSON = GetOrderJSONFile(name);
                    MoveOrderJSONFile(name);
                    try
                    {
                        OrderAttributes orderRow = new OrderAttributes();
                        orderRow.ContainerNumber = 0;
                        orderRow.ContainerType = siteOrderViewModel.ContainerType;
                        orderRow.ContainerName = siteOrderViewModel.ContainerName;

                        if (siteOrderViewModel.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table)
                        {
                            int containerNumber = 0;
                            string tableName = "";
                            bool isTableNumberInteger = int.TryParse(siteOrderViewModel.ContainerNumber, out containerNumber);

                            if (isTableNumberInteger)
                                orderRow.ContainerNumber = containerNumber;
                            else
                                orderRow.TableName = tableName = siteOrderViewModel.ContainerNumber;
                            test = ValidateTable(siteOrderViewModel.ContainerNumber);
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
                        orderRow.TerminalName = siteOrderViewModel.TerminalName;
                        orderRow.TransactionDate = siteOrderViewModel.TransactionDate;
                        orderRow.TransactionType = siteOrderViewModel.TransactionType;
                        orderRow.UserType = siteOrderViewModel.UserType;
                        orderRow.MembershipProfileId = siteOrderViewModel.UserReferenceId; //memberid
                        orderRow.Email = siteOrderViewModel.UserEmailId;
                        orderRow.SiteId = siteOrderViewModel.SiteId;
                        orderRow.OnlinerderId = siteOrderViewModel.OrderId;
                        //to do happy hour implementation

                        //generate tab key if tab not exist..
                        orderRow.TabKey = orderRow.ContainerType == 0 ? GetOrCreateTabForOnlineOrdering(orderRow.ContainerName)
                                            : GetOrCreateTableForOnlineOrdering(orderRow.ContainerNumber, orderRow.ContainerName, orderRow.TableName); //TODo 

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
                        }
                        siteOrderViewModel.IsConfirmed = true;
                        siteOrderViewModel.IsHappyHourApplied = CanHappyHourBeApplied();
                    }
                    catch (Exception ex)
                    {
                        siteOrderViewModel.IsConfirmed = false;
                        siteOrderViewModel.IsHappyHourApplied = false;
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
        public void AddWaiterTerminal(string terminalName)
        {
            try
            {
                long profileKey = GenerateKey("PROFILE");
                FbCommand command = dbQueries.InsertProfileForWaiterApp(connection, transaction, profileKey, terminalName);
                command.ExecuteNonQuery();
                long deviceKey = GenerateKey("DEVICES");
                command = dbQueries.InsertTerminalForWaiterApp(connection, transaction, deviceKey, profileKey, terminalName);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in AddWaiterTerminal adding Waiter Terminal " + e.Message, e);
                throw;
            }
 
        }
        public void AddWaiterStaff(long siteCode)
        {
            try
            {
                long contactKey = GenerateKey("CONTACTS");
                FbCommand command = dbQueries.InsertStaffForWaiterApp(connection, transaction, contactKey ,siteCode);
                command.ExecuteNonQuery();

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in AddWaiterStaff adding Staff " + e.Message, e);
                throw;
            }

        }
        public bool ValidateTable(string tableNo)
        {
            bool retValue = false;
            int tableNoContainer = 0;
            string tableNameContainer = "";
            int tableNoFromDB = 0;
            string tableNameFromDB = "";
            try
            {
                bool isTableNumberInteger = int.TryParse(tableNo, out tableNoContainer);
                if(!isTableNumberInteger)
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
    
        }
        public bool ValidateItemsInOrders(ApiSiteOrderViewModel siteOrderViewModel)
        {
            bool retValue = false;

            try
            {
                foreach (var item in siteOrderViewModel.OrderItems)
                {
                    foreach (var itemSize in item.OrderItemSizes)
                    {
                        if (!CheckIfItemExist(itemSize.OrderItemSizeId))
                        {
                            retValue = false;
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
        public bool CheckIfItemExist(long itemSizeId)
        {
            bool retValue = false;

            try
            {
                FbCommand command = dbQueries.IsItemAvailable(connection, transaction, itemSizeId);
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
                path = Path.Combine(path, "logs");
                path = Path.Combine(path, "Pending Orders");

                string[] dirs = Directory.GetFiles(@path, orderGUID +"*");
                foreach (string dir in dirs)
                {
                    fileName = dir;
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
        public void ProcessOrders()
        {
            bool temp = true;
            try
            {
                if (/*Ordered from WaiterAPP*/temp)
                {
                    if (/*PaymentType is PayNow && Payment is Available*/temp)
                    {
                        //Retrive order from Pendind Orders Dir
                        //If not found request order from WEB using orderGUID
                        //Order Processing - Insert orders details in tables
                        if (/*Insertion Of Order && Insertion of Payments && Receipt Generation == successful*/temp)
                        {
                            //OrderStatus = isConfirmed
                            //Send Response	
                        }
                        else
                        {
                            //OrderStatus = POSError
                        }
                    }
                    if (/*Validation for table*/temp)
                    {
                        if (/*Item Available validation*/temp)
                        {
                            if (/*PayNow && !PaymentNotAvailble*/temp)
                            {
                                //OrderStatus = IsValidated
                                //Save JSON file in Pending Orders 
                                //Send response (Terminal Name Waiter APP)
                            }
                            else if (/*PayLater*/temp)
                            {
                                if (/*Insertion of order (Seating condintion should be ignored)*/temp)
                                {
                                    //OrderStatus = IsConfirmed
                                }
                                else
                                {
                                    //OrderStatus = POSerror
                                }
                            }
                        }
                        else
                        {
                            //OrderStatus = Item Not Available
                        }
                    }
                    else
                    {
                        //OrderStatus = Invalid Table
                    }
                }
                else
                {
                    //call insertorderinDB method
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in ProcessOrders " + e.Message, e);
                throw;
            }
        }

        #endregion
    }

}
