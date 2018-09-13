using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.OnlineOrdering.OrderModels
{
    [DataContract]
    public class OrderItemSizeModel
    {
        [DataMember]
        public long OrderItemSizeId { get; set; }
        [DataMember]
        public long OrderItemId { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public long ItemSizeId { get; set; }
        [DataMember]
        public double Quantity { get; set; }
        [DataMember]
        public double MenuPrice { get; set; }
        [DataMember]
        public double Price { get; set; }
        [DataMember]
        public double PriceInclusive { get; set; }
        [DataMember]
        public double BasePrice { get; set; }
        [DataMember]
        public long ReferenceOrderItemSizeId { get; set; }
        [DataMember]
        public string ItemSizeUniqueId { get; set; }
        [DataMember]
        public List<OrderItemSizeDiscountModel> OrderItemSizeDiscounts { get; set; }
        [DataMember]
        public List<OrderItemSizeTaxProfileModel> OrderItemSizeTaxProfiles { get; set; }
    }
}
