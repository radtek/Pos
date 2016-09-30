using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class RequestInfo
    {
        [DataMember]
        public string RequestKey { get; set; }
        [DataMember]
        public long SiteCode { get; set; }
        [DataMember]
        public DateTime RequestTime { get; set; }
    }
}
