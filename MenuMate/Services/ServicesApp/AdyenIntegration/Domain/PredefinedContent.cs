using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class PredefinedContent
    {
        [DataMember]
        public string ReferenceID { get; set; }
        [DataMember]
        public string Language { get; set; }
    }
}
