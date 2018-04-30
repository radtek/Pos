using System;
using System.Runtime.Serialization;
using AdyenIntegration.Domain.Common;

namespace AdyenIntegration.Domain.RequestEnvelop
{
    [DataContract]
    public class SaleToPOIRequest
    {
        [DataMember]
        public MessageHeader MessageHeader { get; set; }
        [DataMember]
        public PaymentRequest PaymentRequest { get; set; }
        [DataMember]
        public DiagnosisRequest DiagnosisRequest { get; set; }
        [DataMember]
        public ReversalRequest ReversalRequest { get; set; }
    }
}
