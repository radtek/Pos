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

        public LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            return GetGiftCardBalanceFromCloud(inSyndicateCode, requestInfo, loyaltyLogs);
        }

        public LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            return GetPocketVoucherDetailFromCloud(inSyndicateCode, requestInfo, loyaltyLogs);
        }

        public VoucherTransactionResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction, List<string> loyaltyLogs)
        {
            return PostVoucherTransaction(inSyndicateCode, transaction, loyaltyLogs);
        }

        public LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo, List<string> loyaltyLogs)
        {
            return ReleaseVouchersInCloud(inSyndicateCode, releasedVoucherInfo, loyaltyLogs);
        }

        private LoyaltyGiftCardResponse GetGiftCardBalanceFromCloud(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetGiftCardBalance(inSyndicateCode, CreateRequest(requestInfo), loyaltyLogs);
                loyaltyLogs.Add("Creating Gift Card Response With No Error            ");
                return (CreateGiftCardResponseNoError(CreateGiftCardInfo(response)));
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateGiftCardResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed,
                            CreateGiftCardInfo("Failed to Authenticate"));
            }
            catch (LoyaltymateOperationException ex)
            {
                loyaltyLogs.Add("Loyaltymate Operation Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateGiftCardResponseError(ex.Message,
                            @"Invalid Gift Voucher Number",
                            LoyaltyResponseCode.InvalidGiftVoucher,
                            CreateGiftCardInfo("Invalid Gift Voucher Number"));
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                         " + exc.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateGiftCardResponseError(
                             @"Failed to request gift card balance from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetGiftCardFailed,
                             CreateGiftCardInfo("Failed to request gift card balance from the server"));
            }
        }

        private LoyaltyVoucherResponse GetPocketVoucherDetailFromCloud(string inSyndicateCode, RequestInfo requestInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.GetPocketVoucherDetail(inSyndicateCode, CreateRequest(requestInfo), loyaltyLogs);
                loyaltyLogs.Add("Creating Gift Card Response With No Error            ");
                return (CreateVoucherResponseNoError(CreateVoucherInfo(response)));
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateVoucherResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, new VoucherInfo());
            }
            catch (LoyaltymateOperationException ex)
            {
                loyaltyLogs.Add("Loyaltymate Operation Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateVoucherResponseError(ex.Message, @"Invalid Pocket Voucher Number",
                            LoyaltyResponseCode.InvalidPocketVoucher, new VoucherInfo());
            }
            catch (Exception exc)
            {
                loyaltyLogs.Add("Exception is                                         " + exc.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateVoucherResponseError(@"Failed to request voucher detail from the server",
                             exc.Message,
                             LoyaltyResponseCode.GetPocketVoucherFailed,
                             new VoucherInfo());
            }
        }

        private VoucherTransactionResponse PostVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                loyaltyLogs.Add("Posting Voucher Transaction at                        ");
                var response = loyaltymateService.PostVoucherTransactions(inSyndicateCode, CreateVoucherTransaction(transaction), loyaltyLogs);
                if (response != null)
                {
                    foreach (var item in response)
                    {
                        if (!item.IsProcessedSuccessfully)
                        {
                            loyaltyLogs.Add("Creating Voucher Transaction Response With Error at           " + DateTime.Now.ToString("hh:mm:ss tt"));
                            return CreateVoucherTransactionResponseError(item.Error,
                                                                        item.Error,
                                                            LoyaltyResponseCode.PostTransactionFailed, null);
                        }
                    }
                    loyaltyLogs.Add("Creating Voucher Transaction Response With No Error           " + DateTime.Now.ToString("hh:mm:ss tt"));
                    return CreateVoucherTransactionResponseNoError(GetGiftCardExpiryDate(response));
                }
                return CreateVoucherTransactionResponseError(
                        @"Failed to process vouchers",
                        @"Failed to process vouchers",
                        LoyaltyResponseCode.PostTransactionFailed, null);
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateVoucherTransactionResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed, null);
            }
            catch (Exception ex)
            {
                loyaltyLogs.Add("Exception is                                         " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateVoucherTransactionResponseError(
                    "@Failed to post transaction to server",
                    ex.Message,
                    LoyaltyResponseCode.PostTransactionFailed, null);
            }
        }

        public LoyaltyResponse ReleaseVouchersInCloud(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo, List<string> loyaltyLogs)
        {
            try
            {
                ILoyaltymateService loyaltymateService = new LoyaltymateService();
                var response = loyaltymateService.ReleaseVouchers(inSyndicateCode, CreateReleasedVoucherTransaction(releasedVoucherInfo), loyaltyLogs);
                loyaltyLogs.Add("Releasing Voucher In Cloud at                        ");
                if (response)
                {
                    loyaltyLogs.Add("Creating Voucher Transaction Response With No Error  ");
                    return CreateResponseNoError();
                }
                else
                {
                    loyaltyLogs.Add("Creating Voucher Transaction Response With  Error    ");
                    return CreateResponseError(
                        "@Failed to process vouchers",
                        "",
                        LoyaltyResponseCode.PostTransactionFailed);
                }
            }
            catch (AuthenticationFailedException ex)
            {
                loyaltyLogs.Add("Authentication Failed Exception is                   " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
                return CreateResponseError(
                            @"Failed to Authenticate",
                            ex.Message,
                            LoyaltyResponseCode.AuthenticationFailed);
            }
            catch (Exception ex)
            {
                loyaltyLogs.Add("Exception is                                         " + ex.Message);
                loyaltyLogs.Add("Time is                                              " + DateTime.Now.ToString("hh:mm:ss tt"));
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

        GiftCardInfo CreateGiftCardInfo(GiftCardApiViewModel inGiftCardInfo)
        {
            var result = new GiftCardInfo();
            if (inGiftCardInfo.ExpiryDate.HasValue)
                result.ExpiryDate = inGiftCardInfo.ExpiryDate.Value;
            if (inGiftCardInfo.StartDate.HasValue)
                result.StartDate = inGiftCardInfo.StartDate.Value;
            result.GiftCardNumber = inGiftCardInfo.GiftCardNumber;
            result.PointBalance = inGiftCardInfo.PointBalance;
            result.StatusCode = (int)inGiftCardInfo.Result;
            return result;
        }

        GiftCardInfo CreateGiftCardInfo(string message)
        {
            var result = new GiftCardInfo();
            result.PointBalance = 0;
            result.StatusCode = 2;
            result.ResponseMessage = message;
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
                voucherUsageViewModel.GiftCard = new ApiGiftCardUsageRequestViewModel();
                voucherUsageViewModel.GiftCard.TransactionType = Loyaltymate.Enum.GiftCardVoucherTransactionType.GiftCardUsage;
                voucherUsageViewModel.GiftCard.GiftCardNumber = inTransactionInfo.GiftCardNumber;
                voucherUsageViewModel.GiftCard.PointsRedeemed = inTransactionInfo.PointsRedeemed;
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
                voucherUsageViewModel.GiftCardRecharge = new ApiGiftCardUsageRequestViewModel();
                voucherUsageViewModel.GiftCardRecharge.TransactionType = Loyaltymate.Enum.GiftCardVoucherTransactionType.GiftCardRecharge;
                voucherUsageViewModel.GiftCardRecharge.GiftCardNumber = inTransactionInfo.PurchasedGiftCardNumber;
                voucherUsageViewModel.GiftCardRecharge.PointsRedeemed = inTransactionInfo.PointsPurchased;
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

        DateTime? GetGiftCardExpiryDate(List<ApiProcessingResult> processingResults)
        {
            var giftCardRechargeResult = processingResults.FirstOrDefault(s => s.ElementType == Loyaltymate.Enum.ElementType.GiftCardRecharge);
            if (giftCardRechargeResult != null)
                return giftCardRechargeResult.ResponseTime;
            return null;
        }


    }
}
