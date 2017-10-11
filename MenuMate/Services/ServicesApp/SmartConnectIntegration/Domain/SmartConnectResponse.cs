using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class SmartConnectResponse
    {
        [DataMember]
        public string TransactionId { get; set; }
        [DataMember]
        public DateTimeOffset TransactionTime { get; set; }
        [DataMember]
        public string MerchantId { get; set; }
        [DataMember]
        public string DeviceId { get; set; }
        [DataMember]
        public TransactionStatus TransactionStatus { get; set; }
        [DataMember]
        public SmartConnectDataObject SmartConnectData { get; set; }
    }
}
