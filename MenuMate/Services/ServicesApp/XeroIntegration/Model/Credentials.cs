using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace XeroIntegration.Model
{
    [XmlRoot(ElementName = "Credentials")]
    public class Credentials
    {
        [XmlAttribute(AttributeName = "appID")]
        public string AppID { get; set; }
        [XmlAttribute(AttributeName = "organisation")]
        public string Organisation { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string UserAgentString { get; set; }
        [XmlAttribute(AttributeName = "appType")]
        public string AppType { get; set; }
        [XmlAttribute(AttributeName = "consumerKey")]
        public string ConsumerKey { get; set; }
        [XmlAttribute(AttributeName = "consumerSecret")]
        public string ConsumerSecret { get; set; }
        [XmlAttribute(AttributeName = "pfxFileName")]
        public string PfxFileName { get; set; }
        [XmlAttribute(AttributeName = "password")]
        public string PfxPassword { get; set; }
        [XmlAttribute(AttributeName = "APIEndpointURL")]
        public string APIEndpointURL { get; set; }
    }
}
