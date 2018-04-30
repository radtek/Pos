using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentToken
    {
        [DataMember]
        public string TokenRequestedType { get; set; }
        [DataMember]
        public string TokenValue { get; set; }
        [DataMember]
        public string ExpiryDateTime { get; set; }
    }
}
