using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using SmartConnectIntegration.Enums;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class TransactionTypes : POSDetails
    {
        [DataMember]
        public string Transactiontype { get; set; }    
    }
}
