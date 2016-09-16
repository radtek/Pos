using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class ReleasedVoucherInfo
    {
        [DataMember]
        public string TransactionReferenceNumber { get; set; }
        [DataMember]
        public string PocketVoucherNumber { get; set; }
        [DataMember]
        public string GiftCardNumber { get; set; }
        [DataMember]
        public string VoucherName { get; set; }
        [DataMember]
        public List<DiscountUsageInfo> DiscountCodes { get; set; }
    }
}
