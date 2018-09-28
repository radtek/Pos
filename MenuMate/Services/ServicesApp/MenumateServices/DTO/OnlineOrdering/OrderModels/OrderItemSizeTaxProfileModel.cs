using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using MenumateServices.DTO.Enum;

namespace MenumateServices.DTO.OnlineOrdering.OrderModels
{
    [DataContract]
    public class OrderItemSizeTaxProfileModel
    {
        [DataMember]
        public long OrderItemSizeTaxProfileId { get; set; }
        [DataMember]
        public long OrderItemSizeId { get; set; }
        [DataMember]
        public TaxProfileType TaxProfileType { get; set; }
        [DataMember]
        public double Percentage { get; set; }
        [DataMember]
        public double Value { get; set; }
        [DataMember]
        public long ItemSizeTaxProfileId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public double Rate { get; set; }
        [DataMember]
        public int Priority { get; set; }
        [DataMember]
        public long CompanyId { get; set; }
    }
}
