using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FirebirdSql.Data.FirebirdClient;
using System.Diagnostics;

namespace MenumateServices.WebMate.InternalClasses
{
    /// <summary>
    /// Provides number of database queries to retrieve related data
    /// </summary>
    public class WebOrderDBQueries
    {
        /// <summary>
        /// 
        /// </summary>
        public WebOrderDBQueries()
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

            internal static readonly WebOrderDBQueries instance = new WebOrderDBQueries();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static WebOrderDBQueries Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderKey"></param>
        /// <param name="inTransaction_"></param>
        /// <param name="inConnection_"></param>
        /// <returns></returns>
        public FbCommand CreateSaveWebOrderCmd(
                                int inWebOrderKey,
                                WebOrderDBInfo inInfo,
                                FbConnection inConnection_,
                                FbTransaction inTransaction_)
        {
            FbCommand result = new FbCommand(@"", inConnection_, inTransaction_);

            //....................................................

            try
            {
                result.CommandText =
                     @"INSERT INTO WEBORDERS
                (
                    WebOrder_Key,
                    GUID,
                    Name,
                    Store_Name,
                    Order_Date,
                    Expected_Date,
                    Tab_Key,
                    Time_Key,
                    Status,
                    Prepaid,
                    Contacts_Key
                )
                VALUES
                (
                    @weborderKey,
                    @guid,
                    @name,
                    @storeName,
                    @orderDate,
                    @expectedDate,
                    @tabKey,
                    @timeKey,
                    @status,
                    @prepaid,
                    @contactskey
                )";

                result.Parameters.AddWithValue("@weborderKey", inWebOrderKey);
                result.Parameters.AddWithValue("@guid", inInfo.GUID);
                result.Parameters.AddWithValue("@name", inInfo.Name);
                result.Parameters.AddWithValue("@storeName", inInfo.StoreName);
                result.Parameters.AddWithValue("@orderDate", inInfo.OrderDate);
                result.Parameters.AddWithValue("@expectedDate", inInfo.ExpectedDate);
                result.Parameters.AddWithValue("@tabKey", inInfo.TabKey);
                result.Parameters.AddWithValue("@timeKey", inInfo.TimeKey);
                result.Parameters.AddWithValue("@status", inInfo.Status);
                result.Parameters.AddWithValue("@prepaid", inInfo.PaymentRequired ? 0 : inInfo.OrderTotal);   //!inInfo.PaymentRequired
                result.Parameters.AddWithValue("@contactskey", inInfo.ContactKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateSaveWebOrderCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 260, short.MaxValue);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebDataKey"></param>
        /// <param name="inWebOrderKey"></param>
        /// <param name="inInfo"></param>
        /// <param name="inConnection_"></param>
        /// <param name="inTransaction_"></param>
        /// <returns></returns>
        public FbCommand CreateSaveWebDataCmd(
                        int inWebDataKey,
                        int inWebOrderKey,
                        string inDataCategory,
                        string inDataInfo,
                        string inDataValue,
                        FbConnection inConnection_,
                        FbTransaction inTransaction_)
        {
            FbCommand result = new FbCommand(@"", inConnection_, inTransaction_);

            //....................................................
            try
            {

                result.CommandText =
                  @"INSERT INTO WEBDATA
                (
                    WebData_Key,
                    WebOrder_Key,
                    Category,
                    Element,
                    Data
                )
                VALUES
                (
                    @webdataKey,
                    @weborderKey,
                    @category,
                    @element,
                    @data
                )";

                result.Parameters.AddWithValue("@webdataKey", inWebDataKey);
                result.Parameters.AddWithValue("@weborderKey", inWebOrderKey);
                result.Parameters.AddWithValue("@category", inDataCategory);
                result.Parameters.AddWithValue("@element", inDataInfo);
                result.Parameters.AddWithValue("@data", inDataValue);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateSaveWebDataCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 172, short.MaxValue);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="thirdPartyCode"></param>
        /// <returns></returns>
        public FbCommand CreateItemSizeKeyFromThridPartyCodeKeyCmd(
                                                            FbConnection connection,
                                                            FbTransaction transaction,
                                                            int thirdPartyCodeKey,
                                                            int inMenuKey)  //add changes for menukey...
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //....................................................
            try
            {
                string _menukey = "";
                if (inMenuKey > 0)
                {
                    _menukey = " Menu.Menu_Key = @menu_key and ";
                }

                command.CommandText = @"
                                    SELECT
                                        ITEMSIZE_KEY
                                    FROM 
                                        ItemSize
                                        Left Join ThirdPartyCodes On ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key
                                        Left Join Item on Item.Item_Key = ItemSize.Item_Key
                                        Left Join Course on  Course.Course_Key = Item.Course_Key
                                        Left Join ServingCourses On Course.ServingCourses_Key = ServingCourses.ServingCourses_Key
                                        Left Join Menu on Menu.Menu_Key =  Course.Menu_Key
                                        Left Join ArcCategories on  ItemSize.Category_Key = ArcCategories.Category_Key
                                        Left Join CategoryGroups on CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGroups_Key
                                    WHERE " +
                                        _menukey +
                                        " ThirdPartyCodes.ThirdPartyCodes_Key = @THIRD_PARTY_CODE_KEY ";
                if (inMenuKey > 0)
                {
                    command.Parameters.AddWithValue("@menu_key", inMenuKey); //make changes here for chit..   
                }
                command.Parameters.AddWithValue("@THIRD_PARTY_CODE_KEY", thirdPartyCodeKey);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateItemSizeKeyFromThridPartyCodeKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 171, short.MaxValue);
            }
            //....................................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="itemSizeKey"></param>
        /// <returns></returns>
        public FbCommand CreateCheckItemCompleteEnabledCmd(
                                                    FbConnection connection,
                                                    FbTransaction transaction,
                                                    int itemSizeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //....................................................

            try
            {
                result.CommandText = @"
                                SELECT 
                                    Item.Enabled 
                                FROM 
                                    ItemSize 
                                    Left Join Item on Item.Item_Key = ItemSize.Item_Key 
			                    WHERE 
                                    ItemSize.ITEMSIZE_KEY = @ITEMSIZE_KEY 
                                ORDER BY 
                                    Item.IAO
                                ";

                result.Parameters.AddWithValue("ITEMSIZE_KEY", itemSizeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateCheckItemCompleteEnabledCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 173, short.MaxValue);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="itemSizeKey"></param>
        /// <returns></returns>
        public FbCommand LoadBasicItemCompleteCmd(
                                        FbConnection connection,
                                        FbTransaction transaction,
                                        int itemSizeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //....................................................

            try
            {
                result.CommandText = @"
                                    SELECT
                                        Menu.Menu_Key, 
                                        Menu.Menu_Name, 
                                        Menu.Menu_Type, 
                                        Course.Course_Key, 
                                        Course.Course_ID, 
                                        Course.Course_Name, 
                                        Course.Course_Kitchen_Name, 
                                        Course.No_Default_Serving_Course,
                                        Course.ServingCourses_Key,
                                        ServingCourses.ServingCourse_Name, 
                                        ServingCourses.SERVINGCOURSE_KITCHEN_NAME, 
                                        Item.Item_Key, 
                                        Item.Item_Name, 
                                        Item.IAO, 
                                        Item.Item_Kitchen_Name, 
                                        Item.Enabled, 
                                        Item.Item_Category, 
                                        Item.Item_ID, 
                                        Item.Print_Chit, 
                                        Item.Display_Sizes, 
                                        Item.Print_Underlined, 
                                        Item.Print_Bold, 
                                        Item.Print_Colour, 
                                        Item.Print_Font, 
                                        Item.Print_Double_Width, 
                                        Item.Print_Double_Height,
                                        ItemSize.ItemSize_Key, 
                                        ItemSize.Size_ID, 
                                        ItemSize.Size_Name, 
                                        ItemSize.Size_Kitchen_Name, 
                                        ItemSize.Price,
                                        ItemSize.Special_Price, 
                                        ItemSize.GST_Percent, 
                                        ItemSize.Cost, 
                                        ItemSize.Cost_GST_Percent, 
                                        ItemSize.Free, 
                                        ItemSize.Points_Percent, 
                                        ItemSize.Setmenu_Mask, 
                                        ItemSize.Mem_Sale_Count, 
                                        ItemSize.Mem_Discount_Percent,
                                        ItemSize.Loc_Sale_Count, 
                                        ItemSize.Loc_Discount_Percent, 
                                        ItemSize.PLU, 
                                        ArcCategories.Category, 
                                        ArcCategories.Category_Key,
                                        CategoryGroups.Name Category_Group_Name, 
                                        CategoryGroups.CategoryGroups_Key, 
                                        ItemSize.ThirdPartyCodes_Key,
                                        ThirdPartyCodes.Code ThirdPartyCodeKey
                                    FROM 
                                        ItemSize
                                        Left Join ThirdPartyCodes On ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key
                                        Left Join Item on Item.Item_Key = ItemSize.Item_Key
                                        Left Join Course on  Course.Course_Key = Item.Course_Key
                                        Left Join ServingCourses On Course.ServingCourses_Key = ServingCourses.ServingCourses_Key
                                        Left Join Menu on Menu.Menu_Key =  Course.Menu_Key
                                        Left Join ArcCategories on  ItemSize.Category_Key = ArcCategories.Category_Key
                                        Left Join CategoryGroups on CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGroups_Key
                                    WHERE
                                        ItemSize.ITEMSIZE_KEY = @ITEMSIZE_KEY
                                    ORDER BY
                                        Menu.Menu_Name,
                                        Course.CAO,
                                        Item.IAO,
                                        ItemSize.ISAO
                                    ";

                result.Parameters.AddWithValue("@ITEMSIZE_KEY", itemSizeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateCheckItemCompleteEnabledCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 174, short.MaxValue);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="itemSizeKey"></param>
        /// <returns></returns>
        public FbCommand LoadBreakDownCategoriesCmd(
                                        FbConnection connection,
                                        FbTransaction transaction,
                                        int itemSizeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //.....................................................

            try
            {
                result.CommandText = @"
                                    SELECT
                                        ItemSizeCategory.ItemSize_Key,
                                        ArcCategories.Category_Key,
                                        ArcCategories.Category
                                    FROM
                                        ItemSizeCategory
                                        Left Join ArcCategories On ArcCategories.Category_Key = ItemSizeCategory.Category_Key
                                    WHERE
                                        ItemSizeCategory.ItemSize_Key = @ITEMSIZE_KEY 
                                    ORDER BY
                                        ArcCategories.Category
                                    ";

                result.Parameters.AddWithValue("@ITEMSIZE_KEY", itemSizeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadBreakDownCategoriesCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 175, short.MaxValue);
            }

            //.......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="itemSizeKey"></param>
        /// <returns></returns>
        public FbCommand LoadSalesRecipesCmd(
                                    FbConnection connection,
                                    FbTransaction transaction,
                                    int itemSizeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //.....................................................

            try
            {
                result.CommandText = @"
                                    SELECT
                                        Recipe.Recipe_Key,
                                        Recipe.Stock_Code,
                                        Recipe.Qty,
                                        StockCosts.Cost_GST_Percent,
                                        StockCosts.Cost Stock_Unit_Cost,
                                        Recipe.Stock_Location
                                    FROM
                                        Recipe
                                        Left Join StockCosts On StockCosts.Stock_Code = Recipe.Stock_Code 
                                            AND StockCosts.Location = Recipe.Stock_Location
                                    WHERE
                                        Recipe.ITEMSIZE_KEY = @ITEMSIZE_KEY
                                    ";

                result.Parameters.AddWithValue("@ITEMSIZE_KEY", itemSizeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadSalesRecipesCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 176, short.MaxValue);
            }

            //.......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="optionKey"></param>
        /// <returns></returns>
        public FbCommand LoadItemOptionCmd(
                                    FbConnection connection,
                                    FbTransaction transaction,
                                    int optionKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //.....................................................

            try
            {
                result.CommandText = @"
                                    SELECT 
                                        OPTIONS_KEY, 
                                        COURSE_KEY, 
                                        PALM_OPTION_ID,
                                        OPTION_ID, 
                                        OPTION_NAME, 
                                        OPTION_ORDER, 
                                        REDIR_ID, 
                                        ENABLED, 
                                        FLAGS, 
                                        FORCED_MASK, 
                                        OPTION_KITCHEN_NAME, 
                                        PRINT_UNDERLINED, 
                                        PRINT_BOLD, 
                                        PRINT_COLOUR, 
                                        PRINT_FONT, 
                                        PRINT_DOUBLE_WIDTH, 
                                        PRINT_DOUBLE_HEIGHT, 
                                        HANDHELD_NAME, 
                                        RECEIPT_NAME 
                                    FROM 
                                        OPTIONS 
                                    WHERE 
                                        OPTIONS_KEY = @OPTIONS_KEY
                                    ";

                result.Parameters.AddWithValue("@OPTIONS_KEY", optionKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadItemOptionCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 177, short.MaxValue);
            }

            //.......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="currentTimeKey"></param>
        /// <returns></returns>
        public FbCommand OpenSaleStartTimeCmd(
                                        FbConnection connection,
                                        FbTransaction transaction,
                                        int currentTimeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //........................................................

            try
            {
                result.CommandText = @"
                                 INSERT INTO TURNAROUNDTIMES(
                                    TIME_KEY,
                                    SALE_START_TIME)
                                 VALUES(
                                    @TIME_KEY,
                                    @SALE_START_TIME)
                                 ";

                result.Parameters.AddWithValue("@TIME_KEY", currentTimeKey);
                result.Parameters.AddWithValue("@SALE_START_TIME", DateTime.Now);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in OpenSaleStartTimeCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 178, short.MaxValue);
            }

            //.........................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="currentTimeKey"></param>
        /// <returns></returns>
        public FbCommand CloseSaleStartTimeCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                int timeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //........................................................

            try
            {
                result.CommandText = @"
                                 UPDATE TURNAROUNDTIMES
                                 SET 
                                    SALE_END_TIME = @SALE_END_TIME,
                                    MAKE_START_TIME = @MAKE_START_TIME
                                 WHERE
                                    TIME_KEY = @TIME_KEY
                                 ";

                result.Parameters.AddWithValue("@TIME_KEY", timeKey);
                result.Parameters.AddWithValue("@SALE_END_TIME", DateTime.Now);
                result.Parameters.AddWithValue("@MAKE_START_TIME", DateTime.Now);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CloseSaleStartTimeCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 179, short.MaxValue);
            }

            //.........................................................

            return result;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="webOrderKey"></param>
        /// <param name="timeKey"></param>
        /// <returns></returns>
        public FbCommand SetWebOrderTimeKeyCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                int webOrderKey,
                                int timeKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    UPDATE WEBORDERS 
                                    SET 
                                        TIME_KEY = @TIME_KEY 
                                    WHERE 
                                        WEBORDER_KEY = @WEBORDER_KEY
                                    ";

                command.Parameters.AddWithValue("@TIME_KEY", timeKey);
                command.Parameters.AddWithValue("@WEBORDER_KEY", webOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetWebOrderTimeKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 180, short.MaxValue);
            }

            //............................................

            return command;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        public FbCommand GetTabKeyByWebOrderKeyCmd(
                                            FbConnection connection,
                                            FbTransaction transaction,
                                            int webOrderKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        TAB_KEY 
                                    FROM 
                                        WEBORDERS 
                                    WHERE 
                                        WEBORDER_KEY = @WEBORDER_KEY
                                    ";

                command.Parameters.AddWithValue("@WEBORDER_KEY", webOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTabKeyByWebOrderKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="tabKey"></param>
        /// <param name="tabName"></param>
        /// <param name="tabType"></param>
        /// <returns></returns>
        public FbCommand CreateTabCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                int tabKey,
                                string tabName,
                                int tabType,
                                string id_number)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    INSERT INTO TAB 
                                    (
                                        TAB_KEY,
                                        CREDIT_LIMIT,
                                        TAB_PERMANENT,
                                        TAB_TYPE,
                                        TAB_NAME,
                                        ID_NUMBER
                                    ) 
                                    VALUES 
                                    (
                                        @TAB_KEY,
                                        @CREDIT_LIMIT,
                                        @TAB_PERMANENT,
                                        @TAB_TYPE,
                                        @TAB_NAME,
                                        @ID_NUMBER
                                    )
                                    ";

                command.Parameters.AddWithValue("@TAB_KEY", tabKey);
                command.Parameters.AddWithValue("@CREDIT_LIMIT", -1);
                command.Parameters.AddWithValue("@TAB_PERMANENT", 'F');
                command.Parameters.AddWithValue("@TAB_TYPE", tabType);
                command.Parameters.AddWithValue("@TAB_NAME", tabName);
                command.Parameters.AddWithValue("@ID_NUMBER", id_number);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateTabCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 182, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="tabKey"></param>
        /// <param name="credit"></param>
        /// <returns></returns>
        public FbCommand CreditTabCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                int tabKey,
                                decimal credit)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    UPDATE TAB
                                    SET
                                        CREDIT = @CREDIT
                                    WHERE
                                        TAB_KEY = @TAB_KEY
                                    ";

                command.Parameters.AddWithValue("@CREDIT", credit);
                command.Parameters.AddWithValue("@TAB_KEY", tabKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreditTabCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 183, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="categoryName"></param>
        /// <returns></returns>
        public FbCommand SetTabKeyForWebOrderCmd(
                                        FbConnection connection,
                                        FbTransaction transaction,
                                        int webOrderKey,
                                        int tabKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    UPDATE WEBORDERS 
                                    SET 
                                        TAB_KEY = @TAB_KEY 
                                    WHERE 
                                        WEBORDER_KEY = @WEBORDER_KEY
                                    ";

                command.Parameters.AddWithValue("@TAB_KEY", tabKey);
                command.Parameters.AddWithValue("@WEBORDER_KEY", webOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetTabKeyForWebOrderCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 184, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        public FbCommand GetTimeKeyByWebOrderKeyCmd(
                            FbConnection inConnection,
                            FbTransaction inTransaction,
                            int inWebOrderKey)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        TIME_KEY 
                                    FROM 
                                        WEBORDERS 
                                    WHERE 
                                        WEBORDER_KEY = @WEBORDER_KEY
                                    ";

                command.Parameters.AddWithValue("@WEBORDER_KEY", inWebOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTimeKeyByWebOrderKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 185, short.MaxValue);
                
            }
            return command;
            //............................................

            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inTimeKey"></param>
        /// <returns></returns>
        public FbCommand CreateTimeCmd(
                                FbConnection inConnection,
                                FbTransaction inTransaction,
                                int inTimeKey)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);
            try
            {            

                //...........................................

                command.CommandText = @"
                                    INSERT INTO TURNAROUDTIMES 
                                    (
                                        TIME_KEY,
                                        MAKE_START_TIME
                                    ) 
                                    VALUES 
                                    (
                                        @timeKey,
                                        @makeStartTime
                                    )
                                    ";

                command.Parameters.AddWithValue("@timeKey", inTimeKey);
                command.Parameters.AddWithValue("@makeStartTime", DateTime.Now);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateTimeCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 186, short.MaxValue);
            }

            //............................................
            return command;

            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inWebOrderKey"></param>
        /// <param name="inTimeKey"></param>
        /// <returns></returns>
        public FbCommand SetTimeKeyForWebOrderCmd(
                            FbConnection inConnection,
                            FbTransaction inTransaction,
                            int inWebOrderKey,
                            int inTimeKey)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);
            try
            {
                //...........................................
                command.CommandText = @"
                                    UPDATE WEBORDERS 
                                    SET 
                                        TIME_KEY = @timekey 
                                    WHERE 
                                        WEBORDER_KEY = @webOrderKey
                                    ";

                command.Parameters.AddWithValue("@timekey", inTimeKey);
                command.Parameters.AddWithValue("@webOrderKey", inWebOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetTimeKeyForWebOrderCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 187, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inWebOrderKey"></param>
        /// <returns></returns>
        public FbCommand GetContactKeyByWebOrderKeyCmd(
                            FbConnection inConnection,
                            FbTransaction inTransaction,
                            int inWebOrderKey)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        CONTACTS_KEY 
                                    FROM 
                                        WEBORDERS 
                                    WHERE 
                                        WEBORDER_KEY = @WEBORDER_KEY
                                    ";

                command.Parameters.AddWithValue("@WEBORDER_KEY", inWebOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetContactKeyByWebOrderKeyCmd " + e.Message, e);
               //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 188, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inWebOrderKey"></param>
        /// <param name="inInfo"></param>
        /// <returns></returns>
        public FbCommand CreateContactCmd(
                               FbConnection inConnection,
                               FbTransaction inTransaction,
                               int inContactsKey,
                               WebOrderDBInfo inInfo)
        {
            FbCommand result = new FbCommand(@"", inConnection, inTransaction);

            //....................................................

            try
            {
                result.CommandText =
                      @"INSERT INTO CONTACTS
                (
                    Contacts_Key,
                    Contact_ID,
                    Contact_Type,
                    Initials,
                    Location_Address,
                    Phone,
                    Total_Spent,
                    PIN,
                    Access_Level,
                    Palm_Large_Font,
                    Palm_Advanced_User,
                    Palm_Timeout,
                    Tab_Enabled,
                    Payroll_ID,
                    Contacts_3rdParty_Key,
                    Palm_Screen_Lock,
                    Palm_Classic_Mode,
                    Palm_Sound_Level,
                    Palm_Get_All_Tables,
                    Site_ID,
                    Known_As,
                    Point_Rules,
                    Account_Profile,
                    Hourly_Rate,
                    Earnt_Points,
                    Loaded_Points                   
                )
                VALUES
                (
                    @contactsKey,
                    @contactID,
                    @contactType,
                    @initials,
                    @locationAddress,
                    @phone,
                    @totalSpent,
                    @pin,
                    @accessLevel,
                    @palmLargeFont,
                    @palmAdvancedUser,
                    @palmTimeout,
                    @tabEnabled,
                    @payrollID,
                    @contacts3rsPartyKey,
                    @palmScreenLock,
                    @palmClassicMode,
                    @palmSoundLevel,
                    @palmGetAllTables,
                    @siteID,
                    @knownAs,
                    @pointRules,
                    @accountProfile,
                    @horlyRate,
                    @earntPoints,
                    @loadedPoints
               )";

                result.Parameters.AddWithValue("@contactsKey", inContactsKey);
                result.Parameters.AddWithValue("@contactID", 0);
                result.Parameters.AddWithValue("@contactType", 0);
                result.Parameters.AddWithValue("@initials", @"");
                result.Parameters.AddWithValue("@locationAddress", addressFromWebInfo(inInfo));
                result.Parameters.AddWithValue("@phone", inInfo.Phone);
                result.Parameters.AddWithValue("@totalSpent", 0.00);
                result.Parameters.AddWithValue("@pin", @"");
                result.Parameters.AddWithValue("@accessLevel", 0);
                result.Parameters.AddWithValue("@palmLargeFont", false);
                result.Parameters.AddWithValue("@palmAdvancedUser", false);
                result.Parameters.AddWithValue("@palmTimeout", 30);
                result.Parameters.AddWithValue("@tabEnabled", false);
                result.Parameters.AddWithValue("@payrollID", 0);
                result.Parameters.AddWithValue("@contacts3rsPartyKey", 0);
                result.Parameters.AddWithValue("@palmScreenLock", true);
                result.Parameters.AddWithValue("@palmClassicMode", false);
                result.Parameters.AddWithValue("@palmSoundLevel", 2);
                result.Parameters.AddWithValue("@palmGetAllTables", true);
                result.Parameters.AddWithValue("@siteID", 0);
                result.Parameters.AddWithValue("@knownAs", inInfo.Name);
                result.Parameters.AddWithValue("@pointRules", 0);
                result.Parameters.AddWithValue("@accountProfile", 0);
                result.Parameters.AddWithValue("@horlyRate", 0.00);
                result.Parameters.AddWithValue("@earntPoints", 0.00);
                result.Parameters.AddWithValue("@loadedPoints", 0.00);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateContactCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 189, short.MaxValue);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inInfo"></param>
        /// <returns></returns>
        public FbCommand GetContactInDBCmd(
                               FbConnection inConnection,
                               FbTransaction inTransaction,
                               WebOrderDBInfo inInfo)
        {
            FbCommand result = new FbCommand(@"", inConnection, inTransaction);

            //...........................................

            try
            {
                result.CommandText = @"
                                    SELECT 
                                        COUNT(Contacts_key) 
                                    FROM 
                                        Contacts
									WHERE 
                                        Phone = @phone OR KnownAs = @knownAs OR Location_Address like @CATEGORY
                                    ";

                result.Parameters.AddWithValue("@locationAddress", addressFromWebInfo(inInfo));
                result.Parameters.AddWithValue("@phone", inInfo.Phone);
                result.Parameters.AddWithValue("@knownAs", inInfo.Name);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetContactInDBCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 190, short.MaxValue);
            }

            //............................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inWebOrderKey"></param>
        /// <param name="inContactsKey"></param>
        /// <returns></returns>
        public FbCommand SetContactsKeyForWebOrderCmd(
                               FbConnection inConnection,
                               FbTransaction inTransaction,
                               int inWebOrderKey,
                               int inContactsKey)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    UPDATE WEBORDERS 
                                    SET 
                                        CONTACTS_KEY = @contactskey 
                                    WHERE 
                                        WEBORDER_KEY = @webOrderKey
                                    ";

                command.Parameters.AddWithValue("@contactskey", inContactsKey);
                command.Parameters.AddWithValue("@webOrderKey", inWebOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetContactsKeyForWebOrderCmd " + e.Message, e);
               //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 191, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="categoryName"></param>
        /// <returns></returns>
        public FbCommand GetArchiveCategoryCmd(
                                        FbConnection connection,
                                        FbTransaction transaction,
                                        string categoryName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        CATEGORY_KEY,
                                        CATEGORY 
                                    FROM 
                                        ARCCATEGORIES
									WHERE 
                                        CATEGORY = @CATEGORY
                                    ";

                command.Parameters.AddWithValue("@CATEGORY", categoryName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetArchiveCategoryCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 192, short.MaxValue);
            }
            //............................................
            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="webOrderKey"></param>
        /// <returns></returns>
        public FbCommand GetGuidFromOrderKeyCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                int webOrderKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        GUID 
                                    FROM 
                                        WEBORDERS 
                                    WHERE 
                                        WEBORDER_KEY = @WEBORDER_KEY
                                    ";

                command.Parameters.AddWithValue("@WEBORDER_KEY", webOrderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetGuidFromOrderKeyCmd " + e.Message, e); 
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 193, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="itemId"></param>
        /// <param name="sizeName"></param>
        /// <param name="terminalName"></param>
        /// <param name="transactionNumber"></param>
        /// <returns></returns>
        public FbCommand CheckDuplicateOrderCmd(
                                            FbConnection connection,
                                            FbTransaction transaction,
                                            int itemId,
                                            string sizeName,
                                            string terminalName,
                                            string transactionNumber)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {

                command.CommandText = @"
                                    SELECT 
                                        ORDER_KEY 
                                    FROM 
                                        ORDERS
		                            WHERE
		                                ITEM_ID = @ITEM_ID 
                                        AND SIZE_NAME = @SIZE_NAME 
                                        AND TERMINAL_NAME = @TERMINAL_NAME 
                                        AND TRANSNO = @TRANSNO
                                    ";

                command.Parameters.AddWithValue("@ITEM_ID", itemId);
                command.Parameters.AddWithValue("@SIZE_NAME", sizeName);
                command.Parameters.AddWithValue("@TERMINAL_NAME", terminalName);
                command.Parameters.AddWithValue("@TRANSNO", transactionNumber);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckDuplicateOrderCmd " + e.Message, e); 
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 194, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="orderDbItem"></param>
        /// <returns></returns>
        public FbCommand SetOrderCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                WebOrderDBBaseItem orderDbItem)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    INSERT INTO ORDERS 
                                    (
                                        ORDER_KEY,
			                            TAB_KEY,
			                            ITEM_NAME,
			                            ITEM_ID,
			                            SIZE_NAME,
			                            NOTE,
			                            PARTY_NAME,
                                        TABLE_NUMBER,
                                        TABLE_NAME,
			                            SEATNO,
                                        PRICE,
                                        PRINTED,
                                        TRANSNO,
			                            ORDER_TYPE,
                                        TERMINAL_NAME,
			                            MENU_NAME,
                                        TAB_NAME,
                                        COURSE_NAME,
                                        HAPPYHOUR,
			                            ORDER_LOCATION,
                                        TAB_TYPE,
                                        TIME_STAMP,
                                        COST,
                                        LOYALTY_KEY,
                                        MASTER_CONTAINER,
			                            SETMENU_MASK,
                                        SETMENU_GROUP,
                                        ITEM_CATEGORY,
			                            SECURITY_REF,
                                        TIME_KEY,
			                            GST_PERCENT,
			                            COST_GST_PERCENT,
                                        SIDE_ORDER_KEY,
			                            QTY,
			                            DISCOUNT,
			                            DISCOUNT_REASON,
			                            REDEEMED,
			                            ITEM_KITCHEN_NAME,
			                            SIZE_KITCHEN_NAME,
			                            COURSE_KITCHEN_NAME,
			                            POINTS_PERCENT,
			                            CATEGORY_KEY,
			                            THIRDPARTYCODES_KEY,
			                            MEMBER_FREE_SALE_COUNT,
			                            MEMBER_FREE_SALE_DISCOUNT,
			                            LOCATION_FREE_SALE_COUNT,
			                            LOCATION_FREE_SALE_DISCOUNT,
			                            IS_FREE,
			                            SERVINGCOURSES_KEY,
			                            PRICE_LEVEL0,
			                            PRICE_LEVEL1,
			                            ITEM_TYPE,
			                            MENU_ITEM_KEY,
		                                PLU,
		                                ORDER_TYPE_MESSAGE,
			                            CONTACTS_KEY,
			                            ACTIVECHITNUMBER_KEY, 
                                        PATRON_COUNT
                                    )
			                        VALUES (
			                            @ORDER_KEY,
			                            @TAB_KEY,
			                            @ITEM_NAME,
			                            @ITEM_ID,
			                            @SIZE_NAME,
			                            @NOTE,
			                            @PARTY_NAME,
                                        @TABLE_NUMBER,
			                            @TABLE_NAME,
                                        @SEATNO,
			                            @PRICE,
			                            @PRINTED,
			                            @TRANSNO,
			                            @ORDER_TYPE,
			                            @TERMINAL_NAME,
			                            @MENU_NAME,
			                            @TAB_NAME,
			                            @COURSE_NAME,
			                            @HAPPYHOUR,
			                            @ORDER_LOCATION,
			                            @TAB_TYPE,
			                            @TIME_STAMP,
                                        @COST,
                                        @LOYALTY_KEY,
                                        @MASTER_CONTAINER,
			                            @SETMENU_MASK,
			                            @SETMENU_GROUP,
			                            @ITEM_CATEGORY,
			                            @SECURITY_REF,
                                        @TIME_KEY,
                                        @GST_PERCENT,
                                        @COST_GST_PERCENT,
                                        @SIDE_ORDER_KEY,
			                            @QTY,
			                            @DISCOUNT,
			                            @DISCOUNT_REASON,
			                            @REDEEMED,
			                            @ITEM_KITCHEN_NAME,
			                            @SIZE_KITCHEN_NAME,
			                            @COURSE_KITCHEN_NAME,
			                            @POINTS_PERCENT,
			                            @CATEGORY_KEY,
			                            @THIRDPARTYCODES_KEY,
			                            @MEMBER_FREE_SALE_COUNT,
			                            @MEMBER_FREE_SALE_DISCOUNT,
			                            @LOCATION_FREE_SALE_COUNT,
			                            @LOCATION_FREE_SALE_DISCOUNT,
			                            @IS_FREE,
			                            @SERVINGCOURSES_KEY,
			                            @PRICE_LEVEL0,
			                            @PRICE_LEVEL1,
			                            @ITEM_TYPE,
			                            @MENU_ITEM_KEY,
		                                @PLU,
		                                @ORDER_TYPE_MESSAGE,
			                            @CONTACTS_KEY,
			                            @ACTIVECHITNUMBER_KEY, 
                                        @PATRON_COUNT)
                                    ";

                command.Parameters.AddWithValue("@ORDER_KEY", orderDbItem.OrderKey);
                command.Parameters.AddWithValue("@TAB_KEY", orderDbItem.TabKey);
                command.Parameters.AddWithValue("@ITEM_NAME", orderDbItem.ItemName);
                command.Parameters.AddWithValue("@ITEM_ID", orderDbItem.ItemId);
                command.Parameters.AddWithValue("@SIZE_NAME", orderDbItem.SizeName);
                command.Parameters.AddWithValue("@NOTE", orderDbItem.Note);
                command.Parameters.AddWithValue("@PARTY_NAME", orderDbItem.PartyName);
                command.Parameters.AddWithValue("@TABLE_NUMBER", orderDbItem.TableNumber);
                command.Parameters.AddWithValue("@TABLE_NAME", QueryUtilities.GetSubstring(orderDbItem.TabContainerName, 1, 25));
                command.Parameters.AddWithValue("@SEATNO", orderDbItem.SeatNumber);
                command.Parameters.AddWithValue("@PRICE", orderDbItem.Price);
                command.Parameters.AddWithValue("@PRINTED", 'F');
                command.Parameters.AddWithValue("@TRANSNO", orderDbItem.TransactionNumber);
                command.Parameters.AddWithValue("@ORDER_TYPE", orderDbItem.OrderType);
                command.Parameters.AddWithValue("@TERMINAL_NAME", orderDbItem.TerminalName);
                command.Parameters.AddWithValue("@MENU_NAME", orderDbItem.MenuName);
                command.Parameters.AddWithValue("@TAB_NAME", QueryUtilities.GetSubstring(orderDbItem.TabName, 1, 32));
                command.Parameters.AddWithValue("@COURSE_NAME", orderDbItem.CourseName);
                command.Parameters.AddWithValue("@HAPPYHOUR", orderDbItem.HappyHour ? 'T' : 'F');
                command.Parameters.AddWithValue("@ORDER_LOCATION", orderDbItem.OrderLocation);
                command.Parameters.AddWithValue("@TAB_TYPE", orderDbItem.TabType);
                command.Parameters.AddWithValue("@TIME_STAMP", orderDbItem.TimeStamp);
                command.Parameters.AddWithValue("@COST", orderDbItem.Cost);
                command.Parameters.AddWithValue("@LOYALTY_KEY", orderDbItem.LoyaltyKey);
                command.Parameters.AddWithValue("@MASTER_CONTAINER", orderDbItem.MasterContainer);
                command.Parameters.AddWithValue("@SETMENU_MASK", orderDbItem.SetMenuMask);
                command.Parameters.AddWithValue("@SETMENU_GROUP", orderDbItem.SetMenuGroup);
                command.Parameters.AddWithValue("@ITEM_CATEGORY", "");
                command.Parameters.AddWithValue("@SECURITY_REF", orderDbItem.SecurityRef);
                command.Parameters.AddWithValue("@TIME_KEY", orderDbItem.TimeKey);
                command.Parameters.AddWithValue("@GST_PERCENT", orderDbItem.GSTPercent);
                command.Parameters.AddWithValue("@COST_GST_PERCENT", orderDbItem.CostGSTPercent);
                command.Parameters.AddWithValue("@QTY", orderDbItem.Qty);
                command.Parameters.AddWithValue("@DISCOUNT", orderDbItem.Discount);
                command.Parameters.AddWithValue("@DISCOUNT_REASON", QueryUtilities.GetSubstring(orderDbItem.DiscountReason, 1, 40));
                command.Parameters.AddWithValue("@REDEEMED", 0.0f);
                command.Parameters.AddWithValue("@ITEM_KITCHEN_NAME", orderDbItem.ItemKitchenName);
                command.Parameters.AddWithValue("@SIZE_KITCHEN_NAME", orderDbItem.SizeKitchenName);
                command.Parameters.AddWithValue("@COURSE_KITCHEN_NAME", orderDbItem.CourseKitchenName);
                command.Parameters.AddWithValue("@POINTS_PERCENT", orderDbItem.PointsPercent);
                command.Parameters.AddWithValue("@CATEGORY_KEY", orderDbItem.Categories.PrimaryArcCategoryKey);
                command.Parameters.AddWithValue("@THIRDPARTYCODES_KEY", orderDbItem.ThirdPartyCodeKey);
                command.Parameters.AddWithValue("@MEMBER_FREE_SALE_COUNT", orderDbItem.MemberFreeSaleCount);
                command.Parameters.AddWithValue("@MEMBER_FREE_SALE_DISCOUNT", orderDbItem.MemberFreeSaleDiscount);
                command.Parameters.AddWithValue("@LOCATION_FREE_SALE_COUNT", orderDbItem.LocationFreeSaleCount);
                command.Parameters.AddWithValue("@LOCATION_FREE_SALE_DISCOUNT", orderDbItem.LocationFreeSaleDiscount);
                command.Parameters.AddWithValue("@IS_FREE", orderDbItem.IsFree ? 'T' : 'F');
                command.Parameters.AddWithValue("@SERVINGCOURSES_KEY", orderDbItem.ServingCoursesKey);
                command.Parameters.AddWithValue("@PRICE_LEVEL0", orderDbItem.PriceLevel0);
                command.Parameters.AddWithValue("@PRICE_LEVEL1", orderDbItem.PriceLevel1);
                command.Parameters.AddWithValue("@ITEM_TYPE", orderDbItem.ItemType);
                command.Parameters.AddWithValue("@MENU_ITEM_KEY", orderDbItem.MenuItemKey); // ItemOrderedFrom->ItemKey
                command.Parameters.AddWithValue("@PLU", orderDbItem.PLU);
                command.Parameters.AddWithValue("@ORDER_TYPE_MESSAGE", orderDbItem.OrderTypeMessage);
                command.Parameters.AddWithValue("@CONTACTS_KEY", orderDbItem.ContactsKey);
                command.Parameters.AddWithValue("@ACTIVECHITNUMBER_KEY", DBNull.Value);
                command.Parameters.AddWithValue("@PATRON_COUNT", orderDbItem.PatronCount);


                if (orderDbItem.SideOrderKey > 0)
                    command.Parameters.AddWithValue("@SIDE_ORDER_KEY", orderDbItem.SideOrderKey);
                else
                    command.Parameters.AddWithValue("@SIDE_ORDER_KEY", DBNull.Value);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderCmd " + e.Message, e); 
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 195, short.MaxValue);
            }

            //............................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="orderKey"></param>
        /// <param name="categories"></param>
        /// <returns></returns>
        public FbCommand SetOrderBreakdownCategoryCmd(
                                                FbConnection connection,
                                                FbTransaction transaction,
                                                int orderKey,
                                                WebOrderDBCategory category)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //......................................................

            try
            {
                command.CommandText = @"
                                    INSERT INTO ORDERCATEGORY 
                                    (
						                ORDER_KEY,
						                CATEGORY_KEY
                                    )
					                VALUES 
                                    (
						                @ORDER_KEY,
						                @CATEGORY_KEY
                                    )
                                    ";

                command.Parameters.AddWithValue("@ORDER_KEY", orderKey);
                command.Parameters.AddWithValue("@CATEGORY_KEY", category.CategoryKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e); 
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
            }

            //......................................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="orderKey"></param>
        /// <param name="recipe"></param>
        /// <returns></returns>
        public FbCommand SetOrderRecipeCmd(
                                        FbConnection connection,
                                        FbTransaction transaction,
                                        int orderKey,
                                        WebOrderDBRecipe recipe)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //......................................................

            try
            {
                command.CommandText = @"
                                    INSERT INTO ORDERRECIPE 
                                    (
						                ORDERRECIPE_KEY,
						                ORDER_KEY,
						                STOCK_CODE,
						                QTY,
						                COST,
						                STOCK_LOCATION
                                    ) 
					                VALUES 
                                    (
						                @ORDERRECIPE_KEY,
						                @ORDER_KEY,
						                @STOCK_CODE,
						                @QTY,
						                @COST,
						                @STOCK_LOCATION
                                    )
                                    ";

                command.Parameters.AddWithValue("@ORDERRECIPE_KEY", recipe.RecipeKey);
                command.Parameters.AddWithValue("@ORDER_KEY", orderKey);
                command.Parameters.AddWithValue("@STOCK_CODE", recipe.StockCode);
                command.Parameters.AddWithValue("@QTY", recipe.Quantity);
                command.Parameters.AddWithValue("@COST", recipe.Cost);
                command.Parameters.AddWithValue("@STOCK_LOCATION", recipe.StockLocation);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderRecipeCmd " + e.Message, e); 
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 215, short.MaxValue);
            }

            //......................................................

            return command;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="connection"></param>
        /// <param name="transaction"></param>
        /// <param name="orderKey"></param>
        /// <param name="option"></param>
        /// <returns></returns>
        public FbCommand SetOrderOptionCmd(
                                FbConnection connection,
                                FbTransaction transaction,
                                int orderKey,
                                WebOrderDBItemOption option)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //......................................................

            try
            {
                command.CommandText = @"
                                    INSERT INTO ORDEROPTION 
                                    (
					                    ORDEROPTION_KEY,
					                    ORDER_KEY,
					                    OPTION_NAME,
					                    OPTION_ID,
					                    PLUS,
					                    OPTION_KITCHEN_NAME
                                    ) 
				                    VALUES 
                                    (
					                    @ORDEROPTION_KEY,
					                    @ORDER_KEY,
					                    @OPTION_NAME,
					                    @OPTION_ID,
					                    @PLUS,
					                    @OPTION_KITCHEN_NAME
                                    )
                                    ";

                command.Parameters.AddWithValue("@ORDEROPTION_KEY", option.OptionKey);
                command.Parameters.AddWithValue("@ORDER_KEY", orderKey);
                command.Parameters.AddWithValue("@OPTION_NAME", option.Name);
                command.Parameters.AddWithValue("@OPTION_ID", option.OptionID);
                command.Parameters.AddWithValue("@PLUS", option.IsPlus ? 'T' : 'F');
                command.Parameters.AddWithValue("@OPTION_KITCHEN_NAME", option.KitchenName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderRecipeCmd " + e.Message, e); 
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 216, short.MaxValue);
            }

            //......................................................

            return command;
        }

        public FbCommand CreateWebOrderAcceptedCmd(
                                                FbConnection inConnection,
                                                FbTransaction inTransaction,
                                                       string inOrderHandle)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //......................................................

            try
            {
                command.CommandText = @"
                                    SELECT COUNT(WebOrder_Key) as orderCount
                                    FROM WEBORDERS
                                    WHERE guid = @orderHandle
                                    ";

                command.Parameters.AddWithValue("@orderHandle", inOrderHandle);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateWebOrderAcceptedCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 217, short.MaxValue);
            }

            //......................................................

            return command;
        }

        /// <summary>
        /// AQUI 06/12/2013
        /// </summary>
        /// <param name="inConnection"></param>
        /// <param name="inTransaction"></param>
        /// <param name="inThirdPartyCode"></param>
        /// <returns></returns>
        public FbCommand CreateReadThirdPartyCodeKeyCmd(
                                   FbConnection inConnection,
                                   FbTransaction inTransaction,
                                   string inThirdPartyCode,
                                   int inMenukey)  //changes for menu_key
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //......................................................

            try
            {
                string _menukey = "";
                if (inMenukey > 0)
                {
                    _menukey = " Menu.Menu_Key = @menu_key and ";
                }
                command.CommandText = @"
                                    SELECT                                        
                                        ItemSize.ThirdPartyCodes_Key as tpcKey                                        
                                    FROM 
                                        ItemSize
                                        Left Join ThirdPartyCodes On ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key
                                        Left Join Item on Item.Item_Key = ItemSize.Item_Key
                                        Left Join Course on  Course.Course_Key = Item.Course_Key
                                        Left Join ServingCourses On Course.ServingCourses_Key = ServingCourses.ServingCourses_Key
                                        Left Join Menu on Menu.Menu_Key =  Course.Menu_Key
                                        Left Join ArcCategories on  ItemSize.Category_Key = ArcCategories.Category_Key
                                        Left Join CategoryGroups on CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGroups_Key
                                    WHERE " +
                                        _menukey +
                                        " ThirdPartyCodes.Code = @code ";
                if (inMenukey > 0)
                {
                    command.Parameters.AddWithValue("@menu_key", inMenukey);
                }
                command.Parameters.AddWithValue("@code", inThirdPartyCode);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateReadThirdPartyCodeKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 218, short.MaxValue);
            }
            //......................................................

            return command;
        }

        public FbCommand GetChitDetailsCmd(
                                   FbConnection inConnection,
                                   FbTransaction inTransaction,
                                   string orderType)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //......................................................

            try
            {

                string order_type = "";
                if (orderType == "Delivery")
                {
                    order_type = " ONLINE_DELIVERY_ORDER = @value ";
                }
                else
                {
                    order_type = " ONLINE_PICKUP_ORDER = @value ";  
                }
                command.CommandText = @" SELECT CHITNUMBER_KEY, DEFAULT_MENU FROM CHITNUMBER WHERE " + order_type  ;
                //......................................................
                command.Parameters.AddWithValue("@value", "T");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetChitDetailsCmd " + e.Message, e);
               //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 219, short.MaxValue);
            }

            return command;
        }

        public FbCommand GetMenuKeyCmd(
                                   FbConnection inConnection,
                                   FbTransaction inTransaction,
                                   string menuName)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            //......................................................

            try
            {
                command.CommandText = @"
                                    SELECT a.MENU_KEY
                                    FROM MENU a
                                    Where a.MENU_NAME = @menu_name
                                    ";

                command.Parameters.AddWithValue("@menu_name", menuName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetMenuKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 220, short.MaxValue);
            }
            //......................................................

            return command;
        }

        public FbCommand GetWebmateInterfaceCmd(
                                           FbConnection inConnection,
                                           FbTransaction inTransaction)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            try
            {
                int value = 2117;
                //......................................................
                command.CommandText = @" SELECT a.VARIABLES_KEY, a.INTEGER_VAL FROM VARSPROFILE a where a.VARIABLES_KEY = @value ";
                //......................................................
                command.Parameters.AddWithValue("@value", value);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetWebmateInterfaceCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 221, short.MaxValue);
            }

            return command;
        }

