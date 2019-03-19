using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiOrderItemSizeTaxProfileViewModel
    {
        public long OrderItemSizeTaxProfileId { get; set; }
        public long OrderItemSizeId { get; set; }
        public TaxProfileType TaxProfileType { get; set; }
        public double Percentage { get; set; }
        public double Value { get; set; }
        public long ItemSizeTaxProfileId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public double Rate { get; set; }
        public int Priority { get; set; }
        public long CompanyId { get; set; }
    }
}
