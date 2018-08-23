﻿using Loyaltymate.Model;
using System.Collections.Generic;
using Loyaltymate.Model.OnlineOrderingModel;

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
        bool SyncSiteMenu(ApiSiteMenuViewModel siteMenus);
    }
}