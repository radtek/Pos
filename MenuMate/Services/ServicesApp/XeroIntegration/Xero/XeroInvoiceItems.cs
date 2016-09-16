using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.Xero
{
    [XmlRoot(ElementName = "Items")]
    public class XeroInvoiceItems
    {
        [XmlElement(ElementName = "Item")]
        public List<XeroInvoiceItem> Item { get; set; }
    }
}
