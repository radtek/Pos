using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AustriaFiscalIntegration.Domain
{
    [DataContract]
    public class ReceiptReponseLocal
    {
        [DataMember]
        public string[] ReceiptFooter { get; set; }
        [DataMember]
        public SignaturItemLocal[] Signatures { get; set; }
        [DataMember]
        public string[] PayLines { get; set; }
        [DataMember]
        public PayItemLocal[] PayItems { get; set; }
        [DataMember]
        public string[] ChargeLines { get; set; }
        [DataMember]
        public ChargeItemLocal[] ChargeItems { get; set; }
        [DataMember]
        public string[] ReceiptHeader { get; set; }
        [DataMember]
        public long State { get; set; }
        [DataMember]
        public DateTime ReceiptMoment { get; set; }
        [DataMember]
        public string CashBoxIdentification { get; set; }
        [DataMember]
        public string ReceiptReference { get; set; }
        [DataMember]
        public string TerminalID { get; set; }
        [DataMember]
        public long QueueRow { get; set; }
        [DataMember]
        public string QueueItemID { get; set; }
        [DataMember]
        public string QueueID { get; set; }
        [DataMember]
        public string CashBoxID { get; set; }
        [DataMember]
        public string ReceiptIdentification { get; set; }
        [DataMember]
        public string StateData { get; set; }
        [DataMember]
        public bool UnableToConnectToFiscalService { get; set; }
    }
}
