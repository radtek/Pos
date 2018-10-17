using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class Promotions
    {
        [DataMember]
        public bool BeforeCheckIn { get; set; }
        [DataMember]
        public bool AfterCheckIn { get; set; }
        [DataMember]
        public bool DuringStay { get; set; }
        [DataMember]
        public bool BeforeCheckOut { get; set; }
        [DataMember]
        public bool AfterCheckOut { get; set; }
    }
}
