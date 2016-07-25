using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MMOrderItemsController.EventArgs;

namespace MMOrderItemsController
{
    public enum MMOrderItemsError { ServerNotAvailable = -1, InvalidData = -2 };

    public interface IOrderItemsController
    {

        /// <summary>
        /// 
        /// </summary>
        EventHandler<System.EventArgs> CloseCustomerDisplay
        {
            get;
            set;
        }
        
        /// <summary>
        /// 
        /// </summary>
        EventHandler<System.EventArgs> ResetCustomerDisplay
        {
            get;
            set;
        }

        /// <summary>
        /// 
        /// </summary>
        EventHandler<OrderItemsUpdatedEventArgs> OrderUpdated
        {
            get;
            set;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        void SetRefreshInterval(Int32 inInterval);

        /// <summary>
        /// 
        /// </summary>
        void StartRefresh();

        /// <summary>
        /// 
        /// </summary>
        void StopRefresh();
    }
}
