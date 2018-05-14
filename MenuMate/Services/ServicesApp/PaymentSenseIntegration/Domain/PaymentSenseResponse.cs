using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class PaymentSenseResponse
    {
        [DataMember]
        public string RequestId { get; set; }
        [DataMember]
        public string Location { get; set; }
    }
}
