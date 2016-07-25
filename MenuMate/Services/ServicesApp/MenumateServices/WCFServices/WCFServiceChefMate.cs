using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.IO;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using FirebirdSql.Data.FirebirdClient;

using MenumateServices.ChefMate.DTO;
using MenumateServices.Utilities;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceChefMate" in both code and config file together.
    public class WCFServiceChefMate : IWCFServiceChefMate
    {
        #region Utilities
        #region CONSTANTS
        private const string USERNAME = "SYSDBA";
        private const string PASSWORD = "masterkey";
        #endregion

        #region ENUMS
        enum TerminalNameType { COMPUTER = 0, USER_ADDED }
        #endregion

        #region DELEGATES
        private delegate void queryMethod(FbCommand inCommand);
        #endregion

        #region PRIVATE
        private Properties.Settings settings = Properties.Settings.Default;

        private FbConnection makeFbConnection()
        {
            string connectionString = "";
            connectionString += "User=" + USERNAME + "; ";
            connectionString += "Password=" + PASSWORD + "; ";

            ServiceInfo serviceInfo = ServiceInfo.Instance;

            connectionString += "Database=" + Path.Combine(serviceInfo.ChefmateDatabaseLocation(), settings.ChefmateDatabaseName) + "; ";
            connectionString += "Datasource=" + serviceInfo.ChefmateDatabaseServerPath() + "; ";
            //connectionString += "Database=" + Path.Combine(settings.ChefmateDatabaseLocation + settings.ChefmateDatabaseName) + "; ";
            //connectionString += "Datasource=" + settings.ChefmateDatabaseIPAddress + "; ";

            return new FbConnection(connectionString);
        }

        private int generateKey(FbCommand inCommand, string inTableName)
        {
            object commandResult = null;
            string generatorName = "GEN_" + inTableName;
            inCommand.CommandText = "SELECT GEN_ID(" + generatorName + ", 1) FROM RDB$DATABASE";
            if ((commandResult = inCommand.ExecuteScalar()) == null)
            {
                throw new KeyGeneratorException(inTableName + " key genertor failed to produce key.");
            }
            return Convert.ToInt32(commandResult);
        }

        private void executeQuery(queryMethod inQueryMethod, DTO_ChefMateResponse outResponse)
        {
            initalizeResponse(outResponse);
            try
            {
                using (FbConnection connection = makeFbConnection())
                {
                    connection.Open();
                    FbTransaction transaction = connection.BeginTransaction();
                    try
                    {
                        FbCommand command = new FbCommand("", connection, transaction);
                        command.CommandTimeout = 5;
                        inQueryMethod(command);
                        transaction.Commit();
                    }
                    catch (Exception e)
                    {
                        transaction.Rollback();
                        ServiceLogger.LogChefMateException("", e);
                        throw e;
                    }
                }
            }
            catch (KeyGeneratorException e)
            {
                keyGeneratorExceptionResponse(outResponse, e);
            }
            catch (ScalerFailureException e)
            {
                scalerFailureExceptionResponse(outResponse, e);
            }
            catch (FbException e)
            {
                firebirdExceptionResponse(outResponse, e);
            }
            catch (Exception e)
            {
                exceptionResponse(outResponse, e);
            }
        }

        private void initalizeResponse(DTO_ChefMateResponse outResponse)
        {
            outResponse.Successful = true;
            outResponse.Response = ResponseCode.Successful;
            outResponse.Description = "";
            outResponse.Message = "";
        }

        private void firebirdExceptionResponse(DTO_ChefMateResponse outResponse, FbException e)
        {
            outResponse.Successful = false;
            outResponse.Response = ResponseCode.FirebirdError;
            outResponse.Description = "Firebird has thrown an FbException.";
            outResponse.Message = e.Message;
        }

        private void keyGeneratorExceptionResponse(DTO_ChefMateResponse outResponse, KeyGeneratorException e)
        {
            outResponse.Successful = false;
            outResponse.Response = ResponseCode.KeyGeneratorError;
            outResponse.Description = "The key generator for this table has failed.";
            outResponse.Message = e.Message;
        }

        private void scalerFailureExceptionResponse(DTO_ChefMateResponse response, ScalerFailureException e)
        {
            response.Successful = false;
            response.Response = ResponseCode.ScalerFailureError;
            response.Description = "An scaler query has return a null value.";
            response.Message = e.Message;
        }

        private void exceptionResponse(DTO_ChefMateResponse response, Exception e)
        {
            response.Successful = false;
            response.Response = ResponseCode.UnknownError;
            response.Description = "An exception has been caught.";
            response.Message = e.Message;
        }
        #endregion

        #region EXCEPTIONS
        private class KeyGeneratorException : Exception
        {
            public KeyGeneratorException() : base() { }
            public KeyGeneratorException(string message) : base(message) { }
            public KeyGeneratorException(string message, System.Exception inner) : base(message, inner) { }
        }

        private class ScalerFailureException : Exception
        {
            public ScalerFailureException() : base() { }
            public ScalerFailureException(string message) : base(message) { }
            public ScalerFailureException(string message, System.Exception inner) : base(message, inner) { }
        }
        #endregion
        #endregion

        #region DBAuxiliary
        public DTO_ChefMateBooleanResponse DatabaseConnectionTest()
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            initalizeResponse(response);
            response.ReturnValue = true;
            try
            {
                using (FbConnection connection = makeFbConnection())
                {
                    connection.Open();
                    FbCommand command = new FbCommand(@"select rdb$relation_name
                                                        from rdb$relations
                                                        where rdb$view_blr is null 
                                                        and (rdb$system_flag is null or rdb$system_flag = 0);",
                                                      connection);
                    command.ExecuteReader();
                }
            }
            catch (FbException e)
            {
                response.ReturnValue = false;
                firebirdExceptionResponse(response, e);
            }
            catch (Exception e)
            {
                response.ReturnValue = false;
                exceptionResponse(response, e);
            }
            return response;
        }
        #endregion

        #region DBGroup
        #region PUBLIC
        public DTO_ChefMateBooleanResponse CheckGroupInDb(ChefMateGroupType inGroupType, int inGroupPosDbKey)
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            response.ReturnValue = false;
            queryMethod query = delegate(FbCommand inCommand) { checkGroupInDb(inCommand, inGroupType, inGroupPosDbKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("CheckGroupInDb()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse AddGroup(string inName, ChefMateGroupType inGroupType, int inGroupPosDbKey, int inGroupDisplayOrder)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addGroup(inCommand, inName, inGroupType, inGroupPosDbKey, inGroupDisplayOrder); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse UpdateGroupDisplayOrder(int inGroupPosDbKey, int inGroupDisplayOrder, ChefMateGroupType inGroupType)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { updateGroupDisplayOrder(inCommand, inGroupPosDbKey, inGroupDisplayOrder, inGroupType); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerResponse GetGroupDisplayOrder(int inGroupPosDbKey, ChefMateGroupType inGroupType)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { getGroupDisplayOrder(inCommand, inGroupPosDbKey, inGroupType, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void checkGroupInDb(FbCommand inCommand, ChefMateGroupType inGroupType, int inGroupPosDbKey, DTO_ChefMateBooleanResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"SELECT
                    GROUP_KEY
                FROM
                    GROUPS
                WHERE
                    GROUP_TYPE = @GROUP_TYPE 
                    AND
                    GROUP_POS_KEY = @GROUP_POS_KEY";
            switch (inGroupType)
            {
                case ChefMateGroupType.COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'C');
                    break;
                case ChefMateGroupType.SERVING_COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'S');
                    break;
            }
            inCommand.Parameters.AddWithValue("@GROUP_POS_KEY", inGroupPosDbKey);

            commandResult = inCommand.ExecuteScalar();
            outResponse.ReturnValue = commandResult != null;
        }

        private void addGroup(FbCommand inCommand, string inName, ChefMateGroupType inGroupType, int inGroupPosDbKey, int inGroupDisplayOrder)
        {
            int groupKey = generateKey(inCommand, "GROUPS");

            inCommand.CommandText =
              @"INSERT INTO GROUPS
                (
                    GROUP_KEY,
                    GROUP_NAME,
                    GROUP_TYPE,
                    GROUP_POS_KEY,
                    GROUP_DISPLAY_ORDER
                )
                VALUES
                (
                    @GROUP_KEY,
                    @GROUP_NAME,
                    @GROUP_TYPE,
                    @GROUP_POS_KEY,
                    @GROUP_DISPLAY_ORDER
                )";
            inCommand.Parameters.AddWithValue("@GROUP_KEY", groupKey);
            inCommand.Parameters.AddWithValue("@GROUP_NAME", inName);
            switch (inGroupType)
            {
                case ChefMateGroupType.COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'C');
                    break;
                case ChefMateGroupType.SERVING_COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'S');
                    break;
            }
            inCommand.Parameters.AddWithValue("@GROUP_POS_KEY", inGroupPosDbKey);
            inCommand.Parameters.AddWithValue("@GROUP_DISPLAY_ORDER", inGroupDisplayOrder);
            inCommand.ExecuteNonQuery();
        }

        private void updateGroupDisplayOrder(FbCommand inCommand, int inGroupPosDbKey, int inGroupDisplayOrder, ChefMateGroupType inGroupType)
        {
            int groupKey = getGroupKey(inCommand, inGroupPosDbKey, inGroupType);
            inCommand.CommandText =
              @"UPDATE GROUPS
                SET 
                    GROUP_DISPLAY_ORDER = @GROUP_DISPLAY_ORDER
                WHERE
                    GROUP_KEY = @GROUP_KEY";
            inCommand.Parameters.AddWithValue("@GROUP_KEY", groupKey);
            inCommand.Parameters.AddWithValue("@GROUP_DISPLAY_ORDER", inGroupDisplayOrder);
            inCommand.ExecuteNonQuery();
        }

        private void getGroupDisplayOrder(FbCommand inCommand, int inGroupPosDbKey, ChefMateGroupType inGroupType, DTO_ChefMateIntegerResponse outResponse)
        {
            int groupKey = getGroupKey(inCommand, inGroupPosDbKey, inGroupType);
            inCommand.Parameters.Clear();
            inCommand.CommandText =
              @"SELECT GROUP_DISPLAY_ORDER 
                FROM GROUPS
                WHERE
                    GROUP_KEY = @GROUP_KEY AND GROUP_TYPE = @GROUP_TYPE";

            switch (inGroupType)
            {
                case ChefMateGroupType.COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", "C");
                    break;
                case ChefMateGroupType.SERVING_COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", "S");
                    break;
                default:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", "");
                    break;
            }

            inCommand.Parameters.AddWithValue("@GROUP_KEY", groupKey);
            outResponse.ReturnValue = Convert.ToInt32(inCommand.ExecuteScalar());
        }

        private int getGroupKey(FbCommand inCommand, int inGroupPosKey, ChefMateGroupType inGroupType)
        {
            object commandResult;
            int groupKey;

            inCommand.CommandText =
              @"SELECT
                    GROUP_KEY
                FROM
                    GROUPS
                WHERE
                    GROUP_POS_KEY = @GROUP_POS_KEY
                AND
                    GROUP_TYPE = @GROUP_TYPE";
            inCommand.Parameters.AddWithValue("@GROUP_POS_KEY", inGroupPosKey);
            switch (inGroupType)
            {
                case ChefMateGroupType.COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'C');
                    break;
                case ChefMateGroupType.SERVING_COURSE:
                    inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'S');
                    break;
            }

            commandResult = inCommand.ExecuteScalar();
            if (commandResult == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                throw new ScalerFailureException(string.Format("Scaler returned in null {0}. GroupPosKey: {1}, GroupType: {2}.", methodBase.Name, inGroupPosKey, inGroupType));
            }
            else
            {
                groupKey = Convert.ToInt32(commandResult);
            }

            return groupKey;
        }
        #endregion
        #endregion

        #region DBItem
        #region PUBLIC
        public DTO_ChefMateBooleanResponse CheckItemInDb(int inItemPosDbKey)
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            response.ReturnValue = false;
            queryMethod query = delegate(FbCommand inCommand) { checkItemInDb(inCommand, inItemPosDbKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("CheckGroupInDb()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse AddItem(string inName, int inItemPosDbKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addItem(inCommand, inName, inItemPosDbKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //ServiceLogger.LogChefMateError(//.log("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerResponse GetItemKey(int inItemPosDbKey)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { getItemKey(inCommand, inItemPosDbKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("CheckGroupInDb()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateBooleanResponse UpdateOrderItemGroupKey(int inOrderItemKey, int inCourseKey, int inSCourseKey)
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            response.ReturnValue = false;
            queryMethod query = delegate(FbCommand inCommand) { updateOrderItemGroupKey(inCommand, inOrderItemKey, inCourseKey, inSCourseKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("CheckGroupInDb()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void checkItemInDb(FbCommand inCommand, int inItemPosDbKey, DTO_ChefMateBooleanResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"SELECT
                    ITEM_KEY
                FROM
                    ITEM
                WHERE
                    ITEM_POS_KEY = @ITEM_POS_KEY";
            inCommand.Parameters.AddWithValue("@ITEM_POS_KEY", inItemPosDbKey);

            commandResult = inCommand.ExecuteScalar();
            outResponse.ReturnValue = commandResult != null;
        }

        private void addItem(FbCommand inCommand, string inName, int inItemPosDbKey)
        {
            int itemKey = generateKey(inCommand, "ITEM");

            inCommand.CommandText =
              @"INSERT INTO ITEM
                (
                    ITEM_KEY,
                    ITEM_NAME,
                    ITEM_POS_KEY
                )
                VALUES
                (
                    @ITEM_KEY,
                    @ITEM_NAME,
                    @ITEM_POS_KEY
                )";
            inCommand.Parameters.AddWithValue("@ITEM_KEY", itemKey);
            inCommand.Parameters.AddWithValue("@ITEM_NAME", inName);
            inCommand.Parameters.AddWithValue("@ITEM_POS_KEY", inItemPosDbKey);
            inCommand.ExecuteNonQuery();
        }

        private int getItemKey(FbCommand inCommand, int inItemPosDbKey, DTO_ChefMateIntegerResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"SELECT
                    ITEM_KEY
                FROM
                    ITEM
                WHERE
                    ITEM_POS_KEY = @ITEM_POS_KEY";
            inCommand.Parameters.AddWithValue("@ITEM_POS_KEY", inItemPosDbKey);

            commandResult = inCommand.ExecuteScalar();
            if (commandResult == null)
            {
                outResponse.ReturnValue = -1;
            }
            else
            {
                outResponse.ReturnValue = Convert.ToInt32(commandResult);
            }
            return outResponse.ReturnValue;
        }

        private void updateOrderItemGroupKey(FbCommand inCommand, int inOrderItemKey, int inCourseKey, int inSCourseKey, DTO_ChefMateBooleanResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"UPDATE
                    ORDERITEM
                SET
                    C_ORDERGROUP_KEY = @C_ORDERGROUP_KEY,
                    SC_ORDERGROUP_KEY = @SC_ORDERGROUP_KEY
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";

            // @"UPDATE ORDERS SET TABLE_TAB_NAME = @TABLE_TAB_NAME WHERE ORDER_POS_KEY = @ORDER_POS_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
            inCommand.Parameters.AddWithValue("@C_ORDERGROUP_KEY", inCourseKey);
            inCommand.Parameters.AddWithValue("@SC_ORDERGROUP_KEY", inSCourseKey);

            commandResult = inCommand.ExecuteScalar();
            outResponse.ReturnValue = commandResult != null;
        }

        #endregion
        #endregion

        #region DBOption
        #region PUBLIC
        public DTO_ChefMateBooleanResponse CheckOptionInDb(string inOptionName)
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            response.ReturnValue = false;
            queryMethod query = delegate(FbCommand inCommand) { checkOptionInDb(inCommand, inOptionName, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("CheckGroupInDb()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse AddOption(string inOptionName)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addOption(inCommand, inOptionName); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void checkOptionInDb(FbCommand inCommand, string inOptionName, DTO_ChefMateBooleanResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"SELECT
                    OPTION_KEY
                FROM
                    OPTION
                WHERE
                    OPTION_NAME = @OPTION_NAME";
            inCommand.Parameters.AddWithValue("@OPTION_NAME", inOptionName);

            commandResult = inCommand.ExecuteScalar();
            outResponse.ReturnValue = commandResult != null;
        }

        private void addOption(FbCommand inCommand, string inOptionName)
        {
            int optionKey = generateKey(inCommand, "OPTION");

            inCommand.CommandText =
              @"INSERT INTO OPTION
                (
                    OPTION_KEY,
                    OPTION_NAME
                )
                VALUES
                (
                    @OPTION_KEY,
                    @OPTION_NAME
                )";
            inCommand.Parameters.AddWithValue("@OPTION_KEY", optionKey);
            inCommand.Parameters.AddWithValue("@OPTION_NAME", inOptionName);
            inCommand.ExecuteNonQuery();
        }

        private int getOptionKey(FbCommand inCommand, string inOptionName)
        {
            object commandResult;
            int optionKey;

            inCommand.CommandText =
              @"SELECT
                    OPTION_KEY
                FROM
                    OPTION
                WHERE
                    OPTION_NAME = @OPTION_NAME";
            inCommand.Parameters.AddWithValue("@OPTION_NAME", inOptionName);

            commandResult = inCommand.ExecuteScalar();
            if (commandResult == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                throw new ScalerFailureException(string.Format("Scaler returned in null {0}. OptionName: {1}.", methodBase.Name, inOptionName));
            }
            else
            {
                optionKey = Convert.ToInt32(commandResult);
            }
            return optionKey;
        }
        #endregion
        #endregion

        #region DBOrder
        #region PUBLIC
        public DTO_ChefMateIntegerResponse AddOrder(DTO_ChefMateOrder inOrderStruct)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { addOrder(inCommand, inOrderStruct, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateOrderResponse GetOrder(int inOrderKey)
        {
            DTO_ChefMateOrderResponse response = new DTO_ChefMateOrderResponse();
            response.ReturnValue = new DTO_ChefMateOrder();
            queryMethod query = delegate(FbCommand inCommand) { getOrder(inCommand, inOrderKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateFullOrderResponse GetFullOrderFromOrderKey(int inOrderKey, int inTerminalKey)
        {
            DTO_ChefMateFullOrderResponse response = new DTO_ChefMateFullOrderResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getFullOrderFromOrderKey(inCommand, inOrderKey, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateIntegerListResponse GetOrderKeysFromTerminalKey(int inTerminalKey)
        {
            DTO_ChefMateIntegerListResponse response = new DTO_ChefMateIntegerListResponse();
            response.ReturnValue = new List<int>();
            queryMethod query = delegate(FbCommand inCommand) { getOrderKeysFromTerminalKey(inCommand, inTerminalKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateDataTableResponse GetBumpedOrders()
        {
            DTO_ChefMateDataTableResponse response = new DTO_ChefMateDataTableResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getBumpedOrders(inCommand, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateDataTableResponse GetBumpedOrderInformation(int inOrderKey)
        {
            DTO_ChefMateDataTableResponse response = new DTO_ChefMateDataTableResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getBumpedOrderInformation(inCommand, inOrderKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse UnbumpOrder(int inOrderKey, int inTerminalKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { unbumpOrder(inCommand, inOrderKey, inTerminalKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateDataTableResponse GetModelKeysFromOrderKey(int inOrderKey, int inTerminalKey)
        {
            DTO_ChefMateDataTableResponse response = new DTO_ChefMateDataTableResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getModelKeysFromOrderKey(inCommand, inOrderKey, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateResponse UpdateOrderTerminalName(int inOrderKey, string inTableTabName)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { updateOrderTerminalName(inCommand, inOrderKey, inTableTabName); };
            executeQuery(query, response);
            return response;
        }

        public DTO_ChefMateFullOrderResponse GetFullOrderFromOrderPosKey(int inOrderPosKey, int inTerminalKey)
        {
            DTO_ChefMateFullOrderResponse response = new DTO_ChefMateFullOrderResponse { ReturnValue = null };
            queryMethod query = delegate(FbCommand inCommand) { getFullOrderFromOrderPosKey(inCommand, inOrderPosKey, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateFullOrderResponse GetFullOrderFromTableTabName(string inTableTabName, int inTerminalKey)
        {
            DTO_ChefMateFullOrderResponse response = new DTO_ChefMateFullOrderResponse { ReturnValue = null };
            queryMethod query = delegate(FbCommand inCommand) { getFullOrderFromTableTabName(inCommand, inTableTabName, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        #endregion

        #region PRIVATE
        private void addOrder(FbCommand inCommand, DTO_ChefMateOrder inOrderStruct, DTO_ChefMateIntegerResponse outResponse)
        {
            int orderDbKey = generateKey(inCommand, "ORDERS");

            inCommand.CommandText =
              @"INSERT INTO ORDERS
                (
                    ORDER_KEY,
                    ORDER_NUMBER,
                    ORDER_POS_KEY,
                    PATRON_COUNT,
                    CHIT_VALUE,
                    TABLE_TAB_NAME,
                    PARTY_NAME,
                    SERVER_NAME,
                    CUSTOMER_NAME,
                    ORDER_TYPE,
                    ORDER_SALE_START_TIME,
                    ORDER_SALE_FINISH_TIME,
                    DELIVERY_TIME
                )
                VALUES
                (
                    @ORDER_KEY,
                    @ORDER_NUMBER,
                    @ORDER_POS_KEY,
                    @PATRON_COUNT,
                    @CHIT_VALUE,
                    @TABLE_TAB_NAME,
                    @PARTY_NAME,
                    @SERVER_NAME,
                    @CUSTOMER_NAME,
                    @ORDER_TYPE,
                    @ORDER_SALE_START_TIME,
                    @ORDER_SALE_FINISH_TIME,
                    @DELIVERY_TIME
                )";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", orderDbKey);
            inCommand.Parameters.AddWithValue("@ORDER_NUMBER", inOrderStruct.OrderNumber);
            inCommand.Parameters.AddWithValue("@ORDER_POS_KEY", inOrderStruct.OrderPosDbKey);
            inCommand.Parameters.AddWithValue("@PATRON_COUNT", inOrderStruct.PatronCount);
            inCommand.Parameters.AddWithValue("@CHIT_VALUE", inOrderStruct.ChitValue);
            inCommand.Parameters.AddWithValue("@TABLE_TAB_NAME", inOrderStruct.TableTabName);
            inCommand.Parameters.AddWithValue("@PARTY_NAME", inOrderStruct.PartyName);
            inCommand.Parameters.AddWithValue("@SERVER_NAME", inOrderStruct.ServerName);
            inCommand.Parameters.AddWithValue("@CUSTOMER_NAME", inOrderStruct.CustomerName);
            inCommand.Parameters.AddWithValue("@ORDER_TYPE", inOrderStruct.OrderType);
            inCommand.Parameters.AddWithValue("@ORDER_SALE_START_TIME", inOrderStruct.SaleStartTime);
            inCommand.Parameters.AddWithValue("@ORDER_SALE_FINISH_TIME", inOrderStruct.SaleFinishTime);
            inCommand.Parameters.AddWithValue("@DELIVERY_TIME", inOrderStruct.DeliveryTime);
            inCommand.ExecuteNonQuery();

            outResponse.ReturnValue = orderDbKey;
        }

        private void getOrder(FbCommand inCommand, int inOrderKey, DTO_ChefMateOrderResponse outResponse)
        {
            DTO_ChefMateOrder dbOrderStruct;

            inCommand.CommandText =
              @"SELECT
                    ORDER_NUMBER,
                    ORDER_POS_KEY,
                    CHIT_VALUE,
                    TABLE_TAB_NAME,
                    PARTY_NAME,
                    SERVER_NAME,
                    CUSTOMER_NAME,
                    ORDER_TYPE,
                    PATRON_COUNT,
                    ORDER_SALE_START_TIME,
                    ORDER_SALE_FINISH_TIME,
                    DELIVERY_TIME
                FROM
                    ORDERS
                WHERE
                    ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                reader.Read();
                dbOrderStruct = new DTO_ChefMateOrder();

                dbOrderStruct.OrderDbKey = inOrderKey;
                dbOrderStruct.OrderNumber = Convert.ToInt32(reader["ORDER_NUMBER"]);
                dbOrderStruct.OrderPosDbKey = Convert.ToInt32(reader["ORDER_POS_KEY"]);
                dbOrderStruct.PatronCount = Convert.ToInt32(reader["PATRON_COUNT"]);
                dbOrderStruct.ChitValue = Convert.ToString(reader["CHIT_VALUE"]);
                dbOrderStruct.TableTabName = Convert.ToString(reader["TABLE_TAB_NAME"]);
                dbOrderStruct.PartyName = Convert.ToString(reader["PARTY_NAME"]);
                dbOrderStruct.ServerName = Convert.ToString(reader["SERVER_NAME"]);
                dbOrderStruct.CustomerName = Convert.ToString(reader["CUSTOMER_NAME"]);
                dbOrderStruct.OrderType = Convert.ToString(reader["ORDER_TYPE"]);
                dbOrderStruct.SaleStartTime = Convert.ToDateTime(reader["ORDER_SALE_START_TIME"]);
                dbOrderStruct.SaleFinishTime = Convert.ToDateTime(reader["ORDER_SALE_FINISH_TIME"]);
                dbOrderStruct.DeliveryTime = Convert.ToDateTime(reader["DELIVERY_TIME"] == DBNull.Value ? DateTime.MinValue : reader["DELIVERY_TIME"]);
            }

            outResponse.ReturnValue = dbOrderStruct;
        }

        private void getFullOrderFromOrderKey(FbCommand inCommand, int inOrderKey, int inTerminalKey, DTO_ChefMateFullOrderResponse outResponse)
        {
            IList<DTO_ChefMateFullOrder> dbFullOrderStructList = new List<DTO_ChefMateFullOrder>();

            inCommand.CommandText =
              @"SELECT                    
                    O.ORDER_NUMBER,
                    O.ORDER_POS_KEY,
                    O.CHIT_VALUE,
                    O.TABLE_TAB_NAME,
                    O.PARTY_NAME,
                    O.SERVER_NAME,
                    O.CUSTOMER_NAME,
                    O.ORDER_TYPE,
                    O.PATRON_COUNT,
                    O.ORDER_SALE_START_TIME,
                    O.ORDER_SALE_FINISH_TIME,       
                    O.DELIVERY_TIME,
                    C.ORDERCOURSE_KEY,
                    C.COURSE_NAME,
                    C.COURSE_POS_KEY,
                    C.COURSE_DISPLAY_ORDER,
                    C.ORDERCOURSE_CALLED_AWAY,
                    C.ORDERCOURSE_BUMP_TIME,
                    S.ORDERSERVINGCOURSE_KEY,
                    S.SERVINGCOURSE_NAME,
                    S.SERVINGCOURSE_POS_KEY,
                    S.SERVINGCOURSE_DISPLAY_ORDER,
                    S.ORDERSERVINGCOURSE_CALLED_AWAY,
                    S.ORDERSERVINGCOURSE_BUMP_TIME,
                    I.ORDERITEM_KEY,
                    I.ITEM_POS_KEY,
                    I.ORDERITEM_POS_KEY,
                    I.ITEM_NAME,
                    I.STATUS,
                    I.ORDERITEM_NOTE,
                    T.ORDERITEMTIME_TIME
                FROM
                    ORDERS O INNER JOIN MODELCOURSEVIEW C 
                    ON O.ORDER_KEY = C.ORDER_KEY
                    INNER JOIN MODELSERVINGCOURSEVIEW S 
                    ON O.ORDER_KEY = S.ORDER_KEY
                    INNER JOIN MODELITEMVIEW I
                    ON C.ORDERCOURSE_KEY = I.C_ORDERGROUP_KEY
                    AND S.ORDERSERVINGCOURSE_KEY = I.SC_ORDERGROUP_KEY
                    INNER JOIN ORDERITEMTIME T
                    ON I.ORDERITEM_KEY = T.ORDERITEM_KEY
                    AND T.ORDERITEMTIME_TYPE = 'A'
                WHERE O.ORDER_KEY = @ORDER_KEY AND I.TERMINAL_KEY = @TERMINAL_KEY ";


                inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);            
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    DTO_ChefMateFullOrder dbFullOrderStruct = new DTO_ChefMateFullOrder
                    {                        
                        OrderDbKey = inOrderKey,
                        OrderNumber = Convert.ToInt32(reader["ORDER_NUMBER"]),
                        OrderPosDbKey = Convert.ToInt32(reader["ORDER_POS_KEY"]),
                        PatronCount = Convert.ToInt32(reader["PATRON_COUNT"]),
                        ChitValue = Convert.ToString(reader["CHIT_VALUE"]),
                        TableTabName = Convert.ToString(reader["TABLE_TAB_NAME"]),
                        PartyName = Convert.ToString(reader["PARTY_NAME"]),
                        ServerName = Convert.ToString(reader["SERVER_NAME"]),
                        CustomerName = Convert.ToString(reader["CUSTOMER_NAME"]),
                        OrderType = Convert.ToString(reader["ORDER_TYPE"]),
                        SaleStartTime = Convert.ToDateTime(reader["ORDER_SALE_START_TIME"]),
                        SaleFinishTime = Convert.ToDateTime(reader["ORDER_SALE_FINISH_TIME"]),
                        CourseKey = Convert.ToInt32(reader["ORDERCOURSE_KEY"]),
                        CourseName = Convert.ToString(reader["COURSE_NAME"]),
                        CoursePosKey = Convert.ToInt32(reader["COURSE_POS_KEY"]),
                        CourseDisplayOrder = Convert.ToInt32(reader["COURSE_DISPLAY_ORDER"]),
                        CourseCalledAway = Convert.ToString(reader["ORDERCOURSE_CALLED_AWAY"]) == "T",
                        CourseBumpTime =
                            Convert.ToDateTime(reader["ORDERCOURSE_BUMP_TIME"] == DBNull.Value
                                ? DateTime.MinValue
                                : reader["ORDERCOURSE_BUMP_TIME"]),
                        ServingCourseKey = Convert.ToInt32(reader["ORDERSERVINGCOURSE_KEY"]),
                        ServingCourseName = Convert.ToString(reader["SERVINGCOURSE_NAME"]),
                        ServingCoursePosKey = Convert.ToInt32(reader["SERVINGCOURSE_POS_KEY"]),
                        ServingCourseDisplayOrder = Convert.ToInt32(reader["SERVINGCOURSE_DISPLAY_ORDER"]),
                        ServingCourseCalledAway = Convert.ToString(reader["ORDERSERVINGCOURSE_CALLED_AWAY"]) == "T",
                        ServingCourseBumpTime =
                            Convert.ToDateTime(reader["ORDERSERVINGCOURSE_BUMP_TIME"] == DBNull.Value
                                ? DateTime.MinValue
                                : reader["ORDERSERVINGCOURSE_BUMP_TIME"]),
                        OrderItemKey = Convert.ToInt32(reader["ORDERITEM_KEY"]),
                        ItemPosDbKey = Convert.ToInt32(reader["ITEM_POS_KEY"]),
                        OrderItemPosDbKey = Convert.ToInt32(reader["ORDERITEM_POS_KEY"]),
                        ItemName = Convert.ToString(reader["ITEM_NAME"]),
                        OrderItemStatus = Convert.ToString(reader["STATUS"]),
                        OrderItemNote = Convert.ToString(reader["ORDERITEM_NOTE"]),
                        OrderItemArrivalTime =
                            Convert.ToDateTime(reader["ORDERITEMTIME_TIME"] == DBNull.Value
                                ? DateTime.MinValue
                                : reader["ORDERITEMTIME_TIME"]),
                        OrderItemOptionKey = -1,
                        OptionName = "",
                        OrderItemOptionIsPlus = false,
                        OrderItemSideKey = -1,
                        OrderItemSidePosKey = -1,
                        SideName = "",
                        OrderItemSideIsCanceled = false,
                        DeliveryTime = Convert.ToDateTime(reader["DELIVERY_TIME"] == DBNull.Value ? DateTime.MinValue : reader["DELIVERY_TIME"])
                    };

                    FbCommand optionsSidesCommand = new FbCommand("", inCommand.Connection, inCommand.Transaction);
                    bool itemAddedToOutput = false;

                    itemAddedToOutput |= loadItemOptions(optionsSidesCommand, dbFullOrderStruct, dbFullOrderStructList);
                    itemAddedToOutput |= loadItemSides(optionsSidesCommand, dbFullOrderStruct, dbFullOrderStructList);

                    if (!itemAddedToOutput)
                    {
                        dbFullOrderStructList.Add(dbFullOrderStruct);
                    }
                }
            }

            outResponse.ReturnValue = dbFullOrderStructList;
        }

        private static bool loadItemOptions(FbCommand inCommand, DTO_ChefMateFullOrder inDbFullOrderStruct, IList<DTO_ChefMateFullOrder> outDbFullOrderStructList)
        {
            bool itemAddedToOutput = false;

            inCommand.CommandText =
                @"SELECT
                    ORDERITEMOPTION_KEY, 
                    OPTION_NAME, 
                    IS_PLUS
                FROM 
                    MODELOPTIONVIEW
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inDbFullOrderStruct.OrderItemKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    DTO_ChefMateFullOrder dbFullOrderWithOptionStruct = new DTO_ChefMateFullOrder();
                    inDbFullOrderStruct.CopyTo(dbFullOrderWithOptionStruct);

                    dbFullOrderWithOptionStruct.OrderItemOptionKey = Convert.ToInt32(reader["ORDERITEMOPTION_KEY"]);
                    dbFullOrderWithOptionStruct.OptionName = Convert.ToString(reader["OPTION_NAME"]);
                    dbFullOrderWithOptionStruct.OrderItemOptionIsPlus = Convert.ToString(reader["IS_PLUS"]) == "T";

                    outDbFullOrderStructList.Add(dbFullOrderWithOptionStruct);

                    itemAddedToOutput = true;
                }
            }
            return itemAddedToOutput;
        }

        private static bool loadItemSides(FbCommand inCommand, DTO_ChefMateFullOrder inDbFullOrderStruct, IList<DTO_ChefMateFullOrder> outDbFullOrderStructList)
        {
            bool itemAddedToOutput = false;

            inCommand.CommandText =
              @"SELECT
                    ORDERITEMSIDE_KEY,
                    ORDERITEMSIDE_POS_KEY,
                    SIDE_NAME,
                    IS_CANCELED
                FROM
                    MODELSIDEVIEW
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inDbFullOrderStruct.OrderItemKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    DTO_ChefMateFullOrder dbFullOrderWithSideStruct = new DTO_ChefMateFullOrder();
                    inDbFullOrderStruct.CopyTo(dbFullOrderWithSideStruct);

                    dbFullOrderWithSideStruct.OrderItemSideKey = Convert.ToInt32(reader["ORDERITEMSIDE_KEY"]);
                    dbFullOrderWithSideStruct.OrderItemSidePosKey = Convert.ToInt32(reader["ORDERITEMSIDE_POS_KEY"]);
                    dbFullOrderWithSideStruct.SideName = Convert.ToString(reader["SIDE_NAME"]);
                    dbFullOrderWithSideStruct.OrderItemSideIsCanceled = Convert.ToString(reader["IS_CANCELED"]) == "T"; ;

                    outDbFullOrderStructList.Add(dbFullOrderWithSideStruct);

                    itemAddedToOutput = true;
                }
            }
            return itemAddedToOutput;
        }

        private void getOrderKeysFromTerminalKey(FbCommand inCommand, int inTerminalKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> orderKeys = new List<int>();

            inCommand.CommandText =
              @"SELECT
                    DISTINCT ORDER_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    TERMINAL_KEY = @TERMINAL_KEY";
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    orderKeys.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = orderKeys;
        }

        private void getBumpedOrders(FbCommand inCommand, DTO_ChefMateDataTableResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT 
                    DISTINCT O.ORDER_KEY,
	                ORDER_NUMBER AS ""ORDER NUMBER"",
                    TABLE_TAB_NAME AS ""TABLE/TAB NAME"",
                    CHIT_VALUE AS ""CHIT NUMBER"",
                    OTV.ARRIVAL_TIME,
	                OTV.BUMP_TIME AS ""BUMP TIME""
                FROM
	                ORDERS O INNER JOIN ORDERTIMESVIEW OTV
	                ON O.ORDER_KEY = OTV.ORDER_KEY
                    INNER JOIN MODELKEYSVIEW MKV
                    ON O.ORDER_KEY = MKV.ORDER_KEY
                WHERE
                    MKV.TERMINAL_KEY IS NULL
                AND
                    OTV.BUMP_TIME > DATEADD(-4 HOUR TO timestamp 'NOW')
                ORDER BY
	                ""BUMP TIME"" DESC";

            FbDataAdapter dataAdapter = new FbDataAdapter(inCommand);
            DataTable bumpedOrdersDataTable = new DataTable("BumpedOrdersDataSet");

            dataAdapter.Fill(bumpedOrdersDataTable);

            outResponse.ReturnValue = bumpedOrdersDataTable;
        }

        private void getBumpedOrderInformation(FbCommand inCommand, int inOrderKey, DTO_ChefMateDataTableResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT 
                    SC.GROUP_NAME AS ""SERVING COURSE"",
                    C.GROUP_NAME AS ""COURSE"",
                    MIV.ITEM_NAME AS ""ITEM""
                FROM
	                MODELKEYSVIEW MKV INNER JOIN MODELGROUPVIEW SC
                    ON MKV.SC_ORDERGROUP_KEY = SC.ORDERGROUP_KEY
                    AND SC.GROUP_TYPE = 'S'
                    INNER JOIN MODELGROUPVIEW C
                    ON MKV.C_ORDERGROUP_KEY = C.ORDERGROUP_KEY
                    AND C.GROUP_TYPE = 'C'
                    INNER JOIN MODELITEMVIEW MIV
                    ON MKV.ORDERITEM_KEY = MIV.ORDERITEM_KEY
                WHERE
                    MKV.ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            FbDataAdapter dataAdapter = new FbDataAdapter(inCommand);
            DataTable bumpedOrdersInfoDataTable = new DataTable("bumpedOrdersInfoDataTable");

            dataAdapter.Fill(bumpedOrdersInfoDataTable);

            outResponse.ReturnValue = bumpedOrdersInfoDataTable;
        }

        private void unbumpOrder(FbCommand inCommand, int inOrderKey, int inTerminalKey, DTO_ChefMateResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT
                    MODELKEYSVIEW.ORDERITEM_KEY
                FROM
                    ORDERS INNER JOIN MODELKEYSVIEW
	                ON ORDERS.ORDER_KEY = MODELKEYSVIEW.ORDER_KEY
                WHERE
                    MODELKEYSVIEW.TERMINAL_KEY IS NULL
                AND
                    ORDERS.ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read() && outResponse.Successful)
                {
                    DTO_ChefMateResponse response = UpdateOrderItemTerminalKey(reader.GetInt32(0), inTerminalKey);

                    outResponse.Successful = response.Successful;
                    outResponse.Message = response.Message;
                    outResponse.Description = response.Description;
                    outResponse.Response = response.Response;
                }
            }
        }

        private void getModelKeysFromOrderKey(FbCommand inCommand, int inOrderKey, int inTerminalKey, DTO_ChefMateDataTableResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT
                    ORDER_KEY,
                    C_ORDERGROUP_KEY AS COURSE_KEY,
                    SC_ORDERGROUP_KEY AS SERVING_COURSE_KEY,
                    ORDERITEM_KEY AS ITEM_KEY,
                    TERMINAL_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            if (inTerminalKey != 0)
            {
                inCommand.CommandText += " AND TERMINAL_KEY = @TERMINAL_KEY";
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            }

            FbDataAdapter dataAdapter = new FbDataAdapter(inCommand);
            DataTable modelKeysDataTable = new DataTable("ModelKeysDataSet");

            dataAdapter.Fill(modelKeysDataTable);

            outResponse.ReturnValue = modelKeysDataTable;
        }

        private void updateOrderTerminalName(FbCommand inCommand, int inOrderKey, string inTableTabName)
        {
            inCommand.CommandText =
                  @"UPDATE
                        ORDERS
                    SET
                        TABLE_TAB_NAME = @TABLE_TAB_NAME
                    WHERE
                        ORDER_POS_KEY = @ORDER_POS_KEY";
            inCommand.Parameters.AddWithValue("@TABLE_TAB_NAME", inTableTabName);
            inCommand.Parameters.AddWithValue("@ORDER_POS_KEY", inOrderKey);
            inCommand.ExecuteNonQuery();
        }

        private void getFullOrderFromOrderPosKey(FbCommand inCommand, int inOrderPosKey, int inTerminalKey,
            DTO_ChefMateFullOrderResponse outResponse)
        {
            inCommand.CommandText = "SELECT ORDER_KEY FROM ORDERS O WHERE O.ORDER_POS_KEY = @OrderPosKey";
            inCommand.Parameters.AddWithValue("@OrderPosKey", inOrderPosKey);

            object orderKey = inCommand.ExecuteScalar();

            if (orderKey != null)
            {
                getFullOrderFromOrderKey(inCommand, Convert.ToInt32(orderKey), inTerminalKey, outResponse);
            }
        }

        private void getFullOrderFromTableTabName(FbCommand inCommand, String inTableTabName, int inTerminalKey,
            DTO_ChefMateFullOrderResponse outResponse)
        {
            inCommand.CommandText = "SELECT FIRST 1 O.ORDER_KEY FROM ORDERS O WHERE O.TABLE_TAB_NAME = @TABLE_TAB_NAME ORDER BY O.ORDER_KEY DESC";
            inCommand.Parameters.AddWithValue("@TABLE_TAB_NAME", inTableTabName);

            object orderKey = inCommand.ExecuteScalar();

            if (orderKey != null)
            {
                getFullOrderFromOrderKey(inCommand, Convert.ToInt32(orderKey), inTerminalKey, outResponse);
            }
        }        

        #endregion
        #endregion

        #region DBOrderGroup
        #region PUBLIC
        public DTO_ChefMateIntegerResponse AddOrderGroup(int inOrderKey, DTO_ChefMateOrderGroup inOrderGroupStuct)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { addOrderGroup(inCommand, inOrderKey, inOrderGroupStuct, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateOrderGroupResponse GetOrderGroup(int inOrderGroupKey)
        {
            DTO_ChefMateOrderGroupResponse response = new DTO_ChefMateOrderGroupResponse();
            response.ReturnValue = new DTO_ChefMateOrderGroup();
            queryMethod query = delegate(FbCommand inCommand) { getOrderGroup(inCommand, inOrderGroupKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateFullOrderResponse GetFullOrderFromOrderGroupKey(int inOrderGroupKey, int inTerminalKey)
        {
            DTO_ChefMateFullOrderResponse response = new DTO_ChefMateFullOrderResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getFullOrderFromOrderGroupKey(inCommand, inOrderGroupKey, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateIntegerListResponse GetOrderGroupKeysFromOrderKey(int inOrderKey, ChefMateGroupType inGroupType, int inTerminalKey)
        {
            DTO_ChefMateIntegerListResponse response = new DTO_ChefMateIntegerListResponse();
            response.ReturnValue = new List<int>();
            queryMethod query = null;
            switch (inGroupType)
            {
                case ChefMateGroupType.COURSE:
                    query = delegate(FbCommand inCommand) { getCourseOrderGroupKeysFromOrderKey(inCommand, inOrderKey, inTerminalKey, response); };
                    break;
                case ChefMateGroupType.SERVING_COURSE:
                    query = delegate(FbCommand inCommand) { getServingCourseOrderGroupKeysFromOrderKey(inCommand, inOrderKey, inTerminalKey, response); };
                    break;
            }

            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerResponse GetOrderKey(int inOrderGroupKey)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { getOrderKey(inCommand, inOrderGroupKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse CallAwayOrderGroup(int inOrderGroupKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { callAwayOrderGroup(inCommand, inOrderGroupKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void addOrderGroup(FbCommand inCommand, int inOrderKey, DTO_ChefMateOrderGroup inOrderGroupStuct, DTO_ChefMateIntegerResponse outResponse)
        {
            int orderGroupKey = generateKey(inCommand, "ORDERGROUP");

            int groupKey = getGroupKey(inCommand, inOrderGroupStuct.GroupPosKey, inOrderGroupStuct.GroupType);

            inCommand.CommandText =
              @"INSERT INTO ORDERGROUP
                (
                    ORDERGROUP_KEY,
                    ORDER_KEY,
                    GROUP_KEY,
                    ORDERGROUP_CALLED_AWAY
                )
                VALUES
                (
                    @ORDERGROUP_KEY,
                    @ORDER_KEY,
                    @GROUP_KEY,
                    @ORDERGROUP_CALLED_AWAY
                )";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", orderGroupKey);
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);
            inCommand.Parameters.AddWithValue("@GROUP_KEY", groupKey);
            inCommand.Parameters.AddWithValue("@ORDERGROUP_CALLED_AWAY", inOrderGroupStuct.GroupCalledAway ? 'T' : 'F');
            inCommand.ExecuteNonQuery();

            outResponse.ReturnValue = orderGroupKey;
        }

        private void getOrderGroup(FbCommand inCommand, int inOrderGroupKey, DTO_ChefMateOrderGroupResponse outResponse)
        {
            DTO_ChefMateOrderGroup dbOrderGroupStruct;

            inCommand.CommandText =
              @"SELECT
                    GROUP_NAME,
                    GROUP_TYPE,
                    GROUP_POS_KEY,
                    GROUP_DISPLAY_ORDER,
                    ORDERGROUP_CALLED_AWAY
                FROM
                    MODELGROUPVIEW
                WHERE
                    ORDERGROUP_KEY = @ORDERGROUP_KEY";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", inOrderGroupKey);
            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                reader.Read();
                dbOrderGroupStruct = new DTO_ChefMateOrderGroup();

                switch (Convert.ToString(reader["GROUP_TYPE"]))
                {
                    case "C":
                        dbOrderGroupStruct.GroupType = ChefMateGroupType.COURSE;
                        break;
                    case "S":
                    default:
                        dbOrderGroupStruct.GroupType = ChefMateGroupType.SERVING_COURSE;
                        break;
                }
                dbOrderGroupStruct.GroupName = Convert.ToString(reader["GROUP_NAME"]);
                dbOrderGroupStruct.GroupPosKey = Convert.ToInt32(reader["GROUP_POS_KEY"]);
                dbOrderGroupStruct.GroupDisplayOrder = Convert.ToInt32(reader["GROUP_DISPLAY_ORDER"]);
                dbOrderGroupStruct.GroupCalledAway = Convert.ToString(reader["ORDERGROUP_CALLED_AWAY"]) == "T";
            }

            outResponse.ReturnValue = dbOrderGroupStruct;
        }

        private void getFullOrderFromOrderGroupKey(FbCommand inCommand, int inOrderGroupKey, int inTerminalKey, DTO_ChefMateFullOrderResponse outResponse)
        {
            IList<DTO_ChefMateFullOrder> dbFullOrderStructList = new List<DTO_ChefMateFullOrder>();

            inCommand.CommandText =
              @"SELECT
                    O.ORDER_KEY,
                    O.ORDER_NUMBER,
                    O.ORDER_POS_KEY,
                    O.CHIT_VALUE,
                    O.TABLE_TAB_NAME,
                    O.PARTY_NAME,
                    O.SERVER_NAME,
                    O.CUSTOMER_NAME,
                    O.ORDER_TYPE,
                    O.PATRON_COUNT,
                    O.ORDER_SALE_START_TIME,
                    O.ORDER_SALE_FINISH_TIME,
                    C.ORDERCOURSE_KEY,
                    C.COURSE_NAME,
                    C.COURSE_POS_KEY,
                    C.COURSE_DISPLAY_ORDER,
                    C.ORDERCOURSE_CALLED_AWAY,
                    C.ORDERCOURSE_BUMP_TIME,
                    S.ORDERSERVINGCOURSE_KEY,
                    S.SERVINGCOURSE_NAME,
                    S.SERVINGCOURSE_POS_KEY,
                    S.SERVINGCOURSE_DISPLAY_ORDER,
                    S.ORDERSERVINGCOURSE_CALLED_AWAY,
                    S.ORDERSERVINGCOURSE_BUMP_TIME,
                    I.ORDERITEM_KEY,
                    I.ITEM_POS_KEY,
                    I.ORDERITEM_POS_KEY,
                    I.ITEM_NAME,
                    I.STATUS,
                    I.ORDERITEM_NOTE,
                    T.ORDERITEMTIME_TIME
                FROM
                    ORDERS O INNER JOIN MODELCOURSEVIEW C 
                    ON O.ORDER_KEY = C.ORDER_KEY
                    INNER JOIN MODELSERVINGCOURSEVIEW S 
                    ON O.ORDER_KEY = S.ORDER_KEY
                    INNER JOIN MODELITEMVIEW I
                    ON C.ORDERCOURSE_KEY = I.C_ORDERGROUP_KEY
                    AND S.ORDERSERVINGCOURSE_KEY = I.SC_ORDERGROUP_KEY
                    INNER JOIN ORDERITEMTIME T
                    ON I.ORDERITEM_KEY = T.ORDERITEM_KEY
                    AND T.ORDERITEMTIME_TYPE = 'A'
                WHERE
                    (C.ORDERCOURSE_KEY = @ORDERGROUP_KEY OR S.ORDERSERVINGCOURSE_KEY = @ORDERGROUP_KEY) AND I.TERMINAL_KEY = @TERMINAL_KEY";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", inOrderGroupKey);
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    DTO_ChefMateFullOrder dbFullOrderStruct = new DTO_ChefMateFullOrder();

                    dbFullOrderStruct.OrderDbKey = Convert.ToInt32(reader["ORDER_KEY"]);
                    dbFullOrderStruct.OrderNumber = Convert.ToInt32(reader["ORDER_NUMBER"]);
                    dbFullOrderStruct.OrderPosDbKey = Convert.ToInt32(reader["ORDER_POS_KEY"]);
                    dbFullOrderStruct.PatronCount = Convert.ToInt32(reader["PATRON_COUNT"]);
                    dbFullOrderStruct.ChitValue = Convert.ToString(reader["CHIT_VALUE"]);
                    dbFullOrderStruct.TableTabName = Convert.ToString(reader["TABLE_TAB_NAME"]);
                    dbFullOrderStruct.PartyName = Convert.ToString(reader["PARTY_NAME"]);
                    dbFullOrderStruct.ServerName = Convert.ToString(reader["SERVER_NAME"]);
                    dbFullOrderStruct.CustomerName = Convert.ToString(reader["CUSTOMER_NAME"]);
                    dbFullOrderStruct.OrderType = Convert.ToString(reader["ORDER_TYPE"]);
                    dbFullOrderStruct.SaleStartTime = Convert.ToDateTime(reader["ORDER_SALE_START_TIME"]);
                    dbFullOrderStruct.SaleFinishTime = Convert.ToDateTime(reader["ORDER_SALE_FINISH_TIME"]);
                    dbFullOrderStruct.CourseKey = Convert.ToInt32(reader["ORDERCOURSE_KEY"]);
                    dbFullOrderStruct.CourseName = Convert.ToString(reader["COURSE_NAME"]);
                    dbFullOrderStruct.CoursePosKey = Convert.ToInt32(reader["COURSE_POS_KEY"]);
                    dbFullOrderStruct.CourseDisplayOrder = Convert.ToInt32(reader["COURSE_DISPLAY_ORDER"]);
                    dbFullOrderStruct.CourseCalledAway = Convert.ToString(reader["ORDERCOURSE_CALLED_AWAY"]) == "T";
                    dbFullOrderStruct.CourseBumpTime = Convert.ToDateTime(reader["ORDERCOURSE_BUMP_TIME"] == DBNull.Value ? DateTime.MinValue : reader["ORDERCOURSE_BUMP_TIME"]);
                    dbFullOrderStruct.ServingCourseKey = Convert.ToInt32(reader["ORDERSERVINGCOURSE_KEY"]);
                    dbFullOrderStruct.ServingCourseName = Convert.ToString(reader["SERVINGCOURSE_NAME"]);
                    dbFullOrderStruct.ServingCoursePosKey = Convert.ToInt32(reader["SERVINGCOURSE_POS_KEY"]);
                    dbFullOrderStruct.ServingCourseDisplayOrder = Convert.ToInt32(reader["SERVINGCOURSE_DISPLAY_ORDER"]);
                    dbFullOrderStruct.ServingCourseCalledAway = Convert.ToString(reader["ORDERSERVINGCOURSE_CALLED_AWAY"]) == "T";
                    dbFullOrderStruct.ServingCourseBumpTime = Convert.ToDateTime(reader["ORDERSERVINGCOURSE_BUMP_TIME"] == DBNull.Value ? DateTime.MinValue : reader["ORDERSERVINGCOURSE_BUMP_TIME"]);
                    dbFullOrderStruct.OrderItemKey = Convert.ToInt32(reader["ORDERITEM_KEY"]);
                    dbFullOrderStruct.ItemPosDbKey = Convert.ToInt32(reader["ITEM_POS_KEY"]);
                    dbFullOrderStruct.OrderItemPosDbKey = Convert.ToInt32(reader["ORDERITEM_POS_KEY"]);
                    dbFullOrderStruct.ItemName = Convert.ToString(reader["ITEM_NAME"]);
                    dbFullOrderStruct.OrderItemStatus = Convert.ToString(reader["STATUS"]);
                    dbFullOrderStruct.OrderItemNote = Convert.ToString(reader["ORDERITEM_NOTE"]);
                    dbFullOrderStruct.OrderItemArrivalTime = Convert.ToDateTime(reader["ORDERITEMTIME_TIME"] == DBNull.Value ? DateTime.MinValue : reader["ORDERITEMTIME_TIME"]);
                    dbFullOrderStruct.OrderItemOptionKey = -1;
                    dbFullOrderStruct.OptionName = "";
                    dbFullOrderStruct.OrderItemOptionIsPlus = false;
                    dbFullOrderStruct.OrderItemSideKey = -1;
                    dbFullOrderStruct.OrderItemSidePosKey = -1;
                    dbFullOrderStruct.SideName = "";
                    dbFullOrderStruct.OrderItemSideIsCanceled = false;

                    FbCommand optionsSidesCommand = new FbCommand("", inCommand.Connection, inCommand.Transaction);
                    bool itemAddedToOutput = false;

                    itemAddedToOutput |= loadItemOptions(optionsSidesCommand, dbFullOrderStruct, dbFullOrderStructList);
                    itemAddedToOutput |= loadItemSides(optionsSidesCommand, dbFullOrderStruct, dbFullOrderStructList);

                    if (!itemAddedToOutput)
                    {
                        dbFullOrderStructList.Add(dbFullOrderStruct);
                    }
                }
            }

            outResponse.ReturnValue = dbFullOrderStructList;
        }

        private void getCourseOrderGroupKeysFromOrderKey(FbCommand inCommand, int inOrderKey, int inTerminalKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> orderGroupKeys = new List<int>();

            inCommand.CommandText =
              @"SELECT
                    C_ORDERGROUP_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            if (inTerminalKey != 0)
            {
                inCommand.CommandText += " AND TERMINAL_KEY = @TERMINAL_KEY";
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            }

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    orderGroupKeys.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = orderGroupKeys;
        }

        private void getServingCourseOrderGroupKeysFromOrderKey(FbCommand inCommand, int inOrderKey, int inTerminalKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> orderGroupKeys = new List<int>();

            inCommand.CommandText =
              @"SELECT
                    SC_ORDERGROUP_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            if (inTerminalKey != 0)
            {
                inCommand.CommandText += " AND TERMINAL_KEY = @TERMINAL_KEY";
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            }

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    orderGroupKeys.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = orderGroupKeys;
        }

        private void getOrderKey(FbCommand inCommand, int inOrderGroupKey, DTO_ChefMateIntegerResponse outResponse)
        {
            object commandResult;
            int orderKey;

            inCommand.CommandText =
              @"SELECT
                    ORDER_KEY
                FROM
                    ORDERGROUP
                WHERE
                    ORDERGROUP_KEY = @ORDERGROUP_KEY";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", inOrderGroupKey);

            commandResult = inCommand.ExecuteScalar();
            if (commandResult == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                throw new ScalerFailureException(string.Format("Scaler returned in null {0}. OrderGroupKey: {1}.", methodBase.Name, inOrderGroupKey));
            }
            else
            {
                orderKey = Convert.ToInt32(commandResult);
                //Do we need to check if the information in the Item table is correct?
            }

            outResponse.ReturnValue = orderKey;
        }

        private void callAwayOrderGroup(FbCommand inCommand, int inOrderGroupKey)
        {
            inCommand.CommandText =
              @"UPDATE 
                    ORDERGROUP
                SET
                    ORDERGROUP_CALLED_AWAY = 'T'
                WHERE
                    ORDERGROUP_KEY = @ORDERGROUP_KEY";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", inOrderGroupKey);
            inCommand.ExecuteNonQuery();
        }

        private int getOrderGroupKey(FbCommand inCommand, int inOrderPosDbKey, ChefMateGroupType inGroupType, int inGroupPosDbKey)
        {
            object commandResult = null;
            int orderGroupKey = 0;

            inCommand.CommandText =
              @"SELECT
                    ORDERGROUP_KEY
                FROM
                    ORDERGROUP OG INNER JOIN GROUPS G
                    ON OG.GROUP_KEY = G.GROUP_KEY
                    INNER JOIN ORDERS O
                    ON OG.ORDER_KEY = O.ORDER_KEY
                WHERE
                    O.ORDER_POS_KEY = @ORDER_POS_KEY
                AND
                    G.GROUP_TYPE = @GROUP_TYPE
                AND
                    G.GROUP_POS_KEY = @GROUP_POS_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_POS_KEY", inOrderPosDbKey);
            if (inGroupType == ChefMateGroupType.COURSE)
            {
                inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'C');
            }
            else if (inGroupType == ChefMateGroupType.SERVING_COURSE)
            {
                inCommand.Parameters.AddWithValue("@GROUP_TYPE", 'S');
            }
            inCommand.Parameters.AddWithValue("@GROUP_POS_KEY", inGroupPosDbKey);

            if ((commandResult = inCommand.ExecuteScalar()) == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                throw new ScalerFailureException(string.Format("Scaler returned in null {0}. OrderPosDbKey: {1}, GroupType: {2}, GroupPosDbKey: {3}.", methodBase.Name, inOrderPosDbKey, inGroupType, inGroupPosDbKey));
            }
            orderGroupKey = Convert.ToInt32(commandResult);

            return orderGroupKey;
        }
        #endregion
        #endregion

        #region DBOrderItem
        #region PUBLIC
        public DTO_ChefMateIntegerResponse AddOrderItem(int inServingCourseOrderGroupKey, int inCourseOrderGroupKey, int inTerminalKey, DTO_ChefMateOrderItem inOrderItemStruct)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { addOrderItem(inCommand, inServingCourseOrderGroupKey, inCourseOrderGroupKey, inTerminalKey, inOrderItemStruct, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse AddOrderItemTerminal(int inOrderItemKey, int inTerminalKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addOrderItemTerminal(inCommand, inOrderItemKey, inTerminalKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateOrderItemResponse GetOrderItem(int inOrderItemKey)
        {
            DTO_ChefMateOrderItemResponse response = new DTO_ChefMateOrderItemResponse();
            response.ReturnValue = new DTO_ChefMateOrderItem();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItem(inCommand, inOrderItemKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateFullOrderResponse GetFullOrderFromOrderItemKey(int inOrderItemKey, int inTerminalKey)
        {
            DTO_ChefMateFullOrderResponse response = new DTO_ChefMateFullOrderResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getFullOrderFromOrderItemKey(inCommand, inOrderItemKey, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateResponse UpdateOrderItemStatus(int inOrderItemKey, String inStatus)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { updateOrderItemStatus(inCommand, inOrderItemKey, inStatus); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse UpdateOrderItemTerminalKey(int inOrderItemKey, int inTerminalKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { updateOrderItemTerminalKey(inCommand, inOrderItemKey, inTerminalKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse UpdateOrderItemPosDbKey(int inOrderItemKey, int inOrderItemPosDbKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { updateOrderItemPosDbKey(inCommand, inOrderItemKey, inOrderItemPosDbKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerListResponse GetOrderItemKeysFromOrderKey(int inOrderKey, int inTerminalKey)
        {
            DTO_ChefMateIntegerListResponse response = new DTO_ChefMateIntegerListResponse();
            response.ReturnValue = new List<int>();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemKeysFromOrderKey(inCommand, inOrderKey, inTerminalKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerListResponse GetOrderItemKeysFromOrderGroupKey(int inOrderGroupKey, int inTerminalKey)
        {
            DTO_ChefMateIntegerListResponse response = new DTO_ChefMateIntegerListResponse();
            response.ReturnValue = new List<int>();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemKeysFromOrderGroupKey(inCommand, inOrderGroupKey, inTerminalKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateModelKeysResponse GetModelKeys(int inOrderItemkey)
        {
            DTO_ChefMateModelKeysResponse response = new DTO_ChefMateModelKeysResponse();
            response.OrderKey = 0;
            response.CourseKey = 0;
            response.ServingCourseKey = 0;
            queryMethod query = delegate(FbCommand inCommand) { getModelKeys(inCommand, inOrderItemkey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateDataTableResponse GetBumpedOrderItems(int inOrderKey)
        {
            DTO_ChefMateDataTableResponse response = new DTO_ChefMateDataTableResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getBumpedOrderItems(inCommand, inOrderKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void addOrderItem(FbCommand inCommand,
                                  int inServingCourseOrderGroupKey,
                                  int inCourseOrderGroupKey,
                                  int inTerminalKey,
                                  DTO_ChefMateOrderItem inOrderItemStruct,
                                  DTO_ChefMateIntegerResponse outResponse)
        {
            int orderItemKey = generateKey(inCommand, "ORDERITEM");

            int itemKey = getItemKey(inCommand, inOrderItemStruct.ItemPosDbKey, new DTO_ChefMateIntegerResponse());
            string itemStatus = inOrderItemStruct.OrderItemStatus;
            string orderItemNote = inOrderItemStruct.OrderItemNote;
            if (orderItemNote.Count() > 80)
            {
                orderItemNote = orderItemNote.Substring(0, 80);
            }

            inCommand.CommandText =
              @"INSERT INTO ORDERITEM
                (
                    ORDERITEM_KEY,
                    ITEM_KEY,
                    SC_ORDERGROUP_KEY,
                    C_ORDERGROUP_KEY,
                    TERMINAL_KEY,
                    ORDERITEM_POS_KEY,
                    STATUS,
                    ORDERITEM_NOTE
                )
                VALUES
                (
                    @ORDERITEM_KEY,
                    @ITEM_KEY,
                    @SC_ORDERGROUP_KEY,
                    @C_ORDERGROUP_KEY,
                    @TERMINAL_KEY,
                    @ORDERITEM_POS_KEY,
                    @STATUS,
                    @ORDERITEM_NOTE
                )";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", orderItemKey);
            inCommand.Parameters.AddWithValue("@ITEM_KEY", itemKey);
            inCommand.Parameters.AddWithValue("@SC_ORDERGROUP_KEY", inServingCourseOrderGroupKey);
            inCommand.Parameters.AddWithValue("@C_ORDERGROUP_KEY", inCourseOrderGroupKey);
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            inCommand.Parameters.AddWithValue("@ORDERITEM_POS_KEY", inOrderItemStruct.OrderItemPosDbKey);
            inCommand.Parameters.AddWithValue("@STATUS", itemStatus);
            inCommand.Parameters.AddWithValue("@ORDERITEM_NOTE", orderItemNote);
            inCommand.ExecuteNonQuery();

            outResponse.ReturnValue = orderItemKey;
        }

        private void addOrderItemTerminal(FbCommand inCommand, int inOrderItemKey, int inTerminalKey)
        {
            int orderItemTerminalKey = generateKey(inCommand, "ORDERITEMTERMINAL");

            inCommand.CommandText =
              @"INSERT INTO ORDERITEMTERMINAL
                (
                    ORDERITEMTERMINAL_KEY,
                    ORDERITEM_KEY,
                    TERMINAL_KEY
                )
                VALUES
                (
                    @ORDERITEMTERMINAL_KEY,
                    @ORDERITEM_KEY,
                    @TERMINAL_KEY
                )";
            inCommand.Parameters.AddWithValue("@ORDERITEMTERMINAL_KEY", orderItemTerminalKey);
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            inCommand.ExecuteNonQuery();
        }

        private void getOrderItem(FbCommand inCommand, int inOrderItemKey, DTO_ChefMateOrderItemResponse outResponse)
        {
            DTO_ChefMateOrderItem dbOrderItemStruct;

            inCommand.CommandText =
              @"SELECT
                    ITEM_POS_KEY,
                    ORDERITEM_POS_KEY,
                    ITEM_NAME,
                    STATUS,
                    ORDERITEM_NOTE
                FROM
                    MODELITEMVIEW
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                reader.Read();

                dbOrderItemStruct = new DTO_ChefMateOrderItem();

                dbOrderItemStruct.OrderItemKey = inOrderItemKey;
                dbOrderItemStruct.ItemPosDbKey = Convert.ToInt32(reader["ITEM_POS_KEY"]);
                dbOrderItemStruct.OrderItemPosDbKey = Convert.ToInt32(reader["ORDERITEM_POS_KEY"]);
                dbOrderItemStruct.ItemName = Convert.ToString(reader["ITEM_NAME"]);
                dbOrderItemStruct.OrderItemStatus = Convert.ToString(reader["STATUS"]);
                dbOrderItemStruct.OrderItemNote = Convert.ToString(reader["ORDERITEM_NOTE"]);
            }

            outResponse.ReturnValue = dbOrderItemStruct;
        }

        private void getFullOrderFromOrderItemKey(FbCommand inCommand, int inOrderItemKey, int inTerminalKey, DTO_ChefMateFullOrderResponse outResponse)
        {
            IList<DTO_ChefMateFullOrder> dbFullOrderStructList = new List<DTO_ChefMateFullOrder>();

            inCommand.CommandText =
              @"SELECT
                    O.ORDER_KEY,
                    O.ORDER_NUMBER,
                    O.ORDER_POS_KEY,
                    O.CHIT_VALUE,
                    O.TABLE_TAB_NAME,
                    O.PARTY_NAME,
                    O.SERVER_NAME,
                    O.CUSTOMER_NAME,
                    O.ORDER_TYPE,
                    O.PATRON_COUNT,
                    O.ORDER_SALE_START_TIME,
                    O.ORDER_SALE_FINISH_TIME,
                    O.DELIVERY_TIME,
                    C.ORDERCOURSE_KEY,
                    C.COURSE_NAME,
                    C.COURSE_POS_KEY,
                    C.COURSE_DISPLAY_ORDER,
                    C.ORDERCOURSE_CALLED_AWAY,
                    C.ORDERCOURSE_BUMP_TIME,
                    S.ORDERSERVINGCOURSE_KEY,
                    S.SERVINGCOURSE_NAME,
                    S.SERVINGCOURSE_POS_KEY,
                    S.SERVINGCOURSE_DISPLAY_ORDER,
                    S.ORDERSERVINGCOURSE_CALLED_AWAY,
                    S.ORDERSERVINGCOURSE_BUMP_TIME,
                    I.ORDERITEM_KEY,
                    I.ITEM_POS_KEY,
                    I.ORDERITEM_POS_KEY,
                    I.ITEM_NAME,
                    I.STATUS,
                    I.ORDERITEM_NOTE,
                    T.ORDERITEMTIME_TIME
                FROM
                    ORDERS O INNER JOIN MODELCOURSEVIEW C 
                    ON O.ORDER_KEY = C.ORDER_KEY
                    INNER JOIN MODELSERVINGCOURSEVIEW S 
                    ON O.ORDER_KEY = S.ORDER_KEY
                    INNER JOIN MODELITEMVIEW I
                    ON C.ORDERCOURSE_KEY = I.C_ORDERGROUP_KEY
                    AND S.ORDERSERVINGCOURSE_KEY = I.SC_ORDERGROUP_KEY
                    INNER JOIN ORDERITEMTIME T
                    ON I.ORDERITEM_KEY = T.ORDERITEM_KEY
                    AND T.ORDERITEMTIME_TYPE = 'A'
                WHERE
                    I.ORDERITEM_KEY = @ORDERITEM_KEY AND I.TERMINAL_KEY = @TERMINAL_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    DTO_ChefMateFullOrder dbFullOrderStruct = new DTO_ChefMateFullOrder();

                    dbFullOrderStruct.OrderDbKey = Convert.ToInt32(reader["ORDER_KEY"]);
                    dbFullOrderStruct.OrderNumber = Convert.ToInt32(reader["ORDER_NUMBER"]);
                    dbFullOrderStruct.OrderPosDbKey = Convert.ToInt32(reader["ORDER_POS_KEY"]);
                    dbFullOrderStruct.PatronCount = Convert.ToInt32(reader["PATRON_COUNT"]);
                    dbFullOrderStruct.ChitValue = Convert.ToString(reader["CHIT_VALUE"]);
                    dbFullOrderStruct.TableTabName = Convert.ToString(reader["TABLE_TAB_NAME"]);
                    dbFullOrderStruct.PartyName = Convert.ToString(reader["PARTY_NAME"]);
                    dbFullOrderStruct.ServerName = Convert.ToString(reader["SERVER_NAME"]);
                    dbFullOrderStruct.CustomerName = Convert.ToString(reader["CUSTOMER_NAME"]);
                    dbFullOrderStruct.OrderType = Convert.ToString(reader["ORDER_TYPE"]);
                    dbFullOrderStruct.SaleStartTime = Convert.ToDateTime(reader["ORDER_SALE_START_TIME"]);
                    dbFullOrderStruct.SaleFinishTime = Convert.ToDateTime(reader["ORDER_SALE_FINISH_TIME"]);
                    dbFullOrderStruct.CourseKey = Convert.ToInt32(reader["ORDERCOURSE_KEY"]);
                    dbFullOrderStruct.CourseName = Convert.ToString(reader["COURSE_NAME"]);
                    dbFullOrderStruct.CoursePosKey = Convert.ToInt32(reader["COURSE_POS_KEY"]);
                    dbFullOrderStruct.CourseDisplayOrder = Convert.ToInt32(reader["COURSE_DISPLAY_ORDER"]);
                    dbFullOrderStruct.CourseCalledAway = Convert.ToString(reader["ORDERCOURSE_CALLED_AWAY"]) == "T";
                    dbFullOrderStruct.CourseBumpTime = Convert.ToDateTime(reader["ORDERCOURSE_BUMP_TIME"] == DBNull.Value ? DateTime.MinValue : reader["ORDERCOURSE_BUMP_TIME"]);
                    dbFullOrderStruct.ServingCourseKey = Convert.ToInt32(reader["ORDERSERVINGCOURSE_KEY"]);
                    dbFullOrderStruct.ServingCourseName = Convert.ToString(reader["SERVINGCOURSE_NAME"]);
                    dbFullOrderStruct.ServingCoursePosKey = Convert.ToInt32(reader["SERVINGCOURSE_POS_KEY"]);
                    dbFullOrderStruct.ServingCourseDisplayOrder = Convert.ToInt32(reader["SERVINGCOURSE_DISPLAY_ORDER"]);
                    dbFullOrderStruct.ServingCourseCalledAway = Convert.ToString(reader["ORDERSERVINGCOURSE_CALLED_AWAY"]) == "T";
                    dbFullOrderStruct.ServingCourseBumpTime = Convert.ToDateTime(reader["ORDERSERVINGCOURSE_BUMP_TIME"] == DBNull.Value ? DateTime.MinValue : reader["ORDERSERVINGCOURSE_BUMP_TIME"]);
                    dbFullOrderStruct.OrderItemKey = Convert.ToInt32(reader["ORDERITEM_KEY"]);
                    dbFullOrderStruct.ItemPosDbKey = Convert.ToInt32(reader["ITEM_POS_KEY"]);
                    dbFullOrderStruct.OrderItemPosDbKey = Convert.ToInt32(reader["ORDERITEM_POS_KEY"]);
                    dbFullOrderStruct.ItemName = Convert.ToString(reader["ITEM_NAME"]);
                    dbFullOrderStruct.OrderItemStatus = Convert.ToString(reader["STATUS"]);
                    dbFullOrderStruct.OrderItemNote = Convert.ToString(reader["ORDERITEM_NOTE"]);
                    dbFullOrderStruct.OrderItemArrivalTime = Convert.ToDateTime(reader["ORDERITEMTIME_TIME"] == DBNull.Value ? DateTime.MinValue : reader["ORDERITEMTIME_TIME"]);
                    dbFullOrderStruct.OrderItemOptionKey = -1;
                    dbFullOrderStruct.OptionName = "";
                    dbFullOrderStruct.OrderItemOptionIsPlus = false;
                    dbFullOrderStruct.OrderItemSideKey = -1;
                    dbFullOrderStruct.OrderItemSidePosKey = -1;
                    dbFullOrderStruct.SideName = "";
                    dbFullOrderStruct.OrderItemSideIsCanceled = false;
                    dbFullOrderStruct.DeliveryTime = Convert.ToDateTime(reader["DELIVERY_TIME"] == DBNull.Value ? DateTime.MinValue : reader["DELIVERY_TIME"]);

                    FbCommand optionsSidesCommand = new FbCommand("", inCommand.Connection, inCommand.Transaction);
                    bool itemAddedToOutput = false;

                    itemAddedToOutput |= loadItemOptions(optionsSidesCommand, dbFullOrderStruct, dbFullOrderStructList);
                    itemAddedToOutput |= loadItemSides(optionsSidesCommand, dbFullOrderStruct, dbFullOrderStructList);

                    if (!itemAddedToOutput)
                    {
                        dbFullOrderStructList.Add(dbFullOrderStruct);
                    }
                }
            }

            outResponse.ReturnValue = dbFullOrderStructList;
        }

        private void updateOrderItemStatus(FbCommand inCommand, int inOrderItemKey, String inStatus)
        {
            DTO_ChefMateStringResponse itemStatusResponse = new DTO_ChefMateStringResponse();
            getOrderItemStatus(inCommand, inOrderItemKey, itemStatusResponse);
            if (itemStatusResponse.ReturnValue != "CNCL")
            {
                inCommand.CommandText =
                  @"UPDATE
                        ORDERITEM
                    SET
                        STATUS = @STATUS
                    WHERE
                        ORDERITEM_KEY = @ORDERITEM_KEY";
                inCommand.Parameters.AddWithValue("@STATUS", inStatus);
                inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
                inCommand.ExecuteNonQuery();
            }
        }

        private void getOrderItemStatus(FbCommand inCommand, int inOrderItemKey, DTO_ChefMateStringResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT 
                    STATUS
                FROM 
                    ORDERITEM
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);

            outResponse.ReturnValue = Convert.ToString(inCommand.ExecuteScalar());
        }

        private void updateOrderItemTerminalKey(FbCommand inCommand, int inOrderItemKey, int inTerminalKey)
        {
            inCommand.CommandText =
              @"UPDATE
                    ORDERITEM
                SET
                    TERMINAL_KEY = @TERMINAL_KEY
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            if (inTerminalKey != 0)
            {
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            }
            else
            {
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", DBNull.Value);
            }
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);

            inCommand.ExecuteNonQuery();
        }

        private void updateOrderItemPosDbKey(FbCommand inCommand, int inOrderItemKey, int inOrderItemPosDbKey)
        {
            inCommand.CommandText =
              @"UPDATE
                    ORDERITEM
                SET
                    ORDERITEM_POS_KEY = @ORDERITEM_POS_KEY
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_POS_KEY", inOrderItemPosDbKey);
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);

            inCommand.ExecuteNonQuery();
        }

        private void getOrderItemKeysFromOrderKey(FbCommand inCommand, int inOrderKey, int inTerminalKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> itemKeyList = new List<int>();
            inCommand.CommandText =
              @"SELECT
                    ORDERITEM_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            if (inTerminalKey != 0)
            {
                inCommand.CommandText += " AND TERMINAL_KEY = @TERMINAL_KEY";
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            }

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    itemKeyList.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = itemKeyList;
        }

        private void getOrderItemKeysFromOrderGroupKey(FbCommand inCommand, int inOrderGroupKey, int inTerminalKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> orderItemKeys = new List<int>();

            inCommand.CommandText =
              @"SELECT
                    ORDERITEM_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    C_ORDERGROUP_KEY = @ORDERGROUP_KEY
                OR
                    SC_ORDERGROUP_KEY = @ORDERGROUP_KEY";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", inOrderGroupKey);

            if (inTerminalKey != 0)
            {
                inCommand.CommandText += " AND TERMINAL_KEY = @TERMINAL_KEY";
                inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            }

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    orderItemKeys.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = orderItemKeys;
        }

        private void getModelKeys(FbCommand inCommand, int inOrderItemkey, DTO_ChefMateModelKeysResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT
                    ORDER_KEY,
                    C_ORDERGROUP_KEY,
                    SC_ORDERGROUP_KEY
                FROM
                    MODELKEYSVIEW
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemkey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                if (reader.Read())
                {
                    outResponse.OrderKey = Convert.ToInt32(reader["ORDER_KEY"]);
                    outResponse.CourseKey = Convert.ToInt32(reader["C_ORDERGROUP_KEY"]);
                    outResponse.ServingCourseKey = Convert.ToInt32(reader["SC_ORDERGROUP_KEY"]);
                }
                else
                {
                    System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                    throw new ScalerFailureException(string.Format("Scaler returned in null {0}. OrderItemkey: {1}.", methodBase.Name, inOrderItemkey));
                }
            }
        }

        private void getBumpedOrderItems(FbCommand inCommand, int inOrderKey, DTO_ChefMateDataTableResponse outResponse)
        {
            inCommand.CommandText =
          @"SELECT 
                MIV.ORDERITEM_KEY,
                MIV.ITEM_NAME AS ""ITEM""
            FROM
	            MODELITEMVIEWW MIV INNER JOIN MODELKEYSVIEW MKV
	            ON MIV.ORDERITEM_KEY = MKV.ORDERITEM_KEY
            WHERE
                MKV.ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            FbDataAdapter dataAdapter = new FbDataAdapter(inCommand);
            DataTable bumpedOrderItemsDataTable = new DataTable("BumpedOrderItemsDataSet");

            dataAdapter.Fill(bumpedOrderItemsDataTable);

            outResponse.ReturnValue = bumpedOrderItemsDataTable;
        }
        #endregion
        #endregion

        #region DBOrderItemOption
        #region PUBLIC
        public DTO_ChefMateIntegerResponse AddOrderItemOption(int inOrderItemKey, string inOptionName, bool isPlus)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { addOrderItemOption(inCommand, inOrderItemKey, inOptionName, isPlus, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateOrderItemOptionResponse GetOrderItemOption(int inOrderItemOptionKey)
        {
            DTO_ChefMateOrderItemOptionResponse response = new DTO_ChefMateOrderItemOptionResponse();
            response.ReturnValue = new DTO_ChefMateOrderItemOption();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemOption(inCommand, inOrderItemOptionKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerListResponse GetOrderItemOptionKeys(int inOrderItemKey)
        {
            DTO_ChefMateIntegerListResponse response = new DTO_ChefMateIntegerListResponse();
            response.ReturnValue = new List<int>();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemOptionKeys(inCommand, inOrderItemKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void addOrderItemOption(FbCommand inCommand, int inOrderItemKey, string inOptionName, bool isPlus, DTO_ChefMateIntegerResponse outResponse)
        {
            int orderItemOptionKey = generateKey(inCommand, "ORDERITEMOPTION");

            int optionDbKey = getOptionKey(inCommand, inOptionName);
            char dbIsPlus = isPlus ? 'T' : 'F';

            inCommand.CommandText =
              @"INSERT INTO ORDERITEMOPTION
                (
                    ORDERITEMOPTION_KEY,
                    ORDERITEM_KEY,
                    OPTION_KEY,
                    IS_PLUS
                )
                VALUES
                (
                    @ORDERITEMOPTION_KEY,
                    @ORDERITEM_KEY,
                    @OPTION_KEY,
                    @IS_PLUS
                )";
            inCommand.Parameters.AddWithValue("@ORDERITEMOPTION_KEY", orderItemOptionKey);
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
            inCommand.Parameters.AddWithValue("@OPTION_KEY", optionDbKey);
            inCommand.Parameters.AddWithValue("@IS_PLUS", dbIsPlus);
            inCommand.ExecuteNonQuery();

            outResponse.ReturnValue = orderItemOptionKey;
        }

        private void getOrderItemOption(FbCommand inCommand, int inOrderItemOptionKey, DTO_ChefMateOrderItemOptionResponse outResponse)
        {
            DTO_ChefMateOrderItemOption dbOrderItemOptionStruct;

            inCommand.CommandText =
              @"SELECT
                    OPTION_NAME,
                    IS_PLUS
                FROM
                    MODELOPTIONVIEW
                WHERE
                    ORDERITEMOPTION_KEY = @ORDERITEMOPTION_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEMOPTION_KEY", inOrderItemOptionKey);
            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                reader.Read();

                dbOrderItemOptionStruct = new DTO_ChefMateOrderItemOption();

                dbOrderItemOptionStruct.OrderItemOptionKey = inOrderItemOptionKey;
                dbOrderItemOptionStruct.OptionName = Convert.ToString(reader["OPTION_NAME"]);
                dbOrderItemOptionStruct.IsPlus = Convert.ToString(reader["IS_PLUS"]) == "T";
            }

            outResponse.ReturnValue = dbOrderItemOptionStruct;
        }

        private void getOrderItemOptionKeys(FbCommand inCommand, int inOrderItemKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> orderItemOptionKeys = new List<int>();

            inCommand.CommandText =
              @"SELECT
                    ORDERITEMOPTION_KEY
                FROM
                    ORDERITEMOPTION
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    orderItemOptionKeys.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = orderItemOptionKeys;
        }
        #endregion
        #endregion

        #region DBOrderItemSide
        #region PUBLIC
        public DTO_ChefMateIntegerResponse AddOrderItemSide(int inOrderItemKey, DTO_ChefMateOrderItemSide inOrderItemSideStruct)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { addOrderItemSide(inCommand, inOrderItemKey, inOrderItemSideStruct, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerResponse GetOrderItemSideKey(int inOrderPosDbKey, ChefMateGroupType inGroupType, int inGroupPosDbKey, int inItemPosDbKey, int inSidePosDbKey)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemSideKey(inCommand, inOrderPosDbKey, inGroupType, inGroupPosDbKey, inItemPosDbKey, inSidePosDbKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse CancelOrderItemSide(int inOrderItemSideKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { cancelOrderItemSide(inCommand, inOrderItemSideKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateOrderItemSideResponse GetOrderItemSide(int inOrderItemSideKey)
        {
            DTO_ChefMateOrderItemSideResponse response = new DTO_ChefMateOrderItemSideResponse();
            response.ReturnValue = new DTO_ChefMateOrderItemSide();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemSide(inCommand, inOrderItemSideKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateIntegerListResponse GetOrderItemSideKeys(int inOrderItemKey)
        {
            DTO_ChefMateIntegerListResponse response = new DTO_ChefMateIntegerListResponse();
            response.ReturnValue = new List<int>();
            queryMethod query = delegate(FbCommand inCommand) { getOrderItemSideKeys(inCommand, inOrderItemKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void addOrderItemSide(FbCommand inCommand, int inOrderItemKey, DTO_ChefMateOrderItemSide inOrderItemSideStruct, DTO_ChefMateIntegerResponse outResponse)
        {
            int orderItemSideKey = generateKey(inCommand, "ORDERITEMSIDE");

            int sideDbKey = getSideKey(inCommand, inOrderItemSideStruct.SideName);

            inCommand.CommandText =
              @"INSERT INTO ORDERITEMSIDE
                (
                    ORDERITEMSIDE_KEY,
                    ORDERITEM_KEY,
                    SIDE_KEY,
                    ORDERITEMSIDE_POS_KEY,
                    IS_CANCELED
                )
                VALUES
                (
                    @ORDERITEMSIDE_KEY,
                    @ORDERITEM_KEY,
                    @SIDE_KEY,
                    @ORDERITEMSIDE_POS_KEY,
                    'F'
                )";
            inCommand.Parameters.AddWithValue("@ORDERITEMSIDE_KEY", orderItemSideKey);
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);
            inCommand.Parameters.AddWithValue("@SIDE_KEY", sideDbKey);
            inCommand.Parameters.AddWithValue("@ORDERITEMSIDE_POS_KEY", inOrderItemSideStruct.OrderItemSidePosKey);
            inCommand.ExecuteNonQuery();

            outResponse.ReturnValue = orderItemSideKey;
        }

        private void getOrderItemSideKey(FbCommand inCommand,
                                         int inOrderPosDbKey,
                                         ChefMateGroupType inGroupType,
                                         int inGroupPosDbKey,
                                         int inItemPosDbKey,
                                         int inSidePosDbKey,
                                         DTO_ChefMateIntegerResponse outResponse)
        {
            object commandResult = null;
            int sideKey = 0;
            int orderGroupKey = getOrderGroupKey(inCommand, inOrderPosDbKey, inGroupType, inGroupPosDbKey);

            inCommand.CommandText =
              @"SELECT
                    OIS.ORDERITEMSIDE_KEY
                FROM
                    ITEM I INNER JOIN ORDERITEM OI
                    ON I.ITEM_KEY = OI.ITEM_KEY
                    INNER JOIN ORDERITEMSIDE OIS
                    ON OI.ORDERITEM_KEY = OIS.ORDERITEM_KEY
                    INNER JOIN SIDE S
                    ON S.SIDE_KEY = OIS.SIDE_KEY
                WHERE
                    OI.ORDERGROUP_KEY = @ORDERGROUP_KEY
                AND
                    I.ITEM_POS_KEY = @ITEM_POS_KEY
                AND
                    S.SIDE_POS_KEY = @SIDE_POS_KEY";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", orderGroupKey);
            inCommand.Parameters.AddWithValue("@ITEM_POS_KEY", inItemPosDbKey);
            inCommand.Parameters.AddWithValue("@SIDE_POS_KEY", inSidePosDbKey);
            if ((commandResult = inCommand.ExecuteScalar()) == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                string exceptionMessage = string.Format("Scaler returned in null {0}. OrderPosDbKey: {1}, GroupType: {2}, GroupPosDbKey: {3}, ItemPosDbKey: {4}, SidePosDbKey: {5}.",
                                                        methodBase.Name,
                                                        inOrderPosDbKey,
                                                        inGroupType,
                                                        inGroupPosDbKey,
                                                        inItemPosDbKey,
                                                        inSidePosDbKey);
                throw new ScalerFailureException(exceptionMessage);
            }
            sideKey = Convert.ToInt32(commandResult);

            outResponse.ReturnValue = sideKey;
        }

        private void cancelOrderItemSide(FbCommand inCommand, int inOrderItemSideKey)
        {
            inCommand.CommandText =
              @"UPDATE
                    ORDERITEMSIDE
                SET
                    IS_CANCELED = 'T'
                WHERE
                    ORDERITEMSIDE_KEY = @ORDERITEMSIDE_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEMSIDE_KEY", inOrderItemSideKey);
            inCommand.ExecuteNonQuery();
        }

        private void getOrderItemSide(FbCommand inCommand, int inOrderItemSideKey, DTO_ChefMateOrderItemSideResponse outResponse)
        {
            DTO_ChefMateOrderItemSide orderItemSideStruct = new DTO_ChefMateOrderItemSide();

            inCommand.CommandText =
              @"SELECT
                    ORDERITEMSIDE_POS_KEY,
                    SIDE_NAME,
                    IS_CANCELED
                FROM
                    MODELSIDEVIEW
                WHERE
                    ORDERITEMSIDE_KEY = @ORDERITEMSIDE_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEMSIDE_KEY", inOrderItemSideKey);
            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                if (reader.HasRows)
                {
                    reader.Read();
                    orderItemSideStruct.OrderItemSideKey = inOrderItemSideKey;
                    orderItemSideStruct.OrderItemSidePosKey = Convert.ToInt32(reader["ORDERITEMSIDE_POS_KEY"]);
                    orderItemSideStruct.SideName = Convert.ToString(reader["SIDE_NAME"]);
                    orderItemSideStruct.IsCanceled = Convert.ToString(reader["IS_CANCELED"]) == "T";
                }
            }

            outResponse.ReturnValue = orderItemSideStruct;
        }

        private void getOrderItemSideKeys(FbCommand inCommand, int inOrderItemKey, DTO_ChefMateIntegerListResponse outResponse)
        {
            IList<int> orderItemSideKeys = new List<int>();

            inCommand.CommandText =
              @"SELECT
                    ORDERITEMSIDE_KEY
                FROM
                    ORDERITEMSIDE
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                while (reader.Read())
                {
                    orderItemSideKeys.Add(reader.GetInt32(0));
                }
            }

            outResponse.ReturnValue = orderItemSideKeys;
        }
        #endregion
        #endregion

        #region DBOrderItemTime
        #region PUBLIC
        public DTO_ChefMateResponse ItemArrived(int inOrderItemDbKey, DateTime inTimestamp)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addOrderItemTime(inCommand, inOrderItemDbKey, inTimestamp, 'A'); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse ItemBumped(int inOrderItemDbKey, DateTime inTimestamp)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addOrderItemTime(inCommand, inOrderItemDbKey, inTimestamp, 'B'); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse UnbumpItem(int inOrderItemDbKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { removeOrderItemBumpTime(inCommand, inOrderItemDbKey); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateDateTimeResponse GetOrderGroupBumpTime(int inOrderGroupKey)
        {
            DTO_ChefMateDateTimeResponse response = new DTO_ChefMateDateTimeResponse();
            response.ReturnValue = DateTime.MinValue;
            queryMethod query = delegate(FbCommand inCommand) { getOrderGroupBumpTime(inCommand, inOrderGroupKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateDateTimeResponse GetOrderArrivalTime(int inOrderKey)
        {
            DTO_ChefMateDateTimeResponse response = new DTO_ChefMateDateTimeResponse();
            response.ReturnValue = DateTime.MinValue;
            queryMethod query = delegate(FbCommand inCommand) { getOrderArrivalTime(inCommand, inOrderKey, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void addOrderItemTime(FbCommand inCommand, int inOrderItemDbKey, DateTime inTimestamp, char inTimeTypeCode)
        {
            int orderItemTimeKey = generateKey(inCommand, "ORDERITEMTIME");

            inCommand.CommandText =
              @"INSERT INTO ORDERITEMTIME
                (
                    ORDERITEMTIME_KEY,
                    ORDERITEM_KEY,
                    ORDERITEMTIME_TYPE,
                    ORDERITEMTIME_TIME
                )
                VALUES
                (
                    @ORDERITEMTIME_KEY,
                    @ORDERITEM_KEY,
                    @ORDERITEMTIME_TYPE,
                    @ORDERITEMTIME_TIME
                )";
            inCommand.Parameters.AddWithValue("@ORDERITEMTIME_KEY", orderItemTimeKey);
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemDbKey);
            inCommand.Parameters.AddWithValue("@ORDERITEMTIME_TYPE", inTimeTypeCode);
            inCommand.Parameters.AddWithValue("@ORDERITEMTIME_TIME", inTimestamp.AddMilliseconds(-inTimestamp.Millisecond));
            inCommand.ExecuteNonQuery();
        }

        private void removeOrderItemBumpTime(FbCommand inCommand, int inOrderItemDbKey)
        {
            inCommand.CommandText =
              @"DELETE FROM 
                    ORDERITEMTIME
                WHERE
                    ORDERITEM_KEY = @ORDERITEM_KEY
                AND
                    ORDERITEMTIME_TYPE = 'B'";
            inCommand.Parameters.AddWithValue("@ORDERITEM_KEY", inOrderItemDbKey);
            inCommand.ExecuteNonQuery();
        }

        private void getOrderGroupBumpTime(FbCommand inCommand, int inOrderGroupKey, DTO_ChefMateDateTimeResponse outResponse)
        {
            DateTime groupBumpTime = DateTime.MinValue;
            inCommand.CommandText =
              @"SELECT
                    BUMP_TIME
                FROM 
                    ORDERGROUPTIMESVIEW
                WHERE
                    ORDERGROUP_KEY = @ORDERGROUP_KEY
                AND
                    BUMP_TIME IS NOT NULL";
            inCommand.Parameters.AddWithValue("@ORDERGROUP_KEY", inOrderGroupKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                if (reader.Read())
                {
                    groupBumpTime = reader.GetDateTime(0);
                }
            }

            outResponse.ReturnValue = groupBumpTime;
        }

        private void getOrderArrivalTime(FbCommand inCommand, int inOrderKey, DTO_ChefMateDateTimeResponse outResponse)
        {
            DateTime orderArrivalTime = DateTime.MinValue;
            inCommand.CommandText =
              @"SELECT
                    ARRIVAL_TIME
                FROM 
                    ORDERTIMESVIEW
                WHERE
                    ORDER_KEY = @ORDER_KEY";
            inCommand.Parameters.AddWithValue("@ORDER_KEY", inOrderKey);

            using (FbDataReader reader = inCommand.ExecuteReader())
            {
                if (reader.Read())
                {
                    orderArrivalTime = reader.GetDateTime(0);
                }
            }

            outResponse.ReturnValue = orderArrivalTime;
        }
        #endregion
        #endregion

        #region DBSide
        #region PUBLIC
        public DTO_ChefMateBooleanResponse CheckSideInDb(string inSideName)
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            response.ReturnValue = false;
            queryMethod query = delegate(FbCommand inCommand) { checkSideInDb(inCommand, inSideName, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("CheckGroupInDb()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }

        public DTO_ChefMateResponse AddSide(string inSideName)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { addSide(inCommand, inSideName); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void checkSideInDb(FbCommand inCommand, string inSideName, DTO_ChefMateBooleanResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"SELECT
                    SIDE_KEY
                FROM
                    SIDE
                WHERE
                    SIDE_NAME = @SIDE_NAME";
            inCommand.Parameters.AddWithValue("@SIDE_NAME", inSideName);

            commandResult = inCommand.ExecuteScalar();
            outResponse.ReturnValue = commandResult != null;
        }

        private void addSide(FbCommand inCommand, string inSideName)
        {
            int sideKey = generateKey(inCommand, "SIDE");

            inCommand.CommandText =
              @"INSERT INTO SIDE
                (
                    SIDE_KEY,
                    SIDE_NAME
                )
                VALUES
                (
                    @SIDE_KEY,
                    @SIDE_NAME
                )";
            inCommand.Parameters.AddWithValue("@SIDE_KEY", sideKey);
            inCommand.Parameters.AddWithValue("@SIDE_NAME", inSideName);
            inCommand.ExecuteNonQuery();
        }

        private int getSideKey(FbCommand inCommand, string inSideName)
        {
            object commandResult;
            int sideKey;

            inCommand.CommandText =
              @"SELECT
                    SIDE_KEY
                FROM
                    SIDE
                WHERE
                    SIDE_NAME = @SIDE_NAME";
            inCommand.Parameters.AddWithValue("@SIDE_NAME", inSideName);

            commandResult = inCommand.ExecuteScalar();
            if (commandResult == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                throw new ScalerFailureException(string.Format("Scaler returned in null {0}. SideName: {1}.", methodBase.Name, inSideName));
            }
            else
            {
                sideKey = Convert.ToInt32(commandResult);
            }
            return sideKey;
        }
        #endregion
        #endregion

        #region DBTerminal
        #region PUBLIC
        public DTO_ChefMateIntegerResponse AddTerminal(string inTerminalName, string inIpAddress)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { addTerminal(inCommand, inTerminalName, inIpAddress, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateIntegerResponse GetTerminalKey(string inTerminalName)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;
            queryMethod query = delegate(FbCommand inCommand) { getTerminalKey(inCommand, inTerminalName, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateStringResponse GetTerminalIpAddress(int inTerminalKey)
        {
            DTO_ChefMateStringResponse response = new DTO_ChefMateStringResponse();
            response.ReturnValue = "";
            queryMethod query = delegate(FbCommand inCommand) { getTerminalIpAddress(inCommand, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateResponse SetTerminalIpAddress(string inIpAddress, int inTerminalKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { setTerminalIpAddress(inCommand, inIpAddress, inTerminalKey); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateResponse SetTerminalType(ChefMateTerminalType inType, int inTerminalKey)
        {
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { setTerminalType(inCommand, inType, inTerminalKey); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateDataTableResponse GetTerminalsDataTable(int inTerminalKeyToExclude, ChefMateTerminalType inTerminalType)
        {
            DTO_ChefMateDataTableResponse response = new DTO_ChefMateDataTableResponse();
            response.ReturnValue = null;
            queryMethod query = delegate(FbCommand inCommand) { getTerminalsDataTable(inCommand, inTerminalKeyToExclude, inTerminalType, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateTerminalNamesResponse GetAvailableTerminalNames(int inTerminalKey)
        {
            // gets available terminal names for a given terminal key
            DTO_ChefMateTerminalNamesResponse response = new DTO_ChefMateTerminalNamesResponse();
            queryMethod query = delegate(FbCommand inCommand) { getAvailableTerminalNames(inCommand, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateResponse AssignTerminalName(int inTerminalKey, int inTerminalNameKey)
        {
            // assigns a terminal name for a given terminal key
            DTO_ChefMateResponse response = new DTO_ChefMateResponse();
            queryMethod query = delegate(FbCommand inCommand) { assignTerminalName(inCommand, inTerminalKey, inTerminalNameKey); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateBooleanResponse CheckAlreadyAssignedTerminalName(string newTerminalName)
        {
            // assigns a terminal name for a given terminal key
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            queryMethod query = delegate(FbCommand inCommand) { isAlreadyAssignedTerminalName(inCommand, newTerminalName, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateIntegerResponse SaveTerminalName(string inTerminalName)
        {
            DTO_ChefMateIntegerResponse response = new DTO_ChefMateIntegerResponse();
            response.ReturnValue = -1;

            // add terminal name
            queryMethod query = delegate(FbCommand inCommand) { addTerminalName(inCommand, inTerminalName, TerminalNameType.USER_ADDED); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateTerminalNamesResponse GetAssignedNameByTerminalKey(int inTerminalKey)
        {
            // gets available terminal names for a given terminal key
            DTO_ChefMateTerminalNamesResponse response = new DTO_ChefMateTerminalNamesResponse();
            queryMethod query = delegate(FbCommand inCommand) { getAssignedNameByTerminalKey(inCommand, inTerminalKey, response); };
            executeQuery(query, response);

            return response;
        }

        public DTO_ChefMateBooleanResponse CheckTerminalInformation(int inTerminalKey, string inTerminalComputerName)
        {
            DTO_ChefMateBooleanResponse response = new DTO_ChefMateBooleanResponse();
            queryMethod query = delegate(FbCommand inCommand) { checkTerminalInformation(inCommand, inTerminalKey, inTerminalComputerName, response); };
            executeQuery(query, response);

            return response;
        }

        #endregion

        #region PRIVATE
        private void addTerminal(FbCommand inCommand, string inTerminalName, string inIpAddress, DTO_ChefMateIntegerResponse outResponse)
        {
            int terminalNameKey = addTerminalName(inCommand, inTerminalName, TerminalNameType.COMPUTER);

            int terminalKey = generateKey(inCommand, "TERMINAL");

            inCommand.CommandText =
              @"INSERT INTO TERMINAL
                (
                    TERMINAL_KEY,
                    COMPUTERNAME_KEY,
                    TERMINALNAME_KEY,
                    IP_ADDRESS
                )
                VALUES
                ( 
                    @TERMINAL_KEY,
                    @COMPUTERNAME_KEY,
                    @TERMINALNAME_KEY,
                    @IP_ADDRESS
                )";

            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", terminalKey);
            inCommand.Parameters.AddWithValue("@COMPUTERNAME_KEY", terminalNameKey);
            inCommand.Parameters.AddWithValue("@TERMINALNAME_KEY", terminalNameKey);
            inCommand.Parameters.AddWithValue("@IP_ADDRESS", inIpAddress);
            inCommand.ExecuteNonQuery();

            outResponse.ReturnValue = terminalKey;
        }

        private int addTerminalName(FbCommand inCommand, string inTerminalName, TerminalNameType inNameType)
        {
            int terminalNameKey = generateKey(inCommand, "TERMINALNAMES");

            inCommand.CommandText =
              @"INSERT INTO TERMINALNAMES
                (
                    TERMINALNAMES_KEY,
                    NAME,
                    NAMETYPE
                )
                VALUES
                ( 
                    @TERMINALNAMES_KEY,
                    @NAME,
                    @NAMETYPE
                )";
            inCommand.Parameters.AddWithValue("@TERMINALNAMES_KEY", terminalNameKey);
            inCommand.Parameters.AddWithValue("@NAME", inTerminalName);
            inCommand.Parameters.AddWithValue("@NAMETYPE", inNameType);
            inCommand.ExecuteNonQuery();

            return terminalNameKey;
        }

        private void getTerminalKey(FbCommand inCommand, string inTerminalName, DTO_ChefMateIntegerResponse outResponse)
        {
            object commandResult;
            int terminalKey;

            inCommand.CommandText =
              @"SELECT
                    T.TERMINAL_KEY
                FROM
                    TERMINAL T INNER JOIN TERMINALNAMES TN
                    ON T.TERMINALNAME_KEY = TN.TERMINALNAMES_KEY
                WHERE
                    TN.NAME = @TERMINAL_NAME";
            inCommand.Parameters.AddWithValue("@TERMINAL_NAME", inTerminalName);

            commandResult = inCommand.ExecuteScalar();
            if (commandResult == null)
            {
                terminalKey = -1;
            }
            else
            {
                terminalKey = Convert.ToInt32(commandResult);
            }
            outResponse.ReturnValue = terminalKey;
        }

        private void getTerminalIpAddress(FbCommand inCommand, int inTerminalKey, DTO_ChefMateStringResponse outResponse)
        {
            object commandResult;

            inCommand.CommandText =
              @"SELECT
                    IP_ADDRESS
                FROM
                    TERMINAL
                WHERE
                    TERMINAL_KEY = @TERMINAL_KEY";
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);

            commandResult = inCommand.ExecuteScalar();

            outResponse.ReturnValue = commandResult as string;
        }

        private void setTerminalIpAddress(FbCommand inCommand, string inIpAddress, int inTerminalKey)
        {
            inCommand.CommandText =
              @"UPDATE
                    TERMINAL
                SET
                    IP_ADDRESS = @IP_ADDRESS
                WHERE
                    TERMINAL_KEY = @TERMINAL_KEY";
            inCommand.Parameters.AddWithValue("@IP_ADDRESS", inIpAddress);
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            inCommand.ExecuteNonQuery();
        }

        private void setTerminalType(FbCommand inCommand, ChefMateTerminalType inType, int inTerminalKey)
        {
            inCommand.CommandText =
              @"UPDATE
                    TERMINAL
                SET
                    TERMINAL_TYPE = @TERMINAL_TYPE
                WHERE
                    TERMINAL_KEY = @TERMINAL_KEY";
            switch (inType)
            {
                case ChefMateTerminalType.KITCHEN:
                    inCommand.Parameters.AddWithValue("@TERMINAL_TYPE", "K");
                    break;
                case ChefMateTerminalType.RUNNER:
                    inCommand.Parameters.AddWithValue("@TERMINAL_TYPE", "R");
                    break;
            }
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            inCommand.ExecuteNonQuery();
        }

        private void getTerminalsDataTable(FbCommand inCommand, int inTerminalKeyToExclude, ChefMateTerminalType inTerminalType, DTO_ChefMateDataTableResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT
                    T.TERMINAL_KEY,
                    TN.NAME AS ""TERMINAL NAME"",
                    T.IP_ADDRESS
                FROM
                    TERMINAL T INNER JOIN TERMINALNAMES TN 
                    ON T.TERMINALNAME_KEY = TN.TERMINALNAMES_KEY
                WHERE
                    T.TERMINAL_KEY != @TERMINAL_KEY
                AND
                    TERMINAL_TYPE = @TERMINAL_TYPE";
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKeyToExclude);
            if (inTerminalType == ChefMateTerminalType.KITCHEN)
            {
                inCommand.Parameters.AddWithValue("@TERMINAL_TYPE", 'K');
            }
            else if (inTerminalType == ChefMateTerminalType.RUNNER)
            {
                inCommand.Parameters.AddWithValue("@TERMINAL_TYPE", 'R');
            }

            FbDataAdapter dataAdapter = new FbDataAdapter(inCommand);
            DataTable terminalsDataTable = new DataTable("TerminalsDataTable");

            dataAdapter.Fill(terminalsDataTable);

            outResponse.ReturnValue = terminalsDataTable;
        }

        private void getAvailableTerminalNames(FbCommand inCommand, int inTerminalKey, DTO_ChefMateTerminalNamesResponse response)
        {
            inCommand.CommandText = @"
                    select 
                        TN.TERMINALNAMES_KEY, 
                        TN.NAME, 
                        TN.NAMETYPE, 
                        T.TERMINAL_KEY
                    from 
                        TERMINALNAMES TN, 
                        TERMINAL T
                    where 
                        (TN.TERMINALNAMES_KEY = T.COMPUTERNAME_KEY 
                            or TN.TERMINALNAMES_KEY = T.TERMINALNAME_KEY)
                        and T.TERMINAL_KEY = @TERMINAL_KEY

                    union

                    select 
                        TN.TERMINALNAMES_KEY, 
                        TN.NAME, 
                        TN.NAMETYPE, 
                        T.TERMINAL_KEY
                    from 
                        TERMINALNAMES TN
                        left join TERMINAL T on TN.TERMINALNAMES_KEY = T.TERMINALNAME_KEY
                    where 
                        T.TERMINAL_KEY is null 
                        and TN.NAMETYPE = @NAME_TYPE";

            inCommand.Parameters.Add("TERMINAL_KEY", inTerminalKey);
            inCommand.Parameters.Add("NAME_TYPE", DTO_ChefmateTerminalNameType.USERADDEDNAME);

            FbDataReader reader = inCommand.ExecuteReader();
            setTerminalNamesFromReader(reader, response);
            reader.Close();
        }

        private void setTerminalNamesFromReader(FbDataReader reader, DTO_ChefMateTerminalNamesResponse response)
        {
            if (reader.HasRows)
            {
                DTO_ChefmateTerminalName terminalName;

                while (reader.Read())
                {
                    terminalName = new DTO_ChefmateTerminalName();

                    loadTerminalNameInfoFromReader(terminalName, reader);

                    // populate available terminal names list
                    response.AvailableTerminalNames.Add(terminalName);

                    // when the terminal name is assigned to a terminal, its terminal key is not null
                    // also this can be either computer name or a user added name
                    if (!reader.IsDBNull(reader.GetOrdinal("TERMINAL_KEY"))
                        && (terminalName.Type == DTO_ChefmateTerminalNameType.USERADDEDNAME || terminalName.Type == DTO_ChefmateTerminalNameType.COMPUTERNAME))
                    {
                        response.AssignedTerminalName = terminalName;
                    }
                }
            }
        }

        private void getAssignedNameByTerminalKey(FbCommand inCommand, int inTerminalKey, DTO_ChefMateTerminalNamesResponse response)
        {
            inCommand.CommandText = @"
                            select 
                                TN.TERMINALNAMES_KEY,
                                TN.NAME,
                                TN.NAMETYPE
                            from 
                                TERMINALNAMES TN
                                join TERMINAL T on T.TERMINALNAME_KEY = TN.TERMINALNAMES_KEY
                            where
                                T.TERMINAL_KEY = @TERMINAL_KEY";

            inCommand.Parameters.AddWithValue("TERMINAL_KEY", inTerminalKey);
            FbDataReader reader = inCommand.ExecuteReader();

            if (reader.Read())
            {
                loadTerminalNameInfoFromReader(response.AssignedTerminalName, reader);
            }
        }

        private void checkTerminalInformation(FbCommand inCommand, int inTerminalKey, string inTerminalComputerName, DTO_ChefMateBooleanResponse response)
        {
            inCommand.CommandText =
              @"SELECT
                    TN.NAME
                FROM
                    TERMINAL T INNER JOIN TERMINALNAMES TN
                    ON T.COMPUTERNAME_KEY = TN.TERMINALNAMES_KEY
                WHERE
                    T.TERMINAL_KEY = @TERMINAL_KEY";
            inCommand.Parameters.AddWithValue("@TERMINAL_KEY", inTerminalKey);
            FbDataReader reader = inCommand.ExecuteReader();

            response.ReturnValue = false;
            if (reader.Read())
            {
                response.ReturnValue = inTerminalComputerName == Convert.ToString(reader["NAME"]);
            }
        }

        private void loadTerminalNameInfoFromReader(DTO_ChefmateTerminalName terminalName, FbDataReader reader)
        {
            terminalName.Key = readColumnValue(reader, "TERMINALNAMES_KEY", -99);
            terminalName.Name = readColumnValue(reader, "NAME", "");
            terminalName.Type = (DTO_ChefmateTerminalNameType)readColumnValue(reader, "NAMETYPE", 0);
        }

        private void assignTerminalName(FbCommand inCommand, int inTerminalKey, int inTerminalNameKey)
        {
            inCommand.CommandText =
                          @"UPDATE 
                                TERMINAL T
                            SET
                                T.TERMINALNAME_KEY = @TERMINALNAME_KEY
                            WHERE
                                T.TERMINAL_KEY = @TERMINAL_KEY";

            inCommand.Parameters.Add("TERMINALNAME_KEY", inTerminalNameKey);
            inCommand.Parameters.Add("TERMINAL_KEY", inTerminalKey);

            inCommand.ExecuteNonQuery();
        }

        private void isAlreadyAssignedTerminalName(FbCommand inCommand, string newTerminalName, DTO_ChefMateBooleanResponse response)
        {
            inCommand.CommandText = @"
                            select 
                                TN.TERMINALNAMES_KEY
                            from 
                                TERMINALNAMES TN
                            where
                                upper(TN.NAME) = upper(@TERMINAL_NAME)
                                and TN.TERMINALNAMES_KEY in
                                (
                                    select t.TERMINALNAME_KEY from TERMINAL t
                                    union
                                    select t.COMPUTERNAME_KEY from TERMINAL t
                                )";

            inCommand.Parameters.AddWithValue("TERMINAL_NAME", newTerminalName);
            response.ReturnValue = Convert.ToInt32(inCommand.ExecuteScalar()) > 0;
        }

        #endregion
        #endregion

        #region DBVersion
        #region PUBLIC
        public DTO_ChefMateStringResponse GetDatabaseVersion()
        {
            DTO_ChefMateStringResponse response = new DTO_ChefMateStringResponse();
            response.ReturnValue = "";
            queryMethod query = delegate(FbCommand inCommand) { getDatabaseVersion(inCommand, response); };
            executeQuery(query, response);

            if (!response.Successful)
            {
                //?????.logError("AddGroup()", "WCFServiceChefMate", response.Message);
            }
            return response;
        }
        #endregion

        #region PRIVATE
        private void getDatabaseVersion(FbCommand inCommand, DTO_ChefMateStringResponse outResponse)
        {
            inCommand.CommandText =
              @"SELECT
                    VERSION_NUMBER
                FROM
                    VERSION
                WHERE
                    VERSION_TIME_STAMP = (SELECT MAX(VERSION_TIME_STAMP) FROM VERSION)";
            string version = inCommand.ExecuteScalar() as string;
            if (version == null)
            {
                System.Reflection.MethodBase methodBase = System.Reflection.MethodBase.GetCurrentMethod();
                throw new ScalerFailureException(string.Format("Scaler returned in null {0}.", methodBase.Name));
            }
            outResponse.ReturnValue = version;
        }
        #endregion
        #endregion

        #region DBAccessUtilities

        /// <summary>
        /// Reads an integer column value.
        /// Returns the supplied default value if the column is null
        /// </summary>
        /// <param name="reader"></param>
        /// <param name="columnName"></param>
        /// <param name="defaultValue"></param>
        /// <returns></returns>
        private int readColumnValue(FbDataReader reader, string columnName, int defaultValue)
        {
            int index = reader.GetOrdinal(columnName);
            int result = reader.IsDBNull(index)
                            ? defaultValue
                            : reader.GetInt32(index);

            return result;
        }

        /// <summary>
        /// Reads a string column value
        /// Returns the supplied default value if the column is null
        /// </summary>
        /// <param name="reader"></param>
        /// <param name="columnName"></param>
        /// <param name="defaultValue"></param>
        /// <returns></returns>
        private string readColumnValue(FbDataReader reader, string columnName, string defaultValue)
        {
            int index = reader.GetOrdinal(columnName);
            string result = reader.IsDBNull(index)
                            ? defaultValue
                            : reader.GetString(index);

            return result;
        }

        #endregion
    }
}
