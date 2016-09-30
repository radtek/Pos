using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class VoucherTransactionInfo
    {
        //Gift Card
        [DataMember]
        public string GiftCardNumber { get; set; }
        [DataMember]
        public double PointsRedeemed { get; set; }

        //Gift Card
        [DataMember]
        public string PurchasedGiftCardNumber { get; set; }
        [DataMember]
        public double PointsPurchased { get; set; }


        //Member Voucher
        [DataMember]
        public string VoucherName { get; set; }
        [DataMember]
        public double MemberVoucherDiscountAmount { get; set; }

        //Pocket Voucher
        [DataMember]
        public string PocketVoucherNumber { get; set; }
        [DataMember]
        public double PocketVoucherDiscountAmount { get; set; }

        //Discounts
        [DataMember]
        public List<DiscountUsageInfo> DiscountUsages { get; set; }

        [DataMember]
        public DateTime TransactionDate { get; set; }
        [DataMember]
        public double TotalSaleAmount { get; set; }
        [DataMember]
        public string MemberUniqueId { get; set; }
        [DataMember]
        public long SiteCode { get; set; }
        [DataMember]
        public string InvoiceNumber { get; set; }

        [DataMember]
        public string TransactionReferenceNumber { get; set; }
    }
}
