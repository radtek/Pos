using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace FiscalPrinterIntegration.Domain
{
    [DataContract]
    public class DiscountDetails
    {
        [DataMember]
        public string Type { get; set; }
        [DataMember]
        public string Amount { get; set; }
        [DataMember]
        public string Description { get; set; }
        [DataMember]
        public string DiscountGroup { get; set;}
        [DataMember]
        public string DiscountMode { get; set; }
    }
}
