using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class BasicInquiry
    {
        [DataMember]
        public string ClientToken { get; set; }
        [DataMember]
        public string AccessToken { get; set; }
        [DataMember]
        public Extent Extent { get; set; }
    }
}
