using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.Model
{
    [XmlRoot(ElementName = "Organisations")]
    public class Organisations
    {
        [XmlAttribute(AttributeName = "name")]
        public string name { get; set; }
    }
}
