using System;
using System.Runtime.Serialization;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain.Diagnosis;
using AdyenIntegration.Domain.Constants;
using AdyenIntegration.Domain.LoginLogOut;
using AdyenIntegration.Domain.TransactionStatus;

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
        public ReversalRequest ReversalRequest { get; set;}
        [DataMember]
        public LoginRequest LoginRequest { get; set; }
        [DataMember]
        public LogoutRequest LogoutRequest { get; set; }
        [DataMember]
        public TransactionStatusRequest TransactionStatusRequest { get; set; }
    }
}
