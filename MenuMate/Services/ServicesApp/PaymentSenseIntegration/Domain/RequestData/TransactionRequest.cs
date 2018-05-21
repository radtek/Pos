using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class TransactionRequest
    {
        [DataMember]
        public string transactionType { get; set; }//TransactionType starting from small letter because api is not accepting this parameter in caps when doint refund.
        [DataMember]
        public double amount { get; set; } //amount starting from small letter because api is not accepting this parameter in caps.
        [DataMember]
        public string currency { get; set; }//amount starting from small letter because api is not accepting this parameter in caps  when doint refund.
    }
}
