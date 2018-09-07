using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using Loyaltymate.Enum;

namespace MenumateServices.DTO.OnlineOrdering.DBOrders
{

    public class OnlineOrderDBQueries
    {
        /// <summary>
        /// 
        /// </summary>
        public OnlineOrderDBQueries()
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

            internal static readonly OnlineOrderDBQueries instance = new OnlineOrderDBQueries();
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        public static OnlineOrderDBQueries Instance
        {
            get
            {
                return Nested.instance;
            }
        }


        public FbCommand GetTabKeyByTabName(FbConnection connection, FbTransaction transaction, string tabName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        TAB_KEY 
                                    FROM 
                                        ORDERS 
                                    WHERE 
                                        TAB_NAME = @TAB_NAME
                                    ";

                command.Parameters.AddWithValue("@TAB_NAME", tabName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTabKeyByWebOrderKeyCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public bool GetTabExists(FbConnection connection, FbTransaction transaction, int tabKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        TAB_KEY 
                                    FROM 
                                        TAB 
                                    WHERE 
                                        TAB_KEY = @TAB_KEY
                                    ";
                command.Parameters.AddWithValue("@TAB_KEY", tabKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTabExists " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command.ExecuteNonQuery() > 0;
        }

        public FbCommand GetSeatForTable(FbConnection connection, FbTransaction transaction, int tableKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT 
                                        SEAT.SEAT_KEY 
                                    FROM 
                                        SEAT INNER JOIN TABLES ON SEAT.TABLE_KEY = TABLES.TABLE_KEY 
                                    WHERE 
                                        TABLES.TABLE_KEY = @TABLE_KEY AND SEAT.SEATNO = @SEATNO
                                    ";

                command.Parameters.AddWithValue("@TABLE_KEY", tableKey);
                command.Parameters.AddWithValue("@SEATNO", 1);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSeatForTable " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand GetTabKeyBySeatKey(FbConnection connection, FbTransaction transaction, int seatKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                   SELECT TAB_KEY 
			                        FROM 
				                        SEAT 
			                        WHERE 
				                        SEAT_KEY = @SEAT_KEY
                                    ";

                command.Parameters.AddWithValue("@SEAT_KEY", seatKey);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTabKeyBySeatKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand SetSeatTabKey(FbConnection connection, FbTransaction transaction, int seatKey, int tabKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                         UPDATE SEAT
                                           SET TAB_KEY = @TAB_KEY 
		                                WHERE 
		                                SEAT_KEY = @SEAT_KEY
                                    ";

                command.Parameters.AddWithValue("@SEAT_KEY", seatKey);
                command.Parameters.AddWithValue("@TAB_KEY", tabKey);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTabKeyBySeatKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand CreateSeatForTable(FbConnection connection, FbTransaction transaction, int tableKey, int seatKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                   INSERT INTO SEAT (
                                            SEAT_KEY,
                                            TABLE_KEY,
                                            SEATNO) 
                                         VALUES (
                                            @SEAT_KEY,
                                            @TABLE_KEY,
                                            @SEATNO);
                                    ";

                command.Parameters.AddWithValue("@SEAT_KEY", seatKey);
                command.Parameters.AddWithValue("@TABLE_KEY", tableKey);
                command.Parameters.AddWithValue("@SEATNO", 1);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetorCreateSeatForTable " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand GetTableKeyByTableNumber(FbConnection connection, FbTransaction transaction, int tableNumber)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    SELECT TABLE_KEY FROM TABLES
                                        WHERE TABLE_NUMBER = @TABLE_NUMBER
                                    ";



                command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTableKeyByTableName " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand InsertRecordToOrders(FbConnection connection, FbTransaction transaction, OrderAttributes orderDbItem)
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
                                        PATRON_COUNT,
                                        ONLINE_CHIT_NO, 
                                        ONLINE_CHIT_TYPE, 
                                        ONLINE_ORDER_ID,
                                        IS_DOCKET_PRINTED
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
                                        @PATRON_COUNT,
                                        @ONLINE_CHIT_NO, 
                                        @ONLINE_CHIT_TYPE, 
                                        @ONLINE_ORDER_ID,
                                        @IS_DOCKET_PRINTED)";

                command.Parameters.AddWithValue("@ORDER_KEY", orderDbItem.OrderId);
                command.Parameters.AddWithValue("@TAB_KEY", orderDbItem.TabKey);
                command.Parameters.AddWithValue("@ITEM_NAME", orderDbItem.Name);
                command.Parameters.AddWithValue("@ITEM_ID", orderDbItem.ItemSizeId);
                command.Parameters.AddWithValue("@SIZE_NAME", orderDbItem.SizeName);
                command.Parameters.AddWithValue("@NOTE", "");
                command.Parameters.AddWithValue("@PARTY_NAME", "");
                command.Parameters.AddWithValue("@TABLE_NUMBER", orderDbItem.ContainerType == Loyaltymate.Enum.OrderContainerType.Table ? orderDbItem.ContainerNumber : 0);
                command.Parameters.AddWithValue("@TABLE_NAME", orderDbItem.ContainerType == Loyaltymate.Enum.OrderContainerType.Table ?
                    QueryUtilities.GetSubstring("Table #" + orderDbItem.ContainerNumber, 1, 25) : QueryUtilities.GetSubstring("Table #" + orderDbItem.ContainerName, 1, 25));
                command.Parameters.AddWithValue("@SEATNO", orderDbItem.ContainerType == Loyaltymate.Enum.OrderContainerType.Table ? 1 : 0);
                command.Parameters.AddWithValue("@PRICE", orderDbItem.Price);
                command.Parameters.AddWithValue("@PRINTED", 'F');
                command.Parameters.AddWithValue("@TRANSNO", orderDbItem.TramsNo);
                command.Parameters.AddWithValue("@ORDER_TYPE", orderDbItem.OrderType);
                command.Parameters.AddWithValue("@TERMINAL_NAME", orderDbItem.TerminalName);
                command.Parameters.AddWithValue("@MENU_NAME", orderDbItem.MenuName);
                command.Parameters.AddWithValue("@TAB_NAME", orderDbItem.ContainerType == Loyaltymate.Enum.OrderContainerType.Table ? "Guest 1" : QueryUtilities.GetSubstring(orderDbItem.ContainerName, 1, 32));
                command.Parameters.AddWithValue("@COURSE_NAME", orderDbItem.CourseName);
                command.Parameters.AddWithValue("@HAPPYHOUR", 'F');
                command.Parameters.AddWithValue("@ORDER_LOCATION", orderDbItem.Location);
                command.Parameters.AddWithValue("@TAB_TYPE", orderDbItem.ContainerType == Loyaltymate.Enum.OrderContainerType.Table ? 3 : 0);
                command.Parameters.AddWithValue("@TIME_STAMP", orderDbItem.TransactionDate);
                command.Parameters.AddWithValue("@COST", orderDbItem.Cost);
                command.Parameters.AddWithValue("@LOYALTY_KEY", orderDbItem.MembershipProfileId); //to test loyalty key
                command.Parameters.AddWithValue("@MASTER_CONTAINER", orderDbItem.SizeName); //todo 
                command.Parameters.AddWithValue("@SETMENU_MASK", orderDbItem.SetMenuMask);
                command.Parameters.AddWithValue("@SETMENU_GROUP", 0); //todo in future
                command.Parameters.AddWithValue("@ITEM_CATEGORY", QueryUtilities.GetSubstring(orderDbItem.ItemCategory, 1, 40));
                command.Parameters.AddWithValue("@SECURITY_REF", orderDbItem.SecurityRef);
                command.Parameters.AddWithValue("@TIME_KEY", orderDbItem.TimeKey);
                command.Parameters.AddWithValue("@GST_PERCENT", orderDbItem.GSTPercent);
                command.Parameters.AddWithValue("@COST_GST_PERCENT", orderDbItem.CostGSTPercent);
                command.Parameters.AddWithValue("@QTY", orderDbItem.Quantity);
                command.Parameters.AddWithValue("@DISCOUNT", 0);
                command.Parameters.AddWithValue("@DISCOUNT_REASON", "");
                command.Parameters.AddWithValue("@REDEEMED", 0.0f);
                command.Parameters.AddWithValue("@ITEM_KITCHEN_NAME", orderDbItem.Name);
                command.Parameters.AddWithValue("@SIZE_KITCHEN_NAME", orderDbItem.SizeName);
                command.Parameters.AddWithValue("@COURSE_KITCHEN_NAME", orderDbItem.CourseKitchenName);
                command.Parameters.AddWithValue("@POINTS_PERCENT", orderDbItem.PointsPercent);
                command.Parameters.AddWithValue("@CATEGORY_KEY", orderDbItem.CategoryKey);
                command.Parameters.AddWithValue("@THIRDPARTYCODES_KEY", orderDbItem.ThirdPartyCodeKey);
                command.Parameters.AddWithValue("@MEMBER_FREE_SALE_COUNT", 0);
                command.Parameters.AddWithValue("@MEMBER_FREE_SALE_DISCOUNT", 0);
                command.Parameters.AddWithValue("@LOCATION_FREE_SALE_COUNT", 0);
                command.Parameters.AddWithValue("@LOCATION_FREE_SALE_DISCOUNT", 0);
                command.Parameters.AddWithValue("@IS_FREE", 'F');
                command.Parameters.AddWithValue("@SERVINGCOURSES_KEY", orderDbItem.SetvingCourseKey);
                command.Parameters.AddWithValue("@PRICE_LEVEL0", orderDbItem.Price);
                command.Parameters.AddWithValue("@PRICE_LEVEL1", orderDbItem.Price);
                command.Parameters.AddWithValue("@ITEM_TYPE", orderDbItem.OrderType);
                command.Parameters.AddWithValue("@MENU_ITEM_KEY", orderDbItem.MenuItemKey); // ItemOrderedFrom->ItemKey
                command.Parameters.AddWithValue("@PLU", orderDbItem.PLU);
                command.Parameters.AddWithValue("@ORDER_TYPE_MESSAGE", "");
                command.Parameters.AddWithValue("@CONTACTS_KEY", orderDbItem.MembershipProfileId);
                command.Parameters.AddWithValue("@ACTIVECHITNUMBER_KEY", DBNull.Value);
                command.Parameters.AddWithValue("@PATRON_COUNT", orderDbItem.PatronCount);
                command.Parameters.AddWithValue("@ONLINE_CHIT_NO", 0);
                command.Parameters.AddWithValue("@ONLINE_CHIT_TYPE", orderDbItem.TransactionType);
                command.Parameters.AddWithValue("@ONLINE_ORDER_ID", orderDbItem.OrderGuid);
                command.Parameters.AddWithValue("@IS_DOCKET_PRINTED", "F");

                if (orderDbItem.SideOrderKey > 0)
                    command.Parameters.AddWithValue("@SIDE_ORDER_KEY", orderDbItem.SideOrderKey); //todo
                else
                    command.Parameters.AddWithValue("@SIDE_ORDER_KEY", DBNull.Value);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertRecordToOrders " + e.Message, e);
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 195, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand CreateTab(FbConnection connection, FbTransaction transaction, int tabKey, string tabName, string id_number)
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
                command.Parameters.AddWithValue("@TAB_TYPE", 0);
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

        public FbCommand CreateTable(FbConnection connection, FbTransaction transaction, int tableKey, int tableNumber)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................

            try
            {
                command.CommandText = @"
                                    INSERT INTO TABLES (
                                            TABLE_KEY,
                                            TABLE_NUMBER,
                                            TABLE_NAME,
                                            PARTY_NAME,
                                            CIRCLE,
                                            TEMPORARY) 
                                    VALUES (
                                            @TABLE_KEY,
                                            @TABLE_NUMBER,
                                            @TABLE_NAME,
                                            @PARTY_NAME,
                                            @CIRCLE,
                                            @TEMPORARY)
                                    ";

                command.Parameters.AddWithValue("@TABLE_KEY", tableKey);
                command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
                command.Parameters.AddWithValue("@TABLE_NAME", "Table# " + tableNumber);
                command.Parameters.AddWithValue("@PARTY_NAME", 0);
                command.Parameters.AddWithValue("@CIRCLE", 'F');
                command.Parameters.AddWithValue("@TEMPORARY", 'F');
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateTable " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 182, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand InsertBreakDownCategoryToDB(FbConnection connection, FbTransaction transaction, long orderKey, int categoryKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

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
                command.Parameters.AddWithValue("@CATEGORY_KEY", categoryKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
            }

            return command;
        }

        public FbCommand GetItemSizeInfo(FbConnection connection, FbTransaction transaction, long itemSizeId)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.ITEMSIZE_KEY, A.PLU, A.ITEM_KEY MENU_ITEM_KEY, a.POINTS_PERCENT,
                                            A.SETMENU_MASK, A.THIRDPARTYCODES_KEY,A.COST, CATEGORY_KEY, 
                                            a.COST_GST_PERCENT, a.DEFAULT_PATRON_COUNT, a.GST_PERCENT  
                                        FROM ITEMSIZE a  
                                        WHERE A.ITEMSIZE_IDENTIFIER = @ITEMSIZE_IDENTIFIER ";

                command.Parameters.AddWithValue("@ITEMSIZE_IDENTIFIER", itemSizeId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
            }

            return command;
        }

        public FbCommand LoadBreakDownCategoriesCmd(FbConnection connection, FbTransaction transaction, int itemSizeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            try
            {
                result.CommandText = @"
                                    SELECT
                                        ItemSizeCategory.ItemSize_Key, ArcCategories.Category_Key, ArcCategories.Category
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
            return result;
        }

        public FbCommand GetItemSizeTaxProfileKey(FbConnection connection, FbTransaction transaction, int itemSizeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            try
            {
                result.CommandText = @"
                                        SELECT a.PROFILE_KEY
                                        FROM TAXPROFILES_ITEMSIZE a
                                        WHERE a.ITEMSIZE_KEY = @ITEMSIZE_KEY
                                        GROUP BY 1
                                    ";

                result.Parameters.AddWithValue("@ITEMSIZE_KEY", itemSizeKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetItemSizeTaxProfileKey " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 175, short.MaxValue);
            }
            return result;
        }

//        public FbCommand InsertIntoSecurity(FbConnection connection, FbTransaction transaction, OrderAttributes orderDbItem, long securityKey)
//        {
//            FbCommand command = new FbCommand(@"", connection, transaction);

//            try
//            {
//                command.CommandText = @"
//                                   INSERT INTO SECURITY (SECURITY_KEY, SECURITY_REF, SECURITY_EVENT, FROM_VAL, TO_VAL, NOTE, TERMINAL_NAME, USER_KEY, TIME_STAMP)
//                                     VALUES (@SECURITY_KEY, @SECURITY_REF, @SECURITY_EVENT, @FROM_VAL, @TO_VAL, @NOTE, @TERMINAL_NAME, @USER_KEY, @TIME_STAMP)";

//                command.Parameters.AddWithValue("@SECURITY_KEY", securityKey);
//                command.Parameters.AddWithValue("@SECURITY_REF", orderDbItem.SecurityRef);
//                command.Parameters.AddWithValue("@SECURITY_EVENT", "Online ordered by");
//                command.Parameters.AddWithValue("@FROM_VAL", orderDbItem.ContainerName);
//                command.Parameters.AddWithValue("@TO_VAL", orderDbItem.ContainerName);
//                command.Parameters.AddWithValue("@NOTE", "");
//                command.Parameters.AddWithValue("@TERMINAL_NAME", orderDbItem.Location);
//                command.Parameters.AddWithValue("@USER_KEY", orderDbItem.MembershipProfileId);
//                command.Parameters.AddWithValue("@TIME_STAMP", orderDbItem.TransactionDate);
//            }
//            catch (Exception e)
//            {
//                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
//                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
//            }

//            return command;
//        }

        public FbCommand GetItemInfo(FbConnection connection, FbTransaction transaction, long itemId)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.ITEM_IDENTIFIER, a.ITEM_CATEGORY, a.ITEM_KITCHEN_NAME, a.ITEM_NAME, 
                                                c.COURSE_NAME,c.COURSE_KITCHEN_NAME, m.MENU_NAME, c.SERVINGCOURSES_KEY 
                                        FROM ITEM a
                                        INNER JOIN COURSE c ON a.COURSE_KEY = c.COURSE_KEY INNER JOIN MENU m ON c.MENU_KEY = M.MENU_KEY 
                                        WHERE a.ITEM_IDENTIFIER = @ITEM_IDENTIFIER  ";

                command.Parameters.AddWithValue("@ITEM_IDENTIFIER", itemId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
            }

            return command;
        }

        public FbCommand OpenSaleStartTimeCmd(FbConnection connection, FbTransaction transaction, int currentTimeKey)
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
        public FbCommand CloseSaleStartTimeCmd(FbConnection connection, FbTransaction transaction, int timeKey)
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

        public FbCommand InsertTaxProfilesOrders(FbConnection connection, FbTransaction transaction, long tpis_Key, long profileKey, long orderKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"
                                   INSERT INTO TAXPROFILES_ORDERS (TPO_KEY, PROFILE_KEY, ORDER_KEY)
                                                     VALUES (@TPO_KEY, @PROFILE_KEY, @ORDER_KEY)";

                command.Parameters.AddWithValue("@TPO_KEY", tpis_Key);
                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
                command.Parameters.AddWithValue("@ORDER_KEY", orderKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertTaxProfilesOrders " + e.Message, e);
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
            }

            return command;
        }

        #endregion
    }

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
}
