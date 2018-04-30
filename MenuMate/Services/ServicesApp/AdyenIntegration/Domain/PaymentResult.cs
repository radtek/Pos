using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentResult
    {
        [DataMember]
        public string PaymentType { get; set; }
        [DataMember]
        public PaymentInstrumentData PaymentInstrumentData { get; set; }
        [DataMember]
        public AmountsResp AmountsResp { get; set; }
        [DataMember]
        public Instalment Instalment { get; set; }
        [DataMember]
        public CurrencyConversion CurrencyConversion { get; set; }
        [DataMember]
        public string MerchantOverrideFlag { get; set; }
        [DataMember]
        public CapturedSignature CapturedSignature { get; set; }
        [DataMember]
        public string ProtectedSignature { get; set; }
        [DataMember]
        public string CustomerLanguage { get; set; }
        [DataMember]
        public string OnlineFlag { get; set; }
        [DataMember]
        public string AuthenticationMethod { get; set; }
        [DataMember]
        public string ValidityDate { get; set; }
        [DataMember]
        public PaymentAcquirerData PaymentAcquirerData { get; set; }
    }
}
