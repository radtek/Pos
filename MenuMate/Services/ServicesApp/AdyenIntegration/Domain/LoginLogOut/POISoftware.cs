using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.LoginLogOut
{
    [DataContract]
    public class POISoftware
    {
        [DataMember]
        public string CertificationCode { get; set; }
        [DataMember]
        public string SoftwareVersion { get; set; }
        [DataMember]
        public string ManufacturerID { get; set; }
        [DataMember]
        public string ApplicationName { get; set; }
    }
}
    