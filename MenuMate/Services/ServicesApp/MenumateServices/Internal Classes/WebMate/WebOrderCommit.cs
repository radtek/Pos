using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;

using System.IO;
using System.Diagnostics;

using MenumateServices.WebMate.DTO;
using MenumateServices.Tools;
using XMLManager;
using FirebirdSql.Data.FirebirdClient;

namespace MenumateServices.WebMate.InternalClasses
{
    /// <summary>
    /// 
    /// </summary>
    public class WebOrderCommit : WebOrderResponsive
    {
        private static Properties.Settings settings = Properties.Settings.Default;

        WebOrderXMLManager web_order_xml_manager;

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public WebOrderCommit() : base()
        {
            Init();
        }

        /// <summary>
        /// 
        /// </summary>
        public void Init()
        {
            try
            {
                web_order_xml_manager = new WebOrderXMLManager();
                web_order_xml_manager.Init();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 120, short.MaxValue);
            }

        }

        /// <summary>
        /// Saves an new incomplete order. The order DOES NOT go to the DB.
        /// </summary>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        public void OpenOrder(string inHandle)
        {
            try
            {
                web_order_xml_manager.OpenIncomplete(inHandle);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 121, short.MaxValue);
            }            
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        public void LoadOrder(string inOrderHandle, WebOrder outWebOrder)
        {
            try
            {
                web_order_xml_manager.LoadIncomplete(inOrderHandle, outWebOrder);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 122, short.MaxValue);
            }            
        }

        /// <summary>
        /// Commits a complete order. The order GOES to the DB.
        /// </summary>
        /// <param name="inOrder"></param>
        /// <returns></returns>
        public void CommitOrder(WebOrder inWebOrder)
        {
            try
            {
                dbValidateWebOrder(inWebOrder);
                web_order_xml_manager.SaveComplete(inWebOrder);
                web_order_xml_manager.RemoveIncomplete(inWebOrder);
            }
            catch(Exception e)
            {
                web_order_xml_manager.RemoveIncomplete(inWebOrder);
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 123, short.MaxValue);
                throw;
            }

        }

        /// <summary>
        /// IS BEING NOT USED
        /// </summary>
        /// <param name="outWebOrder"></param>
        /// <returns></returns>
        public void RemoveOrder(WebOrder inWebOrder)
        {            
            try
            {
                web_order_xml_manager.RemoveIncomplete(inWebOrder);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 124, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="inOrderHeader"></param>
        /// <returns></returns>
        public void SetHeaderSection(string inWebOrderHandle, DTO_WebOrderHeader inOrderHeader)
        {
            try
            {
                web_order_xml_manager.SetIncompleteHeaderSection(inWebOrderHandle, inOrderHeader);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 125, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrderHeader"></param>
        public void LoadHeaderSection(string inWebOrderHandle, DTO_WebOrderHeader outOrderHeader)
        {            
            try
            {
                web_order_xml_manager.LoadIncompleteHeaderSection(inWebOrderHandle, outOrderHeader);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 126, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="inOrderFrom"></param>
        /// <returns></returns>
        public void SetFromSection(string inWebOrderHandle, DTO_WebOrderFrom inOrderFrom)
        {            
            try
            {
                web_order_xml_manager.SetIncompleteFromSection(inWebOrderHandle, inOrderFrom);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 127, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrderFrom"></param>
        public void LoadFromSection(string inWebOrderHandle, DTO_WebOrderFrom outOrderFrom)
        {
            try
            {
                web_order_xml_manager.LoadIncompleteFromSection(inWebOrderHandle, outOrderFrom);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 128, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="intOrderAccount"></param>
        /// <returns></returns>
        public void SetAccountSection(string inWebOrderHandle, DTO_WebOrderAccount intOrderAccount)
        {            
            try
            {
                web_order_xml_manager.SetIncompleteAccountSection(inWebOrderHandle, intOrderAccount);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 129, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="outOrderAccount"></param>
        public void LoadAccountSection(string inWebOrderHandle, DTO_WebOrderAccount outOrderAccount)
        {            
            try
            {
                web_order_xml_manager.LoadIncompleteAccountSection(inWebOrderHandle, outOrderAccount);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 130, short.MaxValue);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inStoreName"></param>
        /// <param name="inOrderHandle"></param>
        /// <param name="outStatus"></param>
        public void GetCommittedWebOrderStatus(
                                           string inSiteName,
                                           string inStoreName,
                                           string inOrderHandle,
                                       out WebOrderStatus outStatus)
        {
            if (web_order_xml_manager.WebOrderWaitingToBeAccepted(
                                        inSiteName,
                                        inStoreName,
                                        inOrderHandle))
            {
                outStatus = WebOrderStatus.Committed;
            }
            else if (web_order_xml_manager.WebOrderIncomplete(
                                                    inSiteName,
                                                    inStoreName,
                                                    inOrderHandle))
            {
                outStatus = WebOrderStatus.Incomplete;
            }
            else if (web_order_xml_manager.WebOrderNotAccepted(
                                           inSiteName,
                                           inStoreName,
                                           inOrderHandle))
            {
                outStatus = WebOrderStatus.NotAccepted;
            }
            else if (dbWebOrderAccepted(inOrderHandle))
            {
                outStatus = WebOrderStatus.Accepted;
            }
            else
            {
                outStatus = WebOrderStatus.Unknown;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        //public bool dbWebOrderAccepted(string inOrderHandle)
        //{
        //    bool result = false;
        //    //::::::::::::::::::::::::::::::::::::::::::::::
        //    try
        //    {
        //        WebOrderDB webOrderDB = new WebOrderDB();
        //        webOrderDB.BeginTransaction();
        //        result = webOrderDB.WebOrderAccepted(inOrderHandle);
        //        webOrderDB.EndTransaction();
        //    }
        //    catch (Exception e)
        //    {
        //        EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 131, short.MaxValue);
        //    }
        //    //::::::::::::::::::::::::::::::::::::::::::::::
        //    return result;
        //}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        public bool dbWebOrderAccepted(string inOrderHandle)
        {
            bool result = false;
            //::::::::::::::::::::::::::::::::::::::::::::::
            try
            {
                WebOrderDB webOrderDB = new WebOrderDB();
                bool isProperConnection = false;
                using (webOrderDB.connection_ = webOrderDB.BeginConnection())
                {
                    //check webmate enabled here or not..
                    using (webOrderDB.transaction_ = webOrderDB.BeginFBtransaction())
                    {
                        isProperConnection = true;
                        result = webOrderDB.WebOrderAccepted(inOrderHandle);
                        webOrderDB.transaction_.Commit();
                    }
                }
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 131, short.MaxValue);
            }
            //::::::::::::::::::::::::::::::::::::::::::::::
            return result;
        }
        #endregion

        #region Private

        /// <summary>
        /// Returns true if Menumate.exe is open
        /// NOT USED!!!!
        /// </summary>
        /// <returns></returns>
        bool menumateIsUp()
        {
            return processIsOpen(@"menumate.exe");
        }

        /// <summary>
        /// Tries to open Menumate.exe
        /// NOT USED!!!!
        /// </summary>
        void tryToOpenMenumate()
        {
            if (menumateMayBeWakenUp())
            {
                openProcess(Path.Combine(settings.MenumateLocation, settings.MenumateName));
            }
        }

        /// <summary>
        /// TODO: Returns whether Menumate.exe app may be waken up or not. e.g. 
        /// It was closed on purpose for maintenance therefore it cannot be waken up
        /// NOT USED!!!!
        /// </summary>
        /// <returns></returns>
        bool menumateMayBeWakenUp()
        {
            return true;
        }

        /// <summary>
        /// Creates a Web Order's "Menumate off-line" response
        /// NOT USED!!!!
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderResponse createResponseMenumateOffLine()
        {
            return createResponseError(@"Menumate is off-line", @"", ResponseCode.MenumateIsOffLine);
        }

        /// <summary>
        /// Returns true if a proccess is open
        /// NOT USED!!!!
        /// </summary>
        /// <param name="inProcessName"></param>
        /// <returns></returns>
        bool processIsOpen(string inProcessName)
        {
            foreach (Process process in Process.GetProcesses())
            {
                if (process.ProcessName.Contains(inProcessName))
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Opens a process
        /// NOT USED!!!!
        /// </summary>
        /// <param name="inProcessURI"></param>
        /// <returns></returns>
        bool openProcess(string inProcessURI)
        {
            try
            {
                Process.Start(inProcessURI);

                return true;
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 132, short.MaxValue);
                return false;
            }
        }

        /// <summary>
        /// Validates an order in the database by checking out the Member Number
        /// </summary>
        /// <param name="inOrder"></param>
        void dbValidateWebOrder(WebOrder inOrder)
        {
            try
            {
                dbValidateWebOrderSiteID(inOrder.FromSection.SiteID);
                dbValidateWebOrderSiteName(inOrder.FromSection.SiteName);
                dbValidateWebOrderItems(inOrder.AccountSection.OrderItems);
            }
            catch (Exception e)
            {
                EventLog.WriteEntry("IN Application Exception Create", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 300, short.MaxValue);
            }

            //WebOrderDBValidate.Instance.ValidateOrder(inOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteID"></param>
        void dbValidateWebOrderSiteID(string inSiteID)
        {
            WebOrderDB webOrderDB = new WebOrderDB();

            if (!webOrderDB.ValidWebOrderSiteID(inSiteID))
            {
                throw new ExceptionOrderInvalidSiteID(@"Cannot commit an order with an invalid site ID");
            }

            //WebOrderDBValidate.Instance.ValidateOrder(inOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        void dbValidateWebOrderSiteName(string inSiteName)
        {
            if (inSiteName.Length == 0)
            {
                throw new ExceptionOrderEmptySiteName(@"Cannot commit an order with an empty site name");
            }

            //WebOrderDBValidate.Instance.ValidateOrder(inOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderItems"></param>
        void dbValidateWebOrderItems(DTO_WebOrderItem[] inOrderItems)
        {
            if (inOrderItems.Length == 0)
            {
                throw new ExceptionOrderEmpty(@"Cannot commit an empty order");
            }

            //WebOrderDBValidate.Instance.ValidateOrder(inOrder);
        }

        #endregion
    }
}
