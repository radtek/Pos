using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "Items")]
    public class Items
    {
        [XmlElement(ElementName = "Item")]
        public List<Item> Item { get; set; }
    }
}
