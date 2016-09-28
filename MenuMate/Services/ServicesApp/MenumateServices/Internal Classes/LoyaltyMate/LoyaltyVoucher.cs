using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.DTO.LoyaltyMate;
using Loyaltymate.Sevices;
using MenumateServices.LoyaltyMate;
using Loyaltymate.Exceptions;
using Loyaltymate.Model;

namespace MenumateServices.Internal_Classes.LoyaltyMate
{
    public class LoyaltyVoucher : LoyaltyResponsive
    {
        private static volatile LoyaltyVoucher _instance;
        private static object syncRoot = new Object();

        private LoyaltyVoucher()
        {

        }

        public static LoyaltyVoucher Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (syncRoot)
                    {
                        if (_instance == null)
                            _instance = new LoyaltyVoucher();
                    }
                }

                return _instance;
            }
        }

        public LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo)
        {
            return GetGiftCardBalanceFromCloud(inSyndicateCode, requestInfo);
        }

        public LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo)
        {
            return GetPocketVoucherDetailFromCloud(inSyndicateCode, requestInfo);
        }

        public LoyaltyResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction)
        {
            return PostVoucherTransaction(inSyndicateCode, transaction);
        }

        public LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo)
        {
            return ReleaseVouchersInCloud(inSyndicateCode, releasedVoucherInfo);
        }

        private LoyaltyGiftCardResponse GetGiftCardBalanceFromCloud(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetGiftCardBalance(inSyndicateCode, CreateRequest(requestInfo));
                return (CreateGiftCardResponseNoError(response));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateGiftCardResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, 0);
            }
            catch (LoyaltymateOperationException ex)
            {
                return CreateGiftCardResponseError(ex.Message,
                            @"Invalid Gift Voucher Number",
                            LoyaltyResponseCode.InvalidGiftVoucher, 0);
            }
            catch (Exception exc)
            {
                return CreateGiftCardResponseError(
                             @"Failed to request gift card balance from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetGiftCardFailed,
                             0);
            }
        }

        private LoyaltyVoucherResponse GetPocketVoucherDetailFromCloud(string inSyndicateCode, RequestInfo requestInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetPocketVoucherDetail(inSyndicateCode, CreateRequest(requestInfo));
                return (CreateVoucherResponseNoError(CreateVoucherInfo(response)));
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateVoucherResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, new VoucherInfo());
            }
            catch (LoyaltymateOperationException ex)
            {
                return CreateVoucherResponseError(ex.Message, @"Invalid Pocket Voucher Number",
                            LoyaltyResponseCode.InvalidPocketVoucher, new VoucherInfo());
            }
            catch (Exception exc)
            {
                return CreateVoucherResponseError(@"Failed to request voucher detail from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetPocketVoucherFailed,
                             new VoucherInfo());
            }
        }

        private LoyaltyResponse PostVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.PostVoucherTransactions(inSyndicateCode, CreateVoucherTransaction(transaction));
                if (response != null)
                {
                    foreach (var item in response)
                    {
                        if (!item.IsProcessedSuccessfully)
                            return CreateResponseError(item.Error,
                                                            item.Error,
                                                            LoyaltyResponseCode.PostTransactionFailed);
                    }
                    return CreateResponseNoError();
                }
                return CreateResponseError(
                        @"Failed to process vouchers",
                        @"Failed to process vouchers",
                        LoyaltyResponseCode.PostTransactionFailed);
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception ex)
            {
                return CreateResponseError(
                    "@Failed to post transaction to server",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed);
            }
        }

        public LoyaltyResponse ReleaseVouchersInCloud(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.ReleaseVouchers(inSyndicateCode, CreateReleasedVoucherTransaction(releasedVoucherInfo));
                if (response)
                    return CreateResponseNoError();
                else
                    return CreateResponseError(
                        "@Failed to process vouchers",
                        "",
                        LoyaltyResponseCode.PostTransactionFailed);
            }
            catch (AuthenticationFailedException ex)
            {
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception ex)
            {
                return CreateResponseError(
                    "@Failed to process vouchers",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed);
            }
        }

        VoucherInfo CreateVoucherInfo(ApiPocketVoucherViewModel inVoucherInfo)
        {
            var result = new VoucherInfo();
            result.DiscountCode = inVoucherInfo.DiscountCode;
            result.NumberOfUsesRemaining = inVoucherInfo.NumberOfUsesRemaining;
            result.VoucherName = inVoucherInfo.VoucherName;
            return result;
        }

        ApiReleasedVoucherViewModel CreateReleasedVoucherTransaction(ReleasedVoucherInfo releasedVoucherInfo)
        {
            var voucherTransaction = new ApiReleasedVoucherViewModel()
             {
                 GiftCardNumber = releasedVoucherInfo.GiftCardNumber,
                 PocketVoucherNumber = releasedVoucherInfo.PocketVoucherNumber,
                 VoucherName = releasedVoucherInfo.VoucherName,
                 TransactionReferenceNumber = releasedVoucherInfo.TransactionReferenceNumber
             };
            if (releasedVoucherInfo.DiscountCodes != null)
            {
                voucherTransaction.DiscountCodes = new List<string>();
                releasedVoucherInfo.DiscountCodes.ForEach(s => voucherTransaction.DiscountCodes.Add(s.DiscountCode));
            }

            return voucherTransaction;
        }

        ApiVouchersUsageViewModel CreateVoucherTransaction(VoucherTransactionInfo inTransactionInfo)
        {
            var voucherUsageViewModel = new ApiVouchersUsageViewModel();
            voucherUsageViewModel.TransactionReferenceNumber = inTransactionInfo.TransactionReferenceNumber;
            Guid memberUniqueId = Guid.Empty;
            Guid.TryParse(inTransactionInfo.MemberUniqueId, out memberUniqueId);
            if (!string.IsNullOrWhiteSpace(inTransactionInfo.GiftCardNumber))
            {
                voucherUsageViewModel.GiftCard = new ApiGiftCardTransactionRequestViewModel();
                voucherUsageViewModel.GiftCard.TransactionType = Loyaltymate.Enum.GiftCardVoucherTransactionType.GiftCardUsage;
                voucherUsageViewModel.GiftCard.GiftCardNumber = inTransactionInfo.GiftCardNumber;
                voucherUsageViewModel.GiftCard.PointsTransactionAmount = inTransactionInfo.PointsRedeemed;
                voucherUsageViewModel.GiftCard.MemberUniqueId = memberUniqueId;
                if (memberUniqueId == Guid.Empty)
                    voucherUsageViewModel.GiftCard.MemberUniqueId = null;
                voucherUsageViewModel.GiftCard.SiteCode = inTransactionInfo.SiteCode;
                voucherUsageViewModel.GiftCard.TotalSaleAmount = inTransactionInfo.TotalSaleAmount;
                voucherUsageViewModel.GiftCard.Date = inTransactionInfo.TransactionDate;
                voucherUsageViewModel.GiftCard.InvoiceNumber = inTransactionInfo.InvoiceNumber;
            }

            if (!string.IsNullOrWhiteSpace(inTransactionInfo.PurchasedGiftCardNumber))
            {
                voucherUsageViewModel.GiftCardRecharge = new ApiGiftCardTransactionRequestViewModel();
                voucherUsageViewModel.GiftCardRecharge.TransactionType = Loyaltymate.Enum.GiftCardVoucherTransactionType.GiftCardRecharge;
                voucherUsageViewModel.GiftCardRecharge.GiftCardNumber = inTransactionInfo.PurchasedGiftCardNumber;
                voucherUsageViewModel.GiftCardRecharge.PointsTransactionAmount = inTransactionInfo.PointsPurchased;
                voucherUsageViewModel.GiftCardRecharge.MemberUniqueId = memberUniqueId;
                if (memberUniqueId == Guid.Empty)
                    voucherUsageViewModel.GiftCardRecharge.MemberUniqueId = null;
                voucherUsageViewModel.GiftCardRecharge.SiteCode = inTransactionInfo.SiteCode;
                voucherUsageViewModel.GiftCardRecharge.TotalSaleAmount = inTransactionInfo.TotalSaleAmount;
                voucherUsageViewModel.GiftCardRecharge.Date = inTransactionInfo.TransactionDate;
                voucherUsageViewModel.GiftCardRecharge.InvoiceNumber = inTransactionInfo.InvoiceNumber;
            }




            if (!string.IsNullOrWhiteSpace(inTransactionInfo.PocketVoucherNumber))
            {
                voucherUsageViewModel.PocketVoucher = new ApiPocketVoucherUsageRequestViewModel();
                voucherUsageViewModel.PocketVoucher.PocketVoucherNumber = inTransactionInfo.PocketVoucherNumber;
                voucherUsageViewModel.PocketVoucher.DiscountAmount = inTransactionInfo.PocketVoucherDiscountAmount;
                voucherUsageViewModel.PocketVoucher.MemberUniqueId = memberUniqueId;
                if (memberUniqueId == Guid.Empty)
                    voucherUsageViewModel.PocketVoucher.MemberUniqueId = null;
                voucherUsageViewModel.PocketVoucher.SiteCode = inTransactionInfo.SiteCode;
                voucherUsageViewModel.PocketVoucher.TotalSaleAmount = inTransactionInfo.TotalSaleAmount;
                voucherUsageViewModel.PocketVoucher.Date = inTransactionInfo.TransactionDate;
                voucherUsageViewModel.PocketVoucher.InvoiceNumber = inTransactionInfo.InvoiceNumber;
            }

            if (!string.IsNullOrWhiteSpace(inTransactionInfo.VoucherName))
            {
                voucherUsageViewModel.Voucher = new ApiVoucherUsageRequestViewModel();
                voucherUsageViewModel.Voucher.VoucherName = inTransactionInfo.VoucherName;
                voucherUsageViewModel.Voucher.DiscountAmount = inTransactionInfo.MemberVoucherDiscountAmount;
                voucherUsageViewModel.Voucher.MemberUniqueId = memberUniqueId;
                voucherUsageViewModel.Voucher.SiteCode = inTransactionInfo.SiteCode;
                voucherUsageViewModel.Voucher.TotalSaleAmount = inTransactionInfo.TotalSaleAmount;
                voucherUsageViewModel.Voucher.Date = inTransactionInfo.TransactionDate;
                voucherUsageViewModel.Voucher.InvoiceNumber = inTransactionInfo.InvoiceNumber;
            }

            if (inTransactionInfo.DiscountUsages != null)
            {
                voucherUsageViewModel.Discounts = new List<ApiDiscountUsageRequestViewModel>();
                foreach (var item in inTransactionInfo.DiscountUsages)
                {
                    var discountusage = new ApiDiscountUsageRequestViewModel();
                    discountusage.Date = inTransactionInfo.TransactionDate;
                    discountusage.DiscountAmount = item.DiscountAmount;
                    discountusage.DiscountCode = item.DiscountCode;
                    discountusage.MemberUniqueId = memberUniqueId;
                    if (memberUniqueId == Guid.Empty)
                        discountusage.MemberUniqueId = null;
                    discountusage.SiteCode = inTransactionInfo.SiteCode;
                    discountusage.TotalSaleAmount = inTransactionInfo.TotalSaleAmount;
                    discountusage.InvoiceNumber = inTransactionInfo.InvoiceNumber;
                    voucherUsageViewModel.Discounts.Add(discountusage);
                }
            }
            return voucherUsageViewModel;
        }

        ApiRequestViewModel CreateRequest(RequestInfo requestInfo)
        {
            return new ApiRequestViewModel()
            {
                RequestKey = requestInfo.RequestKey,
                RequestTime = requestInfo.RequestTime,
                SiteCode = requestInfo.SiteCode
            };
        }
    }
}
