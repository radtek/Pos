using System;
using System.Runtime.Serialization;
using AdyenIntegration.Domain.ResponseEnvelop;

namespace AdyenIntegration.Domain.ResponseEnvelop
{
    [DataContract]
    public class ResponseEnvelop
    {
        [DataMember]
        public SaleToPOIResponse SaleToPOIResponse { get; set; }
    }
}
