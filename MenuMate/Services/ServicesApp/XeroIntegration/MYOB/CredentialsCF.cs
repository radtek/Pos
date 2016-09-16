using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "CredentialsCF")]
    public class CredentialsCF
    {
        [XmlElement(ElementName = "UserName")]
        public string UserName { get; set; }
        [XmlElement(ElementName = "Password")]
        public string Password { get; set; }
    }
}
