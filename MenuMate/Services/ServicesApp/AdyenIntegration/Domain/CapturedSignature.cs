using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class CapturedSignature
    {
        [DataMember]
        public AreaSize AreaSize { get; set; }
        [DataMember]
        public string SignaturePoint { get; set; }
    }
}
