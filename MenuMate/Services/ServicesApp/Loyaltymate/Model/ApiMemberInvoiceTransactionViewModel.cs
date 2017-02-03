using System;

namespace Loyaltymate.Model
{
    public class ApiMemberInvoiceTransactionViewModel
    {
        public string InvoiceNumber { get; set; }
        public double SpendAmount { get; set; }
        public DateTime TransactionDate { get; set; }
        public string MemberUniqueId { get; set; }

        public long SiteCode { get; set; }
    }
}