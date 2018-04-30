using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentData
    {
        [DataMember]
        public string PaymentType { get; set; }
    }
}
