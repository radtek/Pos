using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.AdjustAuthorisation
{
    [DataContract]
    public class ModificationAmount
    {
        [DataMember]
        public string currency { get; set; }
        [DataMember]
        public int value { get; set; }
    }
}
