using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateOnlineOrdering
{
    [DataContract]
    public class OnlineOrderingDetails
    {
        [DataMember]
        public bool IsMemberOrderingEnabled { get; set; }
        [DataMember]
        public bool IsWaiterOrderingEnabled { get; set; }
        [DataMember]
        public bool IsSuccessful { get; set; }
        [DataMember]
        public string ResponseText { get; set; } 
    }
}
