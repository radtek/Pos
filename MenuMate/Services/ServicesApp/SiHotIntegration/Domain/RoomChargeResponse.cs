using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class RoomChargeResponse
    {
        [DataMember]
        public string Response { get; set; }
    }
}
