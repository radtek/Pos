using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.Xero
{
    [XmlRoot(ElementName = "Contact")]
    public class XeroContact
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}
