using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain;
using System.Runtime.Serialization;


namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SaleSoftware
    {
        [DataMember]
        public string ManufacturerID { get; set; }
        [DataMember]
        public string ApplicationName { get; set; }
        [DataMember]
        public string SoftwareVersion { get; set; }
        [DataMember]
        public string CertificationCode { get; set; }
    }
}
