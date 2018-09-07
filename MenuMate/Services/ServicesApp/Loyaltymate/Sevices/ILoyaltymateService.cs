using Loyaltymate.Model;
using System.Collections.Generic;
using Loyaltymate.Model.OnlineOrderingModel;
using Loyaltymate.Model.OnlineOrderingModel.TaxSettingModel;
using Loyaltymate.Model.OnlineOrderingModel.OrderModels;

namespace Loyaltymate.Sevices
{
    public interface ILoyaltymateService
    {
        ApiMemberViewModel SaveMember(ApiMemberViewModel member, string syndicateCode);
        ApiMemberViewModel GetMemberByUniqueId(ApiRequestViewModel requestViewModel, string syndicateCode);
        ApiMemberViewModel GetMemberByCardCode(ApiRequestViewModel requestViewModel, string syndicateCode);
        ApiMemberViewModel GetMemberByEmail(ApiRequestViewModel requestViewModelrequestViewModel, string inSyndicateCode);
        bool UpdateMemberCardCode(string inSyndicateCode, ApiUpdateCardCodeRequestViewModel requestViewModel);
        bool PostTransaction(PointsTransactionViewModel pointsTransaction, string syndicateCode);
        bool PostInvoiceTransaction(ApiMemberInvoiceTransactionViewModel invoiceTransaction, string syndicateCode);
        ApiCompanyViewModel GetCompanyDetail(string inSyndicateCode);
        GiftCardApiViewModel GetGiftCardBalance(string inSyndicateCode, ApiRequestViewModel requestViewModel);
        ApiPocketVoucherViewModel GetPocketVoucherDetail(string inSyndicateCode, ApiRequestViewModel requestViewModel);
        List<ApiProcessingResult> PostVoucherTransactions(string inSyndicateCode, ApiVouchersUsageViewModel voucherTransaction);
        bool ReleaseVouchers(string inSyndicateCode, ApiReleasedVoucherViewModel voucherTransaction);
        bool SyncSiteMenu(string inSyndicateCode, ApiSiteMenuViewModel siteMenus);
        bool SyncSiteTaxSettings(string inSyndicateCode, ApiSiteTaxSettings siteTaxSettings);
        bool InsertOrdersToDB(ApiSiteOrderViewModel siteOrderViewModel);
        bool UpdateOrderStatus(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel);
        bool PostOnlineOrderInvoiceInfo(string inSyndicateCode, ApiSiteOrderViewModel siteOrderViewModel);
        ApiOnlineOrderingResponse GetOnlineOrderingInformation(string inSyndicateCode, int inSiteCode);
    }
}