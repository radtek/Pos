using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using AdyenIntegration.Domain;
using AdyenIntegration.Domain.Diagnosis;

namespace AdyenIntegration.Domain.LoginLogOut
{
    [DataContract]
    public class POISystemData
    {
        [DataMember]
        public POISoftware POISoftware { get; set; }
        [DataMember]
        public string DateTime { get; set; }
        [DataMember]
        public POIStatus MyProperty { get; set; }
    }
}
