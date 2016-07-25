using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace XeroIntegration.Model
{
    [XmlRoot(ElementName = "CredentialsList")]
    public class CredentialsList
    {
        [XmlElement(ElementName = "Credentials")]
        public List<Credentials> Credentials { get; set; }
        [XmlAttribute(AttributeName = "appInUse")]
        public string AppInUse { get; set; }
    }
}
