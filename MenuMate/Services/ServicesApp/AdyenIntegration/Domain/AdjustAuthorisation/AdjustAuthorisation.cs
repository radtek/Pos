using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.AdjustAuthorisation
{
    [DataContract]
    public class AdjustAuthorisation
    {
        [DataMember]
        public string merchantAccount { get; set; }
        [DataMember]
        public ModificationAmount modificationAmount { get; set; }
        [DataMember]
        public string originalReference { get; set; }
        [DataMember]
        public string reference { get; set; }
        [DataMember]
        public AdditionalData additionalData { get; set; }
    }
}
