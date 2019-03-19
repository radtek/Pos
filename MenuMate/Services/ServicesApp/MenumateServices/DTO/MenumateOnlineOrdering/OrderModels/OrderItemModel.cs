using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
{
    [DataContract]
    public class OrderItemModel
    {
        [DataMember]
        public long OrderItemId { get; set; }
        [DataMember]
        public long OrderId { get; set; }
        [DataMember]                    
        public string Name { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]                        
        public long SiteItemId { get; set; }
        [DataMember]
        public double Price { get; set; }
        [DataMember]
        public string ItemUniqueId { get; set; }
        [DataMember]
        public List<OrderItemSizeModel> OrderItemSizes { get; set; }

    }
}
