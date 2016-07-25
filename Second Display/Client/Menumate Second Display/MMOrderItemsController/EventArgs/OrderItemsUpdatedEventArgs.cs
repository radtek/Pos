using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;

using MMOrderItem;

namespace MMOrderItemsController.EventArgs
{
    /// <summary>
    /// 
    /// </summary>
    public class OrderItemsUpdatedEventArgs : System.EventArgs
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderItemList"></param>
        public OrderItemsUpdatedEventArgs(XmlDocument inPOSOrder) : base()
        {
            pos_order = inPOSOrder;
        }

        /// <summary>
        /// 
        /// </summary>
        public XmlDocument POSOrder 
        {
            get { return pos_order; }
        }

        #endregion

        #region PROTECTED

        XmlDocument pos_order;

        #endregion
    }
}
