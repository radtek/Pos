using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FirebirdSql.Data.FirebirdClient;

using MenumateServices.WebMate.DTO;
using System.Diagnostics;

namespace MenumateServices.WebMate.InternalClasses
{
    public class WebOrderDBAccessProcess
    {
        #region PUBLIC

        public event EventHandler<WebOrderDBAccessEventArgs> HasCompleted;
        public event EventHandler<WebOrderDBAccessEventArgs> HasFailed;
        public event EventHandler<WebOrderDBAccessEventArgs> DeadLock;

        /// <summary>
        /// 
        /// </summary>
        public WebOrderDBAccessProcess()
        {
            order_uri = @"";

            started = false;
        }

        /// <summary>
        /// 
        /// </summary>
        public string OrderURI
        {
            get { return order_uri; }
            set { order_uri = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public WebOrder WebOrder
        {
            get { return web_order;  }
            set { web_order = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool Started
        {
            get { return started; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime PollTime
        {
            get { return poll_time; }
        }

        /// <summary>
        /// 
        /// </summary>
        public DateTime StartTime
        {
            get { return start_time; }
        }

        /// <summary>
        /// 
        /// </summary>
        public void Start()
        {
            try
            {
                started = true;
                start_time = DateTime.Now;

                dbSaveOrder(web_order);

                OnComplete(createEventArgs(WebOrderDBAccessResult.Succesful, @""));
            }
            catch(FbException fbex)
            {
                if (fbex.ErrorCode == DEAD_LOCK_CODE) // dead lock
                {
                    string errorMsg = string.Format(@"{0}. Error code: {1}", fbex.Message, fbex.ErrorCode);
                    OnDeadLock(createEventArgs(WebOrderDBAccessResult.DeadLock, errorMsg));
                }
                else
                {
                    string errorMsg = string.Format(@"{0}. Error code: {1}", fbex.Message, fbex.ErrorCode);
                    OnFail(createEventArgs(WebOrderDBAccessResult.FailedToAccess, errorMsg));
                }
            }
            catch (Exception exc)
            {
                OnFail(createEventArgs(WebOrderDBAccessResult.FailedToAccess, exc.Message));
            }
        }

        #endregion

        #region PROTECTED

        const int DEAD_LOCK_CODE = 335544336;

        protected string order_uri;
        protected WebOrder web_order;

        bool started;
        DateTime poll_time;
        DateTime start_time;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderURI"></param>
        /// <param name="inResult"></param>
        /// <returns></returns>
        protected WebOrderDBAccessEventArgs createEventArgs(WebOrderDBAccessResult inResult, string inErrorMsg)
        {
            return new WebOrderDBAccessEventArgs(inResult, inErrorMsg);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        protected void OnComplete(WebOrderDBAccessEventArgs args)
        {
            EventHandler<WebOrderDBAccessEventArgs> handler = HasCompleted;

            if (handler != null)
            {
                handler(this, args);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        protected void OnFail(WebOrderDBAccessEventArgs args)
        {
            EventHandler<WebOrderDBAccessEventArgs> handler = HasFailed;

            if (handler != null)
            {
                handler(this, args);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void OnDeadLock(WebOrderDBAccessEventArgs args)
        {
            EventHandler<WebOrderDBAccessEventArgs> handler = DeadLock;

            if (handler != null)
            {
                handler(this, args);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="webOrderXML"></param>
        void dbSaveOrder(WebOrder inOrder)
        {
            WebOrderDB webOrderDB = new WebOrderDB();

            try
            {
                webOrderDB.BeginTransaction();
                //check webmate enabled here or not..
                bool checkWebmateIsEnabled = checkWebmateEnabledOrNot(webOrderDB);
                if (checkWebmateIsEnabled)
                {
                    int weborderKey = dbSaveOrderInfo(inOrder, webOrderDB);
                    int menu_key = getMenuKeyFormChit(webOrderDB);
                    dbSaveOrderItems(weborderKey, inOrder, webOrderDB, menu_key);
                }
                else
                {
                   SetWebmateForMessage(webOrderDB); 
                }
                ServiceLogger.Log("In dbSaveOrder(WebOrder inOrder) before calling webOrderDB.EndTransaction() for order with GUID " + inOrder.Handle);
                webOrderDB.EndTransaction();
            }
            catch (Exception e)
            {
                ServiceLogger.Log("In dbSaveOrder(WebOrder inOrder) before calling webOrderDB.RollbackTransaction() with GUID " + inOrder.Handle);
                webOrderDB.RollbackTransaction();
                ServiceLogger.Log(@"WebOrderDBAccessProcess.dbSaveOrder(WebOrder inOrder) ROLLBACK");
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 120, short.MaxValue);
                throw;
            }
       }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        int dbSaveOrderInfo(WebOrder inOrder, WebOrderDB inWebOrderDB)
        {
            return inWebOrderDB.SaveOrderInfo(createWebOrderDBInfo(inOrder));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        void dbSaveOrderItems(int inWebOrderKey, WebOrder inOrder, WebOrderDB inWebOrderDB, int inMenuKey)
        {
            inWebOrderDB.SaveOrderItems(
                                inWebOrderKey,
                                createWebOrderDBItems(inOrder, inWebOrderDB, inMenuKey), inMenuKey, inOrder.AccountSection.Delivery.Phone);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        WebOrderDBInfo createWebOrderDBInfo(WebOrder inOrder)
        {
            WebOrderDBInfo result = new WebOrderDBInfo();

            //....................................................

            try
            {
                DTO_WebOrderHeader headerSection = inOrder.GetCompleteHeaderSection();
                DTO_WebOrderAccount accountSection = inOrder.GetCompleteAccountSection();

                result.GUID = inOrder.Handle;
                result.Name = accountSection.Name;
                result.StoreName = headerSection.StoreName;
                result.OrderDate = headerSection.OrderDate;
                result.ExpectedDate = headerSection.ExpectedDate;
                result.Status = WebOrderDB.WebOrderStatus.ewosReceived;

                result.Recipient = accountSection.Delivery.Recipient;
                result.StreetNo = accountSection.Delivery.StreetNo;
                result.StreetName = accountSection.Delivery.StreetName;
                result.Suburb = accountSection.Delivery.Suburb;
                result.City = accountSection.Delivery.City;
                result.Country = accountSection.Delivery.Country;
                result.Note = accountSection.Delivery.Note;
                result.Phone = accountSection.Delivery.Phone;

                result.Comments = accountSection.Comments;
                result.PaymentRequired = accountSection.Payments.PaymentRequired;

                result.OrderTotal = headerSection.OrderTotal;
                result.Email = accountSection.Email;
                result.OrderType = accountSection.OrderType;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 301, short.MaxValue);
            }
            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        WebOrderDBItem[] createWebOrderDBItems(WebOrder inOrder, WebOrderDB inWebOrderDB, int inMenuKey) //changes for menu_key
        {
            List<WebOrderDBItem> result = new List<WebOrderDBItem>();

            //....................................................

            try
            {
                DTO_WebOrderAccount accountSection = inOrder.GetCompleteAccountSection();
                DTO_WebOrderHeader headerSection = inOrder.GetCompleteHeaderSection();
                foreach (DTO_WebOrderItem orderItem in accountSection.OrderItems)
                {
                    WebOrderDBItem item = new WebOrderDBItem();
                    item.ThirdPartyCodeKey = dbGetThirdPartyCodeKey(orderItem.UID, inWebOrderDB, inMenuKey); //changes for menu_key
                    item.Qty = orderItem.Qty;
                    item.Price = orderItem.BasePrice;
                    item.PriceLevel0 = orderItem.BasePrice;
                    item.PriceLevel1 = orderItem.BasePrice;
                    item.TabName = accountSection.Name;
                    item.TimeStamp = headerSection.OrderDate;
                    item.Note = orderItem.Note;

                    //--------------------------------------------------------
                    // Options are ignored in this version of Webmate
                    //--------------------------------------------------------
                    /*
                    foreach (DTO_WebOrderItemOption orderItemOption in orderItem.ItemOptions)
                    {
                        WebOrderDBItemOption option = new WebOrderDBItemOption();
                        option.OptionKey = Convert.ToInt32(orderItemOption.UID);
                        item.ItemOptions.Add(option);                   
                    }
                    */

                    foreach (DTO_WebOrderItemSide orderItemSide in orderItem.ItemSides)
                    {
                        WebOrderDBItemSide side = new WebOrderDBItemSide();
                        side.ThirdPartyCodeKey = dbGetThirdPartyCodeKey(orderItemSide.UID, inWebOrderDB, inMenuKey); //changes for menu_key
                        side.Qty = orderItemSide.Qty;
                        side.Price = orderItemSide.BasePrice;
                        side.PriceLevel0 = orderItemSide.BasePrice;
                        side.PriceLevel1 = orderItemSide.BasePrice;
                        side.TimeStamp = headerSection.OrderDate;
                        side.TransactionNumber = orderItemSide.UID;
                        side.TabName = accountSection.Name;
                        side.Note = orderItem.Note;
                        item.ItemSides.Add(side);
                    }

                    result.Add(item);
                }
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 302, short.MaxValue);
            }

            //....................................................

            return result.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inThirdPartyCode"></param>
        /// <returns></returns>
        protected Int32 dbGetThirdPartyCodeKey(string inThirdPartyCode, WebOrderDB inWebOrderDB, int inMenukey)
        {           
            Int32 result = inWebOrderDB.GetThirdPartyCodeKey(inThirdPartyCode, inMenukey);

            if (result > 0)
            {
                return result;
            }

            throw new Exception(string.Format(@"Invalid Third Party Code: {0}", inThirdPartyCode));
        }

        public int getMenuKeyFormChit(WebOrderDB webOrderDB)
        {
            int menu_key = 0;
            try
            {
                DTO_WebOrderAccount accountSection = WebOrder.GetCompleteAccountSection();
                string inorderType = accountSection.OrderType;
                if (inorderType != "" || inorderType != null)
                {
                    menu_key = webOrderDB.GetPOSChitDetails(inorderType);
                }
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 303, short.MaxValue);
            }
            return menu_key;  
        }

        //check and update for Webmate..
        public bool checkWebmateEnabledOrNot(WebOrderDB webOrderDB)
        {
            bool _isEnableOrNot = webOrderDB.CheckWemateInterface();            
            return _isEnableOrNot;
        }
        public void SetWebmateForMessage(WebOrderDB webOrderDB)
        {
            try
            {
                webOrderDB.SetForWemateInterfaceMessage();
            }
            catch (Exception e)
            {
              EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 304, short.MaxValue);
            }          
        }

        #endregion
    }
}
