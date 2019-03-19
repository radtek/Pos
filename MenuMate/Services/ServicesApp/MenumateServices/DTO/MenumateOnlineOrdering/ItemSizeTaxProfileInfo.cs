using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class ItemSizeTaxProfileInfo
    {
        [DataMember]
        public long ItemSizeTaxProfileId { get; set; }
        [DataMember]
        public long TaxProfileId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public double Rate { get; set; }
        [DataMember]
        public TaxProfileType Type { get; set; }
        [DataMember]
        public int Priority { get; set; }
    }
}
