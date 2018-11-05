using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.AdjustAuthorisation
{
    [DataContract]
    public class AdditionalData
    {
        [DataMember]
        public string industryUsage { get; set; }
    }
}
