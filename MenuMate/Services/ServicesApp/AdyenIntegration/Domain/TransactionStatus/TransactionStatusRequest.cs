using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AdyenIntegration.Domain;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.TransactionStatus
{
    [DataContract]
    public class TransactionStatusRequest
    {
        [DataMember]
        public string[] DocumentQualifier { get; set; }
        [DataMember]
        public bool ReceiptReprintFlag { get; set; }
        [DataMember]
        public MessageReference MessageReference { get; set; }
    }
}
