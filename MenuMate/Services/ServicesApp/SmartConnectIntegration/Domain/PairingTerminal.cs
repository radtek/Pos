﻿using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class PairingTerminal : POSDetails
    {
        [DataMember]
        public string PosRegisterName { get; set; }
        //How the pairing code will be fetched.
    }
}
