using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using MenumateServices.ChefMate.DTO;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServiceChefMate" in both code and config file together.
    [ServiceContract]
    public interface IWCFServiceChefMate
    {
        #region DBAuxiliary
        [OperationContract]
        DTO_ChefMateBooleanResponse DatabaseConnectionTest();
        #endregion

        #region DBGroup
        [OperationContract]
        DTO_ChefMateBooleanResponse CheckGroupInDb(ChefMateGroupType inGroupType, int inGroupPosDbKey);
        
        [OperationContract]
        DTO_ChefMateResponse AddGroup(string inName, ChefMateGroupType inGroupType, int inGroupPosDbKey, int inGroupDisplayOrder);
        
        [OperationContract]
        DTO_ChefMateResponse UpdateGroupDisplayOrder(int inGroupPosDbKey, int inGroupDisplayOrder, ChefMateGroupType inGroupType);
        
        [OperationContract]
        DTO_ChefMateIntegerResponse GetGroupDisplayOrder(int inGroupPosDbKey, ChefMateGroupType inGroupType);
        #endregion

        #region DBItem
        [OperationContract]
        DTO_ChefMateBooleanResponse CheckItemInDb(int inItemPosDbKey);
        
        [OperationContract]
        DTO_ChefMateResponse AddItem(string inName, int inItemPosDbKey);

        [OperationContract]
        DTO_ChefMateIntegerResponse GetItemKey(int inItemPosDbKey);

        [OperationContract]
        DTO_ChefMateBooleanResponse UpdateOrderItemGroupKey(int inOrderItemKey, int inCourseKey, int inSCourseKey);
        #endregion

        #region DBOption
        [OperationContract]
        DTO_ChefMateBooleanResponse CheckOptionInDb(string inOptionName);
        
        [OperationContract]
        DTO_ChefMateResponse AddOption(string inOptionName);
        #endregion

        #region DBOrder
        [OperationContract]
        DTO_ChefMateIntegerResponse AddOrder(DTO_ChefMateOrder inOrderStruct);
        
        [OperationContract]
        DTO_ChefMateOrderResponse GetOrder(int inOrderKey);

        [OperationContract]
        DTO_ChefMateFullOrderResponse GetFullOrderFromOrderKey(int inOrderKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateIntegerListResponse GetOrderKeysFromTerminalKey(int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateDataTableResponse GetBumpedOrders();
        
        [OperationContract]
        DTO_ChefMateDataTableResponse GetBumpedOrderInformation(int inOrderKey);
        
        [OperationContract]
        DTO_ChefMateResponse UnbumpOrder(int inOrderKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateDataTableResponse GetModelKeysFromOrderKey(int inOrderKey, int inTerminalKey);

        [OperationContract]
        DTO_ChefMateResponse UpdateOrderTerminalName(int inOrderKey, string inTableTabName);

        [OperationContract]
        DTO_ChefMateFullOrderResponse GetFullOrderFromOrderPosKey(int inOrderPosKey, int inTerminalKey);

        [OperationContract]
        DTO_ChefMateFullOrderResponse GetFullOrderFromTableTabName(string inTableTabName, int inTerminalKey);

        #endregion

        #region DBOrderGroup
        [OperationContract]
        DTO_ChefMateIntegerResponse AddOrderGroup(int inOrderKey, DTO_ChefMateOrderGroup inOrderGroupStuct);
        
        [OperationContract]
        DTO_ChefMateOrderGroupResponse GetOrderGroup(int inOrderGroupKey);

        [OperationContract]
        DTO_ChefMateFullOrderResponse GetFullOrderFromOrderGroupKey(int inOrderGroupKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateIntegerListResponse GetOrderGroupKeysFromOrderKey(int inOrderKey, ChefMateGroupType inGroupType, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateIntegerResponse GetOrderKey(int inOrderGroupKey);
        
        [OperationContract]
        DTO_ChefMateResponse CallAwayOrderGroup(int inOrderGroupKey);
        #endregion

        #region DBOrderItem
        [OperationContract]
        DTO_ChefMateIntegerResponse AddOrderItem(int inServingCourseOrderGroupKey, int inCourseOrderGroupKey, int inTerminalKey, DTO_ChefMateOrderItem inOrderItemStruct);
        
        [OperationContract]
        DTO_ChefMateResponse AddOrderItemTerminal(int inOrderItemKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateOrderItemResponse GetOrderItem(int inOrderItemKey);

        [OperationContract]
        DTO_ChefMateFullOrderResponse GetFullOrderFromOrderItemKey(int inOrderItemKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateResponse UpdateOrderItemStatus(int inOrderItemKey, string inStatus);
        
        [OperationContract]
        DTO_ChefMateResponse UpdateOrderItemTerminalKey(int inOrderItemKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateResponse UpdateOrderItemPosDbKey(int inOrderItemKey, int inOrderItemPosDbKey);
        
        [OperationContract]
        DTO_ChefMateIntegerListResponse GetOrderItemKeysFromOrderKey(int inOrderKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateIntegerListResponse GetOrderItemKeysFromOrderGroupKey(int inOrderGroupKey, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateModelKeysResponse GetModelKeys(int inOrderItemkey);
        
        [OperationContract]
        DTO_ChefMateDataTableResponse GetBumpedOrderItems(int inOrderKey);
        #endregion

        #region DBOrderItemOption
        [OperationContract]
        DTO_ChefMateIntegerResponse AddOrderItemOption(int inOrderItemKey, string inOptionName, bool isPlus);
        
        [OperationContract]
        DTO_ChefMateOrderItemOptionResponse GetOrderItemOption(int inOrderItemOptionKey);
        
        [OperationContract]
        DTO_ChefMateIntegerListResponse GetOrderItemOptionKeys(int inOrderItemKey);
        #endregion

        #region DBOrderItemSide
        [OperationContract]
        DTO_ChefMateIntegerResponse AddOrderItemSide(int inOrderItemKey, DTO_ChefMateOrderItemSide inOrderItemSideStruct);
        
        [OperationContract]
        DTO_ChefMateIntegerResponse GetOrderItemSideKey(int inOrderPosDbKey, ChefMateGroupType inGroupType, int inGroupPosDbKey, int inItemPosDbKey, int inSidePosDbKey);
        
        [OperationContract]
        DTO_ChefMateResponse CancelOrderItemSide(int inOrderItemSideKey);
        
        [OperationContract]
        DTO_ChefMateOrderItemSideResponse GetOrderItemSide(int inOrderItemSideKey);
        
        [OperationContract]
        DTO_ChefMateIntegerListResponse GetOrderItemSideKeys(int inOrderItemKey);
        #endregion

        #region DBOrderItemTime
        [OperationContract]
        DTO_ChefMateResponse ItemArrived(int inOrderItemDbKey, DateTime inTimestamp);
        
        [OperationContract]
        DTO_ChefMateResponse ItemBumped(int inOrderItemDbKey, DateTime inTimestamp);
        
        [OperationContract]
        DTO_ChefMateResponse UnbumpItem(int inOrderItemDbKey);
        
        [OperationContract]
        DTO_ChefMateDateTimeResponse GetOrderGroupBumpTime(int inOrderGroupKey);
        
        [OperationContract]
        DTO_ChefMateDateTimeResponse GetOrderArrivalTime(int inOrderKey);
        #endregion

        #region DBSide
        [OperationContract]
        DTO_ChefMateBooleanResponse CheckSideInDb(string inSideName);
        
        [OperationContract]
        DTO_ChefMateResponse AddSide(string inSideName);
        #endregion

        #region DBTerminal
        [OperationContract]
        DTO_ChefMateIntegerResponse AddTerminal(string inTerminalName, string inIpAddress);
        
        [OperationContract]
        DTO_ChefMateIntegerResponse GetTerminalKey(string inTerminalName);
        
        [OperationContract]
        DTO_ChefMateStringResponse GetTerminalIpAddress(int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateResponse SetTerminalIpAddress(string inIpAddress, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateResponse SetTerminalType(ChefMateTerminalType inType, int inTerminalKey);
        
        [OperationContract]
        DTO_ChefMateDataTableResponse GetTerminalsDataTable(int inTerminalKeyToExclude, ChefMateTerminalType inTerminalType);

        [OperationContract]
        DTO_ChefMateTerminalNamesResponse GetAvailableTerminalNames(int inTerminalKey);

        [OperationContract]
        DTO_ChefMateResponse AssignTerminalName(int inTerminalKey, int inTerminalNameKey);

        [OperationContract]
        DTO_ChefMateBooleanResponse CheckAlreadyAssignedTerminalName(string newTerminalName);

        [OperationContract]
        DTO_ChefMateIntegerResponse SaveTerminalName(string terminalName);

        [OperationContract]
        DTO_ChefMateTerminalNamesResponse GetAssignedNameByTerminalKey(int inTerminalKey);

        [OperationContract]
        DTO_ChefMateBooleanResponse CheckTerminalInformation(int inTerminalKey, string inTerminalComputerName);
       
        #endregion

        #region DBVersion
        [OperationContract]
        DTO_ChefMateStringResponse GetDatabaseVersion();
        #endregion
    }
}
