using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class CardData
    {
        [DataMember]
        public string PaymentBrand { get; set; }
        [DataMember]
        public string MaskedPan { get; set; }
        [DataMember]
        public string PaymentAccountRef { get; set; }
        [DataMember]
        public string EntryMode { get; set; }
        [DataMember]
        public string CardCountryCode { get; set; }
        [DataMember]
        public string ProtectedCardData { get; set; }
        [DataMember]
        public SensitiveCardData SensitiveCardData { get; set; }
        [DataMember]
        public string[] AllowedProductCode { get; set; }
        [DataMember]
        public AllowedProduct[] AllowedProduct { get; set; }
        [DataMember]
        public PaymentToken PaymentToken { get; set; }
        [DataMember]
        public CustomerOrder[] CustomerOrder { get; set; }
    }
}
