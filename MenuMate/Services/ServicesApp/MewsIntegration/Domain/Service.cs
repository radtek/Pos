using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Service
    {
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public bool IsActive { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string StartTime { get; set; }
        [DataMember]
        public string EndTime { get; set; }
        [DataMember]
        public Promotions Promotions { get; set; }
    }
}
