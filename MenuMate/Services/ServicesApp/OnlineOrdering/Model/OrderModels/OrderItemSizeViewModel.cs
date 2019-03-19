using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiOrderItemSizeViewModel
    {
        public long OrderItemSizeId { get; set; }//save in db
        public long OrderItemId { get; set; } ////save in db
        public string Name { get; set; }
        public long ItemSizeId { get; set; }
        public double Quantity { get; set; }
        public double MenuPrice { get; set; }
        public double Price { get; set; }
        public double PriceInclusive { get; set; }
        public double BasePrice { get; set; }
        public long ReferenceOrderItemSizeId { get; set; } //to send back
        public string ItemSizeUniqueId { get; set; }
        public List<ApiOrderItemSizeDiscountViewModel> OrderItemSizeDiscounts { get; set; }
        public List<ApiOrderItemSizeTaxProfileViewModel> OrderItemSizeTaxProfiles { get; set; }
    }
}
