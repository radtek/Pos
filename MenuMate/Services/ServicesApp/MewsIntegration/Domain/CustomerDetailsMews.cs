using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class CustomerDetailsMews
    {
        [DataMember]
        public string Id { get; set; }
        [DataMember]
        public string Number { get; set; }
        [DataMember]
        public string Title { get; set; }
        [DataMember]
        public string Gender { get; set; }
        [DataMember]
        public string FirstName { get; set; }
        [DataMember]
        public string LastName { get; set; }
        [DataMember]
        public List<string> Classifications { get; set; }
        [DataMember]
        public string Email { get; set; }
        [DataMember]
        public string Phone { get; set; }
    }
}
