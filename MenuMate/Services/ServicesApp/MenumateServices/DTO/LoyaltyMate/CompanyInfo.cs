using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class CompanyInfo
    {
        [DataMember]
        public List<TierLevelInfo> TierLevels { get; set; }
        [DataMember]
        public List<DiscountInfo> Discounts { get; set; }
        [DataMember]
        public bool HasPocketVouchersAvailable { get; set; }
        [DataMember]
        public bool HasGiftCardsAvailable { get; set; }
    }
}
