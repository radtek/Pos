using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class ReversalRequest
    {
        [DataMember]
        public OriginalPOITransaction OriginalPOITransaction { get; set; }
        [DataMember]
        public string ReversalReason { get; set; }
        [DataMember]
        public double ReversedAmount { get; set; }
    }
}
