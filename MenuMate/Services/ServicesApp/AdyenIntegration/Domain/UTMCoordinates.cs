using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class UTMCoordinates
    {
        [DataMember]
        public string UTMZone { get; set; }
        [DataMember]
        public string UTMEastward { get; set; }
        [DataMember]
        public string UTMNorthward { get; set; }
    }
}
