using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using WebmateConsole.WebmateServiceReference;

using System.ComponentModel;

namespace WebmateConsole
{
    public class WebmateClient
    {
        string errorMsg_        = "";
        ResponseCode errorCode_ = ResponseCode.Successful;

        WCFServiceWebMateClient wcfClient_ = new WCFServiceWebMateClient();

        #region PUBLIC
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHeader"></param>
        /// <param name="inFrom"></param>
        /// <param name="inAccount"></param>
        /// <param name="outWebOrder"></param>
        /// <returns></returns>
        public bool OpenWebOrder(
                            DTO_WebOrderHeader  inHeader,
                            DTO_WebOrderFrom    inFrom,
                            DTO_WebOrderAccount inAccount,
                        ref DTO_WebOrder outWebOrder)
        {
            errorMsg_  = "";
            errorCode_ = ResponseCode.Successful;

            inHeader.PropertyChanged  += propertyChangedEventHandler;
            inFrom.PropertyChanged    += propertyChangedEventHandler;
            inAccount.PropertyChanged += propertyChangedEventHandler;

            DTO_WebOrderResponse response = wcfClient_.OpenWebOrder(inHeader, inFrom, inAccount, ref outWebOrder);

            if (!response.Succesful)
            {
                errorMsg_  = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        /// <returns></returns>
        public bool CommitWebOrder(DTO_WebOrder inWebOrder)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            inWebOrder.PropertyChanged += propertyChangedEventHandler;

            DTO_WebOrderResponse response = wcfClient_.CommitOrder(inWebOrder);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStoreName"></param>
        /// <param name="inGUID"></param>
        /// <param name="inScheduled"></param>
        /// <param name="inRespond"></param>
        /// <param name="inOrderDate"></param>
        /// <param name="inExpectedDate"></param>
        /// <param name="inOrderTotal"></param>
        /// <param name="outOrderHeader"></param>
        /// <returns></returns>
        public bool CreateWebOrderHeader(
                        string inStoreName,
                        string inGUID,
                        UInt16 inScheduled,
                        bool inRespond,
                        DateTime inOrderDate,
                        DateTime inExpectedDate,
                        Decimal inOrderTotal,
                    ref DTO_WebOrderHeader outOrderHeader)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderHeader(
                                                            inStoreName,
                                                            inGUID,
                                                            inScheduled,
                                                            inRespond,
                                                            inOrderDate,
                                                            inExpectedDate,
                                                            inOrderTotal,
                                                        ref outOrderHeader);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inID"></param>
        /// <param name="outOrderFrom"></param>
        /// <returns></returns>
        public bool CreateWebOrderFrom(
                        string inID,
                    ref DTO_WebOrderFrom outOrderFrom)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderFrom(
                                                                inID,
                                                            ref outOrderFrom);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        public bool CreateWebOrderAccount(
                         UInt32 inCovers,
                         string inName,
                         string inMemberNumber,
                         DTO_WebOrderPayments inPayments,
                         DTO_WebOrderComments inComments,
                         DTO_WebOrderItems    inItems,
                     ref DTO_WebOrderAccount  outOrderAccount)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            inPayments.PropertyChanged += propertyChangedEventHandler;
            inComments.PropertyChanged += propertyChangedEventHandler;
            inItems.PropertyChanged    += propertyChangedEventHandler;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderAccount(
                                                            1,          // Covers
                                                            "Jorge",    // Name
                                                            "001-001",  // Member Number
                                                            inPayments, // Payments
                                                            inComments, // Comments
                                                            inItems,    // Items
                                                        ref outOrderAccount);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPaymentRequired"></param>
        /// <param name="outOrderPayments"></param>
        /// <returns></returns>
        public bool CreateWebOrderPayments(
                        bool inPaymentRequired,
                    ref DTO_WebOrderPayments outOrderPayments)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderPayments(
                                                            inPaymentRequired,
                                                        ref outOrderPayments);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inComments"></param>
        /// <param name="outComments"></param>
        /// <returns></returns>
        public bool CreateWebOrderComments(
                                 string[] inComments,
                             ref DTO_WebOrderComments outComments)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderComments(
                                                            inComments,
                                                        ref outComments);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inUID"></param>
        /// <param name="inBasePrice"></param>
        /// <param name="inDiscountID"></param>
        /// <param name="inQty"></param>
        /// <param name="inOrderItemOptions"></param>
        /// <param name="inOrderItemSides"></param>
        /// <param name="outOrderItem"></param>
        public bool CreateWebOrderItem(
                            DTO_UID                 inUID,
                            Decimal                 inBasePrice,
                            UInt32                  inDiscountID,
                            UInt32                  inQty,
                            DTO_WebOrderItemOptions inOrderItemOptions,
                            DTO_WebOrderItemSides   inOrderItemSides,
                        ref DTO_WebOrderItem        outOrderItem)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            inOrderItemOptions.PropertyChanged += propertyChangedEventHandler;
            inOrderItemSides.PropertyChanged   += propertyChangedEventHandler;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderItem(
                                                            inUID,
                                                            inBasePrice,
                                                            inDiscountID,
                                                            inQty,
                                                            inOrderItemOptions,
                                                            inOrderItemSides,
                                                        ref outOrderItem);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inUID"></param>
        /// <param name="inBasePrice"></param>
        /// <param name="inDiscountID"></param>
        /// <param name="inQty"></param>
        /// <param name="outItemSide"></param>
        /// <returns></returns>
        public bool CreateWebOrderItemSide(
                                DTO_UID inUID,
                                Decimal inBasePrice,
                                UInt32 inDiscountID,
                                UInt32 inQty,
                            ref DTO_WebOrderItemSide outItemSide)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderItemSide(
                                                            inUID,
                                                            inBasePrice,
                                                            inDiscountID,
                                                            inQty,
                                                        ref outItemSide);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inUID"></param>
        /// <param name="outItemOption"></param>
        /// <returns></returns>
        public bool CreateWebOrderItemOption(
                                        DTO_UID inUID,
                                    ref DTO_WebOrderItemOption outItemOption)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderItemOption(
                                                            inUID,
                                                        ref outItemOption);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemList"></param>
        /// <param name="outOrderItems"></param>
        public bool CreateWebOrderItems(
                            DTO_WebOrderItem[] inItemList, 
                        ref DTO_WebOrderItems outOrderItems)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderItems(inItemList, ref outOrderItems);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemList"></param>
        /// <param name="outItemOptions"></param>
        public bool CreateWebOrderItemOptions(
                            DTO_WebOrderItemOption[] inItemList,
                        ref DTO_WebOrderItemOptions outItemOptions)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderItemOptions(inItemList, ref outItemOptions);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItemList"></param>
        /// <param name="outItemSides"></param>
        public bool CreateWebOrderItemSides(
                            DTO_WebOrderItemSide[] inItemList,
                        ref DTO_WebOrderItemSides outItemSides)
        {
            errorMsg_ = "";
            errorCode_ = ResponseCode.Successful;

            DTO_WebOrderResponse response = wcfClient_.CreateWebOrderItemSides(inItemList, ref outItemSides);

            if (!response.Succesful)
            {
                errorMsg_ = response.Message;
                errorCode_ = response.ResponseCode;
            }

            return response.Succesful;
        }

        /// <summary>
        /// 
        /// </summary>
        public string ErrorMessage
        {
            get { return errorMsg_; }
        }
        #endregion

        #region PRIVATE
        void propertyChangedEventHandler(object sender, PropertyChangedEventArgs e)
        {
        }
        #endregion
    }
}
