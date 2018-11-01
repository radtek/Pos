using System.Collections.Generic;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class RootObject
    {
        [DataMember]
        public List<AccountingCategory> AccountingCategories { get; set; }
        [DataMember]
        public List<Outlet> Outlets { get; set; }
        [DataMember]
        public List<Service> Services { get; set; }
        [DataMember]
        public List<Customer> Customers { get; set; }
        [DataMember]
        public List<Space> Spaces { get; set; }
    }
}
