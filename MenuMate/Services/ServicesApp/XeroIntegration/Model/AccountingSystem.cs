using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.Model
{
    [XmlRoot(ElementName = "AccountingSystem")]
    public class AccountingSystem
    {
        [XmlElement(ElementName = "CredentialsList")]
        public CredentialsList CredentialsList { get; set; }
        [XmlAttribute(AttributeName = "type")]
        public string Type { get; set; }
    }
}
