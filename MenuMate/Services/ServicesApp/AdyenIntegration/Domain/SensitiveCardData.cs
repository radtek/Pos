using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class SensitiveCardData
    {
        [DataMember]
        public string PAN { get; set; }
        [DataMember]
        public string CardSeqNumb { get; set; }
        [DataMember]
        public string ExpiryDate { get; set; }
        [DataMember]
        public TrackData TrackData { get; set; }
    }
}
