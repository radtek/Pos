using System;
using System.Diagnostics;
using MenumateServices.DTO.LoyaltyMate;
using MenumateServices.LoyaltyMate;
using MenumateServices.Internal_Classes.LoyaltyMate;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceLoyaltyMate" in both code and config file together.
    public class WCFServiceLoyaltyMate : IWCFServiceLoyaltyMate
    {

        public LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo)
        {
            try
            {
                return LoyaltyMember.Instance.SaveMember(inSyndicateCode, inInfo);
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In SaveMember LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 58, short.MaxValue);
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
                EventLog.WriteEntry("In GetMemberByUniqueId LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 59, short.MaxValue);
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
                EventLog.WriteEntry("In GetMemberByCardCode LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 60, short.MaxValue);
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
                EventLog.WriteEntry("In GetMemberByEmail LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 61, short.MaxValue);
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
                EventLog.WriteEntry("In UpdateMemberCardCode LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 62, short.MaxValue);
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
                EventLog.WriteEntry("In PostTransaction LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 63, short.MaxValue);
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
                EventLog.WriteEntry("In GetCompanyInformation LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 64, short.MaxValue);
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
                EventLog.WriteEntry("In GetGiftCardBalance LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 65, short.MaxValue);
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
                EventLog.WriteEntry("In GetPocketVoucherDetail LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 66, short.MaxValue);
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
                EventLog.WriteEntry("In ProcessVoucherTransaction LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 67, short.MaxValue);
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
                EventLog.WriteEntry("In ReleaseVouchers LoyaltyMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 68, short.MaxValue);
            }
            return null;
        }

    }
}
