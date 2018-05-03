using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.Diagnosis
{
    [DataContract]
    public class DiagnosisResponse
    {
        [DataMember]
        public POIStatus POIStatus { get; set; }
        [DataMember]
        public Response Response { get; set; }
    }
}
