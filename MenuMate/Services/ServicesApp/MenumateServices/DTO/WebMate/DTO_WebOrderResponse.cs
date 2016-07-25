using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.WebMate.DTO
{
    /// <summary>
    /// 
    /// </summary>
    [DataContract]
    public enum ResponseCode
    {
        [EnumMember] Successful,
        [EnumMember] MenumateIsOffLine,
        [EnumMember] FailedToOpenOrder,
        [EnumMember] FailedToCommitOrder,
        [EnumMember] FailedToCreateOrderHeaderSection,
        [EnumMember] FailedToCreateOrderFromSection,
        [EnumMember] FailedToCreateOrderAccountSection,
        [EnumMember] FailedToCreateOrderPaymentSection,
        [EnumMember] FailedToCreateOrderCommentsSection,
        [EnumMember] FailedToCreateOrderItemSection,
        [EnumMember] FailedToCreateOrderItem,
        [EnumMember] FailedToCreateOrderItemSide,
        [EnumMember] FailedToCreateOrderItemOption,
        [EnumMember] FailedToRetrieveOrderHeaderSection,
        [EnumMember] FailedToRetrieveOrderFromSection,
        [EnumMember] FailedToRetrieveOrderAccountSection,
        [EnumMember] OrderGUIDDuplicate,
        [EnumMember] OrderEmpty,
        [EnumMember] FailedToGetCommittedOrderStatus,
        [EnumMember] FailedToCreateOrderEmailSection,
        [EnumMember] FailedToCreateOrderOrderTypeSection
    };

    /// <summary>
    /// 
    /// </summary>
    [DataContract]
    public class DTO_WebOrderResponse
    {
        #region PUBLIC
        [DataMember]
        public bool Succesful { get; set; }

        [DataMember]
        public string Message { get; set; }

        [DataMember]
        public string Description { get; set; }

        [DataMember]
        public ResponseCode Response { get; set; }
        #endregion
    }
}
