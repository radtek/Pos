using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateOnlineOrdering
{
    [DataContract]
    public class LoyaltyOOResponse
    {
        [DataMember]
        public bool IsSuccessful { get; set; }
        [DataMember]
        public string ResponseText { get; set; }
    }
}
