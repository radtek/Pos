using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class DiscountUsageInfo
    {
        [DataMember]
        public string DiscountCode { get; set; }
        [DataMember]
        public double DiscountAmount { get; set; }
    }
}
