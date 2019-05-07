using System;
using System.Diagnostics;
using System.IO;
using System.ServiceModel.Activation;
using System.ServiceModel;
using System.Collections.Generic;
using MenumateServices.Tools;
using MenumateServices.Internal_Classes.MenumateOnlineOrdering;
using MenumateServices.DTO.MenumateOnlineOrdering;
using MenumateServices.DTO.MenumateOnlineOrdering.DBOrders;
using MenumateServices.DTO.MenumateOnlineOrdering.OrderModels;
using MenumateServices.DTO.MenumateOnlineOrdering.MenuModels;
using OnlineOrdering.Model.OrderModels;
using OnlineOrdering.Utility;

namespace MenumateServices.WCFServices
{
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    [ServiceBehavior(MaxItemsInObjectGraph = 65536000)]
    public class WCFServiceOnlineOrdering :IWCFServiceOnlineOrdering
    {
        private List<string> stringList = new List<string>();
        public OOLoyaltyResponse SyncTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings)
        {
            try
            {
                stringList.Add("Syncing Tax Settings of services at                " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltySite.Instance.SyncSiteTaxSettings(inSyndicateCode, siteTaxSettings, stringList);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in SaveMember of services is             " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearStringList();
            return null;
        }

        public OOLoyaltyResponse SyncMenu(string inSyndicateCode, SiteMenuInfo siteViewModel)
        {
            try
            {
                stringList.Add("Syncing Menu of services at                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltySite.Instance.SyncSiteMenu(inSyndicateCode, siteViewModel, stringList);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in SyncMenu of services is               " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearStringList();
            return null;
        }

        public void GetOrdersFromWeb(string inSyndicateCode, string orders)
        {
            try
            {
                stringList.Add("Getting Orders From Web of services at             " + DateTime.Now.ToString("hh:mm:ss tt"));
                stringList.Add("Received order string is:                          " + orders);
                List<ApiSiteOrderViewModel> siteOrderViewModel = new List<ApiSiteOrderViewModel>();
                siteOrderViewModel = JsonUtility.Deserialize<List<ApiSiteOrderViewModel>>(orders);
                var requestData = JsonUtility.Serialize<List<ApiSiteOrderViewModel>>(siteOrderViewModel);//just to test json
                stringList.Add("Received order json is:                            " + requestData);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                
                bool retVal = InsertOrdersToDB(ref siteOrderViewModel);
                stringList.Add("After InsertOrdersToDB function calling at         " + DateTime.Now.ToString("hh:mm:ss tt"));

                LoyaltySite.Instance.UpdateOrderStatus(inSyndicateCode, siteOrderViewModel, stringList);
                requestData = JsonUtility.Serialize<List<ApiSiteOrderViewModel>>(siteOrderViewModel);
                stringList.Add("After Updating order status to web json            " + requestData);                 
             }
            catch (Exception exc)
            {
                stringList.Add("Exception in GetOrdersFromWeb of services is       " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearStringList();
        }

        private bool InsertOrdersToDB(ref List<ApiSiteOrderViewModel> siteOrderViewModel)
        {
            bool result = false;
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try
            {
                stringList.Add("Inserting Orders To DB at                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction at                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        stringList.Add("inside transaction at                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                        onlineOrderDB.ProcessOrders(ref siteOrderViewModel, stringList);
                        stringList.Add("After ProcessOrders calling                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                        onlineOrderDB.transaction.Commit();
                        ServiceLogger.Log(@"after commit in InsertOrdersToDB(ApiSiteOrderViewModel ) with order ");
                    }
                }
                ServiceLogger.Log(@"outside using in dbWebOrderAccepted(string inOrderHandle) with order ");
            }
            catch (Exception e)
            {
                onlineOrderDB.RollbackTransaction();
                ServiceLogger.Log(@"In InsertOrdersToDB " + e.Message);
                EventLog.WriteEntry("IN Order Creation ", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 131, short.MaxValue);
                stringList.Add("Exception in InsertOrdersToDB                      " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
            //::::::::::::::::::::::::::::::::::::::::::::::
            WriteAndClearStringList();
            return result;

        }

        public OOLoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel)
        {
            try
            {
                stringList.Add("Posting Online Order Invoice Info at               " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltySite.Instance.PostOnlineOrderInvoiceInfo(inSyndicateCode, siteOrderModel, stringList);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in PostOnlineOrderInvoiceInfo is         " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            return null;
        }

        public LoyaltyOOResponse SyncOnlineOrderingDetails(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("Syncing Online Ordering Details at                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltyOnlineOrdering.Instance.GetOnlineOrderingInformation(inSyndicateCode, siteCode, stringList);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in SyncOnlineOrderingDetails is          " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearStringList();
            return null;
        }

        public bool UnsetOrderingDetails(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("Unset Ordering Details at                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltyOnlineOrdering.Instance.UnsetOrderingDetails(inSyndicateCode, siteCode, stringList);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in UnsetOrderingDetails is               " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearStringList();
            return false;
        }

        public void InsertWaiterTerminal(string terminalInfo)
        {
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try 
            {
                ApiWaiterAppPosTerminal apiWaiterAppPosTerminal = new ApiWaiterAppPosTerminal();
                apiWaiterAppPosTerminal = JsonUtility.Deserialize<ApiWaiterAppPosTerminal>(terminalInfo);
                var requestData = JsonUtility.Serialize<ApiWaiterAppPosTerminal>(apiWaiterAppPosTerminal);

                stringList.Add("Creating Waiter Terminal at                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction...");
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        stringList.Add("Checking If Waiter Terminal Exist              ");
                        bool IsTerminalExist = onlineOrderDB.CheckIfWaiterTerminalExist(apiWaiterAppPosTerminal.DeviceId);
                        if(!IsTerminalExist)
                        {
                            stringList.Add("Adding Waiter Terminal                         ");
                            onlineOrderDB.AddWaiterTerminal(apiWaiterAppPosTerminal.Name, apiWaiterAppPosTerminal.DeviceId);
                        }

                        stringList.Add("Checking If Waiter Staff Exist                 ");
                        bool IsWaiterStaffExist = onlineOrderDB.CheckIfWaiterStaffExist();
                        if (!IsWaiterStaffExist)
                        {
                            stringList.Add("Adding Waiter Staff                            ");
                            onlineOrderDB.AddWaiterStaff();
                        }
                            onlineOrderDB.transaction.Commit();
                            ServiceLogger.Log(@"after commit in CreateWaiterTerminal ");
                    }
                }
                ServiceLogger.Log(@"outside using in CreateWaiterTerminal");
            }
            catch (Exception exc)
            {
                onlineOrderDB.RollbackTransaction();
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in Inserting Waiter Terminal             " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            } 
        }

        public void CreateRequestForAppZed(string zedRequest)
        {
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try
            {
                ApiWaiterAppPosTerminal apiWaiterAppPosTerminal = new ApiWaiterAppPosTerminal();
                apiWaiterAppPosTerminal = JsonUtility.Deserialize<ApiWaiterAppPosTerminal>(zedRequest);
                var requestData = JsonUtility.Serialize<ApiWaiterAppPosTerminal>(apiWaiterAppPosTerminal);
                stringList.Add("Creating Request For AppZed                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                
                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction...");
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        stringList.Add("Checking If Zed Request Exist                  ");
                        bool IsAppZedRowExist = onlineOrderDB.CheckIfZedRequestExist(apiWaiterAppPosTerminal.Name);

                        if (!IsAppZedRowExist)
                        {
                            stringList.Add("Inserting in AppZed Row                        ");
                            onlineOrderDB.InsertAppZedRow(apiWaiterAppPosTerminal.Name, apiWaiterAppPosTerminal.DeviceId);
                        }
                        onlineOrderDB.transaction.Commit();
                        ServiceLogger.Log(@"after commit in CreateRequestForAppZed ");
                    }
                }
                ServiceLogger.Log(@"outside using in CreateRequestForAppZed");


            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in Creating Request For AppZed           " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            }
        }

        public OnlineOrderingDetails GetOnlineOrderingDetailsBySiteCode(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("Syncing Online Ordering Details at                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltyOnlineOrdering.Instance.GetOnlineOrderingDetails(inSyndicateCode, siteCode, stringList);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in GetOnlineOrderingDetailsBySiteCode is          " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearStringList();
            return null;
        }
        
        private void WriteAndClearStringList()
        {
            // Check folder and file name should remain same as in previous releases
            stringList.Add("======================================================================================================================");
            FileWriter.WriteToFile(stringList, "Online Ordering Logs", "OnlineOrderingLogs ");
            stringList.Clear();
        }
    }
}
