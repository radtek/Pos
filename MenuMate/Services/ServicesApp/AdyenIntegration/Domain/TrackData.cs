using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class TrackData
    {
        [DataMember]
        public int TrackNumb { get; set; }
        [DataMember]
        public string TrackFormat { get; set; }
        [DataMember]
        public string TrackValue { get; set; }
    }
}
