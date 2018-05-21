using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class PostRequestResponse
    {
        [DataMember]
        public string RequestId { get; set; }
        [DataMember]
        public string Location { get; set; }
    }
}
