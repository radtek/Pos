using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel.OrderModels
{
    public class ApiOrderItemSizeViewModel
    {
        public long OrderItemSizeId { get; set; }
        public long OrderItemId { get; set; }
        public string Name { get; set; }
        public long ItemSizeId { get; set; }
        public double Quantity { get; set; }
        public double MenuPrice { get; set; }
        public double Price { get; set; }
        public double PriceInclusive { get; set; }
        public double BasePrice { get; set; }
        public long ReferenceOrderItemSizeId { get; set; }
        public List<ApiOrderItemSizeDiscountViewModel> OrderItemSizeDiscounts { get; set; }
        public List<ApiOrderItemSizeTaxProfileViewModel> OrderItemSizeTaxProfiles { get; set; }
    }
}
