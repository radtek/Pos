using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AustriaFiscalIntegration.Domain
{
    [DataContract]
    public class PayItemLocal
    {
        [DataMember]
        public long Position { get; set; }
        [DataMember]
        public double Quantity { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public double Amount { get; set; }
        [DataMember]
        public long PayItemCase { get; set; }
        [DataMember]
        public string PayItemCaseData { get; set; }
        [DataMember]
        public string AccountNumber { get; set; }
        [DataMember]
        public string CostCenter { get; set; }
        [DataMember]
        public string MoneyGroup { get; set; }
        [DataMember]
        public string MoneyNumber { get; set; }
        [DataMember]
        public DateTime Moment { get; set; }        
    }
}
