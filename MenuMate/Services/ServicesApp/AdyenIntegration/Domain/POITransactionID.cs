using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class POITransactionID
    {
        [DataMember]
        public string TimeStamp { get; set; }
        [DataMember]
        public string TransactionID { get; set; }
    }
}
