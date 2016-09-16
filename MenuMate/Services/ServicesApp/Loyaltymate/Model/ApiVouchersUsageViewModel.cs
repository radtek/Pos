using System.Collections.Generic;
using System;

namespace Loyaltymate.Model
{
    public class ApiVouchersUsageViewModel
    {
        public DateTime RequestTime { get; set; }
        public string TransactionReferenceNumber { get; set; }
        public ApiGiftCardUsageRequestViewModel GiftCard { get; set; }
        public ApiPocketVoucherUsageRequestViewModel PocketVoucher { get; set; }
        public ApiVoucherUsageRequestViewModel Voucher { get; set; }
        public List<ApiDiscountUsageRequestViewModel> Discounts { get; set; }
    }
}   