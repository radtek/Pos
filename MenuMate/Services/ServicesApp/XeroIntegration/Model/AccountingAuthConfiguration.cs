using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace AccountingIntegration.Model
{
    [XmlRoot(ElementName = "AccountingIntegration")]
    public class AccountingAuthConfiguration
    {
        [XmlElement(ElementName = "AccountingSystem")]
        public List<AccountingSystem> AccountingSystemList { get; set; }
    }
}
