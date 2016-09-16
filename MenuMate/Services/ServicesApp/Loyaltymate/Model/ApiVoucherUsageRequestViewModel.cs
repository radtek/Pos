using System;

namespace Loyaltymate.Model
{
    public class ApiVoucherUsageRequestViewModel
    {
        public string VoucherName { get; set; }
        public double DiscountAmount { get; set; }
        public double TotalSaleAmount { get; set; }
        public Guid MemberUniqueId { get; set; }
        public long SiteCode { get; set; }
        public DateTime Date { get; set; }
        public string InvoiceNumber { get; set; }
    }
}