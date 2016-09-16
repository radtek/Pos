using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.Xero
{
    [XmlRoot(ElementName = "Payments")]
    public class XeroInvoicePayments
    {
        [XmlElement(ElementName = "Payment")]
        public List<XeroInvoicePayment> Item { get; set; }
    }
}
