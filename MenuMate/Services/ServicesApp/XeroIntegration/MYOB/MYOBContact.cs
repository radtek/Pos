using System.Xml.Serialization;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "Contact")]
    public class MYOBContact
    {
        [XmlAttribute(AttributeName = "Name")]
        public string Name { get; set; }
    }
}
