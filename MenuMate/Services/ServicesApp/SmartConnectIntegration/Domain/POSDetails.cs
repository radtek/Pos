using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class POSDetails
    {
        [DataMember]
        public string PosRegisterId { get; set; }
        [DataMember]
        public string PosBusinessName { get; set; }
        [DataMember]
        public string PosVendorName { get; set; }
        [DataMember]
        public string IpAddress { get; set; }
    }
}
