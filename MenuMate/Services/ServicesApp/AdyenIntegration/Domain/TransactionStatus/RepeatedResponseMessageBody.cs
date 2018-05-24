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
    public class RepeatedResponseMessageBody
    {
        [DataMember]
        public PaymentResponse PaymentResponse { get; set; }
    }
}
