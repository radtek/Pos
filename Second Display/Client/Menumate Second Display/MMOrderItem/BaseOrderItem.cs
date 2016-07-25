using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MMOrderItem
{
    public class BaseOrderItem : BaseItem
    {
        /// <summary>
        /// Unique ID within an order
        /// </summary>
        public Int32 OrderItemID { get; set; }

        /// <summary>
        /// Final order item's price after discounts, service charge and taxes,
        /// </summary>
        public Decimal Price { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public float Qty { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public BaseOrderItem() : base()
        {
            OrderItemID = 0;
            Price = 0.0M;
            Qty = 0.0F;
        }
    }
}
