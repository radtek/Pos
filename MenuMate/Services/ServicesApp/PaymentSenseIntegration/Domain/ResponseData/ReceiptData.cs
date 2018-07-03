using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace PaymentSenseIntegration.Domain
{
    [DataContract]
    public class ReceiptData
    {
        [DataMember]
        public string[] Format { get; set; }
        [DataMember]
        public string Type { get; set; }
        [DataMember]
        public string Value { get; set; }
    }
}
