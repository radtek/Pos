using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using SmartConnectIntegration.Enums;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    class TransactionTypes : POSDetails
    {
        [DataMember]
        public int Transactiontype { get; set; }    
    }
}
