using System;
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
            return LoyaltyMember.Instance.SaveMember(inSyndicateCode, inInfo);
        }

        public LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, RequestInfo requestInfo)
        {
            return LoyaltyMember.Instance.GetMemberByUniqueCode(inSyndicateCode, requestInfo);
        }

        public LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, RequestInfo requestInfo)
        {
            return LoyaltyMember.Instance.GetByCardCode(inSyndicateCode, requestInfo);
        }

        public LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, RequestInfo requestInfo)
        {
            return LoyaltyMember.Instance.GetByEmail(inSyndicateCode, requestInfo);
        }

        public LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode)
        {
            return LoyaltyMember.Instance.UpdateMemberCardCode(inSyndicateCode, uniqueId, memberCardCode);
        }

        public LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction)
        {
            return LoyaltyMember.Instance.PostTransaction(inSyndicateCode, transaction);
        }

        public LoyaltyCompanyResponse GetCompanyInformation(string inSyndicateCode)
        {
            return LoyaltyCompany.Instance.GetCompanyInformation(inSyndicateCode);
        }

        public LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo)
        {
            return LoyaltyVoucher.Instance.GetGiftCardBalance(inSyndicateCode, requestInfo);
        }

        public LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo)
        {
            return LoyaltyVoucher.Instance.GetPocketVoucherDetail(inSyndicateCode, requestInfo);
        }

        public VoucherTransactionResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction)
        {
            return LoyaltyVoucher.Instance.ProcessVoucherTransaction(inSyndicateCode, transaction);
        }

        public LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo)
        {
            return LoyaltyVoucher.Instance.ReleaseVouchers(inSyndicateCode, releasedVoucherInfo);
        }

    }
}
