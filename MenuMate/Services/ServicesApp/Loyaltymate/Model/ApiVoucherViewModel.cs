using System;
using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class ApiVoucherViewModel
    {
        public long VoucherId { get; set; }
        public string Name { get; set; }
        public string DiscountCode { get; set; }
        public int? NumberOfUsesAllowed { get; set; }
    }
}