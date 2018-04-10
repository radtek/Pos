using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using FirebirdSql.Data.FirebirdClient;

using MenumateServices.WebMate.InternalClasses.Database;
using System.Diagnostics;

namespace MenumateServices.WebMate.InternalClasses
{
    /// <summary>
    /// 
    /// </summary>
    public class WebOrderDBInfo
    {
        public string GUID { get; set; }
        public string Name { get; set; }
        public string StoreName { get; set; }
        public DateTime OrderDate { get; set; }
        public DateTime ExpectedDate { get; set; }
        public WebOrderDB.WebOrderStatus Status { get; set; }

        public int TabKey { get; set; }
        public int TimeKey { get; set; }
        public int ContactKey { get; set; }

        public string Recipient { get; set; }
        public string StreetNo { get; set; }
        public string StreetName { get; set; }
        public string Suburb { get; set; }
        public string City { get; set; }
        public string Country { get; set; }
        public string Note { get; set; }
        public string Phone { get; set; }

        public string[] Comments { get; set; }
        public bool PaymentRequired { get; set; }

        public decimal OrderTotal { get; set; }

        public string Email { get; set; }
        public string OrderType { get; set; }
    }

    /// <summary>
    /// Represents an item in a web order
    /// </summary>
    public class WebOrderDBItem : WebOrderDBBaseItem
    {
        #region PUBLIC

        public List<WebOrderDBItemSide> ItemSides { get; set; }
        public List<WebOrderDBItemOption> ItemOptions { get; set; }
        public List<WebOrderDBRecipe> ItemRecipes { get; set; }
        public int OwnerWebOrderKey { get; set; }



        #endregion

        #region CONSTRUCTORS

        public WebOrderDBItem()
        {
            ItemSides = new List<WebOrderDBItemSide>();
            ItemOptions = new List<WebOrderDBItemOption>();
            ItemRecipes = new List<WebOrderDBRecipe>();
        }

        #endregion
    }

    /// <summary>
    /// Side item
    /// </summary>
    public class WebOrderDBItemSide : WebOrderDBBaseItem
    {
    }

    /// <summary>
    /// Options for web order item
    /// </summary>
    public class WebOrderDBItemOption
    {
        public int OptionKey { get; set; }
        public long Palm_ID { get; set; }
        public int OptionID { get; set; }
        public string Name { get; set; }
        public string KitchenName { get; set; }
        public bool Enabled { get; set; }
        public bool IsPlus { get; set; }
        public int ForcedMask { get; set; }
        public int Flags { get; set; }
        public int GroupNumber { get; set; }
        public bool PlusOption { get; set; }
        public bool MinusOption { get; set; }
        public bool AllowSkip { get; set; }
        public bool DisallowMuliSelect { get; set; }

        public bool Underlined { get; set; }
        public bool Bold { get; set; }
        public FontSize Width { get; set; }
        public FontSize Height { get; set; }
        public FontType Font { get; set; }
        public FontColor Color { get; set; }
    }

    /// <summary>
    /// 
    /// </summary>
    public enum FontSize
    {
        Normal,
        Double
    }

    /// <summary>
    /// 
    /// </summary>
    public enum FontColor
    {
        Black,
        Red
    }

    /// <summary>
    /// 
    /// </summary>
    public enum FontType
    {
        FontA,
        FontB
    }

    /// <summary>
    /// 
    /// </summary>
    public class WebOrderDBRecipe
    {
        public int RecipeKey { get; set; }
        public decimal Cost { get; set; }
        public double CostGSTPercent { get; set; }
        public string StockCode { get; set; }
        public string StockLocation { get; set; }
        public double Quantity { get; set; }
        public bool Enabled { get; set; }
        public string Reason { get; set; }
    }



    /// <summary>
    /// Chit Details for web order item
    /// </summary>
    public class WebOrderDBChitDetails
    {
        public int Chitnumber_Key { get; set; }
        public string DEFAULT_MENU { get; set; }
    }


    /// <summary>
    /// 
    /// </summary>
    public abstract class WebOrderDBBaseItem
    {
        #region PUBLIC

        public int OrderKey { get; internal set; }
        public int OrderMoveKey { get; set; }
        public int TabKey { get; set; }
        public string ItemName { get; set; }
        public int ItemKey { get; set; }
        public int IAO { get; set; }
        public int ItemId { get; set; }
        public string SizeName { get; set; }
        public string Note { get; set; }
        public string ServerName { get; set; }
        public string ServerInitials { get; set; }
        public string PartyName { get; set; }
        public int TableNumber { get; set; }
        public string TableName { get; set; }
        public int SeatNumber { get; set; }
        public decimal Price { get; set; }
        public decimal PriceAdjust { get; set; }
        public bool Printed { get; set; }
        public string TransactionNumber { get; set; }
        public int OrderType { get; set; }
        public string TerminalName { get; set; }
        public string MenuName { get; set; }
        public string TabName { get; set; }
        public string TabContainerName { get; set; }
        public string CourseName { get; set; }
        public bool HappyHour { get; set; }
        public string OrderLocation { get; set; }
        public int TabType { get; set; }
        public DateTime TimeStamp { get; set; }
        public decimal Cost { get; set; }
        public int LoyaltyKey { get; set; }
        public string MasterContainer { get; set; }
        public int SetMenuMask { get; set; }
        public int SetMenuGroup { get; set; }
        public string ItemCategory { get; set; }
        public string ServingCoursesName { get; set; }
        public string ServingCoursesKitchenName { get; set; }
        public int ServingCoursesKey { get; set; }
        public decimal Discount { get; set; }
        public decimal Redeemed { get; set; }
        public int SecurityRef { get; set; }
        public decimal PointsPercent { get; set; }
        public string ItemKitchenName { get; set; }
        public int TimeKey { get; set; }
        public decimal GSTPercent { get; set; }
        public decimal CostGSTPercent { get; set; }
        public string SizeKitchenName { get; set; }
        public string CourseKitchenName { get; set; }
        public int SideOrderKey { get; set; }
        public float Qty { get; set; }
        public int ThirdPartyCodeKey { get; set; }
        public int CategoryKey { get; set; }
        public int MemberFreeSaleCount { get; set; }
        public decimal MemberFreeSaleDiscount { get; set; }
        public decimal LocationFreeSaleCount { get; set; }
        public decimal LocationFreeSaleDiscount { get; set; }
        public bool IsFree { get; set; }
        public string DiscountReason { get; set; }
        public decimal PriceLevel0 { get; set; }
        public decimal PriceLevel1 { get; set; }
        public int CancelStockType { get; set; }
        public int ItemType { get; set; }
        public int MenuItemKey { get; set; }
        public int PLU { get; set; }
        public string OrderTypeMessage { get; set; }
        public int ContactsKey { get; set; }
        public int ActiveChitNumberKey { get; set; }
        public int PatronCount { get; set; }
        public string ItemHandheldName { get; set; }
        public string ItemReceiptName { get; set; }
        public string SizeHandheldName { get; set; }
        public string SizeReceiptName { get; set; }
        public string CourceHandheldName { get; set; }
        public string CourseReceiptName { get; set; }

