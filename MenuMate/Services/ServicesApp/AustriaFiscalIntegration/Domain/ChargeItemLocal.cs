using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace AustriaFiscalIntegration.Domain
{
    [DataContract]
    public class ChargeItemLocal
    {
        [DataMember]
        public double UnitQuantity { get; set; }
        [DataMember]
        public string Unit { get; set; }
        [DataMember]
        public string ProductBarcode { get; set; }
        [DataMember]
        public string ProductNumber { get; set; }
        [DataMember]
        public string ProductGroup { get; set; }
        [DataMember]
        public string CostCenter { get; set; }
        [DataMember]
        public string AccountNumber { get; set; }
        [DataMember]
        public double VATAmount { get; set; }
        [DataMember]
        public string ChargeItemCaseData { get; set; }
        [DataMember]
        public long ChargeItemCase { get; set; }
        [DataMember]
        public double VATRate { get; set; }
        [DataMember]
        public double Amount { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public double Quantity { get; set; }
        [DataMember]
        public long Position { get; set; }
        [DataMember]
        public double UnitPrice { get; set; }
        [DataMember]
        public DateTime Moment { get; set; }
    }
}
