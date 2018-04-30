using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class AreaSize
    {
        [DataMember]
        public string X { get; set; }
        [DataMember]
        public string Y { get; set; }
    }
}