        public FbCommand UpdateToNotifyWebmateInterfaceEnableOrDisableCmd(
                                   FbConnection inConnection,
                                   FbTransaction inTransaction)
        {
            FbCommand command = new FbCommand(@"", inConnection, inTransaction);

            try
            {
                int value = 4133;
                int int_val = 1;
                //......................................................
                command.CommandText = @" UPDATE VARSPROFILE a SET a.INTEGER_VAL = @int_val WHERE a.VARIABLES_KEY = @value ";
                //......................................................
                command.Parameters.AddWithValue("@value", value);
                command.Parameters.AddWithValue("@int_val", int_val);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in UpdateToNotifyWebmateInterfaceEnableOrDisableCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" +e.StackTrace, EventLogEntryType.Error, 222, short.MaxValue);
            }
            return command;
        }


        #endregion

        #region PRIVATE
        string addressFromWebInfo(WebOrderDBInfo inInfo)
        {
            return string.Format(@"{0} {1}, {2}, {3}. {4}",
                                 inInfo.StreetNo,
                                 inInfo.StreetName,
                                 inInfo.Suburb,
                                 inInfo.City,
                                 inInfo.Country
                                 );
        }
        #endregion
    }
}

/// <summary>
/// 
/// </summary>
public static class QueryUtilities
{
    /// <summary>
    /// 
    /// </summary>
    /// <param name="original"></param>
    /// <param name="start"></param>
    /// <param name="maxLength"></param>
    /// <returns></returns>
    public static string GetSubstring(string original, int start, int maxLength)
    {
        if (!string.IsNullOrEmpty(original)
            && maxLength < original.Length)
            return original.Substring(start, maxLength);
        else
            return original;
    }
}
