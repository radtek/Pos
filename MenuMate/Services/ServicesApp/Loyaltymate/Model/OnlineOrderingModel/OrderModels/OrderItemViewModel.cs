﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Model.OnlineOrderingModel.OrderModels
{
    public class ApiOrderItemViewModel
    {
        public long OrderItemId { get; set; } ////save in db
        public long OrderId { get; set; }//save in db
        public string Name { get; set; }
        public string Description { get; set; }
        public long SiteItemId { get; set; }
        public double Price { get; set; }
        public List<ApiOrderItemSizeViewModel> OrderItemSizes { get; set; }

    }
}
