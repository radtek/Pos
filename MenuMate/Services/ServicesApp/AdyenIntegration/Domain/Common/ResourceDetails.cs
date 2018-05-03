using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.Common
{
    [DataContract]
    public class ResourceDetails
    {
        [DataMember]
        public string APIKey { get; set; }
        [DataMember]
        public string URL { get; set; }
    }
}
