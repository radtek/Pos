using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PaymentAcquirerData
    {
        [DataMember]
        public string AcquirerID { get; set; }
        [DataMember]
        public string MerchantID { get; set; }
        [DataMember]
        public string AcquirerPOIID { get; set; }
        [DataMember]
        public AcquirerTransactionID AcquirerTransactionID { get; set; }
        [DataMember]
        public string ApprovalCode { get; set; }
        [DataMember]
        public string HostReconciliationID { get; set; }
    }
}
