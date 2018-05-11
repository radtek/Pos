using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class AmountsReq
    {
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public double RequestedAmount { get; set; }
    }
}
