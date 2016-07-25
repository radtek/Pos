using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Timers;
using System.Xml;

using MMOrderItemsController;
using MMOrderItemsController.EventArgs;
using MMOrderItem;

namespace MMOrderItemsManager
{
    public class OrderItemsManager
    {
        #region PUBLIC

        public enum OrderManagerType { MenumatePOS };

        /// <summary>
        /// 
        /// </summary>
        public OrderItemsManager()
        {
            controllers = new List<IOrderItemsController>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        /// <param name="inManagerType"></param>
        public void SetRefreshInterval(Int32 inInterval, OrderManagerType inManagerType)
        {
            getController(inManagerType).SetRefreshInterval(inInterval);
        }

        public void SetCloseCustomerDisplayHandler(EventHandler<EventArgs> inHandler, OrderManagerType inManagerType)
        {
            getController(inManagerType).CloseCustomerDisplay += inHandler;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHandler"></param>
        /// <param name="inManagerType"></param>
        public void SetResetCustomerDisplayHandler(EventHandler<EventArgs> inHandler, OrderManagerType inManagerType)
        {
            getController(inManagerType).ResetCustomerDisplay += inHandler;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHandler"></param>
        public void SetOrderItemsUpdatedHandler(EventHandler<OrderItemsUpdatedEventArgs> inHandler, OrderManagerType inManagerType)
        {
            getController(inManagerType).OrderUpdated += inHandler;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        public void StartRefresh(OrderManagerType inManagerType)
        {
            getController(inManagerType).StartRefresh();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        public void StopRefresh(OrderManagerType inManagerType)
        {
            getController(inManagerType).StopRefresh();
        }

        #endregion

        #region PROTECTED

        List<IOrderItemsController> controllers;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        /// <returns></returns>
        protected IOrderItemsController getController(OrderManagerType inManagerType)
        {
            int index = (int)inManagerType;

            try
            {
                return controllers[index];
            }
            catch
            {
                return createController(inManagerType);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        /// <returns></returns>
        protected IOrderItemsController createController(OrderManagerType inManagerType)
        {
            IOrderItemsController result;

            switch(inManagerType)
            {
                case OrderManagerType.MenumatePOS: result = new XMLOrderItemsController(); break;

                default: result = new XMLOrderItemsController(); break;
            }

            controllers.Add(result);

            return result;
        }

        #endregion

        #region PRIVATE
        #endregion
    }
}
