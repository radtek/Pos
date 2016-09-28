using System;
using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class ApiGiftCardTransactionRequestViewModel
    {
        public string GiftCardNumber { get; set; }
        public double PointsTransactionAmount { get; set; }
        public double TotalSaleAmount { get; set; }
        public Guid? MemberUniqueId { get; set; }
        public long SiteCode { get; set; }
        public DateTime Date { get; set; }
        public string InvoiceNumber { get; set; }
        public GiftCardVoucherTransactionType TransactionType { get; set; }
    }

}