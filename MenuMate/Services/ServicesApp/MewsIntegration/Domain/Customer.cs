using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using Newtonsoft.Json;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Customer
    {
        [JsonProperty(PropertyName = "Customer")]
        [DataMember]
        public CustomerDetails CustomerDetails { get; set; }
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public string FirstName { get; set; }
        [DataMember]
        public string LastName { get; set; }
        [DataMember]
        public string RoomNumber { get; set; }
    }
}
