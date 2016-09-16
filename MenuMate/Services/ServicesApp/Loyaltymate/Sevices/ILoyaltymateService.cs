using Loyaltymate.Model;
using System.Collections.Generic;

namespace Loyaltymate.Sevices
{
    public interface ILoyaltymateService
    {
        ApiMemberViewModel SaveMember(ApiMemberViewModel member, string syndicateCode);
        ApiMemberViewModel GetMemberByUniqueId(ApiRequestViewModel requestViewModel, string syndicateCode);
        ApiMemberViewModel GetMemberByCardCode(ApiRequestViewModel requestViewModel, string syndicateCode);
        ApiMemberViewModel GetMemberByEmail(ApiRequestViewModel requestViewModel, string inSyndicateCode);
        bool UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode);
        bool PostTransaction(PointsTransactionViewModel pointsTransaction, string syndicateCode);
        bool PostInvoiceTransaction(ApiMemberInvoiceTransactionViewModel invoiceTransaction, string syndicateCode);
        ApiCompanyViewModel GetCompanyDetail(string inSyndicateCode);
        double GetGiftCardBalance(string inSyndicateCode, ApiRequestViewModel requestViewModel);
        ApiPocketVoucherViewModel GetPocketVoucherDetail(string inSyndicateCode, ApiRequestViewModel requestViewModel);
        List<ApiProcessingResult> PostVoucherTransactions(string inSyndicateCode, ApiVouchersUsageViewModel voucherTransaction);
        bool ReleaseVouchers(string inSyndicateCode, ApiReleasedVoucherViewModel voucherTransaction);
    }
}