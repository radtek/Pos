using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentRequest
    {
        [DataMember]
        public SaleData SaleData { get; set; }
        [DataMember]
        public PaymentTransaction PaymentTransaction { get; set; }
        [DataMember]
        public PaymentData PaymentData { get; set; }
    }
}
