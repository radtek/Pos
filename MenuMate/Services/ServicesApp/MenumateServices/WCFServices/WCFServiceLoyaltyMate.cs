using System;
using System.Diagnostics;
using MenumateServices.DTO.LoyaltyMate;
using MenumateServices.LoyaltyMate;
using MenumateServices.Internal_Classes.LoyaltyMate;
using MenumateServices.DTO.OnlineOrdering;
using System.ServiceModel.Activation;
using System.ServiceModel;
using Loyaltymate.Utility;
using MenumateServices.DTO.OnlineOrdering.DBOrders;
using MenumateServices.DTO.OnlineOrdering.OrderModels;
using MenumateServices.DTO.OnlineOrdering.MenuModels;
using Loyaltymate.Model.OnlineOrderingModel.OrderModels;
using System.Collections.Generic;
using MenumateServices.Tools;

namespace MenumateServices.WCFServices
{
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    [ServiceBehavior(MaxItemsInObjectGraph = 65536000)]
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceLoyaltyMate" in both code and config file together.
    public class WCFServiceLoyaltyMate : IWCFServiceLoyaltyMate
    {

        private List<string> stringList = new List<string>();
        public LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo)
        {
            try
            {
                return LoyaltyMember.Instance.SaveMember(inSyndicateCode, inInfo);
            }
            catch (Exception exc)
            {
                //EventLog.WriteEntry("In SaveMember LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 58, short.MaxValue);
                ServiceLogger.LogException(exc.Message, exc);
            }
            return null;
        }

        public LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                return LoyaltyMember.Instance.GetMemberByUniqueCode(inSyndicateCode, requestInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetMemberByUniqueId LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 59, short.MaxValue);
            }
            return null;
        }

        public LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                return LoyaltyMember.Instance.GetByCardCode(inSyndicateCode, requestInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetMemberByCardCode LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 60, short.MaxValue);
            }
            return null;
        }

        public LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                return LoyaltyMember.Instance.GetByEmail(inSyndicateCode, requestInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetMemberByEmail LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 61, short.MaxValue);
            }
            return null;
        }

        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            try
            {
                return LoyaltyMember.Instance.UpdateMemberCardCode(inSyndicateCode, uniqueId, memberCardCode);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In UpdateMemberCardCode LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 62, short.MaxValue);
            }
            return null;
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction)
        {
            try
            {
                return LoyaltyMember.Instance.PostTransaction(inSyndicateCode, transaction);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In PostTransaction LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 63, short.MaxValue);
            }
            return null;
        }

        public LoyaltyCompanyResponse GetCompanyInformation(string inSyndicateCode)
        {
            try
            {
                return LoyaltyCompany.Instance.GetCompanyInformation(inSyndicateCode);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetCompanyInformation LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 64, short.MaxValue);
            }
            return null;
        }

        public LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                return LoyaltyVoucher.Instance.GetGiftCardBalance(inSyndicateCode, requestInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetGiftCardBalance LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 65, short.MaxValue);
            }
            return null;
        }

        public LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                return LoyaltyVoucher.Instance.GetPocketVoucherDetail(inSyndicateCode, requestInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetPocketVoucherDetail LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 66, short.MaxValue);
            }
            return null;
        }

        public VoucherTransactionResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction)
        {
            try
            {
                return LoyaltyVoucher.Instance.ProcessVoucherTransaction(inSyndicateCode, transaction);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In ProcessVoucherTransaction LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 67, short.MaxValue);
            }
            return null;
        }

        public LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo)
        {
            try
            {
                return LoyaltyVoucher.Instance.ReleaseVouchers(inSyndicateCode, releasedVoucherInfo);
            }
            catch (Exception exc)
            {
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In ReleaseVouchers LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 68, short.MaxValue);
            }
            return null;
        }

        public LoyaltyResponse SyncTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings)
        {
            try
            {
                stringList.Add("-----------------------------------------Inside SyncTaxSettings------------------------------------------------------");
                WriteAndClearStringList();
                return LoyaltySite.Instance.SyncSiteTaxSettings(inSyndicateCode, siteTaxSettings);
            }
            catch (Exception exc)
            {
                stringList.Add("Exception in SyncTaxSettings     "+ exc.Message);
                WriteAndClearStringList();
                ServiceLogger.LogException(exc.Message, exc);
            }
            return null;
        }

        public LoyaltyResponse SyncMenu(string inSyndicateCode, SiteMenuInfo siteViewModel)
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

        public LoyaltyResponse PostOnlineOrderInvoiceInfo(string inSyndicateCode, SiteOrderModel siteOrderModel)
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

        public LoyaltyOnlineOrderingResponse SyncOnlineOrderingDetails(string inSyndicateCode, int siteCode)
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
                stringList.Add("Exception in PostOnlineOrderInvoiceInfo     " + exc.Message);
                WriteAndClearStringList();
            }
            return null; 
        }

        private void WriteAndClearStringList()
        {
            FileWriter.WriteToFile(stringList);
            stringList.Clear();
        }
    }
}
