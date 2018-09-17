using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class CustomerSearch
    {
        [DataMember]
        public string ClientToken { get; set; }
        [DataMember]
        public string AccessToken { get; set; }
        [DataMember]
        public string FirstName { get; set; }
        [DataMember]
        public string LastName { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string SpaceId { get; set; }
    }
}
