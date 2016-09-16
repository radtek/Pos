using System;

namespace Loyaltymate.Model
{
    public class ApiGiftCardUsageRequestViewModel
    {
        public string GiftCardNumber { get; set; }
        public double PointsRedeemed { get; set; }
        public double TotalSaleAmount { get; set; }
        public Guid? MemberUniqueId { get; set; }
        public long SiteCode { get; set; }
        public DateTime Date { get; set; }
        public string InvoiceNumber { get; set; }
    }
}