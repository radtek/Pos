using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SiHotIntegration.Domain
{
    [DataContract]
    public class RoomDetails
    {
        [DataMember]
        public string TransNo { get; set; }
        [DataMember]
        public List<GuestDetails> GuestDetailsList { get; set; }
        [DataMember]
        public bool IsSuccessful { get; set; }
        [DataMember]
        public string ResponseMessage { get; set; }
    }
}
