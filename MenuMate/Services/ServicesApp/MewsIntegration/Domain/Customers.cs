using System.Collections.Generic;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Customers
    {
        [DataMember]
        public List<Customer> CustomersList { get; set; }
    }
}
