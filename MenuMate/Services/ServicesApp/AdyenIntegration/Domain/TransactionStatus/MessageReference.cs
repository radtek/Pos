using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain.TransactionStatus
{
    [DataContract]
    public class MessageReference
    {
        [DataMember]
        public string MessageCategory { get; set; }
        [DataMember]
        public string SaleID { get; set; }
        [DataMember]
        public string ServiceID { get; set; }
    }
}
