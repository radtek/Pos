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
                stringList.Add("-----------------------------------------Inside SyncTaxSettings------------------------------------------------------");
                WriteAndClearStringList();
                CreateWaiterTerminal(siteTaxSettings.SiteId);
                return LoyaltySite.Instance.SyncSiteTaxSettings(inSyndicateCode, siteTaxSettings);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in SyncTaxSettings     " + exc.Message);
                WriteAndClearStringList();
                ServiceLogger.LogException(exc.Message, exc);
            }
            return null;
        }

        public OOLoyaltyResponse SyncMenu(string inSyndicateCode, SiteMenuInfo siteViewModel)
        {
            try
            {
                stringList.Add("-----------------------------------------Inside SyncMenu------------------------------------------------------");
                WriteAndClearStringList();
                return LoyaltySite.Instance.SyncSiteMenu(inSyndicateCode, siteViewModel);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in SyncMenu     " + exc.Message);
                WriteAndClearStringList();
            }
            return null;
        }

        public void GetOrdersFromWeb(string inSyndicateCode, string orders)
        {
            try
            {
                stringList.Add("-----------------------------------------Inside GetOrdersFromWeb------------------------------------------------------");
                stringList.Add("received order string is: " + orders);
                List<ApiSiteOrderViewModel> siteOrderViewModel = new List<ApiSiteOrderViewModel>();
                siteOrderViewModel = JsonUtility.Deserialize<List<ApiSiteOrderViewModel>>(orders);
                var requestData = JsonUtility.Serialize<List<ApiSiteOrderViewModel>>(siteOrderViewModel);//just to test json
                stringList.Add("Received order json is:                " + requestData);
                WriteAndClearStringList();

                bool retVal = InsertOrdersToDB(ref siteOrderViewModel);
                stringList.Add("After InsertOrdersToDB function calling.");
                WriteAndClearStringList();

                LoyaltySite.Instance.UpdateOrderStatus(inSyndicateCode, siteOrderViewModel);
                requestData = JsonUtility.Serialize<List<ApiSiteOrderViewModel>>(siteOrderViewModel);
                stringList.Add("After Updating order status to web..json is ..." + requestData);
                WriteAndClearStringList();
                SaveOrderJSON(orders);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in GetOrdersFromWeb     " + exc.Message);
                WriteAndClearStringList();
            }
        }

        private bool InsertOrdersToDB(ref List<ApiSiteOrderViewModel> siteOrderViewModel)
        {
            bool result = false;
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try
            {
                stringList.Add("----------------------------Inside InsertOrdersToDB--------------------------------------------");
                stringList.Add("Making connection...");
                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction...");
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        stringList.Add("inside transaction using ...");
                        WriteAndClearStringList();
                        onlineOrderDB.AddRecords(ref siteOrderViewModel);
                        stringList.Add("After calling AddRecords inside InsertOrdersToDB function..");
                        WriteAndClearStringList();
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
                stringList.Add("Exception in InsertOrdersToDB     " + e.Message);
                WriteAndClearStringList();
            }
            //::::::::::::::::::::::::::::::::::::::::::::::
            return result;

        }

        public OOLoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel)
        {
            try
            {
                stringList.Add("-------------------------------------------------------Inside PostOnlineOrderInvoiceInfo-------------------------------------------------------...");
                WriteAndClearStringList();
                return LoyaltySite.Instance.PostOnlineOrderInvoiceInfo(inSyndicateCode, siteOrderModel);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in PostOnlineOrderInvoiceInfo     " + exc.Message);
                WriteAndClearStringList();
            }
            return null;
        }

        public LoyaltyOOResponse SyncOnlineOrderingDetails(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("-------------------------------------------------------Inside SyncOnlineOrderingDetails-------------------------------------------------------...");
                WriteAndClearStringList();
                return LoyaltyOnlineOrdering.Instance.GetOnlineOrderingInformation(inSyndicateCode, siteCode);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in SyncOnlineOrderingDetails     " + exc.Message);
                WriteAndClearStringList();
            }
            return null;
        }

        public bool UnsetOrderingDetails(string inSyndicateCode, int siteCode)
        {
            try
            {
                stringList.Add("-------------------------------------------------------Inside UnsetOrderingDetails-------------------------------------------------------...");
                WriteAndClearStringList();
                return LoyaltyOnlineOrdering.Instance.UnsetOrderingDetails(inSyndicateCode, siteCode);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in UnsetOrderingDetails     " + exc.Message);
                WriteAndClearStringList();
            }
            return false;
        }
        public void CreateWaiterTerminal(long siteID)
        {
            OnlineOrderDB onlineOrderDB = new OnlineOrderDB();
            try
            {
                stringList.Add("----------------------------Inside CreateWaiterTerminal--------------------------------------------");
                stringList.Add("Making connection...");
                using (onlineOrderDB.connection = onlineOrderDB.BeginConnection())
                {
                    stringList.Add("Making transaction...");
                    using (onlineOrderDB.transaction = onlineOrderDB.BeginFBtransaction())
                    {
                        stringList.Add("inside transaction using ...");
                        WriteAndClearStringList();
                        //onlineOrderDB.AddWaiterTerminal("WAITER APP 1");
                        onlineOrderDB.AddWaiterStaff(siteID);
                        stringList.Add("After calling AddWaiterTerminal inside CreateWaiterTerminal function..");
                        WriteAndClearStringList();
                        onlineOrderDB.transaction.Commit();
                        ServiceLogger.Log(@"after commit in CreateWaiterTerminal ");
                    }
                }
                ServiceLogger.Log(@"outside using in CreateWaiterTerminal");
            }
            catch (Exception e)
            {
                onlineOrderDB.RollbackTransaction();
                ServiceLogger.Log(@"In CreateWaiterTerminal " + e.Message);
                EventLog.WriteEntry("IN Waiter terminal creation ", e.Message + "Trace" + e.StackTrace, EventLogEntryType.Error, 131, short.MaxValue);
                stringList.Add("Exception in CreateWaiterTerminal     " + e.Message);
                WriteAndClearStringList();
            }
            //::::::::::::::::::::::::::::::::::::::::::::::
        }
        private void SaveOrderJSON(string orders)
        {
            try 
            {
                List<ApiSiteOrderViewModel> siteOrderViewModelList = new List<ApiSiteOrderViewModel>();
                siteOrderViewModelList = JsonUtility.Deserialize<List<ApiSiteOrderViewModel>>(orders);
                string orderGUID = "";
                foreach (var siteOrderViewModel in siteOrderViewModelList)
                {
                    orderGUID = siteOrderViewModel.OrderGuid;
                    var requestData = JsonUtility.Serialize<ApiSiteOrderViewModel>(siteOrderViewModel);

                    string path = System.IO.Path.GetDirectoryName(
                    System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                    string location = Path.Combine(path, "logs");
                    if (location.Contains(@"file:\"))
                    {
                        location = location.Replace(@"file:\", "");
                    }
                    if (!Directory.Exists(location))
                        Directory.CreateDirectory(location);

                    string folderName = "Pending Orders";
                    location = Path.Combine(location, folderName);

                    if (!Directory.Exists(location))
                        Directory.CreateDirectory(location);

                    string name = orderGUID + DateTime.Now.ToString("ddMMyyyyHHmmss") + ".json";
                    string fileName = Path.Combine(location, name);

                    if (!File.Exists(fileName))
                    {
                        using (StreamWriter sw = File.CreateText(fileName))
                        {
                            sw.WriteLine(requestData.ToString());
                        }
                    }
                }
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                stringList.Add("Exception in SaveOrderJSON     " + exc.Message);
                WriteAndClearStringList();
            }

        }

        private void WriteAndClearStringList()
        {
            FileWriter.WriteToFile(stringList, "Online Ordering Logs", "OnlineOrderingLogs ");
            stringList.Clear();
        }
    }
}
