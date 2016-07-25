using System;

namespace SalesForceCommunication.Domain
{
    public class PocketVoucherDetail
    {
        public bool? PocketVoucher { get; set; }
        public string Url { get; set; }
        public string UserName { get; set; }
        public string Password { get; set; }
        public string MerchantId { get; set; }
        public DateTime? DateUpdated { get; set; }
    }
}