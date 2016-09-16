using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class VoucherInfo
    {
        [DataMember]
        public string VoucherName { get; set; }
        [DataMember]
        public string DiscountCode { get; set; }
        [DataMember]
        public int NumberOfUsesRemaining { get; set; }
    }
}
