using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class GiftCardInfo
    {
        [DataMember]
        public double PointBalance { get; set; }
        [DataMember]
        public string GiftCardNumber { get; set; }
        [DataMember]
        public DateTime ExpiryDate { get; set; }
        [DataMember]
        public DateTime StartDate { get; set; }
        [DataMember]
        public string ResponseMessage { get; set; }
        [DataMember]
        public int StatusCode { get; set; }
    }
}
