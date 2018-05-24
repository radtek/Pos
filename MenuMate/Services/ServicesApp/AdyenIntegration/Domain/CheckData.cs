using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class CheckData
    {
        [DataMember]
        public string BankID { get; set; }
        [DataMember]
        public string AccountNumber { get; set; }
        [DataMember]
        public string CheckNumber { get; set; }
        [DataMember]
        public TrackData TrackData { get; set; }
        [DataMember]
        public string CheckCardNumber { get; set; }
        [DataMember]
        public string Country { get; set; }
    }
}
