using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "Contact")]
    public class MYOBContact
    {
        [XmlAttribute(AttributeName = "Name")]
        public string Name { get; set; }
    }
}
