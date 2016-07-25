using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace XeroIntegration.Model
{
    [XmlRoot(ElementName = "OAuth")]
    public class OAuth
    {
        [XmlElement(ElementName = "CredentialsList")]
        public CredentialsList CredentialsList { get; set; }
    }
}
