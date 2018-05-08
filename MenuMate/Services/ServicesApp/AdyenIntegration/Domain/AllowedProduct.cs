using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class AllowedProduct
    {
        [DataMember]
        public string ProductCode { get; set; }
        [DataMember]
        public string EanUpc { get; set; }
        [DataMember]
        public string ProductLabel { get; set; }
        [DataMember]
        public string AdditionalProductInfo { get; set; }
    }
}
