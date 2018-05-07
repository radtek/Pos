using System;
using System.Runtime.Serialization;
using AdyenIntegration.Domain;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentResponse
    {
        [DataMember]
        public PaymentReceipt[] PaymentReceipt { get; set; }
        [DataMember]
        public POIData POIData { get; set; }
        [DataMember]
        public PaymentResult PaymentResult { get; set; }
        [DataMember]
        public SaleData SaleData { get; set; }
        [DataMember]
        public Response Response { get; set; }
        [DataMember]
        public string[] PaymentReceiptUsable { get; set; }
    }
}
