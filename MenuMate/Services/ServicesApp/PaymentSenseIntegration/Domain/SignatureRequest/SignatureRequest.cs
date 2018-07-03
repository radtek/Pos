using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain.SignatureRequest
{
    [DataContract]
    public class SignatureRequest
    {
        [DataMember]
        public string accepted { get; set; }
    }
}
