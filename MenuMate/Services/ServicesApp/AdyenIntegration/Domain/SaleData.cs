using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SaleData
    {
        [DataMember]
        public SaleTransactionID SaleTransactionID { get; set; }
        [DataMember]
        public string SaleReferenceID { get; set; }
        [DataMember]
        public string TokenRequestedType { get; set; }
        [DataMember]
        public string SaleToAcquirerData { get; set; }
    }
}
