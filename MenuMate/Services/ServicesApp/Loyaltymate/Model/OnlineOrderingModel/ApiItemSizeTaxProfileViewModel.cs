using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OnlineOrdering.Enum;

namespace OnlineOrdering.Model
{
    public class ApiItemSizeTaxProfileViewModel
    {
        public long ItemSizeTaxProfileId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public double Rate { get; set; }
        public TaxProfileType Type { get; set; }
        public int Priority { get; set; }
    }
}
