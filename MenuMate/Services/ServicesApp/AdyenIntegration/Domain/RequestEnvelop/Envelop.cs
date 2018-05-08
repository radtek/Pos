using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.RequestEnvelop
{
    [DataContract]
    public class Envelop
    {
        [DataMember]
        public SaleToPOIRequest SaleToPOIRequest { get; set; }
    }
}
