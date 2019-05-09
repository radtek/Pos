using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FirebirdSql.Data.FirebirdClient;
using OnlineOrdering.Enum;

namespace MenumateServices.DTO.MenumateOnlineOrdering.DBOrders
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
                                        TAB 
                                    WHERE 
                                        TAB_NAME = @TAB_NAME AND TAB_TYPE = @TAB_TYPE
                                    ";

                command.Parameters.AddWithValue("@TAB_NAME", tabName);
                command.Parameters.AddWithValue("@TAB_TYPE", 0);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTabKeyByWebOrderKeyCmd " + e.Message, e);
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand CheckTableAlreadyOccupied(FbConnection connection, FbTransaction transaction, string memberEmail, int tableNumber)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            try
            {
                command.CommandText = @"
                                    SELECT A.ORDER_KEY 
                                    FROM ORDERS a
                                    WHERE a.TABLE_NUMBER = @TABLE_NUMBER AND a.EMAIL <> @EMAIL ";

                command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
                command.Parameters.AddWithValue("@EMAIL", memberEmail);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckTableAlreadyOccupied table number " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand TableWithSameMemberAlreadyExist(FbConnection connection, FbTransaction transaction, string memberEmail, int tableNumber)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            try
            {
                command.CommandText = @"
                                    SELECT A.ORDER_KEY 
                                    FROM ORDERS a
                                    WHERE a.TABLE_NUMBER = @TABLE_NUMBER AND a.EMAIL = @EMAIL ";

                command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
                command.Parameters.AddWithValue("@EMAIL", memberEmail);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableWithSameMemberExist   " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand TableWithSameMemberAlreadyExist(FbConnection connection, FbTransaction transaction, string memberEmail, string tableName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            try
            {
                command.CommandText = @"
                                    SELECT A.ORDER_KEY 
                                    FROM ORDERS a
                                    WHERE a.TABLE_NAME = @TABLE_NAME AND a.EMAIL = @EMAIL ";

                command.Parameters.AddWithValue("@TABLE_NAME", tableName);
                command.Parameters.AddWithValue("@EMAIL", memberEmail);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableWithSameMemberExist   " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand CheckTableAlreadyOccupied(FbConnection connection, FbTransaction transaction, string memberEmail, string tableName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            try
            {
                command.CommandText = @"
                                    SELECT A.ORDER_KEY 
                                    FROM ORDERS a
                                    WHERE a.TABLE_NAME = @TABLE_NAME AND a.EMAIL <> @EMAIL ";

                command.Parameters.AddWithValue("@TABLE_NAME", tableName);
                command.Parameters.AddWithValue("@EMAIL", memberEmail);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckTableAlreadyOccupied table name " + e.Message, e);
                throw;
            }

            return command;
        }

        //returning query because command.executrnonquery() > 0 creating problem at many places so iterating val
        public FbCommand ReadFloorPlanProperty(FbConnection connection, FbTransaction transaction, int variableKey, int intVal)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            try
            {
                command.CommandText = @"
                                    SELECT a.VARIABLES_KEY,  a.INTEGER_VAL
                                    FROM VARSPROFILE a
                                    WHERE a.VARIABLES_KEY = @VARIABLES_KEY and A.INTEGER_VAL = @INTEGER_VAL ";

                command.Parameters.AddWithValue("@VARIABLES_KEY", variableKey);
                command.Parameters.AddWithValue("@INTEGER_VAL", intVal);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsFloorPlanEnabled " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand GetTabExists(FbConnection connection, FbTransaction transaction, int tabKey)
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
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
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
                throw;
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
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand SetSeatTabKey(FbConnection connection, FbTransaction transaction, int seatKey, int tabKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

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
                throw;
            }

            return command;
        }

        public FbCommand UpdateTableName(FbConnection connection, FbTransaction transaction, int tableKey, string tableName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"
                                         UPDATE TABLES a SET a.TABLE_NAME = @TABLE_NAME  
                                         WHERE a.TABLE_KEY = @TABLE_KEY ";

                command.Parameters.AddWithValue("@TABLE_KEY", tableKey);
                command.Parameters.AddWithValue("@TABLE_NAME", tableName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetTableName " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand UpdateTabName(FbConnection connection, FbTransaction transaction, int tabKey, string tabName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"
                                         UPDATE TAB SET TAB_NAME = @TAB_NAME WHERE TAB_KEY = @TAB_KEY ";

                command.Parameters.AddWithValue("@TAB_KEY", tabKey);
                command.Parameters.AddWithValue("@TAB_NAME", tabName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in UpdateTabName " + e.Message, e);
                throw;
            }

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
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 181, short.MaxValue);
            }

            //............................................

            return command;
        }

        public FbCommand GetTableKeyByTableNumber(FbConnection connection, FbTransaction transaction, int tableNumber)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"
                                    SELECT TABLE_KEY FROM TABLES
                                        WHERE TABLE_NUMBER = @TABLE_NUMBER ";

                command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTableKeyByTableName " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand InsertRecordToOrders(FbConnection connection, FbTransaction transaction, OrderAttributes orderDbItem)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

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
                                        BASE_PRICE,                                        
                                        PRICE_INCL,
                                        ONLINE_CHIT_TYPE,
                                        IS_DOCKET_PRINTED,
                                        SITE_ID,
                                        ORDER_ITEM_ID,
                                        ORDER_ITEM_SIZE_ID,
                                        REFERENCE_ORDER_ITEM_SIZE_ID,
                                        EMAIL,                                         
                                        ONLINE_ORDER_ID,
                                        ORDER_GUID,
                                        ROOM_NO, 
                                        FIRST_NAME, 
                                        LAST_NAME
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
                                        @BASE_PRICE,                                        
                                        @PRICE_INCL,
                                        @ONLINE_CHIT_TYPE,
                                        @IS_DOCKET_PRINTED,
                                        @SITE_ID,
                                        @ORDER_ITEM_ID,
                                        @ORDER_ITEM_SIZE_ID,
                                        @REFERENCE_ORDER_ITEM_SIZE_ID,
                                        @EMAIL,                                         
                                        @ONLINE_ORDER_ID,
                                        @ORDER_GUID,
                                        @ROOM_NO, 
                                        @FIRST_NAME, 
                                        @LAST_NAME)";

                command.Parameters.AddWithValue("@ORDER_KEY", orderDbItem.OrderId);
                command.Parameters.AddWithValue("@TAB_KEY", orderDbItem.TabKey);
                command.Parameters.AddWithValue("@ITEM_NAME", orderDbItem.Name);
                command.Parameters.AddWithValue("@ITEM_ID", orderDbItem.ItemId);
                command.Parameters.AddWithValue("@SIZE_NAME", orderDbItem.SizeName);
                command.Parameters.AddWithValue("@NOTE", "");
                command.Parameters.AddWithValue("@PARTY_NAME", "");
                command.Parameters.AddWithValue("@TABLE_NUMBER", orderDbItem.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ? orderDbItem.ContainerNumber : 0);
                command.Parameters.AddWithValue("@TABLE_NAME", orderDbItem.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ?
                    QueryUtilities.GetSubstring(orderDbItem.TableName, 1, 25) : QueryUtilities.GetSubstring(orderDbItem.ContainerName, 1, 25));
                command.Parameters.AddWithValue("@SEATNO", orderDbItem.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ? 1 : 0);
                command.Parameters.AddWithValue("@PRICE", orderDbItem.Price);
                command.Parameters.AddWithValue("@PRINTED", 'F');
                command.Parameters.AddWithValue("@TRANSNO", orderDbItem.TramsNo);
                command.Parameters.AddWithValue("@ORDER_TYPE", orderDbItem.OrderType);
                command.Parameters.AddWithValue("@TERMINAL_NAME", orderDbItem.TerminalName);
                command.Parameters.AddWithValue("@MENU_NAME", orderDbItem.MenuName);
                command.Parameters.AddWithValue("@TAB_NAME", orderDbItem.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ? "Guest 1" : QueryUtilities.GetSubstring(orderDbItem.ContainerName, 1, 32));
                command.Parameters.AddWithValue("@COURSE_NAME", orderDbItem.CourseName);
                command.Parameters.AddWithValue("@HAPPYHOUR", 'F');
                command.Parameters.AddWithValue("@ORDER_LOCATION", orderDbItem.Location);
                command.Parameters.AddWithValue("@TAB_TYPE", orderDbItem.ContainerType == OnlineOrdering.Enum.OrderContainerType.Table ? 3 : 0);
                command.Parameters.AddWithValue("@TIME_STAMP", DateTime.Now);
                command.Parameters.AddWithValue("@COST", orderDbItem.Cost);
                command.Parameters.AddWithValue("@LOYALTY_KEY", orderDbItem.MembershipProfileId); //to test loyalty key
                command.Parameters.AddWithValue("@MASTER_CONTAINER", orderDbItem.MasterContainer);
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
                command.Parameters.AddWithValue("@ITEM_KITCHEN_NAME", orderDbItem.ItemKitchenName);
                command.Parameters.AddWithValue("@SIZE_KITCHEN_NAME", orderDbItem.SizeKitchenName);
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
                command.Parameters.AddWithValue("@BASE_PRICE", orderDbItem.BasePrice);
                command.Parameters.AddWithValue("@PRICE_INCL", orderDbItem.PriceInclusive);
                command.Parameters.AddWithValue("@ONLINE_CHIT_TYPE", orderDbItem.TransactionType);
                command.Parameters.AddWithValue("@IS_DOCKET_PRINTED", "F");
                command.Parameters.AddWithValue("@SITE_ID", orderDbItem.SiteId);
                command.Parameters.AddWithValue("@ORDER_ITEM_ID", orderDbItem.OrderItemId);
                command.Parameters.AddWithValue("@ORDER_ITEM_SIZE_ID", orderDbItem.OrderItemSizeId);
                command.Parameters.AddWithValue("@REFERENCE_ORDER_ITEM_SIZE_ID", orderDbItem.ReferenceOrderItemSizeId);
                command.Parameters.AddWithValue("@EMAIL", orderDbItem.Email);
                command.Parameters.AddWithValue("@ONLINE_ORDER_ID", orderDbItem.OnlinerderId);
                command.Parameters.AddWithValue("@ORDER_GUID", orderDbItem.OrderGuid);
                command.Parameters.AddWithValue("@ROOM_NO", "");
                command.Parameters.AddWithValue("@FIRST_NAME", "");
                command.Parameters.AddWithValue("@LAST_NAME", "");



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
            return command;
        }

        public FbCommand CreateTab(FbConnection connection, FbTransaction transaction, int tabKey, string tabName, int tabType)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

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
                command.Parameters.AddWithValue("@ID_NUMBER", "");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateTabCmd " + e.Message, e);
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 182, short.MaxValue);
            }

            return command;
        }

        public FbCommand CreateTable(FbConnection connection, FbTransaction transaction, int tableKey, int tableNumber, string tableName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            if (tableNumber == 0)
                throw new Exception("Table number can not be zero.");
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
                command.Parameters.AddWithValue("@TABLE_NAME", ""); //tableName todo later..
                command.Parameters.AddWithValue("@PARTY_NAME", "");
                command.Parameters.AddWithValue("@CIRCLE", 'F');
                command.Parameters.AddWithValue("@TEMPORARY", 'F');
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateTable " + e.Message, e);
                throw;
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
                throw;
                //EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 196, short.MaxValue);
            }

            return command;
        }

        public FbCommand GetItemSizeInfo(FbConnection connection, FbTransaction transaction, string itemSizeId)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.ITEMSIZE_KEY, A.PLU, A.ITEM_KEY MENU_ITEM_KEY, a.POINTS_PERCENT,
                                            A.SETMENU_MASK, A.THIRDPARTYCODES_KEY,A.COST, CATEGORY_KEY, 
                                            a.COST_GST_PERCENT, a.DEFAULT_PATRON_COUNT, a.GST_PERCENT,a.SIZE_NAME,a.SIZE_KITCHEN_NAME   
                                        FROM ITEMSIZE a  
                                        WHERE A.ITEMSIZE_IDENTIFIER = @ITEMSIZE_IDENTIFIER ";

                command.Parameters.AddWithValue("@ITEMSIZE_IDENTIFIER", itemSizeId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand GetItemUniqueIdByItemSIzeUniqueID(FbConnection connection, FbTransaction transaction, string itemSizeId)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT ITEM.ITEM_IDENTIFIER 
                                        FROM ITEM 
                                        INNER JOIN ITEMSIZE ON ITEM.ITEM_KEY = ITEMSIZE.ITEM_KEY 
                                        WHERE ITEMSIZE.ITEMSIZE_IDENTIFIER = @ITEMSIZE_IDENTIFIER ";

                command.Parameters.AddWithValue("@ITEMSIZE_IDENTIFIER", itemSizeId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
                throw;
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
                throw;
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
                throw;
            }
            return result;
        }

        public FbCommand GetSideParentOrderKey(FbConnection connection, FbTransaction transaction, long orderItemSizeId)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            try
            {
                result.CommandText = @"
                                        SELECT a.ORDER_KEY
                                        FROM ORDERS a 
                                        WHERE a.ORDER_ITEM_SIZE_ID = @ORDER_ITEM_SIZE_ID
                                        GROUP BY 1
                                    ";

                result.Parameters.AddWithValue("@ORDER_ITEM_SIZE_ID", orderItemSizeId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSideParentOrderKey " + e.Message, e);
                throw;
            }
            return result;
        }

        public FbCommand GetItemInfo(FbConnection connection, FbTransaction transaction, string itemId)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.ITEM_IDENTIFIER, a.ITEM_ID, a.ITEM_CATEGORY, a.ITEM_KITCHEN_NAME, a.ITEM_NAME, 
                                                c.COURSE_NAME,c.COURSE_KITCHEN_NAME, m.MENU_NAME, c.SERVINGCOURSES_KEY 
                                        FROM ITEM a
                                        INNER JOIN COURSE c ON a.COURSE_KEY = c.COURSE_KEY INNER JOIN MENU m ON c.MENU_KEY = M.MENU_KEY 
                                        WHERE a.ITEM_IDENTIFIER = @ITEM_IDENTIFIER  ";

                command.Parameters.AddWithValue("@ITEM_IDENTIFIER", itemId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetOrderBreakdownCategoryCmd " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand OpenSaleStartTimeCmd(FbConnection connection, FbTransaction transaction, int currentTimeKey)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

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
                throw;
            }

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
                throw;
            }

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
                throw;
            }

            return command;
        }

        public FbCommand IsOnlineOrderingEnabled(FbConnection connection, FbTransaction transaction)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT  VARSPROFILE.VARSPROFILE_KEY, VARSPROFILE.PROFILE_KEY FROM VARSPROFILE 
                                        WHERE VARIABLES_KEY = @VARIABLES_KEY AND INTEGER_VAL = @INTEGER_VAL ";

                command.Parameters.AddWithValue("@VARIABLES_KEY", 9637);
                command.Parameters.AddWithValue("@INTEGER_VAL", 1);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsOnlineOrderingEnabled " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand IsVariableKeyExist(FbConnection connection, FbTransaction transaction, int variableKey, int profileKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT  VARSPROFILE.VARSPROFILE_KEY, VARSPROFILE.PROFILE_KEY FROM VARSPROFILE 
                                        WHERE VARIABLES_KEY = @VARIABLES_KEY AND INTEGER_VAL = @INTEGER_VAL AND PROFILE_KEY = @PROFILE_KEY ";

                command.Parameters.AddWithValue("@VARIABLES_KEY", variableKey);
                command.Parameters.AddWithValue("@INTEGER_VAL", 1);
                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTerminalExemptFromHappyHour " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand GetDeviceKey(FbConnection connection, FbTransaction transaction, int profileKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.DEVICE_KEY FROM DEVICES a WHERE a.PROFILE_KEY = @PROFILE_KEY ";

                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetDeviceKey " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand GetTerminalHHProfiles(FbConnection connection, FbTransaction transaction, int profileKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT  HAPPYHOURPROFILES_KEY  FROM TERMINALHAPPYHOURPROFILES THH
                                        INNER JOIN DEVICES ON THH.TERMINALS_KEY = DEVICES.DEVICE_KEY
                                        WHERE DEVICES.PROFILE_KEY = @PROFILE_KEY 
                                        ORDER BY HAPPYHOURPROFILES_KEY ASC ";

                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetTerminalHHProfiles " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand LoadHHProfileInfo(FbConnection connection, FbTransaction transaction, int hhProfileKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @" SELECT a.HAPPYHOURPROFILES_KEY, a.HAPPYHOURPROFILE_NAME, a.HAPPYHOURDAY_STARTTIME, a.HAPPYHOURDAY_ENDTIME, 
                                            a.HAPPYHOURDAY_PROFILEDATE,a.PRICELEVEL_KEY,  coalesce(hprior.HAPPYHOUR_PRIORITY_VAL,0) HAPPYHOUR_PRIORITY_VAL  
                                        FROM HAPPYHOURPROFILES a 
                                        LEFT JOIN HAPPYHOUR_PRIORITY hprior on hprior.HAPPYHOURPROFILES_KEY =a.HAPPYHOURPROFILES_KEY  
                                        WHERE a.HAPPYHOURPROFILES_KEY = @HAPPYHOURPROFILES_KEY and a.ISACTIVE= @ISACTIVE ";

                command.Parameters.AddWithValue("@HAPPYHOURPROFILES_KEY", hhProfileKey);
                command.Parameters.AddWithValue("@ISACTIVE", "T");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadHHProfileInfo " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand LoadDaysInfoForSelectedProfile(FbConnection connection, FbTransaction transaction, int hhProfileKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @" SELECT  HAPPYHOURDAYS_KEY  FROM HAPPYHOURDAYS 
                                        WHERE HAPPYHOURPROFILES_KEY = @HAPPYHOURPROFILES_KEY 
                                        ORDER BY HAPPYHOURDAYS_KEY ASC ";

                command.Parameters.AddWithValue("@HAPPYHOURPROFILES_KEY", hhProfileKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in LoadDaysInfoForSelectedProfile " + e.Message, e);
                throw;
            }

            return command;
        }

        public FbCommand IsTableMarkedForOnlineordering(FbConnection connection, FbTransaction transaction, int tableNumber, string tableName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.TABLE_NUMBER, A.TABLE_NAME 
                                        FROM TABLES a
                                        WHERE A.ACCEPT_OO = @ACCEPT_OO ";
                if (tableNumber > 0)
                {
                    command.CommandText += " AND a.TABLE_NUMBER = @TABLE_NUMBER ";
                    command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
                }
                else
                {
                    command.CommandText += " AND a.TABLE_NAME = @TABLE_NAME ";
                    command.Parameters.AddWithValue("@TABLE_NAME", tableName);
                }

                command.Parameters.AddWithValue("@ACCEPT_OO", "T");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableMarkedForOnlineordering " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand InsertProfileForWaiterApp(FbConnection connection, FbTransaction transaction, long profileKey, string terminalName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {
                command.CommandText =
                                    @"
                                     INSERT INTO PROFILE (
		                                PROFILE_KEY,
		                                PROFILE_TYPE,
		                                NAME) 
		                                VALUES (
		                                @PROFILE_KEY,
		                                @PROFILE_TYPE,
		                                @NAME);
                                    ";

                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
                command.Parameters.AddWithValue("@PROFILE_TYPE", 1);
                command.Parameters.AddWithValue("@NAME", QueryUtilities.GetFromSubstring(terminalName, 0, 255));

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CreateTerminalForWaiterApp " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand InsertTerminalForWaiterApp(FbConnection connection, FbTransaction transaction, long deviceKey, long profileKey, string terminalName, string deviceId)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {
                command.CommandText =
                                    @"
                                    INSERT INTO DEVICES (
                                        DEVICE_KEY,
                                        DEVICE_NAME,
                                        PRODUCT,
                                        DEVICE_ID,
                                        DEVICE_TYPE,
                                        LOCATION_KEY,
                                        PROFILE_KEY,
                                        UNIQUE_DEVICE_ID,
                                        IP) 
                                    VALUES (
                                       @DEVICE_KEY,
                                       @DEVICE_NAME,
		                               @PRODUCT,
                                       @DEVICE_ID,
		                               @DEVICE_TYPE,
                                       @LOCATION_KEY,
                                       @PROFILE_KEY,
                                       @UNIQUE_DEVICE_ID,
                                       @IP);
                                    ";

                command.Parameters.AddWithValue("@DEVICE_KEY", deviceKey);
                command.Parameters.AddWithValue("@DEVICE_NAME", terminalName);
                command.Parameters.AddWithValue("@PRODUCT", terminalName);
                command.Parameters.AddWithValue("@DEVICE_ID", 0);
                command.Parameters.AddWithValue("@DEVICE_TYPE", DTO.Enum.AppType.devWaiter);
                command.Parameters.AddWithValue("@LOCATION_KEY", 1);
                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
                command.Parameters.AddWithValue("@UNIQUE_DEVICE_ID", QueryUtilities.GetFromSubstring(deviceId, 0, 250));
                command.Parameters.AddWithValue("@IP", "");

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertTerminalForWaiterApp " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand InsertStaffForWaiterApp(FbConnection connection, FbTransaction transaction, long contactKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {
                command.CommandText =
                                    @"
			                        INSERT INTO CONTACTS ( 
                                        CONTACTS_KEY, 
                                        CONTACT_TYPE, 
                                        NAME, 
                                        PIN, 
                                        SITE_ID,
                                        MEMBER_NUMBER)
                                    VALUES ( 
                                        @CONTACTS_KEY, 
                                        @CONTACT_TYPE, 
                                        @NAME, 
                                        @PIN, 
                                        @SITE_ID, 
                                        @MEMBER_NUMBER);
                                    ";

                command.Parameters.AddWithValue("@CONTACTS_KEY", contactKey);
                command.Parameters.AddWithValue("@CONTACT_TYPE", 0);
                command.Parameters.AddWithValue("@NAME", "WAITER");
                command.Parameters.AddWithValue("@PIN", "11");
                command.Parameters.AddWithValue("@SITE_ID", 0);
                command.Parameters.AddWithValue("@MEMBER_NUMBER", "");


            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertStaffForWaiterApp " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand IsItemAvailable(FbConnection connection, FbTransaction transaction, long itemSizeUniqueKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                // Also check if actually the item can be used, because it may happen that item is there in table 
                // but not usable
                command.CommandText = @"SELECT a.ITEMSIZE_KEY 
                                        FROM ITEMSIZE a
                                        WHERE A.ITEMSIZE_IDENTIFIER = @ITEMSIZE_IDENTIFIER ";

                command.Parameters.AddWithValue("@ITEMSIZE_IDENTIFIER", itemSizeUniqueKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsItemAvailable " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand IsTableAvailable(FbConnection connection, FbTransaction transaction, int tableNumber, string tableName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"SELECT a.TABLE_NUMBER, A.TABLE_NAME 
                                        FROM TABLES a
                                        WHERE ";
                if (tableNumber > 0)
                {
                    command.CommandText += " a.TABLE_NUMBER = @TABLE_NUMBER ";
                    command.Parameters.AddWithValue("@TABLE_NUMBER", tableNumber);
                }
                else
                {
                    command.CommandText += " a.TABLE_NAME = @TABLE_NAME ";
                    command.Parameters.AddWithValue("@TABLE_NAME", tableName);
                }

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in IsTableMarkedForOnlineordering " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand InsertDataIntoDayArcBill(FbConnection connection, FbTransaction transaction, DayArcBillAttributes dayArcBillRow)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {
                command.CommandText =
                                    @"
                    INSERT INTO DAYARCBILL ( 
                            ARCBILL_KEY,  
                            TERMINAL_NAME,  
                            STAFF_NAME,  
                            TIME_STAMP,  
                            TOTAL,  
                            DISCOUNT, 		
                            PATRON_COUNT,  
                            RECEIPT,  
                            SECURITY_REF, 
                            BILLED_LOCATION,  
                            INVOICE_NUMBER,  
                            SALES_TYPE,  
                            INVOICE_KEY,      
                            ROUNDING_ADJUSTMENT, 
                            ORDER_IDENTIFICATION_NUMBER,  
                            REFUND_REFRECEIPT, 
                            EFTPOS_SERVICE_ID,
                            IS_PRINT_REQUIRED,
                            APP_TYPE,
                            ONLINE_ORDER_ID,
                            ORDER_GUID,
                            SITE_ID) 
                      VALUES (
		                    @ARCBILL_KEY,  
                            @TERMINAL_NAME,  
                            @STAFF_NAME,  
                            @TIME_STAMP,  
                            @TOTAL,  
                            @DISCOUNT,  
                            @PATRON_COUNT,  
                            @RECEIPT, 
		                    @SECURITY_REF,  
                            @BILLED_LOCATION, 
                            @INVOICE_NUMBER,  
                            @SALES_TYPE,  
                            @INVOICE_KEY, 
                            @ROUNDING_ADJUSTMENT, 
                            @ORDER_IDENTIFICATION_NUMBER,  
                            @REFUND_REFRECEIPT, 
                            @EFTPOS_SERVICE_ID,
                            @IS_PRINT_REQUIRED,
                            @APP_TYPE,
                            @ONLINE_ORDER_ID,
                            @ORDER_GUID,
                            @SITE_ID)
                    ;";

                command.Parameters.AddWithValue("@ARCBILL_KEY", dayArcBillRow.ArcBillId);
                command.Parameters.AddWithValue("@TERMINAL_NAME", dayArcBillRow.TerminalName);
                command.Parameters.AddWithValue("@STAFF_NAME", dayArcBillRow.StaffName);
                command.Parameters.AddWithValue("@TIME_STAMP", DateTime.Now);
                command.Parameters.AddWithValue("@TOTAL", dayArcBillRow.Total);
                command.Parameters.AddWithValue("@DISCOUNT", dayArcBillRow.Discount);
                command.Parameters.AddWithValue("@PATRON_COUNT", dayArcBillRow.PatronCount);
                command.Parameters.AddWithValue("@RECEIPT", null);
                command.Parameters.AddWithValue("@SECURITY_REF", dayArcBillRow.SecurityRef);
                command.Parameters.AddWithValue("@BILLED_LOCATION", dayArcBillRow.BilledLocation);
                command.Parameters.AddWithValue("@INVOICE_NUMBER", dayArcBillRow.InvoiceNumber);
                command.Parameters.AddWithValue("@SALES_TYPE", dayArcBillRow.SaleType);
                command.Parameters.AddWithValue("@INVOICE_KEY", 0);
                command.Parameters.AddWithValue("@ROUNDING_ADJUSTMENT", dayArcBillRow.RoundingAdjustment);
                command.Parameters.AddWithValue("@ORDER_IDENTIFICATION_NUMBER", 0);
                command.Parameters.AddWithValue("@REFUND_REFRECEIPT", "");
                command.Parameters.AddWithValue("@EFTPOS_SERVICE_ID", "");
                command.Parameters.AddWithValue("@IS_PRINT_REQUIRED", dayArcBillRow.IsPrintRequired ? 'T' : 'F');
                command.Parameters.AddWithValue("@APP_TYPE", dayArcBillRow.ApplicationType);
                command.Parameters.AddWithValue("@ONLINE_ORDER_ID", dayArcBillRow.OnlinOrderId);
                command.Parameters.AddWithValue("@ORDER_GUID", QueryUtilities.GetFromSubstring(dayArcBillRow.OrderGuid, 0, 50));
                command.Parameters.AddWithValue("@SITE_ID", dayArcBillRow.SiteId);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoDayArcBill " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoDayArcBillPay(FbConnection connection, FbTransaction transaction, DayArcBillPayAttributes dayArcBillPayRow)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {
                command.CommandText =
                @"
		            INSERT INTO DAYARCBILLPAY ( 
                            DAYARCBILLPAY_KEY,  
                            ARCBILL_KEY,  
                            PAY_TYPE,  
                            VOUCHER_NUMBER, 
		                    SUBTOTAL,  
                            ROUNDING,  
                            CASH_OUT,  
                            TAX_FREE,  
                            NOTE, 
                            PAY_TYPE_DETAILS, 
                            PROPERTIES, 
                            GROUP_NUMBER, 
                            PAYMENT_CARD_TYPE, 
                            PAY_GROUP,
                            CHARGED_TO_XERO,
                            TIP_AMOUNT)  
                    VALUES (
		                    @DAYARCBILLPAY_KEY,  
                            @ARCBILL_KEY,  
                            @PAY_TYPE,  
                            @VOUCHER_NUMBER,  
                            @SUBTOTAL,  
                            @ROUNDING,  
                            @CASH_OUT,  
                            @TAX_FREE, 
		                    @NOTE, 
                            @PAY_TYPE_DETAILS, 
                            @PROPERTIES,  
                            @GROUP_NUMBER , 
                            @PAYMENT_CARD_TYPE, 
                            @PAY_GROUP,
                            @CHARGED_TO_XERO,
                            @TIP_AMOUNT)
                            ;";

                command.Parameters.AddWithValue("@DAYARCBILLPAY_KEY", dayArcBillPayRow.DayArcBillPayId);
                command.Parameters.AddWithValue("@ARCBILL_KEY", dayArcBillPayRow.DayArcBillId);
                command.Parameters.AddWithValue("@PAY_TYPE", QueryUtilities.GetFromSubstring(dayArcBillPayRow.PayType, 0, 50));
                command.Parameters.AddWithValue("@VOUCHER_NUMBER", "");
                command.Parameters.AddWithValue("@SUBTOTAL", dayArcBillPayRow.SubTotal);
                command.Parameters.AddWithValue("@ROUNDING", dayArcBillPayRow.Rounding);
                command.Parameters.AddWithValue("@CASH_OUT", 'F');
                command.Parameters.AddWithValue("@TAX_FREE", 'F');
                command.Parameters.AddWithValue("@NOTE", dayArcBillPayRow.Note);
                command.Parameters.AddWithValue("@PAY_TYPE_DETAILS", dayArcBillPayRow.PayTypeDetails);
                command.Parameters.AddWithValue("@PROPERTIES", dayArcBillPayRow.Properties);
                command.Parameters.AddWithValue("@GROUP_NUMBER", 0);
                command.Parameters.AddWithValue("@PAYMENT_CARD_TYPE", dayArcBillPayRow.PaymentCardType);
                command.Parameters.AddWithValue("@PAY_GROUP", dayArcBillPayRow.PayGroup);
                command.Parameters.AddWithValue("@CHARGED_TO_XERO", 'F');
                command.Parameters.AddWithValue("@TIP_AMOUNT", 0);

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoDayArcBillPay " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoDayPatronCount(FbConnection connection, FbTransaction transaction, long patronCountKey, long dayArcBillKey, List<string> stringList)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                stringList.Add("Inserting Data Into DayPatronCount table           " + DateTime.Now.ToString("hh:mm:ss tt"));
                command.CommandText =
                    @"
				    INSERT INTO DAYPATRONCOUNT ( 
                            PATRONCOUNT_KEY,  
                            ARCBILL_KEY,  
                            PATRON_TYPE,  
                            PATRON_COUNT) 
				    VALUES ( 
                            @PATRONCOUNT_KEY,  
                            @ARCBILL_KEY,  
                            @PATRON_TYPE,  
                            @PATRON_COUNT) 
                    ;";
                command.Parameters.AddWithValue("@PATRONCOUNT_KEY", patronCountKey);
                command.Parameters.AddWithValue("@ARCBILL_KEY", dayArcBillKey);
                command.Parameters.AddWithValue("@PATRON_TYPE", ""); // Need to Confirm 
                command.Parameters.AddWithValue("@PATRON_COUNT", 1);
            }
            catch (Exception e)
            {
                stringList.Add("Exception in InsertDataIntoDayPatronCount          " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(@"in InsertDataIntoDayPatronCount " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoDayArcRef(FbConnection connection, FbTransaction transaction, long dayArcRefKey, long dayArcBillKey, List<string> stringList)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                stringList.Add("Inserting Data Into DayArcRef table                " + DateTime.Now.ToString("hh:mm:ss tt"));
                command.CommandText =
                    @"
			        INSERT INTO DAYARCREF ( 
                            DAYARCREF_KEY,  
                            ARCBILL_KEY,  
                            REFERENCE,  
                            REFS_KEY )  
                    VALUES (
			                @DAYARCREF_KEY,  
                            @ARCBILL_KEY,  
                            @REFERENCE,  
                            @REFS_KEY )
                    ;";
                command.Parameters.AddWithValue("@DAYARCREF_KEY", dayArcRefKey);
                command.Parameters.AddWithValue("@ARCBILL_KEY", dayArcBillKey);
                command.Parameters.AddWithValue("@REFERENCE", "");//Need to Confirm
                command.Parameters.AddWithValue("@REFS_KEY", 1);//Need to Confirm
            }
            catch (Exception e)
            {
                stringList.Add("Exception in InsertDataIntoDayArcRef               " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(@"in InsertDataIntoDayArcRef " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoDayArchive(FbConnection connection, FbTransaction transaction, DayArchiveAttributes dayArchiveRow)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    INSERT INTO DAYARCHIVE (
				            ARCHIVE_KEY,
				            ARCBILL_KEY,
				            TERMINAL_NAME,
				            MENU_NAME,
				            COURSE_NAME,
				            ITEM_NAME,
				            ITEM_CATEGORY,
				            ITEM_SHORT_NAME,
				            ITEM_ID,
				            SIZE_NAME,
				            TABLE_NUMBER,
				            TABLE_NAME,
				            SEAT_NUMBER,
				            SERVER_NAME,
				            TAB_NAME,
				            LOYALTY_NAME,
				            ORDER_TYPE,
				            TIME_STAMP,
				            TIME_STAMP_BILLED,
				            ORDER_LOCATION,
				            PRICE,
				            COST,
				            HAPPY_HOUR,
				            NOTE,
				            SECURITY_REF,
				            TIME_KEY,
				            GST_PERCENT,
				            COST_GST_PERCENT,
				            QTY,
				            DISCOUNT,
				            REDEEMED,
				            POINTS_PERCENT,
				            POINTS_EARNED,
				            LOYALTY_KEY,
				            THIRDPARTYCODES_KEY,
				            CATEGORY_KEY,
				            DISCOUNT_REASON,
				            PRICE_LEVEL0,
				            PRICE_LEVEL1,
				            SERVINGCOURSES_KEY,
				            CHIT_NAME,
				            CHIT_OPTION,
				            BASE_PRICE,
				            DISCOUNT_WITHOUT_TAX,
				            TAX_ON_DISCOUNT,
                            PRICE_INCL, 
                            PRICE_ADJUST, 
                            ONLINE_CHIT_TYPE,
                            ORDER_GUID,
                            ITEMSIZE_IDENTIFIER 
				            )
				      VALUES 
				            (
				            @ARCHIVE_KEY,
				            @ARCBILL_KEY,
				            @TERMINAL_NAME,
				            @MENU_NAME,
				            @COURSE_NAME,
				            @ITEM_NAME,
				            @ITEM_CATEGORY,
				            @ITEM_SHORT_NAME,
				            @ITEM_ID,
				            @SIZE_NAME,
				            @TABLE_NUMBER,
				            @TABLE_NAME,
				            @SEAT_NUMBER,
				            @SERVER_NAME,
				            @TAB_NAME,
				            @LOYALTY_NAME,
				            @ORDER_TYPE,
				            @TIME_STAMP,
				            @TIME_STAMP_BILLED,
				            @ORDER_LOCATION,
				            @PRICE,
				            @COST,
				            @HAPPY_HOUR,
				            @NOTE,
				            @SECURITY_REF,
				            @TIME_KEY,
				            @GST_PERCENT,
				            @COST_GST_PERCENT,
				            @QTY,
				            @DISCOUNT,
				            @REDEEMED,
				            @POINTS_PERCENT,
				            @POINTS_EARNED,
				            @LOYALTY_KEY,
				            @THIRDPARTYCODES_KEY,
				            @CATEGORY_KEY,
				            @DISCOUNT_REASON,
				            @PRICE_LEVEL0,
				            @PRICE_LEVEL1,
				            @SERVINGCOURSES_KEY,
				            @CHIT_NAME,
				            @CHIT_OPTION,
				            @BASE_PRICE,
				            @DISCOUNT_WITHOUT_TAX,
				            @TAX_ON_DISCOUNT,
                            @PRICE_INCL, 
                            @PRICE_ADJUST, 
                            @ONLINE_CHIT_TYPE,  
                            @ORDER_GUID,
                            @ITEMSIZE_IDENTIFIER 
				        );
                    ";
                command.Parameters.AddWithValue("@ARCHIVE_KEY", dayArchiveRow.ArchiveId);
                command.Parameters.AddWithValue("@ARCBILL_KEY", dayArchiveRow.ArcBillId);
                command.Parameters.AddWithValue("@TERMINAL_NAME", dayArchiveRow.TerminalName);
                command.Parameters.AddWithValue("@MENU_NAME", dayArchiveRow.MenuName);
                command.Parameters.AddWithValue("@COURSE_NAME", dayArchiveRow.CourseName);
                command.Parameters.AddWithValue("@ITEM_NAME", dayArchiveRow.ItemName);
                command.Parameters.AddWithValue("@ITEM_CATEGORY", dayArchiveRow.ItemCategory);
                command.Parameters.AddWithValue("@ITEM_SHORT_NAME", dayArchiveRow.ItemShortName);
                command.Parameters.AddWithValue("@ITEM_ID", dayArchiveRow.ItemId);
                command.Parameters.AddWithValue("@SIZE_NAME", dayArchiveRow.SizeName);
                command.Parameters.AddWithValue("@TABLE_NUMBER", dayArchiveRow.TableNumber);
                command.Parameters.AddWithValue("@TABLE_NAME", dayArchiveRow.TableNumber);
                command.Parameters.AddWithValue("@SEAT_NUMBER", dayArchiveRow.SeatNumber);
                command.Parameters.AddWithValue("@SERVER_NAME", dayArchiveRow.ServerName);
                command.Parameters.AddWithValue("@TAB_NAME", QueryUtilities.GetFromSubstring(dayArchiveRow.TabName, 0, 80));
                command.Parameters.AddWithValue("@LOYALTY_NAME", dayArchiveRow.LoyaltyName);
                command.Parameters.AddWithValue("@ORDER_TYPE", dayArchiveRow.OrderType);
                command.Parameters.AddWithValue("@TIME_STAMP", DateTime.Now);
                command.Parameters.AddWithValue("@TIME_STAMP_BILLED", DateTime.Now);
                command.Parameters.AddWithValue("@ORDER_LOCATION", dayArchiveRow.LoyaltyName);
                command.Parameters.AddWithValue("@PRICE", dayArchiveRow.Price);
                command.Parameters.AddWithValue("@COST", dayArchiveRow.Cost);
                command.Parameters.AddWithValue("@HAPPY_HOUR", 'F'); //Need to Confirm
                command.Parameters.AddWithValue("@NOTE", QueryUtilities.GetFromSubstring(dayArchiveRow.Note, 0, 80));
                command.Parameters.AddWithValue("@SECURITY_REF", dayArchiveRow.SecurityRef);
                command.Parameters.AddWithValue("@TIME_KEY", dayArchiveRow.TimeKey);
                command.Parameters.AddWithValue("@GST_PERCENT", dayArchiveRow.GstPercent);
                command.Parameters.AddWithValue("@COST_GST_PERCENT", dayArchiveRow.CostGstPercent);
                command.Parameters.AddWithValue("@QTY", dayArchiveRow.Qty);
                command.Parameters.AddWithValue("@DISCOUNT", dayArchiveRow.Discount);
                command.Parameters.AddWithValue("@REDEEMED", dayArchiveRow.Redeemed);
                command.Parameters.AddWithValue("@POINTS_PERCENT", dayArchiveRow.PointsPercent);
                command.Parameters.AddWithValue("@POINTS_EARNED", dayArchiveRow.PointsEarned);
                command.Parameters.AddWithValue("@LOYALTY_KEY", dayArchiveRow.LoyaltyId);
                command.Parameters.AddWithValue("@THIRDPARTYCODES_KEY", dayArchiveRow.ThirdPartyCodesId);
                command.Parameters.AddWithValue("@CATEGORY_KEY", dayArchiveRow.CategoryId);
                command.Parameters.AddWithValue("@DISCOUNT_REASON", "");
                command.Parameters.AddWithValue("@PRICE_LEVEL0", dayArchiveRow.PriceLevel0);
                command.Parameters.AddWithValue("@PRICE_LEVEL1", dayArchiveRow.PriceLevel1);
                command.Parameters.AddWithValue("@SERVINGCOURSES_KEY", dayArchiveRow.ServingCousesId);
                command.Parameters.AddWithValue("@CHIT_NAME", "");
                command.Parameters.AddWithValue("@CHIT_OPTION", "");
                command.Parameters.AddWithValue("@BASE_PRICE", dayArchiveRow.BasePrice);
                command.Parameters.AddWithValue("@DISCOUNT_WITHOUT_TAX", 0);
                command.Parameters.AddWithValue("@TAX_ON_DISCOUNT", 0);
                command.Parameters.AddWithValue("@PRICE_INCL", dayArchiveRow.PriceIncl);
                command.Parameters.AddWithValue("@PRICE_ADJUST", dayArchiveRow.PriceAdjust);
                command.Parameters.AddWithValue("@ONLINE_CHIT_TYPE", 0);//Need to confirm
                //command.Parameters.AddWithValue("@ONLINE_ORDER_ID", dayArchiveRow.OnlineOrderId);
                command.Parameters.AddWithValue("@ORDER_GUID", dayArchiveRow.OrderGuid);
                command.Parameters.AddWithValue("@ITEMSIZE_IDENTIFIER", dayArchiveRow.ItemSizeIdentifier);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoDayArchive " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoDayArcOrderTaxes(FbConnection connection, FbTransaction transaction, DayArcOrderTaxesAttributes dayArcOrderTaxesRow)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    INSERT INTO DAYARCORDERTAXES (
	                       ARCORDERTAXES_KEY,
	                       ARCHIVE_KEY,
	                       TAX_NAME,
	                       TAX_VALUE, 
	                       TAX_TYPE ) 
	                VALUES 
	                (
	                       @ARCORDERTAXES_KEY,
	                       @ARCHIVE_KEY,
	                       @TAX_NAME,
	                       @TAX_VALUE, 
	                       @TAX_TYPE 
	                );";
                command.Parameters.AddWithValue("@ARCORDERTAXES_KEY", dayArcOrderTaxesRow.ArcOrderTaxesID);
                command.Parameters.AddWithValue("@ARCHIVE_KEY", dayArcOrderTaxesRow.ArchiveId);
                command.Parameters.AddWithValue("@TAX_NAME", dayArcOrderTaxesRow.TaxName);
                command.Parameters.AddWithValue("@TAX_VALUE", dayArcOrderTaxesRow.TaxValue);
                command.Parameters.AddWithValue("@TAX_TYPE", dayArcOrderTaxesRow.TaxType);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoDayArcOrderTaxes " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoDayArcCategory(FbConnection connection, FbTransaction transaction, long archiveKey, long categoryKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    INSERT INTO DAYARCCATEGORY ( 
                           ARCHIVE_KEY, 
                           CATEGORY_KEY)  
                    VALUES ( 
                           @ARCHIVE_KEY,
					       @CATEGORY_KEY);";
                command.Parameters.AddWithValue("@ARCHIVE_KEY", archiveKey);
                command.Parameters.AddWithValue("@CATEGORY_KEY", categoryKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoDayArcCategory " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand InsertDataIntoSecurity(FbConnection connection, FbTransaction transaction, SecurityAttributes securityRow)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    INSERT INTO SECURITY ( 
                           SECURITY_KEY,  
                           SECURITY_REF,  
                           SECURITY_EVENT,  
                           FROM_VAL,  
                           TO_VAL,  
                           NOTE, 
		                   TERMINAL_NAME,  
                           USER_KEY,  
                           TIME_STAMP)  
                    VALUES ( 
                           @SECURITY_KEY,  
                           @SECURITY_REF,  
                           @SECURITY_EVENT,  
                           @FROM_VAL, 
		                   @TO_VAL,  
                           @NOTE,  
                           @TERMINAL_NAME,  
                           @USER_KEY,  
                           @TIME_STAMP);";
                command.Parameters.AddWithValue("@SECURITY_KEY", securityRow.SecurityId);
                command.Parameters.AddWithValue("@SECURITY_REF", securityRow.SecurityRef);
                command.Parameters.AddWithValue("@SECURITY_EVENT", QueryUtilities.GetFromSubstring(securityRow.SecurityEvent, 0, 50));
                command.Parameters.AddWithValue("@FROM_VAL", QueryUtilities.GetFromSubstring(securityRow.FromVal, 0, 50));
                command.Parameters.AddWithValue("@TO_VAL", QueryUtilities.GetFromSubstring(securityRow.ToVal, 0, 50));
                command.Parameters.AddWithValue("@NOTE", QueryUtilities.GetFromSubstring(securityRow.Note, 0, 50));
                command.Parameters.AddWithValue("@TERMINAL_NAME", securityRow.TerminalName);
                command.Parameters.AddWithValue("@USER_KEY", securityRow.UserId);
                command.Parameters.AddWithValue("@TIME_STAMP", DateTime.Now);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoSecurity " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand GetItemInfoForDayArchiveRow(FbConnection connection, FbTransaction transaction, int itemSizeIdentifier)
        {
            FbCommand result = new FbCommand(@"", connection, transaction);

            //....................................................

            try
            {
                result.CommandText = @"
                                    SELECT
                                       
                                        Menu.Menu_Name, 
                                        Course.Course_Key, 
                                        Course.Course_Name,
                                        Course.ServingCourses_Key,
                                        Item.Item_Key, 
                                        Item.Item_Kitchen_Name, 
                                        ItemSize.GST_Percent, 
                                        ItemSize.Cost, 
                                        ItemSize.Cost_GST_Percent, 
                                        ItemSize.Points_Percent, 
                                        ArcCategories.Category_Key,
                                        ArcCategories.Category,
                                        ItemSize.ThirdPartyCodes_Key
                                
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
                                        ItemSize.ITEMSIZE_IDENTIFIER = @ITEMSIZE_IDENTIFIER
                                    ORDER BY
                                        Menu.Menu_Name
                                    ";

                result.Parameters.AddWithValue("@ITEMSIZE_IDENTIFIER", itemSizeIdentifier);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetItemInfoForDayArchiveRow " + e.Message, e);
            }

            return result;
        }
        public FbCommand GetSyndicateCodeQuery(FbConnection connection, FbTransaction transaction)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    SELECT NAME FROM SYNDCODES WHERE ENABLED = @ENABLED;
                    ";
                command.Parameters.AddWithValue("@ENABLED", 'T');
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSyndicateCodeQuery " + e.Message, e);
                throw;
            }
            return command;
        }
        public FbCommand GetSecurityInfoQuery(FbConnection connection, FbTransaction transaction, string contactName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);
            try
            {
                command.CommandText = @"
                                    SELECT a.INITIALS, a.CONTACTS_KEY
                                    FROM CONTACTS a
                                    WHERE a.NAME = @NAME ;";

                command.Parameters.AddWithValue("@NAME", contactName);


            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetSecurityInfoQuery " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand InsertDataIntoOnlineOrders(FbConnection connection, FbTransaction transaction, OnlineOrderAttributes onlineOrderRow)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            //...........................................
            try
            {
                command.CommandText =
                @"
		            INSERT INTO ONLINEORDERS ( 
                            ONLINE_ORDER_KEY,  
                            PROFILE_KEY,
                            EFTPOS_RECEIPT,
                            INVOICE_NUMBER,
                            APP_TYPE,
                            ISPOSTED,
                            TERMINAL_NAME)    
                    VALUES (
		                    @ONLINE_ORDER_KEY,  
                            @PROFILE_KEY,  
                            @EFTPOS_RECEIPT,  
                            @INVOICE_NUMBER,  
                            @APP_TYPE,  
                            @ISPOSTED,  
                            @TERMINAL_NAME)
                            ;";

                command.Parameters.AddWithValue("@ONLINE_ORDER_KEY", onlineOrderRow.OnlineOrderId);
                command.Parameters.AddWithValue("@PROFILE_KEY", onlineOrderRow.ProfileId);
                command.Parameters.AddWithValue("@EFTPOS_RECEIPT", onlineOrderRow.EftposReceipt);
                command.Parameters.AddWithValue("@INVOICE_NUMBER", onlineOrderRow.InvoiceNumber);
                command.Parameters.AddWithValue("@APP_TYPE", onlineOrderRow.AppType);
                command.Parameters.AddWithValue("@ISPOSTED", onlineOrderRow.IsPosted ? "T" : "F");
                command.Parameters.AddWithValue("@TERMINAL_NAME", onlineOrderRow.TerminalName);
                

            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoOnlineOrders " + e.Message, e);
                throw;
            }
            return command;
        }

        public FbCommand GetProfileKeyQuery(FbConnection connection, FbTransaction transaction, string deviceKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    SELECT PROFILE_KEY FROM DEVICES WHERE UNIQUE_DEVICE_ID = @UNIQUE_DEVICE_ID;
                    ";
                command.Parameters.AddWithValue("@UNIQUE_DEVICE_ID", deviceKey);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in GetProfileKeyQuery " + e.Message, e);
                throw;
            }
            return command;
        }

        public FbCommand CheckIfWaiterAppTerminalExist(FbConnection connection, FbTransaction transaction, string terminalInfo)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    SELECT DEVICE_NAME FROM DEVICES WHERE UNIQUE_DEVICE_ID = @UNIQUE_DEVICE_ID;
                    ";
                command.Parameters.AddWithValue("@UNIQUE_DEVICE_ID", terminalInfo);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckIfWaiterAppTerminalExist " + e.Message, e);
                throw;
            }
            return command;
        }

        public FbCommand CheckIfZedRequestExistQuery(FbConnection connection, FbTransaction transaction, string terminalName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    SELECT APP_ZED_STATUSKEY FROM APPZEDSTATUS WHERE TERMINAL_NAME = @TERMINAL_NAME AND IS_ZED_REQUIRED = @IS_ZED_REQUIRED;
                    ";
                command.Parameters.AddWithValue("@TERMINAL_NAME", terminalName);
                command.Parameters.AddWithValue("@IS_ZED_REQUIRED", 'T');
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckIfZedRequestExistQuery " + e.Message, e);
                throw;
            }
            return command;
        }

        public FbCommand InsertAppZedRowQuery(FbConnection connection, FbTransaction transaction, string terminalName, long appZedKey, long profileKey)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
			        INSERT INTO APPZEDSTATUS ( 
                            APP_ZED_STATUSKEY,  
                            PROFILE_KEY,  
                            APP_TYPE,  
                            TERMINAL_NAME,
                            IS_ZED_REQUIRED,
                            TIME_STAMP_REQUESTED)  
                    VALUES (
			                @APP_ZED_STATUSKEY,  
                            @PROFILE_KEY,  
                            @APP_TYPE,  
                            @TERMINAL_NAME,
                            @IS_ZED_REQUIRED,
                            @TIME_STAMP_REQUESTED)
                            ;";
                command.Parameters.AddWithValue("@APP_ZED_STATUSKEY", appZedKey);
                command.Parameters.AddWithValue("@PROFILE_KEY", profileKey);
                command.Parameters.AddWithValue("@APP_TYPE", DTO.Enum.AppType.devWaiter);//Need to Confirm
                command.Parameters.AddWithValue("@TERMINAL_NAME", terminalName);
                command.Parameters.AddWithValue("@IS_ZED_REQUIRED", 'T');
                command.Parameters.AddWithValue("@TIME_STAMP_REQUESTED", DateTime.Now);//Need to Confirm
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in InsertDataIntoDayArcRef " + e.Message, e);
                throw;
            }
            return command;
        }

        public FbCommand CheckIfWaiterStaffExistQuery(FbConnection connection, FbTransaction transaction)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText =
                    @"
                    SELECT CONTACTS_KEY FROM CONTACTS WHERE NAME = @NAME;
                    ";
                command.Parameters.AddWithValue("@NAME", "WAITER");
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in CheckIfWaiterStaffExistQuery " + e.Message, e);
                throw;
            }
            return command;
        }

        public FbCommand UpdateProfileForWaiterApp(FbConnection connection, FbTransaction transaction, string deviceID, string deviceName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"
                                         UPDATE PROFILE a 
                                         SET a.NAME = @NAME
                                         WHERE a.PROFILE_KEY =
                                         (SELECT b.PROFILE_KEY FROM DEVICES b WHERE b.UNIQUE_DEVICE_ID = @UNIQUE_DEVICE_ID);
                                         ";

                command.Parameters.AddWithValue("@UNIQUE_DEVICE_ID", deviceID);
                command.Parameters.AddWithValue("@NAME", deviceName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetTableName " + e.Message, e);
                throw;
            }

            return command;
        }
        public FbCommand UpdateTerminalForWaiterApp(FbConnection connection, FbTransaction transaction, string deviceID, string deviceName)
        {
            FbCommand command = new FbCommand(@"", connection, transaction);

            try
            {
                command.CommandText = @"
                                         UPDATE DEVICES a SET a.DEVICE_NAME = @DEVICE_NAME
                                         WHERE a.UNIQUE_DEVICE_ID = @UNIQUE_DEVICE_ID;
                                         ";

                command.Parameters.AddWithValue("@UNIQUE_DEVICE_ID", deviceID);
                command.Parameters.AddWithValue("@NAME", deviceName);
            }
            catch (Exception e)
            {
                ServiceLogger.LogException(@"in SetTableName " + e.Message, e);
                throw;
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

        public static string GetFromSubstring(string str, int start, int maxLength)
        {
            if (str != null)
                str = str.Length > maxLength ? str.Substring(start, maxLength) : str;
            else
                str = "";
            return str;
        }
    }
}