        public WebOrderDBCategoriesList Categories { get; set; }

        #endregion

        #region CONSTRUCTORS

        public WebOrderDBBaseItem()
        {
            Categories = new WebOrderDBCategoriesList();
        }

        #endregion
    }

    /// <summary>
    /// 
    /// </summary>
    public class WebOrderDBCategory
    {
        public int CategoryKey { get; set; }
        public string CategoryName { get; set; }
    }

    /// <summary>
    /// 
    /// </summary>
    public class WebOrderDBCategoriesList : List<WebOrderDBCategory>
    {
        public int FinancialCategoryKey { get; set; }
        public string FinancialCategoryGroup { get; set; }
        public string FinancialCategory { get; set; }
        public int PrimaryArcCategoryKey { get; set; }
    }

    /// <summary>
    /// 
    /// </summary>
    public enum WebOrderDBStatus
    {
        NormalOrder,
        Unused,
        CanceledOrder,
        CreditNonExistingOrder
    }

    /// <summary>
    /// 
    /// </summary>
    public enum WebOrderETabType
    {
        TabNormal,
        TabStaff,
        TabMember,
        TabTableSeat,
        TabCashAccount,
        TabRoom,
        TabInvoice,
        TabWeb,
        TabParkedSale,
        TabHeld,
        TabNone
    }

    /// <summary>
    /// Provides DB access
    /// </summary>
    public class WebOrderDB
    {
        public enum WebOrderStatus
        {
            ewosReceived = 0,
            ewosProcessed,
            ewosBilled
        };

        // Singleton object: Povides with connections to the Menumate's dabase
        WebOrderDBConnection dbConnection_ = WebOrderDBConnection.Instance;
        WebOrderDBQueries dbQueries_ = WebOrderDBQueries.Instance;

        public FbTransaction transaction_;
        public FbConnection connection_;

        /// <summary>
        /// 
        /// </summary>
        public WebOrderDB()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly WebOrderDB instance = new WebOrderDB();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static WebOrderDB Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool BeginTransaction()
        {
            try
            {
                connection_ = dbConnection_.Open();
                ServiceLogger.Log("In BeginTransaction() after dbConnection_.Open().");
                transaction_ = connection_.BeginTransaction();
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

        /// <summary>
        /// 
        /// </summary>
        public void EndTransaction()
        {
            try
            {
                if (transaction_ != null)
                {
                    transaction_.Commit();
                    ServiceLogger.Log("In EndTransaction() after transaction_.Commit().");
                }
                if (connection_ != null)
                {
                    ServiceLogger.Log("In EndTransaction() before connection_.Close().");
                    connection_.Close();
                    ServiceLogger.Log("In EndTransaction() after connection_.Close().");
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in EndTransaction " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 133, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public FbConnection BeginConnection()
        {
            return dbConnection_.Open();
        }
        /// <summary>
        /// 
        /// </summary>
        public FbTransaction BeginFBtransaction()
        {
            return connection_.BeginTransaction();
        }

        /// <summary>
        /// 
        /// </summary>
        public void RollbackTransaction()
        {
            try
            {
                if (transaction_ != null)
                    transaction_.Rollback();
                if (connection_ != null)
                    connection_.Close();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in RollbackTransaction " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 134, short.MaxValue);
            }
        }

        /// <summary>
        /// TODO: checkWebOrderContactAndCreateIfNew(inInfo);
        /// </summary>
        /// <param name="inInfo"></param>
        public int SaveOrderInfo(WebOrderDBInfo inInfo)
        {
            int webOrderKey = _generateKeyWithGeneratorName(@"GEN_WEBORDERS_KEY");

            try
            {
                inInfo.TabKey = getOrCreateTabForWebOrder(webOrderKey, inInfo.Name, inInfo.Phone);
                inInfo.TimeKey = setTimeKey();
                inInfo.ContactKey = 0;

                //checkWebOrderContactAndCreateIfNew(inInfo);

                FbCommand command = dbQueries_.CreateSaveWebOrderCmd(
                                                    webOrderKey,
                                                    inInfo,
                                                    connection_,
                                                    transaction_);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                executeCommand(command);

                if (!inInfo.PaymentRequired)
                {
                    command = dbQueries_.CreditTabCmd(
                                                connection_,
                                                transaction_,
                                                inInfo.TabKey,
                                                inInfo.OrderTotal);

                    executeCommand(command);
                }

                //................................................

                int webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"Recipient",
                    inInfo.Recipient);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"StreetNo",
                    inInfo.StreetNo);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"StreetName",
                    inInfo.StreetName);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"Suburb",
                    inInfo.Suburb);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"City",
                    inInfo.City);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"Country",
                    inInfo.Country);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"Note",
                    inInfo.Note);

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"DELIVERY",
                    @"Phone",
                    inInfo.Phone);

