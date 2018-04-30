using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class PairingTerminal : POSDetails
    {
        [DataMember]
        public string PosRegisterName { get; set; }
        [DataMember]
        public string PairingCode { get; set; }
    }
}
