using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Order
    {
        [DataMember]
        public string ClientToken { get; set; }
        [DataMember]
        public string AccessToken { get; set; }
        [DataMember]
        public string CustomerId { get; set; }
        [DataMember]
        public string ServiceId { get; set; }
        [DataMember]
        public string ConsumptionUtc { get; set; }
        [DataMember]
        public List<Bill> Bills { get; set; }
        [DataMember]
        public List<Item> Items { get; set; }
        [DataMember]
        public string Notes { get; set; }
    }
}
