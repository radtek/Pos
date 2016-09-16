using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "CompanyFileCredentialsCF")]
    public class CompanyFileCredentialsCF
    {
        [XmlElement(ElementName = "CredentialsCF")]
        public CredentialsCF CredentialsDetails { get; set; }
    }
}