                foreach (string comment in inInfo.Comments)
                {
                    webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                    SaveOrderData(
                        webOrderDataKey,
                        webOrderKey,
                        @"COMMENTS",
                        @"Comment",
                        comment);
                }
                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"PAYMENT_INFO",
                    @"Info",
                    inInfo.PaymentRequired ? @"Collect Payment" : @"PAID");

                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"EMAIL",
                    @"Email",
                    inInfo.Email);


                webOrderDataKey = _generateKeyWithGeneratorName(@"GEN_WEBDATA_KEY");
                SaveOrderData(
                    webOrderDataKey,
                    webOrderKey,
                    @"ORDERTYPE",
                    @"OrderType",
                    inInfo.OrderType);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SaveOrderInfo " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 223, short.MaxValue);
            }          //................................................

            return webOrderKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderKey"></param>
        /// <param name="iWebOrderDataKey"></param>
        /// <param name="inDataCategory"></param>
        /// <param name="inDataInfo"></param>
        /// <param name="inDataValue"></param>
        public void SaveOrderData(
                        int inWebOrderKey,
                        int iWebOrderDataKey,
                        string inDataCategory,
                        string inDataInfo,
                        string inDataValue)
        {
            try
            {
                FbCommand command = dbQueries_.CreateSaveWebDataCmd(
                                                        inWebOrderKey,
                                                        iWebOrderDataKey,
                                                        inDataCategory,
                                                        inDataInfo,
                                                        inDataValue,
                                                        connection_,
                                                        transaction_);

                executeCommand(command);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SaveOrderData " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 262, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItems"></param>
        public void SaveOrderItems(
                        int webOrderKey,
                        WebOrderDBItem[] inItems,
                        int inMenuKey,
                        string number)
        {
            try
            {
                for (int i = 0; i < inItems.Length; i++)
                {
                    populateOrderItem(
                                webOrderKey,
                                inItems[i],
                                inMenuKey);

                    saveOrderItemInDB(
                                webOrderKey,
                                inItems[i],
                                number);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SaveOrderItems " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 224, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        public bool WebOrderAccepted(string inOrderHandle)
        {
            bool result = false;

            //..........................................

            try
            {
                FbCommand command = dbQueries_.CreateWebOrderAcceptedCmd(
                                                        connection_,
                                                        transaction_,
                                                        inOrderHandle);

                ///command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                FbDataReader reader = command.ExecuteReader();

                if (reader.HasRows && reader.Read())
                {
                    result = reader.GetInt32(reader.GetOrdinal("orderCount")) > 0;
                }

                reader.Close();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in WebOrderAccepted " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 225, short.MaxValue);
            }

            //..........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteID"></param>
        /// <returns></returns>
        public bool ValidWebOrderSiteID(string inSiteID)
        {
            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inThirdPartyCode"></param>
        /// <returns></returns>
        public Int32 GetThirdPartyCodeKey(string inThirdPartyCode, int inMenukey)
        {
            Int32 result = 0;

            //..........................................

            try
            {
                FbCommand command = dbQueries_.CreateReadThirdPartyCodeKeyCmd(
                                                        connection_,
                                                        transaction_,
                                                        inThirdPartyCode,
                                                        inMenukey);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                FbDataReader reader = command.ExecuteReader();

                if (reader.HasRows && reader.Read())
                {
                    result = reader.GetInt32(reader.GetOrdinal("tpcKey"));
                }

                reader.Close();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetThirdPartyCodeKey " + e.Message, e);
              //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 170, short.MaxValue);
            }

            //..........................................

            return result;
        }
        #endregion

        #region Private
        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderKey"></param>
        /// <param name="orderDbItem"></param>
        private void populateOrderItem(
                            int webOrderKey,
                            WebOrderDBItem orderDbItem,
                            int inMenuKey)
        {
            try
            {
                decimal price = orderDbItem.Price;
                int itemSizeKey = getItemSizeKeyFromThirdPartyCodeKey(
                                                orderDbItem.ThirdPartyCodeKey, inMenuKey);

                if (itemSizeKey == 0)
                    throw new Exception(string.Format(@"Item not found. Check the third party code: {0}",
                                        orderDbItem.ThirdPartyCodeKey));

                if (itemSizeKey == -1)
                    throw new Exception(string.Format(@"Third party code assigned to more than one item size. Check the third party code: {0}",
                                        orderDbItem.ThirdPartyCodeKey));

                if (!checkItemIsEnabled(itemSizeKey))
                    throw new Exception(string.Format(@"Item ordered is disabled or missing in the menu. Third party code: {0}",
                                                      orderDbItem.ThirdPartyCodeKey));

                loadCompleteOrder(
                            webOrderKey,
                            itemSizeKey,
                            ref orderDbItem,
                            inMenuKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in populateOrderItem " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 142, short.MaxValue);
            }
        }

        /// <summary>
        /// Returns  0: If the third party code is not found
        /// Returns -1: If the third party code has been assigned to more than one item size
        /// Returns the item size key: otherwise
        /// </summary>
        /// <param name="thirdPartyCode"></param>
        /// <returns></returns>
        private int getItemSizeKeyFromThirdPartyCodeKey(int thirdPartyCodeKey, int inMenukey)
        {
            FbCommand command = dbQueries_.CreateItemSizeKeyFromThridPartyCodeKeyCmd(
                                                 connection_,
                                                 transaction_,
                                                 thirdPartyCodeKey,
                                                 inMenukey);

            //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command)
            try
            {
                using (FbDataReader reader = command.ExecuteReader())
                {
                    List<object> keyList = new List<object>();

                    //There is not other way to know how many records the reader holds
                    while (reader.Read())
                    {
                        object value = getReaderColumnValue(
                                          reader, "ITEMSIZE_KEY", 0);

                        keyList.Add(value);
                    }

                    //.................................................

                    if (keyList.Count == 0)
                    {
                        return 0;
                    }

                    //.................................................

                    if (keyList.Count > 1)
                    {
                        return -1;
                    }

                    //.................................................
                    return Convert.ToInt32(keyList.ToArray()[0]);

                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getItemSizeKeyFromThirdPartyCodeKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 143, short.MaxValue);
                return 0;
            }
            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="itemSizeKey"></param>
        /// <returns></returns>
        private bool checkItemIsEnabled(int itemSizeKey)
        {
            FbCommand command = dbQueries_.CreateCheckItemCompleteEnabledCmd(
                                                                        connection_,
                                                                        transaction_,
                                                                        itemSizeKey);

            //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

            return Convert.ToChar(executeScalarCommand(command)) == 'T';
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="itemSizeKey"></param>
        /// <param name="orderDbItem"></param>
        void loadCompleteOrder(
                        int webOrderKey,
                        int itemSizeKey,
                        ref WebOrderDBItem orderDbItem,
                        int inMenuKey)
        {
            WebOrderDBBaseItem baseOrder = orderDbItem;

            loadBaseOrderData(
                    itemSizeKey,
                    ref baseOrder);

            loadBaseOrderBreakdownCategories(
                            itemSizeKey,
                            ref baseOrder);

            orderDbItem = (WebOrderDBItem)baseOrder;

            loadRecipes(
                     itemSizeKey,
                     ref orderDbItem);

            loadOptions(
                     ref orderDbItem);

            loadSides(
                    ref orderDbItem, inMenuKey);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="orderDbItem"></param>
        private void loadBaseOrderData(
                                     int itemSizeKey,
                                     ref WebOrderDBBaseItem orderDbItem)
        {
            try
            {
                FbCommand command = dbQueries_.LoadBasicItemCompleteCmd(
                                                                connection_,
                                                                transaction_,
                                                                itemSizeKey);

                /// command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                FbDataReader reader = command.ExecuteReader();
                if (reader.HasRows
                    && reader.Read())
                    loadBaseOrderDataFromQuery(
                                            reader,
                                            ref orderDbItem);

                reader.Close();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadBaseOrderData " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 144, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="reader"></param>
        /// <param name="orderDbItem"></param>
        private void loadBaseOrderDataFromQuery(
                                FbDataReader reader,
                                ref WebOrderDBBaseItem orderDbItem)
        {
            try
            {
                orderDbItem.MenuName = reader.GetString(reader.GetOrdinal("Menu_Name"));
                orderDbItem.ItemType = reader.GetInt32(reader.GetOrdinal("Menu_Type"));
                orderDbItem.OrderType = (int)WebOrderDBStatus.NormalOrder;
                orderDbItem.CourseName = reader.GetString(reader.GetOrdinal("Course_Name"));
                orderDbItem.CourseKitchenName = reader.GetString(reader.GetOrdinal("Course_Kitchen_Name"));
                orderDbItem.ServingCoursesName = reader.GetString(reader.GetOrdinal("ServingCourse_Name"));
                orderDbItem.ServingCoursesKitchenName = reader.GetString(reader.GetOrdinal("SERVINGCOURSE_KITCHEN_NAME"));
                orderDbItem.ServingCoursesKey = reader.GetInt32(reader.GetOrdinal("ServingCourses_Key"));
                orderDbItem.ItemKey = reader.GetInt32(reader.GetOrdinal("Item_Key"));
                orderDbItem.IAO = reader.GetInt32(reader.GetOrdinal("IAO"));
                orderDbItem.ItemName = reader.GetString(reader.GetOrdinal("Item_Name"));
                orderDbItem.ItemKitchenName = reader.GetString(reader.GetOrdinal("Item_Kitchen_Name"));
                orderDbItem.ItemId = reader.GetInt32(reader.GetOrdinal("Item_ID"));
                orderDbItem.SizeName = reader.GetString(reader.GetOrdinal("Size_Name"));
                orderDbItem.SizeKitchenName = reader.GetString(reader.GetOrdinal("Size_Kitchen_Name"));
                orderDbItem.PriceLevel0 = orderDbItem.Price;
                orderDbItem.PriceLevel1 = reader.GetDecimal(reader.GetOrdinal("Special_Price"));
                orderDbItem.GSTPercent = reader.GetDecimal(reader.GetOrdinal("GST_Percent"));
                orderDbItem.Cost = reader.GetDecimal(reader.GetOrdinal("Cost"));
                orderDbItem.CostGSTPercent = reader.GetDecimal(reader.GetOrdinal("Cost_GST_Percent"));
                orderDbItem.PointsPercent = reader.GetDecimal(reader.GetOrdinal("Points_Percent"));
                orderDbItem.SetMenuMask = reader.GetInt32(reader.GetOrdinal("Setmenu_Mask"));
                orderDbItem.MemberFreeSaleCount = reader.GetInt32(reader.GetOrdinal("Mem_Sale_Count"));
                orderDbItem.MemberFreeSaleDiscount = reader.GetDecimal(reader.GetOrdinal("Mem_Discount_Percent"));
                orderDbItem.LocationFreeSaleCount = reader.GetDecimal(reader.GetOrdinal("Loc_Discount_Percent"));
                orderDbItem.LocationFreeSaleDiscount = reader.GetDecimal(reader.GetOrdinal("Loc_Discount_Percent"));
                orderDbItem.PLU = reader.GetInt32(reader.GetOrdinal("PLU"));
                orderDbItem.TerminalName = "WebMate";
                orderDbItem.OrderLocation = "WebMate";
                orderDbItem.TabType = (int)WebOrderETabType.TabWeb;
                orderDbItem.Categories.FinancialCategory = reader.GetString(reader.GetOrdinal("Category"));
                orderDbItem.Categories.FinancialCategoryKey = reader.GetInt32(reader.GetOrdinal("Category_Key"));
                orderDbItem.Categories.FinancialCategoryGroup = reader.GetString(reader.GetOrdinal("Category_Group_Name"));
                orderDbItem.MenuItemKey = orderDbItem.ItemKey;
                //changes for chit
                orderDbItem.Price = orderDbItem.Price;

                //orderDbItem.Price = orderDbItem.PriceLevel0;
                //orderDbItem.Price = orderDbItem.PriceLevel1;

                if (string.IsNullOrEmpty(orderDbItem.CourseKitchenName))
                    orderDbItem.CourseKitchenName = orderDbItem.CourseName;
                if (string.IsNullOrEmpty(orderDbItem.ItemKitchenName))
                    orderDbItem.ItemKitchenName = orderDbItem.ItemName;
                if (string.IsNullOrEmpty(orderDbItem.SizeKitchenName))
                    orderDbItem.SizeKitchenName = orderDbItem.SizeName;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadBaseOrderDataFromQuery " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 263, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="itemSizeKey"></param>
        /// <param name="orderDbItem"></param>
        private void loadBaseOrderBreakdownCategories(
                                int itemSizeKey,
                                ref WebOrderDBBaseItem orderDbItem)
        {
            try
            {
                FbCommand command = dbQueries_.LoadBreakDownCategoriesCmd(
                                                                    connection_,
                                                                    transaction_,
                                                                    itemSizeKey);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        orderDbItem.Categories.Add(new WebOrderDBCategory()
                        {
                            CategoryKey = reader.GetInt32(reader.GetOrdinal("Category_Key")),
                            CategoryName = reader.GetString(reader.GetOrdinal("Category"))
                        });
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadBaseOrderBreakdownCategories " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 145, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="itemSizeKey"></param>
        /// <param name="orderDbItem"></param>
        private void loadRecipes(
                            int itemSizeKey,
                            ref WebOrderDBItem orderDbItem)
        {
            try
            {
                FbCommand command = dbQueries_.LoadSalesRecipesCmd(
                                                            connection_,
                                                            transaction_,
                                                            itemSizeKey);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        WebOrderDBRecipe recipe = getWebOrderDBRecipeFromReader(reader);
                        orderDbItem.ItemRecipes.Add(recipe);
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadRecipes " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 146, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="reader"></param>
        /// <returns></returns>
        private WebOrderDBRecipe getWebOrderDBRecipeFromReader(FbDataReader reader)
        {
            WebOrderDBRecipe recipe = new WebOrderDBRecipe();

            try
            {
                recipe.StockCode = Convert.ToString(getReaderColumnValue(reader, "Stock_Code", ""));
                recipe.StockLocation = Convert.ToString(getReaderColumnValue(reader, "Stock_Location", ""));
                recipe.Quantity = Convert.ToDouble(getReaderColumnValue(reader, "Qty", 0.0));
                recipe.Cost = Convert.ToDecimal(getReaderColumnValue(reader, "Stock_Unit_Cost", 0.0m));
                recipe.CostGSTPercent = Convert.ToDouble(getReaderColumnValue(reader, "Cost_GST_Percent", 0.0));
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getWebOrderDBRecipeFromReader " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 264, short.MaxValue);
            }

            return recipe;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="orderDbItem"></param>
        private void loadOptions(ref WebOrderDBItem orderDbItem)
        {
            try
            {

                for (int i = 0; i < orderDbItem.ItemOptions.Count; i++)
                {
                    WebOrderDBItemOption option = orderDbItem.ItemOptions[i];
                    FbCommand command = dbQueries_.LoadItemOptionCmd(
                                                                connection_,
                                                                transaction_,
                                                                option.OptionKey);

                    //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                    using (FbDataReader reader = command.ExecuteReader())
                    {
                        if (reader.Read())
                        {
                            loadOptionDataFromReader(reader, ref option);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"getWebOrderDBRecipeFromReader " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 147, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="reader"></param>
        /// <param name="option"></param>
        private void loadOptionDataFromReader(
                                    FbDataReader reader,
                                    ref WebOrderDBItemOption option)
        {
            try
            {
                option.Name = Convert.ToString(getReaderColumnValue(reader, "OPTION_NAME", ""));
                option.KitchenName = Convert.ToString(getReaderColumnValue(reader, "OPTION_KITCHEN_NAME", ""));
                option.OptionID = Convert.ToInt32(getReaderColumnValue(reader, "OPTION_ID", 0));
                option.ForcedMask = Convert.ToInt32(getReaderColumnValue(reader, "FORCED_MASK", 0));
                option.Flags = Convert.ToInt32(getReaderColumnValue(reader, "FLAGS", 0));
                option.GroupNumber = option.Flags & 0x0F;
                option.DisallowMuliSelect = Convert.ToBoolean(option.Flags & 0x10);
                option.PlusOption = Convert.ToBoolean(option.Flags & 0x20);
                option.MinusOption = Convert.ToBoolean(option.Flags & 0x40);
                option.AllowSkip = Convert.ToBoolean(option.Flags & 0x80);
                option.Underlined = Convert.ToString(getReaderColumnValue(reader, "PRINT_UNDERLINED", "F")) == "T";
                option.Bold = Convert.ToString(getReaderColumnValue(reader, "PRINT_BOLD", "F")) == "T";
                option.Color = (FontColor)getReaderColumnValue(reader, "PRINT_COLOUR", FontColor.Black);
                option.Font = (FontType)getReaderColumnValue(reader, "PRINT_FONT", FontType.FontA);
                option.Width = (FontSize)getReaderColumnValue(reader, "PRINT_FONT", FontSize.Normal);
                option.Width = (FontSize)getReaderColumnValue(reader, "PRINT_FONT", FontSize.Normal);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"loadOptionDataFromReader " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 265, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="orderDbItem"></param>
        private void loadSides(ref WebOrderDBItem orderDbItem, int inMenukey)
        {
            try
            {
                for (int i = 0; i < orderDbItem.ItemSides.Count; i++)
                {
                    WebOrderDBBaseItem side = orderDbItem.ItemSides[i];
                    int sideItemSizeKey = getItemSizeKeyFromThirdPartyCodeKey(side.ThirdPartyCodeKey, inMenukey);

                    if (sideItemSizeKey == 0)
                        throw new Exception(string.Format(@"Item side not found. Check the third party code: {0}",
                                            side.ThirdPartyCodeKey));

                    if (sideItemSizeKey == -1)
                        throw new Exception(string.Format(@"Third party code assigned to more than one item side. Check the third party code: {0}",
                                            side.ThirdPartyCodeKey));

                    FbCommand command = dbQueries_.LoadBasicItemCompleteCmd(
                                                                    connection_,
                                                                    transaction_,
                                                                    sideItemSizeKey);

                    using (FbDataReader reader = command.ExecuteReader())
                    {
                        if (reader.Read())
                        {
                            loadBaseOrderDataFromQuery(
                                                     reader,
                                                     ref side);

                            loadBaseOrderBreakdownCategories(
                                                     sideItemSizeKey,
                                                     ref side);

                            side.Categories.PrimaryArcCategoryKey = getArcCategoryKeyFromCategory(side.Categories.FinancialCategory);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in loadSides " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 148, short.MaxValue);
            }   
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="orderDbItem"></param>
        private void saveOrderItemInDB(
                            int webOrderKey,
                            WebOrderDBItem orderDbItem,
                            string number)
        {
            try
            {
                if (!isDuplicateOrder(orderDbItem))
                {
                    insertOrderItemToDB(
                                    webOrderKey,
                                    orderDbItem,
                                    number);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in saveOrderItemInDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 149, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="orderDbItem"></param>
        /// <returns></returns>
        private bool isDuplicateOrder(
                            WebOrderDBItem orderDbItem)
        {
            FbCommand command = dbQueries_.CheckDuplicateOrderCmd(
                                                            connection_,
                                                            transaction_,
                                                            orderDbItem.ItemId,
                                                            orderDbItem.SizeName,
                                                            orderDbItem.TerminalName,
                                                            orderDbItem.TransactionNumber);

            return executeBoolCommand(command);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="orderDbItem"></param>
        private void insertOrderItemToDB(
                            int webOrderKey,
                            WebOrderDBItem orderDbItem,
                            string number)
        {
            try
            {
                WebOrderDBBaseItem baseItem = orderDbItem;
                //AQUI!!!!
                orderDbItem.TimeKey = getOrCreateTimeKeyForWebOrder(webOrderKey); //setTimeKey();
                orderDbItem.SecurityRef = getNextSecurityRef();
                orderDbItem.TransactionNumber = Convert.ToString(getNextTransactionNumber());
                orderDbItem.TabKey = getOrCreateTabForWebOrder(webOrderKey, baseItem.TabName, number);
                orderDbItem.Categories.PrimaryArcCategoryKey = getArcCategoryKeyFromCategory(baseItem.Categories.FinancialCategory);
                orderDbItem.TabContainerName = getGuidFromWebOrderKey(webOrderKey);

                insertOrderBaseDataToDB(
                                    connection_,
                                    transaction_,
                                    ref baseItem);

                insertOrderBreakdownCategoriesToDB(
                                    connection_,
                                    transaction_,
                                    baseItem.OrderKey,
                                    baseItem.Categories);

                orderDbItem = (WebOrderDBItem)baseItem;

                insertOrderRecipesToDB(
                                    connection_,
                                    transaction_,
                                    orderDbItem.OrderKey,
                                    orderDbItem.ItemRecipes);

                insertOrderOptionsToDB(
                                    connection_,
                                    transaction_,
                                    orderDbItem.OrderKey,
                                    orderDbItem.ItemOptions);

                insertOrderSidesToDB(webOrderKey,
                                    connection_,
                                    transaction_,
                                    orderDbItem.OrderKey,
                                    orderDbItem.TabKey,
                                    orderDbItem.TransactionNumber,
                                    orderDbItem.ItemSides,
                                    orderDbItem.SecurityRef,
                                    orderDbItem.TimeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in insertOrderItemToDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 150, short.MaxValue);
            }

        }


        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int setTimeKey()
        {
            int timeKey = getTimeKey();
            try
            {
                openSaleStartTime(timeKey);
                closeSaleStartTime(timeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in setTimeKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 266, short.MaxValue);
            }

            return timeKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int getTimeKey()
        {
            return _generateKey("TURN_AROUND_TIMES");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="timeKey"></param>
        private void openSaleStartTime(int timeKey)
        {
            try
            {
                FbCommand command = dbQueries_.OpenSaleStartTimeCmd(
                                                                    connection_,
                                                                    transaction_,
                                                                    timeKey);
                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in openSaleStartTime " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 267, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="timeKey"></param>
        private void closeSaleStartTime(int timeKey)
        {
            try
            {
                FbCommand command = dbQueries_.CloseSaleStartTimeCmd(
                                                                   connection_,
                                                                   transaction_,
                                                                   timeKey);
                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in closeSaleStartTime " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 268, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int getNextSecurityRef()
        {
            return _generateKey("SECURITY_REF");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        private int getOrCreateTabForWebOrder(
                                    int webOrderKey,
                                    string tabName,
                                    string id_number)
        {
            int tabKey = findTabKeyForWebOrderKey(webOrderKey);
            try
            {
                if (tabKey == 0)
                    tabKey = createTabForWebOrder(
                                            webOrderKey,
                                            tabName,
                                            id_number);
            }
            catch (Exception e) 
            {
                ServiceLogger.LogException(@"in getOrCreateTabForWebOrder " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 133, short.MaxValue);
            }
            return tabKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderKey"></param>
        /// <returns></returns>
        private int getOrCreateTimeKeyForWebOrder(int inWebOrderKey)
        {
            int timeKey = findTimeKeyForWebOrderKey(inWebOrderKey);
            try
            {
                if (timeKey == 0)
                    timeKey = createTimeForWebOrder(inWebOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getOrCreateTimeKeyForWebOrder " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 151, short.MaxValue);
            }
            return timeKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInfo"></param>
        private void checkWebOrderContactAndCreateIfNew(WebOrderDBInfo inInfo)
        {
            if (!findContacInDB(inInfo))
            {
                createContactInDB(inInfo);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        private int findTabKeyForWebOrderKey(int webOrderKey)
        {
            int tabKey = 0;

            try
            {
                FbCommand command = dbQueries_.GetTabKeyByWebOrderKeyCmd(
                                                                        connection_,
                                                                        transaction_,
                                                                        webOrderKey);

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
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 269, short.MaxValue);
            }

            return tabKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        private int createTabForWebOrder(
                                int webOrderKey,
                                string tabName,
                                string id_number)
        {
            int tabKey = createWebOrderTabInDB(
                                            tabName,
                                            WebOrderETabType.TabWeb,
                                            id_number);
            updateWebOrderWithTabKey(
                                webOrderKey,
                                tabKey);

            return tabKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int createWebOrderTabInDB(
                                string tabName,
                                WebOrderETabType tabType, string id_number)
        {
            int tabKey = _generateKey("TAB");

            try
            {
                FbCommand command = dbQueries_.CreateTabCmd(
                                                connection_,
                                                transaction_,
                                                tabKey,
                                                tabName,
                                                (int)tabType,
                                                id_number);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in createWebOrderTabInDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 270, short.MaxValue);
            }

            return tabKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderKey"></param>
        /// <param name="tabKey"></param>
        private void updateWebOrderWithTabKey(int webOrderKey, int tabKey)
        {
            try
            {
                FbCommand command = dbQueries_.SetTabKeyForWebOrderCmd(
                                                                    connection_,
                                                                    transaction_,
                                                                    webOrderKey,
                                                                    tabKey);
                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in updateWebOrderWithTabKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 152, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderKey"></param>
        /// <returns></returns>
        private int findTimeKeyForWebOrderKey(int inWebOrderKey)
        {
            int timeKey = 0;
            try
            {
                FbCommand command = dbQueries_.GetTimeKeyByWebOrderKeyCmd(
                                                                    connection_,
                                                                    transaction_,
                                                                    inWebOrderKey);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        timeKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                                        reader,
                                                        "TIME_KEY",
                                                        0));
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in findTimeKeyForWebOrderKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 153, short.MaxValue);
            }
            return timeKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int createTimeForWebOrder(int inWebOrderKey)
        {
            int timeKey = createTimeInDB();

            updateWebOrderWithTimeKey(
                                inWebOrderKey,
                                timeKey);

            return timeKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int createTimeInDB()
        {
            int timeKey = _generateKey("TURN_AROUND_TIMES");

            try
            {
                FbCommand command = dbQueries_.CreateTimeCmd(
                                                            connection_,
                                                            transaction_,
                                                            timeKey);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in createTimeInDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 271, short.MaxValue);
            }

            return timeKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderKey"></param>
        /// <param name="inTimeKey"></param>
        private void updateWebOrderWithTimeKey(int inWebOrderKey, int inTimeKey)
        {
            try
            {
                FbCommand command = dbQueries_.SetTimeKeyForWebOrderCmd(
                                                                    connection_,
                                                                    transaction_,
                                                                    inWebOrderKey,
                                                                    inTimeKey);
                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);
                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in updateWebOrderWithTimeKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 154, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInfo"></param>
        /// <returns></returns>
        private bool findContacInDB(WebOrderDBInfo inInfo)
        {
            int contactKey = 0;

            try
            {
                FbCommand command = dbQueries_.GetContactInDBCmd(
                                                      connection_,
                                                      transaction_,
                                                      inInfo);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        contactKey = Convert.ToInt32(
                                        getReaderColumnValue(
                                           reader,
                                           "CONTACTS_KEY",
                                           0));
                }

                return contactKey > 0;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in findContacInDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 155, short.MaxValue);
                return false;
            }
            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int createContactInDB(WebOrderDBInfo inInfo)
        {
            int contactseKey = _generateKey("CONTACTS");
            try
            {
                FbCommand command = dbQueries_.CreateContactCmd(
                                     connection_,
                                     transaction_,
                                     contactseKey,
                                     inInfo);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                command.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in createContactInDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 156, short.MaxValue);
            }
            return contactseKey;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private int getArcCategoryKeyFromCategory(string categoryName)
        {
            int result = 0;
            try
            {
                FbCommand command = dbQueries_.GetArchiveCategoryCmd(
                                                                connection_,
                                                                transaction_,
                                                                categoryName);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        result = reader.GetInt32(reader.GetOrdinal("CATEGORY_KEY"));
                    }
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getArcCategoryKeyFromCategory " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 157, short.MaxValue);
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        private string getGuidFromWebOrderKey(int webOrderKey)
        {
            string result = "";
            try
            {
                FbCommand command = dbQueries_.GetGuidFromOrderKeyCmd(
                                                                connection_,
                                                                transaction_,
                                                                webOrderKey);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.Read())
                        result = reader.GetString(reader.GetOrdinal("GUID"));
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in getGuidFromWebOrderKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 158, short.MaxValue);
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private int getNextTransactionNumber()
        {
            return _generateKey("PCINTERNALTRANSNUMBER");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection_"></param>
        /// <param name="transaction_"></param>
        /// <param name="orderDbItem"></param>
        /// <returns></returns>
        private bool insertOrderBaseDataToDB(
                                FbConnection connection_,
                                FbTransaction transaction_,
                                ref WebOrderDBBaseItem orderDbItem)
        {
            orderDbItem.OrderKey = _generateKey("ORDERS");

            try
            {
                FbCommand command = dbQueries_.SetOrderCmd(
                                                        connection_,
                                                        transaction_,
                                                        orderDbItem);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                return command.ExecuteNonQuery() > 0;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in insertOrderBaseDataToDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 159, short.MaxValue);
                return false;
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection_"></param>
        /// <param name="transaction_"></param>
        /// <param name="webOrderDBCategoriesList"></param>
        /// <returns></returns>
        private bool insertOrderBreakdownCategoriesToDB(
                                        FbConnection connection_,
                                        FbTransaction transaction_,
                                        int orderKey,
                                        WebOrderDBCategoriesList webOrderDBCategoriesList)
        {
            try
            {
                foreach (WebOrderDBCategory category in webOrderDBCategoriesList)
                {
                    category.CategoryKey = getArcCategoryKeyFromCategory(category.CategoryName);
                    FbCommand command = dbQueries_.SetOrderBreakdownCategoryCmd(
                                                                           connection_,
                                                                           transaction_,
                                                                           orderKey,
                                                                           category);

                    //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                    command.ExecuteNonQuery();
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in insertOrderBreakdownCategoriesToDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 160, short.MaxValue);
            }
            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection_"></param>
        /// <param name="transaction_"></param>
        /// <param name="list"></param>
        private bool insertOrderRecipesToDB(
                                        FbConnection connection_,
                                        FbTransaction transaction_,
                                        int orderKey,
                                        List<WebOrderDBRecipe> list)
        {
            bool result = true;
            try
            {
                foreach (WebOrderDBRecipe recipe in list)
                {
                    recipe.RecipeKey = _generateKey("ORDERRECIPE");

                    FbCommand command = dbQueries_.SetOrderRecipeCmd(
                                                                connection_,
                                                                transaction_,
                                                                orderKey,
                                                                recipe);

                    result = command.ExecuteNonQuery() > 0;
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in insertOrderRecipesToDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 161, short.MaxValue);
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection_"></param>
        /// <param name="transaction_"></param>
        /// <param name="list"></param>
        /// <returns></returns>
        private bool insertOrderOptionsToDB(
                                    FbConnection connection_,
                                    FbTransaction transaction_,
                                    int orderKey,
                                    List<WebOrderDBItemOption> list)
        {
            bool result = true;
            try
            {
                foreach (WebOrderDBItemOption option in list)
                {
                    option.OptionKey = _generateKey("ORDEROPTION");

                    FbCommand command = dbQueries_.SetOrderOptionCmd(
                                                                connection_,
                                                                transaction_,
                                                                orderKey,
                                                                option);

                    result = command.ExecuteNonQuery() > 0;
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in insertOrderOptionsToDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 162, short.MaxValue);
            }
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection_"></param>
        /// <param name="transaction_"></param>
        /// <param name="list"></param>
        /// <returns></returns>
        private bool insertOrderSidesToDB(int webOrderKey,
                                FbConnection connection_,
                                FbTransaction transaction_,
                                int masterOrderKey,
                                int masterTabKey,
                                string transactionNumber,
                                List<WebOrderDBItemSide> list,
                                int security_ref,
                                int time_key)
        {
            bool status = false;
            try
            {
                for (int i = 0; i < list.Count; i++)
                {
                    WebOrderDBBaseItem side = list[i];
                    side.SideOrderKey = masterOrderKey;
                    side.TabKey = masterTabKey;
                    side.TransactionNumber = transactionNumber;
                    side.TabContainerName = getGuidFromWebOrderKey(webOrderKey);
                    side.SecurityRef = security_ref;
                    side.TimeKey = time_key;
                    status = insertOrderBaseDataToDB(
                                                connection_,
                                                transaction_,
                                                ref side);

                    insertOrderBreakdownCategoriesToDB(
                                                connection_,
                                                transaction_,
                                                side.OrderKey,
                                                side.Categories);
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in insertOrderSidesToDB " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 163, short.MaxValue);
            }
            return status;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTableName"></param>
        /// <returns></returns>
        protected int _generateKey(string inTableName)
        {
            object commandResult = null;
            try
            {
                string generatorName = "GEN_" + inTableName;
                string sql = "SELECT GEN_ID(" + generatorName + ", 1) FROM RDB$DATABASE";

                FbCommand command = new FbCommand(sql, connection_, transaction_);
                if ((commandResult = command.ExecuteScalar()) == null)
                {
                    throw new KeyGeneratorException(inTableName + " key genertor failed to produce key.");
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in _generateKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 164, short.MaxValue);
            }
            return Convert.ToInt32(commandResult);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inGeneratorName"></param>
        /// <returns></returns>
        protected int _generateKeyWithGeneratorName(string inGeneratorName)
        {
            object commandResult = null;
            try
            {
                string sql = "SELECT GEN_ID(" + inGeneratorName + ", 1) FROM RDB$DATABASE";

                FbCommand command = new FbCommand(sql, connection_, transaction_);
                if ((commandResult = command.ExecuteScalar()) == null)
                {
                    throw new KeyGeneratorException(inGeneratorName + " key genertor failed to produce key.");
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in _generateKeyWithGeneratorName " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 134, short.MaxValue);
            }
            return Convert.ToInt32(commandResult);

        }

        public int IfExistWebOrderCmd(string GUID)
        {
            object commandResult = null;
            try
            {
                string sql = "SELECT  count(WEBORDERS.GUID)  guidcount FROM WEBORDERS where WEBORDERS.GUID = '" + GUID+"'";

                FbCommand command = new FbCommand(sql, connection_, transaction_);
                commandResult = command.ExecuteScalar();


            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GUID " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 134, short.MaxValue);
            }
            return Convert.ToInt32(commandResult);

        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSQL"></param>
        /// <returns></returns>
        protected void executeQuery(string inSQL)
        {
            try
            {
                FbCommand command = new FbCommand(inSQL, connection_, transaction_);
                executeCommand(command);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in executeQuery " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 135, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSQL"></param>
        /// <returns></returns>
        protected bool executeBoolQuery(string inSQL)
        {
            bool result = false;
            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            try
            {
                FbCommand command = new FbCommand(inSQL, connection_, transaction_);
                result = executeBoolCommand(command);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in executeBoolQuery " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 136, short.MaxValue);
            }
            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCommand"></param>
        /// <returns></returns>
        bool executeBoolCommand(FbCommand inCommand)
        {
            bool result = false;
            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            try
            {
                using (FbDataReader reader = inCommand.ExecuteReader())
                {
                    result = reader.Read(); //reader.HasRows() will always be true even though there are no actual readable data
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in executeBoolCommand " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 121, short.MaxValue);
            }
            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCommand"></param>
        /// <returns></returns>
        void executeCommand(FbCommand inCommand)
        {
            try
            {
                using (FbDataReader reader = inCommand.ExecuteReader())
                {
                }
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in executeCommand " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 165, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inCommand"></param>
        /// <returns></returns>
        object executeScalarCommand(FbCommand inCommand)
        {
            return inCommand.ExecuteScalar();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="reader"></param>
        /// <param name="columnName"></param>
        /// <param name="defaultValue"></param>
        /// <returns></returns>
        object getReaderColumnValue(
                            FbDataReader reader,
                            string columnName,
                            object defaultValue)
        {
            int ordinal = reader.GetOrdinal(columnName);
            if (reader.IsDBNull(ordinal))
                return defaultValue;
            else
                return reader[ordinal];
        }

        public int GetPOSChitDetails(string inOrderType)
        {
            //..........................................
            int menu_key = 0;
            try
            {
                WebOrderDBChitDetails option = new WebOrderDBChitDetails();

                FbCommand command = dbQueries_.GetChitDetailsCmd(
                                                    connection_,
                                                    transaction_,
                                                    inOrderType);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        option.Chitnumber_Key = Convert.ToInt32(getReaderColumnValue(reader, "CHITNUMBER_KEY", 0));
                        option.DEFAULT_MENU = Convert.ToString(getReaderColumnValue(reader, "DEFAULT_MENU", ""));
                    }                    
                }
                if (option.DEFAULT_MENU != "" || option.DEFAULT_MENU != null)
                {
                    menu_key = GetMenuKey(option.DEFAULT_MENU);
                }
                return menu_key;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetPOSChitDetails " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 137, short.MaxValue);
                return menu_key = 0;
                //throw;
            }


        }


        public int GetMenuKey(string inMenuName)
        {
            //..........................................
            int menukey = 0;
            try
            {

                FbCommand command = dbQueries_.GetMenuKeyCmd(
                                                    connection_,
                                                    transaction_,
                                                    inMenuName);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);

                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        menukey = Convert.ToInt32(getReaderColumnValue(reader, "MENU_KEY", 0));
                    }                    
                } 
                return menukey;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetMenuKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 166, short.MaxValue);
                return menukey;
            }
        }

        //check webmate enabled or not
        public bool CheckWemateInterface()
        {
            //..........................................
            bool _isEnabled = false;
            try
            {

                FbCommand command = dbQueries_.GetWebmateInterfaceCmd(
                                                    connection_,
                                                    transaction_);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);
                using (FbDataReader reader = command.ExecuteReader())
                {
                    if (reader.HasRows && reader.Read())
                    {
                        _isEnabled = Convert.ToBoolean(getReaderColumnValue(reader, "INTEGER_VAL", 0));
                    }                    
                } 
                return _isEnabled;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckWemateInterface " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 139, short.MaxValue);
                return _isEnabled = false;
            }
        }

        //Enable Setting to Notify webmate Interface enabled or not
        public bool SetForWemateInterfaceMessage()
        {
            //..........................................
            bool result = false;
            try
            {

                FbCommand command = dbQueries_.UpdateToNotifyWebmateInterfaceEnableOrDisableCmd(
                                                    connection_,
                                                    transaction_);

                //command.CommandTimeout = Convert.ToInt32(DBTimeOuts.Command);
                result = command.ExecuteNonQuery() > 0;
                return result;
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetForWemateInterfaceMessage " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 140, short.MaxValue);
                return result = false;
            }
        }




        #endregion
    }

    /// <summary>
    /// 
    /// </summary>
    internal class WebMateDbException : Exception
    {
        public WebMateDbException() : base() { }
        public WebMateDbException(string message) : base(message) { }
        public WebMateDbException(string message, System.Exception inner) : base(message, inner) { }
    }

    /// <summary>
    /// 
    /// </summary>
    internal class KeyAlreadyExistsException : WebMateDbException
    {
        public KeyAlreadyExistsException() : base() { }
        public KeyAlreadyExistsException(string message) : base(message) { }
        public KeyAlreadyExistsException(string message, System.Exception inner) : base(message, inner) { }
    }

    /// <summary>
    /// 
    /// </summary>
    internal class KeyGeneratorException : Exception
    {
        public KeyGeneratorException() : base() { }
        public KeyGeneratorException(string message) : base(message) { }
        public KeyGeneratorException(string message, System.Exception inner) : base(message, inner) { }
    }


}
