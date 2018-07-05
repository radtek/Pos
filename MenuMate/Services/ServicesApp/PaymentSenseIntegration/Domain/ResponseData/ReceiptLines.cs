using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class ReceiptLines
    {
        [DataMember]
        public ReceiptData[] Merchant { get; set; }
        [DataMember]
        public ReceiptData[] Customer { get; set; }
        [DataMember]
        public List<string> MerchantReceipt { get; set; }
        [DataMember]
        public List<string> CustomerReceipt { get; set; }
    }
}
