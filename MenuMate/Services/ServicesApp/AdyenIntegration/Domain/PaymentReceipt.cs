using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentReceipt
    {
        [DataMember]
        public string DocumentQualifier { get; set; }
        [DataMember]
        public string IntegratedPrintFlag { get; set; }
        [DataMember]
        public string RequiredSignatureFlag { get; set; }
        [DataMember]
        public OutputContent OutputContent { get; set; }
    }
}
