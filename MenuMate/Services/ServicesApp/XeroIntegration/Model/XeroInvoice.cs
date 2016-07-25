using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace XeroIntegration.Model
{
    [XmlRoot(ElementName = "XeroInvoice")]
    public class XeroInvoice
    {
        [XmlElement(ElementName = "Contact")]
        public XeroContact Contact { get; set; }
        [XmlElement(ElementName = "Items")]
        public XeroInvoiceItems Items { get; set; }
        [XmlAttribute(AttributeName = "type")]
        public string Type { get; set; }
        [XmlAttribute(AttributeName = "status")]
        public string Status { get; set; }
        [XmlAttribute(AttributeName = "number")]
        public string Number { get; set; }
        [XmlAttribute(AttributeName = "reference")]
        public string Reference { get; set; }
        [XmlAttribute(AttributeName = "lineAmountType")]
        public string LineAmountType { get; set; }
        [XmlAttribute(AttributeName = "date")]
        public string Date { get; set; }
        [XmlAttribute(AttributeName = "dueDate")]
        public string DueDate { get; set; }
        [XmlElement(ElementName = "Payments")]
        public XeroInvoicePayments Payments { get; set; }
      

    }
}
