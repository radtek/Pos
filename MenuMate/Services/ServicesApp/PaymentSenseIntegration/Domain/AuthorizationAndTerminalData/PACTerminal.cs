using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class PACTerminal
    {
        [DataMember]
        public string TPI { get; set; }//TPI
        [DataMember]
        public string[] Capabilities { get; set; } //will store capabilities like sale, refund, reports etc.
        [DataMember]
        public string Currency { get; set; }
        [DataMember]
        public string Status { get; set; }
        [DataMember]
        public string Location { get; set; }
    }
}
