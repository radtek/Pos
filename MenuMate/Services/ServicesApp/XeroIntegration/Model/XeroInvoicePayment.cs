using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace XeroIntegration.Model
{
    [XmlRoot(ElementName = "Payment")]
    public class XeroInvoicePayment
    {
        [XmlAttribute(AttributeName = "accountCode")]
        public string AccountCode { get; set; }
        [XmlAttribute(AttributeName = "description")]
        public string Description { get; set; }
        [XmlAttribute(AttributeName = "unitAmount")]
        public string UnitAmount { get; set; }
        [XmlAttribute(AttributeName = "taxAmount")]
        public string TaxAmount { get; set; }
       
    }
}
