using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentTransaction
    {
        [DataMember]
        public AmountsReq AmountsReq { get; set; }
    }
}
