using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using SmartConnectIntegration.Enums;

namespace SmartConnectIntegration.Domain
{
    [DataContract]
    public class SmartConnectDataObject
    {
        [DataMember]
        public string TransactionResult { get; set; }
        [DataMember]
        public string  Receipt { get; set; }
        [DataMember]
        public string RequestId { get; set; }
        [DataMember]
        public string AcquirerRef { get; set; }
        [DataMember]
        public string AccountType { get; set; }
        [DataMember]
        public DateTime Timestamp { get; set; }
        [DataMember]
        public string Result { get; set; }
        [DataMember]
        public string Function { get; set; }
        [DataMember]
        public string AuthId { get; set; }
        [DataMember]
        public string CardPan { get; set; }
        [DataMember]
        public string AmountTotal { get; set; }
        [DataMember]
        public string Merchant { get; set; }
        [DataMember]
        public string CardType { get; set; }
        [DataMember]
        public string TerminalRef { get; set; }
        [DataMember]
        public string AmountSurcharge { get; set; }
        [DataMember]
        public string AmountTip { get; set; }
    }
}
