using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MMOrderItem
{
    public class BaseItem
    {
        /// <summary>
        /// Global unique ID 
        /// </summary>
        public Int32 ItemID { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public string Description { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public BaseItem()
        {
            ItemID = 0;
            Description = @"";
        }
    }
}
