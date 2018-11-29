using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AustriaFiscalIntegration.Domain
{
   [DataContract]
    public class ReceiptRequestLocal
    {
        [DataMember]
        public string Customer { get; set; }
        [DataMember]
        public string Area { get; set; }
        [DataMember]
        public string User { get; set; }
        [DataMember]
        public double? ReceiptAmount { get; set; }
        [DataMember]
        public string ftReceiptCaseData { get; set; }
        [DataMember]
        public long ftReceiptCase { get; set; }
        [DataMember]
        public string Settlement { get; set; }
        [DataMember]
        public PayItemLocal[] PayItems { get; set; }
        [DataMember]
        public DateTime ReceiptMoment { get; set; }
        [DataMember]
        public string ReceiptReference { get; set; }
        [DataMember]
        public string TerminalID { get; set; }
        [DataMember]
        public string ftPosSystemId { get; set; }
        [DataMember]
        public string ftQueueID { get; set; }
        [DataMember]
        public string ftCashBoxID { get; set; }
        [DataMember]
        public ChargeItemLocal[] ChargeItems { get; set; }
        [DataMember]
        public string PreviousReceiptReference { get; set; }
    }
}
