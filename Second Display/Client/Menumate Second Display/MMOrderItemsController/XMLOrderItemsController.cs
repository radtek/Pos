using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Timers;
using System.Xml;
using System.IO;

using XMLManager;
using SharedMemory;
using MMOrderItemsController.EventArgs;
using MMOrderItem;

namespace MMOrderItemsController
{
    public class XMLOrderItemsController : IOrderItemsController
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        public XMLOrderItemsController()
        {
            Mode = XMLOrderItemsControllerMode.SharedMemory;

            initMemController();
            initTimer();

            if (mem_controller == null) // Server not ready
            {
                initReconnectTimer();
                startReconnectTimer(true);
            }


        }

        public enum XMLOrderItemsControllerMode { File, SharedMemory }

        /// <summary>
        /// 
        /// </summary>
        public XMLOrderItemsControllerMode Mode
        {
            get { return order_items_controller_mode; }
            set { order_items_controller_mode = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public EventHandler<System.EventArgs> CloseCustomerDisplay
        {
            get { return close_customer_display; }
            set { close_customer_display = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public EventHandler<System.EventArgs> ResetCustomerDisplay
        {
            get { return reset_customer_display; }
            set { reset_customer_display = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public EventHandler<OrderItemsUpdatedEventArgs> OrderUpdated
        {
            get { return order_items_updated;  }
            set { order_items_updated = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        public void SetRefreshInterval(Int32 inInterval)
        {
            order_items_timer.Interval = inInterval;
        }

        /// <summary>
        /// 
        /// </summary>
        public void StartRefresh()
        {
            startTimer(true);
        }

        /// <summary>
        /// 
        /// </summary>
        public void StopRefresh()
        {
            startTimer(false);
        }

        #endregion

        #region PROTECTED

        XMLOrderItemsControllerMode order_items_controller_mode;
        Timer order_items_timer;

        EventHandler<System.EventArgs> close_customer_display;
        EventHandler<System.EventArgs> reset_customer_display;
        EventHandler<OrderItemsUpdatedEventArgs> order_items_updated;

        const string SM_CLIENT_NAME = @"MM_CUSTOMER_DISPLAY";
        const Int64 SM_SERVER_CAPACITY = 65556; // 64KB 
        const Int64 SM_CLIENT_CAPACITY = 1024;  //  1KB

        // Server Side
        const Int64 SM_ORDER_READY_POSITION         = 0;
        const Int64 SM_ORDER_CLOSE_POSITION         = 1;
        const Int64 SM_ORDER_RESET_POSITION         = 2;
        const Int64 SM_ORDER_SEQ_NO_POSITION        = 3;
        const Int64 SM_ORDER_LARGE_FIGURES_POSITION = 4;
        const Int64 SM_ORDER_SIZE_POSITION          = 1020; //7;
        const Int64 SM_ORDER_POSITION               = 1024; //11;

        // Client Side
        const Int64 SM_BUSY_POSITION  = 0;

        bool server_has_reset;
        Int32 order_seq_no;
        SharedMemoryController mem_controller;
        Timer reconnect_timer;

        /// <summary>
        /// 
        /// </summary>
        protected void initMemController()
        {
            server_has_reset = false;
            order_seq_no = 0;

            SharedMemControllerBuilder smc_builder = SharedMemControllerBuilder.Instance;

            mem_controller = smc_builder.BuildClientController(
                                             SM_CLIENT_NAME, 
                                             SM_SERVER_CAPACITY, 
                                             SM_CLIENT_CAPACITY);

            setBusy( false );
        }

        /// <summary>
        /// 
        /// </summary>
        protected void initReconnectTimer()
        {
            // Create a timer with a 1 second interval.
            reconnect_timer = new System.Timers.Timer(3000);

            // Hook up the Elapsed event for the timer. 
            reconnect_timer.Elapsed += OnReconnectTimedEvent;

            startReconnectTimer(false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStart"></param>
        protected void startReconnectTimer(bool inStart)
        {
            reconnect_timer.Enabled = inStart;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void OnReconnectTimedEvent(Object source, ElapsedEventArgs e)
        {
            initMemController();

            if (mem_controller != null)
            {
                startReconnectTimer(false);
                StartRefresh();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void initTimer()
        {
            // Create a timer with a 1 second interval.
            order_items_timer = new System.Timers.Timer(100);

            // Hook up the Elapsed event for the timer. 
            order_items_timer.Elapsed += OnOrderItemsTimedEvent;

            StopRefresh();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void startTimer(bool inStart)
        {
            order_items_timer.Enabled = inStart;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void OnOrderItemsTimedEvent(Object source, ElapsedEventArgs e)
        {
            Timer timer = (Timer)source;
            timer.Enabled = false;

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            setBusy(true);

            XmlDocument xmlOrder = new XmlDocument();

            switch (Mode)
            {
                case XMLOrderItemsControllerMode.File: checkForOrderItemDataInFile(xmlOrder); break;
                case XMLOrderItemsControllerMode.SharedMemory: checkForOrderItemDataInMem(xmlOrder); break;

                default: ; break;
            }

            setBusy(false);

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            timer.Enabled = true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXmlOrder"></param>
        protected void checkForOrderItemDataInFile(XmlDocument inXmlOrder)
        {
            string orderFilename = createXMLOrderFileName();

            if (File.Exists(orderFilename))
            {
                openXMLOrderDoc(inXmlOrder, orderFilename);

                OrderItemsUpdatedEventArgs eventArgs = createOrderItemsUpdatedEventArgs(inXmlOrder);
                OnOrderItemsUpdated(eventArgs);

                closeXmlOrder(inXmlOrder);
                removeXmlOrderFile();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXmlOrder"></param>
        protected void checkForOrderItemDataInMem(XmlDocument inXmlOrder)
        {
            bool dataAvailable = false;

            //..................................................

            if( closeCustomerDisplay() )
            {
                notifyCloseCustomerDisplay();

                return;
            }

            //..................................................

            if( resetCustomerDisplay() )
            {
                notifyResetCustomerDisplay();

                return;
            }

            //...................................................

            if(mem_controller == null)
            {
                loadXMLServerNotAvailable(inXmlOrder);
                dataAvailable = true;
            }
            else
            {
                dataAvailable = loadXMLOrderStream(inXmlOrder);
            }

            if( dataAvailable )
            {
                OrderItemsUpdatedEventArgs eventArgs = createOrderItemsUpdatedEventArgs(inXmlOrder);
                OnOrderItemsUpdated(eventArgs);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void notifyCloseCustomerDisplay()
        {
            System.EventArgs eventArgs = new System.EventArgs();
            OnCloseCustomerDisplay(eventArgs);
        }

        /// <summary>
        /// 
        /// </summary>
        protected void notifyResetCustomerDisplay()
        {
            System.EventArgs eventArgs = new System.EventArgs();
            OnResetCustomerDisplay(eventArgs);

            order_seq_no = 0;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inXmlOrder"></param>
        /// <returns></returns>
        protected void closeXmlOrder(XmlDocument inXmlOrder)
        {
            inXmlOrder = null;
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void removeXmlOrderFile()
        {
            string orderFilename = createXMLOrderFileName();

            if (File.Exists(orderFilename))
            {
                File.Delete(orderFilename);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outXmlOrder"></param>
        /// <returns></returns>
        protected string createXMLOrderFileName()
        {
            return @"XMLOrder.xml";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outXmlOrder"></param>
        /// <param name="inXMLFileName"></param>
        protected void openXMLOrderDoc(XmlDocument outXmlOrder, string inXMLFileName)
        {
            XmlTextReader orderDocReader = new XmlTextReader(inXMLFileName);
            outXmlOrder.Load(orderDocReader);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected bool closeCustomerDisplay()
        {
            if (mem_controller == null)
            {
                return false;
            }

            //...............................................

            byte serverClose = mem_controller.ReadByte(SM_ORDER_CLOSE_POSITION);

            bool result = Convert.ToBoolean(serverClose);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected bool resetCustomerDisplay()
        {
            if(mem_controller == null)
            {
                return false;
            }

            //...............................................

            byte serverOrderReset = mem_controller.ReadByte(SM_ORDER_RESET_POSITION);

            bool result = !server_has_reset && Convert.ToBoolean(serverOrderReset);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderItems"></param>
        /// <returns></returns>
        protected OrderItemsUpdatedEventArgs createOrderItemsUpdatedEventArgs(XmlDocument inOrder)
        {
            OrderItemsUpdatedEventArgs result = new OrderItemsUpdatedEventArgs(inOrder);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEventArgs"></param>
        protected virtual void OnCloseCustomerDisplay(System.EventArgs inEventArgs)
        {
            EventHandler<System.EventArgs> handler = CloseCustomerDisplay;

            if (handler != null)
            {
                handler(this, inEventArgs);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEventArgs"></param>
        protected virtual void OnResetCustomerDisplay(System.EventArgs inEventArgs)
        {
            EventHandler<System.EventArgs> handler = ResetCustomerDisplay;

            if (handler != null)
            {
                handler(this, inEventArgs);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="eventArgs"></param>
        protected virtual void OnOrderItemsUpdated(OrderItemsUpdatedEventArgs inEventArgs)
        {
            EventHandler<OrderItemsUpdatedEventArgs> handler = OrderUpdated;

            if(handler != null)
            {
                handler(this, inEventArgs);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outXmlOrder"></param>
        /// <returns></returns>
        protected bool loadXMLOrderStream(XmlDocument outXmlOrder)
        {
            Int32 orderSeqNo = mem_controller.ReadInt32(SM_ORDER_SEQ_NO_POSITION);

            if(orderSeqNo == order_seq_no)
            {
                return false;
            }

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::
 
            bool result = false;

            //:::::::::::::::::::::::::::::::::::::::

            if( dataReady() )
            {
                // This is the way of setting it to false since the client shared memory controller has no access to 
                // the server shared memory space space
                server_has_reset = false;

                setBusy(true);

                //..........................................

                orderSeqNo = mem_controller.ReadInt32(SM_ORDER_SEQ_NO_POSITION);

                if( orderSeqNo != order_seq_no )
                {
                    order_seq_no = orderSeqNo;

                    int bytesRead = 0;
                    Int32 orderSize = mem_controller.ReadInt32(SM_ORDER_SIZE_POSITION);

                    byte[] orderBlock = new byte[orderSize];
                    bytesRead = mem_controller.ReadBlock(SM_ORDER_POSITION, orderSize, orderBlock);

                    if (bytesRead == orderSize)
                    {
                        MemoryStream orderStream = new MemoryStream(orderBlock);

                        outXmlOrder.Load(orderStream);
                    }
                    else
                    {
                        loadXMLReadMemError(outXmlOrder);
                    }

                    result = true;
                }

                //..........................................

                setBusy(false);
            }

            //:::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        protected void loadXMLServerNotAvailable(XmlDocument outXmlOrder)
        {
           string errorStr = Convert.ToString((int)MMOrderItemsError.ServerNotAvailable);

            loadXMLErrorMessage(outXmlOrder, errorStr, @"Menumate POS not available");
        }

        /// <summary>
        /// 
        /// </summary>
        protected void loadXMLReadMemError(XmlDocument outXmlOrder)
        {
            string errorStr = Convert.ToString((int)MMOrderItemsError.InvalidData);

            loadXMLErrorMessage(outXmlOrder, errorStr, @"Invalid data from Menumate POS");
        }

        /// <summary>
        /// 
        /// </summary>
        protected void loadXMLErrorMessage(XmlDocument outXmlOrder, string inOrderHandle, string inErrorMsg)
        {
            XmlNode root = XMLDocManager.CreateRoot(outXmlOrder, @"POSOrder");
            XMLDocManager.AddAttribute(outXmlOrder, root, @"handle", inOrderHandle);

            XmlNode scs = XMLDocManager.AddNodeChild(outXmlOrder, root, @"ServingCourses");

            XmlNode sc = XMLDocManager.AddNodeChild(outXmlOrder, scs, @"ServingCourse");
            XMLDocManager.AddAttribute(outXmlOrder, sc, @"name", @"");

            XmlNode its = XMLDocManager.AddNodeChild(outXmlOrder, sc, @"Items");

            XmlNode it = XMLDocManager.AddNodeChild(outXmlOrder, its, @"Item");
            XMLDocManager.AddAttribute(outXmlOrder, it, @"displayName", @"Customer Display Error");
            XMLDocManager.AddAttribute(outXmlOrder, it, @"value", @"");

            it = XMLDocManager.AddNodeChild(outXmlOrder, its, @"Item");
            XMLDocManager.AddAttribute(outXmlOrder, it, @"displayName", inErrorMsg);
            XMLDocManager.AddAttribute(outXmlOrder, it, @"value", @"");

            //.........................................................................

            XmlNode totals = XMLDocManager.AddNodeChild(outXmlOrder, root, @"Totals");

            XmlNode tdc = XMLDocManager.AddNodeChild(outXmlOrder, totals, @"TotalDiscount");
            XMLDocManager.AddAttribute(outXmlOrder, tdc, @"name", @"Total");
            XMLDocManager.AddAttribute(outXmlOrder, tdc, @"value", @"");

            XmlNode total = XMLDocManager.AddNodeChild(outXmlOrder, totals, @"Total");
            XMLDocManager.AddAttribute(outXmlOrder, total, @"name", @"Total");
            XMLDocManager.AddAttribute(outXmlOrder, total, @"value", @"");

            XmlNode change = XMLDocManager.AddNodeChild(outXmlOrder, totals, @"Change");
            XMLDocManager.AddAttribute(outXmlOrder, change, @"name", @"Change");
            XMLDocManager.AddAttribute(outXmlOrder, change, @"value", @"");

            XmlNode tsch = XMLDocManager.AddNodeChild(outXmlOrder, totals, @"TotalServiceCharge");
            XMLDocManager.AddAttribute(outXmlOrder, tsch, @"name", @"Total Service Charge");
            XMLDocManager.AddAttribute(outXmlOrder, tsch, @"value", @"");
            XMLDocManager.AddAttribute(outXmlOrder, tsch, @"visible", @"false");

            XmlNode ttx = XMLDocManager.AddNodeChild(outXmlOrder, totals, @"TotalTax");
            XMLDocManager.AddAttribute(outXmlOrder, ttx, @"name", @"Total Tax");
            XMLDocManager.AddAttribute(outXmlOrder, ttx, @"value", @"");
            XMLDocManager.AddAttribute(outXmlOrder, ttx, @"visible", @"false");
        }

        /// <summary>
        /// 
        /// </summary>
        protected bool dataReady()
        {
            Byte ready = mem_controller.ReadByte(SM_ORDER_READY_POSITION);

            return Convert.ToBoolean(ready);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inBusy"></param>
        protected void setBusy(bool inBusy)
        {
            byte busy = Convert.ToByte(inBusy);

            if (mem_controller != null)
            {
                mem_controller.Write(SM_BUSY_POSITION, busy);
            }
        }

        #endregion

        #region PRIVATE
        #endregion
    }
}
