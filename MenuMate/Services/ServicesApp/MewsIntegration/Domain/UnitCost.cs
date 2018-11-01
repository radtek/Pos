using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MewsIntegration.Domain
{
    [DataContract]
    public class UnitCost
    {
        [DataMember]
        public double Amount { get; set; }
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public double Tax { get; set; }
    }
}
