using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Customer
    {
        [DataMember]
        public List<string> Address { get; set; }
        [DataMember]
        public DateTime BirthDate { get; set; }
        [DataMember]
        public string BirthPlace { get; set; }
        [DataMember]
        public object CategoryId { get; set; }
        [DataMember]
        public List<string> Classifications { get; set; }
        [DataMember]
        public DateTime CreatedUtc { get; set; }
        [DataMember]
        public string Email { get; set; }
        [DataMember]
        public string FirstName { get; set; }
        [DataMember]
        public string Gender { get; set; }
        [DataMember]
        public string Id { get; set; }
        //[DataMember]
        //public object IdentityCard { get; set; }
        [DataMember]
        public string LanguageCode { get; set; }
        [DataMember]
        public string LastName { get; set; }
        [DataMember]
        public string LoyaltyCode { get; set; }
        [DataMember]
        public string NationalityCode { get; set; }
        [DataMember]
        public string Notes { get; set; }
        //[DataMember]
        //public List<Option> Options { get; set; }
        [DataMember]
        public string Number { get; set; }
        //[DataMember]
        //public object Passport { get; set; }
        [DataMember]
        public string Phone { get; set; }
        [DataMember]
        public object SecondLastName { get; set; }
        [DataMember]
        public string TaxIdentificationNumber { get; set; }
        [DataMember]
        public string Title { get; set; }
        [DataMember]
        public DateTime UpdatedUtc { get; set; }
        //[DataMember]
        //public object Visa { get; set; }
    }
}
