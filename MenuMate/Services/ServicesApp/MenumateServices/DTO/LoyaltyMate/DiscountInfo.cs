using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using Loyaltymate.Enum;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class DiscountInfo
    {
        [DataMember]
        public long DiscountId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Code { get; set; }
        [DataMember]
        public DiscountType DiscountType { get; set; }
        [DataMember]
        public ImplicationType ImplicationType { get; set; }
        [DataMember]
        public decimal Value { get; set; }
        [DataMember]
        public decimal RoundToDecimalPlaces { get; set; }
        [DataMember]
        public DisplayOption DisplayAs { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public int DiscountGroup { get; set; }
        [DataMember]
        public bool IsMembersOnlyDiscount { get; set; }
        [DataMember]
        public bool IsMemberExemptDiscount { get; set; }
        [DataMember]
        public int? PriorityOrder { get; set; }
        [DataMember]
        public int? AppearanceOrder { get; set; }
        [DataMember]
        public bool IsCategoryFilterApplicable { get; set; }
        [DataMember]
        public decimal MaximumValue { get; set; }
        [DataMember]
        public int MinimumNumberOfItemsAllowed { get; set; }
        [DataMember]
        public int MaximumNumberOfItemsAllowed { get; set; }
        [DataMember]
        public int DailyUsageAllowedPerMember { get; set; }
        [DataMember]
        public ProductPriority ProductPriority { get; set; }
        [DataMember]
        public bool IsAutoMembersDiscount { get; set; }
    }
}
