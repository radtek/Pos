using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Space
    {
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public bool IsActive { get; set; }
        [DataMember]
        public string ParentSpaceId { get; set; }
        [DataMember]
        public string CategoryId { get; set; }
        [DataMember]
        public string Type { get; set; }
        [DataMember]
        public string Number { get; set; }
        [DataMember]
        public string FloorNumber { get; set; }
        [DataMember]
        public string BuildingNumber { get; set; }
        [DataMember]
        public string State { get; set; }
    }
}
