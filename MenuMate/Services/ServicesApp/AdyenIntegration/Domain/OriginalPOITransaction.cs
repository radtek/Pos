using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class OriginalPOITransaction
    {
        [DataMember]
        public POITransactionID POITransactionID { get; set; }
    }
}
