using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class Geolocation
    {
        [DataMember]
        public GeographicCoordinates GeographicCoordinates { get; set; }
        [DataMember]
        public UTMCoordinates UTMCoordinates { get; set; }
    }
}
