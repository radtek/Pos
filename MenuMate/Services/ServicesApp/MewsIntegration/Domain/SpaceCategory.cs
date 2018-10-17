using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class SpaceCategory
    {
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public bool IsActive { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string ShortName { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public int Ordering { get; set; }
        [DataMember]
        public int UnitCount { get; set; }
        [DataMember]
        public int ExtraUnitCount { get; set; }
    }
}
