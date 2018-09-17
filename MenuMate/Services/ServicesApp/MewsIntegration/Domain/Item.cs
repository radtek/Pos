using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Item
    {
        [DataMember]
        public string Type { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public int UnitCount { get; set; }
        [DataMember]
        public UnitCost UnitCost { get; set; }
        [DataMember]
        public DateTime ConsumedUtc { get; set; }
        [DataMember]
        public Category Category { get; set; }
    }
}
