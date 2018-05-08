using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SaleTransactionID
    {
        [DataMember]
        public string TransactionID { get; set; }
        [DataMember]
        public string TimeStamp { get; set; }
    }
}
