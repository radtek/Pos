using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class MobileData
    {
        [DataMember]
        public string MobileCountryCode { get; set; }
        [DataMember]
        public string MobileNetworkCode { get; set; }
        [DataMember]
        public string MaskedMSISDN { get; set; }
        [DataMember]
        public Geolocation Geolocation { get; set; }
        [DataMember]
        public string ProtectedMobileData { get; set; }
        [DataMember]
        public SensitiveMobileData SensitiveMobileData { get; set; }
    }
}
