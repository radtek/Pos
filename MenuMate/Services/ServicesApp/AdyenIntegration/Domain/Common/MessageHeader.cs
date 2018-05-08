using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.Common
{
    
    //[DataContract]
    public class MessageHeader
    {
        [DataMember]
        public string ProtocolVersion { get; set; }
        [DataMember]
        public string MessageClass { get; set; }
        [DataMember]
        public string MessageCategory { get; set; }
        [DataMember]
        public string MessageType { get; set; }
        [DataMember]
        public string ServiceID { get; set; }
        [DataMember]
        public string POIID { get; set; }
        [DataMember]
        public string SaleID { get; set; }
    }
}
