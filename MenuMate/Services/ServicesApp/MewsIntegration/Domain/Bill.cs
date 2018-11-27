using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Bill
    {
        [DataMember]
        public string OutletId { get; set; }
        [DataMember]
        public string Number { get; set; }
        [DataMember]
        public string ClosedUtc { get; set; }
        [DataMember]
        public List<Item> Items { get; set; }

    }
}
