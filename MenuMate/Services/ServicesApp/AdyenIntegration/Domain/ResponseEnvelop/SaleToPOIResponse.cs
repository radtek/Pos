using System;
using System.Runtime.Serialization;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain.Diagnosis;

namespace AdyenIntegration.Domain.ResponseEnvelop
{
    [DataContract]
    public class SaleToPOIResponse
    {
        [DataMember]
        public PaymentResponse PaymentResponse { get; set; }
        [DataMember]
        public MessageHeader MessageHeader { get; set; }
        [DataMember]
        public DiagnosisResponse DiagnosisResponse { get; set; }
        [DataMember]
        public EventNotification EventNotification { get; set; }
    }
}
