using System;
using System.Diagnostics;
using MenumateServices.DTO.LoyaltyMate;
using MenumateServices.LoyaltyMate;
using MenumateServices.Internal_Classes.LoyaltyMate;
using System.ServiceModel.Activation;
using System.ServiceModel;
using Loyaltymate.Utility;
using System.Collections.Generic;
using MenumateServices.Tools;
using MenumateServices.DTO.MenumateRegistration;

namespace MenumateServices.WCFServices
{
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    [ServiceBehavior(MaxItemsInObjectGraph = 65536000)]
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceLoyaltyMate" in both code and config file together.
    public class WCFServiceLoyaltyMate : IWCFServiceLoyaltyMate
    {

        private List<string> stringList = new List<string>();
        private List<string> loyaltyLogs = new List<string>();
        public LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo)
        {
            LoyaltyMemberResponse loyaltyMemberResponse = null;
            try
            {
                loyaltyLogs.Add("SaveMember of services at                          " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyMemberResponse = LoyaltyMember.Instance.SaveMember(inSyndicateCode, inInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                //EventLog.WriteEntry("In SaveMember LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 58, short.MaxValue);
                loyaltyLogs.Add("Exception in SaveMember of services is             " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
            }
            WriteAndClearLoyaltyList();
            return loyaltyMemberResponse;
        }

        public LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, RequestInfo requestInfo)
        {
            LoyaltyMemberResponse loyaltyMemberResponse = null;
            try
            {
                loyaltyLogs.Add("Getting Member By UniqueId of services at          " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyMemberResponse = LoyaltyMember.Instance.GetMemberByUniqueCode(inSyndicateCode, requestInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetMemberByUniqueId LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 59, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyMemberResponse;
        }

        public LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, RequestInfo requestInfo)
        {
            LoyaltyMemberResponse loyaltyMemberResponse = null;
            try
            {
                loyaltyLogs.Add("Getting Member By Card Code of services at         " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyMemberResponse = LoyaltyMember.Instance.GetByCardCode(inSyndicateCode, requestInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetMemberByCardCode LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 60, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyMemberResponse;
        }

        public LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, RequestInfo requestInfo)
        {
            LoyaltyMemberResponse loyaltyMemberResponse = null;
            try
            {
                loyaltyLogs.Add("Getting Member By Email of services at             " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyMemberResponse = LoyaltyMember.Instance.GetByEmail(inSyndicateCode, requestInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetMemberByEmail LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 61, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyMemberResponse;
        }

        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            LoyaltyResponse loyaltyResponse = null;
            try
            {
                loyaltyLogs.Add("Updating Member Card Code of services at           " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyResponse = LoyaltyMember.Instance.UpdateMemberCardCode(inSyndicateCode, uniqueId, memberCardCode, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In UpdateMemberCardCode LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 62, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyResponse;
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction)
        {
            LoyaltyResponse loyaltyResponse = null;
            try
            {
                loyaltyLogs.Add("Posting Transaction of services at                 " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyResponse = LoyaltyMember.Instance.PostTransaction(inSyndicateCode, transaction, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In PostTransaction LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 63, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyResponse;
        }

        public LoyaltyCompanyResponse GetCompanyInformation(string inSyndicateCode)
        {
            LoyaltyCompanyResponse loyaltyCompanyResponse = null;
            try
            {
                loyaltyLogs.Add("Getting Company Information of services at         " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyCompanyResponse = LoyaltyCompany.Instance.GetCompanyInformation(inSyndicateCode, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetCompanyInformation LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 64, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyCompanyResponse;
        }

        public LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo)
        {
            LoyaltyGiftCardResponse loyaltyGiftCardResponse = null;
            try
            {
                loyaltyLogs.Add("Getting Gift card balance of services at           " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyGiftCardResponse = LoyaltyVoucher.Instance.GetGiftCardBalance(inSyndicateCode, requestInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetGiftCardBalance LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 65, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyGiftCardResponse;
        }

        public LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo)
        {

            LoyaltyVoucherResponse loyaltyVoucherResponse = null;
            try
            {
                loyaltyLogs.Add("Getting Pocket Voucher Detail at                   " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyVoucherResponse = LoyaltyVoucher.Instance.GetPocketVoucherDetail(inSyndicateCode, requestInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In GetPocketVoucherDetail LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 66, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyVoucherResponse;
        }

        public VoucherTransactionResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction)
        {
            VoucherTransactionResponse voucherTransactionResponse = null;
            try
            {
                loyaltyLogs.Add("Processing Voucher Transaction at                  " + DateTime.Now.ToString("hh:mm:ss tt"));
                voucherTransactionResponse = LoyaltyVoucher.Instance.ProcessVoucherTransaction(inSyndicateCode, transaction, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In ProcessVoucherTransaction LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 67, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return voucherTransactionResponse;
        }

        public LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo)
        {
            LoyaltyResponse loyaltyResponse = null;
            try
            {
                loyaltyLogs.Add("Releasing Voucher at                               " + DateTime.Now.ToString("hh:mm:ss tt"));
                loyaltyResponse = LoyaltyVoucher.Instance.ReleaseVouchers(inSyndicateCode, releasedVoucherInfo, loyaltyLogs);
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                       " + exc.Message);
                loyaltyLogs.Add("Time is                                            " + DateTime.Now.ToString("hh:mm:ss tt"));
                ServiceLogger.LogException(exc.Message, exc);
                //EventLog.WriteEntry("In ReleaseVouchers LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 68, short.MaxValue);
            }
            WriteAndClearLoyaltyList();
            return loyaltyResponse;
        }

        /*public LoyaltyResponse SyncTaxSettings(string inSyndicateCode, SiteTaxSettingsinfo siteTaxSettings)
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
        }*/

        private void WriteAndClearStringList()
        {
            FileWriter.WriteToFile(stringList, "Online Ordering Logs", "OnlineOrderingLogs ");
            stringList.Clear();
        }

        private void WriteAndClearLoyaltyList()
        {
            loyaltyLogs.Add("======================================================================================================================");
            FileWriter.WriteToFile(loyaltyLogs, "LoyaltyMate Post Logs", "LoyaltyMatePosts ");
            loyaltyLogs.Clear();
        }

    }
}
