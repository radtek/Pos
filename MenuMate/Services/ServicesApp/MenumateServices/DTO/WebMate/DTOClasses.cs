using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

/// <summary>
/// Classess in this file:
///     DTO_WebOrderResponse:    Describes a WebMate Order Response
///     DTO_WebOrder:            Describes a WebMate Order
///     DTO_WebOrderHeader:      Describes a WebMate Order's Header
///     DTO_WebOrderFrom:        Describes a WebMate Order's "From" Section
///     DTO_WebOrderAccount:     Describes a WebMate Order's Account
///     DTO_WebOrderPayments:    Describes a WebMate Order's Payment
///     DTO_WebOrderComments:    Describes a WebMate Order's Comments
///     DTO_BaseItems:           Describes a Base Item List
///     DTO_WebOrderItems:       Describes a WebMate Order's Item List
///     DTO_WebOrderItemOptions: Describes a WebMate Order's Item Option List
///     DTO_WebOrderItemSides:   Describes a WebMate Order's Item Side List
///     DTO_BaseItem:            Describes a Base Item
///     DTO_UID:                 Describes a Unique Identifier. It can be both an string and a Integer
///     DTO_BaseWebOrderItem:    Describes a Base WebMate Order's Item
///     DTO_WebOrderItem:        Describes a WebMate Order's Item
///     DTO_WebOrderItemOption:  Describes a WebMate Order's Item Option
///     DTO_WebOrderItemSide:    Describes a WebMate Order's Item Side
/// </summary>

namespace MenumateServices.WebMate.DTO
{
    public enum ResponseCode
    {
        Successful = 0,
        MenumateIsOffLine,
        FailedToOpenOrder,
        FailedToCommitOrder,
        FailedToCreateOrderHeaderSection,
        FailedToCreateOrderFromSection,
        FailedToCreateOrderAccountSection,
        FailedToCreateOrderPaymentSection,
        FailedToCreateOrderCommentsSection,
        FailedToCreateOrderItemSection,
        FailedToCreateOrderItem,
        FailedToCreateOrderItemSide,
        FailedToCreateOrderItemOption,
        OrderGUIDDuplicate
    };

