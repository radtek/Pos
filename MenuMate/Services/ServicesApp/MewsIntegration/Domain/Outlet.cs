using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Outlet
    {
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public bool IsActive { get; set; }
        [DataMember]
        public string Name { get; set; }
    }
}
