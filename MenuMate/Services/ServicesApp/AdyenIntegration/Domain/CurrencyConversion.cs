using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class CurrencyConversion
    {
        [DataMember]
        public string CustomerApprovedFlag { get; set; }
        [DataMember]
        public ConvertedAmount ConvertedAmount { get; set; }
        [DataMember]
        public double Rate { get; set; }
        [DataMember]
        public double Markup { get; set; }
        [DataMember]
        public double Commission { get; set; }
        [DataMember]
        public string Declaration { get; set; }
    }
}
