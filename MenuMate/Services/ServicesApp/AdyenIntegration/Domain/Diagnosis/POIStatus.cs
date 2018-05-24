using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.Diagnosis
{
    [DataContract]
    public class POIStatus
    {
        [DataMember]
        public string CommunicationOKFlag { get; set; }
        [DataMember]
        public string GlobalStatus { get; set; }
    }
}
