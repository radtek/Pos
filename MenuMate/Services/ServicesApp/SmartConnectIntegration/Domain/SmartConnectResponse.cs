using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using SmartConnectIntegration.Enums;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class SmartConnectResponse
    {
        [DataMember]
        public bool ResponseSuccessful { get; set; }
        [DataMember]
        public string ResponseMessage { get; set; }
        [DataMember]
        public string TransactionId { get; set; }
        [DataMember]
        public DateTimeOffset TransactionTime { get; set; }
        [DataMember]
        public string MerchantId { get; set; }
        [DataMember]
        public string DeviceId { get; set; }
        [DataMember]
        public string TransactionStatus { get; set; }
        [DataMember]
        public SmartConnectDataObject SmartConnectData { get; set; }
    }
}
