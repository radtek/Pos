using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel.OrderModels
{
    public class ApiOrderItemSizeDiscountViewModel
    {
        public long OrderItemSizeDiscountId { get; set; }
        public long OrderItemSizeId { get; set; }
        public string Name { get; set; }
        public double Value { get; set; }
        public string Code { get; set; }
    }
}
