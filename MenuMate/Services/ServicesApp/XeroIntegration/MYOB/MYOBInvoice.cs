using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "MYOBInvoice")]
    public class MYOBInvoice
    {
        [XmlElement(ElementName = "Contact")]
        public MYOBContact Contact { get; set; }
        [XmlElement(ElementName = "Items")]
        public Items Items { get; set; }
        [XmlAttribute(AttributeName = "Status")]
        public string Status { get; set; }
        [XmlAttribute(AttributeName = "Number")]
        public string Number { get; set; }

        [XmlAttribute(AttributeName = "Date")]
        public string Date { get; set; }
        [XmlAttribute(AttributeName = "Jobcode")]
        public string Jobcode { get; set; }
        [XmlAttribute(AttributeName = "CashGLCode")]
        public string CashGLCode { get; set; }

    }
}
