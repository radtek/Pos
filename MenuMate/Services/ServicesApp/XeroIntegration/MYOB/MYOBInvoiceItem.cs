using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "Item")]
    public class Item
    {
        [XmlAttribute(AttributeName = "GLCode")]
        public string GLCode { get; set; }
        [XmlAttribute(AttributeName = "Description")]
        public string Description { get; set; }
        [XmlAttribute(AttributeName = "UnitAmount")]
        public string UnitAmount { get; set; }
        [XmlAttribute(AttributeName = "TaxName")]
        public string TaxStatus { get; set; }
        [XmlAttribute(AttributeName = "TaxAmount")]
        public string TaxAmount { get; set; }
    }
}
