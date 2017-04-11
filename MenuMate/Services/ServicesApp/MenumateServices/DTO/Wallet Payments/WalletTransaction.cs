using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Wallet_Payments
{
    [DataContract]
    public class WalletTransaction
    {
        [DataMember]
        public string ScannedCode { get; set; }
        [DataMember]
        public double Amount { get; set; }
        [DataMember]
        public string ReferenceNumber { get; set; }
        [DataMember]
        public double RefundFee { get; set; }
    }
}
