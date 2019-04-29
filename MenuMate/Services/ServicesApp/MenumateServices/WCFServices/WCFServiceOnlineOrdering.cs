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
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try
            {
                stringList.Add("Syncing Tax Settings of services at                " + DateTime.Now.ToString("hh:mm:ss tt"));
                return LoyaltySite.Instance.SyncSiteTaxSettings(inSyndicateCode, siteTaxSettings, stringList);
            }
            catch (Exception exc)
            {
                onlineOrderDB.RollbackTransaction();
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
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                UpdateInvoiceInforWaiterAppOrders(siteOrderViewModel, inSyndicateCode, stringList);
                stringList.Add("UpdateInvoiceInforWaiterAppOrders at               " + DateTime.Now.ToString("hh:mm:ss tt"));
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

        /*public void CreateWaiterTerminal(long siteID)
        {
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try
            {
                stringList.Add("Creating Waiter Terminal at                        " + DateTime.Now.ToString("hh:mm:ss tt"));
                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction...");
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        stringList.Add("inside transaction using                       ");
                        onlineOrderDB.AddWaiterTerminal("WAITER APP 1");
                        //onlineOrderDB.AddWaiterStaff(siteID);
                        onlineOrderDB.transaction.Commit();
                        ServiceLogger.Log(@"after commit in CreateWaiterTerminal ");
                    }
                }
                ServiceLogger.Log(@"outside using in CreateWaiterTerminal");
            }
            catch (Exception e)
            {
                onlineOrderDB.RollbackTransaction();
                stringList.Add("Exception in CreateWaiterTerminal is               " + e.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.Log(@"In CreateWaiterTerminal " + e.Message);
                EventLog.WriteEntry("IN Waiter terminal creation ", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 131, short.MaxValue);
            }
            WriteAndClearStringList();
            
        }*/
        private void UpdateInvoiceInforWaiterAppOrders(List<ApiSiteOrderViewModel> siteOrderViewModelList, string syndicateCode, List<string> stringList)
        {
            try
            {
                SiteOrderModel siteOrderModel = new SiteOrderModel();

                stringList.Add("Updating InvoiceIn for Waiter App Orders           " + DateTime.Now.ToString("hh:mm:ss tt"));
                if (!String.IsNullOrEmpty(syndicateCode))
                {
                    foreach (var siteOrderViewModel in siteOrderViewModelList)
                    {
                        if (siteOrderViewModel.UserType == OnlineOrdering.Enum.UserType.Staff && siteOrderViewModel.ApiSiteOrderPaymentViewModels != null)
                        {
                            siteOrderModel = LoyaltySite.Instance.CreateSiteOrderModel(siteOrderViewModel, stringList);

                            PostOnlineOrderInvoiceInfo(syndicateCode, siteOrderModel);
                        }
                    }
                }

            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in UpdateInvoiceInforWaiterAppOrders     " + exc.Message);
                stringList.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
            } 
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
                        bool IsTerminalExist = onlineOrderDB.CheckIfWaiterTerminalExist(apiWaiterAppPosTerminal.DeviceId);
                        stringList.Add("inside transaction using                       ");
                        if(!IsTerminalExist)
                            onlineOrderDB.AddWaiterTerminal(apiWaiterAppPosTerminal.Name, apiWaiterAppPosTerminal.DeviceId);
                        //onlineOrderDB.AddWaiterStaff(siteID);
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

                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction...");
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        bool IsAppZedRowExist = onlineOrderDB.CheckIfZedRequestExist(apiWaiterAppPosTerminal.Name);

                        if (!IsAppZedRowExist)
                            onlineOrderDB.InsertAppZedRow(apiWaiterAppPosTerminal.Name, apiWaiterAppPosTerminal.DeviceId);
                        ServiceLogger.Log(@"after commit in CreateRequestForAppZed ");
                    }
                }
                ServiceLogger.Log(@"outside using in CreateRequestForAppZed");

            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
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
