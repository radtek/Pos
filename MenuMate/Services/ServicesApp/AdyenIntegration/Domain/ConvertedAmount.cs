using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class ConvertedAmount
    {
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public double AmountValue { get; set; }
    }
}