    /// <summary>
    /// Describes a WebMate Order Response
    ///     Succesful:        True if the function succeeds 
    ///     Message:          Response's message
    ///     Description:      Detailed description of the response's message
    ///     ResponseCode: The transaction's response code.  Take a look at "enum ResponseCode"
    /// </summary>
    [DataContract]
    public class DTO_WebOrderResponse
    {        
        bool         succesful_;
        string       message_;
        string       description_;
        ResponseCode responseCode_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderResponse(
                        bool         inSuccesful,
                        string       inMessage,
                        string       inDescription,
                        ResponseCode inResponseCode)
        {
            succesful_      = inSuccesful;
            message_      = inMessage;
            description_  = inDescription;
            responseCode_ = inResponseCode;
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        bool Succesful
        {
            get { return succesful_;  }
            set { succesful_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        string Message
        {
            get { return message_;  }
            set { message_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        string Description
        {
            get { return description_ ; }
            set { description_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        ResponseCode ResponseCode
        {
            get { return responseCode_;  }
            set { responseCode_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static DTO_WebOrderResponse createResponseNoError()
        {
            return createResponse(true, "", "", ResponseCode.Successful);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        public static DTO_WebOrderResponse createResponseError(
                                        string inMessage,
                                        string inDescription,
                                        ResponseCode inResponseCode)
        {
            return new DTO_WebOrderResponse(
                                false,
                                inMessage,
                                inDescription,
                                inResponseCode);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSuccesful"></param>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        public static DTO_WebOrderResponse createResponse(
                                                bool                inSuccesful,
                                                string              inMessage,
                                                string              inDescription,
                                                ResponseCode inResponseCode)
        {
            return new DTO_WebOrderResponse(
                                inSuccesful,
                                inMessage,
                                inDescription,
                                inResponseCode);
        }

        #endregion
    }

    /// <summary>
    /// Describes a WebMate Order
    /// </summary>
    [DataContract]
    public class DTO_WebOrder
    {
        DTO_WebOrderHeader  header_;
        DTO_WebOrderFrom     from_;
        DTO_WebOrderAccount account_;

        /// <summary>
        /// 
        /// </summary> 
        /// <param name="inHeader"></param>
        internal DTO_WebOrder(
                        DTO_WebOrderHeader  inHeader, 
                        DTO_WebOrderFrom    inFrom,
                        DTO_WebOrderAccount inAccount)
        {
            header_  = inHeader;
            from_    = createFrom(inFrom);
            account_ = createAccount(inAccount);
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderHeader Header
        {
            get { return header_;  }
            set { header_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderFrom From
        {
            get { return from_;  }
            set { from_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderAccount Account
        {
            get { return account_;  }
            set { account_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderPayments Payments
        {
            get { return account_.Payments;  }
            set { account_.Payments = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderComments Comments
        {
            get { return account_.Comments;  }
            set { account_.Comments = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderItems Items
        {
            get { return account_.Items;  }
            set { account_.Items = value; }
        }
        #endregion

        #region Private
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFrom"></param>
        /// <returns></returns>
        DTO_WebOrderFrom createFrom(DTO_WebOrderFrom inFrom)
        {
            DTO_WebOrderFrom result = new DTO_WebOrderFrom(inFrom.ID);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inAccount"></param>
        /// <returns></returns>
        DTO_WebOrderAccount createAccount(DTO_WebOrderAccount inAccount)
        {
            DTO_WebOrderAccount result = new DTO_WebOrderAccount(
                                                inAccount.Covers,
                                                inAccount.Name,
                                                inAccount.MemberNumber,
                                                inAccount.Payments,
                                                inAccount.Comments,
                                                inAccount.Items);

            return result;
        }
        #endregion
    }
    
    /// <summary>
    /// Describes a WebMate Order's Header
    /// </summary>
    [DataContract]
    public class DTO_WebOrderHeader
    {
        string storeName_;
        string guid_;
        UInt16 scheduled_;
        bool respond_;
        DateTime orderDate_;
        DateTime expectedDate_;
        Decimal orderTotal_; 

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderHeader(
                            string inStoreName,
                            string inGUID,
                            UInt16 inScheduled,
                            bool inRespond,
                            DateTime inOrderDate,
                            DateTime inExpectedDate,
                            Decimal inOrderTotal )
        {
            storeName_    = inStoreName;
            guid_         = inGUID;
            scheduled_    = inScheduled;
            respond_      = inRespond;
            orderDate_    = inOrderDate;
            expectedDate_ = inExpectedDate;
            orderTotal_   = inOrderTotal; 
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string StoreName
        {
            get { return storeName_; }
            set { storeName_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string GUID
        {
            get { return guid_; }
            set { guid_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt16 Scheduled
        {
            get { return scheduled_; }
            set { scheduled_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public bool Respond
        {
            get { return respond_; }
            set { respond_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DateTime OrderDate
        {
            get { return orderDate_; }
            set { orderDate_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DateTime ExpectedDate
        {
            get { return expectedDate_; }
            set { expectedDate_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public Decimal OrderTotal
        {
            get { return orderTotal_; }
            set { orderTotal_ = value; }
        }

        #endregion

    }

    /// <summary>
    /// Describes a WebMate Order's "From" Section
    /// So far there is only the name of the website the order is placed from
    /// e.g Mobie2Go
    /// </summary>
    [DataContract]
    public class DTO_WebOrderFrom
    {
        string id_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderFrom(string inID)
        {
            id_ = inID;
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string ID 
        {
            get { return id_;  }
            set { id_ = value; }
        }

        #endregion
    }

    /// <summary>
    /// Describes a WebMate Order's Account
    /// Account: Payments, Comments, Items
    /// </summary>
    [DataContract]
    public class DTO_WebOrderAccount
    {
        UInt32 covers_;
        string name_;
        string memberNumber_;

        DTO_WebOrderPayments payments_;
        DTO_WebOrderComments comments_;
        DTO_WebOrderItems items_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderAccount(
                            UInt32 inCovers,
                            string inName,
                            string inMemberNumber,
                            DTO_WebOrderPayments inPayments,
                            DTO_WebOrderComments inComments,
                            DTO_WebOrderItems inItems)
        {
            covers_       = inCovers;
            name_         = inName;
            memberNumber_ = inMemberNumber;

            payments_ = createPayments(inPayments);
            comments_ = createComments(inComments);
            items_    = createItems(inItems);
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt32 Covers
        {
            get { return covers_;  }
            set { covers_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string Name
        {
            get { return name_;  }
            set { name_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string MemberNumber
        {
            get { return memberNumber_;  }
            set { memberNumber_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderPayments Payments
        {
            get { return payments_;  }
            set { payments_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderComments Comments
        {
            get { return comments_;  }
            set { comments_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderItems Items
        {
            get { return items_;  }
            set { items_ = value; }
        }
        #endregion

        #region Private
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPayments"></param>
        /// <returns></returns>
        DTO_WebOrderPayments createPayments(DTO_WebOrderPayments inPayments)
        {
            DTO_WebOrderPayments result = new DTO_WebOrderPayments(inPayments.PaymentRequired);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inComments"></param>
        /// <returns></returns>
        DTO_WebOrderComments createComments(DTO_WebOrderComments inComments)
        {
            DTO_WebOrderComments result = new DTO_WebOrderComments(inComments.Comments);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItems"></param>
        /// <returns></returns>
        DTO_WebOrderItems createItems(DTO_WebOrderItems inItems)
        {
            DTO_WebOrderItems result = new DTO_WebOrderItems(inItems.Items);

            return result;
        }
        #endregion
    }

    /// <summary>
    /// Describes a WebMate Order's Payment
    /// So far there is only: Payment Required or Not
    /// </summary>
    [DataContract]
    public class DTO_WebOrderPayments
    {
        bool paymentRequired_;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPaymentRequired"></param>
        internal DTO_WebOrderPayments(bool inPaymentRequired)
        {
            paymentRequired_ = inPaymentRequired;
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public bool PaymentRequired
        {
            get { return paymentRequired_;  }
            set { paymentRequired_ = value; }
        }
        #endregion
    }

    /// <summary>
    /// Describes a WebMate Order's Comments
    /// </summary>
    [DataContract]
    public class DTO_WebOrderComments
    {
        List<string> comments_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderComments()
        {
            comments_ = createList();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inComments"></param>
        internal DTO_WebOrderComments(string[] inComments)
        {
            comments_ = createListWithComments(inComments);
        }

        #region Public
    
        /// <summary>
        /// Return a copy of the OrderComment List.
        /// Modifying an element in the returned list will not affect the actual OrderComment List in this class
        /// </summary>
        [DataMember]
        public string[] Comments
        {
            get { return comments_.ToArray(); }
            set 
            { 
                comments_.Clear();  
                foreach( string comment in value)
                {
                    comments_.Add( comment ); 
                }
            }
        }

        /// <summary>
        /// Returns true if the Comment is succefuly added to the Comment List
        /// </summary>
        /// <param name="inComment"></param>
        public bool AddComment(string inComment)
        {
            try
            {
                comments_.Add(inComment);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Returns true if the Comment is succefuly removed from the Comment List 
        /// </summary>
        /// <param name="inIndex"></param>
        public bool RemoveComment(string inComment)
        {
            try
            {
                comments_.Remove(inComment);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Returns true if the Comment at the Index is succefuly removed from the Comment List  
        /// </summary>
        /// <param name="inIndex"></param>
        public bool RemoveCommentAt(UInt32 inIndex)
        {
            try
            {
                comments_.RemoveAt((int)inIndex);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        #endregion

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        List<string> createList()
        {
            List<string> result = new List<string>();

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inComments"></param>
        /// <returns></returns>
        List<string> createListWithComments(string[] inComments)
        {
            List<string> result = new List<string>();

            result.AddRange(inComments);

            return result;
        }

        #endregion
    }

    /// <summary>
    /// Describes a WebMate Order's Item List
    /// </summary>
    [DataContract]
    public class DTO_WebOrderItems
    {
        List<DTO_BaseWebOrderItem> items_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderItems()
        {
            items_ = new List<DTO_BaseWebOrderItem>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItems"></param>
        internal DTO_WebOrderItems(DTO_BaseWebOrderItem[] inItems)
        {
            items_ = new List<DTO_BaseWebOrderItem>();
            items_.AddRange(inItems);
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_BaseWebOrderItem[] Items
        {
            get { return items_.ToArray(); }
            set { items_.Clear(); items_.AddRange(value);}
        }

        /// <summary>
        /// Returns true if the Item is successfuly added to the Item List.
        /// </summary>
        /// <param name="inItem"></param>
        public bool AddItem(DTO_BaseWebOrderItem inItem)
        {
            try
            {
                items_.Add(inItem);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Returns true if the Item is successfuly removed from the Item List. 
        /// </summary>
        /// <param name="inItem"></param>
        /// <returns></returns>
        public bool RemoveItem(DTO_BaseWebOrderItem inItem)
        {
            try
            {
                items_.Remove(inItem);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        ///  Returns true if the Item at the index is successfuly removed from the Item List. 
        /// </summary>
        /// <param name="inIndex"></param>
        /// <returns></returns>
        public bool RemoveItemAt(UInt32 inIndex)
        {
            try
            {
                items_.RemoveAt((int)inIndex);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        #endregion
    }

    /// <summary>
    /// Describes a WebMate Order's Item Side List
    /// </summary>
    [DataContract]
    public class DTO_WebOrderItemSides
    {
        List<DTO_WebOrderItemSide> items_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderItemSides()
        {
            items_ = new List<DTO_WebOrderItemSide>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItems"></param>
        internal DTO_WebOrderItemSides(DTO_WebOrderItemSide[] inItems)
        {
            items_ = new List<DTO_WebOrderItemSide>();
            items_.AddRange(inItems);
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderItemSide[] Items
        {
            get { return items_.ToArray(); }
            set { items_.Clear(); items_.AddRange(value); }
        }

        /// <summary>
        /// Returns true if the Item is successfuly added to the Item List.
        /// </summary>
        /// <param name="inItem"></param>
        public bool AddItem(DTO_WebOrderItemSide inItem)
        {
            try
            {
                items_.Add(inItem);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Returns true if the Item is successfuly removed from the Item List. 
        /// </summary>
        /// <param name="inItem"></param>
        /// <returns></returns>
        public bool RemoveItem(DTO_WebOrderItemSide inItem)
        {
            try
            {
                items_.Remove(inItem);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        ///  Returns true if the Item at the index is successfuly removed from the Item List. 
        /// </summary>
        /// <param name="inIndex"></param>
        /// <returns></returns>
        public bool RemoveItemAt(UInt32 inIndex)
        {
            try
            {
                items_.RemoveAt((int)inIndex);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        #endregion
    }

    [DataContract]
    public class DTO_WebOrderItemOptions
    {
        List<DTO_WebOrderItemOption> items_;

        /// <summary>
        /// 
        /// </summary>
        internal DTO_WebOrderItemOptions()
        {
            items_ = new List<DTO_WebOrderItemOption>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inItems"></param>
        internal DTO_WebOrderItemOptions(DTO_WebOrderItemOption[] inItems)
        {
            items_ = new List<DTO_WebOrderItemOption>(inItems);
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_WebOrderItemOption[] Items
        {
            get { return items_.ToArray(); }
            set { items_.Clear(); items_.AddRange(value); }
        }

        /// <summary>
        /// Returns true if the Item is successfuly added to the Item List.
        /// </summary>
        /// <param name="inItem"></param>
        public bool AddItem(DTO_WebOrderItemOption inItem)
        {
            try
            {
                items_.Add(inItem);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// Returns true if the Item is successfuly removed from the Item List. 
        /// </summary>
        /// <param name="inItem"></param>
        /// <returns></returns>
        public bool RemoveItem(DTO_WebOrderItemOption inItem)
        {
            try
            {
                items_.Remove(inItem);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        ///  Returns true if the Item at the index is successfuly removed from the Item List. 
        /// </summary>
        /// <param name="inIndex"></param>
        /// <returns></returns>
        public bool RemoveItemAt(UInt32 inIndex)
        {
            try
            {
                items_.RemoveAt((int)inIndex);

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        #endregion
    }

    /// <summary>
    /// Describes a Base Item
    /// </summary>
    [DataContract]
    public class DTO_BaseItem   
    {
        DTO_UID uid_;    // Unique ID. Item's PLU can be used as uid. Option's Database key can also be used as uid

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inGUID"></param>
        internal DTO_BaseItem(DTO_UID inUID)
        {
            uid_ = new DTO_UID(inUID.AsString);
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public DTO_UID UID
        {
            get { return uid_;  }
            set { uid_ = value; }
        }
        #endregion
    }

    /// <summary>
    /// Describes a Unique Identifier. It can be both an string and a Integer
    /// </summary>
    [DataContract]
    public class DTO_UID
    {
        string uid_;

        /// <summary>
        /// 
        /// </summary>
        public DTO_UID()
        {
            uid_ = @"0";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStrID"></param>
        public DTO_UID(string inStrID)
        {
            uid_ = inStrID;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inIntID"></param>
        public DTO_UID(Int64 inIntID)
        {
            uid_ = Convert.ToString(inIntID);
        }

        #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public string AsString
        {
            get { return uid_; }
            set { uid_ = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public Int64 AsInteger
        {
            get { return Convert.ToInt64(uid_); }
            set { uid_ = Convert.ToString(value); }
        }
        #endregion
    }

    /// <summary>
    /// Describes a Base WebMate Order's Item
    /// </summary>
   [DataContract]
    public class DTO_BaseWebOrderItem : DTO_BaseItem
    {
       Decimal   basePrice_; // Price without any discount
       UInt32    qty_;         // Quantity

        /// <summary>
        /// 
        /// </summary>
       internal DTO_BaseWebOrderItem(
                        DTO_UID inUID,
                        Decimal   inBasePrice,
                        UInt32    inQty) : base(inUID) 
        {
            basePrice_ = inBasePrice;
             qty_         = inQty;
        }

       #region Public
        /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public Decimal BasePrice
        {
            get { return basePrice_;  }
            set { basePrice_ = value; }
        }

         /// <summary>
        /// 
        /// </summary>
        [DataMember]
        public UInt32 Qty
        {
            get { return qty_;  }
            set { qty_ = value; }
        }
       #endregion
    }

   /// <summary>
   /// Describes a WebMate Order's Item
   /// </summary>
   [DataContract]
   public class DTO_WebOrderItem : DTO_BaseWebOrderItem
   {
       UInt32                  discountID_;
       DTO_WebOrderItemOptions itemOptions_;
       DTO_WebOrderItemSides   itemSides_;

       internal DTO_WebOrderItem(
                        DTO_UID                 inUID,
                        Decimal                 inBasePrice,
                        UInt32                  inQty,
                        UInt32                  inDiscountID,
                        DTO_WebOrderItemOptions inItemOptions,
                        DTO_WebOrderItemSides   inItemSides) : base(inUID, inBasePrice, inQty)
        {
            ServiceLogger.Log(@"internal DTO_WebOrderItem( ... ) {");

            discountID_ = inDiscountID;
            itemOptions_ = createOptions(inItemOptions);
            itemSides_   = createSides(inItemSides);

            ServiceLogger.Log(@"}");
        }

       #region Public
       /// <summary>
       /// 
       /// </summary>
       [DataMember]
       public UInt32 DiscountID
       {
           get { return discountID_;  }
           set { discountID_ = value; }
       }

       /// <summary>
       /// 
       /// </summary>
       [DataMember]
       public DTO_WebOrderItemOptions ItemOptions
       {
           get { return itemOptions_;  }
           set { itemOptions_ = value; }
       }

       /// <summary>
       /// 
       /// </summary>
       [DataMember]
       public DTO_WebOrderItemSides ItemSides
       {
           get { return itemSides_;  }
           set { itemSides_ = value; }
       }
       #endregion

       #region Private
       /// <summary>
       /// 
       /// </summary>
       /// <param name="inItemOptions"></param>
       /// <returns></returns>
       DTO_WebOrderItemOptions createOptions(DTO_WebOrderItemOptions inItemOptions)
       {
           DTO_WebOrderItemOptions result = new DTO_WebOrderItemOptions(inItemOptions.Items);

           return result;
       }

       /// <summary>
       /// 
       /// </summary>
       /// <param name="inItemSides"></param>
       /// <returns></returns>
       DTO_WebOrderItemSides createSides(DTO_WebOrderItemSides inItemSides)
       {
           DTO_WebOrderItemSides result = new DTO_WebOrderItemSides(inItemSides.Items);

           return result;
       }
       #endregion
   }

   /// <summary>
   /// Describes a WebMate Order's Item Side
   /// </summary>
   [DataContract]
   public class DTO_WebOrderItemSide : DTO_BaseWebOrderItem
   {
       internal DTO_WebOrderItemSide(
                        DTO_UID inUID,
                        Decimal inBasePrice,
                        UInt32  inDiscountID,
                        UInt32  inQty) : base(inUID,inBasePrice,inQty)
       {
       }
   }

   /// <summary>
   /// Describes a WebMate Order's Item Option 
   /// </summary>
   [DataContract]
   public class DTO_WebOrderItemOption : DTO_BaseItem
   {
       internal DTO_WebOrderItemOption(DTO_UID inUID): base(inUID)
       {
       }
   }
}
