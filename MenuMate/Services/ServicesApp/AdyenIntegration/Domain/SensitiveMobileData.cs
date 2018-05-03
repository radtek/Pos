using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SensitiveMobileData
    {
        [DataMember]
        public string MSISDN { get; set; }
        [DataMember]
        public string ISMI { get; set; }
        [DataMember]
        public string IMEI { get; set; }
    }
}
