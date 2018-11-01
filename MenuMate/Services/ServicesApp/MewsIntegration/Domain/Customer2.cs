﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Customer2
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
        //public DateTime BirthDate { get; set; }
        //public DateTime BirthDateUtc { get; set; }
        //public string BirthPlace { get; set; }
        //public object Occupation { get; set; }
        //public string Email { get; set; }
        //public string Phone { get; set; }
        //public DateTime CreatedUtc { get; set; }
        //public DateTime UpdatedUtc { get; set; }
    }
}