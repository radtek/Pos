using Loyaltymate.Model;
using System.Collections.Generic;
using Loyaltymate.Model.OnlineOrderingModel;
using Loyaltymate.Model.OnlineOrderingModel.TaxSettingModel;
using Loyaltymate.Model.OnlineOrderingModel.OrderModels;

namespace Loyaltymate.Sevices
{
    public interface ILoyaltymateService
    {
        ApiMemberViewModel SaveMember(ApiMemberViewModel member, string syndicateCode, List<string> loyaltyLogs);
        ApiMemberViewModel GetMemberByUniqueId(ApiRequestViewModel requestViewModel, string syndicateCode, List<string> loyaltyLogs);
        ApiMemberViewModel GetMemberByCardCode(ApiRequestViewModel requestViewModel, string syndicateCode, List<string> loyaltyLogs);
        ApiMemberViewModel GetMemberByEmail(ApiRequestViewModel requestViewModelrequestViewModel, string inSyndicateCode, List<string> loyaltyLogs);
        bool UpdateMemberCardCode(string inSyndicateCode, ApiUpdateCardCodeRequestViewModel requestViewModel, List<string> loyaltyLogs);
        bool PostTransaction(PointsTransactionViewModel pointsTransaction, string syndicateCode, List<string> loyaltyLogs);
        bool PostInvoiceTransaction(ApiMemberInvoiceTransactionViewModel invoiceTransaction, string syndicateCode, List<string> loyaltyLogs);
        ApiCompanyViewModel GetCompanyDetail(string inSyndicateCode, List<string> loyaltyLogs);
        GiftCardApiViewModel GetGiftCardBalance(string inSyndicateCode, ApiRequestViewModel requestViewModel, List<string> loyaltyLogs);
        ApiPocketVoucherViewModel GetPocketVoucherDetail(string inSyndicateCode, ApiRequestViewModel requestViewModel, List<string> loyaltyLogs);
        List<ApiProcessingResult> PostVoucherTransactions(string inSyndicateCode, ApiVouchersUsageViewModel voucherTransaction, List<string> loyaltyLogs);
        bool ReleaseVouchers(string inSyndicateCode, ApiReleasedVoucherViewModel voucherTransaction, List<string> loyaltyLogs);
        bool SyncSiteMenu(string inSyndicateCode, ApiSiteMenuViewModel siteMenus);
        bool SyncSiteTaxSettings(string inSyndicateCode, ApiSiteTaxSettings siteTaxSettings);
        bool UpdateOrderStatus(string inSyndicateCode, List<ApiSiteOrderViewModel> siteOrderViewModel);
        bool PostOnlineOrderInvoiceInfo(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel);
        ApiOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int inSiteCode);
        bool UnsetOrderingDetails(string inSyndicateCode, int inSiteCode);
    }
}