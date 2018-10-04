using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyOnlineOrderingResponse
    {
        [DataMember]
        public bool IsSuccessful { get; set; }
        [DataMember]
        public string ResponseText { get; set; }
    }
}
