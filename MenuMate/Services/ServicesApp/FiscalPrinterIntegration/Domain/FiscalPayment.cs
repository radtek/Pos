using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FiscalPrinterIntegration.Domain
{
    [DataContract]
    public class FiscalPayment
    {
        [DataMember]
        public string Type { get; set; }
        [DataMember]
        public string Amount { get; set; }
        [DataMember]
        public string Billno { get; set; }
        [DataMember]
        public string Cashno { get; set; }
        [DataMember]
        public string Cashier { get; set; }
        [DataMember]
        public string Source { get; set; }
        [DataMember]
        public string Description { get; set; }
    }
}
