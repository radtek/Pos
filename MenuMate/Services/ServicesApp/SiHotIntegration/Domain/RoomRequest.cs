using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class RoomRequest
    {
        [DataMember]
        public string TransactionNumber { get; set; }
        [DataMember]
        public string RoomNumber { get; set; }
        [DataMember]
        public string IPAddress { get; set; }
        [DataMember]
        public int PortNumber { get; set; }
    }
}
