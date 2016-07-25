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
    public class DTO_WebOrderAccount
    {
        #region PUBLIC
        [DataMember]
        public UInt32 Covers { get; set; }

        [DataMember]
        public string Name { get; set; }

        [DataMember]
        public string MemberNumber { get; set; }

        [DataMember]
        public DTO_WebOrderDelivery Delivery { get; set; }

        [DataMember]
        public DTO_WebOrderPayments Payments { get; set; }

        [DataMember]
        public string[] Comments { get; set; }

        [DataMember]
        public string Email { get; set; }

        [DataMember]
        public string OrderType { get; set; }

        [DataMember]
        public DTO_WebOrderItem[] OrderItems { get; set; }

        public DTO_WebOrderAccount()
        {
            OrderItems = new DTO_WebOrderItem[] { };
        }

        #endregion
    }
}
