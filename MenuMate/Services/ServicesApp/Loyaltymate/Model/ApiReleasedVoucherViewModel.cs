using System.Collections.Generic;

namespace Loyaltymate.Model
{
    public class ApiReleasedVoucherViewModel
    {
        public string TransactionReferenceNumber { get; set; }
        public string PocketVoucherNumber { get; set; }
        public string GiftCardNumber { get; set; }
        public string VoucherName { get; set; }
        public List<string> DiscountCodes { get; set; }
    }
}