using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class POIData
    {
        [DataMember]
        public POITransactionID POITransactionID { get; set; }
    }
}
