using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class ItemSizeInfo
    {
        [DataMember]
        public long ItemSizeId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public bool IsWeighted { get; set; }
        [DataMember]
        public double Price { get; set; }
        [DataMember]
        public bool IsFree { get; set; }
        [DataMember]
        public double PointsPercentage { get; set; }
        [DataMember]
        public double PointsPrice { get; set; }
        [DataMember]
        public bool CanBePaidUsingPoints { get; set; }
        [DataMember]
        public string OrderingUniqueId { get; set; }
        [DataMember]
        public string ThirdPartyId { get; set; }
        [DataMember]
        public int DefaultPatronCount { get; set; }
        [DataMember]
        public List<ItemSizeTaxProfileInfo> ItemSizeTaxProfiles { get; set; }
    }
}
