using System;
using System.Runtime.Serialization;
using AdyenIntegration.Domain.Common;

namespace AdyenIntegration.Domain.ResponseEnvelop
{
    [DataContract]
    public class SaleToPOIResponse
    {
        [DataMember]
        public MessageHeader MessageHeader { get; set; }
        [DataMember]
        public PaymentResponse PaymentResponse { get; set; }
    }
}
