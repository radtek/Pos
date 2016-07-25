using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace XeroIntegration.Model
{
    [XmlRoot(ElementName = "XeroIntegration")]
    public class XeroAuthConfiguration
    {
        [XmlElement(ElementName = "OAuth")]
        public OAuth OAuth { get; set; }
    }
}
