using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.OnlineOrdering.OrderModels
{
    [DataContract]
    public class OrderItemSizeDiscountModel
    {
        [DataMember]
        public long OrderItemSizeDiscountId { get; set; }
        [DataMember]
        public long OrderItemSizeId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public double Value { get; set; }
        [DataMember]
        public string Code { get; set; }
    }
}
