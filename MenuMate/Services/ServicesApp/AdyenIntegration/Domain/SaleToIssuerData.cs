using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SaleToIssuerData
    {
        [DataMember]
        public string StatementReference { get; set; }
    }
}
