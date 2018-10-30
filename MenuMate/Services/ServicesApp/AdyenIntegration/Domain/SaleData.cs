using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SaleData
    {
        //[DataMember]
        //public string OperatorId { get; set; }
        //[DataMember]
        // Language
        //[DataMember]
        //public string ShiftNumber { get; set; }
        [DataMember]
        public SaleTransactionID SaleTransactionID { get; set; }
        [DataMember]
        public string SaleReferenceID { get; set; }
        //[DataMember]
        //public SaleTerminalData SaleTerminalData { get; set; }
        //[DataMember]
        //public string TokenRequestedType { get; set; }
        //[DataMember]
        //public string CustomerOrderID { get; set; }
        //[DataMember]
        // cluster
        //[DataMember]
        //public string SaleToPOIData { get; set; }
        [DataMember]
        public string SaleToAcquirerData { get; set; }
        //[DataMember]
        //public SaleToIssuerData SaleToIssuerData { get; set; }
    }
}
