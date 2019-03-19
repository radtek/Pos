using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OnlineOrdering.Model.OrderModels
{
    public class ApiOrderItemViewModel
    {
        public long OrderItemId { get; set; } ////save in db
        public long OrderId { get; set; }//save in db
        public string Name { get; set; }
        public string Description { get; set; }
        public long SiteItemId { get; set; }
        public double Price { get; set; }
        public string ItemUniqueId { get; set; }
        public List<ApiOrderItemSizeViewModel> OrderItemSizes { get; set; }
    }
}
