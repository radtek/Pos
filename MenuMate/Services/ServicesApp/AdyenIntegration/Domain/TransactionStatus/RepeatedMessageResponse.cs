using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using AdyenIntegration.Domain.Common;
using AdyenIntegration.Domain.Diagnosis;
using AdyenIntegration.Domain.LoginLogOut;

namespace AdyenIntegration.Domain.TransactionStatus
{
    [DataContract]
    public class RepeatedMessageResponse
    {
        [DataMember]
        public RepeatedResponseMessageBody RepeatedResponseMessageBody { get; set; }
        [DataMember]
        public MessageHeader MessageHeader { get; set; }
    }
}
